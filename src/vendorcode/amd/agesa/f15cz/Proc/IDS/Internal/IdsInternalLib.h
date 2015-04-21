/* $NoKeywords:$ */
/**
 * @file
 *
 * IdsInternalLib.h
 *
 * Contains AMD AGESA Internal Integrated Debug Macros
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project: (Proc/Ids/Internal)
 * @e \$Revision: 311976 $   @e \$Date: 2015-01-29 13:34:44 +0800 (Thu, 29 Jan 2015) $
 */
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
 *
 ***************************************************************************/

  /* Macros to aid debugging */
  /* These definitions expand to zero (0) bytes of code when disabled */

#ifndef _IDS_INTERNAL_LIB_H_
#define _IDS_INTERNAL_LIB_H_
#include "mm.h"
#include "IdsBvm.h"

///IDS Internal FILECODE
#define PROC_IDS_INTERNAL_IDSBVM_FILECODE                                               (0xE820)
#define PROC_IDS_INTERNAL_IDSINTERNAL_FILECODE                                          (0xE821)
#define PROC_IDS_INTERNAL_IDSMEMORYLOADCARD_FILECODE                                    (0xE822)
#define PROC_IDS_INTERNAL_IDSGRA_FILECODE                                               (0xE823)

#define PROC_IDS_INTERNAL_FAMILY_0X15_IDSINTF15ALLSERVICE_FILECODE                      (0xE82C)
#define PROC_IDS_INTERNAL_FAMILY_0X15_CZ_IEMF15CZ_FILECODE                              (0xE832)
#define PROC_IDS_INTERNAL_FAMILY_0X15_CZ_IDSINTF15CZALLSERVICE_FILECODE                 (0xE834)
#define PROC_IDS_INTERNAL_FAMILY_0X15_ST_IEMF15ST_FILECODE                              (0xE835)
#define PROC_IDS_INTERNAL_FAMILY_0X15_ST_IDSINTF15STALLSERVICE_FILECODE                 (0xE836)

/*---------------------------------------------------------------------------*/
/**
 *  Modify the value in MEM_TABLE_ALIAS Data Structure.
 *
 *  This MACRO is to used to fill each field in MEM_TABLE_ALIAS Data Structure.
 *
 *  @param[out]   MemTablePtr The pointer of MEM_TABLE_ALIAS table.
 *  @param[in]    Time        See @ref MTTime.
 *  @param[in]    Node        See @ref MTNode
 *  @param[in]    Dct         See @ref MTDct
 *  @param[in]    Dimm        See @ref MTDIMM
 *  @param[in]    ByteLane    See @ref MTBL
 *  @param[in]    BitField    See enum BIT_FIELD_NAME
 *  @param[in]    Attr        See @ref MTAttr
 *  @param[in]    Value       The value to set in this field.
 *
 **/
#define MAKE_MEM_TABLE_ALIAS(MemTablePtr, Time, Node, Dct, Dimm, ByteLane, BitField, Attr, Value) \
          MemTablePtr->time = (Time); \
          MemTablePtr->node = (Node); \
          MemTablePtr->dct = (Dct); \
          MemTablePtr->dimm = (Dimm); \
          MemTablePtr->data.s.bytelane = (ByteLane); \
          MemTablePtr->bfindex = (BitField); \
          MemTablePtr->attr = (Attr); \
          MemTablePtr->data.s.value = (Value); \
          MemTablePtr->vtype = VT_MSK_VALUE;
//
// Definitions and Macros
//

#define PS_NB_VID_1175V                0x1E        ///< NB VID 1.175V.
#define PS_NB_DID0                0                       ///< NB DID 0.
#define PS_CPU_VID_1175V                0x1E      ///< CPU VID 1.175V.
#define VID_1_175V                0x1E                  ///< 1.175V
#define NB_FID_800M                0                    ///< 800MHz
#define PS_CPU_DID                1                     ///< CPU DID 1 = divisor of 2
#define PS_CPU_FID_16G                0              ///< CPU FID of 00 = 1.6GHz

#define IDS_CUSTOM_CORE_PSTATE_START             AGESA_IDS_NV_PSTATE0COREFID
#define IDS_CUSTOM_CORE_PSTATE_OPTION_SIZE       (AGESA_IDS_NV_PSTATE1COREFID - AGESA_IDS_NV_PSTATE0COREFID)

#define GETPXCOREFID(PSSTART, IdsNvPtr, StdHeader)        AmdIdsNvReader ((UINT16) (IDS_CUSTOM_CORE_PSTATE_START + PSSTART * IDS_CUSTOM_CORE_PSTATE_OPTION_SIZE), IdsNvPtr, StdHeader)
#define GETPXCOREVID(PSSTART, IdsNvPtr, StdHeader)        AmdIdsNvReader ((UINT16) (IDS_CUSTOM_CORE_PSTATE_START + PSSTART * IDS_CUSTOM_CORE_PSTATE_OPTION_SIZE + 1), IdsNvPtr, StdHeader)
#define GETPXCOREDID(PSSTART, IdsNvPtr, StdHeader)        AmdIdsNvReader ((UINT16) (IDS_CUSTOM_CORE_PSTATE_START + PSSTART * IDS_CUSTOM_CORE_PSTATE_OPTION_SIZE + 2), IdsNvPtr, StdHeader)
#define GETPXCOREDIDLSD(PSSTART, IdsNvPtr, StdHeader)     AmdIdsNvReader ((UINT16) (IDS_CUSTOM_CORE_PSTATE_START + PSSTART * IDS_CUSTOM_CORE_PSTATE_OPTION_SIZE + 3), IdsNvPtr, StdHeader)

#define IDS_CUSTOM_NB_PSTATE_START              AGESA_IDS_NV_PSTATE0NBVID
#define IDS_CUSTOM_NB_PSTATE_OPTION_SIZE        (AGESA_IDS_NV_PSTATE1NBVID - AGESA_IDS_NV_PSTATE0NBVID)

#define GETPXNBVID(PSSTART, IdsNvPtr, StdHeader)     AmdIdsNvReader ((UINT16) (IDS_CUSTOM_NB_PSTATE_START + PSSTART * IDS_CUSTOM_NB_PSTATE_OPTION_SIZE), IdsNvPtr, StdHeader)
#define GETPXNBDID(PSSTART, IdsNvPtr, StdHeader)     AmdIdsNvReader ((UINT16) (IDS_CUSTOM_NB_PSTATE_START + PSSTART * IDS_CUSTOM_NB_PSTATE_OPTION_SIZE + 1), IdsNvPtr, StdHeader)
#define GETPXNBFID(PSSTART, IdsNvPtr, StdHeader)     AmdIdsNvReader ((UINT16) (IDS_CUSTOM_NB_PSTATE_START + PSSTART * IDS_CUSTOM_NB_PSTATE_OPTION_SIZE + 2), IdsNvPtr, StdHeader)


//
// IDS GRA Option Definitions and Macros
//

#define IDS_GRA_NV_OPTION_START         AGESA_IDS_NV_GENREG1E
#define IDS_GRA_NV_OPTION_SIZE          (AGESA_IDS_NV_GENREG2E - AGESA_IDS_NV_GENREG1E)

#define GET_GENREG_EN(GRASTART, IdsNvPtr, StdHeader)                      AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE), IdsNvPtr, StdHeader)
#define GET_GENREG_TYPE(GRASTART, IdsNvPtr, StdHeader)                    AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 1), IdsNvPtr, StdHeader)
#define GET_GENREG_WHEN(GRASTART, IdsNvPtr, StdHeader)                    AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 2), IdsNvPtr, StdHeader)
#define GET_GENREG_PCI_ALLNODES(GRASTART, IdsNvPtr, StdHeader)            AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 3), IdsNvPtr, StdHeader)
#define GET_GENREG_MSR_ALLCORES(GRASTART, IdsNvPtr, StdHeader)            AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 4), IdsNvPtr, StdHeader)
#define GET_GENREG_MSR_NODE(GRASTART, IdsNvPtr, StdHeader)                AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 5), IdsNvPtr, StdHeader)
#define GET_GENREG_MSR_CORE(GRASTART, IdsNvPtr, StdHeader)                AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 6), IdsNvPtr, StdHeader)
#define GET_GENREG_BUS(GRASTART, IdsNvPtr, StdHeader)                     AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 7), IdsNvPtr, StdHeader)
#define GET_GENREG_DEVICE(GRASTART, IdsNvPtr, StdHeader)                  AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 8), IdsNvPtr, StdHeader)
#define GET_GENREG_FUNC(GRASTART, IdsNvPtr, StdHeader)                    AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 9), IdsNvPtr, StdHeader)
#define GET_GENREG_OFFSET(GRASTART, IdsNvPtr, StdHeader)                  AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 10), IdsNvPtr, StdHeader)
#define GET_GENREG_MSR_HIGH(GRASTART, IdsNvPtr, StdHeader)                AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 11), IdsNvPtr, StdHeader)
#define GET_GENREG_MSR_LOW(GRASTART, IdsNvPtr, StdHeader)                 AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 12), IdsNvPtr, StdHeader)
#define GET_GENREG_OR_HIGHEST(GRASTART, IdsNvPtr, StdHeader)              AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 13), IdsNvPtr, StdHeader)
#define GET_GENREG_OR_HIGHER(GRASTART, IdsNvPtr, StdHeader)               AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 14), IdsNvPtr, StdHeader)
#define GET_GENREG_OR_LOWER(GRASTART, IdsNvPtr, StdHeader)                AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 15), IdsNvPtr, StdHeader)
#define GET_GENREG_OR_LOWEST(GRASTART, IdsNvPtr, StdHeader)               AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 16), IdsNvPtr, StdHeader)
#define GET_GENREG_AND_HIGHEST(GRASTART, IdsNvPtr, StdHeader)             AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 17), IdsNvPtr, StdHeader)
#define GET_GENREG_AND_HIGHER(GRASTART, IdsNvPtr, StdHeader)              AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 18), IdsNvPtr, StdHeader)
#define GET_GENREG_AND_LOWER(GRASTART, IdsNvPtr, StdHeader)               AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 19), IdsNvPtr, StdHeader)
#define GET_GENREG_AND_LOWEST(GRASTART, IdsNvPtr, StdHeader)              AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 20), IdsNvPtr, StdHeader)
#define GET_GENREG_MMIO_HIGH(GRASTART, IdsNvPtr, StdHeader)               AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 21), IdsNvPtr, StdHeader)
#define GET_GENREG_MMIO_LOW(GRASTART, IdsNvPtr, StdHeader)                AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 22), IdsNvPtr, StdHeader)
#define GET_GENREG_PCIIND_OFFSET_HIGH(GRASTART, IdsNvPtr, StdHeader)      AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 23), IdsNvPtr, StdHeader)
#define GET_GENREG_PCIIND_OFFSET_LOW(GRASTART, IdsNvPtr, StdHeader)       AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 24), IdsNvPtr, StdHeader)
#define GET_GENREG_PCIIND_WEBIT(GRASTART, IdsNvPtr, StdHeader)            AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 25), IdsNvPtr, StdHeader)
#define GET_GENREG_GMMX_OFFSET_HIGH(GRASTART, IdsNvPtr, StdHeader)        AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 26), IdsNvPtr, StdHeader)
#define GET_GENREG_GMMX_OFFSET_LOW(GRASTART, IdsNvPtr, StdHeader)         AmdIdsNvReader ((UINT16) (IDS_GRA_NV_OPTION_START + GRASTART * IDS_GRA_NV_OPTION_SIZE + 27), IdsNvPtr, StdHeader)


#define IDS_MAX_GRA_ITEMS   8       ///< Maximum GRA number to support in IDS.

/// Data Structure to define BVM customed hacking timing
typedef struct {
  UINT8 CustomedFlag;                             ///< BVM customed timing flag
  UINT64 BvmCustomedHackingTiming;   ///< BVM customed hacking timing
} BVM_CUSTOMED_TIMING_STRUCT;

#define BVM_CUSTOMEDTIMING_FLAG_UNREADY              0x0
#define BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_NON         0x1
#define BVM_CUSTOMEDTIMING_FLAG_CUSTOMED_ONLY        0x2
#define BVM_CUSTOMEDTIMING_FLAG_BOTH                 0x3
#define BVM_CUSTOMEDTIMING_FLAG_UNSPPORTED           0xFF
#define BVM_INFO_READY      0xAA
#define BVM_INFO_UNREADY 0x00

#define MAX_NUM_BVM_BIN                             0x2
/// Data Structure to define BVM common info data
typedef struct {
  UINT8 BvmInfo;                                       ///< BVM common info struct is set in the heap
  UINT8 BvmBinaryBaseIndex;                                ///< Point to Next BVM Binary Base Index
  UINT64 PBvmBinaryBase[MAX_NUM_BVM_BIN];                  ///< BVM format point list, NULL as the end
  BVM_CUSTOMED_TIMING_STRUCT BvmOverallCustomedTiming; ///< BVM bin valid flag, check the checksum
} BVM_COMMON_INFO_STRUCT;


IDS_STATUS
IdsBvmGetCommonInfo (
  IN OUT   VOID **BvmCommonInfoPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

BOOLEAN
IdsBvmSetCommonInfo (
  IN   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsFillCustomTimingInfo (
  IN       BVM_IMAGE_HEADER *PBvmImageHeader,
  IN OUT   BVM_CUSTOMED_TIMING_STRUCT *PBvmCustomTiming,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsDumpBvmBinaryInfo (
  IN OUT   UINT64 BvmBinaryBase,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

IDS_STATUS
IdsBvmGetExtendModuleBaseViaType (
  IN       UINT32 ModuleType,
  IN OUT   VOID **ExtModulePtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsGetBvmUcodeBase (
  IN OUT   UINT8 ** pUcodePtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

VOID
IdsIntMsrRead (
  IN       UINT32 MsrAddress,
     OUT   UINT64 *Value
  );

VOID
IdsIntMsrWrite (
  IN       UINT32 MsrAddress,
  IN       UINT64 *Value
  );

VOID
IdsBvmOptionHook (
  IN       AGESA_IDS_OPTION IdsOption,
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

//IDS option 0x38 ~ 0x3F is reserved for platform only
#define BVM_PLATFORM_TP_BEFORE_WARMRESET   IDS_PLATFORM_RSVD1
#define BVM_PLATFORM_TP_BEFORE_INT19   IDS_PLATFORM_RSVD8
///Ids Feat SubRoutine

IDS_STATUS
IdsSubPss (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubCst (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubWhea (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubDmi (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubPsi (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubDctConfigPostBefore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubGetDramTable (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsEnforcePorMemFreq (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsEnforceStretchMemFreq (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubMemoryTimingPostBefore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubLoadCardSupport (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardDimmMaskOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardBypassS3Registers (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );


IDS_STATUS
IdsIntSubLoadCardBeforeS3Save (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardAfterS3Save (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardBeforeS3Restore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardAfterDqsTraining (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardBeforeDramInit (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardBeforeMemFreqChg (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );


IDS_STATUS
IdsSubMemErrorRecovery (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubMemRetrainTimes (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubMemMrlRetrainTimes (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSubIgnoreMemError (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardMemErrorRecovery (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardBeforePlatTables (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardAfterPlatTables (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardBypassHook (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardMemMappingOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardAfterDctPhyAccess (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardForcePhyToM0 (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntHdtBreakPointBeforeDramInit (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntHdtBreakPointBeforeMemClr (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardAfterPciRegRestore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntSubLoadCardPlatCfgOverride (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsFeatHdtBreakPointBfGen2Init (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsFeatHdtBreakPointBfGppTraining (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsFeatHdtBreakPointBfReConfiguration (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsFeatHdtBreakPointBfGen3Init (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsFeatHdtBreakPointBfPowerGating (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

AGESA_STATUS
AmdGetIdsInternalHeapTable (
  IN OUT   VOID **IdsInternalTable,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
AmdGetIdsMemTable (
  IN OUT   VOID **IdsMemTable,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

IDS_STATUS
IdsEnforceVddIo (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsMemPowerPolicy (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsSkipFusedMaxDdrRate (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

IDS_STATUS
IdsIntRedirectShutDownToHdt (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  );

typedef VOID F_IDS_SET_CPBDISREG (
  IN       UINT32 *CpbDis,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  );

typedef F_IDS_SET_CPBDISREG *PF_IDS_SET_CPBDISREG;


VOID
IdsIntSubSetCpbDisRegPerCode (
  IN       PF_IDS_SET_CPBDISREG CpbSetCpbDisFunc,
  IN       UINT32 *CpbDisRegList,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
);


/// Enum for hdt breakpoint
typedef enum {
  IDS_HDTBREAKPOINT_DIS,    ///< 0 disable
  IDS_HDTBREAKPOINT_EN    ///< 1 enable
} IDS_HDTBREAKPOINT_OPTIONS;

/// Enum for Channel Interleave F10
typedef enum {
  IDS_CHANNEL_INTERLEAVE_F10_DIS = 0,   ///< 0 disable
  IDS_CHANNEL_INTERLEAVE_F10_ADDR6 = 1,   ///< 1 Address bit 6
  IDS_CHANNEL_INTERLEAVE_F10_ADDR12 = 3,   ///< 3 Address bit 12
  IDS_CHANNEL_INTERLEAVE_F10_XORADDR6 = 5,   ///< 5 Xor Addr bits[20:16,6]
  IDS_CHANNEL_INTERLEAVE_F10_XORADDR9 = 7   ///< 7 Xor Addr bits[20:16,9]
} IDS_CHANNEL_INTERLEAVE_F10_OPTIONS;

/// Enum for Bank Swizzle Mode
typedef enum {
  IDS_BANK_SWIZZLE_MODE_DIS = 0, ///< Disabled
  IDS_BANK_SWIZZLE_MODE_EN = 1, ///< Enable
  IDS_BANK_SWIZZLE_MODE_AUTO = 3, ///< Auto
} IDS_BANK_SWIZZLE_MODE_OPTIONS;

/// Enum for Redirect PowerDown to HDT
typedef enum {
  IDS_REDIRECT_SHUTDOWN_TO_HDT_AUTO = 0, ///< Auto
  IDS_REDIRECT_SHUTDOWN_TO_HDT_EN = 1 ///< Enable
} IDS_REDIRECT_SHUTDOWN_TO_HDT_OPTIONS;

/// Enum for Debug Control LBR
typedef enum {
  IDS_DEBUG_CTRL_LBR_DIS = 0, ///< Disable
  IDS_DEBUG_CTRL_LBR_EN = 1, ///< Enable
  IDS_DEBUG_CTRL_LBR_AUTO = 3, ///< Auto
} IDS_DEBUG_CTRL_LBR_OPTIONS;

///DramTermDyn
///This specifies the programming of the DRAM dynamic termination value for writes when the MRS command is issued to configure MR2 during DDR3 DRAM initialization. D18F2x[1,0]84 [11:10]
typedef enum {
  IDS_NV_AMDDRAMTERMDYN_DISABLED = 0,              ///< Disabled
  IDS_NV_AMDDRAMTERMDYN_RZQ_4 = 1,              ///< RZQ/4
  IDS_NV_AMDDRAMTERMDYN_RZQ_2 = 2,              ///< RZQ/2
  IDS_NV_AMDDRAMTERMDYN_AUTO = 7,              ///< Auto
} IDS_NV_AMDDRAMTERMDYN;


///DramTerm
///This specifies the programming of the DRAM nominal termination value when the MRS command is issued to configure MR1 during DDR3 DRAM initialization. D18F2x[1,0]84 [9:7]
typedef enum {
  IDS_NV_AMDDRAMTERM_DISABLED = 0,              ///< Disabled
  IDS_NV_AMDDRAMTERM_RZQ_4 = 1,              ///< RZQ/4
  IDS_NV_AMDDRAMTERM_RZQ_2 = 2,              ///< RZQ/2
  IDS_NV_AMDDRAMTERM_RZQ_6 = 3,              ///< RZQ/6
  IDS_NV_AMDDRAMTERM_RZQ_12 = 4,              ///< RZQ/12
  IDS_NV_AMDDRAMTERM_RZQ_8 = 5,              ///< RZQ/8
  IDS_NV_AMDDRAMTERM_AUTO = 7,              ///< Auto
} IDS_NV_AMDDRAMTERM;

#define NUM_AGESA_IDS_NV_CPB_BOOST_DIS_CORE 0

#endif // _IDS_INTERNAL_LIB_H_
