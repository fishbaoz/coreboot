/* $NoKeywords:$ */
/**
 * @file
 *
 * mnpmuD3cz.c
 *
 * Support functions for memory training using PMU for CZ
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/CZ)
 * @e \$Revision: 316599 $ @e \$Date: 2015-04-10 08:31:52 +0800 (Fri, 10 Apr 2015) $
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

/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */

#include "AGESA.h"
#include "mport.h"
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mtsdi3.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mnPmuFirmwareCZ.h"
#include "mnPmuFirmwareD3cz.h"
#include "Filecode.h"
#include "heapManager.h"
#include "mnPmuSramMsgBlockCZ.h"
#include "OptionMemory.h"
#include "mncz.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_CZ_MNPMUD3CZ_FILECODE



/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define PMU_TRAINS_ECC_LANES       0x80    ///< PMU trains ECC lanes
/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */


ROMDATA PMU_FIRMWARE PmuFirmwareD3ImageTableCZ[] = {
//
//  PMU Signature  Instance   Package String      NULL Reserved   Size                                           Image Data
//
  {{PMU_SIGNATURE,    '0',    PMU_PACKAGE_STRING,  0,     {0}  }, PMU_SRAM_DMEM_SIZE_CZ + PMU_SRAM_IMEM_SIZE_CZ, PMU_DDR3_CZ_9040   },
  {{PMU_SIGNATURE,    '1',    PMU_PACKAGE_STRING,  0,     {0}  }, PMU_IMAGE_NOT_USEABLE },
};
ROMDATA UINT8 PmuFirmwareD3ImageTableSizeCZ = GET_SIZE_OF (PmuFirmwareD3ImageTableCZ);

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function sets the PMU sequence control for DRAM training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
// This is a placholder for D3 Specific Sequence Control
//
// VOID
// MemNSetPmuSequenceControlD3CZ (
//   IN OUT   MEM_NB_BLOCK *NBPtr
//   )
// {
//   LOCATE_HEAP_PTR LocHeap;
//   PMU_SRAM_MSG_BLOCK_CZ *PmuSramMsgBlockPtr;
//
//   LocHeap.BufferHandle = AMD_MEM_PMU_SRAM_MSG_BLOCK_HANDLE;
//
//   if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) != AGESA_SUCCESS) {
//     ASSERT(FALSE); // Could not locate heap for PMU SRAM Message Block buffer.
//   }
//
//   PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_CZ *) LocHeap.BufferPtr;
//   //
//   // Call Common Sequence Control
//   //
//   MemNSetPmuSequenceControlCZ(NBPtr);
// }
//

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function passes to PMU DRAM configuration over PMU SRAM for DDR3
 *     DRAM init and training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPopulatePmuSramConfigD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  PMU_SRAM_MSG_BLOCK_CZ *PmuSramMsgBlockPtr;
  //
  // Call Common Config routine
  //
  MemNPopulatePmuSramConfigCZ (NBPtr);
  //
  // DDR3 Unbuffered specific configuration
  //
  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_CZ *) NBPtr->PsPtr->PmuSramMsgBlockPtr;
  //
  // NO DDR3 UNBUFFERED-SPECIFIC CONFIG
  //
  return;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function passes to PMU DRAM timings over PMU SRAM for DDR3 DRAM init and training.
 *     Timings are MemPstate dependent.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPopulatePmuSramTimingsD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 Mr0;
  UINT32 Mr1;
  UINT32 Mr2;

  PMU_SRAM_MSG_BLOCK_CZ *PmuSramMsgBlockPtr;

  //
  // Call Common Timing routine
  //
  MemNPopulatePmuSramTimingsCZ (NBPtr);
  //
  // DDR4 Unbuffered-specific timings
  //
  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_CZ *) NBPtr->PsPtr->PmuSramMsgBlockPtr;

  if (NBPtr->MemPstate == 0) {
    PmuSramMsgBlockPtr->M0CkeSetup = (UINT8) (NBPtr->ChannelPtr->DctAddrTmg & 0xFF);
    PmuSramMsgBlockPtr->M0CsOdtSetup = (UINT8) ((NBPtr->ChannelPtr->DctAddrTmg >> 8) & 0xFF);
    PmuSramMsgBlockPtr->M0AddrCmdSetup = (UINT8) (UINT8) ((NBPtr->ChannelPtr->DctAddrTmg >> 16) & 0xFF);
  } else {
    PmuSramMsgBlockPtr->Annex.D3u.M1CkeSetup = (UINT8) (NBPtr->ChannelPtr->DctAddrTmgPs1 & 0xFF);
    PmuSramMsgBlockPtr->Annex.D3u.M1CsOdtSetup = (UINT8) ((NBPtr->ChannelPtr->DctAddrTmgPs1 >> 8) & 0xFF);
    PmuSramMsgBlockPtr->Annex.D3u.M1AddrCmdSetup = (UINT8) ((NBPtr->ChannelPtr->DctAddrTmgPs1 >> 16) & 0xFF);
  }
  //
  // MR Commands
  //
  Mr0 = (UINT16) MemNGetBitFieldNb (NBPtr, BFMxMr0);
  Mr1 = (UINT16) MemNGetBitFieldNb (NBPtr, BFMxMr1);
  Mr2 = (UINT16) MemNGetBitFieldNb (NBPtr, BFMxMr2);

  // Program MrsAddress[8]=1 (DLL):DLL reset for PMU init.
  Mr0 |= 1 << 8;
  if (NBPtr->MemPstate == 0) {
    PmuSramMsgBlockPtr->Annex.D3u.M0_MR0 = (UINT16) Mr0;
    PmuSramMsgBlockPtr->Annex.D3u.M0_MR1 = (UINT16) Mr1;
    PmuSramMsgBlockPtr->Annex.D3u.M0_MR2 = (UINT16) Mr2;
  } else {
    PmuSramMsgBlockPtr->Annex.D3u.M1_MR0 = (UINT16) Mr0;
    PmuSramMsgBlockPtr->Annex.D3u.M1_MR1 = (UINT16) Mr1;
    PmuSramMsgBlockPtr->Annex.D3u.M1_MR2 = (UINT16) Mr2;
  }

  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tMR0: %04x\n", Mr0);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tMR1: %04x\n", Mr1);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tMR2: %04x\n", Mr2);

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function set the PMU sequence control for DRAM training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNSetPmuSequenceControlD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  PMU_SRAM_MSG_BLOCK_CZ *PmuSramMsgBlockPtr;
  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_CZ *) NBPtr->PsPtr->PmuSramMsgBlockPtr;
  //
  // Both images get program with same SequenceCtrl, each image would ignore the ones that did not apply to it.
  //
  if (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU 1D Training\n");
    // [0] 1=DevInit [1] 1=WrLvl Training, [2] 1=RxEnDly Training, [3] 1=1D Rd-Dqs Training, [4] 1=1D Wr-Dq Training.
    PmuSramMsgBlockPtr->SequenceCtrl = 0x1F;
  } else if (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D_2D_READ) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU 1D and 2D read Training\n");
    // [0] 1=DevInit [1] 1=WrLvl Training, [2] 1=RxEnDly Training, [3] 1=1D Rd-Dqs Training, [4] 1=1D Wr-Dq Training, [5] 1=2D Read Training.
    PmuSramMsgBlockPtr->SequenceCtrl = 0x3F;
  } else if (NBPtr->RefPtr->PmuTrainMode == PMU_TRAIN_1D_2D) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU 1D and 2D Training\n");
    // [0] 1=DevInit [1] 1=WrLvl Training, [2] 1=RxEnDly Training, [3] 1=1D Rd-Dqs Training, [4] 1=1D Wr-Dq Training, [5] 1=2D Read Training, [6] 1=2D Write Training.
    PmuSramMsgBlockPtr->SequenceCtrl = 0x7F;
  } else {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tPMU Train auto\n");
    //
    // PMU 1D Training
    //
    PmuSramMsgBlockPtr->SequenceCtrl = 0x1F;
  }
}