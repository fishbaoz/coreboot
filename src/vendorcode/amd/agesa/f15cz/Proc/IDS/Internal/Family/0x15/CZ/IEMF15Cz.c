/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD IEM Option Specific Routines for F15 CZ
 *
 * Contains AMD AGESA debug macros and library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 312538 $   @e \$Date: 2015-02-09 16:53:54 +0800 (Mon, 09 Feb 2015) $
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
#include "AdvancedApi.h"
#include "amdlib.h"
#include "Ids.h"
#include "IdsLib.h"
#include "IdsInternal.h"
#include "IdsInternalLib.h"
#include "mm.h"
#include "mn.h"
#include "mnreg.h"
#include "mnPmuSramMsgBlockCZ.h"
#include "OptionMemory.h"
#include "mncz.h"
#include "mu.h"
#include "heapManager.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

///Please check IdsInternalLib.h for internal FILECODE
#define FILECODE PROC_IDS_INTERNAL_FAMILY_0X15_CZ_IEMF15CZ_FILECODE

/**
 *  This function is early device initialization sequence
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemEarlyDeviceInitD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      NBPtr->DCTPtr->Timings.CasL = 6;
      MemNSetBitFieldNb (NBPtr, BFTcl, 0x0C);
      MemNSetBitFieldNb (NBPtr, BFTcwl, 9);
    }
  }
}

/**
 *  This function is early device initialization sequence
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemEarlyDeviceInitD4CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      NBPtr->DCTPtr->Timings.CasL = 6;
      MemNSetBitFieldNb (NBPtr, BFTcl, 0x0C);
      MemNSetBitFieldNb (NBPtr, BFTcwl, 9);
    }
  }
}

/**
 *  This function is late DCT configuration
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemLateDctConfigD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      // Deassert Procedure:a
      //   MemResetL = 1
      MemNSetBitFieldNb (NBPtr, BFMemResetL, 1);
    }
  }
  MemUWait10ns (20000, NBPtr->MemPtr);

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {

      // Hard coded training
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x10);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F18F80 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F18F81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F1FF80 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x20);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F1FF81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F12F81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F13F81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x0);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F0F081 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x4);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F10028 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x0);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F0F028 | DCT_ACCESS_WRITE);

      // Need to delay the MEMCLK by 1UI
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 1);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x3100028 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 1);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x3200028 | DCT_ACCESS_WRITE);
    }
  }
}

/**
 *  This function is late DCT configuration
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemLateDctConfigD4CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb  (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      // Deassert Procedure:a
      //   MemResetL = 1
      MemNSetBitFieldNb (NBPtr, BFMemResetL, 1);
    }
  }
  MemUWait10ns (20000, NBPtr->MemPtr);

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {

      // Hard coded training
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x10);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F18F80 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F18F81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F1FF80 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x20);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F1FF81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F12F81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x40);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F13F81 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x0);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F0F081 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x4);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F10028 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 0x0);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x03F0F028 | DCT_ACCESS_WRITE);

      // Need to delay the MEMCLK by 1UI
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 1);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x3100028 | DCT_ACCESS_WRITE);
      MemNSetBitFieldNb (NBPtr, RegDctAddlData, 1);
      MemNSetBitFieldNb (NBPtr, RegDctAddlOffset, 0x3200028 | DCT_ACCESS_WRITE);
    }
  }
}


/**
 *  This function is late Max Rd Lat
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemLateMaxRdLatD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNSetBitFieldNb (NBPtr, BFMaxLatency, 0x72);
      MemNSetBitFieldNb (NBPtr, BFTrwtTO, 0xC);
    }
  }
}

/**
 *  This function is late Max Rd Lat
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemLateMaxRdLatD4CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 Dct;

  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      MemNSetBitFieldNb (NBPtr, BFMaxLatency, 0x72);
      MemNSetBitFieldNb (NBPtr, BFTrwtTO, 0xC);
    }
  }
}

/**
 *  This function is for things that need to be done immediately before loading the
 *  PMU Message Block into SRAM.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 **/
VOID
IemBeforePmuSramMsgBlockWriteCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  PMU_SRAM_MSG_BLOCK_CZ *PmuSramMsgBlockPtr;
  //
  // Get PMU SRAM Message Block Buffer Pointer
  //
  PmuSramMsgBlockPtr = (PMU_SRAM_MSG_BLOCK_CZ *) NBPtr->PsPtr->PmuSramMsgBlockPtr;
  //
  // Override Sequence Control
  //
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tEmulation Override: SequenceCtrl = 0x1\n");
  PmuSramMsgBlockPtr->SequenceCtrl = 0x01;
}