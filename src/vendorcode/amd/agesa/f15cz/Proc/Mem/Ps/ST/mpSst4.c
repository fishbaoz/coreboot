/* $NoKeywords:$ */
/**
 * @file},
 *
 * mpSst4.c},
 *
 * Platform specific settings for ST DDR4 SO-DIMM system
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA},
 * @e sub-project: (Mem/Ps/ST)
 * @e \$Revision: 312538 $ @e \$Date: 2015-02-09 16:53:54 +0800 (Mon, 09 Feb 2015) $
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
  * of these Materials shall constitute your acceptance of those terms and},
  * conditions.  If you do not agree to the terms and conditions of the Software},
  * License Agreement, please do not use any portion of these Materials.
  *
  * CONFIDENTIALITY:  The Materials and all other information, identified as
  * confidential and provided to you by AMD shall be kept confidential in
  * accordance with the terms and conditions of the Software License Agreement.
  *
  * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
  * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED},
  * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF},
  * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
  * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
  * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
  * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
  * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
  * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
  * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF},
  * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE},
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
  * available to you.  Additionally, AMD retains the right to modify the},
  * Materials at any time, without notice, and is not obligated to provide such
  * modified Materials to you.
  *
  * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
  * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
  * subject to the restrictions as set forth in FAR 52.227-14 and},
  * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the},
  * Government constitutes acknowledgement of AMD's proprietary rights in them.
  *
  * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
  * direct product thereof will be exported directly or indirectly, into any
  * country prohibited by the United States Export Administration Act and the},
  * regulations thereunder, without the required authorization from the U.S.
  * government nor will be used for any purpose prohibited by the same.
  * ***************************************************************************
  *
 */

#include "AGESA.h"
#include "AdvancedApi.h"
#include "mport.h"
#include "ma.h"
#include "Ids.h"
#include "cpuFamRegisters.h"
#include "cpuRegisters.h"
#include "mm.h"
#include "mn.h"
#include "mp.h"
#include "mu.h"
#include "OptionMemory.h"
#include "PlatformMemoryConfiguration.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)

#define FILECODE PROC_MEM_PS_ST_MPSST4_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
// For A1 or Greater tables
#define AMD_F15_ST_GTE_A1    (~AMD_F15_ST_A0 & AMD_F15_ALL)

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
/*
 *-----------------------------------------------------------------------------
 *                                EXPORTED FUNCTIONS
 *
 *-----------------------------------------------------------------------------
 */

// CAD Bus configuration for SO-DIMM
// Format :
// DimmPerCh, DDRrate,   VDDIO,      Dimm0,   Dimm1,  Slow, AddrCmdTmg, CKE, CS/ODT, AddrCmd, CLK driver strength
//
STATIC CONST PSCFG_CADBUS_ENTRY ROMDATA CadBusSTSODIMM4[] = {
  {1, DDR1333 + DDR1600 + DDR1866 + DDR2133 + DDR2400, V1_2, DIMM_SR,           NP,                0, 0x00000000, 0x1F, 0x1F, 0x1F, 0x1F},
  {1, DDR1333,                                         V1_2, DIMM_DR,           NP,                0, 0x00360000, 0x1F, 0x1F, 0x1F, 0x1F},
  {1, DDR1600 + DDR1866 + DDR2133 + DDR2400,           V1_2, DIMM_DR,           NP,                1, 0x00000000, 0x1F, 0x1F, 0x1F, 0x1F},
  {2, DDR1333 + DDR1600 + DDR1866,                     V1_2, NP,                DIMM_SR,           0, 0x00000000, 0x1F, 0x1F, 0x1F, 0x1F},
  {2, DDR1333,                                         V1_2, NP,                DIMM_DR,           0, 0x00360000, 0x1F, 0x1F, 0x1F, 0x1F},
  {2, DDR1333 + DDR1600 + DDR1866,                     V1_2, DIMM_SR,           NP,                0, 0x00000000, 0x1F, 0x1F, 0x1F, 0x1F},
  {2, DDR1333 + DDR1600 + DDR1866,                     V1_2, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, 1, 0x00000000, 0x1F, 0x1F, 0x3F, 0x1F},
  {2, DDR1333,                                         V1_2, DIMM_DR,           NP,                0, 0x00360000, 0x1F, 0x1F, 0x1F, 0x1F},
  {2, DDR1600 + DDR1866,                               V1_2, NP,                DIMM_DR,           1, 0x00000000, 0x1F, 0x1F, 0x1F, 0x1F},
  {2, DDR1600 + DDR1866,                               V1_2, DIMM_DR,           NP,                1, 0x00000000, 0x1F, 0x1F, 0x1F, 0x1F},

};

CONST PSC_TBL_ENTRY STCadBusTblEnSO4 = {
   {PSCFG_CADBUS, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_ST, AMD_F15_ALL}, PT_DONT_CARE, DDR4_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (CadBusSTSODIMM4) / sizeof (PSCFG_CADBUS_ENTRY),
   (VOID *)&CadBusSTSODIMM4
};


// Data Bus configuration for SO-DIMM (ST A0 Only)
// Format :
// DimmPerCh, DDRrate,                      VDDIO, Dimm0,   Dimm1,                       RttNom,     RttWr,     Rtt_Park,   DQ,   DQS, ODT strength
//
STATIC CONST PSCFG_DATABUS_ENTRY_D4 ROMDATA DataBusSTSODIMM4_A0[] = {
  {1, DDR1333,                               V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x70, 0x70, 0xF},
  {1, DDR1333,                               V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x70, 0x70, 0xF},
  {1, DDR1600 + DDR1866 + DDR2133 + DDR2400, V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x7F, 0x7F, 0xF},
  {1, DDR1600 + DDR1866 + DDR2133 + DDR2400, V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x7F, 0x7F, 0xF},
  {2, DDR1333,                               V1_2, NP,      DIMM_SR,                     RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x70, 0x70, 0xF},
  {2, DDR1333,                               V1_2, NP,      DIMM_DR,                     RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x70, 0x70, 0xF},
  {2, DDR1333,                               V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x70, 0x70, 0xF},
  {2, DDR1333 + DDR1600 + DDR1866,           V1_2, DIMM_SR + DIMM_DR, DIMM_DR + DIMM_SR, RTTNOM_40,  RTTWR_80,  RTTPRK_OFF, 0x75, 0x75, 0xF},
  {2, DDR1333,                               V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x70, 0x70, 0xF},
  {2, DDR1600 + DDR1866,                     V1_2, NP,      DIMM_SR,                     RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x7F, 0x7F, 0xF},
  {2, DDR1600 + DDR1866,                     V1_2, NP,      DIMM_DR,                     RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x7F, 0x7F, 0xF},
  {2, DDR1600 + DDR1866,                     V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x7F, 0x7F, 0xF},
  {2, DDR1600 + DDR1866,                     V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x7F, 0x7F, 0xF},

};

CONST PSC_TBL_ENTRY STDataBusTblEnSO4_A0 = {
   {PSCFG_DATABUS, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_ST, AMD_F15_ST_A0}, PT_DONT_CARE, DDR4_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (DataBusSTSODIMM4_A0) / sizeof (PSCFG_DATABUS_ENTRY_D4),
   (VOID *)&DataBusSTSODIMM4_A0
};

// Data Bus configuration for SO-DIMM (ST A1 or Later)
// Format :
// DimmPerCh, DDRrate,                      VDDIO, Dimm0,   Dimm1,                       RttNom,     RttWr,     Rtt_Park,   DQ,   DQS, ODT strength
//
STATIC CONST PSCFG_DATABUS_ENTRY_D4 ROMDATA DataBusSTSODIMM4[] = {
  {1, DDR1333,                               V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x70, 0x70, 0x30},
  {1, DDR1333,                               V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x70, 0x70, 0x30},
  {1, DDR1600 + DDR1866 + DDR2133 + DDR2400, V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x7F, 0x7F, 0x30},
  {1, DDR1600 + DDR1866 + DDR2133 + DDR2400, V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x7F, 0x7F, 0x30},
  {2, DDR1333,                               V1_2, NP,      DIMM_SR,                     RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x70, 0x70, 0x30},
  {2, DDR1333,                               V1_2, NP,      DIMM_DR,                     RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x70, 0x70, 0x30},
  {2, DDR1333,                               V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x70, 0x70, 0x30},
  {2, DDR1333 + DDR1600 + DDR1866,           V1_2, DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR,     ODT_40, RTTWR_80,  RTTPRK_OFF, 0x75, 0x75, 0x30},
  {2, DDR1333,                               V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x70, 0x70, 0x30},
  {2, DDR1600 + DDR1866,                     V1_2, NP, DIMM_SR,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x7F, 0x7F, 0x30},
  {2, DDR1600 + DDR1866,                     V1_2, NP, DIMM_DR,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x7F, 0x7F, 0x30},
  {2, DDR1600 + DDR1866,                     V1_2, DIMM_SR, NP,                          RTTNOM_OFF, RTTWR_OFF, RTTPRK_60,  0x7F, 0x7F, 0x30},
  {2, DDR1600 + DDR1866,                     V1_2, DIMM_DR, NP,                          RTTNOM_OFF, RTTWR_80,  RTTPRK_240, 0x7F, 0x7F, 0x30},
};

CONST PSC_TBL_ENTRY STDataBusTblEnSO4 = {
   {PSCFG_DATABUS, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_ST, AMD_F15_ST_GTE_A1}, PT_DONT_CARE, DDR4_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (DataBusSTSODIMM4) / sizeof (PSCFG_DATABUS_ENTRY_D4),
   (VOID *)&DataBusSTSODIMM4
};
