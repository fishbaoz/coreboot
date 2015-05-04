/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 15h Carrizo MSR tables with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 313537 $   @e \$Date: 2015-02-24 10:57:13 -0600 (Tue, 24 Feb 2015) $
 *
 */
/*
 ******************************************************************************
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
#include "cpuRegisters.h"
#include "cpuF15CzWorkaroundTable.h"
#include "Table.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZALLCORETABLES_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

STATIC CONST UINT8 ROMDATA F15CzAllCoreRegistersAfterApLaunch[] =
{
//  M S R    T a b l e s
// ----------------------
// MSR_CPUID_EXT_FEATS (0xC0011005)
// bit[51]  NodeId = 1
  MAKE_MSR_ENTRY (MSR_CPUID_EXT_FEATS, 0x0008000000000000, 0x0008000000000000),

// MSR_MC0_MISC (0x00000403)
// bits[52:55] LvtOffset = 1
  MAKE_MSR_ENTRY (MSR_MC0_MISC, 0x0010000000000000, 0x00F0000000000000),

// MSR_MC1_MISC (0x00000407)
// bits[52:55] LvtOffset = 1
  MAKE_MSR_ENTRY (MSR_MC1_MISC, 0x0010000000000000, 0x00F0000000000000),

// MSR_MC2_MISC (0x0000040B)
// bits[52:55] LvtOffset = 1
  MAKE_MSR_ENTRY (MSR_MC2_MISC, 0x0010000000000000, 0x00F0000000000000),

// MSR_MC4_MISC0 (0x00000413)
// bits[52:55] LvtOffset = 1
  MAKE_MSR_ENTRY (MSR_MC4_MISC0, 0x0010000000000000, 0x00F0000000000000),

// MSR_MC5_MISC (0x00000417)
// bits[52:55] LvtOffset = 1
  MAKE_MSR_ENTRY (MSR_MC5_MISC, 0x0010000000000000, 0x00F0000000000000),

// MSR_HWCR (0xC0010015)
// bit[28] SmuLock = 1
// bit[27] EffFreqReadOnlyLock = 1
// bit[12] HltXSpCycEn = 1
  MAKE_MSR_ENTRY (MSR_HWCR, 0x0000000018001000, 0x0000000018001000),

// CPU Watchdog Timer (0xC0010074)
// bits[6:3] CpuWdtCountSel = 1
// bit[0] CpuWdtEn = 1
  MAKE_MSR_ENTRY (CPU_WDT_CFG, 0x0000000000000009, 0x0000000000000079),

// MSR_OSVW_ID_Length (0xC0010140)
// bit[15:0] = 4
  MAKE_MSR_ENTRY (MSR_OSVW_ID_Length, 0x0000000000000004, 0x000000000000FFFF),

// MSR_LWP_CFG (0xC0000105)
// bit [39:32] LwpCoreId
  MAKE_FAM_SPECIFIC_WORKAROUND_ENTRY (F15CzLwpCoreIdId, 0),

// MSRC001_1000
// bit [12]                      = 1
  MAKE_CPU_REV_MSR_ENTRY (AMD_FAMILY_CZ, AMD_F15_CZ_A1, 0xC0011000, 0x0000000000001000, 0x0000000000001000),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

CONST REGISTER_TABLE ROMDATA F15CzAllCoreRegTableAfterApLaunch = {
  AllCores,
  F15CzAllCoreRegistersAfterApLaunch,
};


