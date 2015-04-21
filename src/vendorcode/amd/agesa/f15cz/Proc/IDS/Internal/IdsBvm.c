/* $NoKeywords:$ */
/**
 * @file
 *
 * IdsBvm.c
 *
 * IDS BVM feature support  file
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Proc/Ids/bvm)
 * @e \$Revision: 309090 $ @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
 *
 **/
/*****************************************************************************
  *
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
  * ***************************************************************************
  *
 */
#include "AGESA.h"
#include "amdlib.h"
#include "cpuRegisters.h"
#include "GeneralServices.h"
#include "cpuServices.h"
#include "Table.h"
#include "cpuFamilyTranslation.h"
#include "heapManager.h"
#include "Ids.h"
#include "IdsLib.h"
#include "IdsInternal.h"
#include "IdsInternalLib.h"
#include "IdsBvm.h"
#include "IdsRegAcc.h"
#include "IdsGra.h"
///Please check IdsInternalLib.h for internal FILECODE
#define FILECODE PROC_IDS_INTERNAL_IDSBVM_FILECODE
///Structure of input parameter for BVM register set
typedef struct _BVM_REG_OVERRIDE_IN {
  AGESA_IDS_OPTION IdsOption;     ///< IdsOption used by determine timepoint
  VOID *DataPtr;                  ///< The Pointer of input parameter
} BVM_REG_OVERRIDE_IN;

typedef void (*IDS_BVM_SET_REG) (
  IN      BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN      VOID  *Preg,
  IN      AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsBvmSetPci (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_PCI *PBvmPciReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsBvmSetIo (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_IO *PBvmIoReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsBvmSetIsa (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_ISA *PBvmIsaReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsBvmSetMem (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_MEM *PBvmMemReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsBvmSetDebugRegister (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN      BVM_REG_DR *PBvmDrReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
CheckBvmTiming (
  IN       AGESA_IDS_OPTION IdsOption,
  IN       UINT8 BvmRegType,
  IN       BVM_CUSTOMED_TIMING_STRUCT *BvmCustomedTimingPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );


VOID
IdsBvmRegOverride (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       BVM_IMAGE_HEADER *PBvmImageHeader
  );

//Information for BVM engine
BVM_ENGINE_STRUCT ROMDATA BvmEngineInfo =
{
  0x474E455F4D564226, // '&BVM_ENG'
  BVM_CURRENT_MAJOR_VERSION,
  BVM_CURRENT_MINOR_VERSION
};


/**
 *
 *
 *     BVM codes for set MSR according to input BVM_REG_MSR
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmMsrReg   - point to  BVM_REG_MSR structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetMsr (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_MSR *PBvmMsrReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IDS_REG_MSR  RegMsr;
  AGESA_IDS_OPTION IdsOption;

  IdsOption = PRegOverrideIn->IdsOption;
  RegMsr.Socket = PBvmMsrReg->socketid;
  RegMsr.Core = PBvmMsrReg->coreid;
  RegMsr.MsrAddr = PBvmMsrReg->msraddr;
  RegMsr.AndMask = PBvmMsrReg->AndMask;
  RegMsr.OrMask = PBvmMsrReg->OrMask;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM MSR    R[%x] AndMask[%x] OrMask[%x]\n",
                   PBvmMsrReg->msraddr, (UINTN) PBvmMsrReg->AndMask, (UINTN) PBvmMsrReg->OrMask);
//We can't check specific CPU 's NODE ID before Topology init so skip specific core&node in this timepoint
  if ((IdsOption == IDS_INIT_EARLY_BEFORE) || (IdsOption == IDS_UCODE) || (IdsOption == IDS_INIT_RESET_BEFORE)) {
    if ((PBvmMsrReg->socketid == 0xFF) && (PBvmMsrReg ->coreid == 0xFF)) {
      IdsRegSetMsrCmnTask (&RegMsr, StdHeader);
    }
  } else {
    switch (IdsOption) {
    case IDS_BEFORE_AP_EARLY_HALT:
    case IDS_BEFORE_PM_INIT:
      RegMsr.TimePoint = IDS_AP_RUN_CODE_EARLY;
      break;
    case IDS_INIT_MID_BEFORE:
    case IDS_BEFORE_PCI_INIT:
    case IDS_INIT_LATE_AFTER:
    case BVM_PLATFORM_TP_BEFORE_INT19:
      RegMsr.TimePoint = IDS_AP_RUN_CODE_LATE;
      break;
    case IDS_INIT_POST_BEFORE:
    case IDS_AFTER_S3_RESUME:
      RegMsr.TimePoint = IDS_AP_RUN_CODE_POST;
      break;
    default:
      //Invalid timepoint
      ASSERT (FALSE);
      return;
    }
    IdsRegSetMsr (&RegMsr, StdHeader);
  }
}

/**
 *
 *
 *     BVM codes for set CPU NB reg according to input BVM_REG_CPU_NB
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmCpuNbReg   - point to  BVM_REG_CPU_NB structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetCpuNb (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_CPU_NB *PBvmCpuNbReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 StartModule;
  UINT8 EndModule;
  UINT8 i;
  PCI_ADDR pciaddr;
  UINT32 Value;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM CPUNB  F%xx%x AndMask[%x] OrMask[%x]\n",
                   PBvmCpuNbReg->function, PBvmCpuNbReg->regoff, PBvmCpuNbReg->AndMask, PBvmCpuNbReg->OrMask);

  IdsGetStartEndModule (PBvmCpuNbReg->nodeid, &StartModule, &EndModule);
  for (i = StartModule; i <= EndModule; i++) {
    pciaddr.AddressValue = MAKE_SBDFO (0, 0, 24 + i, PBvmCpuNbReg->function, PBvmCpuNbReg->regoff);
    if (IdsCheckPciExist (pciaddr, StdHeader)) {
      LibAmdPciRead (AccessWidth32, pciaddr, &Value, StdHeader);
      IdsLibDataMaskSet32 (&Value, PBvmCpuNbReg->AndMask, PBvmCpuNbReg->OrMask);
      LibAmdPciWrite (AccessWidth32, pciaddr, &Value, StdHeader);
    }
  }
}

/**
 *
 *
 *     BVM codes for set DCT Addition Data according to input BVM_REG_DCT_ADD_DATA
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmDctAddDataReg   - point to  BVM_REG_DCT_ADD_DATA structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetDctAddData (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_DCT_ADD_DATA *PBvmDctAddDataReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  MEM_NB_BLOCK *NBPtr;
  IDS_REG_MEM MemReg;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM DCTPHY  F2x9C_%x_dct%x AndMask [%x] OrMask [%x]\n",
                   PBvmDctAddDataReg->regoff, PBvmDctAddDataReg->dctnumber,
                   PBvmDctAddDataReg->AndMask, PBvmDctAddDataReg->OrMask);
  NBPtr = (MEM_NB_BLOCK *)PRegOverrideIn->DataPtr;
  MemReg.Type = IDS_REG_MEM_PHY;
  MemReg.Module = PBvmDctAddDataReg->nodeid;
  MemReg.Dct = PBvmDctAddDataReg->dctnumber;
  MemReg.Addr.Index = PBvmDctAddDataReg->regoff;
  MemReg.AndMask = PBvmDctAddDataReg->AndMask;
  MemReg.OrMask = PBvmDctAddDataReg->OrMask;

  IdsRegSetMemBitField (&MemReg, NBPtr);
}

/**
 *
 *
 *     BVM codes for set Common PCI reg according to input BVM_REG_PCI
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmPciReg   - point to  BVM_REG_PCI structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetPci (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_PCI *PBvmPciReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  PCI_ADDR pciaddr;
  UINT32 Value;
  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM PCI    B%xD%xF%xx%x AndMask[%x] OrMask[%x]\n",
                   (UINTN) PBvmPciReg->bus, (UINTN) PBvmPciReg->dev, (UINTN) PBvmPciReg->func,
                   (UINTN) PBvmPciReg->regoff, PBvmPciReg->AndMask, PBvmPciReg->OrMask);
  pciaddr.AddressValue = MAKE_SBDFO (0, PBvmPciReg->bus, PBvmPciReg->dev, PBvmPciReg->func, PBvmPciReg->regoff);
  LibAmdPciRead (AccessWidth32, pciaddr, &Value, StdHeader);
  IdsLibDataMaskSet32 (&Value, PBvmPciReg->AndMask, PBvmPciReg->OrMask);
  LibAmdPciWrite (AccessWidth32, pciaddr, &Value, StdHeader);
}
/**
 *
 *
 *     BVM codes for set Common IO reg according to input BVM_REG_IO
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmIoReg   - point to  BVM_REG_IO structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetIo (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_IO *PBvmIoReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 tmp;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM Io     Port[%x] AndMask[%x] OrMask[%x]\n",
                   PBvmIoReg->ioport, PBvmIoReg->AndMask, PBvmIoReg->OrMask);
  LibAmdIoRead (PBvmIoReg->accesstype, PBvmIoReg->ioport, &tmp, StdHeader);
  IdsLibDataMaskSet32 (&tmp, PBvmIoReg->AndMask, PBvmIoReg->OrMask);
  LibAmdIoWrite (PBvmIoReg->accesstype, PBvmIoReg->ioport, &tmp, StdHeader);

}

/**
 *
 *
 *     BVM codes for set Common ISA reg according to input BVM_REG_ISA
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmIsaReg   - point to  BVM_REG_ISA structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetIsa (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_ISA *PBvmIsaReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 tmp;
  UINT32 index;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM IDXIO  Index[%x] Data[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
                   PBvmIsaReg->isaindex, PBvmIsaReg->isadata, PBvmIsaReg->isaoff, PBvmIsaReg->AndMask, PBvmIsaReg->OrMask);
  index = PBvmIsaReg->isaoff;
  LibAmdIoWrite (PBvmIsaReg->accesstype, PBvmIsaReg->isaindex, &index, StdHeader);
  LibAmdIoRead (PBvmIsaReg->accesstype, PBvmIsaReg->isadata, &tmp, StdHeader);
  IdsLibDataMaskSet32 (&tmp, PBvmIsaReg->AndMask, PBvmIsaReg->OrMask);
  LibAmdIoWrite (PBvmIsaReg->accesstype, PBvmIsaReg->isadata, &tmp, StdHeader);
}

/**
 *
 *
 *     BVM codes for set Common MMIO reg according to input BVM_REG_MEM
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmMemReg   - point to  BVM_REG_MEM structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetMem (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_MEM *PBvmMemReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 temp;
  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM MEM    Addr[%x] AndMask[%x] OrMask[%x]\n",
                   PBvmMemReg->memaddr, PBvmMemReg->AndMask, PBvmMemReg->OrMask);
  LibAmdMemRead (AccessWidth32, PBvmMemReg->memaddr, &temp, StdHeader);
  IdsLibDataMaskSet32 (&temp, PBvmMemReg->AndMask, PBvmMemReg->OrMask);
  LibAmdMemWrite (AccessWidth32, PBvmMemReg->memaddr, &temp, StdHeader);
}

/**
 *
 *
 *     BVM codes for set debug register according to input BVM_REG_DR
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmDrReg   - point to  BVM_REG_DR structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetDebugRegister (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_DR *PBvmDrReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IDS_REG_DR DrReg;

  DrReg.TimePoint = IDS_AP_RUN_CODE_END;
  DrReg.Socket = PBvmDrReg->socketid;
  DrReg.Core = PBvmDrReg->coreid;
  DrReg.Drindex = PBvmDrReg->drindex;
  DrReg.AndMask = PBvmDrReg->AndMask;
  DrReg.OrMask = PBvmDrReg->OrMask;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM DEBUGR Dr%x AndMask[%x] OrMask[%x]\n",
                   PBvmDrReg->drindex, (UINTN) PBvmDrReg->AndMask, (UINTN) PBvmDrReg->OrMask);

  if (PRegOverrideIn->IdsOption == IDS_INIT_RESET_BEFORE) {
    if ((PBvmDrReg->socketid == IDS_ALL_SOCKET) && (PBvmDrReg ->coreid == IDS_ALL_CORE)) {
      IdsRegSetDrCmnTask (&DrReg, StdHeader);
    }
  } else {
    switch (PRegOverrideIn->IdsOption) {
    case IDS_BEFORE_AP_EARLY_HALT:
    case IDS_BEFORE_PM_INIT:
      DrReg.TimePoint = IDS_AP_RUN_CODE_EARLY;
      break;
    case IDS_INIT_MID_BEFORE:
    case IDS_BEFORE_PCI_INIT:
    case IDS_INIT_LATE_AFTER:
    case BVM_PLATFORM_TP_BEFORE_INT19:
      DrReg.TimePoint = IDS_AP_RUN_CODE_LATE;
      break;
    case IDS_INIT_POST_BEFORE:
    case IDS_AFTER_S3_RESUME:
      DrReg.TimePoint = IDS_AP_RUN_CODE_POST;
      break;
    default:
      //Invalid timepoint
      ASSERT (FALSE);
      return;
    }
    IdsRegSetDr (&DrReg, StdHeader);
  }
}

/**
 *
 *
 *     BVM codes for set DCT Extra Data reg for family 10h according to input BVM_REG_DCT_EXTRA_DATA
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmDctExtraDataReg   - point to  BVM_REG_DCT_EXTRA_DATA structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetDctExtraData (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_DCT_EXTRA_DATA *PBvmDctExtraDataReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  MEM_NB_BLOCK *NBPtr;
  IDS_REG_MEM MemReg;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM DCTEXT R%x_dct%x AndMask [%x] OrMask [%x]\n",
                   PBvmDctExtraDataReg->regoff, PBvmDctExtraDataReg->dctnumber, PBvmDctExtraDataReg->AndMask, PBvmDctExtraDataReg->OrMask);

  NBPtr = (MEM_NB_BLOCK *)PRegOverrideIn->DataPtr;
  MemReg.Type = IDS_REG_MEM_EXTRA;
  MemReg.Module = PBvmDctExtraDataReg->nodeid;
  MemReg.Dct = PBvmDctExtraDataReg->dctnumber;
  MemReg.Addr.Index = PBvmDctExtraDataReg->regoff;
  MemReg.AndMask = PBvmDctExtraDataReg->AndMask;
  MemReg.OrMask = PBvmDctExtraDataReg->OrMask;

  IdsRegSetMemBitField (&MemReg, NBPtr);
}

/**
 *
 *
 *     BVM codes for set DCT PCI register
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmMemNbReg   - point to  BVM_REG_DCT_EXTRA_DATA structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetMemNb (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_MEM_NB *PBvmMemNbReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  MEM_NB_BLOCK *NBPtr;
  IDS_REG_MEM MemReg;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM DCTNB  F%xx%x_dct%x AndMask[%x] OrMask[%x]\n",
                   PBvmMemNbReg->PciFunc, PBvmMemNbReg->PciRegOff, PBvmMemNbReg->dctnumber, PBvmMemNbReg->AndMask, PBvmMemNbReg->OrMask);

  NBPtr = (MEM_NB_BLOCK *)PRegOverrideIn->DataPtr;
  MemReg.Type = IDS_REG_MEM_NB;
  MemReg.Module = PBvmMemNbReg->Moduleid;
  MemReg.Dct = PBvmMemNbReg->dctnumber;
  MemReg.Addr.PciAddr.Func = PBvmMemNbReg->PciFunc;
  MemReg.Addr.PciAddr.Offset = PBvmMemNbReg->PciRegOff;
  MemReg.AndMask = PBvmMemNbReg->AndMask;
  MemReg.OrMask = PBvmMemNbReg->OrMask;

  IdsRegSetMemBitField (&MemReg, NBPtr);

}


/**
 *
 *
 *     BVM codes for set indirect PCI register
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmPciIndirectReg   - point to  BVM_REG_PCI_INDIRECT structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetPciIndirect (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_PCI_INDIRECT *PBvmPciIndirectReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IDS_REG_PCI_INDIRECT PciIndirectReg;


  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM PCIIDX B%xD%xF%xx%x_%x WriteEn %x AndMask[%x] OrMask[%x]\n",
                   (UINTN) PBvmPciIndirectReg->bus, (UINTN) PBvmPciIndirectReg->dev,
                   (UINTN) PBvmPciIndirectReg->func, (UINTN) PBvmPciIndirectReg->DataPortOff,
                   (UINTN) PBvmPciIndirectReg->IndirectRegOff, (UINTN) PBvmPciIndirectReg->WriteEnBit,
                   (UINTN) PBvmPciIndirectReg->AndMask, (UINTN) PBvmPciIndirectReg->OrMask);
  PciIndirectReg.Width = AccessWidth32;
  PciIndirectReg.PciAddr = MAKE_SBDFO (0, PBvmPciIndirectReg->bus, PBvmPciIndirectReg->dev, PBvmPciIndirectReg->func, PBvmPciIndirectReg->DataPortOff - 4);
  PciIndirectReg.WriteEnBit = PBvmPciIndirectReg->WriteEnBit;
  PciIndirectReg.IndirectRegOff = PBvmPciIndirectReg->IndirectRegOff;
  PciIndirectReg.AndMask = PBvmPciIndirectReg->AndMask;
  PciIndirectReg.OrMask = PBvmPciIndirectReg->OrMask;
  IdsRegSetPciIndirect (&PciIndirectReg, StdHeader);
}

/**
 *
 *
 *     BVM codes for set GMMX register
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in]   PBvmGmmxReg   - point to  BVM_REG_GMMX structure
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmSetGmmx (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN       BVM_REG_GMMX *PBvmGmmxReg,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IDS_FAM_REG IdsFamReg;

  IDS_HDT_CONSOLE (BVM_TRACE, "\tBVM GMMX   R%x AndMask[%x] OrMask[%x]\n",
                   PBvmGmmxReg->Offset, PBvmGmmxReg->AndMask, PBvmGmmxReg->OrMask);

  IdsFamReg.Type = IDS_FAM_REG_TYPE_GMMX;
  IdsFamReg.Reg.Gmmx.Offset = PBvmGmmxReg->Offset;
  IdsFamReg.Reg.Gmmx.AndMask = PBvmGmmxReg->AndMask;
  IdsFamReg.Reg.Gmmx.OrMask = PBvmGmmxReg->OrMask;
  IdsFamRegAccess (&IdsFamReg, StdHeader);
}

BVM_REG_TIMING_OPTION  AcceptBvmPoint[] = {
  {
    BVM_REG_TYPE_MSR,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_CPU_NB,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_DCT_ADD_DATA,
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeInitializeMCT) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterAutoCycTiming) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterPlatformSpec) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeDInit) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterMemPstate1PartialTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSwWLTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwWLTrnP1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwRxEnTrnP1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterFreqChg) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwWLTrnP2) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwRxEnTrnP2) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSwRxEnTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterDqsRwPosTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterMaxRdLatTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterNbPstateChange) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterInterleave) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSettingMemoryPstate1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterFinalizeMCT)
  },
  {
    BVM_REG_TYPE_DCT_EXTRA_DATA,
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeInitializeMCT) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterAutoCycTiming) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterPlatformSpec) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeDInit) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterMemPstate1PartialTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSwWLTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwWLTrnP1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwRxEnTrnP1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterFreqChg) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwWLTrnP2) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwRxEnTrnP2) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSwRxEnTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterDqsRwPosTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterMaxRdLatTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterNbPstateChange) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterInterleave) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSettingMemoryPstate1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterFinalizeMCT)
  },
  {
    BVM_REG_TYPE_MEM_NB,
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeInitializeMCT) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterAutoCycTiming) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterPlatformSpec) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeDInit) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTBeforeTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterMemPstate1PartialTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSwWLTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwWLTrnP1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwRxEnTrnP1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterFreqChg) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwWLTrnP2) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterHwRxEnTrnP2) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSwRxEnTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterDqsRwPosTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterMaxRdLatTrn) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterNbPstateChange) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterInterleave) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterSettingMemoryPstate1) | \
    IDS_TIMING_BIT (IDS_MT_BASE + MTAfterFinalizeMCT)
  },
  {
    BVM_REG_TYPE_PCI,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_IO,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_ISA,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_MEM,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_DR,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_PCI_INDIRECT,
    IDS_TIMING_BIT (IDS_INIT_RESET_BEFORE) | \
    IDS_TIMING_BIT (IDS_BEFORE_PM_INIT) | IDS_TIMING_BIT (IDS_BEFORE_AP_EARLY_HALT) | \
    IDS_TIMING_BIT (IDS_INIT_POST_BEFORE) | IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  },
  {
    BVM_REG_TYPE_GMMX,
    IDS_TIMING_BIT (IDS_BEFORE_PCI_INIT) | \
    IDS_TIMING_BIT (IDS_INIT_MID_BEFORE) | IDS_TIMING_BIT  (IDS_INIT_LATE_AFTER) | \
    IDS_TIMING_BIT (IDS_AFTER_S3_RESUME) | IDS_TIMING_BIT (BVM_PLATFORM_TP_BEFORE_INT19)
  }
};

BOOLEAN
CheckBvmTiming (
  IN       AGESA_IDS_OPTION IdsOption,
  IN       UINT8 BvmRegType,
  IN       BVM_CUSTOMED_TIMING_STRUCT *BvmCustomedTimingPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN result;
  UINT8 i;
  BVM_REG_TIMING_OPTION BvmRegOption;
  result = FALSE;

  if (IdsOption > 0x3f) {
    return result;
  }

  if ((BvmCustomedTimingPtr->CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY) || (BvmCustomedTimingPtr->CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_BOTH)) {
    if (BvmCustomedTimingPtr->BvmCustomedHackingTiming & (1ULL << IdsOption)) {
      result = TRUE;
    }
    if (result || (BvmCustomedTimingPtr->CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY)) {
      return result;
    }
  }

  // If comes here, we still need to check the defalut timing point
  for (i = 0; i < (sizeof (AcceptBvmPoint) / sizeof (BVM_REG_TIMING_OPTION)); i++) {
    BvmRegOption = AcceptBvmPoint [i];
    if (BvmRegType == BvmRegOption.bvmreg) {
      if ((BvmRegOption.bvmregtiming & (1ULL << IdsOption))) {
        result = TRUE;
        break;
      }
    }
  }

  return result;
}

/**
 *
 *  Get Ids BVM common info data pointer in the AGESA Heap.
 *
 *  @param[in,out] BvmCommonInfoPtr  The Pointer of IDS BVM common info data structure.
 *  @param[in,out] StdHeader    The Pointer of Standard Header.
 *
 *  @retval IDS_SUCCESS       Success to get the pointer of BVM common info data.
 *  @retval IDS_UNSUPPORTED          Fail to get the pointer of BVM common info data.
 *
 **/
IDS_STATUS
IdsBvmGetCommonInfo (
  IN OUT   VOID **BvmCommonInfoPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IDS_STATUS Status;
  LOCATE_HEAP_PTR LocateHeapStructPtr;
  ALLOCATE_HEAP_PARAMS AllocHeapParams;
  AGESA_STATUS agesaStatus;

  Status = IDS_UNSUPPORTED;
  LocateHeapStructPtr.BufferHandle = IDS_EXTEND_HANDLE;
  LocateHeapStructPtr.BufferPtr = NULL;
  if (AGESA_SUCCESS == HeapLocateBuffer (&LocateHeapStructPtr, StdHeader)) {
    *BvmCommonInfoPtr = LocateHeapStructPtr.BufferPtr;
    Status = IDS_SUCCESS;
  } else {
    //GRA binary created at runtime store in heap, when heap transfered, the original base will be
    //invalid, so we need init the BVM heap again.
    AllocHeapParams.BufferHandle = IDS_EXTEND_HANDLE;
    AllocHeapParams.RequestedBufferSize = sizeof (BVM_COMMON_INFO_STRUCT);
    if (StdHeader->HeapStatus == HEAP_LOCAL_CACHE) {
      AllocHeapParams.Persist = HEAP_LOCAL_CACHE;
    } else if (StdHeader->HeapStatus == HEAP_SYSTEM_MEM) {
      AllocHeapParams.Persist = HEAP_SYSTEM_MEM;
    } else {
      Status = IDS_UNSUPPORTED;
    }
    agesaStatus = HeapAllocateBuffer (&AllocHeapParams, StdHeader);
    IDS_HDT_CONSOLE (BVM_TRACE ,"Bvm Heap Init %x\n", agesaStatus);
    if (AGESA_SUCCESS == agesaStatus) {
      *BvmCommonInfoPtr = AllocHeapParams.BufferPtr;
      Status = IDS_SUCCESS;
    }
  }

  return Status;
}

/**
 *
 *  Validate BVM binary
 *
 *  @param[in] BvmBinaryBase    The Pointer of BVM binary
 *
 *  @retval TRUE           BVM binary is valid
 *  @retval FALSE          Invalid BVM binary
 *
 **/
BOOLEAN
IdsBvmValidateBinary (
  IN OUT   UINT8 *BvmBinaryBase
  )
{
  UINT8 sum;
  UINT8 *Ptmp8;
  UINT16 i;
  UINT16 romSize;
  UINT8 *_BvmBinaryBase;
  BVM_IMAGE_HEADER *PBvmImageHeader;

  _BvmBinaryBase = BvmBinaryBase;
  PBvmImageHeader = (BVM_IMAGE_HEADER *) BvmBinaryBase;
  romSize = 2 * 1024;
  if (*((UINT32 *)_BvmBinaryBase) == 'MVB$') {
    sum = 0;
    Ptmp8 = _BvmBinaryBase;
    //Get Rom Size - Start support from V3.2
    if ((PBvmImageHeader->MajorVersion >= 3) && (PBvmImageHeader->MinorVersion >= 2)) {
      romSize = PBvmImageHeader->RomSize;
      ASSERT (romSize);
    }
    //calculate the sum of whole BVM rom
    for (i = 0; i < romSize; i++, Ptmp8++) {
      sum = sum + *Ptmp8;
    }
    //Check if checksum is zero
    if (sum == 0) {
      return TRUE;
    }
  }
  return FALSE;
}
/**
 *
 *  Get Ids BVM Binary Base in Flash
 *
 *  @param[in,out] PBvmBinaryBase    The Pointer to BVM image in Flash
 *  @param[in,out] StdHeader    The Pointer of Standard Header.
 *
 *  @retval TRUE           Success to get  the BVM info data.
 *  @retval FALSE          Fail to get the BVm info data.
 *
 **/
BOOLEAN
IdsGetBvmBinaryInfo (
  IN OUT   UINT64 *PBvmBinaryBase,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN Status;
  UINT8 *_BvmBinaryBase;
  BVM_IMAGE_HEADER *PBvmImageHeader;


  Status = FALSE;
  _BvmBinaryBase = NULL;
  // Callout to get BVM binary base, for some platform don't want BVM to search the whole ROM region which sometimes
  // cause cache victim
  IdsOptionCallout (IDS_CALLOUT_BVM_GET_BASE, &_BvmBinaryBase, StdHeader);
  if ((_BvmBinaryBase == NULL) || (IdsBvmValidateBinary (_BvmBinaryBase) == FALSE)) {
    // Scan Whole ROM from high address to low address with 2k binary to get BVM binary base address
    for (_BvmBinaryBase = (UINT8 *) (UINTN) 0xFFFFF800; _BvmBinaryBase >= (UINT8 *) (UINTN) (0xFFFFF800 - BVM_FLASH_SIZE + 0x800); _BvmBinaryBase -= 0x800) {
      if (IdsBvmValidateBinary (_BvmBinaryBase) == TRUE) {
        PBvmImageHeader = (BVM_IMAGE_HEADER *) _BvmBinaryBase;
        //V3.0 engine doesn't compatible with V2.0 & V1.0 format binary
        if (PBvmImageHeader->MajorVersion == BVM_CURRENT_MAJOR_VERSION) {
          //We found BVM binary in flash ROM, and only record when register counter is non-zero
          //Or have extended module installed
          if ((PBvmImageHeader->UnitNumber != 0) || (PBvmImageHeader->ExtendTableOffset != NULL)) {
            *PBvmBinaryBase = (UINT64) _BvmBinaryBase;
            Status = TRUE;
          }
        }
        break;
      }
    }
  } else {
    *PBvmBinaryBase = (UINT64) _BvmBinaryBase;
    Status = TRUE;
  }
  return Status;
}

/**
 *
 *  Fill BVM Custom Timing Information to BVM Cutom Timing structure
 *
 *  @param[in] PBvmImageHeader    The Pointer to BVM image
 *  @param[in,out] PBvmCustomTiming    The Pointer of BVM Cutom Timing structure
 *  @param[in,out] StdHeader    The Pointer of Standard Header.
 *
 *
 **/
VOID
IdsFillCustomTimingInfo (
  IN       BVM_IMAGE_HEADER *PBvmImageHeader,
  IN OUT   BVM_CUSTOMED_TIMING_STRUCT *PBvmCustomTiming,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT16 i;
  UINT8 type;
  UINT8 Stop;
  UINT8 typesize;
  UINT64 customedtiming;
  UINT64 finalcustomedtiming;
  UINT8 customed;
  UINT8 notcustomed;
  UINT8 CustomedFlag;
  BVM_REG_COMMON *PBvmRegCommon;

  customed = 0;
  notcustomed = 0;
  finalcustomedtiming = 0;

  PBvmRegCommon = (BVM_REG_COMMON *) ((UINT8 *)PBvmImageHeader + PBvmImageHeader->ImageTableOffset);

  for (i = 0, Stop = 0; i < PBvmImageHeader->UnitNumber; i++ ) {
    type = PBvmRegCommon->type;
    customedtiming = PBvmRegCommon->csttiming;
    if (customedtiming == (UINT64)0) {
      notcustomed = 1;
    } else {
      customed = 1;
      finalcustomedtiming |= customedtiming;
    }
    switch (type) {
    case BVM_REG_TYPE_MSR:
      typesize = sizeof (BVM_REG_MSR);
      break;
    case BVM_REG_TYPE_CPU_NB:
      typesize = sizeof (BVM_REG_CPU_NB);
      break;
    case BVM_REG_TYPE_DCT_ADD_DATA:
      typesize = sizeof (BVM_REG_DCT_ADD_DATA);
      break;
    case BVM_REG_TYPE_PCI:
      typesize = sizeof (BVM_REG_PCI);
      break;
    case BVM_REG_TYPE_IO:
      typesize = sizeof (BVM_REG_IO);
      break;
    case BVM_REG_TYPE_ISA:
      typesize = sizeof (BVM_REG_ISA);
      break;
    case BVM_REG_TYPE_MEM:
      typesize = sizeof (BVM_REG_MEM);
      break;
    case BVM_REG_TYPE_DCT_EXTRA_DATA:
      typesize = sizeof (BVM_REG_DCT_EXTRA_DATA);
      break;
    case BVM_REG_TYPE_DR:
      typesize = sizeof (BVM_REG_DR);
      break;
    default:
      typesize = 0;
      Stop = 1;
      break;
    }
    //If found fatal error exit the loop
    if (Stop != 0) {
      break;
    }
    PBvmRegCommon = (BVM_REG_COMMON *) ((UINT8 *)PBvmRegCommon + typesize);
  }
  if (customed && notcustomed) {
    // All the registers hacking use the customed timing slot
    CustomedFlag = BVM_CUSTOMEDTIMING_FLAG_BOTH;
    PBvmCustomTiming->BvmCustomedHackingTiming |= finalcustomedtiming;
  } else if (customed) {
    // Some use customed timing slot, and the other use the defalut timing slot
    CustomedFlag = BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY;
    PBvmCustomTiming->BvmCustomedHackingTiming |= finalcustomedtiming;
  } else {
    // All the registers hacking use the defalut timing slot
    CustomedFlag = BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_NON;
  }
  //Merge with previous custom flag
  if (((PBvmCustomTiming->CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY) &&
       (CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_NON)) ||
      ((PBvmCustomTiming->CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_NON) &&
       (CustomedFlag == BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY))) {
    PBvmCustomTiming->CustomedFlag = BVM_CUSTOMEDTIMING_FLAG_BOTH;
  }

}
/**
 *
 *  Set Ids BVM common info data pointer in the AGESA Heap.
 *
 *  @param[in] StdHeader    The Pointer of Standard Header.
 *
 *  @retval TRUE           Success to set  the BVM info data.
 *  @retval FALSE          Fail to set the BVM info data.
 *
 **/
BOOLEAN
IdsBvmSetCommonInfo (
  IN   AMD_CONFIG_PARAMS *StdHeader
  )
{
  BVM_IMAGE_HEADER *PBvmImageHeader;
  BOOLEAN returnStatus;
  BOOLEAN isBvmBinfound;
  UINT64 BvmBinaryBase;
  BVM_COMMON_INFO_STRUCT *PBvmCommonInfoPtr;
  UINT8  i;

  returnStatus = FALSE;
  PBvmCommonInfoPtr = NULL;
  BvmBinaryBase = NULL;
  if (IDS_SUCCESS == IdsBvmGetCommonInfo (&PBvmCommonInfoPtr, StdHeader)) {
    if (PBvmCommonInfoPtr->BvmInfo != BVM_INFO_READY) {
      // Set the bvm info ready flag
      PBvmCommonInfoPtr->BvmInfo = BVM_INFO_READY;
      PBvmCommonInfoPtr->BvmBinaryBaseIndex = 0;

      i = PBvmCommonInfoPtr->BvmBinaryBaseIndex;
      isBvmBinfound = IdsGetBvmBinaryInfo (&BvmBinaryBase, StdHeader);
      if (isBvmBinfound) {
        //Add to Bvm Binary List
        PBvmCommonInfoPtr->PBvmBinaryBase[i] = BvmBinaryBase;
        PBvmImageHeader = (BVM_IMAGE_HEADER *) PBvmCommonInfoPtr->PBvmBinaryBase[i];
        IdsFillCustomTimingInfo (PBvmImageHeader, &PBvmCommonInfoPtr->BvmOverallCustomedTiming, StdHeader);
        IDS_HDT_CONSOLE (BVM_TRACE, "BVM binary found @%x\n", (UINTN) BvmBinaryBase);
        DEBUG_CODE (IdsDumpBvmBinaryInfo (PBvmCommonInfoPtr->PBvmBinaryBase[i], StdHeader));
        PBvmCommonInfoPtr->BvmBinaryBaseIndex = ++i;
        returnStatus = TRUE;
      }
    }
  }

  return returnStatus;
}

/**
 *
 *
 *     BVM hook routine for IDS dispatcher
 *
 *     @param[in]   IdsOption   - option of IDS_OPTION_HOOK
 *     @param[in,out]   DataPtr   -   The Pointer of input parameter
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 */
VOID
IdsBvmOptionHook (
  IN       AGESA_IDS_OPTION IdsOption,
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 i;
  BOOLEAN TimeMatched;
  BVM_COMMON_INFO_STRUCT *PBvmCommonInfoPtr;
  BVM_REG_OVERRIDE_IN RegOverrideIn;
  BVM_ENGINE_STRUCT *PBvmEngine;

  //To avoid BvmEngineInfo doesn't link into final image
  PBvmEngine = &BvmEngineInfo;

  PBvmCommonInfoPtr = NULL;
  if (IDS_SUCCESS == IdsBvmGetCommonInfo (&PBvmCommonInfoPtr, StdHeader)) {
    if (PBvmCommonInfoPtr->BvmInfo != BVM_INFO_READY) {
      IdsBvmSetCommonInfo (StdHeader);
    }

    //if any valid binary
    if (PBvmCommonInfoPtr->BvmBinaryBaseIndex != 0) {
      TimeMatched = FALSE;
    //check if any type accept this point to save parse time.
      for (i = BVM_REG_TYPE_MSR; i < BVM_MAX_SUPPORT_REG; i++) {
        if (CheckBvmTiming (IdsOption, i, &(PBvmCommonInfoPtr->BvmOverallCustomedTiming), StdHeader)) {
          TimeMatched = TRUE;
          break;
        }
      }
      if (TimeMatched) {
        RegOverrideIn.IdsOption = IdsOption;
        RegOverrideIn.DataPtr = DataPtr;
        //Do the loop for both BVM & GRA
        for (i = 0; i < MAX_NUM_BVM_BIN; i++) {
          if (PBvmCommonInfoPtr->PBvmBinaryBase[i] != NULL) {
            IdsBvmRegOverride (&RegOverrideIn, StdHeader, (BVM_IMAGE_HEADER *)PBvmCommonInfoPtr->PBvmBinaryBase[i]);
          }
        }
      }
    }
  }
}

/**
 *
 *
 *     BVM engine for parse the BVM binary and dispatch
 *
 *     @param[in]   PRegOverrideIn   - Input parameter include IdsOption & Void Pointer
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *     @param[in]     PBvmImageHeader   -  The pointer to the BVM image header structure
 *
 */

VOID
IdsBvmRegOverride (
  IN       BVM_REG_OVERRIDE_IN *PRegOverrideIn,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       BVM_IMAGE_HEADER *PBvmImageHeader
  )
{
  BVM_REG_COMMON *PBvmRegCommon;
  IDS_BVM_SET_REG IdsBvmSetRegFunc;
  UINT16 i;
  UINT8 type;
  UINT8 Stop;
  UINT8 typesize;
  BVM_CUSTOMED_TIMING_STRUCT BvmCutomedTiming;
  BOOLEAN alreadySetReg;

  alreadySetReg = FALSE;
  PBvmRegCommon = (BVM_REG_COMMON *) ((UINT8 *)PBvmImageHeader + PBvmImageHeader->ImageTableOffset);
  for (i = 0, Stop = 0; i < PBvmImageHeader->UnitNumber; i++ ) {
    type = PBvmRegCommon->type;
    BvmCutomedTiming.BvmCustomedHackingTiming = PBvmRegCommon->csttiming;
    if (BvmCutomedTiming.BvmCustomedHackingTiming == 0) {
      BvmCutomedTiming.CustomedFlag = BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_NON;
    } else {
      BvmCutomedTiming.CustomedFlag = BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY;
    }
    switch (type) {
    case BVM_REG_TYPE_MSR:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetMsr;
      typesize = sizeof (BVM_REG_MSR);
      break;
    case BVM_REG_TYPE_CPU_NB:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetCpuNb;
      typesize = sizeof (BVM_REG_CPU_NB);
      break;
    case BVM_REG_TYPE_DCT_ADD_DATA:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetDctAddData;
      typesize = sizeof (BVM_REG_DCT_ADD_DATA);
      break;
    case BVM_REG_TYPE_PCI:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetPci;
      typesize = sizeof (BVM_REG_PCI);
      break;
    case BVM_REG_TYPE_IO:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetIo;
      typesize = sizeof (BVM_REG_IO);
      break;
    case BVM_REG_TYPE_ISA:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetIsa;
      typesize = sizeof (BVM_REG_ISA);
      break;
    case BVM_REG_TYPE_MEM:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetMem;
      typesize = sizeof (BVM_REG_MEM);
      break;
    case BVM_REG_TYPE_DCT_EXTRA_DATA:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetDctExtraData;
      typesize = sizeof (BVM_REG_DCT_EXTRA_DATA);
      break;
    case BVM_REG_TYPE_DR:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetDebugRegister;
      typesize = sizeof (BVM_REG_DR);
      break;
    case BVM_REG_TYPE_MEM_NB:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetMemNb;
      typesize = sizeof (BVM_REG_MEM_NB);
      break;
    case BVM_REG_TYPE_PCI_INDIRECT:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetPciIndirect;
      typesize = sizeof (BVM_REG_PCI_INDIRECT);
      break;
    case BVM_REG_TYPE_GMMX:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsBvmSetGmmx;
      typesize = sizeof (BVM_REG_GMMX);
      break;
    default:
      IdsBvmSetRegFunc = (IDS_BVM_SET_REG) IdsCommonReturn;
      typesize = 0;
      Stop = 1;
      break;
    }
  //If found fatal error exit the loop
    if (Stop != 0) {
      break;
    }
    if (CheckBvmTiming (PRegOverrideIn->IdsOption, type, &BvmCutomedTiming, StdHeader)) {
      IdsBvmSetRegFunc (PRegOverrideIn, PBvmRegCommon, StdHeader);
      alreadySetReg = TRUE;
    }
    PBvmRegCommon = (BVM_REG_COMMON *) ((UINT8 *)PBvmRegCommon + typesize);
  }
  if (alreadySetReg) {
    IDS_HDT_CONSOLE (BVM_TRACE, "\t<<@TP[%x]\n\n",PRegOverrideIn->IdsOption);
  }
}

/**
 *
 *
 *     BVM Agesa Entry Point
 *     BVM website will add it to install.h at its build time
 *
 *     @param[in]   IdsBvmPara   - Parameter include stdheader, and platform timepoint
 *
 *     @retval AGESA_SUCCESS         The function is called successfully
 *
 *
 */
AGESA_STATUS
IdsBvmAgesaEntry (
  IN       IDS_BVM_PARAMS *IdsBvmPara
  )
{

  IdsBvmOptionHook (IdsBvmPara->platformtp, NULL, &IdsBvmPara->StdHeader);

  return AGESA_SUCCESS;
}
/**
 *
 *  Dump BVM Binary Base
 *
 *  @param[in] BvmBinaryBase    Base Address of BVM binary
 *  @param[in, out] StdHeader    The Pointer of Standard Header.
 *
 *
 **/
VOID
IdsDumpBvmBinaryInfo (
  IN OUT   UINT64 BvmBinaryBase,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  BVM_IMAGE_HEADER *PBvmHeader;
  UINT32 i;
  BVM_REG_COMMON *PBvmRegCommon;
  UINT8   type;
  UINT8 typesize;
  UINT8 Stop;
  UINT8 *PBvmBinaryBase;

  PBvmBinaryBase = (UINT8 *) (UINTN) BvmBinaryBase;
  //Debug Code, Dump the GRA BVM runtime memory
  IDS_HDT_CONSOLE (BVM_TRACE,"IDS BVM Memory Dump [%x] Start\n", PBvmBinaryBase);
  PBvmHeader = (BVM_IMAGE_HEADER *) PBvmBinaryBase;
  IDS_HDT_CONSOLE (BVM_TRACE,"MajorVer: %d Num: %d\n", PBvmHeader->MajorVersion, PBvmHeader->UnitNumber);
  PBvmRegCommon = (BVM_REG_COMMON *) ((UINT8 *)PBvmHeader + PBvmHeader->ImageTableOffset);
  for (i = 0, Stop = 0; (i < PBvmHeader->UnitNumber) && (Stop == 0); i++ ) {
    type = PBvmRegCommon->type;
    IDS_HDT_CONSOLE (BVM_TRACE,"\tTiming [%x] ", PBvmRegCommon->csttiming);
    switch (type) {
    case BVM_REG_TYPE_MSR:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[MSR] Socket[%x] Core[%x] Addr[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_MSR *)PBvmRegCommon)->socketid, ((BVM_REG_MSR *)PBvmRegCommon)->coreid,
      ((BVM_REG_MSR *)PBvmRegCommon)->msraddr, (UINTN) ((BVM_REG_MSR *)PBvmRegCommon)->AndMask, (UINTN) ((BVM_REG_MSR *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_MSR);
      break;
    case BVM_REG_TYPE_CPU_NB:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[CPU_NB] Node[%x] Function[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_CPU_NB *)PBvmRegCommon)->nodeid, ((BVM_REG_CPU_NB *)PBvmRegCommon)->function,
      ((BVM_REG_CPU_NB *)PBvmRegCommon)->regoff, ((BVM_REG_CPU_NB *)PBvmRegCommon)->AndMask, ((BVM_REG_CPU_NB *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_CPU_NB);
      break;
    case BVM_REG_TYPE_DCT_ADD_DATA:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[DCT_ADD_DATA] Mstate[%x] Node[%x] dct[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_DCT_ADD_DATA *)PBvmRegCommon)->Mstate, ((BVM_REG_DCT_ADD_DATA *)PBvmRegCommon)->nodeid,
      ((BVM_REG_DCT_ADD_DATA *)PBvmRegCommon)->dctnumber, ((BVM_REG_DCT_ADD_DATA *)PBvmRegCommon)->regoff,
      ((BVM_REG_DCT_ADD_DATA *)PBvmRegCommon)->AndMask, ((BVM_REG_DCT_ADD_DATA *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_DCT_ADD_DATA);
      break;
    case BVM_REG_TYPE_PCI:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[PCI] bus[%x] Dev[%x] Func[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_PCI *)PBvmRegCommon)->bus, ((BVM_REG_PCI *)PBvmRegCommon)->dev, ((BVM_REG_PCI *)PBvmRegCommon)->func,
      ((BVM_REG_PCI *)PBvmRegCommon)->regoff, ((BVM_REG_PCI *)PBvmRegCommon)->AndMask, ((BVM_REG_PCI *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_PCI);
      break;
    case BVM_REG_TYPE_IO:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[IO] accesstype[%x] IoPort[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_IO *)PBvmRegCommon)->accesstype, ((BVM_REG_IO *)PBvmRegCommon)->ioport,
      ((BVM_REG_IO *)PBvmRegCommon)->AndMask, ((BVM_REG_IO *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_IO);
      break;
    case BVM_REG_TYPE_ISA:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[ISA] accesstype[%x] Index[%x] Data[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_ISA *)PBvmRegCommon)->accesstype, ((BVM_REG_ISA *)PBvmRegCommon)->isaindex, ((BVM_REG_ISA *)PBvmRegCommon)->isadata,
      ((BVM_REG_ISA *)PBvmRegCommon)->isaoff, ((BVM_REG_ISA *)PBvmRegCommon)->AndMask, ((BVM_REG_ISA *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_ISA);
      break;
    case BVM_REG_TYPE_MEM:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[MEM] Address[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_MEM *)PBvmRegCommon)->memaddr, ((BVM_REG_MEM*)PBvmRegCommon)->AndMask, ((BVM_REG_MEM *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_MEM);
      break;
    case BVM_REG_TYPE_DCT_EXTRA_DATA:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[DCT_EXTRA_DATA] Node[%x] dct[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_DCT_EXTRA_DATA *)PBvmRegCommon)->nodeid, ((BVM_REG_DCT_EXTRA_DATA *)PBvmRegCommon)->dctnumber,
      ((BVM_REG_DCT_EXTRA_DATA *)PBvmRegCommon)->regoff, ((BVM_REG_DCT_EXTRA_DATA *)PBvmRegCommon)->AndMask, ((BVM_REG_DCT_EXTRA_DATA *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_DCT_EXTRA_DATA);
      break;
    case BVM_REG_TYPE_DR:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[DR] Index[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_DR *)PBvmRegCommon)->drindex, ((BVM_REG_DR *)PBvmRegCommon)->AndMask, ((BVM_REG_DR *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_DR);
      break;
    case BVM_REG_TYPE_MEM_NB:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[MEM_NB] Module[%x] Dct[%x] Function[%x] Offset[%x] AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_MEM_NB *)PBvmRegCommon)->Moduleid, ((BVM_REG_MEM_NB *)PBvmRegCommon)->dctnumber, ((BVM_REG_MEM_NB *)PBvmRegCommon)->PciFunc,
      ((BVM_REG_MEM_NB *)PBvmRegCommon)->PciRegOff, ((BVM_REG_MEM_NB *)PBvmRegCommon)->AndMask, ((BVM_REG_MEM_NB *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_MEM_NB);
      break;
    case BVM_REG_TYPE_PCI_INDIRECT:
      IDS_HDT_CONSOLE (BVM_TRACE,"Type[PCI_IND] B%xD%xF%xx%x_%x WE %x AndMask[%x] OrMask[%x]\n",
      ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->bus, ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->dev, ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->func,
      ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->DataPortOff, ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->IndirectRegOff, ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->WriteEnBit,
      ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->AndMask, ((BVM_REG_PCI_INDIRECT *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_PCI_INDIRECT);
      break;
    case BVM_REG_TYPE_GMMX:
      IDS_HDT_CONSOLE (BVM_TRACE, "Type[GMMX] GMMX_%x AndMask [%x] OrMask [%x]\n",
                       ((BVM_REG_GMMX *)PBvmRegCommon)->Offset, ((BVM_REG_GMMX *)PBvmRegCommon)->AndMask, ((BVM_REG_GMMX *)PBvmRegCommon)->OrMask);
      typesize = sizeof (BVM_REG_GMMX);
      break;
    default:
      IDS_HDT_CONSOLE (BVM_TRACE,"Error: Unrecognize Register Type\n");
      Stop = 1;
      typesize = 0;
      break;
    }
    //If found fatal error exit the loop
    if (Stop != 0) {
      break;
    }
    PBvmRegCommon = (BVM_REG_COMMON *) ((UINT8 *)PBvmRegCommon + typesize);
  }
  IDS_HDT_CONSOLE (BVM_TRACE,"IDS BVM Memory Dump End\n");
}

/**
 *
 *
 *     Get Next extended module base address
 *
 *     @param[in,out]   ExtModulePtr   -   Point to extend module base
 *     @param[in, out]  StdHeader    The Pointer of Standard Header.
 *
 */
VOID
IdsBvmGetNextExtModule (
  IN OUT   BVM_EXTEND_COMMON_HEADER *ExtModulePtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  if ((ExtModulePtr->Type == 0) || (ExtModulePtr->ExtSig != BVM_EXTEND_MODULE_SIGNATURE)) {
    return;
  }

  ExtModulePtr = (BVM_EXTEND_COMMON_HEADER *) ((UINT8 *)ExtModulePtr + ExtModulePtr->Size);
  return;
}

/**
 *
 *     Get extended module base address via type
 *
 *     @param[in]   ModuleType   - Type ID of the module
 *     @param[in,out]   ExtModulePtr   -   Point to extend module base
 *     @param[in, out]  StdHeader    The Pointer of Standard Header.
 *
 *     @retval IDS_SUCCESS       Success to get the pointer of BVM extend module base
 *     @retval IDS_UNSUPPORTED          Fail to get the pointer of BVM extend module base
 */
IDS_STATUS
IdsBvmGetExtendModuleBaseViaType (
  IN       UINT32 ModuleType,
  IN OUT   VOID **ExtModulePtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 i;
  BVM_COMMON_INFO_STRUCT *pBvmCommonInfo;
  BVM_IMAGE_HEADER *pBvmImageHeader;
  BVM_EXTEND_COMMON_HEADER *pBvmExtHeader;

  if (ModuleType == 0) {
    return IDS_UNSUPPORTED;
  }

  if (ExtModulePtr == NULL) {
    return IDS_UNSUPPORTED;
  }

  //Get BVM informations
  if (IDS_SUCCESS == IdsBvmGetCommonInfo (&pBvmCommonInfo, StdHeader)) {
    //traverse all BVM binary, we supposed only BVM binary support the extend module
    for (i = 0; i < pBvmCommonInfo->BvmBinaryBaseIndex; i++) {
      pBvmImageHeader = (BVM_IMAGE_HEADER *) (UINTN) (pBvmCommonInfo->PBvmBinaryBase [i]);
      //check if extend module exsist
      if (pBvmImageHeader->ExtendTableOffset != 0) {
        //Get Extend Module start
        pBvmExtHeader = (BVM_EXTEND_COMMON_HEADER *) ((UINT8 *)pBvmImageHeader + pBvmImageHeader->ExtendTableOffset);
        while ((pBvmExtHeader->Type != 0) && (pBvmExtHeader->ExtSig == BVM_EXTEND_MODULE_SIGNATURE)) {
          if (pBvmExtHeader->Type == ModuleType) {
            *ExtModulePtr = pBvmExtHeader;
            return IDS_SUCCESS;
          }
          IdsBvmGetNextExtModule (pBvmExtHeader, StdHeader);
        }
      }
    }
  }
  return IDS_UNSUPPORTED;
}
