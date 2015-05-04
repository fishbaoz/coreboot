/* $NoKeywords:$ */
/**
 * @file
 *
 * mndctcz.c
 *
 * Northbridge DCT support for CZ
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/CZ)
 * @e \$Revision: 311625 $ @e \$Date: 2015-01-25 20:35:21 -0600 (Sun, 25 Jan 2015) $
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
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mu.h"
#include "mp.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "mncz.h"
#include "mnreg.h"
#include "mnpmu.h"
#include "mftds.h"
#include "merrhdl.h"
#include "cpuFamRegisters.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "Filecode.h"
#include "heapManager.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_CZ_MNDCTCZ_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define UNUSED_CLK 4
#define MAX_RD_DQS_DLY 0x1F

/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */
typedef struct {
  UINT32 BitField; ///< Bit field name of the timing
  UINT8 Min;               ///< Minimum value for timing
  UINT8 Max;               ///< Maximum value for timing
} SPD_TMG;

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
extern MEM_PSC_FLOW_BLOCK* memPlatSpecFlowArray[];
extern MEM_PSC_FLOW MemPGetODTPattern;

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function disables the DCT and mem clock for CZ
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDisableDctCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tDisable DCT %d\n", NBPtr->Dct);

  // Clear BFDphyMemPsSelEn prior PHY accesses
  MemNSetBitFieldNb (NBPtr, BFDphyMemPsSelEn, 0);

  // Tristate Vref on unused channel
  MemNSetBitFieldNb (NBPtr, BFGlobalVrefInSel, 2);

  // Tri-state drivers
  MemNSetBitFieldNb (NBPtr, RegTxImpedance, 0);

  // Program D18F2x9C_x00FA_F04A_dct[3:03:03:0] = 0080h.  Abyte/abit bc. Pwrdn Rcvrs used only in loopback mode
  MemNSetBitFieldNb (NBPtr, RegFAF04A, 0x80);

  // Phy LP2 state
  MemNSetBitFieldNb (NBPtr, RegPwrStateCmd, 4);

  // Ensure that D18F2x78_dct[1:0][ChanVal] = 0.
  MemNSetBitFieldNb (NBPtr, BFChanVal, 0);

  //Disable powergating the ss channel when the Dcc is powergated
  MemNSetBitFieldNb (NBPtr, BFDisDllShutdownSR, 1);

  // Disable DCT
  MemNSetBitFieldNb (NBPtr, BFDisDramInterface, 1);

  // Clear ZqToCsMap as it defaults to 0000_AA55h
  MemNSetBitFieldNb (NBPtr, RegZqToCsMap, 0);

  NBPtr->DCTPtr->Timings.DctMemSize = 0;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function uses calculated values from DCT.Timings structure to
 *      program its registers for CZ
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramCycTimingsCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST CTENTRY TmgAdjTab[] = {
    // BitField, Min, Max, Bias, Ratio_x2
    {BFTcl, 5, 19, 0, 2},
    {BFTrcd, 5, 26, 0, 2},
    {BFTrp, 5, 26, 0, 2},
    {BFTrtp, 4, 14, 0, 2},
    {BFTras, 8, 54, 0, 2},
    {BFTrc, 10, 78, 0, 2},
    {BFTwr, 5, 26, 0, 2},
    {BFTrrd, 4, 13, 0, 2},
    {BFTwtr, 4, 14, 0, 2},
    {BFFourActWindow, 6, 50, 0, 2}
  };

  DCT_STRUCT *DCTPtr;
  UINT8  *MiniMaxTmg;
  UINT16 *MiniMaxTrfc;
  UINT16 TrfcMax;
  UINT8  Value8;
  UINT8  ValFAW;
  UINT8  ValTrrd;
  UINT8  j;
  UINT8  Tcwl;
  UINT8 RdOdtTrnOnDly;
  UINTN BitField;
  MEM_PARAMETER_STRUCT *RefPtr;

  DCTPtr = NBPtr->DCTPtr;
  RefPtr = NBPtr->RefPtr;

  ValFAW = 0;
  ValTrrd = 0;
  TrfcMax = 0;

  //======================================================================
  // Program DRAM Timing values
  //======================================================================
  //
  MiniMaxTmg = &DCTPtr->Timings.CasL;
  for (j = 0; j < GET_SIZE_OF (TmgAdjTab); j++) {
    BitField = TmgAdjTab[j].BitField;

    if (BitField == BFTrp) {
      if (NBPtr->IsSupported[AdjustTrp]) {
        MiniMaxTmg[j] ++;
        if (MiniMaxTmg[j] < 5) {
          MiniMaxTmg[j] = 5;
        }
      }
    }

    if (MiniMaxTmg[j] < TmgAdjTab[j].Min) {
      MiniMaxTmg[j] = TmgAdjTab[j].Min;
    } else if (MiniMaxTmg[j] > TmgAdjTab[j].Max) {
      MiniMaxTmg[j] = TmgAdjTab[j].Max;
    }

    Value8 = (UINT8) MiniMaxTmg[j];

    if (BitField == BFTwr) {
      //
      // Override Twr on MemPstate 0 and DDR2400
      //
      if ((NBPtr->MemPstate == 0) && (NBPtr->DCTPtr->Timings.TargetSpeed == DDR2400_FREQUENCY)) {
        Value8 = 16;
      }

      if ((Value8 > 8) && ((Value8 & 1) != 0)) {
        ASSERT (FALSE);
      }
    } else if (BitField == BFTrrd) {
      ValTrrd = Value8;
    } else if (BitField == BFFourActWindow) {
      ValFAW = Value8;
    }

    if (BitField == BFTcl && Value8 > 18) {
      MemNSetBitFieldNb (NBPtr, BFTclAdj, Value8 - 18);
    }

    MemNSetBitFieldNb (NBPtr, BitField, Value8);
  }

  MiniMaxTrfc = &DCTPtr->Timings.Trfc0;
  for (j = 0; j < 4; j++) {
    if ((NBPtr->DCTPtr->Timings.DctDimmValid & (1 << j)) != 0) {
      if (MiniMaxTrfc[j] > TrfcMax) {
        TrfcMax = MiniMaxTrfc[j];
      }
      MemNSetBitFieldNb (NBPtr, BFTrfc0 + j, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, MiniMaxTrfc[j]));
    }
  }

  Tcwl = (UINT8) (DCTPtr->Timings.Speed / 133) + 2;
  Tcwl = (Tcwl > 5) ? Tcwl : 5;
  MemNSetBitFieldNb (NBPtr, BFTcwl, Tcwl);

  if (RefPtr->DramDoubleRefreshRate) {
    MemNSetBitFieldNb (NBPtr, BFTref, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 3900));      // 3.9 us
    MemNSetBitFieldNb (NBPtr, BFODTSDoubleRefreshRate, 0);
  } else {
    MemNSetBitFieldNb (NBPtr, BFTref, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 7800));      // 7.8 us
    MemNSetBitFieldNb (NBPtr, BFODTSDoubleRefreshRate, 1);
  }

  MemNSetBitFieldNb (NBPtr, BFTrcpage, NBPtr->DCTPtr->Timings.Trcpage);

  // Txs = max(5nCK, tRFC + 10ns)
  MemNSetBitFieldNb (NBPtr, BFTxs, MAX (0x30, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, TrfcMax + 10)));

  RdOdtTrnOnDly = (DCTPtr->Timings.CasL > Tcwl) ? (DCTPtr->Timings.CasL - Tcwl) : 0;
  MemNSetBitFieldNb (NBPtr, BFRdOdtTrnOnDly, RdOdtTrnOnDly);
  NBPtr->FamilySpecificHook[ProgOdtControl] (NBPtr, NULL);

  //
  // Program Tstag
  //
  // Tstag = BIOS: MAX(D18F2x204_dct[1:0]_mp[1:0][Trrd], CEIL(D18F2x204_dct[1:0]_mp[1:0][FourActWindow]/4))
  if (NBPtr->MemPstate == 0) {
    for (j = 0; j < 4; j++) {
      MemNSetBitFieldNb (NBPtr, BFTstag0 + j, MAX (ValTrrd,  (ValFAW + 3) / 4));
    }
  }

  //
  // Program Tmod
  //
  MemNSetBitFieldNb (NBPtr, BFTmod, (DCTPtr->Timings.Speed < DDR1866_FREQUENCY) ? 0x0C :
                                    (DCTPtr->Timings.Speed > DDR1866_FREQUENCY) ? 0x10 : 0x0E);
  //
  // Program Tzqcs and Tzqoper
  //
  // Tzqcs max(64nCK, 80ns)
  MemNSetBitFieldNb (NBPtr, BFTzqcs, MIN (6, (MAX (64, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 80)) + 15) / 16));
  // Tzqoper max(256nCK, 320ns)
  MemNSetBitFieldNb (NBPtr, BFTzqoper, MIN (0xC, (MAX (256, MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 320)) + 31) / 32));

  // Tdllk = DDR3: 512 clocks
  MemNSetBitFieldNb (NBPtr, BFTdllk, 512);

  // Program power management timing
  MemNDramPowerMngTimingCZ (NBPtr);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *     This function programs dram power management timing related registers
 *
 *     @param[in,out]  *NBPtr     - Pointer to the MEM_NB_BLOCK
 *
 *     @return    none
 * ----------------------------------------------------------------------------
 */
VOID
MemNDramPowerMngTimingCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  STATIC CONST UINT8 Tcke[] = {3, 3, 3, 4, 4, 5, 6, 6, 2};
  UINT8 Tck;

  // These timings are based on DDR3 spec
  // Tcksrx BIOS: 0
  Tck = (UINT8) MAX (5, (MemUnsToMemClk (NBPtr->DCTPtr->Timings.Speed, 10)));
  MemNSetBitFieldNb (NBPtr, BFTcksrx, 0);

  // Tcksre = max(5 nCK, 10 ns)
  MemNSetBitFieldNb (NBPtr, BFTcksre, MIN (0x3F, MAX (Tck, 5)));

  // Tckesr BIOS: 2
  MemNSetBitFieldNb (NBPtr, BFTckesr, 2);

  // Tcpded : 1
  MemNSetBitFieldNb (NBPtr, BFTcpded, (MemNGetBitFieldNb (NBPtr, BFDramType) == DRAM_TYPE_DDR3_CZ
                                    && MemNGetBitFieldNb (NBPtr, BFUnBuffDimm) == 1 ? 2 : 4));

  // Tpd = tCKE(min)
  //                               tCKE(min)
  // DDR-667   7.5ns = 3nCk     max(3nCK, 7.5ns)   = 3nCK
  // DDR-800   7.5ns = 3nCk     max(3nCK, 7.5ns)   = 3nCK
  // DDR-1066  5.625ns = 3nCK   max(3nCK, 5.625ns) = 3nCK
  // DDR-1333  5.625ns = 4nCK   max(3nCK, 4nCK)    = 4nCK
  // DDR-1600  5ns = 4nCK       max(3nCK, 4nCK)    = 4nCK
  // DDR-1866  5ns = 5nCK       max(3nCK, 5nCK)    = 5nCK
  // DDR-2133  5ns = 6nCK       max(3nCK, 6nCK)    = 6nCK
  // DDR-2400  5ns = 6nCK       max(3nCK, 6nCK)    = 6nCK
  // Tpd = tCKE(min)
  MemNSetBitFieldNb (NBPtr, BFTpd, Tcke[(NBPtr->DCTPtr->Timings.Speed / 133) - 2]);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function programs evaluate the conditions to program CsMux
 *
 *
 *     @param[in]   DimmAddrMap   - DimmAddrMap
 *     @param[in]   RankDef       - RankDef
 *
 *     @return          TRUE - CsMux should be set
 *     @return          FALSE - CsMux should be reset
 */

BOOLEAN
STATIC
EvaluateCsMuxCZ (
  IN       UINT8   DimmAddrMap,
  IN       UINT8   RankDef
  )
{
  if ((DimmAddrMap == 7 || DimmAddrMap == 9) && (RankDef == 3)) {
    return TRUE;
  } else if ((DimmAddrMap == 0xA || DimmAddrMap == 0xB || DimmAddrMap == 0xC) &&
             (RankDef == 2 || RankDef == 3)) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *      This function programs the memory controller with configuration parameters
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE - An Error value lower than AGESA_FATAL may have occurred
 *     @return          FALSE - An Error value greater than or equal to AGESA_FATAL may have occurred
 *     @return          NBPtr->MCTPtr->ErrCode - Contains detailed AGESA_STATUS value
 */

BOOLEAN
MemNAutoConfigCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 i;
  DIE_STRUCT *MCTPtr;
  DCT_STRUCT *DCTPtr;
  UINT32 PowerDownMode;
  UINT8 NumDimmslots;
  UINT8 RankDef[3];
  UINT8 DimmAddrMap[3];

  MCTPtr = NBPtr->MCTPtr;
  DCTPtr = NBPtr->DCTPtr;

  //
  //======================================================================
  // NB Config 1 Lo Register Value
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFDisDramScrub, 1);

  //
  //======================================================================
  // Build Dram Config Lo Register Value
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFUnBuffDimm, 1);
  MemNSetBitFieldNb (NBPtr, BFPendRefPaybackS3En, 1);
  MemNSetBitFieldNb (NBPtr, BFStagRefEn, 1);
  MemNSetBitFieldNb (NBPtr, BFIdleCycLimit, 8);

  // DimmEccEn
  if (NBPtr->RefPtr->EnableEccFeature && MCTPtr->Status[SbEccDimms]) {
    MemNSetBitFieldNb (NBPtr, BFDimmEccEn, 1);
  }

  // X4Dimm
  MemNSetBitFieldNb (NBPtr, BFX4Dimm, DCTPtr->Timings.Dimmx4Present);

  //
  //======================================================================
  // Build Dram Config Hi Register Value
  //======================================================================
  //
  PowerDownMode = 1;
  IDS_OPTION_HOOK (IDS_POWERDOWN_MODE, &PowerDownMode, &(NBPtr->MemPtr->StdHeader));
  MemNSetBitFieldNb (NBPtr, BFPowerDownMode, PowerDownMode);
  MemNSetBitFieldNb (NBPtr, BFOn3RdCasStallMode, 1);
  MemNSetBitFieldNb (NBPtr, BFMxMrsEn, 7);

  MemNSetBitFieldNb (NBPtr, BFDphyMemPsSelEn, 0);
  //
  //======================================================================
  // Build Dram MRS Register Value
  //======================================================================
  //
  MemNSetBitFieldNb (NBPtr, BFPchgPDModeSel, 1);
  MemNSetBitFieldNb (NBPtr, BFBurstCtrl, 1);

  //======================================================================
  // DRAM Controller Miscellaneous 2
  //======================================================================
  MemNSetBitFieldNb (NBPtr, BFPerRankTimingEn, 0);
  MemNSetBitFieldNb (NBPtr, BFFastSelfRefEntryDis, 0);
  MemNSetBitFieldNb (NBPtr, BFPrtlChPDEnhEn, 0);
  MemNSetBitFieldNb (NBPtr, BFPrtlChPDDynDly, 4);
  MemNSetBitFieldNb (NBPtr, BFPDPhyPSDis, 1);

  NumDimmslots = (UINT8) GetMaxDimmsPerChannel (NBPtr->RefPtr->PlatformMemoryConfiguration,
                                                NBPtr->MCTPtr->SocketId,
                                                NBPtr->ChannelPtr->ChannelID);
  for (i = 0; i < 3; i ++) {
    RankDef[i] = (UINT8) MemNGetBitFieldNb (NBPtr, BFRankDef0 + i);
    DimmAddrMap[i] = (UINT8) ((MemNGetBitFieldNb (NBPtr, BFDramBankAddrReg) >> (i * 4)) & 0xF);
  }
  // BIOS Recommendations for D18F2xA8_dct[1:0][CsMux45]
  if (NumDimmslots == 1 || NumDimmslots == 2) {
    MemNSetBitFieldNb (NBPtr, BFCSMux45, EvaluateCsMuxCZ (DimmAddrMap[0], RankDef[0]) ? 1 : 0);
  } else {
    MemNSetBitFieldNb (NBPtr, BFCSMux45, 0);
  }

  // BIOS Recommendations for D18F2xA8_dct[1:0][CsMux67]
  if (NumDimmslots == 2) {
    MemNSetBitFieldNb (NBPtr, BFCSMux67, EvaluateCsMuxCZ (DimmAddrMap[1], RankDef[1]) ? 1 : 0);
  } else if (NumDimmslots == 3) {
    if (EvaluateCsMuxCZ (DimmAddrMap[0], RankDef[0]) ||
        EvaluateCsMuxCZ (DimmAddrMap[1], RankDef[1]) ||
        EvaluateCsMuxCZ (DimmAddrMap[2], RankDef[2])) {
      MemNSetBitFieldNb (NBPtr, BFCSMux67, 1);
    } else {
      MemNSetBitFieldNb (NBPtr, BFCSMux67, 0);
    }
  } else {
    MemNSetBitFieldNb (NBPtr, BFCSMux67, 0);
  }

  //BIOS: IF (CsMux67 == 1 || CsMux45 == 1) THEN 1 ELSE 0 ENDIF.
  if (MemNGetBitFieldNb (NBPtr, BFCSMux45) == 1 || MemNGetBitFieldNb (NBPtr, BFCSMux67) == 1) {
    MemNSetBitFieldNb (NBPtr, BFExtendedParityEn, 1);
  }

  for (i = 0; i < 4; i++) {
    MemNSetBitFieldNb (NBPtr, BFTstag0 + i, 0x14);
  }

  MemNSetBitFieldNb (NBPtr, BFFlushWrOnS3StpGnt, 1);

  if (MemNGetPackageTypeCZ (NBPtr) == CZ_FP4) {
    MemNSetBitFieldNb (NBPtr, RegZqToCsMap, 0x0000AA55 & ((NBPtr->DCTPtr->Timings.CsPresent & 0xF)
                                                       | (((NBPtr->DCTPtr->Timings.CsPresent) & 0xF) << 8)));
    MemNSetBitFieldNb (NBPtr, RegCkeToCsMap, 0x0000AA55);
  } else {
    ASSERT (FALSE); // Package not supported.
  }

  if (NBPtr->ChannelPtr->RegDimmPresent != 0) {
    MemNSetBitFieldNb (NBPtr, BFSubMemclkRegDly, 1);
  }

  MemNSetBitFieldNb (NBPtr, BFGlobalVrefInSel, 1);

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs Mem Pstate registers
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       MemPstate - Mem Pstate
 *
 */

VOID
MemNProgramMemPstateRegD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 MemPstate
  )
{

  if (MemPstate == 0) {
    MemNSetBitFieldNb (NBPtr, BFMemClkFreq, MEMCLK_FREQ_TO_ID (NBPtr->DCTPtr->Timings.Speed));
    MemNSetBitFieldNb (NBPtr, BFMemClkFreqVal, 1);  /// @todo  BKDG to clarify if this bit needs to be set
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMPState 0 Frequency = %d MHz\n", (UINT16) MEMCLK_ID_TO_FREQ (MemNGetBitFieldNb (NBPtr, BFMemClkFreq)));
  } else {
    MemNSetBitFieldNb (NBPtr, BFM1MemClkFreq, MEMCLK_FREQ_TO_ID (DDR667_FREQUENCY));
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tMPState 1 Frequency = %d MHz\n", (UINT16) MEMCLK_ID_TO_FREQ (MemNGetBitFieldNb (NBPtr, BFM1MemClkFreq)));
  }

  //
  // Non-SPD Timings
  //
  MemNSetBitFieldNb (NBPtr, BFTrwtWB, 0x1C);
  MemNSetBitFieldNb (NBPtr, BFTrwtTO, 0x1B);
  MemNSetBitFieldNb (NBPtr, BFTwrrd, 0xB );

  MemNSetBitFieldNb (NBPtr, BFTrdrdBan, 0x2);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdSc, 0x1);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdScL, 0x1);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdDc, 0xB);
  MemNSetBitFieldNb (NBPtr, BFTrdrdDd, 0xB);

  MemNSetBitFieldNb (NBPtr, BFTwrwrSdSc, 0x1);
  MemNSetBitFieldNb (NBPtr, BFTwrwrSdScL, 0x1);
  MemNSetBitFieldNb (NBPtr, BFTwrwrSdDc, 0xB);
  MemNSetBitFieldNb (NBPtr, BFTwrwrDd, 0xB);

  MemNSetBitFieldNb (NBPtr, BFWrOdtOnDuration, DEFAULT_WR_ODT_CZ);
  MemNSetBitFieldNb (NBPtr, BFRdOdtOnDuration, DEFAULT_RD_ODT_CZ);
  MemNSetBitFieldNb (NBPtr, BFWrOdtTrnOnDly, DEFAULT_RD_ODT_TRNONDLY_CZ);

  MemNSetBitFieldNb (NBPtr, BFTmrd, 4);
  MemNSetBitFieldNb (NBPtr, BFEffArbDis, 0);
  MemNSetBitFieldNb (NBPtr, BFAggrPDDelay, 0x20);

  if (NBPtr->RefPtr->EnablePowerDown) {
    MemNSetTxpNb (NBPtr);
    MemNSetBitFieldNb (NBPtr, BFPchgPDEnDelay, 0x20);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the memory controller for training
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNConfigureDctForTrainingD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFGsyncDis, 0x0);
  MemNSetBitFieldNb (NBPtr, BFAddrCmdTriEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDisAutoRefresh, 0x1);
  MemNSetBitFieldNb (NBPtr, BFForceAutoPchg, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDynPageCloseEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFBankSwizzleMode, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDcqBypassMax, 0x0);
  MemNSetBitFieldNb (NBPtr, BFPowerDownEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFZqcsInterval, 0x0);
  MemNSetBitFieldNb (NBPtr, BFCmdThrottleMode, 0x0);
  MemNSetBitFieldNb (NBPtr, BFODTSEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFBwCapEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFBankSwap, 0x0);
  MemNSetBitFieldNb (NBPtr, BFTraceModeEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDramScrub, 0);
  MemNSetBitFieldNb (NBPtr, BFScrubReDirEn, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the memory controller for mission mode
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNConfigureDctNormalD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFAddrCmdTriEn, 0x1);
  MemNSetBitFieldNb (NBPtr, BFDisAutoRefresh, 0x0);
  MemNSetBitFieldNb (NBPtr, BFForceAutoPchg, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDynPageCloseEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFDcqBypassMax, 0x1F);
  MemNSetBitFieldNb (NBPtr, BFZqcsInterval, 0x2);
  MemNSetBitFieldNb (NBPtr, BFTraceModeEn, 0x0);
  MemNSetBitFieldNb (NBPtr, BFAggrPDEn, 1);
  MemNSetBitFieldNb (NBPtr, BFDataScrambleEn, 1);

  //
  // Power Down Enable
  //
  if (NBPtr->RefPtr->EnablePowerDown) {
    MemNSetBitFieldNb (NBPtr, BFPowerDownEn, 0x1);
  }

  IEM_SKIP_CODE (IEM_LATE_DCT_CONFIG) {
    MemNSetBitFieldNb (NBPtr, BFBankSwap, 0x1);
    MemNSetBitFieldNb (NBPtr, BFBankSwapAddr8En, 0x1);

    if (NBPtr->RefPtr->EnableBankSwizzle) {
      MemNSetBitFieldNb (NBPtr, BFBankSwizzleMode, 1);
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs turnaround timings for each DCT
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramTurnaroundTimingsD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // DDR3 Turnaround Parameters
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdSc, 1);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdScL, 1);
  MemNSetBitFieldNb (NBPtr, BFTrdrdSdDc, MemNGetBitFieldNb (NBPtr, PmuCD_R_R_SD));
  MemNSetBitFieldNb (NBPtr, BFTrdrdDd, MemNGetBitFieldNb (NBPtr, PmuCD_R_R));
  MemNSetBitFieldNb (NBPtr, BFTrdrdBan, MemNGetBitFieldNb (NBPtr, PmuTrdrdban_Phy));

  MemNSetBitFieldNb (NBPtr, BFTwrwrSdSc, 1);
  MemNSetBitFieldNb (NBPtr, BFTwrwrSdScL, 1);
  MemNSetBitFieldNb (NBPtr, BFTwrwrSdDc, MemNGetBitFieldNb (NBPtr, PmuCD_W_W_SD));
  MemNSetBitFieldNb (NBPtr, BFTwrwrDd, MemNGetBitFieldNb (NBPtr, PmuCD_W_W));
  MemNSetBitFieldNb (NBPtr, BFTwrrd, MemNGetBitFieldNb (NBPtr, PmuCD_W_R));

  MemNSetBitFieldNb (NBPtr, BFTrwtTO, MemNGetBitFieldNb (NBPtr, PmuCD_R_W));
  MemNSetBitFieldNb (NBPtr, BFTrwtWB, MemNGetBitFieldNb (NBPtr, BFTrwtTO) + 1);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function looks up platform specific config/timing values from the interface layer and
 *   programs them into DCT.
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @return          TRUE  - No errors
 *     @return          FALSE - Found errors on a channel and disable it
 */

BOOLEAN
MemNPlatformSpecCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8 i;
  BOOLEAN Result;
  Result = FALSE;

  IDS_OPTION_HOOK (IDS_BEFORE_PLAT_TABLES, NBPtr, &(NBPtr->MemPtr->StdHeader));

  // Extract recommended settings from tables
  MemPPreparePsTabLookupConditions (NBPtr);

  i = 0;
  while (memPlatSpecFlowArray[i] != NULL) {
    if (MemPLookupCadBusCfgTabs (NBPtr, (memPlatSpecFlowArray[i])->EntryOfTables)) {
      if (MemPLookupDataBusCfgTabs (NBPtr, (memPlatSpecFlowArray[i])->EntryOfTables)) {
        if (MemPGetODTPattern (NBPtr, (memPlatSpecFlowArray[i])->EntryOfTables)) {
          Result = TRUE;
          break;
        }
      }
    }
    i++;
  }

  if (memPlatSpecFlowArray[i] == NULL) {
    PutEventLog (AGESA_ERROR, MEM_ERROR_SAO_NOT_FOUND, NBPtr->Node, NBPtr->Dct, NBPtr->Channel, 0, &NBPtr->MemPtr->StdHeader);
    SetMemError (AGESA_ERROR, NBPtr->MCTPtr);
    if (!NBPtr->MemPtr->ErrorHandling (NBPtr->MCTPtr, NBPtr->Dct, EXCLUDE_ALL_CHIPSEL, &NBPtr->MemPtr->StdHeader)) {
      ASSERT (FALSE);
    }
  }

  // Disable channel if there are errors
  IDS_SKIP_HOOK (IDS_ENFORCE_PLAT_TABLES, NBPtr, &(NBPtr->MemPtr->StdHeader)) {
    if (!Result) {
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tDisable DCT%d due to unsupported DIMM configuration\n", NBPtr->Dct);
      if (!NBPtr->MemPtr->ErrorHandling (NBPtr->MCTPtr, NBPtr->Dct, EXCLUDE_ALL_CHIPSEL, &NBPtr->MemPtr->StdHeader)) {
        ASSERT (FALSE);
      }
      MemNDisableDctCZ (NBPtr);
    }
  }

  if (Result) {
    // Program settings
    MemNSetBitFieldNb (NBPtr, BFSlowAccessMode, (NBPtr->ChannelPtr->SlowMode) ? 1 : 0);
    MemNSetBitFieldNb (NBPtr, RegPhyRODTCSLow, NBPtr->ChannelPtr->PhyRODTCSLow);
    MemNSetBitFieldNb (NBPtr, RegPhyRODTCSHigh, NBPtr->ChannelPtr->PhyRODTCSHigh);
    MemNSetBitFieldNb (NBPtr, RegPhyWODTCSLow, NBPtr->ChannelPtr->PhyWODTCSLow);
    MemNSetBitFieldNb (NBPtr, RegPhyWODTCSHigh, NBPtr->ChannelPtr->PhyWODTCSHigh);

    // Override settings as requested
    MemProcessConditionalOverrides (NBPtr->RefPtr->PlatformMemoryConfiguration, NBPtr, PSO_ACTION_ODT, ALL_DIMMS);
  }

  return Result;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function retrieves the Max latency parameters
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 *     @param[in]  *MinDlyPtr - Pointer to variable to store the Minimum Delay value
 *     @param[in]  *MaxDlyPtr - Pointer to variable to store the Maximum Delay value
 *     @param[in]  *DlyBiasPtr - Pointer to variable to store Delay Bias value
 *     @param[in]  MaxRcvEnDly - Maximum receiver enable delay value
 */

VOID
MemNGetMaxLatParamsCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT16 MaxRcvEnDly,
  IN OUT   UINT16 *MinDlyPtr,
  IN OUT   UINT16 *MaxDlyPtr,
  IN OUT   UINT16 *DlyBiasPtr
  )
{
  UINT32 N;
  UINT32 T;
  UINT32 P;
  UINT32 MemClkPeriod;

  // 1. P = N = T = 0.
  P = N = T = 0;

  // Calculate a starting MaxRdLatency delay value by converting Tcl MEMCLKs + 2 MEMCLKs + 4 NCLKs + (18 - (ABYTE RdPtrInitVal/2)) MEMCLKs to NCLKs
  P = 2 * (MemNGetBitFieldNb (NBPtr, BFTcl) + 2 + (18 - (MemNGetBitFieldNb (NBPtr, PER_NB_PS (NBPtr->NbPstate, RegRdPtrInitVal)) + 1) / 2));

  // 10. N = (P/(MemClkFreq * 2) + T) * NclkFreq; Convert from PCLKs plus time to NCLKs.
  MemClkPeriod = 1000000 / ((NBPtr->MemPstate == 0) ? NBPtr->DCTPtr->Timings.Speed : DDR667_FREQUENCY);
  N = ((((P * MemClkPeriod + 1) / 2) + T) * NBPtr->NBClkFreq + 500000) / 1000000 + 4;

  // Calculate a starting MaxRdLatency delay value with steps 5, 9, and 12 excluded
  *MinDlyPtr = (UINT16) N;

  *MaxDlyPtr = 0x3FF;

  // 5. Program D18F2x1F[C:0]_dct[1:0][MaxRdLatency] = CEIL(current value + marginfactor).
  // margin factor = 1 NCLK + 3 MEMCLK + ((NBCOF/DdrRate >= 2) ? 4 MEMCLK - 4 NCLK : 0)
  N = 1;
  P = 6;
  if (NBPtr->NBClkFreq >= (UINT16) (((NBPtr->MemPstate == 0) ? NBPtr->DCTPtr->Timings.Speed : DDR667_FREQUENCY) * 2)) {
    P += 8;
    N += (((P * MemClkPeriod + 1) / 2) * NBPtr->NBClkFreq + 500000) / 1000000;
    N -= 4;
  } else {
    N += (((P * MemClkPeriod + 1) / 2) * NBPtr->NBClkFreq + 500000) / 1000000;
  }
  *DlyBiasPtr = (UINT16) N;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     The function resets Rcv Fifo
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *     @param[in]       Dummy - Dummy parameter
 *
 */

VOID
MemNResetRcvFifoCZ (
  IN OUT   struct _MEM_TECH_BLOCK *TechPtr,
  IN       UINT8 Dummy
  )
{
  // Carrizo does not need to reset Rcv FIFO.
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function modifies CS interleaving low address according to several conditions for CZ.
 *
 *     @param[in,out]  *NBPtr    - Pointer to the MEM_NB_BLOCK
 *     @param[in,out]  *LowBit   - Pointer to low bit
 *
 */

BOOLEAN
MemNCSIntLvLowAddrAdjCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN OUT   VOID *LowBit
  )
{
  UINT8 DctSelIntLvAddr;
  UINT8 BankSwapAddr8En;

  DctSelIntLvAddr = (UINT8) MemNGetBitFieldNb (NBPtr, BFDctSelIntLvAddr);
  BankSwapAddr8En =   (UINT8) MemNGetBitFieldNb (NBPtr, BFBankSwapAddr8En);

  //
  //D18F2x[5C:40]_dct[1:0][15:5] = BaseAddr[21:11] &&
  //D18F2x[6C:60]_dct[1:0][15:5] = AddrMask[21:11], so *LowBit needs to be added with 2.
  //
  *(UINT8 *) LowBit += 2;

  if (MemNGetBitFieldNb (NBPtr, BFBankSwap) == 1 && NBPtr->MemPtr->ParameterListPtr->EnableBankSwapOnly == FALSE) {
    if ((BankSwapAddr8En == 1) && (DctSelIntLvAddr == 4)) {
      *(UINT8 *) LowBit = 5;
    } else {
      *(UINT8 *) LowBit = 6;
    }
  }

  return TRUE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function caps speed based on battery life check.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 */
VOID
MemNCapSpeedBatteryLifeCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT16 SupportedFreq[] = {
    DDR2400_FREQUENCY,
    DDR2133_FREQUENCY,
    DDR1866_FREQUENCY,
    DDR1600_FREQUENCY,
    DDR1333_FREQUENCY,
    DDR1066_FREQUENCY,
    DDR800_FREQUENCY,
    DDR667_FREQUENCY
  };

  UINT32 FreqNumeratorInMHz;
  UINT32 FreqDivisor;
  UINT32 VoltageInuV;
  UINT32 NBFreq;
  UINT16 DdrFreq;
  UINT16 j;
  UINT8 Dct;
  INT8   NbPs;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;

  FamilySpecificServices = NULL;
  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, &(NBPtr->MemPtr->StdHeader));


  // Find the lowest supported NB Pstate
  NBFreq = 0;
  for (NbPs = 3; NbPs >= 0; NbPs--) {
    if (FamilySpecificServices->GetNbPstateInfo (FamilySpecificServices,
                                                  NBPtr->MemPtr->PlatFormConfig,
                                                  &NBPtr->PciAddr,
                                                  (UINT32) NbPs,
                                                  &FreqNumeratorInMHz,
                                                  &FreqDivisor,
                                                  &VoltageInuV,
                                                  &(NBPtr->MemPtr->StdHeader))) {
      if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) == 0) {
        NBFreq = FreqNumeratorInMHz / FreqDivisor;
        break;
      }
    }
  }

  ASSERT (NBFreq > 0);

  // Pick Max MEMCLK that is less than or equal to NCLK
  DdrFreq = DDR800_FREQUENCY;
  for (j = 0; j < GET_SIZE_OF (SupportedFreq); j++) {
    if (NBFreq >= SupportedFreq[j]) {
      DdrFreq = SupportedFreq[j];
      break;
    }
  }

  // Cap MemClk frequency to lowest NCLK frequency
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    MemNSwitchDCTNb (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.TargetSpeed > DdrFreq) {
      NBPtr->DCTPtr->Timings.TargetSpeed = DdrFreq;
    }
  }

}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Mode Register initialization.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNModeRegisterInitializationCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{

  UINT32 Mr0;
  UINT32 Mr1;
  UINT32 Mr2;

  Mr0 = MemNCalcMR0 (NBPtr);
  Mr1 = MemNCalcMR1 (NBPtr, 0);
  Mr2 = MemNCalcMR2 (NBPtr, 0);

  // Disable MR0[8](DLLReset) for switching between Memory PStates
  Mr0 &= 0xFEFF;
  MemNSetBitFieldNb (NBPtr, BFMxMr0, Mr0);
  MemNSetBitFieldNb (NBPtr, BFMxMr1, Mr1);
  MemNSetBitFieldNb (NBPtr, BFMxMr2, Mr2);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Configure disable DLL shutdown in self-refresh mode.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNConfigureDisDllShutdownSrCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNBrdcstSetUnConditionalNb (NBPtr, BFDisDllShutdownSR, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function trains Max latency for all dies
 *
 *     @param[in,out]   *TechPtr   - Pointer to the MEM_TECH_BLOCK
 *
 *     @return          TRUE -  No fatal error occurs.
 *     @return          FALSE - Fatal error occurs.
 */

BOOLEAN
MemTTrainMaxLatencyCZ (
  IN OUT   MEM_TECH_BLOCK *TechPtr
  )
{
  UINT32 TestAddrRJ16;
  UINT8 Dct;
  UINT8 ChipSel;
  UINT8  *PatternBufPtr;
  UINT8  *TestBufferPtr;
  UINT8  CurrentNbPstate;
  UINT16 CalcMaxLatDly;
  UINT16 MaxLatDly;
  UINT16 MaxLatLimit;
  UINT16 Margin;
  UINT16 CurTest;
  UINT16 _CL_;
  UINT8 TimesFail;
  UINT8 TimesRetrain;
  UINT16 i;

  MEM_DATA_STRUCT *MemPtr;
  DIE_STRUCT *MCTPtr;
  MEM_NB_BLOCK  *NBPtr;

  NBPtr = TechPtr->NBPtr;
  MCTPtr = NBPtr->MCTPtr;
  MemPtr = NBPtr->MemPtr;
  TechPtr->TrainingType = TRN_MAX_READ_LATENCY;
  TimesRetrain = DEFAULT_TRAINING_TIMES;
  IDS_OPTION_HOOK (IDS_MEM_RETRAIN_TIMES, &TimesRetrain, &MemPtr->StdHeader);

  IDS_HDT_CONSOLE (MEM_STATUS, "\nStart MaxRdLat training\n");
  // Set environment settings before training
  AGESA_TESTPOINT (TpProcMemMaxRdLatencyTraining, &(MemPtr->StdHeader));

  MemTBeginTraining (TechPtr);
  //
  // Initialize the Training Pattern
  //
  if (AGESA_SUCCESS != NBPtr->TrainingPatternInit (NBPtr)) {
    return (BOOLEAN) (MCTPtr->ErrCode < AGESA_FATAL);
  }
  TechPtr->PatternLength = (MCTPtr->Status[Sb128bitmode]) ? 6 : 3;
  //
  // Setup hardware training engine (if applicable)
  //
  NBPtr->FamilySpecificHook[SetupHwTrainingEngine] (NBPtr, &TechPtr->TrainingType);
  //
  // Adjust RdDqsDly used for MaxRdLatency calculation
  //
  NBPtr->FamilySpecificHook[AdjustRdDqsDlyForMaxRdLat] (NBPtr, NULL);

  MaxLatDly = 0;
  _CL_ = TechPtr->PatternLength;
  PatternBufPtr = TechPtr->PatternBufPtr;
  TestBufferPtr = TechPtr->TestBufPtr;
  //
  // Begin max latency training
  //
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    if (MCTPtr->Status[Sb128bitmode] && (Dct != 0)) {
      break;
    }

    NBPtr->SwitchDCT (NBPtr, Dct);

    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      IDS_HDT_CONSOLE (MEM_STATUS, "\tDct %d\n", Dct);
      if (TechPtr->FindMaxDlyForMaxRdLat (TechPtr, &ChipSel)) {
        TechPtr->ChipSel = ChipSel;
        if (NBPtr->GetSysAddr (NBPtr, ChipSel, &TestAddrRJ16)) {
          IDS_HDT_CONSOLE (MEM_STATUS, "\t\tCS %d\n", ChipSel);
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tWrite to address: %04x0000\n", TestAddrRJ16);

          // Write the test patterns
          AGESA_TESTPOINT (TpProcMemMaxRdLatWritePattern, &(MemPtr->StdHeader));
          NBPtr->WritePattern (NBPtr, TestAddrRJ16, PatternBufPtr, _CL_);

          // Sweep max latency delays
          NBPtr->getMaxLatParams (NBPtr, TechPtr->MaxDlyForMaxRdLat, &CalcMaxLatDly, &MaxLatLimit, &Margin);
          AGESA_TESTPOINT (TpProcMemMaxRdLatStartSweep, &(MemPtr->StdHeader));

          TimesFail = 0;
          ERROR_HANDLE_RETRAIN_BEGIN (TimesFail, TimesRetrain)
          {
            MaxLatDly = CalcMaxLatDly;
            for (i = 0; i < (MaxLatLimit - CalcMaxLatDly); i++) {
              NBPtr->SetBitField (NBPtr, BFMaxLatency0 + NBPtr->NbPstate, MaxLatDly);
              IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDly %3x", MaxLatDly);
              TechPtr->ResetDCTWrPtr (TechPtr, 6);

              AGESA_TESTPOINT (TpProcMemMaxRdLatReadPattern, &(MemPtr->StdHeader));
              NBPtr->ReadPattern (NBPtr, TestBufferPtr, TestAddrRJ16, _CL_);
              AGESA_TESTPOINT (TpProcMemMaxRdLatTestPattern, &(MemPtr->StdHeader));
              CurTest = NBPtr->CompareTestPattern (NBPtr, TestBufferPtr, PatternBufPtr, _CL_ * 64);
              NBPtr->FlushPattern (NBPtr, TestAddrRJ16, _CL_);

              if (NBPtr->IsSupported[ReverseMaxRdLatTrain]) {
                // Reverse training decrements MaxLatDly whenever the test passes
                // and uses the last passing MaxLatDly as left edge
                if (CurTest == 0xFFFF) {
                  IDS_HDT_CONSOLE (MEM_FLOW, "  P");
                  if (MaxLatDly == 0) {
                    break;
                  } else {
                    MaxLatDly--;
                  }
                }
              } else {
                // Traditional training increments MaxLatDly until the test passes
                // and uses it as left edge
                if (CurTest == 0xFFFF) {
                  IDS_HDT_CONSOLE (MEM_FLOW, "  P");
                  break;
                } else {
                  MaxLatDly++;
                }
              }
              IDS_HDT_CONSOLE (MEM_FLOW, "\n");
            } // End of delay sweep
            ERROR_HANDLE_RETRAIN_END ((MaxLatDly >= MaxLatLimit), TimesFail)
          }

          AGESA_TESTPOINT (TpProcMemMaxRdLatSetDelay, &(MemPtr->StdHeader));

          if (MaxLatDly >= MaxLatLimit) {
            PutEventLog (AGESA_ERROR, MEM_ERROR_MAX_LAT_NO_WINDOW, NBPtr->Node, NBPtr->Dct, NBPtr->Channel, 0, &NBPtr->MemPtr->StdHeader);
            SetMemError (AGESA_ERROR, MCTPtr);
            NBPtr->DCTPtr->Timings.CsTrainFail |= NBPtr->DCTPtr->Timings.CsPresent;
            MCTPtr->ChannelTrainFail |= (UINT32)1 << Dct;
            if (!NBPtr->MemPtr->ErrorHandling (MCTPtr, NBPtr->Dct, EXCLUDE_ALL_CHIPSEL, &NBPtr->MemPtr->StdHeader)) {
              ASSERT (FALSE);
              return FALSE;
            }
          } else {
            NBPtr->FamilySpecificHook[AddlMaxRdLatTrain] (NBPtr, &TestAddrRJ16);

            IDS_HDT_CONSOLE (MEM_FLOW, "\n\t\tFirst Passing MRL: %03x", MaxLatDly);
            MaxLatDly = MaxLatDly + Margin;
            if (NBPtr->IsSupported[ReverseMaxRdLatTrain]) {
              MaxLatDly++;  // Add 1 to get back to the last passing value
            }
            // Set final delays
            CurrentNbPstate = (UINT8) MemNGetBitFieldNb (NBPtr, BFCurNbPstate);
            ASSERT (CurrentNbPstate <= 3);
            NBPtr->ChannelPtr->DctMaxRdLat [CurrentNbPstate] = MaxLatDly;
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tShifted MaxRdLat: %03x", MaxLatDly);
            // Pick the smallest MaxRdLatency over multiple runs
            if (MaxLatDly < (&NBPtr->DCTPtr->Timings.MaxRdLat0)[NBPtr->NbPstate]) {
              (&NBPtr->DCTPtr->Timings.MaxRdLat0)[NBPtr->NbPstate] = MaxLatDly;
            } else {
              MaxLatDly = (&NBPtr->DCTPtr->Timings.MaxRdLat0)[NBPtr->NbPstate];
            }
            NBPtr->SetBitField (NBPtr, BFMaxLatency0 + NBPtr->NbPstate, MaxLatDly);
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\tFinal MaxRdLat: %03x\n", MaxLatDly);

          }
        }
      }
    }
  }

  // Restore environment settings after training
  MemTEndTraining (TechPtr);
  IDS_HDT_CONSOLE (MEM_FLOW, "\nEnd MaxRdLat training\n\n");
  //
  // Finalize the Pattern
  //
  NBPtr->TrainingPatternFinalize (NBPtr);
  return (BOOLEAN) (MCTPtr->ErrCode < AGESA_FATAL);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Get the MaxDdrRate
 *
 *     @param[in,out]   *NBPtr      - Pointer to the MEM_NB_BLOCK
 *     @param[in]       *OptParam   - Pointer to DdrMaxRate
 *
 */

BOOLEAN
MemNGetDdrMaxRateCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       VOID *OptParam
  )
{
  LOCATE_HEAP_PTR LocHeap;

  LocHeap.BufferHandle = AMD_MEM_DDR_MAX_RATE_HANDLE;
  if (HeapLocateBuffer (&LocHeap, &(NBPtr->MemPtr->StdHeader)) == AGESA_SUCCESS) {
    *(UINT8 *)OptParam = *(UINT8 *)LocHeap.BufferPtr;
  }

  return TRUE;
}
