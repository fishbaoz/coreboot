/* $NoKeywords:$ */
/**
 * @file
 *
 * mnphycz.c
 *
 * Northbridge Phy support for CZ
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/CZ)
 * @e \$Revision: 313730 $ @e \$Date: 2015-02-26 01:31:49 -0600 (Thu, 26 Feb 2015) $
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
#include "cpuRegisters.h"
#include "mport.h"
#include "mm.h"
#include "mn.h"
#include "mt.h"
#include "mu.h"
#include "mnreg.h"
#include "OptionMemory.h"       // need def for MEM_FEAT_BLOCK_NB
#include "mncz.h"
#include "PlatformMemoryConfiguration.h"
#include "merrhdl.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)


#define FILECODE PROC_MEM_NB_CZ_MNPHYCZ_FILECODE
/* features */
#include "mftds.h"
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

/// The structure of one entry of a TxPrePN table
typedef struct {
  UINT16 MaxSpeed;      ///< Highest memory speed for this setting
  UINT8  DrvStrenP;     ///< Driver strength
  UINT8  TxPrePNVal[3]; ///< Tx predriver value per voltage
} TXPREPN_ENTRY;

/// The structure describes how to extract data and program TxPrePN
typedef struct {
  BIT_FIELD_NAME BfName;   ///< Bit field to set
  TXPREPN_ENTRY  *TabPtr;  ///< Pointer to TxPrePN table
  UINT16         PsOffset; ///< Offset to find DrvStrenP from PS struct
} PROG_TXPREPN_STRUCT;

typedef struct {
  UINT8 BankAddr :4;
  UINT8 Addr13 :1;
  UINT8 Addr14 :1;
  UINT8 Addr15 :1;
  UINT8 Reserved :1;
} BANK_ADDR_MAP;

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

VOID
MemNTriStateUnusedPadCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

/*----------------------------------------------------------------------------
 *                            EXPORTED FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG UserOptions;
/* -----------------------------------------------------------------------------*/
/**
 *
 *      This function programs DDR mode for each Phy
 *
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       Mode     - DDR3/DDR4
 *
 */

VOID
MemNSetPhyDdrModeCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  )
{
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\tSet MajorMode=%d\n", Mode);
  // According to the type of DRAM attached, program the following phy mode
  // D18F2x9C_x0000_000E_dct[1:0][D4_Mode]
  MemNSetBitFieldNb (NBPtr, BFD4ModeAbyte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
  // D18F2x9C_x00[F,8:0]1_000E_dct[1:0][D4_Mode]
  MemNSetBitFieldNb (NBPtr, BFD4ModeDbyte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
  // D18F2x9C_x0002_000E_dct[1:0][D4_Mode]
  MemNSetBitFieldNb (NBPtr, BFD4ModeMByte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
  // D18F2x9C_x00[F,3:0]0_[F,B:0]04A_dct[1:0][MajorMode]
  MemNSetBitFieldNb (NBPtr, BFMajorModeAbyte, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x3);
  // D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]043_dct[1:0][MajorMode]
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFMajorModeDbyte), (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x3);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFMajorModeDbyte), (Mode == DRAM_TYPE_DDR3_CZ) ? 0x4 : 0x3);
  // D18F2x9C_x0004_00F0_dct[1:0][AcsmDdrMode(1=DDR4, 0=DDR3)] (ACSM)
  MemNSetBitFieldNb (NBPtr, BFAcsmDdrMode, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0 : 0x1);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs general Phy configuration registers
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPhyGenCfgCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT64 RODT;
  UINT64 WODT;
  UINT8  Cs;
  UINT8  PerCsFunction;
  UINT32 X4Dimm;
  CPUID_DATA CpuId;
  UINT8 RegValue;
  DCT_STRUCT *DCTPtr;

  DCTPtr = NBPtr->DCTPtr;

  X4Dimm = 0;
  PerCsFunction = 1;

  // Program D18F2x9C_x00[F,3:0]0_0009_dct[1:0][HiAddrMode].
  MemNSetBitFieldNb (NBPtr, RegHighAddrMode, 0);

  // Program D18F2x9C_x0[3,1:0][F,8:0]1_0010_dct[1:0][X4Dimm]
  if (MemNGetBitFieldNb (NBPtr, BFPerRankTimingEn) == 0) {
    PerCsFunction = 2;
  }
  for (Cs = 0; Cs < MAX_CS_PER_CHANNEL; Cs++) {
    X4Dimm |= (((DCTPtr->Timings.Dimmx4Present >> (Cs / 2)) & 1) << (Cs / PerCsFunction));
  }
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, RegDByteCtrl1), X4Dimm);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegDByteCtrl1), X4Dimm);

  // Program D18F2x9C_x00F4_00E[7:0]_dct[1:0][OdtRdPatCs, OdtWrPatCs] per 2.10.10.3.1 [DRAM ODT Pin Control].
  RODT = ((UINT64) NBPtr->ChannelPtr->PhyRODTCSHigh << 32) | NBPtr->ChannelPtr->PhyRODTCSLow;
  WODT = ((UINT64) NBPtr->ChannelPtr->PhyWODTCSHigh << 32) | NBPtr->ChannelPtr->PhyWODTCSLow;
  for (Cs = 0; Cs < MAX_CS_PER_CHANNEL; Cs++) {
    MemNSetBitFieldNb (NBPtr, RegAcsmOdtCtrl0 + Cs, (UINT32) ((((RODT >> (Cs * 8)) & 0xF) << 4) | ((WODT >> (Cs * 8)) & 0xF)));
  }

  // Program D18F2x9C_x00[F,8:0]1_0016_dct[1:0][POdtWidthDq, POdtWidthDqs, POdtStartDelayDq, POdtStartDelayDqs, ProcOdtOff, ProcOdtOn].
  MemNSetBitFieldNb (NBPtr, RegProcOdtTmg, 0x1244);

  // Program D18F2x9C_x00F0_0015_dct[3:0][VrefFilt]
  MemNSetBitFieldNb (NBPtr, RegVrefByteAbyte, 0);

  // Program D18F2x9C_x0002_0098_dct[1:0][CalD4D3] = 0
  MemNSetBitFieldNb (NBPtr, BFCalD4D3, 0);

  // Programs the following for maximum power savings prior to training:
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegTxControlDq), 0x0000);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegTxControlDq2), 0x8000);
  MemNSetBitFieldNb (NBPtr, RegCadByteDbgCtrl, 0x0022);
  MemNSetBitFieldNb (NBPtr, RegDataByteDbgCtrl, 0x0033);
  MemNSetBitFieldNb (NBPtr, RegMasterByteDbgCtrl, 0x0001);
  LibAmdCpuidRead (AMD_CPUID_FMF, &CpuId, &(NBPtr->MemPtr->StdHeader));
  RegValue = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28
  if (RegValue == 0) {
    // FP4 package
    MemNSetBitFieldNb (NBPtr, RegMasterD3MerrRcvrCntrl, 0x0000);
  }
  MemNSetBitFieldNb (NBPtr, RegMasterD3EvntMerr, 0x0000);
  MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDataDqDqsRcvCntrl3), 0x0052);

  if (X4Dimm == 0) {
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyDllPowerdown0), 0x800);
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyDllPowerdown2), 0x1000);
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyDllPowerdown2), 0x1001);
  }

  // Program static value for Abyte/DByte DllPumpCount and MaxDurDllNoLock, BIOS:0
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFCadDllPumpCount), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFCadMaxDurDllNoLock), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFDataDllPumpCount), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFDataMaxDurDllNoLock), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFCadDllPumpCount), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFCadMaxDurDllNoLock), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFDataDllPumpCount), 0);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFDataMaxDurDllNoLock), 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the phy registers according to the desired phy VDDIO voltage level
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       Mode     - DDR3/DDR4
 *
 */

VOID
MemNPhyVoltageLevelCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  )
{
  UINT8 Voltage;

  switch (NBPtr->RefPtr->DDR3Voltage) {
  case VOLT1_5:
    Voltage = 3;
    break;
  case VOLT1_35:
    Voltage = 2;
    break;
  case VOLT1_25:
    Voltage = 1;
    break;
  default:
    Voltage = 3;
    break;
  }

  // Program D18F2x9C_x00[F,3:0]0_[F,B:0]04A_dct[1:0][AVoltageLevel].
  MemNSetBitFieldNb (NBPtr, BFAVoltageLevel, Voltage);
  // Program D18F2x9C_x00[F,8:0]1_[F,B:0]04A_dct[1:0][VoltageLevel].
  MemNSetBitFieldNb (NBPtr, BFVoltageLevel, Voltage);
  // Program D18F2x9C_x0002_009B_dct[1:0]= If Ddr4Mode Then EA02h Else E024h
  MemNSetBitFieldNb (NBPtr, RegCalVRefs, 0xE024);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegVrefHSpeed), 0x40);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, RegVrefLpower), 0x40);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs DRAM channel frequency
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]       Mode     - DDR3/DDR4
 *
 */

VOID
MemNProgramChannelFreqCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8  Mode
  )
{
  UINT16 Freq;
  UINT16 PllMultDiv;
  UINT16 PMUClkDiv;

  // 1. Program D18F2x9C_x0002_0093_dct[1:0][PllRegWaitTime] = 4Bh.
  MemNSetBitFieldNb (NBPtr, RegPllRegWaitTime, 0x4B);

  // 2. Program D18F2x9C_x0002_0089_dct[1:0][PllLockTime] = C8h. Restore the default PLL lock time.
  MemNSetBitFieldNb (NBPtr, RegPllLockTime, 0xC8);

  // 3. Program D18F2x9C_x0002_0000_dct[1:0][PllMultDiv]. PMU Reset must be high.
  Freq = NBPtr->DCTPtr->Timings.TargetSpeed * 2;
  PllMultDiv = ((Freq / 100) * 0x100) + ((Freq % 100) / 20);
  MemNSetBitFieldNb (NBPtr, RegPllMemPs0, PllMultDiv);

  // 4. Program D18F2x9C_x0002_0080_dct[1:0][PMUClkDiv].
  // PMU must operate no faster than 533MHz
  PMUClkDiv = CEIL_DIV (((CEIL_DIV (Freq, 533) < 1) ? 0 : CEIL_DIV (Freq, 533)), 2);
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (0, BFPMUClkDiv),  PMUClkDiv);

  // 5. Program D18F2x9C_x0002_0001_dct[1:0][PllMultDiv] = If Ddr4ModeThen '1333 MT/s' else '667 MT/s'.
  MemNSetBitFieldNb (NBPtr, RegPllMemPs1, (Mode == DRAM_TYPE_DDR3_CZ) ? 0x0603 : 0x0D01);

  // 6. IF (Ddr3Mode) program D18F2x9C_x0102_0080_dct[1:0][PMUClkDiv].
  MemNSetBitFieldNb (NBPtr, PER_MEM_PS (1, BFPMUClkDiv), 1);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function does Phy Predriver initialization
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPredriverInitCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST STATIC TXPREPN_ENTRY TxPrePNDqDqs[] = {
    {DDR1066_FREQUENCY, 0x31, {0x8F, 0x8F, 0x8F}},
    {DDR1066_FREQUENCY, 0x70, {0x8F, 0x8F, 0x8F}},
    {DDR1066_FREQUENCY, 0x75, {0x8D, 0x8D, 0x8F}},
    {DDR1066_FREQUENCY, 0x7F, {0x12, 0x12, 0x12}},
    {DDR1600_FREQUENCY, 0x31, {0x8F, 0x8F, 0x8F}},
    {DDR1600_FREQUENCY, 0x70, {0x8F, 0x8F, 0x8F}},
    {DDR1600_FREQUENCY, 0x75, {0x8D, 0x8D, 0x8F}},
    {DDR1600_FREQUENCY, 0x7F, {0x12, 0x12, 0x12}},
    {DDR2400_FREQUENCY, 0x31, {0x8F, 0x8F, 0x8F}},
    {DDR2400_FREQUENCY, 0x70, {0x8F, 0x8F, 0x8F}},
    {DDR2400_FREQUENCY, 0x75, {0x8D, 0x8D, 0x8F}},
    {DDR2400_FREQUENCY, 0x7F, {0x12, 0x12, 0x23}},
  };

  CONST STATIC TXPREPN_ENTRY TxPrePNCmdAddr[] = {
    {DDR1066_FREQUENCY, 0x07, {0x12, 0x23, 0x23}},
    {DDR1066_FREQUENCY, 0x0F, {0x12, 0x12, 0x23}},
    {DDR1066_FREQUENCY, 0x1F, {0x11, 0x12, 0x12}},
    {DDR1066_FREQUENCY, 0x3F, {0x11, 0x12, 0x12}},
    {DDR1600_FREQUENCY, 0x07, {0x12, 0x23, 0x23}},
    {DDR1600_FREQUENCY, 0x0F, {0x12, 0x12, 0x23}},
    {DDR1600_FREQUENCY, 0x1F, {0x11, 0x12, 0x23}},
    {DDR1600_FREQUENCY, 0x3F, {0x11, 0x12, 0x23}},
    {DDR2400_FREQUENCY, 0x07, {0x23, 0x23, 0x34}},
    {DDR2400_FREQUENCY, 0x0F, {0x22, 0x23, 0x34}},
    {DDR2400_FREQUENCY, 0x1F, {0x22, 0x22, 0x34}},
    {DDR2400_FREQUENCY, 0x3F, {0x22, 0x22, 0x34}},
  };

  CONST STATIC TXPREPN_ENTRY TxPrePNClk[] = {
    {DDR1066_FREQUENCY, 0x07, {0x22, 0x33, 0x33}},
    {DDR1066_FREQUENCY, 0x0F, {0x22, 0x22, 0x33}},
    {DDR1066_FREQUENCY, 0x1F, {0x22, 0x22, 0x22}},
    {DDR1066_FREQUENCY, 0x3F, {0x22, 0x22, 0x22}},
    {DDR1600_FREQUENCY, 0x07, {0x33, 0x33, 0x44}},
    {DDR1600_FREQUENCY, 0x0F, {0x33, 0x33, 0x44}},
    {DDR1600_FREQUENCY, 0x1F, {0x33, 0x33, 0x44}},
    {DDR1600_FREQUENCY, 0x3F, {0x33, 0x33, 0x44}},
    {DDR2400_FREQUENCY, 0x07, {0x44, 0x56, 0x67}},
    {DDR2400_FREQUENCY, 0x0F, {0x45, 0x56, 0x67}},
    {DDR2400_FREQUENCY, 0x1F, {0x55, 0x56, 0x67}},
    {DDR2400_FREQUENCY, 0x3F, {0x55, 0x56, 0x66}},
  };

  CONST STATIC PROG_TXPREPN_STRUCT ProgTxPrePN[] = {
    {PER_CAD_PAD (PAD_CKE, ALL_PADS, RegCadTxSlewRate),      (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, CkeStrength)},
    {PER_CAD_PAD (PAD_CS, ALL_PADS, RegCadTxSlewRate),       (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, CsOdtStrength)},
    {PER_CAD_PAD (PAD_ODT, ALL_PADS, RegCadTxSlewRate),      (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, CsOdtStrength)},
    {PER_CAD_PAD (PAD_ADDR_CMD, ALL_PADS, RegCadTxSlewRate), (TXPREPN_ENTRY *)&TxPrePNCmdAddr, OFFSET_OF (MEM_PS_BLOCK, AddrCmdStrength)},
    {PER_CAD_PAD (PAD_CLK, ALL_PADS, RegCadTxSlewRate),      (TXPREPN_ENTRY *)&TxPrePNClk,     OFFSET_OF (MEM_PS_BLOCK, ClkStrength)},
    {PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataTxSlewRate),   (TXPREPN_ENTRY *)&TxPrePNDqDqs,   OFFSET_OF (MEM_PS_BLOCK, DqStrength)},
    {PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataTxSlewRate),  (TXPREPN_ENTRY *)&TxPrePNDqDqs,   OFFSET_OF (MEM_PS_BLOCK, DqsStrength)},
  };

  TXPREPN_ENTRY *TabPtr;
  UINT8 TxSlewRate;
  UINT8 *DrvStrenPtr;
  UINT8 i;
  UINT8 j;

  // Expect all TxPre tables have the same number of entries
  ASSERT ((GET_SIZE_OF (TxPrePNDqDqs) == GET_SIZE_OF (TxPrePNCmdAddr)) && (GET_SIZE_OF (TxPrePNCmdAddr) == GET_SIZE_OF (TxPrePNClk)));

  if (NBPtr->MemPstate == 0) {
    // Program D18F2x9C_x0002_0087_dct[3:0][DisAutoComp, DisPredriverCal] = {1,1}.
    MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 1);
    MemNSetBitFieldNb (NBPtr, BFDisPredriverCal, 1);

    TxSlewRate = 0;
    for (i = 0; i < GET_SIZE_OF (ProgTxPrePN); i++) {
      DrvStrenPtr = ((UINT8 *) NBPtr->PsPtr) + ProgTxPrePN[i].PsOffset;
      TabPtr = ProgTxPrePN[i].TabPtr;
      for (j = 0; j < GET_SIZE_OF (TxPrePNDqDqs); j++) {
        if (NBPtr->DCTPtr->Timings.Speed <= TabPtr[j].MaxSpeed) {
          if (*DrvStrenPtr <= TabPtr[j].DrvStrenP) {
            TxSlewRate = TabPtr[j].TxPrePNVal[CONVERT_VDDIO_TO_ENCODED (NBPtr->RefPtr->DDR3Voltage)];
            break;
          }
        }
      }
      ASSERT (j < GET_SIZE_OF (TxPrePNDqDqs));

      MemNSetBitFieldNb (NBPtr, ProgTxPrePN[i].BfName, ((TxSlewRate >> 4) & 0xF) | ((TxSlewRate << 4) & 0xF0));
    }

    // Program D18F2x9C_x0002_0087_dct[3:0][DisAutoComp] = 0.
    // DisAutoComp will be cleared after setting up FIFO pointers
  }
}
/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function programs the phy registers to configure CAD bus and data bus
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramCadDataBusD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 RxRdPtrOffset;
  UINT16 TxRdPtrOffset;
  UINT16 BankAddrReg;
  UINT16 BankAddrRegPerDimm;
  UINT8 Dimm;
  UINT8 i;
  UINT8 OrAddr13;
  UINT8 OrAddr14;
  UINT8 OrAddr15;
  BANK_ADDR_MAP BankAddrMap[10] = { {0x1, 0, 0, 0},
                                    {0x2, 0, 0, 0},
                                    {0x5, 1, 0, 0},
                                    {0x6, 0, 0, 0},
                                    {0x7, 1, 1, 0},
                                    {0x8, 1, 0, 0},
                                    {0x9, 1, 1, 0},
                                    {0xA, 1, 1, 1},
                                    {0xB, 1, 1, 1},
                                    {0xC, 1, 1, 1},
                                  };

  //------------------------------------------------
  // CAD bus configuration
  //------------------------------------------------
  MemNSetBitFieldNb (NBPtr, RegCadTxImpedance, 0);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCKE\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CKE, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CkeStrength << 6) | NBPtr->PsPtr->CkeStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCS\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CS, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CsOdtStrength << 6) | NBPtr->PsPtr->CsOdtStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tODT\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ODT, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CsOdtStrength << 6) | NBPtr->PsPtr->CsOdtStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tAddrCmd\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ADDR_CMD, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->AddrCmdStrength << 6) | NBPtr->PsPtr->AddrCmdStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCLK\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CLK, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->ClkStrength << 6) | NBPtr->PsPtr->ClkStrength);

  OrAddr13 = 0;
  OrAddr14 = 0;
  OrAddr15 = 0;
  BankAddrReg = (UINT16)NBPtr->GetBitField (NBPtr, BFDramBankAddrReg);
  for (Dimm = 0; Dimm <= 3; Dimm ++) {
    BankAddrRegPerDimm = (BankAddrReg >> (4 * Dimm)) & 0xF;
    if (BankAddrRegPerDimm != 0) {
      for (i = 0; i < GET_SIZE_OF (BankAddrMap); i ++) {
        if (BankAddrRegPerDimm == BankAddrMap[i].BankAddr) {
          OrAddr13 |= BankAddrMap[i].Addr13;
          OrAddr14 |= BankAddrMap[i].Addr14;
          OrAddr15 |= BankAddrMap[i].Addr15;
          break;
        }
      }
      ASSERT (i != GET_SIZE_OF (BankAddrMap));
    }
  }

  if (OrAddr13 == 0) {
    MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ADDR_CMD_A13, ALL_PADS, RegCadTxImpedance), 0);
  }
  if (OrAddr14 == 0) {
    MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ADDR_CMD_A14, ALL_PADS, RegCadTxImpedance), 0);
  }
  if (OrAddr15 == 0) {
    MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ADDR_CMD_A15, ALL_PADS, RegCadTxImpedance), 0);
  }

  //------------------------------------------------
  // Data bus configuration
  //------------------------------------------------
  // Program D18F2x9C_x0002_0087_dct[1:0][DisAutoComp, DisPredriverCal] = {1,1}.
  MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 1);
  MemNSetBitFieldNb (NBPtr, BFDisPredriverCal, 1);

  // Only the value for a single control unit register is described. The values in the table should be broadcast to all
  // instances of registers of the same control unit type, unless otherwise noted.
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQ\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataTxImpedance), (NBPtr->PsPtr->DqStrength << 7) | NBPtr->PsPtr->DqStrength);
  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQS\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataTxImpedance), (NBPtr->PsPtr->DqsStrength << 7) | NBPtr->PsPtr->DqsStrength);
  if (NBPtr->DCTPtr->Timings.Dimmx4Present == 0) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQS_L\n");
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS_L, ALL_BYTES, RegDataTxImpedance), 0);
  }

  IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tProc ODT\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), (NBPtr->PsPtr->OdtStrength << 4) | NBPtr->PsPtr->OdtStrength);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), (NBPtr->PsPtr->OdtStrength << 4) | NBPtr->PsPtr->OdtStrength);

  // Program D18F2x9C_x0002_0087_dct[1:0][DisAutoComp] = 0.
  MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 0);

  //------------------------------------------------
  // FIFO pointer init
  //------------------------------------------------
  // 3. D18F2x9C_x0[3,1:0][F,7:0]1_0028_dct[1:0][RxRdPtrOffset] as follows:
  //    Broadcast the value to all chips.
  //    RxRdPtrOffset = MIN(18, CASL).
  RxRdPtrOffset = MIN (18, NBPtr->DCTPtr->Timings.CasL);
  // 4. D18F2x9C_x0[3,1:0][F,7:0]1_0028_dct[1:0][TxRdPtrOffset] as follows:
  //    Broadcast the value to all chips.
  //    TxRdPtrOffset = MAX(5, CWL).
  TxRdPtrOffset = MAX (5, (NBPtr->DCTPtr->Timings.Speed / 133) + 2);
  MemNSetBitFieldNb (NBPtr, RegDataRdPtrOffset, (TxRdPtrOffset << 6) | RxRdPtrOffset);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function initializes FIFO configurations
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNPhyFifoConfigD3CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 Speed;
  UINT8  RdPtrInit;
  UINT8  DataRdPtrInit;
  UINT8  NbPs;
  UINT32 NbClkFreqMhz;
  UINT8  MinRdPtrInit;
  UINT8  CpuRevisionCz;

  RdPtrInit = 0;
  MinRdPtrInit = 0;
  CpuRevisionCz = (UINT8) MemNGetBitFieldNb (NBPtr, BFBaseModel);

  // 1. For each NbPstate, excluding the "NbPstate PMU" instance, program
  //    Broadcast the value to all timing groups and chips.
  //    D18F2x9C_x00[F,3:0]0_[F,3:0][8,3:0]2E_dct[1:0][RdPtrInitVal]= Table 38.
  //    D18F2x9C_x00[F,8:0]1_0[8,3:0]2E_dct[1:0][RdPtrInitVal] = Table 38.
  for (NbPs = 0; NbPs < 4; NbPs++) {
    NbClkFreqMhz = MemNGetNbClkFreqByPstateUnb (NBPtr, NbPs);
    if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) == 0) {
      Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tNBP%d - M0\n", NbPs);
    } else {
      Speed = DDR667_FREQUENCY;
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tNBP%d - M1\n", NbPs);
    }

    switch (Speed) {
    case DDR667_FREQUENCY:
    case DDR800_FREQUENCY:
    case DDR1066_FREQUENCY:
      RdPtrInit = 0x14;
      break;
    case DDR1333_FREQUENCY:
      RdPtrInit = (NbClkFreqMhz >= 800 && NbClkFreqMhz <= 1000) ? 0x10 : 0x14;
      break;
    case DDR1600_FREQUENCY:
    case DDR1866_FREQUENCY:
      RdPtrInit = 0x14;
      break;
    case DDR2133_FREQUENCY:
    case DDR2400_FREQUENCY:
      RdPtrInit = 0x10;
      break;
    default:
      ASSERT (FALSE);  // MemClk not supported.
    }

    DataRdPtrInit = RdPtrInit;
    MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegRdPtrInitVal), RdPtrInit);
    MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegDataRdPtrInitVal), DataRdPtrInit);
  }

  // Clear DisAutoComp to complete predriver programming
  MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 0);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    DRAM Phy Power Savings, tri-state unused pads, and DLLs.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNDramPhyPowerSavingsCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT8 DLL_RESET_RELOCK_SHIFT = 7;

  IDS_SKIP_HOOK (IDS_MEM_PHY_POWER_SAVING, NBPtr, &NBPtr->MemPtr->StdHeader) {
    // 1. Program DctPhyA41 as follows to disable unused pads.
    // DrvStrenP = DrvStrenP = 0.
    // See 2.10.4. Software does this for each unconnected pad in the package or each pad
    // connected to unused pins
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tTri-state unused pads\n");
    MemNTriStateUnusedPadCZ (NBPtr);

    // 2. Program D18F2x9C_x0[3,1:0][F,3:0]0_0014_dct[1:0][MaxDurDllNoLock] = 0.
    // DctPhyA78[DllResetRelock] must be set to 1 prior to writing this register
    // and then DctPhyA78[DllResetRelock] must be cleared after the register
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCAD DLL standby\n");
    MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, (1 << DLL_RESET_RELOCK_SHIFT));
    MemNSetBitFieldNb (NBPtr, BFCadMaxDurDllNoLock, 0);
    MemNSetBitFieldNb (NBPtr, RegDctPhyA78DllControl, 0);

    if (NBPtr->MemPstate == 1) {
      // 3. Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[1:0] as follows:
      // For M1 context program ODTStrenN = ODTStrenP = 0
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), 0);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), 0);
    }
  }
  // Restore cold reset default values over a warm reset so that a change from "ECC disabled"
  // to "ECC enabled" does not affect the PMU training environment
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, BFPowerDownRcvrDbyte), 0);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, BFPowerDownRcvrDbyte), 0);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910000), 0);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0);
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0091F04E), 0x1300);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *    Additional DRAM Phy Power Savings
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNAddlDramPhyPowerSavingsCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{

  IDS_SKIP_HOOK (IDS_MEM_PHY_POWER_SAVING, NBPtr, &NBPtr->MemPtr->StdHeader) {
    // 4. Program D18F2x9C_x00[F,3:0]0_[F,B:0]04A_dct[1:0][PowerDownRcvr] = 1.
    MemNSetBitFieldNb (NBPtr, BFPowerDownRcvrAbyte, 1);

    // 6. Disable unused DLL components in ABYTE as follows:
    // Program D18F2x9C_x0020_0077_dct[] = 07C0h. Disable Tx DLL in Abyte 2.
    // Program D18F2x9C_x0030_0077_dct[] = 07C0h. Disable Tx DLL in Abyte 3.
    // Program D18F2x9C_x0000_0077_dct[] = 07C4h.
    // Program D18F2x9C_x0010_0077_dct[] = 07C4h.
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00200077), 0x0);
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00300077), 0x0);
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00000077), 0x4);
    if (MemNGetBitFieldNb (NBPtr, BFParEn) != 0) {
      MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00100077), 0x0);
    } else {
      MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhy0x00100077), 0x8);
    }

    // 7. If ECC memory is not connected or is disabled, or the package does not support ECC,
    // then power down data chiplet 8 with the following:
    // Write to all memory P-state instances if available.
    // Program D18F2x9C_x00[F,8:0]1_[F,B:0]04A_dct[1:0][PowerDownRcvr]=1
    // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[1:0] = 07CFh.  Turn off all DLLs
    // Program D18F2x9C_x00[F,8:0]1_0000_dct[1:0][DByteEnable]=1. Actually "disables"
    // Program D18F2x9C_x00[F,8:0]1_0[F,2:0]77_dct[1:0] = 07DFh.  Turn off XCLK
    // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]04D_dct[1:0] = 0000h.  Disable Odt
    // Program D18F2x9C_x0[3,1:0][F,8:0]1_[F,B:0]041_dct[1:0] = 0000h.  Disable output driver impedance
    // Program D18F2x9C_x00[F,8:0]1_[F,B:0]04E_dct[1:0]= 0000h
    if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, BFPowerDownRcvrDbyte), 1);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, BFPowerDownRcvrDbyte), 1);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07CF);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910000), 0x0004);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD00910F77), 0x07DF);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F04D), 0x0000);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0391F041), 0x0000);
      MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDctPhyD0091F04E), 0x0000);
    }

    // 8. Power down unused receivers in data chips as follows:
    // Program D18F2x9C_x00F1_904A_dct[1:0][PowerDownRcvr]=1. DBI.
    // If x4 DIMMs are not present then program D18F2x9C_x00F1_B04A_dct[1:0][PowerDownRcvr]=1.
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1904A), 0x0100);
    if (MemNGetBitFieldNb (NBPtr, BFX4Dimm) == 0) {
      MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegDctPhyD00F1B04A), 0x0100);
    }

    // 10. Power down the PMU
    MemNDisablePmuCZ (NBPtr);

    // D18F2x9C_x0002_005D_dct[1:0][ForceHClk] = 0.
    MemNSetBitFieldNb (NBPtr, PER_INSTANCE (RegMisc5), 0);
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Enable DDR3 PHY calibrating
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNEnablePhyCalibrationCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xA3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0xB3);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 0);
  MemNSetBitFieldNb (NBPtr, BFCalOnce, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x83);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Waits for PHY calibrating to complete
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPendOnPhyCalibrateCompletionCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  // read CalBusy and discard result - this is needed to latch the
  // initial 'busy' status
  MemNGetBitFieldNb (NBPtr, RegCalBusy);

  // Wait until D18F2x9C_x20097 == 0.
  MemNPollBitFieldNb (NBPtr, RegCalBusy, 0, PCI_ACCESS_TIMEOUT, FALSE);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Set the rate for PHY calibrate
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNRateOfPhyCalibrateCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MemNSetBitFieldNb (NBPtr, BFCalInitMode, 0);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x03);
  MemNSetBitFieldNb (NBPtr, BFCalRun, 1);
  MemNSetBitFieldNb (NBPtr, RegCalRate, 0x13);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *     This function tri-state unused pads.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNTriStateUnusedPadCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST UINT8           PinType[4]  = {PSO_MEMCLK_DIS, PSO_CKE_TRI, PSO_ODT_TRI, PSO_CS_TRI};
  CONST UINT8           TabSize[4]  = {             4,           4,           4,          8};
  CONST UINT8           PadName[4]  = {       PAD_CLK,     PAD_CKE,     PAD_ODT,     PAD_CS};
  UINT8  *TabPtr;
  UINT8  i;
  UINT8  k;
  UINT8  Value;
  UINT8  PadNumber;
  BOOLEAN MemoryAllClocks;
  BOOLEAN MemoryAllCke;
  BOOLEAN MemoryAllCs;

  TabPtr = NULL;

  //===================================================================
  // Tristate DATA[8] if ECC is not supported
  //===================================================================
  if (MemNGetBitFieldNb (NBPtr, BFDimmEccEn) == 0) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state ECC Pad\n");
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ECC_BYTE, RegDataTxImpedance), 0);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ECC_BYTE, RegDataTxImpedance), 0);
  }

  MemoryAllClocks = UserOptions.CfgMemoryAllClocksOn;
  IDS_OPTION_HOOK (IDS_ALL_MEMORY_CLOCK, &MemoryAllClocks, &(NBPtr->MemPtr->StdHeader));
  MemoryAllCke = FALSE;
  IDS_OPTION_HOOK (IDS_ALL_MEMORY_CKE, &MemoryAllCke, &(NBPtr->MemPtr->StdHeader));
  MemoryAllCs = FALSE;
  IDS_OPTION_HOOK (IDS_ALL_MEMORY_CS, &MemoryAllCs, &(NBPtr->MemPtr->StdHeader));

  //===================================================================
  // Tristate unused MEMCLK, CKE, ODT and CS to save power
  //===================================================================
  //
  for (k = 0; k < GET_SIZE_OF (PinType); k++) {
    if (NBPtr->IsSupported[CheckFindPSOverideWithSocket]) {
      TabPtr = FindPSOverrideEntry (NBPtr->RefPtr->PlatformMemoryConfiguration, PinType[k], NBPtr->MCTPtr->SocketId, MemNGetSocketRelativeChannelNb (NBPtr, NBPtr->Dct, 0), 0,
                                    &(NBPtr->MCTPtr->LogicalCpuid), &(NBPtr->MemPtr->StdHeader));
    }

    if (TabPtr != NULL) {
      Value = 0;
      for (i = 0; i < TabSize[k]; i++) {
        if ((NBPtr->DCTPtr->Timings.CsPresent & TabPtr[i]) == 0) {
          Value |= (UINT8) (1 << i);
        }
      }

      // CAD/Pad numbers need tri-state
      while (Value != 0) {
        PadNumber = LibAmdBitScanForward (Value);
        Value &= ~ (1 << PadNumber);

        if (PadName[k] == PAD_CLK) {
          if (!MemoryAllClocks) {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state MEMCLK %d\n", PadNumber);
            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCadTxImpedance), 0);
            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCalTxImpedance), 0);
          }
        } else if (PadName[k] == PAD_CKE) {
          if (!MemoryAllCke) {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state CKE %d\n", PadNumber);

            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCadTxImpedance), 0);
            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCalTxImpedance), 0);
          }
        } else if (PadName[k] == PAD_CS) {
          if (!MemoryAllCs) {
            IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state CS %d\n", PadNumber);

            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCadTxImpedance), 0);
            MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCalTxImpedance), 0);
          }
        } else {
          IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tTri-state %s %d\n", ((PadName[k] == PAD_ODT) ? "MEMODT"  : "UnKnown"), PadNumber);

          MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCadTxImpedance), 0);
          MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PadName[k], PadNumber, RegCalTxImpedance), 0);
        }
      }
    }
  }

  MemFInitTableDrive (NBPtr, MTAfterPlatformSpec);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------*/
/**
 *
 *     Determine slow OPN mode is true if any valid NB-Pstates not tied
 *     to M1 has NCLK=800MHZ/M0=667MHZ or NCLK=800MHZ/M0=1333MHZ
 *
 *     @param[in,out]   *NBPtr    - Pointer to the MEM_NB_BLOCK
 *
 *     @return         TRUE  - slow_OPN mode, otherwise False
 *
 * ----------------------------------------------------------------------------
 */
BOOLEAN
STATIC
MemNSlowOpnModeCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT8  NbPs;
  UINT32 NbClkFreqMhz;
  UINT16 Speed;

  for (NbPs = 0; NbPs < 4; NbPs++) {
    if (MemNGetBitFieldNb (NBPtr, BFMemPstate0 + NbPs) != 1) {
      NbClkFreqMhz = MemNGetNbClkFreqByPstateUnb (NBPtr, NbPs);
      Speed = NBPtr->DCTPtr->Timings.TargetSpeed;
      if ((NbClkFreqMhz == 800) && ((Speed == DDR667_FREQUENCY) || (Speed == DDR1333_FREQUENCY))) {
        return TRUE;
      }
    }
  }

  return FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *
 *   This function gets or set DQS timing during training.
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *     @param[in]   TrnDly - type of delay to be set
 *     @param[in]   DrbnVar - encoding of Dimm-Rank-Byte-Nibble to be accessed
 *                  (use either DIMM_BYTE_ACCESS(dimm,byte) or CS_NBBL_ACCESS(cs,nibble) to use this encoding
 *     @param[in]   Field - Value to be programmed
 *     @param[in]   IsSet - Indicates if the function will set or get
 *
 *     @return      value read, if the function is used as a "get"
 */

UINT32
MemNcmnGetSetTrainDlyCZ (
  IN OUT   MEM_NB_BLOCK *NBPtr,
  IN       UINT8 IsSet,
  IN       TRN_DLY_TYPE TrnDly,
  IN       DRBN DrbnVar,
  IN       UINT16 Field
  )
{
  UINT32 Value;
  UINT8 Dimm;
  UINT8 Rank;
  UINT8 Byte;
  UINT8 Nibble;
  BIT_FIELD_NAME BitField;

  BitField = 0;
  Dimm = DRBN_DIMM (DrbnVar);
  Rank = DRBN_RANK (DrbnVar);
  Byte = DRBN_BYTE (DrbnVar);
  Nibble = DRBN_NBBL (DrbnVar);

  ASSERT (Dimm < (NBPtr->CsPerChannel / NBPtr->CsPerDelay));
  ASSERT (Byte <= ECC_DLY);
  if ((Byte == ECC_DLY) && (!NBPtr->MCTPtr->Status[SbEccDimms] || !NBPtr->IsSupported[EccByteTraining])) {
    // When ECC is not enabled
    if (IsSet) {
      // On write, ignore
      return 0;
    } else {
      // On read, redirect to byte 0 to correct fence averaging
      Byte = 0;
    }
  }

  switch (TrnDly) {
  case AccessRcvEnDly:
    BitField = PER_DATA_BYTE (PAD_DQS_H, Byte, RegDataRxDly);
    break;
  case AccessWrDqsDly:
    BitField = PER_DATA_BYTE (PAD_DQS_H, Byte, RegDataTxDly);
    break;
  case AccessWrDatDly:
    BitField = PER_DATA_BYTE (((Nibble == 0) ? PAD_DQ_L : PAD_DQ_H), Byte, RegDataTxDly);
    break;
  case AccessRdDqsDly:
    BitField = PER_DATA_BYTE (((Nibble == 0) ? PAD_DQ_L : PAD_DQ_H), Byte, RegDataRxDly);
    break;
  case AccessRdDqs2dDly:
    BitField = PER_DATA_BYTE (((Nibble == 0) ? PAD_DQ_L : PAD_DQ_H), Byte, RegDataRxDly);
    break;
  case AccessWrDat2dDly:
    BitField = PER_DATA_BYTE (((Nibble == 0) ? PAD_DQ_L : PAD_DQ_H), Byte, RegDataTxDly);
    break;
  default:
    IDS_ERROR_TRAP;
  }
  BitField = PER_DIMM (Dimm, BitField);
  Value = MemNGetBitFieldNb (NBPtr, BitField);

  if (IsSet) {
    MemNSetBitFieldNb (NBPtr, BitField, Field);
  }
  return Value;
}

