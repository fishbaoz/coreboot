/* $NoKeywords:$ */
/**
 * @file
 *
 *
 * S3 Initialization routines
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Common
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
 *
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
 ******************************************************************************
 */


/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "S3.h"
#include "mfs3.h"
#include "Filecode.h"
#include "heapManager.h"
#include "cpuFeatures.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_COMMON_S3INIT_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
CONST UINT32 ROMDATA S3LateHeapTable[] =
{
  EVENT_LOG_BUFFER_HANDLE,
  SOCKET_DIE_MAP_HANDLE,
  NODE_ID_MAP_HANDLE,
  IDS_CONTROL_HANDLE,
  AMD_S3_SCRIPT_SAVE_TABLE_HANDLE,
  AMD_PCIE_COMPLEX_DATA_HANDLE
};

#define S3LATE_TABLE_SIZE (sizeof (S3LateHeapTable) / sizeof (UINT32))


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
S3Init (
  IN OUT   AMD_RTB_PARAMS *AmdRtbParams
  );

AGESA_STATUS
S3InitConstructor (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   S3_DATA_BLOCK     *S3DataBlock
  );

AGESA_STATUS
S3InitDestructor (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   S3_DATA_BLOCK     *S3DataBlock
  );


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This routine is responsible for saving silicon component registers to the
 * SMM save area in preparation of entering system suspend-to-RAM mode.
 *
 * @param[in,out] AmdRtbParams      Required input parameters for the S3Init routine.
 *
 * @return        Aggregated status across all internal AMD S3 save calls invoked.
 *
 */
AGESA_STATUS
S3Init (
  IN OUT   AMD_RTB_PARAMS *AmdRtbParams
  )
{
  UINTN  i;
  UINT32 EarlyBufferSize;
  UINT32 LateBufferSize;
  UINT32 LateContextSize;
  UINT32 HeapSize;
  UINT8  *BufferPointer;
  UINT8  HeapStatus;
  ALLOCATE_HEAP_PARAMS HeapParams;
  LOCATE_HEAP_PTR LocateHeap;
  BUFFER_NODE *FreeSpaceNode;
  ALLOCATE_HEAP_PARAMS AllocParams;
  DEVICE_BLOCK_HEADER *MemoryRelatedDeviceList;
  DEVICE_BLOCK_HEADER *NonMemoryRelatedDeviceList;
  AGESA_STATUS ReturnStatus;
  VOID   *HeapPtrs[S3LATE_TABLE_SIZE];
  UINT32 HeapSizes[S3LATE_TABLE_SIZE];
  UINT32 HeapBuffersPresent;
  HEAP_MANAGER *HeapPtr;
  VOID *MemDataPointer;

  AGESA_TESTPOINT (TpProcS3Init, &AmdRtbParams->StdHeader);

  ASSERT (AmdRtbParams != NULL);

  ReturnStatus = AGESA_SUCCESS;

  IDS_SKIP_HOOK (IDS_BEFORE_S3_SAVE, AmdRtbParams, &AmdRtbParams->StdHeader) {
    HeapBuffersPresent = 0;
    EarlyBufferSize = 0;
    LateBufferSize = 0;
    LateContextSize = 0;
    HeapSize = 0;
    NonMemoryRelatedDeviceList = NULL;
    MemoryRelatedDeviceList = NULL;
    MemDataPointer = NULL;

    LocateHeap.BufferHandle = AMD_SKIP_MEM_S3_SAVE;
    if (HeapLocateBuffer (&LocateHeap, &AmdRtbParams->StdHeader) == AGESA_SUCCESS) {
      EarlyBufferSize = 0;
    } else {
      // Get memory device list
      MemFS3GetDeviceList (&MemoryRelatedDeviceList, &AmdRtbParams->StdHeader);
      LocateHeap.BufferHandle = AMD_MEM_S3_SAVE_HANDLE;
      if (HeapLocateBuffer (&LocateHeap, &AmdRtbParams->StdHeader) == AGESA_SUCCESS) {
        // Memory data has been saved and stored in the heap.
        // Just copy data from heap.
        // First 4 bytes in the heap store the size of the saved memory data.
        EarlyBufferSize = *(UINT32 *) LocateHeap.BufferPtr;
        MemDataPointer = LocateHeap.BufferPtr + 4;
      } else {
        if (MemoryRelatedDeviceList != NULL) {
          // Determine size needed
          EarlyBufferSize = GetWorstCaseContextSize (MemoryRelatedDeviceList, INIT_RESUME, &AmdRtbParams->StdHeader);
        }
      }
    }

    if (UserOptions.CfgS3LateRestore) {
      for (i = 0; i < S3LATE_TABLE_SIZE; i++) {
        LocateHeap.BufferHandle = S3LateHeapTable[i];
        if (HeapLocateBuffer (&LocateHeap, &AmdRtbParams->StdHeader) == AGESA_SUCCESS) {
          HeapBuffersPresent++;
          HeapSize += LocateHeap.BufferSize;
          HeapPtrs[i] = LocateHeap.BufferPtr;
          HeapSizes[i] = LocateHeap.BufferSize;
        } else {
          HeapPtrs[i] = NULL;
          HeapSizes[i] = 0;
        }
      }

      // Determine heap data size requirements
      if (HeapBuffersPresent != 0) {
        HeapSize += ((sizeof (HEAP_MANAGER)) + (HeapBuffersPresent * ((sizeof (BUFFER_NODE)) + (NUM_OF_SENTINEL * SIZE_OF_SENTINEL) + 0xF))); // reserve 0xF per buffer node for 16 byte alignment
      }

      // Get non memory device list
      GetNonMemoryRelatedDeviceList (&NonMemoryRelatedDeviceList, &AmdRtbParams->StdHeader);

      if (NonMemoryRelatedDeviceList != NULL) {
        // Determine size needed
        LateContextSize = GetWorstCaseContextSize (NonMemoryRelatedDeviceList, S3_LATE_RESTORE, &AmdRtbParams->StdHeader);
      }
      LateBufferSize = HeapSize + LateContextSize;
      if (LateBufferSize != 0) {
        LateBufferSize += sizeof (S3_VOLATILE_STORAGE_HEADER);
      }
    }

    if ((EarlyBufferSize != 0) || (LateBufferSize != 0)) {
      //
      // Allocate a buffer
      //
      AllocParams.RequestedBufferSize = EarlyBufferSize + LateBufferSize;
      AllocParams.BufferHandle = AMD_S3_INFO_BUFFER_HANDLE;
      AllocParams.Persist = 0;
      AGESA_TESTPOINT (TpIfBeforeAllocateS3SaveBuffer, &AmdRtbParams->StdHeader);
      if (HeapAllocateBuffer (&AllocParams, &AmdRtbParams->StdHeader) != AGESA_SUCCESS) {
        if (AGESA_ERROR > ReturnStatus) {
          ReturnStatus = AGESA_ERROR;
        }
      }
      AGESA_TESTPOINT (TpIfAfterAllocateS3SaveBuffer, &AmdRtbParams->StdHeader);

      if (EarlyBufferSize != 0) {
        AmdRtbParams->S3DataBlock.NvStorage = AllocParams.BufferPtr;
        if (MemDataPointer != NULL) {
          LibAmdMemCopy (AmdRtbParams->S3DataBlock.NvStorage,
                         MemDataPointer,
                         EarlyBufferSize,
                         &AmdRtbParams->StdHeader);
          SaveDeviceListContext (MemoryRelatedDeviceList,
                                 AmdRtbParams->S3DataBlock.NvStorage,
                                 PSP_INIT_RESUME,
                                 &EarlyBufferSize,
                                 &AmdRtbParams->StdHeader);
        } else {
          SaveDeviceListContext (MemoryRelatedDeviceList,
                                 AmdRtbParams->S3DataBlock.NvStorage,
                                 INIT_RESUME,
                                 &EarlyBufferSize,
                                 &AmdRtbParams->StdHeader);
        }
      }
      AmdRtbParams->S3DataBlock.NvStorageSize = EarlyBufferSize;

      if (LateBufferSize != 0) {
        BufferPointer = AllocParams.BufferPtr;
        AmdRtbParams->S3DataBlock.VolatileStorage = &(BufferPointer[EarlyBufferSize]);

        ((S3_VOLATILE_STORAGE_HEADER *) AmdRtbParams->S3DataBlock.VolatileStorage)->HeapOffset = NULL;
        ((S3_VOLATILE_STORAGE_HEADER *) AmdRtbParams->S3DataBlock.VolatileStorage)->HeapSize = HeapSize;
        ((S3_VOLATILE_STORAGE_HEADER *) AmdRtbParams->S3DataBlock.VolatileStorage)->RegisterDataOffset = NULL;
        ((S3_VOLATILE_STORAGE_HEADER *) AmdRtbParams->S3DataBlock.VolatileStorage)->RegisterDataSize = LateContextSize;

        if (HeapSize != 0) {
          // Transfer heap contents
          ((S3_VOLATILE_STORAGE_HEADER *) AmdRtbParams->S3DataBlock.VolatileStorage)->HeapOffset = sizeof (S3_VOLATILE_STORAGE_HEADER);
          HeapPtr = (HEAP_MANAGER *) &BufferPointer[EarlyBufferSize + sizeof (S3_VOLATILE_STORAGE_HEADER)];
          HeapPtr->UsedSize = sizeof (HEAP_MANAGER);
          HeapPtr->Signature = HEAP_SIGNATURE_VALID;
          HeapPtr->FirstActiveBufferOffset = AMD_HEAP_INVALID_HEAP_OFFSET;
          HeapPtr->FirstFreeSpaceOffset = sizeof (HEAP_MANAGER);
          FreeSpaceNode = (BUFFER_NODE *) ((UINT8 *) HeapPtr + sizeof (HEAP_MANAGER));
          FreeSpaceNode->BufferSize = HeapSize - sizeof (HEAP_MANAGER) - sizeof (BUFFER_NODE);
          FreeSpaceNode->OffsetOfNextNode = AMD_HEAP_INVALID_HEAP_OFFSET;

          HeapStatus = AmdRtbParams->StdHeader.HeapStatus;
          AmdRtbParams->StdHeader.HeapStatus = HEAP_S3_RESUME;
          AmdRtbParams->StdHeader.HeapBasePtr = (UINT64) (UINTN) HeapPtr;

          for (i = 0; i < S3LATE_TABLE_SIZE; i++) {
            if (HeapPtrs[i] != NULL) {
              HeapParams.RequestedBufferSize = HeapSizes[i]; // S3LateHeapTable[i].BufferLength;
              HeapParams.BufferHandle = S3LateHeapTable[i];
              HeapParams.Persist = HEAP_S3_RESUME;
              if (HeapAllocateBuffer (&HeapParams, &AmdRtbParams->StdHeader) == AGESA_SUCCESS) {
                LibAmdMemCopy ((VOID *) HeapParams.BufferPtr, HeapPtrs[i], HeapSizes[i], &AmdRtbParams->StdHeader);
              }
            }
          }

          AmdRtbParams->StdHeader.HeapStatus = HeapStatus;
        }

        if (LateContextSize != 0) {

          ((S3_VOLATILE_STORAGE_HEADER *) AmdRtbParams->S3DataBlock.VolatileStorage)->RegisterDataOffset = HeapSize + sizeof (S3_VOLATILE_STORAGE_HEADER);

          SaveDeviceListContext (NonMemoryRelatedDeviceList,
                                 &(BufferPointer[EarlyBufferSize + HeapSize + sizeof (S3_VOLATILE_STORAGE_HEADER)]),
                                 S3_LATE_RESTORE,
                                 &LateContextSize,
                                 &AmdRtbParams->StdHeader);
        }
        AmdRtbParams->S3DataBlock.VolatileStorageSize = HeapSize + LateContextSize + sizeof (S3_VOLATILE_STORAGE_HEADER);
      }
    }
  }
  return  ReturnStatus;
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This routine is responsible for setting default values for the input parameters
 * needed by the S3Init routine
 *
 * @param[in]     StdHeader      The standard header.
 * @param[in,out] S3DataBlock    Required input parameters for the S3Init routine.
 *
 * @retval        AGESA_SUCCESS  Always Succeeds.
 *
 */
AGESA_STATUS
S3InitConstructor (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   S3_DATA_BLOCK     *S3DataBlock
  )
{
  S3DataBlockInitializer (S3DataBlock);
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * This routine is responsible for deallocation of heap space allocated during
 * S3Init routine
 *
 * @param[in]     StdHeader       The standard header.
 * @param[in,out] S3DataBlock     Required input parameters for the S3Init routine.
 *
 * @retval        AGESA_STATUS
 *
 */
AGESA_STATUS
S3InitDestructor (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN OUT   S3_DATA_BLOCK     *S3DataBlock
  )
{
  AGESA_STATUS ReturnStatus;
  AGESA_STATUS RetVal;
  LOCATE_HEAP_PTR LocateHeap;

  ASSERT (StdHeader != NULL);

  ReturnStatus = AGESA_SUCCESS;

  // Deallocate heap space allocated during memory S3 save
  LocateHeap.BufferHandle = AMD_MEM_S3_SAVE_HANDLE;
  if (HeapLocateBuffer (&LocateHeap, StdHeader) == AGESA_SUCCESS) {
    RetVal = HeapDeallocateBuffer (AMD_MEM_S3_SAVE_HANDLE, StdHeader);
  } else {
    RetVal = MemS3Deallocate (StdHeader);
  }
  if (RetVal > ReturnStatus) {
    ReturnStatus = RetVal;
  }

  RetVal = HeapDeallocateBuffer (AMD_S3_NB_INFO_BUFFER_HANDLE, StdHeader);
  if (RetVal > ReturnStatus) {
    ReturnStatus = RetVal;
  }

  RetVal = HeapDeallocateBuffer (AMD_S3_INFO_BUFFER_HANDLE, StdHeader);
  if (RetVal > ReturnStatus) {
    ReturnStatus = RetVal;
  }

  return ReturnStatus;
}
