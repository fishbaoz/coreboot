/* $NoKeywords:$ */
/**
 * @file
 *
 * mpSmlfP4.c
 *
 * Platform specific settings for ML DDR3 SO-DIMM FP4 system
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project: AGESA
 * @e sub-project: (Mem/Ps/ML/FP4)
 * @e \$Revision: 284886 $ @e \$Date: 2014-02-16 20:35:38 -0600 (Sun, 16 Feb 2014) $
 *
 **/
/*****************************************************************************
  *
  * Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#define FILECODE PROC_MEM_PS_ML_FP4_MPSMLFP4_FILECODE
/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */
#define SOCKET_FP4_ML   3

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
//
// MemClkDis
//
STATIC CONST UINT8 ROMDATA MLSODdr3CLKDisFP4[] = {0xFF, 0xFF, 0x00 , 0x00, 0x00, 0x00, 0x00, 0x00};
CONST PSC_TBL_ENTRY MLClkDisMapEntSOFP4 = {
   {PSCFG_CLKDIS, SODIMM_TYPE + UDIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (MLSODdr3CLKDisFP4) / sizeof (UINT8),
   (VOID *)&MLSODdr3CLKDisFP4
};

//
// ODT tri-state
//
STATIC CONST UINT8 ROMDATA MLSODdr3ODTTriFP4[] = {0xFF, 0xFF, 0x00, 0x00};
CONST PSC_TBL_ENTRY MLSODdr3ODTTriEntFP4 = {
   {PSCFG_ODTTRI, SODIMM_TYPE + UDIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (MLSODdr3ODTTriFP4) / sizeof (UINT8),
   (VOID *)&MLSODdr3ODTTriFP4
};

//
// ChipSel tri-state
//
STATIC CONST UINT8 ROMDATA MLSODdr3CSTriFP4[] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
CONST PSC_TBL_ENTRY MLSODdr3CSTriEntFP4 = {
   {PSCFG_CSTRI, SODIMM_TYPE + UDIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (MLSODdr3CSTriFP4) / sizeof (UINT8),
   (VOID *)&MLSODdr3CSTriFP4
};

//
// CSMapCKE
//
STATIC CONST UINT32 ROMDATA MLSODdr3CSMapCKEFP4[] = {0x0000AA55};
CONST PSC_TBL_ENTRY MLSODdr3CSMapCKEEntFP4 = {
   {PSCFG_CSMAPCKE, SODIMM_TYPE + UDIMM_TYPE, _1DIMM + _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (MLSODdr3CSMapCKEFP4) / sizeof (UINT32),
   (VOID *)&MLSODdr3CSMapCKEFP4
};


// 2D training configuratrions
// Format :
// DimmPerCh,   DDRrate,   VDDIO,   Dimm0,   Dimm1,   Dimm2,   2D
//
STATIC CONST PSCFG_S2D_ENTRY MLUDdr3S2DFP4[] = {
 // DimmPerCh,Frequency,VDDIO,DIMM0,DIMM1,DIMM2,Enable2DTraining
  {_DIMM_NONE, ANY_SPEED, VOLT_ALL, NP + DIMM_SR + DIMM_DR, NP, NP, 1},
  {_1DIMM, ANY_SPEED, VOLT_ALL, NP + DIMM_SR + DIMM_DR, NP, NP, 1},
  {_2DIMM, ANY_SPEED, VOLT_ALL, NP + DIMM_SR + DIMM_DR, NP + DIMM_SR + DIMM_DR, NP, 1}
};
CONST PSC_TBL_ENTRY MLS2DTblEntUFP4 = {
   {PSCFG_S2D, UDIMM_TYPE + SODIMM_TYPE + SODWN_SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (MLUDdr3S2DFP4) / sizeof (PSCFG_S2D_ENTRY),
   (VOID *)&MLUDdr3S2DFP4
};

// ODT pattern for 1 DPC
// Format:
//  Dimm0,   RdODTCSHigh,   RdODTCSLow,   WrODTCSHigh,   WrODTCSLow
//
STATIC CONST PSCFG_1D_ODTPAT_ENTRY ML1DOdtPatFP4[] = {
  {DIMM_SR, 0x00000000, 0x00000000, 0x00000000, 0x00000001},
  {DIMM_DR, 0x00000000, 0x00000000, 0x00000000, 0x00000401},
};
CONST PSC_TBL_ENTRY ML1DOdtPatTblEntFP4 = {
   {PSCFG_ODT_PAT_1D, DT_DONT_CARE, _1DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (ML1DOdtPatFP4) / sizeof (PSCFG_1D_ODTPAT_ENTRY),
   (VOID *)&ML1DOdtPatFP4
};

// ODT pattern for 2 DPC
// Format:
//  Dimm0, Dimm1  RdODTCSHigh,   RdODTCSLow,   WrODTCSHigh,   WrODTCSLow
//
STATIC CONST PSCFG_2D_ODTPAT_ENTRY ML2DOdtPatFP4[] = {
  {NP, DIMM_SR, 0x00000000, 0x00000000, 0x00000000, 0x00020000},
  {NP, DIMM_DR, 0x00000000, 0x00000000, 0x00000000, 0x08020000},
  {DIMM_SR, NP, 0x00000000, 0x00000000, 0x00000000, 0x00000001},
  {DIMM_DR, NP, 0x00000000, 0x00000000, 0x00000000, 0x00000401},
  {DIMM_SR + DIMM_DR, DIMM_SR + DIMM_DR, 0x00000000, 0x01010202, 0x00000000, 0x09030603}
};
CONST PSC_TBL_ENTRY ML2DOdtPatTblEntFP4 = {
   {PSCFG_ODT_PAT_2D, DT_DONT_CARE, _2DIMM, {AMD_FAMILY_16_ML, AMD_F16_ALL}, SOCKET_FP4_ML, DDR3_TECHNOLOGY},
   sizeof (ML2DOdtPatFP4) / sizeof (PSCFG_2D_ODTPAT_ENTRY),
   (VOID *)&ML2DOdtPatFP4
};