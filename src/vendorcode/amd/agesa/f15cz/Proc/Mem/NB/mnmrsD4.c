/* $NoKeywords:$ */
/**
 * @file
 *
 * mnmrsd4.c
 *
 * DDR4 Mode Register Handling Routines
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
#include "merrhdl.h"
#include "cpuRegisters.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_MEM_NB_MNMRSD4_FILECODE

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
 *     This function calculates the value of MR0 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          MR0 value
 *
 */

UINT32
MemNCalcD4MR0 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // CL Value Table          9  10  11  12  13  14  15  16  17  18  19  20  21  22    23  24
  CONST UINT8 CLValues[] = { 0,  1,  2,  3,  4,  5,  6,  7, 13,  8, 14,  9, 15, 10, 0xFF, 11 };
  UINT32 MrsAddress;
  UINT8  TclIndex;
  UINT32 TclEnc;
  UINT32 TwrEnc;

  // Program MrsAddress[1:0]=burst length and control method
  // (BL):based on F2x[1,0]84[BurstCtrl]
  MrsAddress = MemNGetBitFieldNb (NBPtr, BFBurstCtrl);

  // Program MrsAddress[3]=1 (BT):interleaved
  MrsAddress |= (UINT16) 1 << 3;

  // Program MrsAddress[6:4,2]=read CAS latency
  TclIndex = (UINT8) MemNGetBitFieldNb (NBPtr, BFTcl);
  IDS_HDT_CONSOLE(MEM_FLOW, "\t\tTcl = %d\n", TclIndex);
  TclIndex = (TclIndex < 9) ? 0 : TclIndex - 9;

  ASSERT (TclIndex < sizeof (CLValues));
  TclEnc = CLValues[TclIndex];
  ASSERT (TclEnc < 16);

  MrsAddress |= (TclEnc & 0x0E) << 3;
  MrsAddress |= (TclEnc & 1) << 2;

  // Program MrsAddress[8] DllReset = 1
  MrsAddress |= 0x00000100;

  // Program MrsAddress[11:9] = Write Recovery and Read to Precharge
  TwrEnc = MemNGetBitFieldNb (NBPtr, BFTwr);
  TwrEnc = ((TwrEnc - 10 + 1 ) / 2);
  MrsAddress |= TwrEnc << 9;

  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR1 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       ChipSel - Rank to be trained
 *
 *     @return          MR1 value
 *
 */

UINT32
MemNCalcD4MR1 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  )
{
  UINT32 MrsAddress;
  UINT32 RttNom;

  // MrsAddress[0] = 1 (DLL Enable)
  MrsAddress = 1;

  // program MrsAddress[2:1] = output driver impedance control (DIC):
  // 00 = RZQ/7
  // 01 = RZQ/5
  MrsAddress |= ((UINT16) 0 << 1);

  // Program MrsAddres [4:3] AL: 0

  // Program MrsAddress[10:9] = Nominal termination resistance of ODT (RTT_NOM):
  // 0 = RTT_NOM Disable  4 = RZQ/1
  // 1 = RZQ/4            5 = RZQ/5
  // 2 = RZQ/2            6 = RZQ/3
  // 3 = RZQ/6            7 = RZQ/7
  // Different CS may have different RTT.
  RttNom = NBPtr->PsPtr->RttNom[ChipSel];
  IDS_OPTION_HOOK (IDS_MEM_DRAM_TERM, &RttNom, &NBPtr->MemPtr->StdHeader);
  MrsAddress |= ((RttNom & 0x7) << 8);

  // Program MrsAddress[12] = Output disable (QOFF): 0

  // Program MrsAddress[11] = TDQS:
  if ((NBPtr->DCTPtr->Timings.Dimmx4Present != 0) && (NBPtr->DCTPtr->Timings.Dimmx8Present != 0)) {
    if ((NBPtr->DCTPtr->Timings.Dimmx8Present & ((UINT8) 1 << (ChipSel >> 1))) != 0) {
      MrsAddress |= ((UINT16) 1 << 11);
    }
  }

  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR2 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       ChipSel - Rank to be trained
 *
 *     @return          MR2 value
 *
 */

UINT32
MemNCalcD4MR2 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  )
{
  UINT32 MrsAddress;
  UINT32 RttWr;
  UINT32 Tcwl;

  // Program MrsAddress[5:3]=CAS write latency (CWL):
  Tcwl = MemNGetBitFieldNb (NBPtr, BFTcwl);
  IDS_HDT_CONSOLE(MEM_FLOW, "\t\ttCWL = %d\n",Tcwl);
  MrsAddress = ( (Tcwl > 20) ? 20 :
               ( (Tcwl > 12) ? ((Tcwl - 12) / 2) + 9 :
               ( (Tcwl > 8)  ? (Tcwl - 9) :
               ( 9 )))) << 3;

  // Program MrsAddress[7:6] = Low Power Array Self Refresh (LP ASR)
  MrsAddress |= (3 << 6);

  // Program MrsAddress[11:9] = Dynamic termination during writes (RTT_WR):
  // 0 Dynamic ODT Off
  // 1 RZQ/2
  // 2 RZQ/1
  // 3 Hi-Z
  // 4 RZQ/3
  RttWr = NBPtr->PsPtr->RttWr[ChipSel];
  IDS_OPTION_HOOK (IDS_MEM_DYN_DRAM_TERM, &RttWr, &NBPtr->MemPtr->StdHeader);
  ASSERT (RttWr <= 4);
  IDS_HDT_CONSOLE(MEM_FLOW, "\t\tRttWR = %d\n",RttWr);
  MrsAddress |= (RttWr << 9);

  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR3 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          MR3 value
 *
 */

UINT32
MemNCalcD4MR3 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 MrsAddress;

  MrsAddress = 0;

  // A10:A9 Write CMD Latency when CRC and DM are enabled
  // 0 Normal (Fixed 1x)  5 Enable on the fly 2x
  // 1 Fixed 2x           6 Enable on the fly 4x
  // 2 Fixed 4x
  /// @todo: Need Recommendation for this

  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR4 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          UINT32   - MR4 value
 *
 */

UINT32
MemNCalcD4MR4 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 MrsAddress;

  MrsAddress = 0;
  // A9 Self Refresh Abort
  /// @todo: Turn on for Production BIOS. Possibly optimize tXS timing.
  /// MrsAddress |= (1 << 9);

  // A3 Temperature Controlled
  // Refresh Mode 0 = Disable 1 = Enable
  // A2 Temperature Controlled
  // Refresh Range 0 = Normal 1 = Extended
  /// @todo: Platform BIOS option for A2.
  MrsAddress |= (0x3 << 2);
  ///
  /// @todo: Need Reccomendation
  ///
  return MrsAddress;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR5 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       ChipSel - Rank to be trained
 *
 *     @return          MR5 value
 *
 */

UINT32
MemNCalcD4MR5 (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 ChipSel
  )
{
  UINT32 MrsAddress;
  UINT32 RttPark;

  MrsAddress = 0;
  // Program MrsAddress[A10] Data Mask
  /// @todo Needs to be Programmed based on DisDataMask, x4 Dimms, and platform option
  MrsAddress |= 1 << 10;


  // Program MrsAddress[8:6] = RTT_PARK
  RttPark = NBPtr->PsPtr->RttPark[ChipSel];
  ASSERT (RttPark <= 7);
  MrsAddress |= RttPark << 6;
  //
  /// @todo: Need Recommendation
  //
  return MrsAddress;
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function calculates the value of MR6 for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          UINT32 MR6 value
 *
 */

UINT32
MemNCalcD4MR6 (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 MrsAddress;
  UINT8 tCCD_L;

  MrsAddress = 0;
  // A7 VrefDQ Training Enable 0 = Disable(Normal operation Mode) 1 = Enable(Training Mode)
  // A6 VrefDQ Training Range (see Table 15)
  // A5:A0 VrefDQ Training Value (see Table 16)
  //    1 Dimm Installed:  0x18
  //    2 Dimms Installed: 0x23
  /// @todo: DDR4 - verify if these values are family specific and need to be abstracted
  if (NBPtr->ChannelPtr->Dimms == 1) {
    MrsAddress |= 0x18;
  } else {
    MrsAddress |= 0x23;
  }

  //A12:A10 tCCD_L
  tCCD_L = NBPtr->DCTPtr->Timings.TccdL - 4;
  MrsAddress |= (tCCD_L << 10);

  return MrsAddress;
}