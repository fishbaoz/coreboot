/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 15h Carrizo PCI tables with values as defined in BKDG
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 315149 $   @e \$Date: 2015-03-20 15:05:00 +0800 (Fri, 20 Mar 2015) $
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
#include "Ids.h"
#include "amdlib.h"
#include "cpuRegisters.h"
#include "cpuF15CzWorkaroundTable.h"
#include "Table.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZPRIMARYCORETABLES_FILECODE


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

//  P C I    T a b l e s
// ----------------------

STATIC CONST UINT8 ROMDATA F15CzPrimaryCoresRegistersBeforeApLaunch[] =
{
// D18F0x68 - Link Transaction Control
// bits[22:21] DsNpReqLmt = 10b
// bit [19]    ApicExtSpur = 1
// bit [18]    ApicExtId = 1
// bit [17]    ApicExtBrdCst = 1
// bit [15]    LimitCldtCfg = 1
// bit [11]    RespPassPW = 1
// bit [10]    DisFillP = 0
// bit [4]     DisMTS = 1
// bit [3]     DisWrDwP = 0
// bit [2]     DisWrBP = 0
// bit [1]     DisRdDwP = 0
// bit [0]     DisRdBP = 0
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x68), 0x004E8810, 0x006E8C1F),

// MSR_NB_CFG (0xC001001F)
// bit [57]      DisFastTprWr = 1
// bits[54]      InitApicIdCpuIdLo = 1
// bits[18]      DisCstateBoostBlockPstateUp = 1
  MAKE_MSR_ENTRY (MSR_NB_CFG, 0x0240000000040000, 0x0240000000040000),

// D18F0x84 - Link Control
// bits[12]       IsocEn             = 0x1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x84), 0x00001000, 0x00001000),

// D18F0x90 - ONION Link Base Channel Buffer Count
// bit[30]     PReq[3] = 1
// bits[29:28] NpReqData[3:2] = 0
// bits[27:25] FreeData = 2
// bits[19:18] RspData = 1
// bits[17:16] NpReqData[1:0] = 1
// bits[15:12] ProbeCmd = 0
// bits[11:8]  RspCmd = 2
// bits[7:5]   PReq[2:0] = 3
// bits[4:0]   NpReqCmd = 0xA
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x90), 0x4405026A, 0x7E0FFFFF),

// D18F0x94 - ONION Link Isochronous Channel Buffer Count
// bits[28:27] IsocRspData = 0
// bits[26:25] IsocNpReqData = 1
// bits[24:22] IsocRspCmd = 0
// bits[21:19] IsocPReq = 0
// bits[18:16] IsocNpReqCmd = 1
// bits[15:8]  SecBusNum = 0 (D18F1xE0 [BaseBusNum])
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x94), 0x02010000, 0x1FFFFF00),

// D18F0x168 - Extended Link Transaction Control
// bit [20]     XcsSecPickerDstNcHt   = 1
// bit [19]                           = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x168), 0x00180000, 0x00180000),

// D18F3x6C - Data Buffer Count
// bits[30:28] IsocRspDBC = 2
// bits[18:16] UpRspDBC = 2
// bits[7:6]   DnRspDBC = 1
// bits[5:4]   DnReqDBC = 1
// bits[2:0]   UpReqDBC = 2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x6C), 0x20020052, 0x700700F7),

// D18F3x70 - SRI_to_XBAR Command Buffer Count
// bits[26:24]   IsocPreqCBC         = 0x0
// bits[18:16]   UpRspCBC            = 0x3
// bits[14:12]   DnPreqCBC           = 0x1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x70), 0x00031000, 0x07077000),

// D18F3x74 - XBAR_to_SRI Command Buffer Count
// bits[26:24]   IsocPreqCBC         = 0x1
// bits[23:20]   IsocReqCBC          = 0x1
// bits[19:16]   ProbeCBC            = 0xE
// bits[14:12]   DnPreqCBC           = 0x0
// bits[6:4]     DnReqCBC            = 0x0
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x74), 0x011E0000, 0x07FF7070),

// D18F3x78 - MCT to XBAR Buffer Count
// bits[21:16]   RspDBC              = 0x20
// bits[12:8]    ProbeCBC            = 0xC
// bits[5:0]     RspCBC              = 0x14
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x78), 0x00200C14, 0x003F1F3F),

// D18F3x7C - Free List Buffer Count
// bits[19:16]   Sri2XbarFreeXreqDBC = 0x4
// bits[15:12]   Sri2XbarFreeRspCBC  = 0x0
// bits[11:8]    Sri2XbarFreeXreqCBC = 0xA
// bits[5:0]     Xbar2SriFreeListCBC = 0x3A
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x7C), 0x00040A3A, 0x000FFF3F),

// D18F3x140 - SRI_to_XCS Token Count
// bits[24:20] FreeTok = 0xD
// bits[17:16] IsocRspTok = 1
// bits[13:12] IsocReqTok = 1
// bits[11:10] DnRspTok = 3
// bits[9:8]   UpRspTok = 1
// bits[5:4]   UpPreqTok = 2
// bits[1:0]   UpReqTok = 2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x140), 0x00D11D22, 0x01F33F33),

// D18F3x144 - MCT_to_XCS Token Count
// bits[8:4] RspTok = 0x11
// bits[3:0] ProbeTok = 4
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x144), 0x00000114,0x000001FF),

// D18F3x148 - ONION Link_to_XCS Token Count
// bits[31:30] FreeTok[3:2]       = 0
// bits[28]    IsocRspTok1        = 0
// bits[26]    IsocPreqTok1       = 0
// bits[24]    IsocReqTok1        = 0
// bits[23:22] ProbeTok1          = 0
// bits[21:20] RspTok1            = 0
// bits[19:18] PReqTok1           = 0
// bits[17:16] ReqTok1            = 0
// bits[15:14] FreeTok[1:0]       = 2
// bits[13:12] IsocRspTok0        = 0
// bits[11:10] IsocPreqTok0       = 1
// bits[9:8]   IsocReqTok0        = 1
// bits[7:6]   ProbeTok0          = 0
// bits[5:4]   RspTok0            = 2
// bits[3:2]   PReqTok0           = 2
// bits[1:0]   ReqTok0            = 2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x148), 0x0000852A, 0xD5FFFFFF),

// D18F3x17C - Extended Freelist Buffer Count
// bits[10:6] XbarToSrqFreeListBufCnt = 0x6
// bits[4:0]  SPQPrbFreeCBC = 0x12
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x17C), 0x00000192, 0x000007DF),

// D18F3x188 - NB Configuration 2
// bit [27] DisCpuWrSzDw64ReOrd   = 0x1
// bit [25]                       = 0x1
// bit [9]  DisL3HiPriFreeListAlloc = 0x1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x188), 0x0A000200, 0x0A000200),

// D18F3x1A0 - Core Interface Buffer Count
// bits[18:16] CpuToNbFreeBufCnt = 5
// bits[2:0]   CpuCmdBufCnt = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1A0), 0x00050001, 0x00070007),

// D18F5x214 - ONION3 Link Controller Buffer Count Link 0
// bits[25:24] On3DatPrivateLmt   = 0x2
// bits[17:16] On3ReqPrivateLmt   = 0x3
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x214), 0x02030000, 0x03030000),

// D18F5x218 - ONION3 Link Controller Buffer Count Link 1
// bits[25:24] On3DatPrivateLmt   = 0x2
// bits[17:16] On3ReqPrivateLmt   = 0x3
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x218), 0x02030000, 0x03030000),

// D18F5x224 - Extended MCT Buffer Count
// bits[30:28] On3UrgCmdMax       = 0x7
// bits[26:24] On3UrgDatMax       = 0x4
// bits[13:12] XbartoMctDatPrivateLmt = 0x2
// bits[9:8]   XbartoMctReqPrivateLmt = 0x2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x224), 0x74002200, 0x77003300),

// D18F5x22C - ONION3 Configuration
// bits[19:16] On3PrbLnkMap       = 0xA
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x22C), 0x000A0000, 0x000F0000),

// D18F0x90 - Upstream Base Channel Buffer Count - ONION Link
// bit [31] LockBc = 1
//
// NOTE: The entry is intended to be programmed after other bits of D18F0x[90, 94] is programmed and before D18F0x6C[30] is programmed.
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x90), 0x80000000, 0x80000000),

// D18F0x6C - Link Initialization Control
// bit [30] RlsLnkFullTokCntImm = 1
// bit [28] RlsIntFullTokCntImm = 1
//
// NOTE: The entry is intended to be after D18F0x[90, 94] and D18F3x[6C, 70, 74, 78, 7C, 140, 144, 148, 17C, 1A0] are programmed.
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C), 0x50000000, 0x50000000),

// D18F0x6C - Link Initialization Control
// bit [27] ApplyIsocModeEnNow = 1
//
// NOTE: The entry is intended to be after D18F0x6C[30, 28] are programmed.
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C), 0x08000000, 0x08000000),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

STATIC CONST UINT8 ROMDATA F15CzPrimaryCoreRegistersAfterApLaunch[] =
{
// D18F5x170 - Northbridge P-state Control
// bits[30]      NbPstateFidVidSbcEn = 0x1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x170), 0x40000000, 0x40000000),

// D18F0x6C - Link Initialization Control
// bit[0] RouteTblDis = 0
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x6C), 0x00000000, 0x00000001),

// D18F0x110 - Link Clumping Enable for ONION link
// bits [31:1]   ClumpEn             = 0x1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_0, 0x110), 0x00000002, 0xFFFFFFFE),

// D18F1xE0 - Configuration Map
// bits[31:24] BusNumLimit = F8
// bits[23:16] BusNumBase = 0
// bit [1]     WE = 1
// bit [0]     RE = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_1, 0xE0), 0xF8000003, 0xFFFF0003),

// D18F2x118 - Memory Controller Configuration Low Register
// bits[31:28] MctVarPriCntLmt = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_2, 0x118), 0x10000000, 0xF0000000),

// D18F2x1B0 - Extended Memory Controller Configuration Low Register
// bits[11] DisIoCohPref = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_2, 0x1B0), 0x00000800, 0x00000800),

// D18F3x44 - MCA NB Configuration
//
// bit[30]      SyncFloodOnDramAdrParErr = 1
// bit[27]      NbMcaToMstCpuEn = 1
// bit[20]      SyncFloodOnWDT = 1
// bits[11:9]   WDTCntSel[2:0] = 2
// bit[6]       CpuErrDis = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x44), 0x48100440, 0x48100E40),

// D18F3x84 - ACPI Power State Control High
// ACPI State S3
// bits[7:5] ClkDivisorSmafAct4 = 7
// bit [1] NbLowPwrEnSmafAct4   = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x84), 0x000000E2, 0x000000E2),

// D18F3xA8 - Pop Up and Down P-states
// bit[28] = 0
// bit[27] = 0
// bit[25] HwCofVidEn = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0xA8), 0x02000000, 0x1A000000),

// D18F3xD4 - Clock Power Timing Control 0
// bit [31]    NbClkDivApplyAll = 1
// bits[30:28] NbClkDiv = 4
// bits[11:8]  ClkRampHystSel = 0xF
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0xD4), 0xC0000F00, 0xF0000F00),

// D18F3xD8 - Clock Power/Timing Control 1
// bit [31]    = 0
// bits[6:4]   VSRampSlamTime = 0x4
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0xD8), 0x00000040, 0x80000070),

// D18F3xDC - Clock Power/Timing Control 2
// bit [26]      IgnCpuPrbEn = 1
// bit [7]       FastSprSaveRestEn = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0xDC), 0x04000080, 0x04000080),

// D18F3x160 - NB Machine Check Misc 0
// bits[23:20] LvtOffset = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x160), 0x00100000, 0x00F00000),

// D18F3x168 - NB Machine Check Misc 1
// bits[23:20] LvtOffset = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x168), 0x00100000, 0x00F00000),

// D18F3x180 - Extended NB MCA Configuration
// bit [30] NbDefIntEn = 1
// bit [28] SyncFloodOnCC6DramUcErr = 1
// bit [24] McaLogErrAddrWdtErr = 1
// bit [21] SyncFloodOnCpuLeakErr = 1
// bit [20] SyncFloodOnL3OrGpuLeakErr = 0 (if bit[12] = 1)
// bit [19] PwP2pDatErrRmtPropDis = 1
// bit [12] = 1
// bit [11] = 1
// bit [9]  SyncFloodOnUCNbAry = 1
// bit [8]  SyncFloodOnProtErr = 1
// bit [7]  SyncFloodOnTgtAbortErr = 1
// bit [5]  DisPciCfgCpuMstAbortRsp = 1
// bit [0]  McaLogUsPwDatErrEn = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x180), 0x51281BA1, 0x51381BA1),

// D18F3x198 - MCA NB Control Mask (MC4_CTL_MASK)
// bits[30]                          = 0x0
// bits[29]      WrCrcErr            = 0x0
// bits[28]      G5CrcCorrErr        = 0x0
// bits[26]      NbArrayPar          = 0x1
// bits[25]      McaUsPwDatErr       = 0x0
// bits[23]      CrcErr[3]           = 0x0
// bits[22:19]   RtryHt              = 0x0
// bits[18]      DramPar             = 0x0
// bits[16]      Prot                = 0x0
// bits[15]      NBArrayUnc          = 0x0
// bits[14]      NBArrayCor          = 0x0
// bits[13]      DevErr              = 0x0
// bits[12]      WDTRpt              = 0x0
// bits[11]      AtomicRMW           = 0x0
// bits[10]      GartTblWk           = 0x0
// bits[8]                           = 0x0
// bits[7:5]     SyncPkt[2:0]        = 0x0
// bits[4:2]     CrcErr[2:0]         = 0x0
// bits[1]       UnCorrEcc           = 0x0
// bits[0]       CorrEcc             = 0x0
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x198), 0x04000000, 0x76FDFDFF),

// D18F3x1CC - IBS Control
// bits[8] LvtOffsetVal = 1
// bits[3:0] LvtOffset = 0
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x1CC), 0x00000100, 0x0000010F),

// D18F3x200 - Performance Mode Control Register
// bit [3] EnMcqPrbPickThrottle = 1
// bit [2] EnDctOddToNcLnkDatXfr = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x200), 0x0000000C, 0x0000000C),

// D18F3x240 - MCT Configuration Low
// bit [6] WCBufReplaceEn        = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x240), 0x00000040, 0x00000040),

// D18F3x2A0 - Core Interface Buffer Count Register 2
// bits[3:0] CpuToNbFreeBufCntHi = 0x5
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_3, 0x2A0), 0x00000005, 0x0000000F),

// D18F4x110 - Sample and Residency Timers
// bits[11:0] CSampleTimer = 0x2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_4, 0x110), 0x00000002, 0x00000FFF),

// D18F4x15C - Core Performance Boost Control
// bit [8]   CstatePowerEn = 1
// bits[1:0] BoostSrc = 0
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_4, 0x15C), 0x00000100, 0x00000103),

// D18F4x16C - APM TDP Control
// bit [4]       ApmTdpLimitIntEn    = 0x1
// bit [3]       TdpLimitDis         = 0x1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_4, 0x16C), 0x00000018, 0x00000018),

// D18F4x170 - Interrupt Rate Monitor Control
// bits[15:11] DecrRate  = 0x18
// bits[10:8]  BurstLen  = 0x5
// bits[7:4]   Threshold = 0x4
// bits[3:0]   MaxDepth  = 0x5
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_4, 0x170), 0x0000C545, 0x0000FFFF),

// D18F4x218
// bit [5]  = 1
// bit [3]  = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_4, 0x218), 0x00000028, 0x00000028),

// D18F5x88 - NB Configuration 4 (NB_CFG4)
// bit [24] DisHbNpReqBusLock = 1
// bit [14] DisHldRegRdRspChk = 1
// bit [6]  = 0
// bit [0]  = 1 (if D18F3xA8[25] = 1)
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x88), 0x01004001, 0x01004041),

// D18F5x8C - NB Configuration 5 (NB_CFG5)
// bit [26] DisSrqPickNcReqThrtl = 1
// bit [15] EnSrqAllocGt31       = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x8C), 0x04008000, 0x04008000),

// D18F5x98
// bit [30]                      = 1
  MAKE_CPU_REV_PCI_ENTRY (AMD_FAMILY_CZ, AMD_F15_CZ_A0, MAKE_SBDFO (0, 0, 24, FUNC_5, 0x98), 0x40000000, 0x40000000),

// D18F5xA0
// bit[5]  DisOn3WrBurstBoost = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0xA0), 0x00000020, 0x00000020),

// D18F5xE0 - Processor TDP Running Average
// bits[3:0] RunAvgRange = 0x2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0xE0), 0x00000002, 0x0000000F),

// D18F5x128 - Clock Power/Timing Control 3
// bit [27]    SprSaveRestoreEn = 1
// bits[13:12] PwrGateTmr = 1
// bit [9]     FastSlamTimeDown = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x128), 0x08001200, 0x08003200),

// D18F5x170 - Northbridge P-state Control
// bits[12:9]   NbPstateThreshold = 1, if 1 compute unit is enabled
  MAKE_COMPUTE_UNIT_COUNTS_PCI_ENTRY (PERFORMANCE_PROFILE_ALL, (COMPUTE_UNIT_RANGE_0 (1, 1) | COUNT_RANGE_NONE), MAKE_SBDFO (0, 0, 24, FUNC_5, 0x170), 0x00000200, 0x00001E00),

// D18F5x170 - Northbridge P-state Control
// bits[12:9]   NbPstateThreshold = 2, if 2 compute unit is enabled
  MAKE_COMPUTE_UNIT_COUNTS_PCI_ENTRY (PERFORMANCE_PROFILE_ALL, (COMPUTE_UNIT_RANGE_0 (2, 2) | COUNT_RANGE_NONE), MAKE_SBDFO (0, 0, 24, FUNC_5, 0x170), 0x00000400, 0x00001E00),

// D18F5x178 - Northbridge Fusion Configuration
// bit [18]      CstateFusionHsDis    = 1
// bit [17]      Dis2ndGnbAllowPsWait = 1
// bit [11]      AllowSelfRefrS3Dis   = 1
// bit [10]      InbWakeS3Dis         = 1
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x178), 0x00060C00, 0x00060C00),

// D18F5x224 - Extended MCT Buffer Count
// bits[9:8]     XbartoMctReqPrivateLmt = 0x2
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x224), 0x00000200, 0x00000300),

// D18F5x228 - MCT to ONION3 Buffer Count
// bits[20:16]   McttoOn3RspData = 0x12
  MAKE_PCI_ENTRY (MAKE_SBDFO (0, 0, 24, FUNC_5, 0x228), 0x00120000, 0x001F0000),

// MSR_MC4_MISC0 (0x00000413)
// bit [61] Locked
  MAKE_FAM_SPECIFIC_WORKAROUND_ENTRY (F15CzNbMcaLockId, 0),

// Table Terminator
  MAKE_TABLE_TERMINATOR
};

CONST REGISTER_TABLE ROMDATA F15CzPrimaryCoreRegTableBeforeApLaunch = {
  PrimaryCores,
  F15CzPrimaryCoresRegistersBeforeApLaunch,
};

CONST REGISTER_TABLE ROMDATA F15CzPrimaryCoreRegTableAfterApLaunch = {
  PrimaryCores,
  F15CzPrimaryCoreRegistersAfterApLaunch,
};



