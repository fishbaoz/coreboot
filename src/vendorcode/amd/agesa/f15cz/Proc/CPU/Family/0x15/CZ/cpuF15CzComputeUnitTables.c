/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 15h Carrizo Shared MSR table with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 313986 $   @e \$Date: 2015-03-03 10:27:49 +0800 (Tue, 03 Mar 2015) $
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
#include "Table.h"
#include "cpuF15CzPowerMgmt.h"
#include "cpuF15CzWorkaroundTable.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZCOMPUTEUNITTABLES_FILECODE


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
STATIC CONST UINT8 ROMDATA F15CzComputeUnitRegistersAfterApLaunch[] =
{
//  M S R    T a b l e s
// ----------------------

// MSR_TOM2 (0xC001001D)
// bits[63:0] TOP_MEM2 = 0
  MAKE_MSR_ENTRY (MSR_TOM2, 0x0000000000000000, 0xFFFFFFFFFFFFFFFF),

// MSR_SYS_CFG (0xC0010010)
// bit[21] MtrrTom2En = 1
  MAKE_MSR_ENTRY (MSR_SYS_CFG, (1 << 21), (1 << 21)),

// MSR_CU_DEFER_ERR
// bits[7:4]     DeferredLvtOffset    = 2
  MAKE_MSR_ENTRY (0xC0000410, 0x0000000000000020, 0x00000000000000F0),

// MSR_CU_CFG (0xC0011023)
// bit[51] EarlyNbDataReturnEn = 1
// bit[49] ProcFeedbackEn = 1
  MAKE_MSR_ENTRY (MSR_CU_CFG, 0x000A000000000000, 0x000A000000000000),

// MSR_CU_CFG2 (0xC001102A)
// bit [50] RdMmExtCfgQwEn = 1
// bit [38] = 1
// bits[21:18] L2UpsizeCSWT[4:1] = 0
// bit [10] VicResyncChkEn = 1
// bit [9] L2UpsizeCSWT[0] = 0
// bits[4:0] L2UpsizeERT = 0
  MAKE_MSR_ENTRY (MSR_CU_CFG2, 0x0004004000000400, 0x00040040003C061F),

// MSR_CU_CFG3 (0xC001102B)
// bit[42] PwcDisableWalkerSharing = 0
// bit[22] PfcDoubleStride = 1
  MAKE_MSR_ENTRY (MSR_CU_CFG3, 0x0000000000400000, 0x0000040000400000),

// MSR_CU_PFTCFG (0xC001102F)
// bits[36]      PrefetchThrottlingEn = 0x0
// bits[35:30]   ThrottleLevel        = 0x0
// bits[29:24]   AccThresh2           = 0x0
// bits[23:18]   AccThresh1           = 0x0
// bits[17:12]   AccThresh0           = 0x0
// bits[11:6]    TrackThreshold       = 0x0F
// bits[5:0]     CaptureThreshold     = 0x0F
  MAKE_MSR_ENTRY (MSR_CU_PFTCFG, 0x00000000000003CF, 0x0000001FFFFFFFFF),

// MSR_CU_CBBCFG (0xC00110A1)
// bits[15:13]   TimeoutValueForCBBCLock = 1
// bits[12:10]   TimeoutValueForCBBAlloc = 1
// bit [9]       AllocCBBForShared = 1
// bit [8]       AllocCBBForExclusive = 1
// bit [7]       AllocCBBForCLOCK = 1
// bit [6]       AllocCBBForPFLock = 1
// bit [5]       AllocCBBForWccFill = 1
// bit [4]       AllocCBBForLsFill = 1
// bit [2]       EnCBBCLockHandling = 1
// bit [1]       EnCBBCoreBlocking = 1
// bit [0]       MasterEn = 1
  MAKE_MSR_ENTRY (MSR_CU_CBBCFG, 0x00000000000027F7, 0x000000000000FFF7),

// MSR C001_10A4 (0xC00110A4)
// bits[8:7]  = 1
// bits[6:5]  = 1
// bits[4:3]  = 1
// bit [2]    = 1
  MAKE_MSR_ENTRY (0xC00110A4, 0x00000000000000AC, 0x00000000000001FC),

// MSR_CU_CFG2 (0xC001102A)
// bits[37:36] ThrottleNbInterface[3:2] = 0
// bits[7:6]   ThrottleNbInterface[1:0] = 0
  MAKE_COMPUTE_UNIT_COUNTS_MSR_ENTRY ((COMPUTE_UNIT_RANGE_0 (1, 1) | COUNT_RANGE_NONE), MSR_CU_CFG2, 0x0000000000000000, 0x00000030000000C0),

// MSR_CU_CFG2 (0xC001102A)
// bits[37:36] ThrottleNbInterface[3:2] = 0
// bits[7:6]   ThrottleNbInterface[1:0] = 1
  MAKE_COMPUTE_UNIT_COUNTS_MSR_ENTRY ((COMPUTE_UNIT_RANGE_0 (2, 2) | COUNT_RANGE_NONE), MSR_CU_CFG2, 0x0000000000000040, 0x00000030000000C0),

// MSR C001_10C4 (0xC00110C4)
// bit[24]                            = 1
// bit[23]                            = 1
// bit[20]                            = 1
// bit[12]                            = 1
// bit[11]                            = 1
  MAKE_MSR_ENTRY (0xC00110C4, 0x01901800, 0x01901800),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

STATIC CONST UINT8 ROMDATA F15CzComputeUnitRegistersBeforeApLaunch[] =
{
// MSR_FP_CFG (0xC0011028)
// bits[44:41] DiDtCfg4 = D0F0xBC_xD021_1074[20:17]
// bit [40]    DiDtCfg3 = D0F0xBC_xD021_1074[16]
// bits[34:27] DiDtCfg1 = D0F0xBC_xD021_1074[13:6]
// bits[26:25] DiDtCfg2 = D0F0xBC_xD021_1074[15:14]
// bits[22:18] DiDtCfg0 = D0F0xBC_xD021_1074[5:1]
// bit [16]    DiDtMode = D0F0xBC_xD021_1074[0]
  MAKE_COPY_BIT_FIELD_ENTRY (COPY_BIT_FIELD_DEST (MsrRegister, MSR_FP_CFG, 44, 40), COPY_BIT_FIELD_SOURCE (SmuIndexReg, 0xD0211074, 20, 16)),
  MAKE_COPY_BIT_FIELD_ENTRY (COPY_BIT_FIELD_DEST (MsrRegister, MSR_FP_CFG, 34, 27), COPY_BIT_FIELD_SOURCE (SmuIndexReg, 0xD0211074, 13, 6)),
  MAKE_COPY_BIT_FIELD_ENTRY (COPY_BIT_FIELD_DEST (MsrRegister, MSR_FP_CFG, 26, 25), COPY_BIT_FIELD_SOURCE (SmuIndexReg, 0xD0211074, 15, 14)),
  MAKE_COPY_BIT_FIELD_ENTRY (COPY_BIT_FIELD_DEST (MsrRegister, MSR_FP_CFG, 22, 18), COPY_BIT_FIELD_SOURCE (SmuIndexReg, 0xD0211074, 5, 1)),
  MAKE_COPY_BIT_FIELD_ENTRY (COPY_BIT_FIELD_DEST (MsrRegister, MSR_FP_CFG, 16, 16), COPY_BIT_FIELD_SOURCE (SmuIndexReg, 0xD0211074, 0, 0)),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

CONST REGISTER_TABLE ROMDATA F15CzCURegTableAfterApLaunch = {
  ComputeUnitPrimary,
  F15CzComputeUnitRegistersAfterApLaunch,
};

CONST REGISTER_TABLE ROMDATA F15CzCURegTableBeforeApLaunch = {
  ComputeUnitPrimary,
  F15CzComputeUnitRegistersBeforeApLaunch,
};


