/* $NoKeywords:$ */
/**
 * @file
 *
 * mnpmu.c
 *
 * Common Northbridge function for PMU initialization
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB)
 * @e \$Revision: 311790 $ @e \$Date: 2015-01-27 13:03:49 +0800 (Tue, 27 Jan 2015) $
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
#include "amdlib.h"
#include "Ids.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mu.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "merrhdl.h"
#include "cpuRegisters.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_NB_MNPMU_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
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

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function resets the PMU
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPmuResetNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // 1. Program D18F2x9C_x0002_0099_dct[3:0][PmuReset,PmuStall] = 1,1.
  MemNSetBitFieldNb (NBPtr, BFPmuReset, 1);
  MemNSetBitFieldNb (NBPtr, BFPmuStall, 1);

  // 2. Program D18F2x9C_x0002_000E_dct[3:0][PhyDisable]=0.
  MemNSetBitFieldNb (NBPtr, BFPhyDisable, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function commands PMU to start DRAM initialization and/or DRAM training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNStartPmuNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\tStart PMU on image%d...\n", NBPtr->PmuFirmwareImage);
  // Initialize the protocol bit prior use.
  MemNSetBitFieldNb (NBPtr, BFRdy, 1);
  MemNSetBitFieldNb (NBPtr, BFRdy2, 1);

  // Clear PmuStall to start PMU
  MemNSetBitFieldNb (NBPtr, BFPmuStall, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function waits for PMU to complete
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - PMU completes
 *     @return          FALSE - PMU fails
 */

BOOLEAN
MemNPendOnPmuCompletionNb (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 UsMsg;
  UINT16 Us2Msg;
  INT16 Us2MsgIndex;
  INT16 Count;

  UsMsg = 0;
  Us2Msg = 0;
  Count = 0;

  while (UsMsg != USMSG_PMUQEMPTY) {
    // 1. Wait until D18F2x9C_x0002_0004_dct[3:0][UsRdy]==0.
    MemNPollBitFieldNb (NBPtr, BFUsRdy, 0, PCI_ACCESS_TIMEOUT, FALSE);

    // 2. Read D18F2x9C_x0002_0032_dct[3:0][Message]
    UsMsg = (UINT16) MemNGetBitFieldNb (NBPtr, BFUsMessage);
    IDS_HDT_CONSOLE (MEM_PMU, "\t\t\tUsMsg: %x\n", UsMsg);

    Us2MsgIndex = 0;

    // Print out debug data if available
    if (UsMsg == USMSG_US2MSGRDY) {
      IDS_HDT_CONSOLE (MEM_PMU, "\t\t\t\tStream message through US mailbox 2\n");
      do {
        // 1. Wait until D18F2x9C_x0002_0004_dct[3:0][Us2Rdy]==0.
        MemNPollBitFieldNb (NBPtr, BFUs2Rdy, 0, PCI_ACCESS_TIMEOUT, FALSE);

        // 2. Read D18F2x9C_x0002_0034_dct[3:0][Message]
        Us2Msg = (UINT16) MemNGetBitFieldNb (NBPtr, BFUs2Message);
        IDS_HDT_CONSOLE (MEM_PMU, "\t\t\t\tUs2Msg : %x\n", Us2Msg);

        if (Us2MsgIndex == 0) {
          // The first item received is the COUNT.
          Count = (INT16)Us2Msg;
          IDS_HDT_CONSOLE (MEM_PMU, "\t\t\t\tUs2Msg COUNT: %x\n", Count);
        }

        Us2MsgIndex++;

        // 3. Program D18F2x9C_x0002_0035_dct[3:0][Rdy] = 1.
        MemNSetBitFieldNb (NBPtr, BFRdy2, 1);

        // 4. Decrement COUNT and loop to step 1 until COUNT == -1.
      } while (--Count >= 0);
    }

    // 3. Program D18F2x9C_x0002_0033_dct[3:0][Rdy] = 1.
    MemNSetBitFieldNb (NBPtr, BFRdy, 1);

    if (UsMsg == USMSG_FAIL) {
      return FALSE;
    }
  }

  return TRUE;
}
