/* $NoKeywords:$ */
/**
 * @file
 *
 * mnphyD4cz.c
 *
 * Northbridge Phy support for CZ with DDR4
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/NB/CZ)
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


#define FILECODE PROC_MEM_NB_CZ_MNPHYD4CZ_FILECODE
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
 *     This function does Phy Predriver initialization
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNPredriverInitD4CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  CONST STATIC TXPREPN_ENTRY TxPrePNDqDqs[] = {
    {DDR2133_FREQUENCY, 0x31, {0x8F, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x70, {0x8F, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x75, {0x8F, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x7F, {0x8F, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x31, {0x8F, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x70, {0x8F, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x75, {0x8F, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x7F, {0x8F, 0xFF, 0xFF}},
  };

  CONST STATIC TXPREPN_ENTRY TxPrePNCmdAddr[] = {
    {DDR2133_FREQUENCY, 0x07, {0x45, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x0F, {0x34, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x1F, {0x34, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x3F, {0x23, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x07, {0x45, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x0F, {0x34, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x1F, {0x34, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x3F, {0x23, 0xFF, 0xFF}},
  };

  CONST STATIC TXPREPN_ENTRY TxPrePNClk[] = {
    {DDR2133_FREQUENCY, 0x07, {0x56, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x0F, {0x56, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x1F, {0x56, 0xFF, 0xFF}},
    {DDR2133_FREQUENCY, 0x3F, {0x66, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x07, {0x67, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x0F, {0x67, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x1F, {0x67, 0xFF, 0xFF}},
    {DDR2400_FREQUENCY, 0x3F, {0x66, 0xFF, 0xFF}},
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
            TxSlewRate = TabPtr[j].TxPrePNVal[CONVERT_VDDIO_TO_ENCODED (NBPtr->RefPtr->DDRVoltage, DDR4_TECHNOLOGY)];
            ASSERT (TxSlewRate != 0xFF);
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
 *     This function programs the phy registers to configure CAD bus and data bus for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */

VOID
MemNProgramCadDataBusD4CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 RxRdPtrOffset;
  UINT16 TxRdPtrOffset;

  //------------------------------------------------
  // CAD bus configuration
  //------------------------------------------------
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCKE\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CKE, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CkeStrength << 6) | NBPtr->PsPtr->CkeStrength);
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCS\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CS, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CsOdtStrength << 6) | NBPtr->PsPtr->CsOdtStrength);
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tODT\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ODT, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->CsOdtStrength << 6) | NBPtr->PsPtr->CsOdtStrength);
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tAddrCmd\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_ADDR_CMD, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->AddrCmdStrength << 6) | NBPtr->PsPtr->AddrCmdStrength);
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tCLK\n");
  MemNSetBitFieldNb (NBPtr, PER_CAD_PAD (PAD_CLK, ALL_PADS, RegCadTxImpedance), (NBPtr->PsPtr->ClkStrength << 6) | NBPtr->PsPtr->ClkStrength);

  //------------------------------------------------
  // Data bus configuration
  //------------------------------------------------
  // Program D18F2x9C_x0002_0087_dct[1:0][DisAutoComp, DisPredriverCal] = {1,1}.
  MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 1);
  MemNSetBitFieldNb (NBPtr, BFDisPredriverCal, 1);

  // Only the value for a single control unit register is described. The values in the table should be broadcast to all
  // instances of registers of the same control unit type, unless otherwise noted.
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQ\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataTxImpedance), (NBPtr->PsPtr->DqStrength << 7) | NBPtr->PsPtr->DqStrength);
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tDQS\n");
  MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataTxImpedance), (NBPtr->PsPtr->DqsStrength << 7) | NBPtr->PsPtr->DqsStrength);
  //IDS_HDT_CONSOLE (MEM_FLOW, "\t\t\tProc ODT\n");
  if ((NBPtr->MCTPtr->LogicalCpuid.Revision & AMD_F15_CZ_A0) == 0) {
    //
    // CZ A0 or later
    //
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, BFEQStrenHiP), (NBPtr->PsPtr->OdtStrength >> 4) & 0x07);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, BFEQStrenHiP), (NBPtr->PsPtr->OdtStrength >> 4) & 0x07);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), NBPtr->PsPtr->OdtStrength & 0x0F);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), NBPtr->PsPtr->OdtStrength & 0x0F);
  } else {
    //
    // CZ A0
    //
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), NBPtr->PsPtr->OdtStrength);
    MemNSetBitFieldNb (NBPtr, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), NBPtr->PsPtr->OdtStrength);
  }

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
 *     This function initializes FIFO configurations for DDR4
 *
 *     @param[in,out]   *NBPtr   - Pointer to the MEM_NB_BLOCK
 *
 */
VOID
MemNPhyFifoConfigD4CZ (
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT16 Speed;
  UINT8  RdPtrInit;
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
      Speed = NBPtr->M1Speed;
      IDS_HDT_CONSOLE (MEM_FLOW, "\t\tNBP%d - M1\n", NbPs);
    }

    switch (Speed) {
    case DDR667_FREQUENCY:
    case DDR800_FREQUENCY:
    case DDR1066_FREQUENCY:
    case DDR1333_FREQUENCY:
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
    IDS_HDT_CONSOLE (MEM_FLOW, "\t\tRdPtrInit: %x\n", RdPtrInit);
    MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegRdPtrInitVal), RdPtrInit);
    MemNSetBitFieldNb (NBPtr, PER_NB_PS (NbPs, RegDataRdPtrInitVal), RdPtrInit);
  }

  // Clear DisAutoComp to complete predriver programming
  MemNSetBitFieldNb (NBPtr, BFDisAutoComp, 0);
}

/*----------------------------------------------------------------------------
 *                              LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */


