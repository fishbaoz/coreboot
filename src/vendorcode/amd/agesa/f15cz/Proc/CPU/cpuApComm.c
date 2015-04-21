/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU AP related utility functions.
 *
 * Contains code that provides mechanism to invoke and control APIC communication.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#include "cpuCacheInit.h"
#include "cpuRegisters.h"
#include "cpuApComm.h"
#include "cpuApCommApic.h"
#include "OptionApComm.h"
#include "cpuFamilyTranslation.h"
#include "GeneralServices.h"
#include "cpuServices.h"
#include "heapManager.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_CPUAPCOMM_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
#define XFER_ELEMENT_SIZE    sizeof (UINT32)

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
STATIC
PerformFinalHalt (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
ApUtilGetLocalApicBase (
     OUT   UINT64 *ApicBase,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
LocalApicInitialization (
  IN       AMD_CPU_EARLY_PARAMS *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  );

VOID
LocalApicInitializationAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern
VOID
ExecuteFinalHltInstruction (
  IN       UINT32 SharedCore,
  IN       AP_MTRR_SETTINGS  *ApMtrrSettingsList,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

extern
VOID
NmiHandler (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

extern BUILD_OPT_CFG UserOptions;
extern OPTION_AP_COMMUNICATE_CONFIGURATION OptionApCommConfiguration;

/*---------------------------------------------------------------------------------------*/
/**
 * Initialize the Local APIC at the AmdInitEarly entry point.
 *
 * This function acts as a wrapper for calling the LocalApicInitialization
 * routine at AmdInitEarly.
 *
 *  @param[in]   FamilyServices      The current Family Specific Services.
 *  @param[in]   EarlyParams         Service parameters.
 *  @param[in]   StdHeader           Config handle for library and services.
 *
 */
VOID
LocalApicInitializationAtEarly (
  IN       CPU_SPECIFIC_SERVICES  *FamilyServices,
  IN       AMD_CPU_EARLY_PARAMS   *EarlyParams,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AGESA_TESTPOINT (TpProcCpuLocalApicInit, StdHeader);
  LocalApicInitialization (EarlyParams, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Determines the unique ID of the input Socket/Core.
 *
 * This routine converts a socket-core combination to to a number
 * that will be used to directly address a particular core.  This
 * unique value must be less than 128 because we only have a byte
 * to use for status.  APIC IDs are not guaranteed to be below
 * 128.
 *
 * @param[in]      Socket       Socket number of the remote core
 * @param[in]      Core         Core number of the remote core
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 * @return         The unique ID of the desired core
 *
 */
UINT8
ApUtilCalculateUniqueId (
  IN       UINT8 Socket,
  IN       UINT8 Core,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8 UniqueId;

  UniqueId = ((Core << 3) | Socket);
  ASSERT ((UniqueId & 0x80) == 0);
  return (UniqueId);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the 64-bit base address of the executing core's local APIC.
 *
 * This function reads the APICBASE MSR and isolates the programmed address.
 *
 * @param[out]     ApicBase     Base address
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 */
VOID
STATIC
ApUtilGetLocalApicBase (
     OUT   UINT64 *ApicBase,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  LibAmdMsrRead (MSR_APIC_BAR, ApicBase, StdHeader);
  *ApicBase &= LAPIC_BASE_ADDR_MASK;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Calculate the APIC ID for a given core.
 *
 * Get the current node's apic id and deconstruct it to the base id of local apic id space.
 * Then construct the target's apic id using that base.
 * @b Assumes: The target Socket and Core exist!
 * Other Notes:
 *  - Must run after Topology initialization is complete.
 *  - Code sync: This calculation MUST match the assignment
 *    calculation done above in LocalApicInitializationAtEarly function.
 *  - Assumes family homogeneous population of all sockets.
 *
 *  @param[in]      TargetSocket   The socket in which the Core's Processor is installed.
 *  @param[in]      TargetCore     The Core on that Processor
 *  @param[out]     LocalApicId    Its APIC Id
 *  @param[in]      StdHeader      Handle to header for library and services.
 *
 */
VOID
GetLocalApicIdForCore (
  IN       UINT8             TargetSocket,
  IN       UINT8             TargetCore,
     OUT   UINT32            *LocalApicId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  CurrentCore;
  UINT32  CurrentLocalApicId;
  UINT64  LocalApicBase;
  UINT32  TempVar_a;
  UINT64  Address;

  // Get local apic base Address
  ApUtilGetLocalApicBase (&LocalApicBase, StdHeader);
  Address = LocalApicBase + APIC_ID_REG;

  LibAmdMemRead (AccessWidth32, Address, &TempVar_a, StdHeader);

  // ApicId [7:0]
  CurrentLocalApicId = (TempVar_a >> APIC20_ApicId) & 0x000000FF;

  GetCurrentCore (&CurrentCore, StdHeader);
  CurrentLocalApicId = CurrentLocalApicId - CurrentCore + TargetCore;

  *LocalApicId = CurrentLocalApicId;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Reads the APIC register on the designated remote core.
 *
 * This function uses the remote read inter-processor interrupt protocol
 * to read an APIC register from the remote core
 *
 * @param[in]        TargetApicId  Local APIC ID of the desired core
 * @param[in]        RegAddr       APIC register to read
 * @param[in]        StdHeader     Configuration parameters pointer
 *
 * @return           The current value of the remote core's desired APIC register
 *
 */
UINT32
ApUtilRemoteRead (
  IN       UINT32            TargetApicId,
  IN       UINT8             RegAddr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 ApicRegister;
  UINT64 ApicBase;
  UINT64 ApicAddr;

  ApUtilGetLocalApicBase (&ApicBase, StdHeader);
  TargetApicId <<= LOCAL_APIC_ID;

  do {
    ApicAddr = ApicBase + APIC_CMD_HI_REG;
    LibAmdMemWrite (AccessWidth32, ApicAddr, &TargetApicId, StdHeader);
    ApicAddr = ApicBase + APIC_CMD_LO_REG;
    ApicRegister = CMD_REG_TO_READ | (UINT32) RegAddr;
    LibAmdMemWrite (AccessWidth32, ApicAddr, &ApicRegister, StdHeader);
    do {
      LibAmdMemRead (AccessWidth32, ApicAddr, &ApicRegister, StdHeader);
    } while ((ApicRegister & CMD_REG_DELIVERY_STATUS) != 0);
    while ((ApicRegister & CMD_REG_REMOTE_RD_STS_MSK) == CMD_REG_REMOTE_DELIVERY_PENDING) {
      LibAmdMemRead (AccessWidth32, ApicAddr, &ApicRegister, StdHeader);
    }
  } while ((ApicRegister & CMD_REG_REMOTE_RD_STS_MSK) != CMD_REG_REMOTE_DELIVERY_DONE);
  ApicAddr = ApicBase + APIC_REMOTE_READ_REG;
  LibAmdMemRead (AccessWidth32, ApicAddr, &ApicRegister, StdHeader);
  return (ApicRegister);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Writes an APIC register on the executing core.
 *
 * This function gets the base address of the executing core's local APIC,
 * and writes a UINT32 value to a specified offset.
 *
 * @param[in]      RegAddr      APIC register to write to
 * @param[in]      Value        Data to be written to the desired APIC register
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 */
VOID
ApUtilLocalWrite (
  IN       UINT32 RegAddr,
  IN       UINT32 Value,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64 ApicAddr;

  ApUtilGetLocalApicBase (&ApicAddr, StdHeader);
  ApicAddr += RegAddr;

  LibAmdMemWrite (AccessWidth32, ApicAddr, &Value, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Reads an APIC register on the executing core.
 *
 * This function gets the base address of the executing core's local APIC,
 * and reads a UINT32 value from a specified offset.
 *
 * @param[in]      RegAddr      APIC register to read from
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 * @return         The current value of the local APIC register
 *
 */
UINT32
ApUtilLocalRead (
  IN       UINT32  RegAddr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 ApicRegister;
  UINT64 ApicAddr;

  ApUtilGetLocalApicBase (&ApicAddr, StdHeader);
  ApicAddr += RegAddr;
  LibAmdMemRead (AccessWidth32, ApicAddr, &ApicRegister, StdHeader);

  return (ApicRegister);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Waits for a remote core's control byte value to either be equal or
 * not equal to any number of specified values.
 *
 * This function will loop doing remote read IPIs until the remote core's
 * control byte becomes one of the values in the input array if the input
 * flags are set for equality.  Otherwise, the loop will continue until
 * the control byte value is not equal to one of the elements in the
 * array.  The caller can also specify an iteration count for timeout
 * purposes.
 *
 * @param[in]      Socket         Socket number of the target core
 * @param[in]      Core           Core number of the target core
 * @param[in]      WaitParamsPtr  Wait parameter structure
 * @param[in]      StdHeader      Configuration parameteres pointer
 *
 * @return         The current value of the remote core's control byte
 *
 */
UINT8
ApUtilWaitForCoreStatus (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN       AP_WAIT_FOR_STATUS *WaitParamsPtr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  BOOLEAN  IsEqual;
  UINT8 CoreStatus;
  UINT8 i;
  UINT8 j;

  CoreStatus = 0;
  for (i = 0; (WaitParamsPtr->RetryCount == WAIT_INFINITELY) ||
              (i < WaitParamsPtr->RetryCount); ++i) {
    CoreStatus = OptionApCommConfiguration.ApReadRemoteCtrlByte (Socket, Core, StdHeader);
    // Determine whether or not the current remote status is equal
    // to an element in the array.
    IsEqual = FALSE;
    for (j = 0; !IsEqual && j < WaitParamsPtr->NumberOfElements; ++j) {
      if (CoreStatus == WaitParamsPtr->Status[j]) {
        IsEqual = TRUE;
      }
    }
    if ((((WaitParamsPtr->WaitForStatusFlags & WAIT_STATUS_EQUALITY) != 0) && IsEqual) ||
        (((WaitParamsPtr->WaitForStatusFlags & WAIT_STATUS_EQUALITY) == 0) && !IsEqual)) {
      break;
    }
  }
  return (CoreStatus);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Runs the AP task on the executing core.
 *
 * @param[in]      TaskPtr      Function descriptor
 * @param[in]      StdHeader    Configuration parameters pointer
 * @param[in]      ConfigParams Entry point CPU parameters pointer
 *
 * @return         Return value of the task, or zero if the task
 *                 was VOID.
 *
 */
UINT32
ApUtilTaskOnExecutingCore (
  IN       AP_TASK *TaskPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       VOID *ConfigParams
  )
{
  UINT32   InvocationOptions;
  UINT32   ReturnCode;

  ReturnCode = 0;
  InvocationOptions = 0;

  if (TaskPtr->DataTransfer.DataSizeInDwords != 0) {
    InvocationOptions |= AP_TASK_HAS_INPUT;
  }
  if ((TaskPtr->ExeFlags & TASK_HAS_OUTPUT) != 0) {
    InvocationOptions |= AP_TASK_HAS_OUTPUT;
  }
  if ((TaskPtr->ExeFlags & PASS_EARLY_PARAMS) != 0) {
    InvocationOptions |= AP_PASS_EARLY_PARAMS;
  }

  switch (InvocationOptions) {
  case 0:
    TaskPtr->FuncAddress.PfApTask (StdHeader);
    break;
  case AP_TASK_HAS_INPUT:
    TaskPtr->FuncAddress.PfApTaskI (TaskPtr->DataTransfer.DataPtr, StdHeader);
    break;
  case AP_PASS_EARLY_PARAMS:
    TaskPtr->FuncAddress.PfApTaskC (StdHeader, ConfigParams);
    break;
  case (AP_TASK_HAS_INPUT | AP_PASS_EARLY_PARAMS):
    TaskPtr->FuncAddress.PfApTaskIC (TaskPtr->DataTransfer.DataPtr, StdHeader, ConfigParams);
    break;
  case AP_TASK_HAS_OUTPUT:
    ReturnCode = TaskPtr->FuncAddress.PfApTaskO (StdHeader);
    break;
  case (AP_TASK_HAS_INPUT | AP_TASK_HAS_OUTPUT):
    ReturnCode = TaskPtr->FuncAddress.PfApTaskIO (TaskPtr->DataTransfer.DataPtr, StdHeader);
    break;
  case (AP_TASK_HAS_OUTPUT | AP_PASS_EARLY_PARAMS):
    ReturnCode = TaskPtr->FuncAddress.PfApTaskOC (StdHeader, ConfigParams);
    break;
  case (AP_TASK_HAS_INPUT | AP_TASK_HAS_OUTPUT | AP_PASS_EARLY_PARAMS):
    ReturnCode = TaskPtr->FuncAddress.PfApTaskIOC (TaskPtr->DataTransfer.DataPtr, StdHeader, ConfigParams);
    break;
  default:
    ReturnCode = 0;
    break;
  }
  return (ReturnCode);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Securely passes a buffer to the designated remote core.
 *
 * This function uses a sequence of remote reads to transmit a data
 * buffer, one UINT32 at a time.
 *
 * @param[in]      Socket       Socket number of the remote core
 * @param[in]      Core         Core number of the remote core
 * @param[in]      BufferInfo   Information about the buffer to pass, and
 *                              how to pass it
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 */
VOID
ApUtilTransmitBuffer (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN       AP_DATA_TRANSFER  *BufferInfo,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8  TargetCore;
  UINT8  MyUniqueId;
  UINT8  CurrentStatus;
  UINT32 *CurrentPtr;
  UINT32 i;
  UINT32 MyCore;
  UINT32 MySocket;
  UINT32 Ignored;
  AP_WAIT_FOR_STATUS WaitForStatus;
  AGESA_STATUS IgnoredSts;

  if ((BufferInfo->DataTransferFlags & DATA_IN_MEMORY) != 0) {
    OptionApCommConfiguration.ApWriteDataDword ((UINT32) 0x00000000, StdHeader);
  } else {
    OptionApCommConfiguration.ApWriteDataDword ((UINT32) BufferInfo->DataSizeInDwords, StdHeader);
  }
  TargetCore = ApUtilCalculateUniqueId (Socket, Core, StdHeader);

  OptionApCommConfiguration.ApWriteCtrlByte (TargetCore, StdHeader);

  IdentifyCore (StdHeader, &MySocket, &Ignored, &MyCore, &IgnoredSts);

  MyUniqueId = ApUtilCalculateUniqueId ((UINT8)MySocket, (UINT8)MyCore, StdHeader);

  WaitForStatus.Status = &MyUniqueId;
  WaitForStatus.NumberOfElements = 1;
  WaitForStatus.RetryCount = WAIT_INFINITELY;
  WaitForStatus.WaitForStatusFlags = WAIT_STATUS_EQUALITY;

  ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
  OptionApCommConfiguration.ApWriteDataDword (BufferInfo->DataTransferFlags, StdHeader);

  OptionApCommConfiguration.ApWriteCtrlByte (CORE_DATA_FLAGS_READY, StdHeader);
  WaitForStatus.WaitForStatusFlags = 0;
  ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
  if ((BufferInfo->DataTransferFlags & DATA_IN_MEMORY) != 0) {
    ApUtilTransmitPointer (Socket, Core, (VOID **) &BufferInfo->DataPtr, StdHeader);
  } else {
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_STS_DATA_READY_1, StdHeader);
    CurrentStatus = CORE_STS_DATA_READY_0;
    WaitForStatus.Status = &CurrentStatus;
    WaitForStatus.WaitForStatusFlags = WAIT_STATUS_EQUALITY;
    ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
    WaitForStatus.WaitForStatusFlags = 0;
    CurrentPtr = (UINT32 *) BufferInfo->DataPtr;
    for (i = 0; i < BufferInfo->DataSizeInDwords; ++i) {
      OptionApCommConfiguration.ApWriteDataDword (*CurrentPtr++, StdHeader);
      OptionApCommConfiguration.ApWriteCtrlByte (CurrentStatus, StdHeader);
      ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
      CurrentStatus ^= 0x01;
    }
  }
  OptionApCommConfiguration.ApWriteCtrlByte (CORE_ACTIVE, StdHeader);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Securely receives a buffer from the designated remote core.
 *
 * This function uses a sequence of remote reads to receive a data
 * buffer, one UINT32 at a time.
 *
 * @param[in]      Socket       Socket number of the remote core
 * @param[in]      Core         Core number of the remote core
 * @param[in]      BufferInfo   Information about where to place the buffer
 * @param[in]      StdHeader    Configuration parameters pointer
 *
 * @retval         AGESA_SUCCESS Transaction was successful
 * @retval         AGESA_ALERT   The non-NULL desired location to place
 *                               the buffer was not used as the buffer
 *                               resides in a shared memory space.  The
 *                               input data pointer has changed.
 * @retval         AGESA_ERROR   There is not enough room to receive the
 *                               buffer.
 *
 */
AGESA_STATUS
ApUtilReceiveBuffer (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN OUT   AP_DATA_TRANSFER  *BufferInfo,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8    MyUniqueId;
  UINT8    SourceUniqueId;
  UINT8    CurrentStatus;
  UINT32   i;
  UINT32   MySocket;
  UINT32   MyCore;
  UINT32   Ignored;
  UINT32   *CurrentPtr;
  UINT32   TransactionSize;
  AGESA_STATUS ReturnStatus;
  ALLOCATE_HEAP_PARAMS HeapMalloc;
  AP_WAIT_FOR_STATUS WaitForStatus;

  ReturnStatus = AGESA_SUCCESS;
  IdentifyCore (StdHeader, &MySocket, &Ignored, &MyCore, &ReturnStatus);

  MyUniqueId = ApUtilCalculateUniqueId ((UINT8)MySocket, (UINT8)MyCore, StdHeader);

  WaitForStatus.Status = &MyUniqueId;
  WaitForStatus.NumberOfElements = 1;
  WaitForStatus.RetryCount = WAIT_INFINITELY;
  WaitForStatus.WaitForStatusFlags = WAIT_STATUS_EQUALITY;

  ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
  TransactionSize = OptionApCommConfiguration.ApReadRemoteDataDword (Socket, Core, StdHeader);

  if (BufferInfo->DataPtr == NULL && TransactionSize != 0) {
    HeapMalloc.BufferHandle = AMD_CPU_AP_TASKING_HANDLE;
    HeapMalloc.Persist = HEAP_LOCAL_CACHE;
    // Deallocate the general purpose heap structure, if it exists.  Ignore
    // the status in case it does not exist.
    HeapDeallocateBuffer (HeapMalloc.BufferHandle, StdHeader);
    HeapMalloc.RequestedBufferSize = (TransactionSize * XFER_ELEMENT_SIZE);
    if (HeapAllocateBuffer (&HeapMalloc, StdHeader) == AGESA_SUCCESS) {
      BufferInfo->DataPtr = (UINT32 *) HeapMalloc.BufferPtr;
      BufferInfo->DataSizeInDwords = (UINT16) (HeapMalloc.RequestedBufferSize / XFER_ELEMENT_SIZE);
    } else {
      BufferInfo->DataSizeInDwords = 0;
    }
  }

  if (TransactionSize <= BufferInfo->DataSizeInDwords) {
    SourceUniqueId = ApUtilCalculateUniqueId (Socket, Core, StdHeader);
    OptionApCommConfiguration.ApWriteCtrlByte (SourceUniqueId, StdHeader);
    CurrentStatus = CORE_DATA_FLAGS_READY;
    WaitForStatus.Status = &CurrentStatus;
    ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
    BufferInfo->DataTransferFlags =  OptionApCommConfiguration.ApReadRemoteDataDword (Socket, Core, StdHeader);
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_DATA_FLAGS_ACKNOWLEDGE, StdHeader);
    if ((BufferInfo->DataTransferFlags & DATA_IN_MEMORY) != 0) {
      if (BufferInfo->DataPtr != NULL) {
        ReturnStatus = AGESA_ALERT;
      }
      ApUtilReceivePointer (Socket, Core, (VOID **) &BufferInfo->DataPtr, StdHeader);
    } else {
      CurrentStatus = CORE_STS_DATA_READY_1;
      ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
      CurrentStatus = CORE_STS_DATA_READY_0;
      OptionApCommConfiguration.ApWriteCtrlByte (CurrentStatus, StdHeader);
      CurrentPtr = BufferInfo->DataPtr;
      for (i = 0; i < TransactionSize; ++i) {
        ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
        *CurrentPtr++ = OptionApCommConfiguration.ApReadRemoteDataDword (Socket, Core, StdHeader);
        CurrentStatus ^= 0x01;
        OptionApCommConfiguration.ApWriteCtrlByte (CurrentStatus, StdHeader);
      }
    }
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_ACTIVE, StdHeader);
  } else {
    BufferInfo->DataSizeInDwords = (UINT16) TransactionSize;
    ReturnStatus = AGESA_ERROR;
  }
  return (ReturnStatus);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Securely transmits a pointer to the designated remote core.
 *
 * This function uses a sequence of remote reads to transmit a pointer,
 * one UINT32 at a time.
 *
 * @param[in]      Socket        Socket number of the target core
 * @param[in]      Core          Core number of the target core
 * @param[out]     Pointer       Pointer passed from remote core
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 */
VOID
ApUtilTransmitPointer (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  IN       VOID  **Pointer,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8   i;
  UINT8   WaitStatus;
  UINT32  *AddressScratchPtr;
  AP_WAIT_FOR_STATUS WaitForStatus;

  WaitStatus = CORE_NEEDS_PTR;
  WaitForStatus.Status = &WaitStatus;
  WaitForStatus.NumberOfElements = 1;
  WaitForStatus.RetryCount = WAIT_INFINITELY;

  AddressScratchPtr = (UINT32 *) Pointer;

  for (i = 0; i < SIZE_IN_DWORDS (AddressScratchPtr); i++) {
    WaitForStatus.WaitForStatusFlags = WAIT_STATUS_EQUALITY;
    ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
    OptionApCommConfiguration.ApWriteDataDword (*AddressScratchPtr++, StdHeader);
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_STS_DATA_READY_0, StdHeader);
    WaitForStatus.WaitForStatusFlags = 0;
    ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_ACTIVE, StdHeader);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Securely receives a pointer from the designated remote core.
 *
 * This function uses a sequence of remote reads to receive a pointer,
 * one UINT32 at a time.
 *
 * @param[in]      Socket         Socket number of the target core
 * @param[in]      Core           Core number of the target core
 * @param[out]     ReturnPointer Pointer passed from remote core
 * @param[in]      StdHeader     Configuration parameters pointer
 *
 */
VOID
ApUtilReceivePointer (
  IN       UINT8   Socket,
  IN       UINT8   Core,
  OUT   VOID  **ReturnPointer,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8   i;
  UINT8   WaitStatus;
  UINT32  *AddressScratchPtr;
  AP_WAIT_FOR_STATUS WaitForStatus;

  WaitStatus = CORE_STS_DATA_READY_0;
  WaitForStatus.Status = &WaitStatus;
  WaitForStatus.NumberOfElements = 1;
  WaitForStatus.RetryCount = WAIT_INFINITELY;
  AddressScratchPtr = (UINT32 *) ReturnPointer;
  for (i = 0; i < SIZE_IN_DWORDS (AddressScratchPtr); ++i) {
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_NEEDS_PTR, StdHeader);
    WaitForStatus.WaitForStatusFlags = WAIT_STATUS_EQUALITY;
    ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
    *AddressScratchPtr++ = OptionApCommConfiguration.ApReadRemoteDataDword (Socket, Core, StdHeader);
    OptionApCommConfiguration.ApWriteCtrlByte (CORE_ACTIVE, StdHeader);
    WaitForStatus.WaitForStatusFlags = 0;
    ApUtilWaitForCoreStatus (Socket, Core, &WaitForStatus, StdHeader);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Relinquish control of APs
 *
 * This function launches all APs and perform PerformFinalHalt
 *
 * @param[in]  StdHeader         Config handle for library and services
 *
 */
VOID
RelinquishControlOfAllAPs (
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32        Core;
  AP_TASK       TaskPtr;

  ASSERT (IsBsp (StdHeader));

  TaskPtr.FuncAddress.PfApTask = PerformFinalHalt;
  TaskPtr.DataTransfer.DataSizeInDwords = 0;
  TaskPtr.ExeFlags = WAIT_FOR_CORE;

  GetActiveCoresInCurrentSocket (&Core, StdHeader);
  while (Core-- > 1) {
    OptionApCommConfiguration.ApRunCodeOnSocketCore (0, (UINT8) Core, &TaskPtr, StdHeader);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Wakes up a core from the halted state.
 *
 * This function sends a directed NMI inter-processor interrupt to
 * the input Socket/Core.
 *
 * @param[in]      TargetApicId    Local APIC ID of the desired core
 * @param[in]      StdHeader       Configuration parameters pointer
 *
 */
VOID
ApUtilFireDirectedNmi (
  IN       UINT32 TargetApicId,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  TargetApicId <<= LOCAL_APIC_ID;

  ApUtilLocalWrite ((UINT32) APIC_CMD_HI_REG, TargetApicId, StdHeader);
  ApUtilLocalWrite ((UINT32) APIC_CMD_LO_REG, (UINT32) CMD_REG_TO_NMI, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Sets up the AP's IDT with NMI (INT2) being the only valid descriptor
 *
 * This function prepares the executing AP core for recovering from a hlt
 * instruction by initializing its IDTR.
 *
 * @param[in]        NmiIdtDescPtr Pointer to a writable IDT entry to
 *                                 be used for NMIs
 * @param[in]        StdHeader     Configuration parameters pointer
 *
 */
VOID
ApUtilSetupIdtForHlt (
  IN       IDT_DESCRIPTOR *NmiIdtDescPtr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT8   DescSize;
  UINT64  HandlerOffset;
  UINT64  EferRegister;
  IDT_BASE_LIMIT IdtInfo;

  LibAmdMsrRead (MSR_EXTENDED_FEATURE_EN, &EferRegister, StdHeader);
  if ((EferRegister & 0x100) != 0) {
    DescSize = 16;
  } else {
    DescSize = 8;
  }

  HandlerOffset = (UINT64) (UINTN) NmiHandler;
  NmiIdtDescPtr->OffsetLo = (UINT16) HandlerOffset & 0xFFFF;
  NmiIdtDescPtr->OffsetHi = (UINT16) (HandlerOffset >> 16);
  GetCsSelector (&NmiIdtDescPtr->Selector, StdHeader);
  NmiIdtDescPtr->Flags = IDT_DESC_PRESENT | IDT_DESC_TYPE_INT32;
  NmiIdtDescPtr->Rsvd = 0;
  NmiIdtDescPtr->Offset64 = (UINT32) (HandlerOffset >> 32);
  NmiIdtDescPtr->Rsvd64 = 0;
  IdtInfo.Limit = (UINT16) ((DescSize * 3) - 1);
  IdtInfo.Base = (UINT64) (UINTN) NmiIdtDescPtr - (DescSize * 2);
  IDS_EXCEPTION_TRAP (IDS_IDT_UPDATE_EXCEPTION_VECTOR_FOR_AP, &IdtInfo, StdHeader);
  SetIdtr (&IdtInfo , StdHeader);
}

/*---------------------------------------------------------------------------------------
 *                           L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 * Initialize the Local APIC.
 *
 * This function determines and programs the appropriate APIC ID value
 * for the executing core.  This code must be run after topology initialization
 * is complete.
 *
 *  @param[in]     CpuEarlyParamsPtr  Service parameters.
 *  @param[in]     StdHeader          Config handle for library and services.
 *
 */
VOID
STATIC
LocalApicInitialization (
  IN       AMD_CPU_EARLY_PARAMS *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS    *StdHeader
  )
{
  UINT32    CurrentCore;
  UINT32    CurrentNodeNum;
  UINT32    CoreIdBits;
  UINT32    Mnc;
  UINT32    ProcessorCount;
  UINT32    ProcessorApicIndex;
  UINT32    IoApicNum;
  UINT32    StartLocalApicId;
  UINT64    LocalApicBase;
  UINT32    TempVar_a;
  UINT64    MsrData;
  UINT64    Address;
  CPUID_DATA  CpuidData;

  // Local variables default values
  IoApicNum = CpuEarlyParamsPtr->PlatformConfig.NumberOfIoApics;

  GetCurrentCore (&CurrentCore, StdHeader);
  CurrentNodeNum = 0;

  // Get Mnc
  LibAmdCpuidRead (AMD_CPUID_ASIZE_PCCOUNT, &CpuidData, StdHeader);
  CoreIdBits = (CpuidData.ECX_Reg & 0x0000F000) >> 12;
  Mnc = 1 << (CoreIdBits & 0x000F);

  // Get ProcessorCount in the system
  ProcessorCount = 1;

  // Get the APIC Index of this processor.
  ProcessorApicIndex = 0;

  TempVar_a = (Mnc * ProcessorCount) + IoApicNum;
  ASSERT (TempVar_a < 255);

  // Apply apic enumeration rules
  // For systems with >= 16 APICs, put the IO-APICs at 0..n and
  // put the local-APICs at m..z
  // For systems with < 16 APICs, put the Local-APICs at 0..n and
  // put the IO-APICs at (n + 1)..z
  // This is needed because many IO-APIC devices only have 4 bits
  // for their APIC id and therefore must reside at 0..15
  StartLocalApicId = 0;
  if (TempVar_a >= 16) {
    if (IoApicNum >= 1) {
      StartLocalApicId = (IoApicNum - 1) / Mnc;
      StartLocalApicId = (StartLocalApicId + 1) * Mnc;
    }
  }

  // Set local apic id
  TempVar_a = (ProcessorApicIndex * Mnc) + CurrentCore + StartLocalApicId;
  IDS_HDT_CONSOLE (CPU_TRACE, "  Node %d core %d APIC ID = 0x%x\n", CurrentNodeNum, CurrentCore, TempVar_a);
  TempVar_a = TempVar_a << APIC20_ApicId;

  // Enable local apic id
  LibAmdMsrRead (MSR_APIC_BAR, &MsrData, StdHeader);
  MsrData |= APIC_ENABLE_BIT;
  LibAmdMsrWrite (MSR_APIC_BAR, &MsrData, StdHeader);

  // Get local apic base Address
  ApUtilGetLocalApicBase (&LocalApicBase, StdHeader);

  Address = LocalApicBase + APIC_ID_REG;
  LibAmdMemWrite (AccessWidth32, Address, &TempVar_a, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * The last AGESA code that an AP performs
 *
 * This function, run only by APs, breaks down their cache subsystem, sets up
 * for memory to be present upon wake (from IBV Init/Startup IPIs), and halts.
 *
 * @param[in]  StdHeader         Config handle for library and services
 *
 */
VOID
STATIC
PerformFinalHalt (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 PrimaryCore;
  UINT32 HaltFlags;
  UINT32 CacheEnDis;
  CPU_SPECIFIC_SERVICES *FamilyServices;
  CONST CACHE_INFO *CacheInfoPtr;
  UINT8  NumberOfElements;

  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilyServices, StdHeader);
  ASSERT (FamilyServices != NULL);
  // CacheEnDis is a family specific flag, that lets the code to decide whether to
  // keep the cache control bits set or cleared.
  CacheEnDis = FamilyServices->InitCacheDisabled;

  // Determine if the current core has the primary core role.  The first core to execute
  // in each compute unit has the primary role.
  PrimaryCore = (UINT32) IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader);

  // If the core is not PrimaryCore, check if VarMTRRs are not shared among cores in a compute unit,
  // low 32bits of VarMTRR mask is 0xFFFFFFFF. Treat each AP as primary core for setting MTRRs.
  if (PrimaryCore != TRUE) {
    FamilyServices->GetCacheInfo (FamilyServices, (CONST VOID **) &CacheInfoPtr, &NumberOfElements, StdHeader);
    if (((UINT32) CacheInfoPtr->VariableMtrrHeapMask) == 0xFFFFFFFF) {
      PrimaryCore = TRUE;
    }
  }

  // Aggregate the flags for the halt service.
  HaltFlags = PrimaryCore | (CacheEnDis << 1);

  if (IsSecureS3 (StdHeader)) {
    HaltFlags |= 0x4;
  }

  OptionApCommConfiguration.ApWriteCtrlByte (CORE_UNAVAILABLE, StdHeader);
  HaltFlags |= OptionApCommConfiguration.ApCommMethod << 16;
  ExecuteFinalHltInstruction (HaltFlags, UserOptions.CfgApMtrrSettingsList, StdHeader);
}

