/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU AP communication by PCI related utility functions.
 *
 * Contains code that provides mechanism to invoke and control AP communication by PCI.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
#include "Ids.h"
#include "cpuRegisters.h"
#include "cpuApComm.h"
#include "cpuApCommPci.h"
#include "GeneralServices.h"
#include "cpuServices.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_CPUAPCOMMPCI_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern
VOID
ExecuteHltInstruction (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Reads the 'control byte' on the designated remote core.
 *
 * This function will read the current contents of the control byte
 * on the designated core using PCI registers
 *
 * @param[in]      Socket        Socket number of the target core
 * @param[in]      Core          Core number of the target core
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 * @return         The current value of the remote cores control byte
 *
 */
UINT8
ApReadRemoteControlBytePci (
  IN       UINT8 Socket,
  IN       UINT8 Core,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32   PciData;
  PCI_ADDR PciAddress;

  ASSERT (Core < 4); // We only support up to 4 cores

  PciAddress.AddressValue = AP_COMM_CORE0_CONTROL_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
  return (UINT8) ((PciData >> (AP_COMM_CONTROL_BITS_WIDTH * Core)) & AP_COMM_CONTROL_MASK);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Writes the 'control byte' on the executing core.
 *
 * This function writes data to a PCI register
 *
 * @param[in]       Value
 * @param[in]       StdHeader
 *
 */
VOID
ApWriteControlBytePci (
  IN       UINT8 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8    PciData;
  UINT32   CurrentCore;
  PCI_ADDR PciAddress;

  GetCurrentCore (&CurrentCore, StdHeader);

  ASSERT (CurrentCore < 4); // We only support up to 4 cores

  PciAddress.AddressValue = AP_COMM_CORE0_CONTROL_ADDR + CurrentCore;
  PciData = Value;
  LibAmdPciWrite (AccessWidth8, PciAddress, &PciData, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Reads the 'data dword' on the designated remote core.
 *
 * This function will read the current contents of the data dword
 * on the designated core using PCI registers
 *
 * @param[in]      Socket        Socket number of the target core
 * @param[in]      Core          Core number of the target core
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 * @return         The current value of the remote core's data dword
 *
 */
UINT32
ApReadRemoteDataDwordPci (
  IN       UINT8 Socket,
  IN       UINT8 Core,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32   PciData;
  PCI_ADDR PciAddress;

  ASSERT (Core < 4); // We only support up to 4 cores

  PciAddress.AddressValue = AP_COMM_CORE0_DATA_ADDR + 4 * Core;
  LibAmdPciRead (AccessWidth32, PciAddress, &PciData, StdHeader);
  return PciData;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Writes the 'data dword' on the executing core.
 *
 * This function writes data to a PCI register
 *
 * @param[in]      Value        Value to write
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 */
VOID
ApWriteDataDwordPci (
  IN       UINT32 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32   CurrentCore;
  PCI_ADDR PciAddress;

  GetCurrentCore (&CurrentCore, StdHeader);

  ASSERT (CurrentCore < 4); // We only support up to 4 cores

  PciAddress.AddressValue = AP_COMM_CORE0_DATA_ADDR + 4 * CurrentCore;
  LibAmdPciWrite (AccessWidth32, PciAddress, &Value, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Runs the given task on the specified local core.
 *
 * This function is used to invoke an AP to run a specified AGESA
 * procedure.  It can only be called by cores that have subordinate
 * APs -- the BSC at POST, or any socket-relative core 0s at Early.
 *
 * @param[in]      Socket       Socket number of the target core
 * @param[in]      Core         Core number of the target core
 * @param[in]      TaskPtr      Function descriptor
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 * @return         Return value of the task that the AP core ran,
 *                 or zero if the task was VOID.
 *
 */
UINT32
ApRunCodeOnSocketCorePci (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN       AP_TASK *TaskPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8  CoreId;
  UINT8  CurrentStatus;
  UINT8  WaitStatus[3];
  UINT32 ApFlags;
  UINT32 ReturnCode;
  UINT32 TargetApicId;
  AP_WAIT_FOR_STATUS WaitForStatus;

  ApFlags = 0;
  ReturnCode = 0;

  CoreId = ApUtilCalculateUniqueId (Socket, Core, StdHeader);

  GetLocalApicIdForCore (Socket, Core, &TargetApicId, StdHeader);

  if (TaskPtr->DataTransfer.DataSizeInDwords != 0) {
    ApFlags |= AP_TASK_HAS_INPUT;
    if (((TaskPtr->ExeFlags & RETURN_PARAMS) != 0) &&
        ((TaskPtr->DataTransfer.DataTransferFlags & DATA_IN_MEMORY) == 0)) {
      ApFlags |= AP_RETURN_PARAMS;
    }
  }

  if ((TaskPtr->ExeFlags & TASK_HAS_OUTPUT) != 0) {
    ApFlags |= AP_TASK_HAS_OUTPUT;
  }

  if ((TaskPtr->ExeFlags & END_AT_HLT) != 0) {
    ApFlags |= AP_END_AT_HLT;
  }

  if ((TaskPtr->ExeFlags & PASS_EARLY_PARAMS) != 0) {
    ApFlags |= AP_PASS_EARLY_PARAMS;
  }

  WaitStatus[0] = CORE_IDLE;
  WaitStatus[1] = CORE_IDLE_HLT;
  WaitStatus[2] = CORE_UNAVAILABLE;
  WaitForStatus.Status = WaitStatus;
  WaitForStatus.NumberOfElements = 3;
  WaitForStatus.RetryCount = WAIT_INFINITELY;
  WaitForStatus.WaitForStatusFlags = WAIT_STATUS_EQUALITY;
  CurrentStatus = ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);

  if (CurrentStatus != CORE_UNAVAILABLE) {
    ApWriteDataDwordPci (ApFlags, StdHeader);
    ApWriteControlBytePci (CoreId, StdHeader);

    if (CurrentStatus == CORE_IDLE_HLT) {
      ApUtilFireDirectedNmi (TargetApicId, StdHeader);
    }

    ApUtilTransmitPointer (Socket, Core, (VOID **) &TaskPtr->FuncAddress, StdHeader);

    if ((ApFlags & AP_TASK_HAS_INPUT) != 0) {
      ApUtilTransmitBuffer (Socket, Core, &TaskPtr->DataTransfer, StdHeader);
    }

    if ((TaskPtr->ExeFlags & WAIT_FOR_CORE) != 0) {
      if (((ApFlags & AP_TASK_HAS_INPUT) != 0) &&
          ((ApFlags & AP_RETURN_PARAMS) != 0) &&
          ((TaskPtr->DataTransfer.DataTransferFlags & DATA_IN_MEMORY) == 0)) {
        if (ApUtilReceiveBuffer (Socket, Core, &TaskPtr->DataTransfer, StdHeader) == AGESA_ERROR) {
          // There is not enough space to put the return data.  This should never occur.  If it
          // does, this would point to strange heap corruption.
          IDS_ERROR_TRAP;
        }
      }

      ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
      if ((ApFlags & AP_TASK_HAS_OUTPUT) != 0) {
        ReturnCode = ApReadRemoteDataDwordPci (Socket, Core, StdHeader);
      }
    }
  } else {
    ReturnCode = 0;
  }
  return (ReturnCode);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Main entry point for all APs in the system.
 *
 * This routine puts the AP cores in an infinite loop in which the cores
 * will poll their masters, waiting to be told to perform a task.  At early,
 * all socket-relative core zeros will receive their tasks from the BSC.
 * All others will receive their tasks from the core zero of their local
 * processor.  At the end of AmdInitEarly, all cores will switch to receiving
 * their tasks from the BSC.
 *
 * @param[in]     StdHeader       Handle to config for library and services.
 * @param[in]     CpuParams       Cpu params pointer.
 *
 */
VOID
ApEntryPci (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       VOID              *CpuParams
  )
{
  UINT8   RemoteCmd;
  UINT8   CommandStart;
  UINT32  ApFlags;
  UINT32  FuncType;
  UINT32  ReturnCode;
  UINT32  CurrentCore;
  UINT32  *InputDataPtr;
  AP_FUNCTION_PTR FuncAddress;
  IDT_DESCRIPTOR IdtDesc[32];
  AP_DATA_TRANSFER DataTransferInfo;
  AMD_CPU_EARLY_PARAMS *CpuEarlyParams;

  ASSERT (!IsBsp (StdHeader));

  // Initialize local variables
  ReturnCode = 0;
  DataTransferInfo.DataTransferFlags = 0;
  InputDataPtr = NULL;
  CpuEarlyParams = (AMD_CPU_EARLY_PARAMS *) CpuParams;

  // Get CurrentCore
  GetCurrentCore (&CurrentCore, StdHeader);

  IDS_HDT_CONSOLE (CPU_TRACE, "  Socket %d core %d begin AP tasking engine\n", 0, CurrentCore);

  // Setup Interrupt Descriptor Table for sleep mode
  ApUtilSetupIdtForHlt (&IdtDesc[2], StdHeader);

  // Indicate to the BSC that we have reached the tasking engine
  ApWriteControlBytePci (CORE_IDLE, StdHeader);


  // Determine the unique value that the master will write when it has a task
  // for this core to perform.
  CommandStart = ApUtilCalculateUniqueId (
                   0,
                   (UINT8)CurrentCore,
                   StdHeader
                   );
  for (;;) {
    RemoteCmd = ApReadRemoteControlBytePci (0, 0, StdHeader);
    if (RemoteCmd == CommandStart) {
      ApFlags = ApReadRemoteDataDwordPci (0, 0, StdHeader);

      ApUtilReceivePointer (0, 0, (VOID **) &FuncAddress, StdHeader);

      FuncType = ApFlags & (UINT32) (AP_TASK_HAS_INPUT | AP_TASK_HAS_OUTPUT | AP_PASS_EARLY_PARAMS);
      if ((ApFlags & AP_TASK_HAS_INPUT) != 0) {
        DataTransferInfo.DataSizeInDwords = 0;
        DataTransferInfo.DataPtr = NULL;
        DataTransferInfo.DataTransferFlags = 0;
        if (ApUtilReceiveBuffer (0, 0, &DataTransferInfo, StdHeader) == AGESA_ERROR) {
          // There is not enough space to put the input data on the heap.  Undefined behavior is about
          // to result.
          IDS_ERROR_TRAP;
        }
        InputDataPtr = (UINT32 *) DataTransferInfo.DataPtr;
      }
      ApWriteControlBytePci (CORE_ACTIVE, StdHeader);
      switch (FuncType) {
      case 0:
        FuncAddress.PfApTask (StdHeader);
        break;
      case AP_TASK_HAS_INPUT:
        FuncAddress.PfApTaskI (InputDataPtr, StdHeader);
        break;
      case AP_PASS_EARLY_PARAMS:
        FuncAddress.PfApTaskC (StdHeader, CpuEarlyParams);
        break;
      case (AP_TASK_HAS_INPUT | AP_PASS_EARLY_PARAMS):
        FuncAddress.PfApTaskIC (InputDataPtr, StdHeader, CpuEarlyParams);
        break;
      case AP_TASK_HAS_OUTPUT:
        ReturnCode = FuncAddress.PfApTaskO (StdHeader);
        break;
      case (AP_TASK_HAS_INPUT | AP_TASK_HAS_OUTPUT):
        ReturnCode = FuncAddress.PfApTaskIO (InputDataPtr, StdHeader);
        break;
      case (AP_TASK_HAS_OUTPUT | AP_PASS_EARLY_PARAMS):
        ReturnCode = FuncAddress.PfApTaskOC (StdHeader, CpuEarlyParams);
        break;
      case (AP_TASK_HAS_INPUT | AP_TASK_HAS_OUTPUT | AP_PASS_EARLY_PARAMS):
        ReturnCode = FuncAddress.PfApTaskIOC (InputDataPtr, StdHeader, CpuEarlyParams);
        break;
      default:
        ReturnCode = 0;
        break;
      }
      if (((ApFlags & AP_RETURN_PARAMS) != 0)) {
        ApUtilTransmitBuffer (0, 0, &DataTransferInfo, StdHeader);
      }
      if ((ApFlags & AP_TASK_HAS_OUTPUT) != 0) {
        ApWriteDataDwordPci (ReturnCode, StdHeader);
      }
      if ((ApFlags & AP_END_AT_HLT) != 0) {
        RemoteCmd = CORE_IDLE_HLT;
      } else {
        ApWriteControlBytePci (CORE_IDLE, StdHeader);
      }
    }
    if (RemoteCmd == CORE_IDLE_HLT) {
      ApWriteControlBytePci (CORE_IDLE_HLT, StdHeader);
      ExecuteHltInstruction (StdHeader);
      ApWriteControlBytePci (CORE_IDLE, StdHeader);
    }
  }
}

