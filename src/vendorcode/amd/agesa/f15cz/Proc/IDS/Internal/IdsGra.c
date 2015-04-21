/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Integrated Debug library Routines
 *
 * Contains AMD AGESA debug macros and library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 315091 $   @e \$Date: 2015-03-19 17:23:58 +0800 (Thu, 19 Mar 2015) $
 */
/*****************************************************************************
 * Copyright 2008 - 2015 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 *
 * AMD is granting you permission to use this software (the Materials)
 * pursuant to the terms and conditions of your Software License Agreement
 * with AMD.  This header does *NOT* give you permission to use the Materials
 * or any rights under AMD's intellectual property.  Your use of any portion
 * of these Materials shall constitute your acceptance of those terms and
 * conditions.  If you do not agree to the terms and conditions of the Software
 * License Agreement, please do not use any portion of these Materials.
 *
 * CONFIDENTIALITY:  The Materials and all other information, identified as
 * confidential and provided to you by AMD shall be kept confidential in
 * accordance with the terms and conditions of the Software License Agreement.
 *
 * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
 * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
 * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
 * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
 * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
 * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
 * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
 * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
 * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
 *
 * AMD does not assume any responsibility for any errors which may appear in
 * the Materials or any other related information provided to you by AMD, or
 * result from use of the Materials or any related information.
 *
 * You agree that you will not reverse engineer or decompile the Materials.
 *
 * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 * further information, software, technical information, know-how, or show-how
 * available to you.  Additionally, AMD retains the right to modify the
 * Materials at any time, without notice, and is not obligated to provide such
 * modified Materials to you.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
 * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
 * subject to the restrictions as set forth in FAR 52.227-14 and
 * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
 * Government constitutes acknowledgement of AMD's proprietary rights in them.
 *
 * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
 * direct product thereof will be exported directly or indirectly, into any
 * country prohibited by the United States Export Administration Act and the
 * regulations thereunder, without the required authorization from the U.S.
 * government nor will be used for any purpose prohibited by the same.
 ******************************************************************************
 */
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "heapManager.h"
#include "Ids.h"
#include "IdsLib.h"
#include "IdsInternalLib.h"
#include "IdsBvm.h"
#include "IdsGra.h"
#define FILECODE PROC_IDS_INTERNAL_IDSGRA_FILECODE

/**
 *
 *  Initialize Ids GRA table
 *
 *  This function generates IDS GRA using BVM format in HEAP
 *
 *  @param[in,out]  IdsGraHeapBase    Pointer of GraTable in Heap
 *  @param[in,out]  StdHeader     AGESA Header Pointer

 *  @retval TRUE   Initialize GRA Table Successfully
 *
 **/
BOOLEAN
IdsGetGraBinaryInfo (
  IN OUT   UINT64 *IdsGraHeapBase,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN Status;
  UINT32 num;
  UINT32 i;
  UINT32 j;
  UINT32 type;
  UINT32 value1;
  UINT32 value2;
  UINT32 MsrAllCore;
  UINT8 Tmp[512];
  UINT32 HeapSize;
  UINT8 *PIdsGraTmp;
  IDS_NV_ITEM *NvTablePtr;
  BVM_IMAGE_HEADER *PBvmHeader;
  BVM_REG_PCI *PBvmRegPci;
  BVM_REG_CPU_NB *PBvmRegCpuNb;
  BVM_REG_MSR *PBvmRegMsr;
  BVM_REG_MEM *PBvmRegMmio;
  BVM_REG_PCI_INDIRECT *PBvmRegPciIndirect;
  BVM_REG_GMMX *PBvmRegGmmx;
  LOCATE_HEAP_PTR LocateHeapStructPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  UINT8 Sum;
  AGESA_IDS_OPTION IdsOption;
  GRA_WHEN_TO_IDS_OPTION GraWhenToIdsOptionTbl[] = \
  {
    {IdsNvMiscGRA1TimePointVery_Early___After_Reset, IDS_BEFORE_PM_INIT},
    {IdsNvMiscGRA1TimePointEarly___before_Memory_Init, IDS_INIT_POST_BEFORE},
    {IdsNvMiscGRA1TimePointLate___Before_PCI_Init, IDS_BEFORE_PCI_INIT},
    {IdsNvMiscGRA1TimePointVery_Late___Before_OS_Boot, IDS_INIT_LATE_AFTER},
  };

  IdsOption = 0xFFFF;
  Status = FALSE;

  LocateHeapStructPtr.BufferHandle = IDS_GRA_HANDLE;
  LocateHeapStructPtr.BufferPtr = NULL;
  if (AGESA_SUCCESS == HeapLocateBuffer (&LocateHeapStructPtr, StdHeader)) {
    *IdsGraHeapBase = (UINT64) LocateHeapStructPtr.BufferPtr;
    Status = TRUE;
  } else {
    if (AmdGetIdsNvTable (&NvTablePtr, StdHeader) == AGESA_SUCCESS) {
      PIdsGraTmp = &Tmp[0];
      PBvmHeader = (BVM_IMAGE_HEADER *) PIdsGraTmp;
      //Build GRA RunTime Structure Using BVM format
      PBvmHeader->Signature = 'MVB$';
      PBvmHeader->ImageTableOffset = 0x80;
      PBvmHeader->MajorVersion = BVM_CURRENT_MAJOR_VERSION;
      PBvmHeader->MinorVersion = BVM_CURRENT_MINOR_VERSION;
      PIdsGraTmp += sizeof (BVM_IMAGE_HEADER);
      num = 0;
      for (i = 0; i < IDS_MAX_GRA_ITEMS; i++) {
        if (GET_GENREG_EN (i, NvTablePtr, StdHeader) == IdsNvMiscGRAUsrCtlEnabled) {
          //
          // GenReg[i] is enabled.
          //
          type = GET_GENREG_TYPE (i, NvTablePtr, StdHeader);
          ASSERT ((type == IdsNvMiscGRARegTypePCI) ||
                  (type == IdsNvMiscGRARegTypeMSR) ||
                  (type == IdsNvMiscGRARegTypeMMIO) ||
                  (type == IdsNvMiscGRARegTypePciIndirect) ||
                  (type == IdsNvMiscGRARegTypeGmmx));
          ///Get Suitable IDS_OPTION for GRA When using route table
          for (j = 0; j < (sizeof (GraWhenToIdsOptionTbl) / sizeof (GRA_WHEN_TO_IDS_OPTION)); j++) {
            if (GraWhenToIdsOptionTbl[j].GraWhen == GET_GENREG_WHEN (i, NvTablePtr, StdHeader)) {
              IdsOption = GraWhenToIdsOptionTbl[j].IdsOption;
              break;
            }
          }
          ASSERT (IdsOption != 0xFFFF);
          // Add IDS_AFTER_S3_RESUME to support S3 Restore
          /// @todo Memory register S3 resume
          ((BVM_REG_COMMON *)PIdsGraTmp)->csttiming = IDS_TIMING_BIT (IdsOption) | \
                                                      IDS_TIMING_BIT (IDS_AFTER_S3_RESUME);
          //Add before OS HookPoint
          if (GET_GENREG_WHEN (i, NvTablePtr, StdHeader) == IDS_INIT_LATE_AFTER) {
            ((BVM_REG_COMMON *)PIdsGraTmp)->csttiming |= IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19);
          }
          switch (type) {
          case IdsNvMiscGRARegTypePCI:
            //
            // GenReg[i] is for PCI Register.
            //
            if (GET_GENREG_PCI_ALLNODES (i, NvTablePtr, StdHeader) == IdsNvMiscGRA1GenRegNodeEnter_Bus_Device) {
              PBvmRegPci = (BVM_REG_PCI *) PIdsGraTmp;
              PBvmRegPci->type = BVM_REG_TYPE_PCI;
              PBvmRegPci->bus = (UINT8) GET_GENREG_BUS (i, NvTablePtr, StdHeader);
              PBvmRegPci->dev = (UINT8) GET_GENREG_DEVICE (i, NvTablePtr, StdHeader);
              PBvmRegPci->func = (UINT8) GET_GENREG_FUNC (i, NvTablePtr, StdHeader);
              PBvmRegPci->regoff = (UINT16) GET_GENREG_OFFSET (i, NvTablePtr, StdHeader);
              value1 = GET_GENREG_AND_LOWER (i, NvTablePtr, StdHeader);
              value2 = GET_GENREG_AND_LOWEST (i, NvTablePtr, StdHeader);
              PBvmRegPci->AndMask = ((UINT32)value1 << 16) | ((UINT32)value2);
              value1 = GET_GENREG_OR_LOWER (i, NvTablePtr, StdHeader);
              value2 = GET_GENREG_OR_LOWEST (i, NvTablePtr, StdHeader);
              PBvmRegPci->OrMask = ((UINT32)value1 << 16) | ((UINT32)value2);
              PIdsGraTmp += sizeof (BVM_REG_PCI);
              num++;
            } else if (GET_GENREG_PCI_ALLNODES (i, NvTablePtr, StdHeader) == IdsNvMiscGRA1GenRegNodeAll_Nodes) {
              PBvmRegCpuNb = (BVM_REG_CPU_NB *) PIdsGraTmp;
              PBvmRegCpuNb->type = BVM_REG_TYPE_CPU_NB;
              PBvmRegCpuNb->nodeid = IDS_ALL_MODULE;
              PBvmRegCpuNb->function = (UINT8) GET_GENREG_FUNC (i, NvTablePtr, StdHeader);
              PBvmRegCpuNb->regoff = (UINT16) GET_GENREG_OFFSET (i, NvTablePtr, StdHeader);
              value1 = GET_GENREG_AND_LOWER (i, NvTablePtr, StdHeader);
              value2 = GET_GENREG_AND_LOWEST (i, NvTablePtr, StdHeader);
              PBvmRegCpuNb->AndMask = ((UINT32)value1 << 16) | ((UINT32)value2);
              value1 = GET_GENREG_OR_LOWER (i, NvTablePtr, StdHeader);
              value2 = GET_GENREG_OR_LOWEST (i, NvTablePtr, StdHeader);
              PBvmRegCpuNb->OrMask = ((UINT32)value1 << 16) | ((UINT32)value2);
              PIdsGraTmp += sizeof (BVM_REG_CPU_NB);
              num++;
            } else {
              ASSERT (FALSE);
            }
            break;
          case IdsNvMiscGRARegTypeMSR:
            //
            // GenReg[i] is for MSR Register.
            //
            PBvmRegMsr = (BVM_REG_MSR *) PIdsGraTmp;
            PBvmRegMsr->type = BVM_REG_TYPE_MSR;
            MsrAllCore = GET_GENREG_MSR_ALLCORES (i, NvTablePtr, StdHeader);
            if (MsrAllCore == IdsNvMiscGRACoresAll_Cores) {
              PBvmRegMsr->socketid = IDS_ALL_SOCKET;
              PBvmRegMsr->coreid = IDS_ALL_CORE;
            } else if (MsrAllCore == IdsNvMiscGRACoresSpecify_Node_Core) {
              PBvmRegMsr->socketid = (UINT8) GET_GENREG_MSR_NODE (i, NvTablePtr, StdHeader);
              PBvmRegMsr->coreid = (UINT8) GET_GENREG_MSR_CORE (i, NvTablePtr, StdHeader);
            }
            value1 = GET_GENREG_MSR_HIGH (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_MSR_LOW (i, NvTablePtr, StdHeader);
            PBvmRegMsr->msraddr = ((UINT32)value1 << 16) | ((UINT32)value2);
            value1 = GET_GENREG_AND_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_AND_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegMsr->AndMask = ((UINT64)value1 << 16) | ((UINT64)value2);
            value1 = GET_GENREG_AND_HIGHEST (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_AND_HIGHER (i, NvTablePtr, StdHeader);
            PBvmRegMsr->AndMask |= ((UINT64)value1 << 48) | ((UINT64)value2 << 32);
            value1 = GET_GENREG_OR_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_OR_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegMsr->OrMask = ((UINT64)value1 << 16) | ((UINT64)value2);
            value1 = GET_GENREG_OR_HIGHEST (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_OR_HIGHER (i, NvTablePtr, StdHeader);
            PBvmRegMsr->OrMask |= ((UINT64)value1 << 48) | ((UINT64)value2 << 32);
            PIdsGraTmp += sizeof (BVM_REG_MSR);
            num++;
            break;
          case IdsNvMiscGRARegTypeMMIO:
            //
            // GenReg[i] is for MMIO Register.
            //
            PBvmRegMmio = (BVM_REG_MEM *) PIdsGraTmp;
            PBvmRegMmio->type = BVM_REG_TYPE_MEM;
            PBvmRegMmio->memaddr = (GET_GENREG_MMIO_HIGH (i, NvTablePtr, StdHeader) << 16) + (GET_GENREG_MMIO_LOW (i, NvTablePtr, StdHeader) & 0xFFFF);
            value1 = GET_GENREG_AND_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_AND_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegMmio->AndMask = ((UINT32)value1 << 16) | ((UINT32)value2);
            value1 = GET_GENREG_OR_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_OR_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegMmio->OrMask = ((UINT32)value1 << 16) | ((UINT32)value2);
            PIdsGraTmp += sizeof (BVM_REG_MEM);
            num++;
            break;
          case IdsNvMiscGRARegTypePciIndirect:
            //
            // GenReg[i] is for PCI indirect Register.
            //
            PBvmRegPciIndirect = (BVM_REG_PCI_INDIRECT *) PIdsGraTmp;
            PBvmRegPciIndirect->type = BVM_REG_TYPE_PCI_INDIRECT;
            PBvmRegPciIndirect->bus = (UINT8) GET_GENREG_BUS (i, NvTablePtr, StdHeader);
            PBvmRegPciIndirect->dev = (UINT8) GET_GENREG_DEVICE (i, NvTablePtr, StdHeader);
            PBvmRegPciIndirect->func = (UINT8) GET_GENREG_FUNC (i, NvTablePtr, StdHeader);
            PBvmRegPciIndirect->DataPortOff = (UINT16) GET_GENREG_OFFSET (i, NvTablePtr, StdHeader);
            value1 = GET_GENREG_PCIIND_OFFSET_HIGH (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_PCIIND_OFFSET_LOW (i, NvTablePtr, StdHeader);
            PBvmRegPciIndirect->IndirectRegOff = ((UINT32)value1 << 16) | ((UINT32)value2);
            value1 = GET_GENREG_PCIIND_WEBIT (i, NvTablePtr, StdHeader);
            if (value1 == IdsNvMiscGRAPciIndirectWriteEn_Auto) {
              PBvmRegPciIndirect->WriteEnBit = 0;
            } else {
              ASSERT ((value1 >= IdsNvMiscGRAPciIndirectWriteEn_Bit0) &&
                      (value1 <= IdsNvMiscGRAPciIndirectWriteEn_Bit31));
              PBvmRegPciIndirect->WriteEnBit = (UINT32) (1 << value1);
            }
            value1 = GET_GENREG_AND_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_AND_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegPciIndirect->AndMask = ((UINT32)value1 << 16) | ((UINT32)value2);
            value1 = GET_GENREG_OR_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_OR_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegPciIndirect->OrMask = ((UINT32)value1 << 16) | ((UINT32)value2);
            PIdsGraTmp += sizeof (BVM_REG_PCI_INDIRECT);
            num++;
            break;
          case IdsNvMiscGRARegTypeGmmx:
            //
            // GenReg[i] is for Gmmx Register.
            //
            PBvmRegGmmx = (BVM_REG_GMMX *) PIdsGraTmp;
            PBvmRegGmmx->type = BVM_REG_TYPE_GMMX;
            PBvmRegGmmx->Offset = (GET_GENREG_GMMX_OFFSET_HIGH (i, NvTablePtr, StdHeader) << 16) +
                                   (GET_GENREG_GMMX_OFFSET_LOW (i, NvTablePtr, StdHeader) & 0xFFFF);
            value1 = GET_GENREG_AND_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_AND_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegGmmx->AndMask = ((UINT32)value1 << 16) | ((UINT32)value2);
            value1 = GET_GENREG_OR_LOWER (i, NvTablePtr, StdHeader);
            value2 = GET_GENREG_OR_LOWEST (i, NvTablePtr, StdHeader);
            PBvmRegGmmx->OrMask = ((UINT32)value1 << 16) | ((UINT32)value2);
            PIdsGraTmp += sizeof (BVM_REG_GMMX);
            num++;
            break;
          default:
            ASSERT (FALSE);
            break;
          }
        }
      }
      //GRA Enable
      if (num) {
        //Get Heap Size
        HeapSize = (UINT32) (PIdsGraTmp - &Tmp[0]);
        PIdsGraTmp = &Tmp[0];
        ((BVM_IMAGE_HEADER *)PIdsGraTmp)->UnitNumber = num;
        //Calculate Check Sum
        Sum = 0;
        for (i = 0; i < HeapSize; i++, PIdsGraTmp++) {
          Sum = (UINT8) (Sum + *PIdsGraTmp);
        }
        PIdsGraTmp = &Tmp[0];
        ((BVM_IMAGE_HEADER *)PIdsGraTmp)->Checksum = (UINT8) (0x100 - Sum);
        //Allocate Heap
        AllocHeapParams.BufferHandle = IDS_GRA_HANDLE;
        AllocHeapParams.RequestedBufferSize = HeapSize;
        if (StdHeader->HeapStatus == HEAP_LOCAL_CACHE) {
          AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
        } else {
          AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
        }
        DEBUG_CODE (
          IDS_HDT_CONSOLE (IDS_TRACE ,"IdsGra Heap Init %d\n",AllocHeapParams.RequestedBufferSize);
        )
        if (AGESA_SUCCESS == HeapAllocateBuffer (&AllocHeapParams, StdHeader)) {
          PIdsGraTmp = AllocHeapParams.BufferPtr;
          Status = TRUE;
        }
        //Copy temp stack to heap
        LibAmdMemCopy (PIdsGraTmp, &Tmp, HeapSize, StdHeader);
        *IdsGraHeapBase = (UINT64) PIdsGraTmp;
      }
    }
  }
  return Status;
}
/**
 *  Add GRA to BVM image list
 *
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsGraInit (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  BVM_COMMON_INFO_STRUCT *PBvmCommonInfoPtr;
  BVM_IMAGE_HEADER *PBvmImageHeader;
  UINT64 BvmBinaryBase;
  UINT8  i;

  if (IDS_SUCCESS == IdsBvmGetCommonInfo (&PBvmCommonInfoPtr, StdHeader)) {
    if (PBvmCommonInfoPtr->BvmInfo != BVM_INFO_READY) {
      IdsBvmSetCommonInfo (StdHeader);
    }

    i = PBvmCommonInfoPtr->BvmBinaryBaseIndex;
    if (IdsGetGraBinaryInfo (&BvmBinaryBase, StdHeader)) {
      //Add to Bvm Binary List
      PBvmCommonInfoPtr->PBvmBinaryBase[i] = BvmBinaryBase;
      PBvmImageHeader = (BVM_IMAGE_HEADER *) PBvmCommonInfoPtr->PBvmBinaryBase[i];
      IdsFillCustomTimingInfo (PBvmImageHeader, &PBvmCommonInfoPtr->BvmOverallCustomedTiming, StdHeader);
      DEBUG_CODE (
        IDS_HDT_CONSOLE (IDS_TRACE, "BVM GRA BINARY: %d\n", i);
      )
      IdsDumpBvmBinaryInfo (PBvmCommonInfoPtr->PBvmBinaryBase[i], StdHeader);
      PBvmCommonInfoPtr->BvmBinaryBaseIndex = ++i;
    }
  }
  return IDS_SUCCESS;
}

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGraInitEarly =
{
  IDS_FEAT_COMMON,
  IDS_ALL_CORES,
  IDS_INIT_EARLY_BEFORE,
  IDS_FAMILY_ALL,
  IdsGraInit
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGraInitAfterPost =
{
  IDS_FEAT_COMMON,
  IDS_ALL_CORES,
  IDS_BEFORE_PCI_INIT,
  IDS_FAMILY_ALL,
  IdsGraInit
};

