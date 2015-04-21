/* $NoKeywords:$ */
/**
 * @file
 *
 * IdsInternal.c
 *
 * Contains AMD AGESA Internal library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project: (Proc/Ids/Internal)
 * @e \$Revision: 313864 $   @e \$Date: 2015-03-02 15:15:13 +0800 (Mon, 02 Mar 2015) $
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
#include "Ids.h"
#include "IdsLib.h"
#include "IdsInternal.h"
#include "IdsInternalLib.h"
#include "Topology.h"
#include "amdlib.h"
#include "mm.h"
#include "mn.h"
#include "mfStandardTraining.h"
#include "S3.h"
#include "mfs3.h"
#include "cpuRegisters.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuApComm.h"
#include "OptionApComm.h"
#include "cpuPostInit.h"
#include "heapManager.h"
#include "cpuServices.h"
#include "CommonReturns.h"

///Please check IdsInternalLib.h for internal FILECODE
#define FILECODE PROC_IDS_INTERNAL_IDSINTERNAL_FILECODE

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;
extern OPTION_AP_COMMUNICATE_CONFIGURATION OptionApCommConfiguration;

/**
 *
 * IEM SUPPORT
 *
 * IEM Level Table - This is the table that is checked by the IEM macros to
 * determine if a particular level of IEM support is enabled in the current build.
 *
 * When adding a new level of support, add it to this table.
 *
 *
 **/
CONST BOOLEAN IemOptSkipLevels[] = {
  IEMOPT_WAIT,                  // Controls insert/skip of wait functions
  IEMOPT_POLL,                  // Controls insert/skip of Poll functions
  IEMOPT_RESET,                 // Controls insert/skip of Reset functions
  IEMOPT_MEM_RESTORE,           // Controls insert/skip of Mem Restore functions
  IEMOPT_EARLY_MAX_RD_LAT,      // Controls insert/skip of early max rd latency control functions
  IEMOPT_NBBLOCK_INIT,          // Controls override NBBLOCK values
  IEMOPT_DCT_INIT,              // Controls override of DCT Init values
  IEMOPT_BEFORE_DRAM_INIT,      // Controls override of Before DRAM Init values
  IEMOPT_EARLY_DCT_CONFIG,      // Controls Early DCT Configuration
  IEMOPT_EARLY_DEVICE_INIT,     // Controls early configuration of Memory Devices
  IEMOPT_LATE_DCT_CONFIG,       // Controls Late DCT configuration
  IEMOPT_LATE_MAX_RD_LAT,       // Controls insert/skip of late max rd latency control functions
  IEMOPT_AFTER_EXTRACT_G5_SPD,  // Controls After extracting G5 SPD
  IEMOPT_BEFORE_PMU_MSG_BLOCK_WRITE, // Controls override of PMU MSG BLOCK values
  IEMOPT_EVENT_LOG,             // Controls skip event log
  IEMOPT_CPU_POWER_MANAGER_TABLE,    // Control skip CPU power manager table
  IEMOPT_GNB_ACG_CONFIG,        // Controls skip ACG configuration
};

/**
 *
 *  Get Ids Internal table pointer in the AGESA Heap.
 *
 *  @param[in,out] IdsInternalTable  The Pointer of IDS Internal Table.
 *  @param[in,out] StdHeader    The Pointer of Standard Header.
 *
 *  @retval AGESA_SUCCESS       Success to get the pointer of Mem Table.
 *  @retval AGESA_ERROR         Fail to get the pointer of Mem Table.
 *
 **/
AGESA_STATUS
AmdGetIdsInternalHeapTable (
  IN OUT   VOID **IdsInternalTable,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  AGESA_STATUS status;
  LOCATE_HEAP_PTR LocateHeapStructPtr;
  IDS_CONTROL_STRUCT *IdsCtrlPtr;

  LocateHeapStructPtr.BufferHandle = IDS_CONTROL_HANDLE;
  LocateHeapStructPtr.BufferPtr = NULL;
  status = HeapLocateBuffer (&LocateHeapStructPtr, StdHeader);
  if (status == AGESA_SUCCESS) {
    IdsCtrlPtr = (IDS_CONTROL_STRUCT *) LocateHeapStructPtr.BufferPtr;
    *IdsInternalTable = LocateHeapStructPtr.BufferPtr + IdsCtrlPtr->IdsExtendOffset;
  }
  return status;
}

/**
 *  IDS Backend Function for PSS
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsSubPss (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS Status;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  IDS_NV_READ_SKIP (Status, AGESA_IDS_NV_ACPI_PSS, IdsNvPtr, StdHeader) {

    if (Status == (IDS_STATUS) 0x0) {
    //Disable
      PlatformConfig->UserOptionPState = FALSE;
    } else if (Status == (IDS_STATUS) 0x1) {
    //Enable
      PlatformConfig->UserOptionPState = TRUE;
    }
  }
  return Status;
}

/**
 *  IDS Backend Function for CST
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      IDS_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsSubCst (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS Status;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  IDS_NV_READ_SKIP (Status, AGESA_IDS_NV_ACPI_CST, IdsNvPtr, StdHeader) {
    // Only can be disabled, otherwise do nothing
    if (Status == (IDS_STATUS) 0x0) {
      PlatformConfig->CStateIoBaseAddress = 0;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for WHEA
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsSubWhea (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS Status;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  IDS_NV_READ_SKIP (Status, AGESA_IDS_NV_ACPI_WHEA, IdsNvPtr, StdHeader) {
    if (Status == 0) {
      PlatformConfig->UserOptionWhea = FALSE;
    } else {
      PlatformConfig->UserOptionWhea = TRUE;
    }
  }
  return Status;
}

/**
 *  IDS Backend Function for DMI
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsSubDmi (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS Status;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  IDS_NV_READ_SKIP (Status, AGESA_IDS_NV_DMIEN, IdsNvPtr, StdHeader) {
    if (Status == 0) {
      PlatformConfig->UserOptionDmi = FALSE;
    } else {
      PlatformConfig->UserOptionDmi = TRUE;
    }
  }
  return Status;
}
/**
 *  IDS Backend Function for PSI
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsSubPsi (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS Status;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  IDS_NV_READ_SKIP (Status, AGESA_IDS_NV_PSIEN, IdsNvPtr, StdHeader) {
    if (Status == 0) {
      PlatformConfig->VrmProperties[CoreVrm].LowPowerThreshold = (UINT32) Status;
    }
  }
  return Status;
}


/**
 *  IDS Backend Function for DCT configuration control
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_POST_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubDctConfigPostBefore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AMD_POST_PARAMS *PostParamsPtr;
  MEM_PARAMETER_STRUCT *RefPtr;
  IDS_STATUS idsvalue;
  MEM_DATA_STRUCT * memdataptr;

  PostParamsPtr = (AMD_POST_PARAMS *)DataPtr;
  memdataptr = PostParamsPtr->MemConfig.MemData;
  RefPtr = memdataptr->ParameterListPtr;

  idsvalue =  AmdIdsNvReader (AGESA_IDS_NV_MEMORY_PARITY, IdsNvPtr, StdHeader);
  if (idsvalue < (IDS_STATUS)0x2) {
    RefPtr->EnableParity = (BOOLEAN) idsvalue;
  }

  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_MemClr, IdsNvPtr, StdHeader) {
    RefPtr->EnableMemClr = (BOOLEAN) idsvalue;
  }

  //
  // ReceiverPowerMode. Work around for DDR PHY bug.  ENH391725
  //
  idsvalue =  AmdIdsNvReader (AGESA_IDS_NV_RECEIVER_POWER_MODE, IdsNvPtr, StdHeader);
  if (!idsvalue) {
    memdataptr->PhyReceiverLowPower = 1;
  } else {
    memdataptr->PhyReceiverLowPower = 0;
  }

  //
  // Setting up PMU Training Mode from Setup Option through IDS
  //
  idsvalue =  AmdIdsNvReader (AGESA_IDS_NV_PMU_TRAIN_MODE, IdsNvPtr, StdHeader);
  switch (idsvalue) {
  case 0:
    RefPtr->PmuTrainMode = PMU_TRAIN_1D;
    break;
  case 1:
    RefPtr->PmuTrainMode = PMU_TRAIN_1D_2D_READ;
    break;
  case 2:
    RefPtr->PmuTrainMode = PMU_TRAIN_1D_2D;
    break;
  case 3:    // This case is equivalent to value of Token AUTO from CBS
    RefPtr->PmuTrainMode = UserOptions.CfgPmuTrainMode;  // PmuTraining Mode from BLDCFG
    break;
  }
  return IDS_SUCCESS;
}


/**
 *
 *  Get Ids Memory table pointer in the AGESA Heap.
 *
 *  @param[in,out] IdsMemTable  The Pointer of IDS Memory Table.
 *  @param[in,out] StdHeader    The Pointer of Standard Header.
 *
 *  @retval AGESA_SUCCESS       Success to get the pointer of Mem Table.
 *  @retval AGESA_ERROR         Fail to get the pointer of Mem Table.
 *
 **/
AGESA_STATUS
AmdGetIdsMemTable (
  IN OUT   VOID **IdsMemTable,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  AGESA_STATUS status;
  LOCATE_HEAP_PTR LocateHeapStructPtr;
  IDS_CONTROL_STRUCT *IdsCtrlPtr;

  LocateHeapStructPtr.BufferHandle = IDS_CONTROL_HANDLE;
  LocateHeapStructPtr.BufferPtr = NULL;
  status = HeapLocateBuffer (&LocateHeapStructPtr, StdHeader);
  if (status == AGESA_SUCCESS) {
    IdsCtrlPtr = (IDS_CONTROL_STRUCT *) LocateHeapStructPtr.BufferPtr;
    *IdsMemTable = LocateHeapStructPtr.BufferPtr + IdsCtrlPtr->IdsMemTableOffset;
  }
  return status;
}

/**
 *  IDS Backend Function for Option Hook Point: IDS_GET_DRAM_TABLE
 *
 *  This function is used to get the pointer of DRAM override table.
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         The Dram table havn't been init.
 *  @retval IDS_UNSUPPORTED     The Dram table have been init.
 *
 **/
IDS_STATUS
IdsSubGetDramTable (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT16 *pidsmemheap;
  MEM_TABLE_ALIAS **memtable;
  AGESA_STATUS astatus;
  IDS_STATUS istatus;
  pidsmemheap = NULL;
  memtable = (MEM_TABLE_ALIAS **) DataPtr;
  *memtable = NULL;
  astatus = AmdGetIdsMemTable (&pidsmemheap, StdHeader);
  istatus = IDS_SUCCESS;
  if ((astatus == AGESA_SUCCESS) && (pidsmemheap != NULL)) {
    if (*pidsmemheap == 0x55AA) {
      *memtable = (MEM_TABLE_ALIAS *) (pidsmemheap + 1);
      istatus = IDS_UNSUPPORTED;
    } else {
      *memtable = NULL;
      istatus = IDS_SUCCESS;
    }
  }
  return istatus;
}

/**
 *  IDS Backend Function for Memory Error Recovery
 *
 *  This function is used to override Memory Error Recovery.
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubMemErrorRecovery (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS ErrorRecovery;
  IDS_STATUS LcMode;
  IDS_NV_READ_SKIP (ErrorRecovery, AGESA_IDS_NV_MEM_ERROR_RECOVERY, IdsNvPtr, StdHeader) {
    *((BOOLEAN *)DataPtr) = (BOOLEAN)ErrorRecovery;
  }
  // if load card is active, always enable error recovery
  IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if (LcMode != (IDS_STATUS)0) {
      *((BOOLEAN *)DataPtr) = FALSE;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for special handling or Memory Error Recovery
 *  when user wants to ignore error and continue with the initialization
 *  process.
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubIgnoreMemError (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IgnoreError;
  IDS_STATUS LcModeActive;
  IDS_NV_READ_SKIP (IgnoreError, AGESA_IDS_NV_IGNORE_MEM_ERR, IdsNvPtr, StdHeader) {
    if (IgnoreError == (IDS_STATUS)1) {
      *((BOOLEAN *) DataPtr) = TRUE;
    }
  }

  // Force ignore error for load card support
  IDS_NV_READ_SKIP (LcModeActive, AGESA_IDS_NV_LOAD_CARD_ACTIVE, IdsNvPtr, StdHeader) {
    if (LcModeActive != (IDS_STATUS)0) {
      *((BOOLEAN *) DataPtr) = TRUE;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Memory Training Times
 *
 *  This function is used to override Memory Training Times.
 *
 *  @param[in,out]   DataPtr      The Pointer of UINT8.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubMemRetrainTimes (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS RetrainTimes;
  IDS_NV_READ_SKIP (RetrainTimes, AGESA_IDS_NV_MEM_RETRAIN_TIMES, IdsNvPtr, StdHeader) {
    *((UINT8 *)DataPtr) = (UINT8)RetrainTimes;
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Memory MRL Training Times
 *
 *  This function is used to override Memory MRL Training Times.
 *
 *  @param[in,out]   DataPtr      The Pointer of UINT8.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSubMemMrlRetrainTimes (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS MrlRetrainTimes;
  IDS_NV_READ_SKIP (MrlRetrainTimes, AGESA_IDS_NV_MEM_MRL_RETRAIN_TIMES, IdsNvPtr, StdHeader) {
    if (MrlRetrainTimes != 0xFF) {
      *((UINT8 *)DataPtr) = (UINT8)MrlRetrainTimes;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Option Hook Point IDS_POR_MEM_FREQ
 *
 *  This function is used to skip MemClk frequency enforcement if it is asked to.
 *
 *  @param[in,out]   DataPtr      The Pointer to NB block.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsEnforcePorMemFreq (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS PorMemFreq;

  IDS_NV_READ_SKIP (PorMemFreq, AGESA_IDS_NV_POR_MEM_FREQ, IdsNvPtr, StdHeader) {
    //if Enforce support frequency select to disable skip
    if (PorMemFreq == (IDS_STATUS) 1) {
      return IDS_UNSUPPORTED;
    } else {
      return IDS_SUCCESS;
    }
  }

  return IDS_UNSUPPORTED;
}

/**
 *  IDS Backend Function for Option Hook Point IDS_STRETCH_FREQUENCY_LIMIT
 *
 *  This function is used to enforce stretch memory frequency limit.
 *
 *  @param[in,out]   DataPtr      The Pointer to NB block.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsEnforceStretchMemFreq (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  MEM_NB_BLOCK *NBPtr;

  NBPtr = (MEM_NB_BLOCK *) DataPtr;
  if (IdsEnforcePorMemFreq (DataPtr, StdHeader, IdsNvPtr) == IDS_UNSUPPORTED) {
    NBPtr->PsPtr->MemPGetPORFreqLimit (NBPtr);
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Memory timing Control of post before
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubMemoryTimingPostBefore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AMD_POST_PARAMS *PostParamsPtr;
  MEM_PARAMETER_STRUCT *RefPtr;
  IDS_STATUS idsvalue;
  MEM_DATA_STRUCT * memdataptr;
  UINT32 membusspeed;

  PostParamsPtr = (AMD_POST_PARAMS *)DataPtr;
  memdataptr = PostParamsPtr->MemConfig.MemData;
  RefPtr = memdataptr->ParameterListPtr;
  membusspeed = DDR400_FREQUENCY;

  idsvalue =  AmdIdsNvReader (AGESA_IDS_NV_DRAMTIME, IdsNvPtr, StdHeader);
  if (idsvalue == (IDS_STATUS)0x1) {
    idsvalue =  AmdIdsNvReader (AGESA_IDS_NV_MEMSPEED, IdsNvPtr, StdHeader);
    if (idsvalue < (IDS_STATUS)IDS_MEMORYCLOCK_SPEED_END) {
      switch (idsvalue) {
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_400MHZ:
        membusspeed = DDR400_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_533MHZ:
        membusspeed = DDR533_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_667MHZ:
        membusspeed = DDR667_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_800MHZ:
        membusspeed = DDR800_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_1066MHZ:
        membusspeed = DDR1066_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_1333MHZ:
        membusspeed = DDR1333_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_1600MHZ:
        membusspeed = DDR1600_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_1866MHZ:
        membusspeed = DDR1866_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_2100MHZ:
        membusspeed = DDR2100_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_2133MHZ:
        membusspeed = DDR2133_FREQUENCY;
        break;
      case (IDS_STATUS) IDS_MEMORYCLOCK_SPEED_2400MHZ:
        membusspeed = DDR2400_FREQUENCY;
        break;
      default:
        break;
      }
      RefPtr->UserTimingMode = TIMING_MODE_SPECIFIC;
      RefPtr->MemClockValue =  membusspeed;
    }
  }
  return IDS_SUCCESS;
}
/**
 *  IDS Backend Function for Hdt breakpoint before Dram init
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntHdtBreakPointBeforeDramInit (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_HDTBREAKPOINT_BFDRAMINIT, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Hdt breakpoint before set MemClr
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntHdtBreakPointBeforeMemClr (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_HDTBREAKPOINT_BFMEMCLR, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Hdt breakpoint Before Gen2 Init
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsFeatHdtBreakPointBfGen2Init (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_BEFORE_GEN2_INIT, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Hdt breakpoint Before Gpp training
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsFeatHdtBreakPointBfGppTraining (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_BEFORE_GPP_TRAINING, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Hdt breakpoint Before Reconfiguration
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsFeatHdtBreakPointBfReConfiguration (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_BEFORE_RECONFIGURATION, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Hdt breakpoint Before Gen3 Init
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsFeatHdtBreakPointBfGen3Init (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_BEFORE_GEN3_INIT, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Hdt breakpoint Before Power Gating
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsFeatHdtBreakPointBfPowerGating (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS IdsVal;

  IDS_NV_READ_SKIP (IdsVal, AGESA_IDS_NV_BEFORE_POWER_GATING, IdsNvPtr, StdHeader) {
    if (IdsVal == IDS_HDTBREAKPOINT_EN) {
      LibAmdHDTBreakPoint ();
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for config CPB feature
 *
 *  @param[in]   CpbSetCpbDisFunc      The Pointer of function CpbDisReg
 *  @param[in]   CpbDisRegList      The Pointer of CpbDisRegList
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsIntSubSetCpbDisRegPerCode (
  IN       PF_IDS_SET_CPBDISREG CpbSetCpbDisFunc,
  IN       UINT32 *CpbDisRegList,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 Core;
  UINT32 Socket;
  UINT32 IgnoredModule;
  UINT32 IgnoredCore;
  UINT32 ActiveCores;
  AP_TASK   TaskPtr;
  AGESA_STATUS IgnoredSts;

  TaskPtr.FuncAddress.PfApTaskI = (PF_AP_TASK_I) CpbSetCpbDisFunc;
  TaskPtr.ExeFlags = WAIT_FOR_CORE;
  TaskPtr.DataTransfer.DataSizeInDwords = SIZE_IN_DWORDS (CpbDisRegList);
  TaskPtr.DataTransfer.DataTransferFlags = 0;

  IdentifyCore (StdHeader, &Socket, &IgnoredModule, &IgnoredCore, &IgnoredSts);
  GetActiveCoresInCurrentSocket (&ActiveCores, StdHeader);
  ASSERT (ActiveCores <= NUM_AGESA_IDS_NV_CPB_BOOST_DIS_CORE);
  //Init CurCpbList with Core 1

  for (Core = 1; Core < (UINT8) ActiveCores; ++Core) {
    TaskPtr.DataTransfer.DataPtr = &CpbDisRegList[Core];
    OptionApCommConfiguration.ApRunCodeOnSocketCore ((UINT8)Socket, (UINT8)Core, &TaskPtr, StdHeader);
  }

  TaskPtr.DataTransfer.DataPtr = &CpbDisRegList[0];
  ApUtilTaskOnExecutingCore (&TaskPtr, StdHeader, NULL);

}

/**
 *  Internal option hook of IDS_INIT_EARLY_AFTER
 *
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubEarlyAfter (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_CALLOUT_STRUCT IdsCalloutData;
  IdsCalloutData.StdHeader = *StdHeader;

  AgesaGetIdsData (IDS_CALLOUT_INT_EARLY_AFTER, &IdsCalloutData);
  return IDS_SUCCESS;
}

/**
 *  Internal option hook of IDS_INIT_EARLY_BEFORE
 *
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubEarlyBefore (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS ForceAssert;

  IDS_NV_READ_SKIP (ForceAssert, AGESA_IDS_NV_FORCE_ASSERT, IdsNvPtr, StdHeader) {
    if (ForceAssert == (IDS_STATUS) IDS_FORCE_ASSERT_ENABLE) {
      ASSERT (FALSE);
    }
  }

  return IDS_SUCCESS;

}

/**
 *  Internal option hook of IDS_INIT_LATE_AFTER
 *
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubLaterAfter (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_CALLOUT_STRUCT IdsCalloutData;
  IdsCalloutData.StdHeader = *StdHeader;

  AgesaGetIdsData (IDS_CALLOUT_INT_LATE_AFTER, &IdsCalloutData);
  return IDS_SUCCESS;
}

/**
 *
 *
 *     Internal hook routine for IDS dispatcher
 *
 *     @param[in]   IdsOption   - option of IDS_OPTION_HOOK
 *     @param[in,out]   DataPtr   -   The Pointer of input parameter
 *     @param[in]     IdsNvPtr   -  The Pointer of NV Table.
 *     @param[in,out]   StdHeader    - The Pointer of AGESA Header
 *
 *     @retval IDS_SUCCESS         Backend function is called successfully.
 *     @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 */
IDS_STATUS
IdsInternalOptionHook (
  IN       AGESA_IDS_OPTION IdsOption,
  IN OUT   VOID *DataPtr,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  IdsBvmOptionHook (IdsOption, DataPtr, StdHeader);
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Option Hook Point IDS_ENFORCE_VDDIO
 *
 *  This function is used to enforce VDDIO on the board.
 *
 *  @param[in,out]   DataPtr      The Pointer to VDDIO value.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsEnforceVddIo (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS VDDIO;

  IDS_NV_READ_SKIP (VDDIO, AGESA_IDS_NV_ENFORCE_VDDIO, IdsNvPtr, StdHeader) {
    if ((VDDIO == (IDS_STATUS) VOLT1_5_ENCODED_VAL) || (VDDIO == (IDS_STATUS) VOLT1_35_ENCODED_VAL) || (VDDIO == (IDS_STATUS) VOLT1_25_ENCODED_VAL)) {
      * (DIMM_VOLTAGE *) DataPtr = CONVERT_ENCODED_TO_VDDIO (VDDIO, DDR3_TECHNOLOGY);
      return IDS_SUCCESS;
    }
  }

  return IDS_UNSUPPORTED;
}

/**
 *  IDS Backend Function to skip performance optimization based on VDDIO
 *
 *  This function is used to skip performance optimization when POR memory frequency
 *  is not enforced.
 *
 *  @param[in,out]   DataPtr      The Pointer to PowerPolicy value.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsMemPowerPolicy (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS MemPowerPolicy;

  IDS_NV_READ_SKIP (MemPowerPolicy, AGESA_IDS_NV_MEMORY_POWER_POLICY, IdsNvPtr, StdHeader) {
    if ((MemPowerPolicy == (IDS_STATUS) Performance) || (MemPowerPolicy == (IDS_STATUS) BatteryLife)) {
      * (MEMORY_POWER_POLICY *) DataPtr = (MEMORY_POWER_POLICY) MemPowerPolicy;
      return IDS_SUCCESS;
    }
  }

  return IDS_UNSUPPORTED;
}

/**
 *  IDS Backend Function to skip frequency cap of fused MaxDdrRate
 *
 *  This function is used to skip frequency cap of fused MaxDdrRate
 *
 *  @param[in,out]   DataPtr      The Pointer to PowerPolicy value.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsSkipFusedMaxDdrRate (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS idsvalue;

  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_DRAMTIME, IdsNvPtr, StdHeader) {
    // If Dram Timing is set to Manual
    if (idsvalue == (IDS_STATUS)0x1) {
      idsvalue =  AmdIdsNvReader (AGESA_IDS_NV_MEMSPEED, IdsNvPtr, StdHeader);
      // If specified dram frequency is valid
      if (idsvalue < (IDS_STATUS)IDS_MEMORYCLOCK_SPEED_END) {

        *(UINT16 *) DataPtr = 16000;
      }
    }
    return IDS_SUCCESS;
  }
  return IDS_UNSUPPORTED;
}

/**
 *  IDS Backend Function to Enable Redirect Shutdown to HDT
 *
 *  This function is used to Enable Redirect Shutdown to HDT
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntRedirectShutDownToHdt (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS idsvalue;
  UINT64 U64tmp;
  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_REDIRECT_SHUTDOWN_TO_HDT, IdsNvPtr, StdHeader) {
    switch (idsvalue) {
    case IDS_REDIRECT_SHUTDOWN_TO_HDT_EN:
      LibAmdMsrRead (0xC001100A, &U64tmp, StdHeader);
      U64tmp |= BIT20;
      LibAmdMsrWrite (0xC001100A, &U64tmp, StdHeader);
      break;
    case IDS_REDIRECT_SHUTDOWN_TO_HDT_AUTO:
      //Do nothing
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_REDIRECT_INT1_INT3_TO_HDT, IdsNvPtr, StdHeader) {
    switch (idsvalue) {
    case IDS_REDIRECT_SHUTDOWN_TO_HDT_EN:
      LibAmdMsrRead (0xC001100A, &U64tmp, StdHeader);
      U64tmp |= BIT0;
      LibAmdMsrWrite (0xC001100A, &U64tmp, StdHeader);
      break;
    case IDS_REDIRECT_SHUTDOWN_TO_HDT_AUTO:
      //Do nothing
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_DEBUGCTRL_LBR, IdsNvPtr, StdHeader) {
    switch (idsvalue) {
    case IDS_DEBUG_CTRL_LBR_DIS:
    case IDS_DEBUG_CTRL_LBR_EN:
      LibAmdMsrRead (0x1D9, &U64tmp, StdHeader);
      U64tmp &= ~(BIT0);
      U64tmp |= (idsvalue & 0x1);
      LibAmdMsrWrite (0x1D9, &U64tmp, StdHeader);
      break;
    case IDS_DEBUG_CTRL_LBR_AUTO:
      //Do nothing
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Option Hook Point IDS_MEM_DRAM_TERM
 *
 *  This function is used to override "Dram Term"
 *
 *  @param[in,out]   DataPtr      The Pointer to RttNom.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntDramTerm (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT8 *RttNom;
  IDS_STATUS NvValue;

  RttNom = (UINT8 *) DataPtr;
  //DramTerm
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMTERM, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IDS_NV_AMDDRAMTERM_DISABLED:
    case IDS_NV_AMDDRAMTERM_RZQ_4:
    case IDS_NV_AMDDRAMTERM_RZQ_2:
    case IDS_NV_AMDDRAMTERM_RZQ_6:
    case IDS_NV_AMDDRAMTERM_RZQ_12:
    case IDS_NV_AMDDRAMTERM_RZQ_8:
      *RttNom = (UINT8) NvValue;
      IDS_HDT_CONSOLE (MEM_FLOW, "\tIDS Set RttNom = %x\n", *RttNom);
      break;
    //Auto
    case  IDS_NV_AMDDRAMTERM_AUTO:
    default:
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Option Hook Point IDS_MEM_DYN_DRAM_TERM
 *
 *  This function is used to override "Dynamic Dram Term"
 *
 *  @param[in,out]   DataPtr      The Pointer to RttWr.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntDynDramTerm (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT8 *RttWr;
  IDS_STATUS NvValue;

  RttWr = (UINT8 *) DataPtr;

  //DramTermDyn
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMTERMDYN, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IDS_NV_AMDDRAMTERMDYN_DISABLED:
    case IDS_NV_AMDDRAMTERMDYN_RZQ_4:
    case IDS_NV_AMDDRAMTERMDYN_RZQ_2:
      *RttWr = (UINT8) NvValue;
      IDS_HDT_CONSOLE (MEM_FLOW, "\tIDS Set RttWr = %x\n", *RttWr);
      break;
    //Auto
    case  IDS_NV_AMDDRAMTERMDYN_AUTO:
    default:
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  Get internal UCODE base which store inside of BVM binary
 *
 *  Will update the pUcodePtr, if found the Ucode in BVM
 *  @param[in,out]   pUcodePtr    Address of pointer which used to store Ucode address
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsGetBvmUcodeBase (
  IN OUT   UINT8 ** pUcodePtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 *ucodePtr;
  if (IdsBvmGetExtendModuleBaseViaType (BVM_EXTEND_MODULE_TYPE_UCODE, &ucodePtr, StdHeader) == IDS_SUCCESS) {
    *pUcodePtr = (UINT8 *) ((BVM_EXTEND_UCODE_HEADER *)ucodePtr + 1);
  }
}

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntBlockEarlyAfter =
{
  IDS_FEAT_COMMON,
  IDS_BSP_ONLY,
  IDS_INIT_EARLY_AFTER,
  IDS_FAMILY_ALL,
  IdsIntSubEarlyAfter
};


CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntBlockLaterAfter =
{
  IDS_FEAT_COMMON,
  IDS_BSP_ONLY,
  IDS_INIT_LATE_AFTER,
  IDS_FAMILY_ALL,
  IdsIntSubLaterAfter
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntEarlyBefore =
{
  IDS_FEAT_COMMON,
  IDS_BSP_ONLY,
  IDS_INIT_EARLY_AFTER,
  IDS_FAMILY_ALL,
  IdsIntSubEarlyBefore
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryDramTermBlock =
{
  IDS_FEAT_COMMON,
  IDS_ALL_CORES,
  IDS_MEM_DRAM_TERM,
  IDS_FAMILY_ALL,
  IdsIntDramTerm
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryDynDramTermBlock =
{
  IDS_FEAT_COMMON,
  IDS_ALL_CORES,
  IDS_MEM_DYN_DRAM_TERM,
  IDS_FAMILY_ALL,
  IdsIntDynDramTerm
};
