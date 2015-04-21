/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Carrizo boot time calibration code
 *
 * Runs workloads and updates hardware with results.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 314991 $   @e \$Date: 2015-03-18 13:05:34 +0800 (Wed, 18 Mar 2015) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuBtc.h"
#include "cpuF15CzBtc.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15CzPowerMgmt.h"
#include "cpuF15Utilities.h"
#include "cpuFeatures.h"
#include "cpuCpb.h"
#include "cpuLateInit.h"
#include "heapManager.h"
#include "CommonReturns.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZBTC_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define SCRATCH_BUFFER_SIZE_PER_CU    538816
#define XMM_SAVE_BUFFER_SIZE_PER_CORE (256 + 8)
#define LOOP_COUNT                    1

extern
VOID
ExecuteF15CzBtc (
  IN       UINT64  CalPgmIndex,
  IN       VOID    *Scratch
  );

extern
VOID
SaveXmmRegisters (
  IN       UINT8   *XmmSaveBuffer
  );

extern
VOID
RestoreXmmRegisters (
  IN       UINT8   *XmmSaveBuffer
  );
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

UINT8
cpuF15CzGetTargetPstate (
  IN       UINT64              TestNumber,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

VOID
cpuF15CzExecuteBTC (
  IN       BTC_BUFFER          *BtcBufferPtr,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

AGESA_STATUS
STATIC
cpuF15CzBtcBefore (
  IN       BTC_FAMILY_SERVICES *BtcServices,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

VOID
STATIC
cpuF15CzBtcRun (
  IN      BTC_FAMILY_SERVICES *BtcServices,
  IN      UINT8               CalPgmIndex,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  );

VOID
STATIC
cpuF15CzBtcAfter (
  IN      BTC_FAMILY_SERVICES *BtcServices,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                          GLOBAL DATA
 *----------------------------------------------------------------------------------------
 */

#ifndef __PRE_RAM__
STATIC UINT8          OkToStartTest;
#endif
STATIC BTC_BUFFER     BtcBufferPtr;

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Cpu entry point
 *
 * @param[in]  BtcServices     The current CPU's family services.
 * @param[in]  CalPgmIndex     Test index
 * @param[in]  StdHeader       Standard configuration header
 */
VOID
STATIC
cpuF15CzBtcRun (
  IN      BTC_FAMILY_SERVICES *BtcServices,
  IN      UINT8               CalPgmIndex,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8                 TargetPstate;
  ALLOCATE_HEAP_PARAMS  AllocateHeapParams;
  LOCATE_HEAP_PTR       LocateHeapPtr;
  UINTN                 ComputeUnitCount;
  UINTN                 ActualCoreCount;

  // Get scratch buffer
  LocateHeapPtr.BufferHandle = AMD_BTC_SCRATCH_HANDLE;
  if (HeapLocateBuffer (&LocateHeapPtr, StdHeader) != AGESA_SUCCESS) {
    ComputeUnitCount = GetNumberOfCompUnitsInCurrentModule (StdHeader);
    AllocateHeapParams.RequestedBufferSize = (UINT32) (SCRATCH_BUFFER_SIZE_PER_CU * ComputeUnitCount);
    AllocateHeapParams.BufferHandle = AMD_BTC_SCRATCH_HANDLE;
    AllocateHeapParams.Persist = 0;
    if (HeapAllocateBuffer (&AllocateHeapParams, StdHeader) != AGESA_SUCCESS) {
      return;
    } else {
      BtcBufferPtr.Scratch = (VOID *) AllocateHeapParams.BufferPtr;
    }
  } else {
    BtcBufferPtr.Scratch = (VOID *) LocateHeapPtr.BufferPtr;
  }
  // Get XMM save buffer
  LocateHeapPtr.BufferHandle = AMD_BTC_XMM_SAVE_HANDLE;
  if (HeapLocateBuffer (&LocateHeapPtr, StdHeader) != AGESA_SUCCESS) {
    ActualCoreCount = GetActiveCoresInCurrentModule (StdHeader);
    AllocateHeapParams.RequestedBufferSize = (UINT32) (XMM_SAVE_BUFFER_SIZE_PER_CORE * ActualCoreCount);
    AllocateHeapParams.BufferHandle = AMD_BTC_XMM_SAVE_HANDLE;
    AllocateHeapParams.Persist = 0;
    if (HeapAllocateBuffer (&AllocateHeapParams, StdHeader) != AGESA_SUCCESS) {
      return;
    } else {
      BtcBufferPtr.XmmSave = (VOID *) AllocateHeapParams.BufferPtr;
    }
  } else {
    BtcBufferPtr.XmmSave = (VOID *) LocateHeapPtr.BufferPtr;
  }
  // CalPgmIndex
  BtcBufferPtr.TestNumber = (UINT64) CalPgmIndex;

  AGESA_TESTPOINT (TpProcCpuRunBtc, StdHeader);

#ifndef __PRE_RAM__
  InterlockedIncrement ((UINT32 *)&OkToStartTest);
#endif
  TargetPstate = cpuF15CzGetTargetPstate (BtcBufferPtr.TestNumber, StdHeader);
  F15TransitionPstate (NULL, TargetPstate, (BOOLEAN) TRUE, StdHeader);
  cpuF15CzExecuteBTC (&BtcBufferPtr, StdHeader);
#ifndef __PRE_RAM__
  InterlockedDecrement ((UINT32 *)&OkToStartTest);
#endif
  AGESA_TESTPOINT (TpProcCpuEndBtc, StdHeader);

  if (CalPgmIndex == 2) {
    HeapDeallocateBuffer (AMD_BTC_SCRATCH_HANDLE, StdHeader);
    HeapDeallocateBuffer (AMD_BTC_XMM_SAVE_HANDLE, StdHeader);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Work procedure for the Aps.
 *
 * This procedure will perform the following steps :
 * 1..Set Ap to P1 (in effect hP1 as boost is disabled)
 * 2..Wait for semaphore to flag run tests.
 * 3..Transfer to a target Pstate
 * 4..Run boot time calibration workloads
 * 6..Wait for Bsp to signal it has finished workloads and housekeeping
 * 6..If tests completed, wait for semaphore to signal transition Pstate and exit, else...
 *    ... wait for semaphore to signal run next workload.
 *
 * @param[in] ApExeParams       Ap parameters pointer
 */
VOID
cpuF15CzBTCLaunchAps (
  IN       AP_EXE_PARAMS *ApExeParams
  )
{
  UINT8   TargetPstate;

  TargetPstate = cpuF15CzGetTargetPstate (0, &ApExeParams->StdHeader);
  F15TransitionPstate (NULL, TargetPstate, (BOOLEAN) TRUE, &ApExeParams->StdHeader);

  for (;;) {
#ifndef __PRE_RAM__
    while (OkToStartTest == 0) {
      LibAmdCpuPause ();
    }
#endif
// Don't run workload for now
//    TargetPstate = cpuF15CzGetTargetPstate (BtcBufferPtr.TestNumber, &ApExeParams->StdHeader);
//    F15TransitionPstate (NULL, TargetPstate, (BOOLEAN) TRUE, &ApExeParams->StdHeader);
//    cpuF15CzExecuteBTC (&BtcBufferPtr, &ApExeParams->StdHeader);

//    while (OkToStartTest != 0) {
//      LibAmdCpuPause ();
//    }
//    if (BtcBufferPtr.TestNumber == (NUMBER_OF_CAL_PGMS - 1)) {
//      while (OkToStartTest == 0) {
//        LibAmdCpuPause ();
//      }
//      F15TransitionPstate (NULL, (UINT8) 0, (BOOLEAN) TRUE, &ApExeParams->StdHeader);
//      return;
//    }
    F15TransitionPstate (NULL, (UINT8) 0, (BOOLEAN) FALSE, &ApExeParams->StdHeader);
    return;
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Return a target Pstate which is required for the CalPgmIndex
 *
 * @param[in]  TestNumber       Test Number
 * @param[in]  StdHeader        Standard configuration header
 *
 * @return                      Target Pstate which is required for the CalPgmIndex
 */
UINT8
cpuF15CzGetTargetPstate (
  IN       UINT64              TestNumber,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8  TargetPstate;
  PCI_ADDR PciAddress;
  CPB_CTRL_REGISTER CpbCtrl;
  F15_CZ_CLK_PWR_TIMING_CTRL2_REGISTER Cptc2;

  switch (TestNumber) {
  case 0:
    TargetPstate = 7;
    break;
  case 1:
    TargetPstate = 7;
    break;
  case 2:
    TargetPstate = 7;
    break;
  default:
    TargetPstate = 7;
    ASSERT (FALSE);
  }

  PciAddress.AddressValue = CPTC2_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &Cptc2, StdHeader);
  if (TargetPstate > (UINT8) Cptc2.HwPstateMaxVal) {
    TargetPstate = (UINT8) Cptc2.HwPstateMaxVal;
  };

  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &CpbCtrl, StdHeader);
  TargetPstate = TargetPstate - (UINT8) CpbCtrl.NumBoostStates;

  return (TargetPstate);
}
/*----------------------------------------------------------------------------------------*/
/**
 * Run a workload
 *
 * @param[in]  BtcBufferPtr     Buffer for tests
 * @param[in]  StdHeader        Standard configuration header
 */
VOID
cpuF15CzExecuteBTC (
  IN       BTC_BUFFER          *BtcBufferPtr,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 CoreNum;
  UINT8  *WorkBufferPtr;
  UINT8  *XmmSaveBufferPtr;
  UINT8  i;

  WorkBufferPtr = (UINT8 *) BtcBufferPtr->Scratch;
  GetCurrentCore (&CoreNum, StdHeader);
  if (CoreNum > 1) {
    WorkBufferPtr += SCRATCH_BUFFER_SIZE_PER_CU;
  }
  XmmSaveBufferPtr = (UINT8*) BtcBufferPtr->XmmSave;
  XmmSaveBufferPtr += (CoreNum * XMM_SAVE_BUFFER_SIZE_PER_CORE);

  SaveXmmRegisters (XmmSaveBufferPtr);

  for (i = 0; i < LOOP_COUNT; i++) {
    ExecuteF15CzBtc (BtcBufferPtr->TestNumber, WorkBufferPtr);
  }
  RestoreXmmRegisters (XmmSaveBufferPtr);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Setup for boot time calibration tests.
 *
 * This procedure will perform the following steps :
 * 1..Disable boost
 * 2..Set Bsp to P1 (in effect hP1 as boost is disabled)
 * 3..Get Aps into the Ap work procedure
 *
 * @param[in]  BtcServices          The current CPU's family services.
 * @param[in]  StdHeader            Standard configuration header
 */
AGESA_STATUS
STATIC
cpuF15CzBtcBefore (
  IN       BTC_FAMILY_SERVICES *BtcServices,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT8                   TargetPstate;
  AP_EXE_PARAMS           ApParams;

  // Initialize the semaphore.
#ifndef __PRE_RAM__
  OkToStartTest = 0;
#endif

  TargetPstate = cpuF15CzGetTargetPstate (0, StdHeader);
  F15TransitionPstate (NULL, TargetPstate, (BOOLEAN) FALSE, StdHeader);

  ApParams.StdHeader = *StdHeader;
  ApParams.FunctionNumber = ((UINT32) RUN_BTC_ON_ALL_APS + (UINT32) AMD_FAMILY_15_CZ);
  ApParams.RelatedDataBlock = NULL;
  ApParams.RelatedBlockLength = 0;
  ApParams.AllAPs = TRUE;
  RunLateApTaskOnAllAPs (&ApParams, StdHeader);

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Restore pre workload run conditions
 *
 * @param[in]  BtcServices          The current CPU's family services.
 * @param[in]  StdHeader            Standard configuration header
 */
VOID
STATIC
cpuF15CzBtcAfter (
  IN      BTC_FAMILY_SERVICES *BtcServices,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
#ifndef __PRE_RAM__
  InterlockedIncrement ((UINT32 *)&OkToStartTest);
#endif

  F15TransitionPstate (NULL, (UINT8) 0, (BOOLEAN) FALSE, StdHeader);

  IdleAllAps (StdHeader);
}

CONST BTC_FAMILY_SERVICES ROMDATA F15CzBtcSupport =
{
  0,
  (PF_BTC_IS_SUPPORTED) CommonReturnTrue,
  cpuF15CzBtcBefore,
  cpuF15CzBtcRun,
  cpuF15CzBtcAfter,
};

/* TODO: */
VOID
ExecuteF15CzBtc (
  IN       UINT64  CalPgmIndex,
  IN       VOID    *Scratch
  )
{
	printk(BIOS_DEBUG, "TODO: %s\n", __func__);for (;;);
}
