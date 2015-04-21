/* $NoKeywords:$ */
/**
 * @file
 *
 * IDS NV definition for F15St
 *
 * Auto generated from CBS XML file
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS F15St
 * @e \$Revision: 312756 $   @e \$Date: 2015-02-11 11:00:50 +0800 (Wed, 11 Feb 2015) $
 */
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
 *
 ***************************************************************************/


#ifndef _IDSINTF15StNVDEF_H_
#define _IDSINTF15StNVDEF_H_

#define IdsNvBYTEValueAuto 0xFF

///Node 0 Debug Memory Hole
///Enable or disable Debug Memory Hole
typedef enum {
  IdsNvCpuGenMemHole0Disabled = 0,///<Disabled
  IdsNvCpuGenMemHole032M = 2,///<32M
  IdsNvCpuGenMemHole064M = 4,///<64M
  IdsNvCpuGenMemHole0128M = 8,///<128M
  IdsNvCpuGenMemHole0256M = 16,///<256M
} IdsNvCpuGenMemHole0;

///uCode Patching
///Enable or disable microcode patching. If disabled, no microcode patches will be loaded. If enabled, only patches that apply to the installed processor will be loaded.
typedef enum {
  IdsNvCpuGenUCodeDisabled = 0,///<Disabled
  IdsNvCpuGenUCodeAuto = 1,///<Auto
} IdsNvCpuGenUCode;

///Sync Flood on MCA
///Enable or disable BIOS Sync Flood on MCA.
typedef enum {
  IdsNvCpuGenSyncFloodMCADisabled = 0,///<Disabled
  IdsNvCpuGenSyncFloodMCAEnabled = 1,///<Enabled
  IdsNvCpuGenSyncFloodMCAAuto = 3,///<Auto
} IdsNvCpuGenSyncFloodMCA;

///Enable Redirect Shutdown to HDT
///
typedef enum {
  IdsNvCpuGenRedirectShutdownToHDTAuto = 0,///<Auto
  IdsNvCpuGenRedirectShutdownToHDTEnabled = 1,///<Enabled
} IdsNvCpuGenRedirectShutdownToHDT;

///LBR
///
typedef enum {
  IdsNvCpuGenLBRDisabled = 0,///<Disabled
  IdsNvCpuGenLBREnabled = 1,///<Enabled
  IdsNvCpuGenLBRAuto = 3,///<Auto
} IdsNvCpuGenLBR;

///HDT OUT
///
typedef enum {
  IdsNvCpuGenHDTOUTDisabled = 0,///<Disabled
  IdsNvCpuGenHDTOUTEnabled = 1,///<Enabled
} IdsNvCpuGenHDTOUT;

///HDT Breakpoint 1 (Before DRAM init)
///Setting this option to enabled will cause the BIOS to force the system into HDT mode just before DRAM init, right after the backend code for the memory timing options is run and immediately before the DRAM enable bit is set. All of the registers for DCT init will be programmed, but the DRAM Init bit will not have been set yet.
typedef enum {
  IdsNvCpuGenHDTBrkpt1Disabled = 0,///<Disabled
  IdsNvCpuGenHDTBrkpt1Enabled = 1,///<Enabled
} IdsNvCpuGenHDTBrkpt1;

///HDT Breakpoint 2 (Before PCI init)
///Setting this option to enabled will cause the BIOS to force the system into HDT mode just before PCI initialization.
typedef enum {
  IdsNvCpuGenHDTBrkpt2Disabled = 0,///<Disabled
  IdsNvCpuGenHDTBrkpt2Enabled = 1,///<Enabled
} IdsNvCpuGenHDTBrkpt2;

///HDT Breakpoint 3 (Before OS)
///Setting this option to enabled will cause the BIOS to force the system into HDT mode very late in POST.
typedef enum {
  IdsNvCpuGenHDTBrkpt3Disabled = 0,///<Disabled
  IdsNvCpuGenHDTBrkpt3Enabled = 1,///<Enabled
} IdsNvCpuGenHDTBrkpt3;

///HDT Breakpoint 4 (Before Memclr Bit)
///Setting this option to enabled will cause BIOS to force the system into HDT mode before D18F2x110[3]MemClrInit is set
typedef enum {
  IdsNvCpuGenHDTBrkpt4Disabled = 0,///<Disabled
  IdsNvCpuGenHDTBrkpt4Enabled = 1,///<Enabled
} IdsNvCpuGenHDTBrkpt4;

///SMM Code Lock
///SMM code segment/registers.
typedef enum {
  IdsNvCpuGenSmmLockAuto = 0,///<Auto
  IdsNvCpuGenSmmLockEnabled = 1,///<Enabled
} IdsNvCpuGenSmmLock;

///SVM Lock
///SVM Lock registers.
typedef enum {
  IdsNvCpuGenSvmLockAuto = 0,///<Auto
  IdsNvCpuGenSvmLockEnabled = 1,///<Enabled
} IdsNvCpuGenSvmLock;

///Core Leveling
///Sets the number of Cores to be used.
typedef enum {
  IdsNvCpuGenCoreLevCompute_Unit_Leveling = 4,///<Compute Unit Leveling
  IdsNvCpuGenCoreLevONE = 5,///<ONE
  IdsNvCpuGenCoreLevTWO = 1,///<TWO
  IdsNvCpuGenCoreLevTHREE = 6,///<THREE
  IdsNvCpuGenCoreLevFOUR = 7,///<FOUR
  IdsNvCpuGenCoreLevAuto = 0x1F,///<Auto
} IdsNvCpuGenCoreLev;

///Watchdog Timer Disable
///Enable or disable CPU Watchdog Timer
typedef enum {
  IdsNvCpuGenCpuWdt0 = 0,///<0
  IdsNvCpuGenCpuWdt1 = 1,///<1
  IdsNvCpuGenCpuWdtAuto = 3,///<Auto
} IdsNvCpuGenCpuWdt;

///ACPI WHEA
///Controls activation of CPU Machine Check Setting
typedef enum {
  IdsNvCpuGenWHEADisabled = 0,///<Disabled
  IdsNvCpuGenWHEAEnabled = 1,///<Enabled
  IdsNvCpuGenWHEAAuto = 3,///<Auto
} IdsNvCpuGenWHEA;

///Response Pass Posted Write
///Enable or Disable response PassPW
typedef enum {
  IdsNvCpuGenRespPassPWDisabled = 0,///<Disabled
  IdsNvCpuGenRespPassPWEnabled = 1,///<Enabled
  IdsNvCpuGenRespPassPWAuto = 3,///<Auto
} IdsNvCpuGenRespPassPW;

///Downstream Non-Posted Request Limit
///Enable or Disable response PassPW
typedef enum {
  IdsNvCpuGenDsNpReqLmtNo_limit = 0,///<No limit
  IdsNvCpuGenDsNpReqLmtLimited_to_1 = 1,///<Limited to 1
  IdsNvCpuGenDsNpReqLmtLimited_to_4 = 2,///<Limited to 4
  IdsNvCpuGenDsNpReqLmtLimited_to_8 = 3,///<Limited to 8
  IdsNvCpuGenDsNpReqLmtAuto = 7,///<Auto
} IdsNvCpuGenDsNpReqLmt;

///L2 way lock Enable
///L2 way lock enable.
typedef enum {
  IdsNvCpuGenL2WayLockEnDisabled = 0,///<Disabled
  IdsNvCpuGenL2WayLockEnEnabled = 1,///<Enabled
  IdsNvCpuGenL2WayLockEnAuto = 3,///<Auto
} IdsNvCpuGenL2WayLockEn;

///First L2 way locked
///Enable or disable the ICache.
#define IdsNvCpuGenL2WayLockedMin 1
#define IdsNvCpuGenL2WayLockedMax 0xF

///Load Step Throttle Control
///D18F5xEC [0], load step throttle enable
typedef enum {
  IdsNvCpuPMCtlLSTCtlDisabled = 0,///<Disabled
  IdsNvCpuPMCtlLSTCtlEnabled = 1,///<Enabled
  IdsNvCpuPMCtlLSTCtlAuto = 3,///<Auto
} IdsNvCpuPMCtlLSTCtl;

///Prochot SMI Mask
///APIC330[16]
typedef enum {
  IdsNvCpuGenSMIMaskMasked = 1,///<Masked
  IdsNvCpuGenSMIMaskNot_masked = 0,///<Not masked
  IdsNvCpuGenSMIMaskAuto = 3,///<Auto
} IdsNvCpuGenSMIMask;

///ACPI _PSS
///
typedef enum {
  IdsNvCpuPMCtlPssDisabled = 0,///<Disabled
  IdsNvCpuPMCtlPssEnabled = 1,///<Enabled
  IdsNvCpuPMCtlPssAuto = 3,///<Auto
} IdsNvCpuPMCtlPss;

///Custom Core P-States
///Tells BIOS whether to use the setup options below this to configure the P-States, or whether to configure the P-States automatically.
typedef enum {
  IdsNvCpuPstCustomPStateDisabled = 0,///<Disabled
  IdsNvCpuPstCustomPStateEnabled = 1,///<Enabled
} IdsNvCpuPstCustomPState;

///Number of P-State
///Set the Number of P-state.
typedef enum {
  IdsNvCpuPstNumPState1 = 1,///<1
  IdsNvCpuPstNumPState2 = 2,///<2
  IdsNvCpuPstNumPState3 = 3,///<3
  IdsNvCpuPstNumPState4 = 4,///<4
  IdsNvCpuPstNumPState5 = 5,///<5
  IdsNvCpuPstNumPState6 = 6,///<6
  IdsNvCpuPstNumPState7 = 7,///<7
  IdsNvCpuPstNumPState8 = 8,///<8
} IdsNvCpuPstNumPState;

///State 0 Core FID
///Set the frequency to use for Core PState 0.
#define IdsNvCpuPstSta0CoreFidMin 0
#define IdsNvCpuPstSta0CoreFidMax 0x3F

///State 0 Core VID
///Set the voltage to use for Core PState 0.
#define IdsNvCpuPstSta0CoreVidMin 0
#define IdsNvCpuPstSta0CoreVidMax 0xFF

///State 0 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta0CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta0CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta0CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta0CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta0CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta0CoreDid;

///State 1 Core FID
///Set the frequency to use for Core PState 1
#define IdsNvCpuPstSta1CoreFidMin 0
#define IdsNvCpuPstSta1CoreFidMax 0x3F

///State 1 Core VID
///Set the voltage to use for Core PState 1.
#define IdsNvCpuPstSta1CoreVidMin 0
#define IdsNvCpuPstSta1CoreVidMax 0xFF

///State 1 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta1CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta1CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta1CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta1CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta1CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta1CoreDid;

///State 2 Core FID
///Set the frequency to use for Core PState 2
#define IdsNvCpuPstSta2CoreFidMin 0
#define IdsNvCpuPstSta2CoreFidMax 0x3F

///State 2 Core VID
///Set the voltage to use for Core PState 2.
#define IdsNvCpuPstSta2CoreVidMin 0
#define IdsNvCpuPstSta2CoreVidMax 0xFF

///State 2 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta2CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta2CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta2CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta2CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta2CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta2CoreDid;

///State 3 Core FID
///Set the frequency to use for Core PState 3
#define IdsNvCpuPstSta3CoreFidMin 0
#define IdsNvCpuPstSta3CoreFidMax 0x3F

///State 3 Core VID
///Set the voltage to use for Core PState 3.
#define IdsNvCpuPstSta3CoreVidMin 0
#define IdsNvCpuPstSta3CoreVidMax 0x7F

///State 3 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta3CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta3CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta3CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta3CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta3CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta3CoreDid;

///State 4 Core FID
///Set the frequency to use for Core PState 4
#define IdsNvCpuPstSta4CoreFidMin 0
#define IdsNvCpuPstSta4CoreFidMax 0x3F

///State 4 Core VID
///Set the voltage to use for Core PState 4.
#define IdsNvCpuPstSta4CoreVidMin 0
#define IdsNvCpuPstSta4CoreVidMax 0xFF

///State 4 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta4CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta4CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta4CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta4CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta4CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta4CoreDid;

///State 5 Core FID
///Set the frequency to use for Core PState 5
#define IdsNvCpuPstSta5CoreFidMin 0
#define IdsNvCpuPstSta5CoreFidMax 0x3F

///State 5 Core VID
///Set the voltage to use for Core PState 5.
#define IdsNvCpuPstSta5CoreVidMin 0
#define IdsNvCpuPstSta5CoreVidMax 0xFF

///State 5 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta5CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta5CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta5CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta5CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta5CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta5CoreDid;

///State 6 Core FID
///Set the frequency to use for Core PState 6
#define IdsNvCpuPstSta6CoreFidMin 0
#define IdsNvCpuPstSta6CoreFidMax 0x3F

///State 6 Core VID
///Set the voltage to use for Core PState 6.
#define IdsNvCpuPstSta6CoreVidMin 0
#define IdsNvCpuPstSta6CoreVidMax 0xFF

///State 6 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta6CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta6CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta6CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta6CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta6CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta6CoreDid;

///State 7 Core FID
///Set the frequency to use for Core PState 7
#define IdsNvCpuPstSta7CoreFidMin 0
#define IdsNvCpuPstSta7CoreFidMax 0x3F

///State 7 Core VID
///Set the voltage to use for Core PState 7.
#define IdsNvCpuPstSta7CoreVidMin 0
#define IdsNvCpuPstSta7CoreVidMax 0xFF

///State 7 Core DID
///This is the Core Divider.
typedef enum {
  IdsNvCpuPstSta7CoreDidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuPstSta7CoreDidDivide_by_2 = 1,///<Divide by 2
  IdsNvCpuPstSta7CoreDidDivide_by_4 = 2,///<Divide by 4
  IdsNvCpuPstSta7CoreDidDivide_by_8 = 3,///<Divide by 8
  IdsNvCpuPstSta7CoreDidDivide_by_16 = 4,///<Divide by 16
} IdsNvCpuPstSta7CoreDid;

///Core Performance Boost Master Enable
///This specifies core performance boost master enable. D18F4x15C[7]
typedef enum {
  IdsNvCpuPstApmMasterEnDisabled = 0,///<Disabled
  IdsNvCpuPstApmMasterEnEnabled = 1,///<Enabled
  IdsNvCpuPstApmMasterEnManual = 2,///<Manual
  IdsNvCpuPstApmMasterEnAuto = 3,///<Auto
} IdsNvCpuPstApmMasterEn;

///C-state Boost Control
///C-state Boost Control
typedef enum {
  IdsNvCpuPstCstateBoostCtlDisabled = 0,///<Disabled
  IdsNvCpuPstCstateBoostCtlManual = 1,///<Manual
  IdsNvCpuPstCstateBoostCtlAuto = 3,///<Auto
} IdsNvCpuPstCstateBoostCtl;

///Number of boosted states
///Specifies the number of boosted states. D18F4x15C[4:2]
#define IdsNvCpuPstNumBoostStatesMin 0
#define IdsNvCpuPstNumBoostStatesMax 7

///C-state count
///Specifies the number of compute units that must be in CC6 before an APM transition can occur to a boosted P-state that is higher performance than the P-state specified by CstateBoost. A value of 0 disables access to P-states above CstateBoost. D18F4x16C[11:9]
#define IdsNvCpuPstCstCntMin 0
#define IdsNvCpuPstCstCntMax 7

///CstateBoost
///Specifies the P-state which requires the number of compute units specified in CstateCnt to be in CC6 before a transition to a higher performance (lower numbered) boosted P-state is allowed. D18F4x16C[8:6]
#define IdsNvCpuPstCstBoostMin 0
#define IdsNvCpuPstCstBoostMax 7

///APM TDP Boost States
///Specifies the number of Pstates that are considered boosted P-states. D18F4x15C[4:2]
typedef enum {
  IdsNvCpuPstApmBoostStatesDisabled = 0,///<Disabled
  IdsNvCpuPstApmBoostStatesAuto = 1,///<Auto
} IdsNvCpuPstApmBoostStates;

///APM TDP Control
///This specifies TDP limit. D18F4x16C[3:0]
typedef enum {
  IdsNvCpuPstTDPCtlDisabled = 0,///<Disabled
  IdsNvCpuPstTDPCtlManual = 1,///<Manual
  IdsNvCpuPstTDPCtlAuto = 3,///<Auto
} IdsNvCpuPstTDPCtl;

///NodeTdpLimit Enable
///This specifies TDP limit. D18F4x16C[2]
typedef enum {
  IdsNvCpuPstNodeTdpLmtEnDisabled = 0,///<Disabled
  IdsNvCpuPstNodeTdpLmtEnEnabled = 1,///<Enabled
} IdsNvCpuPstNodeTdpLmtEn;

///ApmlSwTdpLimit Enable
///This specifies TDP limit. D18F4x16C[0]
typedef enum {
  IdsNvCpuPstApmlSwTdpLmtEnDisabled = 0,///<Disabled
  IdsNvCpuPstApmlSwTdpLmtEnEnabled = 1,///<Enabled
} IdsNvCpuPstApmlSwTdpLmtEn;

///NB/core sync FIFO pointer adjust Control
///
typedef enum {
  IdsNvCpuPstNbsynPtrAdjCtlManual = 0,///<Manual
  IdsNvCpuPstNbsynPtrAdjCtlAuto = 1,///<Auto
} IdsNvCpuPstNbsynPtrAdjCtl;

///NB/core sync FIFO pointer adjust
///D18F3xDC[14:12]
#define IdsNvCpuPstNbsynPtrAdjMin 0
#define IdsNvCpuPstNbsynPtrAdjMax 7

///NB P-states Support
///Specifies whether Override NB P-state.
typedef enum {
  IdsNvCpuNBPstatesEnable = 1,///<Enable
  IdsNvCpuNBPstatesDisable = 0,///<Disable
  IdsNvCpuNBPstatesAuto = 0xFF,///<Auto
} IdsNvCpuNBPstatesSupport;

///Custom NB P-states
///Specifies whether Override NB P-state.
typedef enum {
  IdsNvCpuCustomNBPstatesDisabled = 0,///<Disabled
  IdsNvCpuCustomNBPstatesManual = 1,///<Manual
  IdsNvCpuCustomNBPstatesAuto = 3,///<Auto
} IdsNvCpuCustomNBPstates;

///Number of high performance NB P-State
///The value scope you key is: 1~8. Example: 3: which means we have 3 high performance NBP-State, Pstate 0,1,2 has High performance NB P-state, MSRC001_00[66:64][22]=0,Pstate 3,4,5,6,7 has low performance NB P-state, MSRC001_00[6B:67][22]=1.This option only works if "Number of NB P-States" choose "two".
#define IdsNvCpuPstNumHiPerfNBPstMin 1
#define IdsNvCpuPstNumHiPerfNBPstMax 8

///Number of NB P-States
///
typedef enum {
  IdsNvCpuPstNumNBPstOne = 0,///<One
  IdsNvCpuPstNumNBPstTwo = 1,///<Two
} IdsNvCpuPstNumNBPst;

///Memory P-state Disable
///1=Memory P-state transitions;0=Memory P-state transitions are enabled.
typedef enum {
  IdsNvCpuPstMemPstateDis0 = 0,///<0
  IdsNvCpuPstMemPstateDis1 = 1,///<1
  IdsNvCpuPstMemPstateDisAuto = 3,///<Auto
} IdsNvCpuPstMemPstateDis;


///Software NB P-state low disable
///1=Transition to NbPstateHi and disable transitions to NbPstateLo.
typedef enum {
  IdsNvCpuPstSwNbPstateLoDis0 = 0,///<0
  IdsNvCpuPstSwNbPstateLoDis1 = 1,///<1
  IdsNvCpuPstSwNbPstateLoDisAuto = 3,///<Auto
} IdsNvCpuPstSwNbPstateLoDis;

///NB P-State 0 Memory P-state
///1=The Northbridge Pstate specified by this register maps to memory P-state 1. 0=The Northbridge P-state specified by this register maps to memory P-state 0. Memory P-states may be globally disabled by programming D18F5x170[MemPstateDis].
typedef enum {
  IdsNvCpuNBPS0MemPstate0 = 0,///<0
  IdsNvCpuNBPS0MemPstate1 = 1,///<1
  IdsNvCpuNBPS0MemPstateAuto = 3,///<Auto
} IdsNvCpuNBPS0MemPstate;

///State 0 NB FID
///Set the frequency to use for NB PState 0. D18F5x160[6:1]
#define IdsNvCpuNBPS0FidMin 0
#define IdsNvCpuNBPS0FidMax 0x3F

///State 0 NB VID
///Set the voltage to use for NB PState 0. D18F5x160[16:10],D18F5x160[21]
#define IdsNvCpuNBPS0VidMin 0
#define IdsNvCpuNBPS0VidMax 0xFF

///State 0 NB DID
///This is the NB Divider. D18F5x160[7]
typedef enum {
  IdsNvCpuNBPS0DidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuNBPS0DidDivide_by_2 = 1,///<Divide by 2
} IdsNvCpuNBPS0Did;

///State 1 NB FID
///Set the frequency to use for NB PState 1. D18F5x164[6:1]
#define IdsNvCpuNBPS1FidMin 0
#define IdsNvCpuNBPS1FidMax 0x3F

///State 1 NB VID
///Set the voltage to use for NB PState 1. D18F5x164[16:10],D18F5x164[21]
#define IdsNvCpuNBPS1VidMin 0
#define IdsNvCpuNBPS1VidMax 0xFF

///State 1 NB DID
///This is the NB Divider. D18F5x164[7]
typedef enum {
  IdsNvCpuNBPS1DidDivide_by_1 = 0,///<Divide by 1
  IdsNvCpuNBPS1DidDivide_by_2 = 1,///<Divide by 2
} IdsNvCpuNBPS1Did;

///MSR Access
///Specifies whether the following registers are read-write or read-only. MSRC001_1070. MSRC001_1072. MSRC001_1073.
typedef enum {
  IdsNvCpuCacDbgLockPolling = 1,     ///<Polling
  IdsNvCpuCacDbgLockDoNotPolling = 0,///<DoNotPolling
} IdsNvCpuMsrAccess;

///Power Policy
///Performance - disable NB P-States, use max DDR speed and max HT speed available; Power - enable NB P-states, throttle DDR speed and HT speed.
typedef enum {
  IdsNvCpuNbPwrPolicyPerformance = 0,///<Performance
  IdsNvCpuNbPwrPolicyPower = 1,///<Power
  IdsNvCpuNbPwrPolicyAuto = 3,///<Auto
} IdsNvCpuNbPwrPolicy;

///Core C6 State
///Enable or disable CC6 Mode.
typedef enum {
  IdsNvCpuCstCC6EnDisabled = 0,///<Disabled
  IdsNvCpuCstCC6EnEnabled = 1,///<Enabled
  IdsNvCpuCstCC6EnAuto = 3,///<Auto
} IdsNvCpuCstCC6En;

///C-state Control
///C-State Control
typedef enum {
  IdsNvCpuCstCtlDisabled = 0,///<Disabled
  IdsNvCpuCstCtlManual = 1,///<Manual
  IdsNvCpuCstCtlAuto = 3,///<Auto
} IdsNvCpuCstCtl;

///Number of C-States
///Select the Number of C-States
typedef enum {
  IdsNvCpuCstNumCStates1_C_State = 0,///<1 C-State
  IdsNvCpuCstNumCStates2_C_States = 1,///<2 C-States
  IdsNvCpuCstNumCStates3_C_States = 2,///<3 C-States
} IdsNvCpuCstNumCStates;

///C-state Action1
///Set C-State Action Field 1. D18F4x118[13:0]
#define IdsNvCpuCstAct1Min 0
#define IdsNvCpuCstAct1Max 0x3FFF

///C-state Action2
///Set C-State Action Field 2. D18F4x118[29:16]
#define IdsNvCpuCstAct2Min 0
#define IdsNvCpuCstAct2Max 0x3FFF

///C-state Action3
///Set C-State Action Field 3. D18F4x11C[13:0]
#define IdsNvCpuCstAct3Min 0
#define IdsNvCpuCstAct3Max 0x3FFF

///Cache Flush on Halt Control
///
typedef enum {
  IdsNvCpuCstCacheFlushHltCtrlDisabled = 0,///<Disabled
  IdsNvCpuCstCacheFlushHltCtrlManual = 1,///<Manual
  IdsNvCpuCstCacheFlushHltCtrlAuto = 3,///<Auto
} IdsNvCpuCstCacheFlushHltCtrl;

///Cache Flush on Halt Clock Timer
///
#define IdsNvCpuCstCacheFlushHltTmrMin 0
#define IdsNvCpuCstCacheFlushHltTmrMax 0x7F

///Cache Flush on Halt Clock divisor
///
#define IdsNvCpuCstCacheFlushHltDivMin 0
#define IdsNvCpuCstCacheFlushHltDivMax 7

///Cache Flush Timer
///
#define IdsNvCpuCstCacheFlushTmrMin 0
#define IdsNvCpuCstCacheFlushTmrMax 0x7F

///Cache Flush Success Monitor Control
///
typedef enum {
  IdsNvCpuCacheFlushSuccMonCtlDisabled = 0,///<Disabled
  IdsNvCpuCacheFlushSuccMonCtlManual = 1,///<Manual
  IdsNvCpuCacheFlushSuccMonCtlAuto = 3,///<Auto
} IdsNvCpuCacheFlushSuccMonCtl;

///Cache Flush Success Monitor Threshold
///
#define IdsNvCpuCacheFlushSuccMonThresholdMin 0
#define IdsNvCpuCacheFlushSuccMonThresholdMax 7

///Cache Flush Immediate on All Halt
///
typedef enum {
  IdsNvCpuCstCacheFlushImmOnAllHltDisabled = 0,///<Disabled
  IdsNvCpuCstCacheFlushImmOnAllHltEnabled = 1,///<Enabled
  IdsNvCpuCstCacheFlushImmOnAllHltAuto = 3,///<Auto
} IdsNvCpuCstCacheFlushImmOnAllHlt;

///Halt CState Index
///
#define IdsNvCpuHltCstIndexMin 0
#define IdsNvCpuHltCstIndexMax 7

///CState Messaging Disable
///0=Messages are sent to FCH when a core enters or exits a Cstate;1=Messages are not sent to FCH when a core enters or exits a Cstate.
typedef enum {
  IdsNvCpuCstMsgDis0 = 0,///<0
  IdsNvCpuCstMsgDis1 = 1,///<1
  IdsNvCpuCstMsgDisAuto = 3,///<Auto
} IdsNvCpuCstMsgDis;

///Interrupt Monitor Wake Enable
///Specifies whether a Timer Tick message from FCH with coding 1111b will automatically wake all cores from a Cstate.
typedef enum {
  IdsNvCpuCstIntWakeEnDisabled = 0,///<Disabled
  IdsNvCpuCstIntWakeEnEnabled = 1,///<Enabled
  IdsNvCpuCstIntWakeEnAuto = 3,///<Auto
} IdsNvCpuCstIntWakeEn;

///Interrupt Monitor Package C6 Limit
///Specifies the threshold for disallowing access to the package C6 state
#define IdsNvCpuCstIntPC6LmtMin 0
#define IdsNvCpuCstIntPC6LmtMax 0xF

///Interrupt Monitor Package C6 enable
///Specifies whether the interrupt monitor is enabled for the package C6 state
typedef enum {
  IdsNvCpuCstIntPC6EnDisabled = 0,///<Disabled
  IdsNvCpuCstIntPC6EnEnabled = 1,///<Enabled
  IdsNvCpuCstIntPC6EnAuto = 3,///<Auto
} IdsNvCpuCstIntPC6En;

///CC6 Power Down Regulator Enable
///F5x128[15]
typedef enum {
  IdsNvCpuCstCC6PwrDwnRegEnDisabled = 0,///<Disabled
  IdsNvCpuCstCC6PwrDwnRegEnEnabled = 1,///<Enabled
  IdsNvCpuCstCC6PwrDwnRegEnAuto = 3,///<Auto
} IdsNvCpuCstCC6PwrDwnRegEn;

///Fast Slam Time Down
///Specifies the voltage stabilization slam time to use for transitions to lower performance P-states, altvid, and PC6. D18F5x128[9]
typedef enum {
  IdsNvCpuPwrFastSlamTimeDownDisabled = 0,///<Disabled
  IdsNvCpuPwrFastSlamTimeDownEnabled = 1,///<Enabled
  IdsNvCpuPwrFastSlamTimeDownAuto = 3,///<Auto
} IdsNvCpuPwrFastSlamTimeDown;

///Enforce supported frequency
///If this option is disable, and the target board does not support a certain VDDIO, then picking up a specific VDDIO through the CMOS option menu is required.
typedef enum {
  IdsNvMemTimingEnforceFreqEnabled = 0,///<Enabled
  IdsNvMemTimingEnforceFreqDisabled = 1,///<Disabled
  IdsNvMemTimingEnforceFreqAuto = 3,///<Auto
} IdsNvMemTimingEnforceFreq;

///DRAM Timing User Controls
///DRAM Timing User Controls
typedef enum {
  IdsNvMemTimingCtlAuto = 0,///<Auto
  IdsNvMemTimingCtlManual = 1,///<Manual
} IdsNvMemTimingCtl;

///Memory Clock Speed
///Set the memory clock frequency. D18F2x94_dct[1:0][4:0]
typedef enum {
  IdsNvMemSpeed333MHz = 2,///<333MHz
  IdsNvMemSpeed400MHz = 3,///<400MHz
  IdsNvMemSpeed533MHz = 4,///<533MHz
  IdsNvMemSpeed667MHz = 5,///<667MHz
  IdsNvMemSpeed800MHz = 6,///<800MHz
  IdsNvMemSpeed933MHz = 7,///<933MHz
  IdsNvMemSpeed1050MHz = 8,///<1050MHz
  IdsNvMemSpeed1066MHz = 9,///<1066MHz
  IdsNvMemSpeed1200MHz = 0xA,///<1200MHz
  IdsNvMemSpeedAuto = 0xF,///<Auto
} IdsNvMemSpeed;

///RdPtr
///Read pointer initial value. D18F2x210
typedef enum {
  IdsNvMemTimingRdPtrReserved_01b = 1,///<Reserved 01b
  IdsNvMemTimingRdPtr3_Clk = 2,///<3 Clk
  IdsNvMemTimingRdPtr2_5_Clk = 3,///<2.5 Clk
  IdsNvMemTimingRdPtr2_Clk = 4,///<2 Clk
  IdsNvMemTimingRdPtr1_5_Clk = 5,///<1.5 Clk
  IdsNvMemTimingRdPtr1_Clk = 6,///<1 Clk
  IdsNvMemTimingRdPtrAuto = 0xFF,///<Auto
} IdsNvMemTimingRdPtr;

///tCL
///Sets the tCL time. D18F2x200_dct[1:0]_mp[1:0][4:0]
typedef enum {
  IdsNvMemTimingTcl5_Clk = 5,///<5 Clk
  IdsNvMemTimingTcl6_Clk = 6,///<6 Clk
  IdsNvMemTimingTcl7_Clk = 7,///<7 Clk
  IdsNvMemTimingTcl8_Clk = 8,///<8 Clk
  IdsNvMemTimingTcl9_Clk = 9,///<9 Clk
  IdsNvMemTimingTcl0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTcl0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTcl0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTcl0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTcl0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTcl0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTcl10h_Clk = 0x10,///<10h Clk
  IdsNvMemTimingTcl11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTcl12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTcl13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTclAuto = 0xFF,///<Auto
} IdsNvMemTimingTcl;

///tRCD
///This sets the RAS# Active to CAS# read/write delay. D18F2x200_dct[1:0]_mp[1:0][12:8]
typedef enum {
  IdsNvMemTimingTrcd5_Clk = 5,///<5 Clk
  IdsNvMemTimingTrcd6_Clk = 6,///<6 Clk
  IdsNvMemTimingTrcd7_Clk = 7,///<7 Clk
  IdsNvMemTimingTrcd8_Clk = 8,///<8 Clk
  IdsNvMemTimingTrcd9_Clk = 9,///<9 Clk
  IdsNvMemTimingTrcd0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTrcd0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTrcd0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTrcd0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTrcd0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTrcd0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTrcd10h_Clk = 0x10,///<10h Clk
  IdsNvMemTimingTrcd11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTrcd12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTrcd13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTrcd14h_Clk = 0x14,///<14h Clk
  IdsNvMemTimingTrcd15h_Clk = 0x15,///<15h Clk
  IdsNvMemTimingTrcd16h_Clk = 0x16,///<16h Clk
  IdsNvMemTimingTrcd17h_Clk = 0x17,///<17h Clk
  IdsNvMemTimingTrcd18h_Clk = 0x18,///<18h Clk
  IdsNvMemTimingTrcd19h_Clk = 0x19,///<19h Clk
  IdsNvMemTimingTrcd1Ah_Clk = 0x1A,///<1Ah Clk
  IdsNvMemTimingTrcdAuto = 0xFF,///<Auto
} IdsNvMemTimingTrcd;

///tRP
///Specify the row precharge time. D18F2x200_dct[1:0]_mp[1:0][20:16]
typedef enum {
  IdsNvMemTimingTrp5_Clk = 5,///<5 Clk
  IdsNvMemTimingTrp6_Clk = 6,///<6 Clk
  IdsNvMemTimingTrp7_Clk = 7,///<7 Clk
  IdsNvMemTimingTrp8_Clk = 8,///<8 Clk
  IdsNvMemTimingTrp9_Clk = 9,///<9 Clk
  IdsNvMemTimingTrp0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTrp0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTrp0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTrp0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTrp0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTrp0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTrp10h_Clk = 0x10,///<10h Clk
  IdsNvMemTimingTrp11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTrp12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTrp13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTrp14h_Clk = 0x14,///<14h Clk
  IdsNvMemTimingTrp15h_Clk = 0x15,///<15h Clk
  IdsNvMemTimingTrp16h_Clk = 0x16,///<16h Clk
  IdsNvMemTimingTrp17h_Clk = 0x17,///<17h Clk
  IdsNvMemTimingTrp18h_Clk = 0x18,///<18h Clk
  IdsNvMemTimingTrp19h_Clk = 0x19,///<19h Clk
  IdsNvMemTimingTrp1Ah_Clk = 0x1A,///<1Ah Clk
  IdsNvMemTimingTrpAuto = 0xFF,///<Auto
} IdsNvMemTimingTrp;

///tRTP
///Specifies the read CAS# to precharge time. D18F2x204_dct[1:0]_mp[1:0][27:24]
typedef enum {
  IdsNvMemTimingTrtp4_Clk = 4,///<4 Clk
  IdsNvMemTimingTrtp5_Clk = 5,///<5 Clk
  IdsNvMemTimingTrtp6_Clk = 6,///<6 Clk
  IdsNvMemTimingTrtp7_Clk = 7,///<7 Clk
  IdsNvMemTimingTrtp8_Clk = 8,///<8 Clk
  IdsNvMemTimingTrtp9_Clk = 9,///<9 Clk
  IdsNvMemTimingTrtp0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTrtp0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTrtp0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTrtp0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTrtp0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTrtpAuto = 0xFF,///<Auto
} IdsNvMemTimingTrtp;

///tRAS
///Specify the min RAS# active time. D18F2x200_dct[1:0]_mp[1:0][29:24]
typedef enum {
  IdsNvMemTimingTras8_Clk = 8,///<8 Clk
  IdsNvMemTimingTras9_Clk = 9,///<9 Clk
  IdsNvMemTimingTras0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTras0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTras0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTras0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTras0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTras0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTras10h_Clk = 0x10,///<10h Clk
  IdsNvMemTimingTras11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTras12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTras13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTras14h_Clk = 0x14,///<14h Clk
  IdsNvMemTimingTras15h_Clk = 0x15,///<15h Clk
  IdsNvMemTimingTras16h_Clk = 0x16,///<16h Clk
  IdsNvMemTimingTras17h_Clk = 0x17,///<17h Clk
  IdsNvMemTimingTras18h_Clk = 0x18,///<18h Clk
  IdsNvMemTimingTras19h_Clk = 0x19,///<19h Clk
  IdsNvMemTimingTras1Ah_Clk = 0x1A,///<1Ah Clk
  IdsNvMemTimingTras1Bh_Clk = 0x1B,///<1Bh Clk
  IdsNvMemTimingTras1Ch_Clk = 0x1C,///<1Ch Clk
  IdsNvMemTimingTras1Dh_Clk = 0x1D,///<1Dh Clk
  IdsNvMemTimingTras1Eh_Clk = 0x1E,///<1Eh Clk
  IdsNvMemTimingTras1Fh_Clk = 0x1F,///<1Fh Clk
  IdsNvMemTimingTras20h_Clk = 0x20,///<20h Clk
  IdsNvMemTimingTras21h_Clk = 0x21,///<21h Clk
  IdsNvMemTimingTras22h_Clk = 0x22,///<22h Clk
  IdsNvMemTimingTras23h_Clk = 0x23,///<23h Clk
  IdsNvMemTimingTras24h_Clk = 0x24,///<24h Clk
  IdsNvMemTimingTras25h_Clk = 0x25,///<25h Clk
  IdsNvMemTimingTras26h_Clk = 0x26,///<26h Clk
  IdsNvMemTimingTras27h_Clk = 0x27,///<27h Clk
  IdsNvMemTimingTras28h_Clk = 0x28,///<28h Clk
  IdsNvMemTimingTras29h_Clk = 0x29,///<29h Clk
  IdsNvMemTimingTras2Ah_Clk = 0x2A,///<2Ah Clk
  IdsNvMemTimingTras2Bh_Clk = 0x2B,///<2Bh Clk
  IdsNvMemTimingTras2Ch_Clk = 0x2C,///<2Ch Clk
  IdsNvMemTimingTras2Dh_Clk = 0x2D,///<2Dh Clk
  IdsNvMemTimingTras2Eh_Clk = 0x2E,///<2Eh Clk
  IdsNvMemTimingTras2Fh_Clk = 0x2F,///<2Fh Clk
  IdsNvMemTimingTras30h_Clk = 0x30,///<30h Clk
  IdsNvMemTimingTras31h_Clk = 0x31,///<31h Clk
  IdsNvMemTimingTras32h_Clk = 0x32,///<32h Clk
  IdsNvMemTimingTras33h_Clk = 0x33,///<33h Clk
  IdsNvMemTimingTras34h_Clk = 0x34,///<34h Clk
  IdsNvMemTimingTras35h_Clk = 0x35,///<35h Clk
  IdsNvMemTimingTras36h_Clk = 0x36,///<36h Clk
  IdsNvMemTimingTrasAuto = 0xFF,///<Auto
} IdsNvMemTimingTras;

///tRRD
///Specify the row-to-row delay. D18F2x204_dct[1:0]_mp[1:0][11:8]
typedef enum {
  IdsNvMemTimingTrrd1_Clk = 1,///<1 Clk
  IdsNvMemTimingTrrd2_Clk = 2,///<2 Clk
  IdsNvMemTimingTrrd3_Clk = 3,///<3 Clk
  IdsNvMemTimingTrrd4_Clk = 4,///<4 Clk
  IdsNvMemTimingTrrd5_Clk = 5,///<5 Clk
  IdsNvMemTimingTrrd6_Clk = 6,///<6 Clk
  IdsNvMemTimingTrrd7_Clk = 7,///<7 Clk
  IdsNvMemTimingTrrd8_Clk = 8,///<8 Clk
  IdsNvMemTimingTrrd9_Clk = 9,///<9 Clk
  IdsNvMemTimingTrrd0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTrrd0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTrrd0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTrrd0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTrrdAuto = 0xFF,///<Auto
} IdsNvMemTimingTrrd;

///tRC
///Specify the RAS# active to RAS# active time. D18F2x204_dct[1:0]_mp[1:0][5:0]
typedef enum {
  IdsNvMemTimingTrc0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTrc0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTrc0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTrc0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTrc0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTrc0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTrc10h_Clk = 0x10,///<10h Clk
  IdsNvMemTimingTrc11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTrc12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTrc13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTrc14h_Clk = 0x14,///<14h Clk
  IdsNvMemTimingTrc15h_Clk = 0x15,///<15h Clk
  IdsNvMemTimingTrc16h_Clk = 0x16,///<16h Clk
  IdsNvMemTimingTrc17h_Clk = 0x17,///<17h Clk
  IdsNvMemTimingTrc18h_Clk = 0x18,///<18h Clk
  IdsNvMemTimingTrc19h_Clk = 0x19,///<19h Clk
  IdsNvMemTimingTrc1A_Clk = 0x1A,///<1A Clk
  IdsNvMemTimingTrc1B_Clk = 0x1B,///<1B Clk
  IdsNvMemTimingTrc1C_Clk = 0x1C,///<1C Clk
  IdsNvMemTimingTrc1D_Clk = 0x1D,///<1D Clk
  IdsNvMemTimingTrc1E_Clk = 0x1E,///<1E Clk
  IdsNvMemTimingTrc1F_Clk = 0x1F,///<1F Clk
  IdsNvMemTimingTrc20h_Clk = 0x20,///<20h Clk
  IdsNvMemTimingTrc21h_Clk = 0x21,///<21h Clk
  IdsNvMemTimingTrc22h_Clk = 0x22,///<22h Clk
  IdsNvMemTimingTrc23h_Clk = 0x23,///<23h Clk
  IdsNvMemTimingTrc24h_Clk = 0x24,///<24h Clk
  IdsNvMemTimingTrc25h_Clk = 0x25,///<25h Clk
  IdsNvMemTimingTrc26h_Clk = 0x26,///<26h Clk
  IdsNvMemTimingTrc27h_Clk = 0x27,///<27h Clk
  IdsNvMemTimingTrc28h_Clk = 0x28,///<28h Clk
  IdsNvMemTimingTrc29h_Clk = 0x29,///<29h Clk
  IdsNvMemTimingTrc2Ah_Clk = 0x2A,///<2Ah Clk
  IdsNvMemTimingTrc2Bh_Clk = 0x2B,///<2Bh Clk
  IdsNvMemTimingTrc2Ch_Clk = 0x2C,///<2Ch Clk
  IdsNvMemTimingTrc2Dh_Clk = 0x2D,///<2Dh Clk
  IdsNvMemTimingTrc2Eh_Clk = 0x2E,///<2Eh Clk
  IdsNvMemTimingTrc2Fh_Clk = 0x2F,///<2Fh Clk
  IdsNvMemTimingTrc30h_Clk = 0x30,///<30h Clk
  IdsNvMemTimingTrc31h_Clk = 0x31,///<31h Clk
  IdsNvMemTimingTrc32h_Clk = 0x32,///<32h Clk
  IdsNvMemTimingTrc33h_Clk = 0x33,///<33h Clk
  IdsNvMemTimingTrc34h_Clk = 0x34,///<34h Clk
  IdsNvMemTimingTrc35h_Clk = 0x35,///<35h Clk
  IdsNvMemTimingTrc36h_Clk = 0x36,///<36h Clk
  IdsNvMemTimingTrc37h_Clk = 0x37,///<37h Clk
  IdsNvMemTimingTrc38h_Clk = 0x38,///<38h Clk
  IdsNvMemTimingTrc39h_Clk = 0x39,///<39h Clk
  IdsNvMemTimingTrc3Ah_Clk = 0x3A,///<3Ah Clk
  IdsNvMemTimingTrc3Bh_Clk = 0x3B,///<3Bh Clk
  IdsNvMemTimingTrc3Ch_Clk = 0x3C,///<3Ch Clk
  IdsNvMemTimingTrc3Dh_Clk = 0x3D,///<3Dh Clk
  IdsNvMemTimingTrc3Eh_Clk = 0x3E,///<3Eh Clk
  IdsNvMemTimingTrc3Fh_Clk = 0x3F,///<3Fh Clk
  IdsNvMemTimingTrc40h_Clk = 0x40,///<40h Clk
  IdsNvMemTimingTrc41h_Clk = 0x41,///<41h Clk
  IdsNvMemTimingTrc42h_Clk = 0x42,///<42h Clk
  IdsNvMemTimingTrc43h_Clk = 0x43,///<43h Clk
  IdsNvMemTimingTrc44h_Clk = 0x44,///<44h Clk
  IdsNvMemTimingTrc45h_Clk = 0x45,///<45h Clk
  IdsNvMemTimingTrc46h_Clk = 0x46,///<46h Clk
  IdsNvMemTimingTrc47h_Clk = 0x47,///<47h Clk
  IdsNvMemTimingTrc48h_Clk = 0x48,///<48h Clk
  IdsNvMemTimingTrc49h_Clk = 0x49,///<49h Clk
  IdsNvMemTimingTrc4Ah_Clk = 0x4A,///<4Ah Clk
  IdsNvMemTimingTrc4Bh_Clk = 0x4B,///<4Bh Clk
  IdsNvMemTimingTrc4Ch_Clk = 0x4C,///<4Ch Clk
  IdsNvMemTimingTrc4Dh_Clk = 0x4D,///<4Dh Clk
  IdsNvMemTimingTrc4Eh_Clk = 0x4E,///<4Eh Clk
  IdsNvMemTimingTrcAuto = 0xFF,///<Auto
} IdsNvMemTimingTrc;

///tRWTTO
///This sets the tWRTTO time. D18F2x21C_dct[1:0]_mp[1:0][12:8]
typedef enum {
  IdsNvMemTimingTrwtTo2_Clk = 2,///<2 Clk
  IdsNvMemTimingTrwtTo3_Clk = 3,///<3 Clk
  IdsNvMemTimingTrwtTo4_Clk = 4,///<4 Clk
  IdsNvMemTimingTrwtTo5_Clk = 5,///<5 Clk
  IdsNvMemTimingTrwtTo6_Clk = 6,///<6 Clk
  IdsNvMemTimingTrwtTo7_Clk = 7,///<7 Clk
  IdsNvMemTimingTrwtTo8_Clk = 8,///<8 Clk
  IdsNvMemTimingTrwtTo9_Clk = 9,///<9 Clk
  IdsNvMemTimingTrwtTo0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTrwtTo0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTrwtTo0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTrwtTo0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTrwtTo0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTrwtTo0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTrwtTo10h_Clk = 0x10,///<10h Clk
  IdsNvMemTimingTrwtTo11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTrwtTo12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTrwtTo13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTrwtTo14h_Clk = 0x14,///<14h Clk
  IdsNvMemTimingTrwtTo15h_Clk = 0x15,///<15h Clk
  IdsNvMemTimingTrwtTo16h_Clk = 0x16,///<16h Clk
  IdsNvMemTimingTrwtTo17h_Clk = 0x17,///<17h Clk
  IdsNvMemTimingTrwtTo18h_Clk = 0x18,///<18h Clk
  IdsNvMemTimingTrwtTo19h_Clk = 0x19,///<19h Clk
  IdsNvMemTimingTrwtTo1Ah_Clk = 0x1A,///<1Ah Clk
  IdsNvMemTimingTrwtTo1Bh_Clk = 0x1B,///<1Bh Clk
  IdsNvMemTimingTrwtToAuto = 0xFF,///<Auto
} IdsNvMemTimingTrwtTo;

///tWR
///Specify when the last write is registered by the DRAM. D18F2x22C_dct[1:0]_mp[1:0][4:0]
typedef enum {
  IdsNvMemTimingTwr5_Clk = 5,///<5 Clk
  IdsNvMemTimingTwr6_Clk = 6,///<6 Clk
  IdsNvMemTimingTwr7_Clk = 7,///<7 Clk
  IdsNvMemTimingTwr8_Clk = 8,///<8 Clk
  IdsNvMemTimingTwr9_Clk = 9,///<9 Clk
  IdsNvMemTimingTwr0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTwr0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTwr0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTwr0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTwr0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTwr0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemTimingTwr11h_Clk = 0x11,///<11h Clk
  IdsNvMemTimingTwr12h_Clk = 0x12,///<12h Clk
  IdsNvMemTimingTwr13h_Clk = 0x13,///<13h Clk
  IdsNvMemTimingTwr14h_Clk = 0x14,///<14h Clk
  IdsNvMemTimingTwr15h_Clk = 0x15,///<15h Clk
  IdsNvMemTimingTwr16h_Clk = 0x16,///<16h Clk
  IdsNvMemTimingTwr17h_Clk = 0x17,///<17h Clk
  IdsNvMemTimingTwr18h_Clk = 0x18,///<18h Clk
  IdsNvMemTimingTwr19h_Clk = 0x19,///<19h Clk
  IdsNvMemTimingTwr1Ah_Clk = 0x1A,///<1Ah Clk
  IdsNvMemTimingTwrAuto = 0xFF,///<Auto
} IdsNvMemTimingTwr;

///tWRRD
///Specify the write to read delay when accessing different DIMMs. D18F2x218_dct[1:0]_mp[1:0][11:8]
typedef enum {
  IdsNvMemTimingTwrrd1_Clk = 1,///<1 Clk
  IdsNvMemTimingTwrrd2_Clk = 2,///<2 Clk
  IdsNvMemTimingTwrrd3_Clk = 3,///<3 Clk
  IdsNvMemTimingTwrrd4_Clk = 4,///<4 Clk
  IdsNvMemTimingTwrrd5_Clk = 5,///<5 Clk
  IdsNvMemTimingTwrrd6_Clk = 6,///<6 Clk
  IdsNvMemTimingTwrrd7_Clk = 7,///<7 Clk
  IdsNvMemTimingTwrrd8_Clk = 8,///<8 Clk
  IdsNvMemTimingTwrrd9_Clk = 9,///<9 Clk
  IdsNvMemTimingTwrrd0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTwrrd0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTwrrdAuto = 0xFF,///<Auto
} IdsNvMemTimingTwrrd;

///tWTR
///Specify the write to read delay when accessing the same DIMM. D18F2x20C_dct[1:0]_mp[1:0][11:8]
typedef enum {
  IdsNvMemTimingTwtr2_Clk = 2,///<2 Clk
  IdsNvMemTimingTwtr3_Clk = 3,///<3 Clk
  IdsNvMemTimingTwtr4_Clk = 4,///<4 Clk
  IdsNvMemTimingTwtr5_Clk = 5,///<5 Clk
  IdsNvMemTimingTwtr6_Clk = 6,///<6 Clk
  IdsNvMemTimingTwtr7_Clk = 7,///<7 Clk
  IdsNvMemTimingTwtr8_Clk = 8,///<8 Clk
  IdsNvMemTimingTwtr9_Clk = 9,///<9 Clk
  IdsNvMemTimingTwtr0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemTimingTwtr0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemTimingTwtr0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemTimingTwtr0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemTimingTwtr0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemTimingTwtrAuto = 0xFF,///<Auto
} IdsNvMemTimingTwtr;

///tWRWRSdSc
///write to write timing same DIMM same chip select. D18F2x214_dct[1:0]_mp[1:0][19:16]
typedef enum {
  IdsNvMemtimingTwrwrSdsc1_Clk = 1,///<1 Clk
  IdsNvMemtimingTwrwrSdsc2_Clk = 2,///<2 Clk
  IdsNvMemtimingTwrwrSdsc3_Clk = 3,///<3 Clk
  IdsNvMemtimingTwrwrSdsc4_Clk = 4,///<4 Clk
  IdsNvMemtimingTwrwrSdsc5_Clk = 5,///<5 Clk
  IdsNvMemtimingTwrwrSdsc6_Clk = 6,///<6 Clk
  IdsNvMemtimingTwrwrSdsc7_Clk = 7,///<7 Clk
  IdsNvMemtimingTwrwrSdsc8_Clk = 8,///<8 Clk
  IdsNvMemtimingTwrwrSdsc9_Clk = 9,///<9 Clk
  IdsNvMemtimingTwrwrSdsc0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTwrwrSdsc0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTwrwrSdscAuto = 0xFF,///<Auto
} IdsNvMemtimingTwrwrSdsc;

///tWRWRSdDc
///write to write timing same DIMM same chip select. D18F2x214_dct[1:0]_mp[1:0][11:8]
typedef enum {
  IdsNvMemtimingTwrwrSdDc2_Clk = 2,///<2 Clk
  IdsNvMemtimingTwrwrSdDc3_Clk = 3,///<3 Clk
  IdsNvMemtimingTwrwrSdDc4_Clk = 4,///<4 Clk
  IdsNvMemtimingTwrwrSdDc6_Clk = 6,///<6 Clk
  IdsNvMemtimingTwrwrSdDc5_Clk = 5,///<5 Clk
  IdsNvMemtimingTwrwrSdDc7_Clk = 7,///<7 Clk
  IdsNvMemtimingTwrwrSdDc8_Clk = 8,///<8 Clk
  IdsNvMemtimingTwrwrSdDc9_Clk = 9,///<9 Clk
  IdsNvMemtimingTwrwrSdDc0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTwrwrSdDc0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTwrwrSdDcAuto = 0xFF,///<Auto
} IdsNvMemtimingTwrwrSdDc;

///tWRWRDd
///write to write timing same DIMM same chip select. D18F2x214_dct[1:0]_mp[1:0][3:0]
typedef enum {
  IdsNvMemtimingTwrwrDd2_Clk = 2,///<2 Clk
  IdsNvMemtimingTwrwrDd3_Clk = 3,///<3 Clk
  IdsNvMemtimingTwrwrDd4_Clk = 4,///<4 Clk
  IdsNvMemtimingTwrwrDd5_Clk = 5,///<5 Clk
  IdsNvMemtimingTwrwrDd6_Clk = 6,///<6 Clk
  IdsNvMemtimingTwrwrDd7_Clk = 7,///<7 Clk
  IdsNvMemtimingTwrwrDd8_Clk = 8,///<8 Clk
  IdsNvMemtimingTwrwrDd9_Clk = 9,///<9 Clk
  IdsNvMemtimingTwrwrDd0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTwrwrDd0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTwrwrDdAuto = 0xFF,///<Auto
} IdsNvMemtimingTwrwrDd;

///tRDRDSdSc
///write to write timing same DIMM same chip select. D18F2x218_dct[1:0]_mp[1:0][27:24]
typedef enum {
  IdsNvMemtimingTRdRdSdsc1_Clk = 1,///<1 Clk
  IdsNvMemtimingTRdRdSdsc2_Clk = 2,///<2 Clk
  IdsNvMemtimingTRdRdSdsc3_Clk = 3,///<3 Clk
  IdsNvMemtimingTRdRdSdsc4_Clk = 4,///<4 Clk
  IdsNvMemtimingTRdRdSdsc5_Clk = 5,///<5 Clk
  IdsNvMemtimingTRdRdSdsc6_Clk = 6,///<6 Clk
  IdsNvMemtimingTRdRdSdsc7_Clk = 7,///<7 Clk
  IdsNvMemtimingTRdRdSdsc8_Clk = 8,///<8 Clk
  IdsNvMemtimingTRdRdSdsc9_Clk = 9,///<9 Clk
  IdsNvMemtimingTRdRdSdsc0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTRdRdSdsc0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTRdRdSdscAuto = 0xFF,///<Auto
} IdsNvMemtimingTRdRdSdsc;

///tRDRDSdDc
///write to write timing same DIMM same chip select. D18F2x218_dct[1:0]_mp[1:0][19:16]
typedef enum {
  IdsNvMemtimingTRdRdSdDc2_Clk = 2,///<2 Clk
  IdsNvMemtimingTRdRdSdDc3_Clk = 3,///<3 Clk
  IdsNvMemtimingTRdRdSdDc4_Clk = 4,///<4 Clk
  IdsNvMemtimingTRdRdSdDc5_Clk = 5,///<5 Clk
  IdsNvMemtimingTRdRdSdDc6_Clk = 6,///<6 Clk
  IdsNvMemtimingTRdRdSdDc8_Clk = 8,///<8 Clk
  IdsNvMemtimingTRdRdSdDc7_Clk = 7,///<7 Clk
  IdsNvMemtimingTRdRdSdDc9_Clk = 9,///<9 Clk
  IdsNvMemtimingTRdRdSdDc0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTRdRdSdDc0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTRdRdSdDcAuto = 0xFF,///<Auto
} IdsNvMemtimingTRdRdSdDc;

///tRDRDDd
///write to write timing same DIMM same chip select. D18F2x218_dct[1:0]_mp[1:0][3:0]
typedef enum {
  IdsNvMemtimingTRdRdDd2_Clk = 2,///<2 Clk
  IdsNvMemtimingTRdRdDd3_Clk = 3,///<3 Clk
  IdsNvMemtimingTRdRdDd4_Clk = 4,///<4 Clk
  IdsNvMemtimingTRdRdDd5_Clk = 5,///<5 Clk
  IdsNvMemtimingTRdRdDd7_Clk = 7,///<7 Clk
  IdsNvMemtimingTRdRdDd6_Clk = 6,///<6 Clk
  IdsNvMemtimingTRdRdDd8_Clk = 8,///<8 Clk
  IdsNvMemtimingTRdRdDd9_Clk = 9,///<9 Clk
  IdsNvMemtimingTRdRdDd0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTRdRdDd0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTRdRdDdAuto = 0xFF,///<Auto
} IdsNvMemtimingTRdRdDd;

///tREF Control
typedef enum {
  IdsNvMemTimingTrefCtlManual = 0x0,///<Tref Manual
  IdsNvMemTimingTrefCtlAuto = 0x1,///<Tref Auto
} IdsNvMemTimingTrefCtl;

///tREF
///Specify the tRef time. D18F2x8C_dct[1:0][17:16]
typedef enum {
  IdsNvMemTimingTrefMin = 0x0,///<Min Tref
  IdsNvMemTimingTrefMax = 0xFFFF,///<Max Tref
} IdsNvMemTimingTref;

///tRWtWB
///Specify the Trwtwb time. D18F2x21C_dct[1:0]_mp[1:0][20:16]
typedef enum {
  IdsNvMemtimingTrwtWB3_Clk = 3,///<3 Clk
  IdsNvMemtimingTrwtWB4_Clk = 4,///<4 Clk
  IdsNvMemtimingTrwtWB5_Clk = 5,///<5 Clk
  IdsNvMemtimingTrwtWB6_Clk = 6,///<6 Clk
  IdsNvMemtimingTrwtWB7_Clk = 7,///<7 Clk
  IdsNvMemtimingTrwtWB8_Clk = 8,///<8 Clk
  IdsNvMemtimingTrwtWB9_Clk = 9,///<9 Clk
  IdsNvMemtimingTrwtWB0Ah_Clk = 0xA,///<0Ah Clk
  IdsNvMemtimingTrwtWB0Bh_Clk = 0xB,///<0Bh Clk
  IdsNvMemtimingTrwtWB0Ch_Clk = 0xC,///<0Ch Clk
  IdsNvMemtimingTrwtWB0Dh_Clk = 0xD,///<0Dh Clk
  IdsNvMemtimingTrwtWB0Eh_Clk = 0xE,///<0Eh Clk
  IdsNvMemtimingTrwtWB0Fh_Clk = 0xF,///<0Fh Clk
  IdsNvMemtimingTrwtWB10h_Clk = 0x10,///<10h Clk
  IdsNvMemtimingTrwtWB11h_Clk = 0x11,///<11h Clk
  IdsNvMemtimingTrwtWB12h_Clk = 0x12,///<12h Clk
  IdsNvMemtimingTrwtWB13h_Clk = 0x13,///<13h Clk
  IdsNvMemtimingTrwtWB14h_Clk = 0x14,///<14h Clk
  IdsNvMemtimingTrwtWB15h_Clk = 0x15,///<15h Clk
  IdsNvMemtimingTrwtWB16h_Clk = 0x16,///<16h Clk
  IdsNvMemtimingTrwtWB17h_Clk = 0x17,///<17h Clk
  IdsNvMemtimingTrwtWB18h_Clk = 0x18,///<18h Clk
  IdsNvMemtimingTrwtWB19h_Clk = 0x19,///<19h Clk
  IdsNvMemtimingTrwtWB1Ah_Clk = 0x1A,///<1Ah Clk
  IdsNvMemtimingTrwtWB1Bh_Clk = 0x1B,///<1Bh Clk
  IdsNvMemtimingTrwtWB1Ch_Clk = 0x1C,///<1Ch Clk
  IdsNvMemtimingTrwtWBAuto = 0xFF,///<Auto
} IdsNvMemtimingTrwtWB;

///trFCx
///Trfcx control
typedef enum {
  IdsNvMemTimingTrfcxManual = 0,///<Manual
  IdsNvMemTimingTrfcxAuto = 1,///<Auto
} IdsNvMemTimingTrfcxCtl;

///trFCx
typedef enum {
  IdsNvMemTimingTrfcxMin = 1,///<Trfx Min
  IdsNvMemTimingTrfcxMax = 0x7FF,///<Trfx Max
} IdsNvMemTimingTrfcx;

///tCKSRX Control
///
typedef enum {
  IdsNvMemTimingTcksrxCtlManual = 0,///<Manual
  IdsNvMemTimingTcksrxCtlAuto = 1,///<Auto
} IdsNvMemTimingTcksrxCtl;

///tCKSRX
///
#define IdsNvMemtimingTcksrxMin 0x0
#define IdsNvMemtimingTcksrxMax 0x3F

///tCKSRE Control
///
typedef enum {
  IdsNvMemTimingTcksreCtlManual = 0,///<Manual
  IdsNvMemTimingTcksreCtlAuto = 1,///<Auto
} IdsNvMemTimingTcksreCtl;

///tCKSRE
///
#define IdsNvMemTimingTcksreMin 0x5
#define IdsNvMemTimingTcksreMax 0x3F

///tCKESR Control
///
typedef enum {
  IdsNvMemTckesrTimingCtlManual = 0,///<Manual
  IdsNvMemTckesrTimingCtlAuto = 1,///<Auto
} IdsNvMemTckesrTimingCtl;

///tCKESR
///
#define IdsNvMemTimingTckesrMin 0x2
#define IdsNvMemTimingTckesrMax 0x3F

///tPD Control
///
typedef enum {
  IdsNvMemTimingTpdCtlManual = 0,///<Manual
  IdsNvMemTimingTpdCtlAuto = 1,///<Auto
} IdsNvMemTimingTpdCtl;

///tPD
///
#define IdsNvMemTimingTpdMin 0x1
#define IdsNvMemTimingTpdMax 0xF

///RdPtr Control
///
typedef enum {
  IdsNvMemTimingRdPtrCtlManual = 0,///<Manual
  IdsNvMemTimingRdPtrCtlAuto = 1,///<Auto
} IdsNvMemTimingRdPtrCtl;

///RdPtr
typedef enum {
  IdsNvMemTimingRdPtrMin = 0,///<RdPtrVal Min
  IdsNvMemTimingRdPtrMax = 0x7F,///<RdPtrVal Max
} IdsNvMemTimingRdPtr;

///Enforce VDDIO
///
typedef enum {
  IdsNvMemCtrllerEnforceVDDIOVDDIO_1_5V = 0,///<VDDIO 1.5V
  IdsNvMemCtrllerEnforceVDDIOVDDIO_1_35V = 1,///<VDDIO 1.35V
  IdsNvMemCtrllerEnforceVDDIOVDDIO_1_25V = 2,///<VDDIO 1.25V
  IdsNvMemCtrllerEnforceVDDIOAuto = 7,///<Auto
} IdsNvMemCtrllerEnforceVDDIO;

///DRAM Controller User Controls
///Specify the mode for controller initialization to Auto or Manual
typedef enum {
  IdsNvMemCtrllerCtlAuto = 0,///<Auto
  IdsNvMemCtrllerCtlManual = 1,///<Manual
} IdsNvMemCtrllerCtl;

///DRAM Controller Selection
///DRAM Controller Selection
typedef enum {
  IdsNvMemCtrllerSelAll = 0, ///<All
  IdsNvMemCtrllerSelDCT0 = 1,///<DCT0
  IdsNvMemCtrllerSelDCT1 = 2,///<DCT1
  IdsNvMemCtrllerSelDCT2 = 3,///<DCT1
  IdsNvMemCtrllerSelDCT3 = 4,///<DCT1
  IdsNvMemCtrllerSelAuto = 0xFF,///<Auto
} IdsNvMemCtrllerSel;

///Power Down Enable
///Enable or disable DDR power down mode, D18F2x94_dct[1:0][15]
typedef enum {
  IdsNvMemCtrllerPwrDnEnDisabled = 0,///<Disabled
  IdsNvMemCtrllerPwrDnEnEnabled = 1,///<Enabled
  IdsNvMemCtrllerPwrDnEnAuto = 3,///<Auto
} IdsNvMemCtrllerPwrDnEn;

///Power Down Mode
///Set the DDR power down mode
typedef enum {
  IdsNvMemCtrllerPwrDnMoChannel = 0,///<Channel
  IdsNvMemCtrllerPwrDnMoChip_Select = 1,///<Chip Select
  IdsNvMemCtrllerPwrDnMoAuto = 3,///<Auto
} IdsNvMemCtrllerPwrDnMo;

///Precharge Power down mode select
///Specifies how a chip select enters and exits power down mode. D18F2x84_dct[1:0][23]
typedef enum {
  IdsNvMemCtrllerPrechargePDModeSelSlow = 0,///<Slow
  IdsNvMemCtrllerPrechargePDModeSelFast = 1,///<Fast
  IdsNvMemCtrllerPrechargePDModeSelDynamic = 2,///<Dynamic
  IdsNvMemCtrllerPrechargePDModeSelAuto = 3,///<Auto
} IdsNvMemCtrllerPrechargePDModeSel;

///Precharge Power Down Entry Delay Control
///Specifies if user want to enable the power down entry delay
typedef enum {
  IdsNvMemCtrllerPrechargePDDlyCtlManual = 0,///<Manual
  IdsNvMemCtrllerPrechargePDDlyCtlAuto = 1,///<Auto
} IdsNvMemCtrllerPrechargePDDlyCtl;

///Precharge Power Down Entry Delay
///Specifies the power down entry delay. D18F2x248_dct[1:0]_mp[1:0][21:16]
#define IdsNvMemCtrllerPrechargePDDlyMin 0
#define IdsNvMemCtrllerPrechargePDDlyMax 0x3F

///Power Down phy power save disable
///1= Disable phy clock gating during precharge power down. 0=Enable phy clock gating during precharge power down to save power. D18F2xA8_dct[1:0][19]
typedef enum {
  IdsNvMemCtrllerPDPhyPowerSaveDis0 = 0,///<0
  IdsNvMemCtrllerPDPhyPowerSaveDis1 = 1,///<1
  IdsNvMemCtrllerPDPhyPowerSaveDisAuto = 3,///<Auto
} IdsNvMemCtrllerPDPhyPowerSaveDis;

///Refresh channel Command Management Disable
///1=DCTs issue refresh commands independently. 0=DCTs stagger the issue of refresh commands. D18F2xA8_dct[1:0][29]
typedef enum {
  IdsNvMemCtrllerRefreshChlCmdManageDis0 = 0,///<0
  IdsNvMemCtrllerRefreshChlCmdManageDis1 = 1,///<1
  IdsNvMemCtrllerRefreshChlCmdManageDisAuto = 3,///<Auto
} IdsNvMemCtrllerRefreshChlCmdManageDis;

///Fast self refresh entry disable
///1=DCT pushes outstanding transactions to memory prior to entering self refresh. 0=DCT enters self refresh immediately unless instructed to push outstanding transactions to memory by D18F2x11C[FlushWrOnStpGnt] or D18F2x1B4[FlushWrOnS3StpGnt]. D18F2xA8_dct[1:0][28]
typedef enum {
  IdsNvMemCtrllerFastSelfRefreshEntryDis0 = 0,///<0
  IdsNvMemCtrllerFastSelfRefreshEntryDis1 = 1,///<1
  IdsNvMemCtrllerFastSelfRefreshEntryDisAuto = 3,///<Auto
} IdsNvMemCtrllerFastSelfRefreshEntryDis;

///Opportunistic write enh disable
///1=Disable enhancement logic. 0=Under certain conditions, low priority writes do not arbitrate or participate in auto precharge deter determination for other commands. D18F2xA8_dct[1:0][23]
typedef enum {
  IdsNvMemCtrllerOpportunisticWriEnhDis0 = 0,///<0
  IdsNvMemCtrllerOpportunisticWriEnhDis1 = 1,///<1
  IdsNvMemCtrllerOpportunisticWriEnhDisAuto = 3,///<Auto
} IdsNvMemCtrllerOpportunisticWriEnhDis;

///Partial channel power down enh Enable
///1=Hysteresis specified by D18F2x244_dct[1:0][PrtlChPDDynDly]. 0=256 clock hysteresis.
typedef enum {
  IdsNvMemCtrllerPartialChlPDEnhEnPartial_Chl_Power_down_delay = 1,///<Partial Chl Power down delay
  IdsNvMemCtrllerPartialChlPDEnhEn256_clocks = 0,///<256 clocks
  IdsNvMemCtrllerPartialChlPDEnhEnAuto = 3,///<Auto
} IdsNvMemCtrllerPartialChlPDEnhEn;

///Aggressive power down enable
///D18F2xA8_dct[1:0][21]
typedef enum {
  IdsNvMemCtrllerAggressivePwrDownEnDisabled = 0,///<Disabled
  IdsNvMemCtrllerAggressivePwrDownEnEnabled = 1,///<Enabled
  IdsNvMemCtrllerAggressivePwrDownEnAuto = 3,///<Auto
} IdsNvMemCtrllerAggressivePwrDownEn;

///Address command tristate enable
///1=Tristate the address, command, and bank buses when a Deselect command is issued. D18F2x78_dct[1:0][17]
typedef enum {
  IdsNvMemCtrllerAddrCmdTriEnDisabled = 0,///<Disabled
  IdsNvMemCtrllerAddrCmdTriEnEnabled = 1,///<Enabled
  IdsNvMemCtrllerAddrCmdTriEnAuto = 3,///<Auto
} IdsNvMemCtrllerAddrCmdTriEn;

///Partial chl power down dynamic delay control
///
typedef enum {
  IdsNvMemCtrllerPartialChlPDDynDlyCtlManual = 0,///<Manual
  IdsNvMemCtrllerPartialChlPDDynDlyCtlAuto = 1,///<Auto
} IdsNvMemCtrllerPartialChlPDDynDlyCtl;

///Partial chl power down dynamic delay
///Specifies the channel idle hysteresis for fast exit/slow exit mode changes when D18F2xA8_dct[1:0][PrtlChPDEnhEn]=1. D18F2x244_dct[1:0][3:0]
#define IdsNvMemCtrllerPartialChlPDDynDlyMin 0
#define IdsNvMemCtrllerPartialChlPDDynDlyMax 8

///Aggressive power down delay control
///
typedef enum {
  IdsNvMemCtrllerAggrePDDlyCtlManual = 0,///<Manual
  IdsNvMemCtrllerAggrePDDlyCtlAuto = 1,///<Auto
} IdsNvMemCtrllerAggrePDDlyCtl;

///Aggressive power down delay
///Specifies a hysteresis count from the last DRAM activity for the DCT to close pages prior to precharge power down. D18F2x248_dct[1:0]_mp[1:0][29:24]
#define IdsNvMemCtrllerAggrePDDlyMin 0
#define IdsNvMemCtrllerAggrePDDlyMax 0x3F

///Slow Access Mode
///Select between 1T and 2T mode on ADDR/CMD
typedef enum {
  IdsNvMemCtrller2TMode1T = 0,///<1T
  IdsNvMemCtrller2TMode2T = 1,///<2T
  IdsNvMemCtrller2TModeAuto = 3,///<Auto
} IdsNvMemCtrller2TMode;

///Data Scramble
typedef enum {
  IdsNvMemCtrllerScrambleDis = 0,///<Disable
  IdsNvMemCtrllerScrambleEn = 1,///<Enable
  IdsNvMemCtrllerScrambleAuto = 3,///<Auto
} IdsNvMemCtrllerScramble;

///BankGroupSwap
typedef enum {
  IdsNvMemCtrllerBankGroupSwapDis = 0,///<Disable
  IdsNvMemCtrllerBankGroupSwapEn = 1,///<Enable
  IdsNvMemCtrllerBankGroupSwapAuto = 3,///<Auto
} IdsNvMemCtrllerBankGroupSwap;

///WrCrcEn
typedef enum {
  IdsNvMemCtrllerWrCrcDis = 0,///<Disable
  IdsNvMemCtrllerWrCrcEn = 1,///<Enable
  IdsNvMemCtrllerWrCrcAuto = 3,///<Auto
} IdsNvMemCtrllerWrCrc;

///Bank Swizzle Mode
///Enable or disable bank swizzle mode
typedef enum {
  IdsNvMemCtrllerBankSwizzleDisabled = 0,///<Disabled
  IdsNvMemCtrllerBankSwizzleEnabled = 1,///<Enabled
  IdsNvMemCtrllerBankSwizzleAuto = 3,///<Auto
} IdsNvMemCtrllerBankSwizzle;

///Phy DLL Standby Control
///Enable or disable Phy DLL Standby
typedef enum {
  IdsNvMemCtrllerDLLStandbyCtlAuto = 0,///<Auto
  IdsNvMemCtrllerDLLStandbyCtlManual = 1,///<Manual
} IdsNvMemCtrllerDLLStandbyCtl;

///RxDllWakeupTime
///Receive DLL wakeup time
#define IdsNvMemCtrllerRxDllWakeupTimeMin 0
#define IdsNvMemCtrllerRxDllWakeupTimeMax 3

///RxCPUpdPeriod
///Receive charge pump period
#define IdsNvMemCtrllerRxCPUpdPeriodMin 0
#define IdsNvMemCtrllerRxCPUpdPeriodMax 7

///RxMaxDurDllNoLock
///Receive maximum duration DLL no lock
#define IdsNvMemCtrllerRxMaxDurDllNoLockMin 0
#define IdsNvMemCtrllerRxMaxDurDllNoLockMax 0xF

///TxDllWakeupTime
///Transmit DLL wakeup time
#define IdsNvMemCtrllerTxDllWakeupTimeMin 0
#define IdsNvMemCtrllerTxDllWakeupTimeMax 3

///TxCPUpdPeriod
///Transmit charge pump period
#define IdsNvMemCtrllerTxCPUpdPeriodMin 0
#define IdsNvMemCtrllerTxCPUpdPeriodMax 7

///TxMaxDurDllNoLock
///Transmit maximum duration DLL no lock
#define IdsNvMemCtrllerTxMaxDurDllNoLockMin 0
#define IdsNvMemCtrllerTxMaxDurDllNoLockMax 0xF

///All Memory Clks Enable
///This allows selection of all memory clocks enable
typedef enum {
  IdsNvMemCtrllerAllMemClksDisabled = 0,///<Disabled
  IdsNvMemCtrllerAllMemClksEnabled = 1,///<Enabled
  IdsNvMemCtrllerAllMemClksAuto = 3,///<Auto
} IdsNvMemCtrllerAllMemClks;

///IntLvRgnSwapMode
///Interleave region swap mode. D18F2x10C[0]
typedef enum {
  IdsNvMemCtrllerIntLvRgnSwapModeDisabled = 0,///<Disabled
  IdsNvMemCtrllerIntLvRgnSwapModeManual = 1,///<Manual
  IdsNvMemCtrllerIntLvRgnSwapModeAuto = 3,///<Auto
} IdsNvMemCtrllerIntLvRgnSwapMode;

///IntLvRgnBaseAddr
///Interleave swap region base address bits[33:27]. D18F2x10C[9:3]
#define IdsNvMemCtrllerIntLvRgnBaseAddrMin 0
#define IdsNvMemCtrllerIntLvRgnBaseAddrMax 0x7F

///IntLvRgnLmtAddr
///Interleave swap region limit address bits[33:27]. D18F2x10C[17:11]
#define IdsNvMemCtrllerIntLvRgnLmtAddrMin 0
#define IdsNvMemCtrllerIntLvRgnLmtAddrMax 0x7F

///DramTermDyn
///Specifies the programming of the DDR3 DRAM dynamic termination value for writes, Rtt_Wr. D18F2x84_dct[1:0][11:10]
typedef enum {
  IdsNvMemCtrllerDramTermDynDisabled = 0,///<Disabled
  IdsNvMemCtrllerDramTermDynRZQ_4 = 1,///<RZQ/4
  IdsNvMemCtrllerDramTermDynRZQ_2 = 2,///<RZQ/2
  IdsNvMemCtrllerDramTermDynAuto = 7,///<Auto
} IdsNvMemCtrllerDramTermDyn;

///DramTerm
///Specifies the programming of the DDR3 DRAM nominal termination value, Rtt_Nom D18F2x84_dct[1:0][9:7]
typedef enum {
  IdsNvMemCtrllerDramTermDisabled = 0,///<Disabled
  IdsNvMemCtrllerDramTermRZQ_4 = 1,///<RZQ/4
  IdsNvMemCtrllerDramTermRZQ_2 = 2,///<RZQ/2
  IdsNvMemCtrllerDramTermRZQ_6 = 3,///<RZQ/6
  IdsNvMemCtrllerDramTermRZQ_12 = 4,///<RZQ/12
  IdsNvMemCtrllerDramTermRZQ_8 = 5,///<RZQ/8
  IdsNvMemCtrllerDramTermAuto = 7,///<Auto
} IdsNvMemCtrllerDramTerm;

///Data transmit FIFO write delay
///Specifies the DCT to phy write data FIFO delay. D18F2x210_dct[1:0]_nbp[3:0][18:16]
typedef enum {
  IdsNvMemCtrllerDataTransFIFOWriDly0_MemClk = 0,///<0 MemClk
  IdsNvMemCtrllerDataTransFIFOWriDly0_5_MemClk = 1,///<0.5 MemClk
  IdsNvMemCtrllerDataTransFIFOWriDly1_0_MemClk = 2,///<1.0 MemClk
  IdsNvMemCtrllerDataTransFIFOWriDly1_5_MemClk = 3,///<1.5 MemClk
  IdsNvMemCtrllerDataTransFIFOWriDly2_0_MemClk = 4,///<2.0 MemClk
  IdsNvMemCtrllerDataTransFIFOWriDly2_5_MemClk = 5,///<2.5 MemClk
  IdsNvMemCtrllerDataTransFIFOWriDly3_0__MemClk = 6,///<3.0  MemClk
  IdsNvMemCtrllerDataTransFIFOWriDlyAuto = 0xFF,///<Auto
} IdsNvMemCtrllerDataTransFIFOWriDly;

///On DIMM temperature sensor enable
///D18F2xA4_dct[1:0][8]
typedef enum {
  IdsNvMemCtrllerOnDIMMTempSensorEnEnabled = 1,///<Enabled
  IdsNvMemCtrllerOnDIMMTempSensorEnDisabled = 0,///<Disabled
  IdsNvMemCtrllerOnDIMMTempSensorEnAuto = 3,///<Auto
} IdsNvMemCtrllerOnDIMMTempSensorEn;

///Disable DLL shutdown in self-refresh mode
///1=Disable the power saving features of shutting down DDR phy DLLs during DRAM self refresh and memory P-states. 0=Shutdown DLLs during DRAM self refresh and allow memory P state transitions.
typedef enum {
  IdsNvMemCtrllerDisDllShutdownSR0 = 0,///<0
  IdsNvMemCtrllerDisDllShutdownSR1 = 1,///<1
  IdsNvMemCtrllerDisDllShutdownSRAuto = 3,///<Auto
} IdsNvMemCtrllerDisDllShutdownSR;

///Fast M-state change disable
///1=The DCT changes MEMCLK frequency only after the NCLK frequency has changed. 0=The DCT changes MEMCLK frequency while the northbridge changes NCLK.
typedef enum {
  IdsNvMemCtrllerFastMstateDis0 = 0,///<0
  IdsNvMemCtrllerFastMstateDis1 = 1,///<1
  IdsNvMemCtrllerFastMstateDisAuto = 3,///<Auto
} IdsNvMemCtrllerFastMstateDis;

///Receive channel maitenance clocks
///
typedef enum {
  IdsNvMemCtrllerRxChMntClkEnDisabled = 0,///<Disabled
  IdsNvMemCtrllerRxChMntClkEnEnabled = 1,///<Enabled
  IdsNvMemCtrllerRxChMntClkEnAuto = 3,///<Auto
} IdsNvMemCtrllerRxChMntClkEn;

///Always enable DLL clocks
///
typedef enum {
  IdsNvMemCtrllerAlwaysEnDllCksDisabled = 0,///<Disabled
  IdsNvMemCtrllerAlwaysEnDllCksEnabled = 1,///<Enabled
  IdsNvMemCtrllerAlwaysEnDllCksAuto = 3,///<Auto
} IdsNvMemCtrllerAlwaysEnDllCks;

///Dll no lock
///
typedef enum {
  IdsNvMemCtrllerDllNoLockDisabled = 0,///<Disabled
  IdsNvMemCtrllerDllNoLock1 = 1,///<1
  IdsNvMemCtrllerDllNoLock2 = 2,///<2
  IdsNvMemCtrllerDllNoLock3 = 3,///<3
  IdsNvMemCtrllerDllNoLock4 = 4,///<4
  IdsNvMemCtrllerDllNoLock5 = 5,///<5
  IdsNvMemCtrllerDllNoLock6 = 6,///<6
  IdsNvMemCtrllerDllNoLock7 = 7,///<7
  IdsNvMemCtrllerDllNoLock8 = 8,///<8
  IdsNvMemCtrllerDllNoLock9 = 9,///<9
  IdsNvMemCtrllerDllNoLock0xA = 10,///<0xA
  IdsNvMemCtrllerDllNoLock0xB = 11,///<0xB
  IdsNvMemCtrllerDllNoLock0xC = 12,///<0xC
  IdsNvMemCtrllerDllNoLock0xD = 13,///<0xD
  IdsNvMemCtrllerDllNoLock0xE = 14,///<0xE
  IdsNvMemCtrllerDllNoLock0xF = 15,///<0xF
  IdsNvMemCtrllerDllNoLockAuto = 0xFF,///<Auto
} IdsNvMemCtrllerDllNoLock;

///Enable receiver pad standby
///
typedef enum {
  IdsNvMemCtrllerEnRxPadStandbyDisabled = 0,///<Disabled
  IdsNvMemCtrllerEnRxPadStandbyEnabled = 1,///<Enabled
  IdsNvMemCtrllerEnRxPadStandbyAuto = 3,///<Auto
} IdsNvMemCtrllerEnRxPadStandby;

///Disable simultaneous read and write
///1=Disable the possibility of simultaneous reads from one DCT and writes to the other DCT.0=Enable the possibility of simultaneous reads from one DCT and writes to the other DCT.
typedef enum {
  IdsNvMemCtrllerDisSimulRdWr0 = 0,///<0
  IdsNvMemCtrllerDisSimulRdWr1 = 1,///<1
  IdsNvMemCtrllerDisSimulRdWrAuto = 3,///<Auto
} IdsNvMemCtrllerDisSimulRdWr;

///Efficient arbitration disable
///0=The DCT optimizes the arbitration phases to improve performance under certain traffic conditions whenever the NCLK to MEMCLK ratio is less than 2:1. 1=The DCT arbitrates normally at all NCLK:MEMCLK ratios.
typedef enum {
  IdsNvMemCtrllerEffArbDis0 = 0,///<0
  IdsNvMemCtrllerEffArbDis1 = 1,///<1
  IdsNvMemCtrllerEffArbDisAuto = 3,///<Auto
} IdsNvMemCtrllerEffArbDis;

///Disable HalfNCLK power gate
///1=Disable power gating. 0=Clocks are gated and held to zero during low power states.
typedef enum {
  IdsNvMemCtrllerDisHalfNclkPwrGate0 = 0,///<0
  IdsNvMemCtrllerDisHalfNclkPwrGate1 = 1,///<1
  IdsNvMemCtrllerDisHalfNclkPwrGateAuto = 3,///<Auto
} IdsNvMemCtrllerDisHalfNclkPwrGate;

///DCT Power Gate Control
///
typedef enum {
  IdsControlDctPowerGateCtlManual = 0,///<Manual
  IdsControlDctPowerGateCtlAuto = 1,///<Auto
} IdsControlDctPowerGateCtl;

///DCT Power Gate Control
///
typedef enum {
  IdsControlDctPowerGateEnMin = 0x0,///<Min
  IdsControlDctPowerGateEnMax = 0x3,///<Max
} IdsControlDctPowerGateEn;

///DCT Clk Gate Control
///
typedef enum {
  IdsControlDctClkGateCtlManual = 0,///<Manual
  IdsControlDctClkGateCtlAuto = 1,///<Auto
} IdsControlDctClkGateCtl;

///DCT Clk Gate Control
///
typedef enum {
  IdsControlDctClkGateEnMin = 0x0,///<Min
  IdsControlDctClkGateEnMax = 0x3,///<Max
} IdsControlDctClkGateEn;

///Slew Rate Control
///
typedef enum {
  IdsNvMemCtrllerSlewRateCtlManual = 0,///<Manual
  IdsNvMemCtrllerSlewRateCtlAuto = 1,///<Auto
} IdsNvMemCtrllerSlewRateCtl;

///Common IDS Control Option
typedef enum {
  IdsControlDisAutoRefresh0 = 0,///<0
  IdsControlDisAutoRefresh1 = 1,///<1
  IdsControlDisAutoRefreshAuto = 3,///<3
} IdsNvMemCtrllerDisAutoRefresh;

///Common IDS Control Option
typedef enum {
  IdsControlAuto = 0,///<0
  IdsControlManual = 1,///<1
} IdsNvMemCtrllerControl;

///Control command throttle mode
///Specifies command throttle mode. D18F2xA4[14:12]
#define IdsNvMemCtrllerCmdThrottleModeMin 0
#define IdsNvMemCtrllerCmdThrottleModeMax 7

///Control command throttle mode Control
typedef enum {
  IdsControlCmdThrottleModeCtlAuto = 0,///<0
  IdsControlCmdThrottleModeCtlManual = 1,///<1
} IdsNvMemCtrllerCmdThrottleModeCtl;

///Common IDS 1BIT Option
typedef enum {
  IdsOptionDisabled = 0,///<Disable
  IdsOptionEnabled = 1,///<Enable
  IdsOptionAuto = 3,///<Auto
} IdsNvMemCtrllerDisAutoRefresh;

///DRAM scrubber redirect enable. D18F3x5C[0]
typedef enum {
  IdsNvMemCtrllerEccScrubRedirectDis = 0,              ///< Disabled
  IdsNvMemCtrllerEccScrubRedirectEn = 1,              ///< Disabled
  IdsNvMemCtrllerEccScrubRedirectAuto = 3,              ///< Auto
} IdsNvMemCtrllerEccScrubRedirect;

///PMOS predriver calibration code
typedef enum {
  IdsNvMemCtrllerTxPreP0x0 = 0x0,///<0x0
  IdsNvMemCtrllerTxPreP0x9 = 0x9,///<0x9
  IdsNvMemCtrllerTxPreP0x12 = 0x12,///<0x12
  IdsNvMemCtrllerTxPreP0x1B = 0x1B,///<0x1B
  IdsNvMemCtrllerTxPreP0x24 = 0x24,///<0x24
  IdsNvMemCtrllerTxPreP0x2D = 0x2D,///<0x2D
  IdsNvMemCtrllerTxPreP0x36 = 0x36,///<0x36
  IdsNvMemCtrllerTxPreP0x3F = 0x3F,///<0x3F
} IdsNvMemCtrllerTxPreP;

///NMOS predriver calibration code
///
typedef enum {
  IdsNvMemCtrllerTxPreN0x0 = 0x0,///<0x0
  IdsNvMemCtrllerTxPreN0x9 = 0x9,///<0x9
  IdsNvMemCtrllerTxPreN0x12 = 0x12,///<0x12
  IdsNvMemCtrllerTxPreN0x1B = 0x1B,///<0x1B
  IdsNvMemCtrllerTxPreN0x24 = 0x24,///<0x24
  IdsNvMemCtrllerTxPreN0x2D = 0x2D,///<0x2D
  IdsNvMemCtrllerTxPreN0x36 = 0x36,///<0x36
  IdsNvMemCtrllerTxPreN0x3F = 0x3F,///<0x3F
} IdsNvMemCtrllerTxPreN;

///Drive Strength User Controls
///Set the mode for Drive Strength initialization to Auto or Manual
typedef enum {
  IdsNvMemDrvCtlAuto = 0,///<Auto
  IdsNvMemDrvCtlManual = 1,///<Manual
} IdsNvMemDrvCtl;

///CKE Drive Strength
///Set the CKE drive strength
typedef enum {
  IdsNvMemDrvCKEDrStr1_0x = 0,///<1.0x
  IdsNvMemDrvCKEDrStr1_25x = 1,///<1.25x
  IdsNvMemDrvCKEDrStr1_5x = 2,///<1.5x
  IdsNvMemDrvCKEDrStr2_0x = 3,///<2.0x
  IdsNvMemDrvCKEDrStrDisabled = 4,///<Disabled
  IdsNvMemDrvCKEDrStr0_25x = 5,///<0.25x
  IdsNvMemDrvCKEDrStr0_5x = 6,///<0.5x
  IdsNvMemDrvCKEDrStrAuto = 0xFF,///<Auto
} IdsNvMemDrvCKEDrStr;

///CS/ODT Drive Strength
///Set the CS/ODT drive strength
typedef enum {
  IdsNvMemDrvCODrStr1_0x = 0,///<1.0x
  IdsNvMemDrvCODrStr1_25x = 1,///<1.25x
  IdsNvMemDrvCODrStr1_5x = 2,///<1.5x
  IdsNvMemDrvCODrStr2_0x = 3,///<2.0x
  IdsNvMemDrvCODrStrDisabled = 4,///<Disabled
  IdsNvMemDrvCODrStr0_25x = 5,///<0.25x
  IdsNvMemDrvCODrStr0_5x = 6,///<0.5x
  IdsNvMemDrvCODrStrAuto = 0xFF,///<Auto
} IdsNvMemDrvCODrStr;

///Addr/Cmd Drive Strength
///Set the Addr/Cmd drive strength
typedef enum {
  IdsNvMemDrvACDrStr1_0x = 0,///<1.0x
  IdsNvMemDrvACDrStr1_25x = 1,///<1.25x
  IdsNvMemDrvACDrStr1_5x = 2,///<1.5x
  IdsNvMemDrvACDrStr2_0x = 3,///<2.0x
  IdsNvMemDrvACDrStrDisabled = 4,///<Disabled
  IdsNvMemDrvACDrStr0_25x = 5,///<0.25x
  IdsNvMemDrvACDrStr0_5x = 6,///<0.5x
  IdsNvMemDrvACDrStrAuto = 0xFF,///<Auto
} IdsNvMemDrvACDrStr;

///Clock Drive Strength
///Set the Clock drive strength
typedef enum {
  IdsNvMemDrvClkDrStr1_0x = 0,///<1.0x
  IdsNvMemDrvClkDrStr1_25x = 1,///<1.25x
  IdsNvMemDrvClkDrStr1_5x = 2,///<1.5x
  IdsNvMemDrvClkDrStr2_0x = 3,///<2.0x
  IdsNvMemDrvClkDrStrDisabled = 4,///<Disabled
  IdsNvMemDrvClkDrStr0_25x = 5,///<0.25x
  IdsNvMemDrvClkDrStr0_5x = 6,///<0.5x
  IdsNvMemDrvClkDrStrAuto = 0xFF,///<Auto
} IdsNvMemDrvClkDrStr;

///Data Drive Strength
///Set the data drive strength
typedef enum {
  IdsNvMemDrvDataDrStr0_75x = 0,///<0.75x
  IdsNvMemDrvDataDrStr1_0x = 1,///<1.0x
  IdsNvMemDrvDataDrStr1_25x = 2,///<1.25x
  IdsNvMemDrvDataDrStr1_5x = 3,///<1.5x
  IdsNvMemDrvDataDrStrDisabled = 4,///<Disabled
  IdsNvMemDrvDataDrStr0_25x = 5,///<0.25x
  IdsNvMemDrvDataDrStr0_5x = 6,///<0.5x
  IdsNvMemDrvDataDrStrAuto = 0xFF,///<Auto
} IdsNvMemDrvDataDrStr;

///ECC DRAM Scrub
///Sets the rate of DRAM. D18F3x58
typedef enum {
  IdsNvMemCtrllerEccDramScrub_Disable = 0,           ///< Disabled
  IdsNvMemCtrllerEccDramScrub_40ns = 1,              ///< 40ns
  IdsNvMemCtrllerEccDramScrub_80ns = 2,              ///< 80ns
  IdsNvMemCtrllerEccDramScrub_160ns = 3,             ///< 160ns
  IdsNvMemCtrllerEccDramScrub_320ns = 4,             ///< 320ns
  IdsNvMemCtrllerEccDramScrub_640ns = 5,             ///< 640ns
  IdsNvMemCtrllerEccDramScrub_1_28us = 6,            ///< 1.28us
  IdsNvMemCtrllerEccDramScrub_2_56us = 7,            ///< 2.56us
  IdsNvMemCtrllerEccDramScrub_5_12us = 8,            ///< 5.12us
  IdsNvMemCtrllerEccDramScrub_10_2us = 9,            ///< 10.2us
  IdsNvMemCtrllerEccDramScrub_20_5us = 0xA,          ///< 20.5us
  IdsNvMemCtrllerEccDramScrub_41_0us = 0xB,          ///< 41.0us
  IdsNvMemCtrllerEccDramScrub_81_9us = 0xC,          ///< 81.9us
  IdsNvMemCtrllerEccDramScrub_163_8us = 0xD,         ///< 163.8us
  IdsNvMemCtrllerEccDramScrub_327_7us = 0xE,         ///< 327.7us
  IdsNvMemCtrllerEccDramScrub_655_4us = 0xF,         ///< 655.4us
  IdsNvMemCtrllerEccDramScrub_1_31ms = 0x10,         ///< 1.31ms
  IdsNvMemCtrllerEccDramScrub_2_62ms = 0x11,         ///< 2.62ms
  IdsNvMemCtrllerEccDramScrub_5_24ms = 0x12,         ///< 5.24ms
  IdsNvMemCtrllerEccDramScrub_10_49ms = 0x13,        ///< 10.49ms
  IdsNvMemCtrllerEccDramScrub_20_97ms = 0x14,        ///< 20.97ms
  IdsNvMemCtrllerEccDramScrub_42ms = 0x15,           ///< 42ms
  IdsNvMemCtrllerEccDramScrub_84ms = 0x16,           ///< 84ms
  IdsNvMemCtrllerEccDramScrub_20ns = 0x1F,           ///< 20ns
  IdsNvMemCtrllerEccDramScrubAuto = 0xFF,            ///< Auto
} IdsNvMemCtrllerEccDramScrub;

///DQS Drive Strength
///Set the DQS drive strength
typedef enum {
  IdsNvMemDrvDQSDrStr0_75x = 0,///<0.75x
  IdsNvMemDrvDQSDrStr1_0x = 1,///<1.0x
  IdsNvMemDrvDQSDrStr1_25x = 2,///<1.25x
  IdsNvMemDrvDQSDrStr1_5x = 3,///<1.5x
  IdsNvMemDrvDQSDrStrDisabled = 4,///<Disabled
  IdsNvMemDrvDQSDrStr0_25x = 5,///<0.25x
  IdsNvMemDrvDQSDrStr0_5x = 6,///<0.5x
  IdsNvMemDrvDQSDrStrAuto = 0xFF,///<Auto
} IdsNvMemDrvDQSDrStr;

///Proc ODT
///Set the DQS drive strength
typedef enum {
  IdsNvMemDrvProcODT240_ohm = 0,///<240 ohm
  IdsNvMemDrvProcODT120_ohm = 1,///<120 ohm
  IdsNvMemDrvProcODT80_ohm = 2,///<80 ohm
  IdsNvMemDrvProcODT60_ohm = 3,///<60 ohm
  IdsNvMemDrvProcODTAuto = 0xFF,///<Auto
} IdsNvMemDrvProcODT;

///Low power drive strength enable
///D18F2x9C_x0D0F_C0[40,00]_dct[1:0][8]
typedef enum {
  IdsNvMemDrvLowPwrDrvStrengthEnDisabled = 0,///<Disabled
  IdsNvMemDrvLowPwrDrvStrengthEnEnabled = 1,///<Enabled
  IdsNvMemDrvLowPwrDrvStrengthEnAuto = 3,///<Auto
} IdsNvMemDrvLowPwrDrvStrengthEn;

///ProcOdt advance
///D18F2x9C_x0D0F_0[F,7:0][53,13]_dct[1:0][14]
typedef enum {
  IdsNvMemDrvProcOdtAdvDisabled = 0,///<Disabled
  IdsNvMemDrvProcOdtAdvEnabled = 1,///<Enabled
  IdsNvMemDrvProcOdtAdvAuto = 3,///<Auto
} IdsNvMemDrvProcOdtAdv;

///DQS Timing User Controls
///Set the mode for DQS training configuration to Auto or Manual
typedef enum {
  IdsNvMemDQSCtlAuto = 0,///<Auto
  IdsNvMemDQSCtlManual = 1,///<Manual
} IdsNvMemDQSCtl;

///Write Data Byte 0 Control
///
typedef enum {
  IdsNvMemDQSWrByte0CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte0CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte0Ctl;

///Write Data Byte 0 Fine Delay
///Set how much delay is added to byte 0 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl0Min 0
#define IdsNvMemDQSWrByteFineDl0Max 31

///Write Data Byte 0 Gross Delay
///Set how much delay is added to byte 3 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg0No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg00_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg01_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg01_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg02_0_MemClk_Delay = 4,///<2.0 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg0;

///Write Data Byte 1 Control
///
typedef enum {
  IdsNvMemDQSWrByte1CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte1CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte1Ctl;

///Write Data Byte 1 Fine Delay
///Set how much delay is added to byte 1 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl1Min 0
#define IdsNvMemDQSWrByteFineDl1Max 31

///Write Data Byte 1 Gross Delay
///Set how much delay is added to byte 1 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg1No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg10_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg11_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg11_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg12_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg12_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg1;

///Write Data Byte 2 Control
///
typedef enum {
  IdsNvMemDQSWrByte2CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte2CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte2Ctl;

///Write Data Byte 2 Fine Delay
///Set how much delay is added to byte 2 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl2Min 0
#define IdsNvMemDQSWrByteFineDl2Max 31

///Write Data Byte 2 Gross Delay
///Set how much delay is added to byte 2 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg2No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg20_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg21_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg21_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg22_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg22_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg2;

///Write Data Byte 3 Control
///
typedef enum {
  IdsNvMemDQSWrByte3CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte3CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte3Ctl;

///Write Data Byte 3 Fine Delay
///Set how much delay is added to byte 3 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl3Min 0
#define IdsNvMemDQSWrByteFineDl3Max 31

///Write Data Byte 3 Gross Delay
///Set how much delay is added to byte 2 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg3No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg30_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg31_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg31_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg32_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg32_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg3;

///Write Data Byte 4 Control
///
typedef enum {
  IdsNvMemDQSWrByte4CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte4CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte4Ctl;

///Write Data Byte 4 Fine Delay
///Set how much delay is added to byte 4 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl4Min 0
#define IdsNvMemDQSWrByteFineDl4Max 31

///Write Data Byte 4 Gross Delay
///Set how much delay is added to byte 4 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg4No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg40_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg41_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg41_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg42_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg42_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg4;

///Write Data Byte 5 Control
///
typedef enum {
  IdsNvMemDQSWrByte5CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte5CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte5Ctl;

///Write Data Byte 5 Fine Delay
///Set how much delay is added to byte 5 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl5Min 0
#define IdsNvMemDQSWrByteFineDl5Max 31

///Write Data Byte 5 Gross Delay
///Set how much delay is added to byte 5 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDgl5No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDgl50_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDgl51_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDgl51_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDgl52_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDgl52_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDgl5;

///Write Data Byte 6 Control
///
typedef enum {
  IdsNvMemDQSWrByte6CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte6CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte6Ctl;

///Write Data Byte 6 Fine Delay
///Set how much delay is added to byte 6 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl6Min 0
#define IdsNvMemDQSWrByteFineDl6Max 31

///Write Data Byte 6 Gross Delay
///Set how much delay is added to byte 6 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg6No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg60_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg61_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg61_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg62_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg62_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg6;

///Write Data Byte 7 Control
///
typedef enum {
  IdsNvMemDQSWrByte7CtlAuto = 0,///<Auto
  IdsNvMemDQSWrByte7CtlManual = 1,///<Manual
} IdsNvMemDQSWrByte7Ctl;

///Write Data Byte 7 Fine Delay
///Set how much delay is added to byte 7 of the data with respect to DQS
#define IdsNvMemDQSWrByteFineDl7Min 0
#define IdsNvMemDQSWrByteFineDl7Max 31

///Write Data Byte 7 Gross Delay
///Set how much delay is added to byte 7 of the data with respect to DQS
typedef enum {
  IdsNvMemDQSWrByteGrossDlg7No_Delay = 0,///<No Delay
  IdsNvMemDQSWrByteGrossDlg70_5_MemClk_Delay = 1,///<0.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg71_0_MemClk_Delay = 2,///<1.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg71_5_MemClk_Delay = 3,///<1.5 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg72_0_MemClk_Delay = 4,///<2.0 MemClk Delay
  IdsNvMemDQSWrByteGrossDlg72_5_MemClk_Delay = 5,///<2.5 MemClk Delay
} IdsNvMemDQSWrByteGrossDlg7;

///Read DQS Byte 0 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt0CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt0CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt0Ctl;

///Read DQS Byte 0 Delay
///Set how much delay is added to the DQS associated with byte 0 with respect to the data
#define IdsNvMemDQSRdDQSBtDl0Min 0
#define IdsNvMemDQSRdDQSBtDl0Max 31

///Read DQS Byte 1 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt1CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt1CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt1Ctl;

///Read DQS Byte 1 Delay
///Set how much delay is added to the DQS associated with byte 1 with respect to the data
#define IdsNvMemDQSRdDQSBtDl1Min 0
#define IdsNvMemDQSRdDQSBtDl1Max 31

///Read DQS Byte 2 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt2CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt2CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt2Ctl;

///Read DQS Byte 2 Delay
///Set how much delay is added to the DQS associated with byte 2 with respect to the data
#define IdsNvMemDQSRdDQSBtDl2Min 0
#define IdsNvMemDQSRdDQSBtDl2Max 31

///Read DQS Byte 3 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt3CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt3CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt3Ctl;

///Read DQS Byte 3 Delay
///Set how much delay is added to the DQS associated with byte 3 with respect to the data
#define IdsNvMemDQSRdDQSBtDl3Min 0
#define IdsNvMemDQSRdDQSBtDl3Max 31

///Read DQS Byte 4 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt4CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt4CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt4Ctl;

///Read DQS Byte 4 Delay
///Set how much delay is added to the DQS associated with byte 4 with respect to the data
#define IdsNvMemDQSRdDQSBtDl4Min 0
#define IdsNvMemDQSRdDQSBtDl4Max 31

///Read DQS Byte 5 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt5CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt5CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt5Ctl;

///Read DQS Byte 5 Delay
///Set how much delay is added to the DQS associated with byte 5 with respect to the data
#define IdsNvMemDQSRdDQSBtDl5Min 0
#define IdsNvMemDQSRdDQSBtDl5Max 31

///Read DQS Byte 6 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt6CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt6CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt6Ctl;

///Read DQS Byte 6 Delay
///Set how much delay is added to the DQS associated with byte 6 with respect to the data
#define IdsNvMemDQSRdDQSBtDl6Min 0
#define IdsNvMemDQSRdDQSBtDl6Max 31

///Read DQS Byte 7 Control
///
typedef enum {
  IdsNvMemDQSRdDQSBt7CtlAuto = 0,///<Auto
  IdsNvMemDQSRdDQSBt7CtlManual = 1,///<Manual
} IdsNvMemDQSRdDQSBt7Ctl;

///Read DQS Byte 7 Delay
///Set how much delay is added to the DQS associated with byte 7 with respect to the data
#define IdsNvMemDQSRdDQSBtDl7Min 0
#define IdsNvMemDQSRdDQSBtDl7Max 31

///CKE Fine Delay Control
///
typedef enum {
  IdsNvMemDQSCkeFineDelayCtlAuto = 0,///<Auto
  IdsNvMemDQSCkeFineDelayCtlManual = 1,///<Manual
} IdsNvMemDQSCkeFineDelayCtl;

///CKE Fine Delay
///Set how long the CKE pins are delayed from the default setup time
#define IdsNvMemDQSCKEFnDlMin 0
#define IdsNvMemDQSCKEFnDlMax 31

///CKE Setup Time
///This bit selects the default setup time for the CKE pins versus MEMCLK
typedef enum {
  IdsNvMemDQSCKESetupTime1_2_Clk = 0,///<1/2 Clk
  IdsNvMemDQSCKESetupTime1_Clk = 1,///<1 Clk
  IdsNvMemDQSCKESetupTimeAuto = 3,///<Auto
} IdsNvMemDQSCKESetupTime;

///CS/ODT Fine Delay Control
///
typedef enum {
  IdsNvMemDQSCsOdtFineDelayCtlAuto = 0,///<Auto
  IdsNvMemDQSCsOdtFineDelayCtlManual = 1,///<Manual
} IdsNvMemDQSCsOdtFineDelayCtl;

///CS/ODT Fine Delay
///Set how long the CS/ODT pins are delayed from the default setup time
#define IdsNvMemDQSCOFnDlMin 0
#define IdsNvMemDQSCOFnDlMax 31

///CS/ODT Setup Time
///This bit selects the default setup time for the CS/ODT pins versus MEMCLK
typedef enum {
  IdsNvMemDQSCOSetupTime1_2_Clk = 0,///<1/2 Clk
  IdsNvMemDQSCOSetupTime1_Clk = 1,///<1 Clk
  IdsNvMemDQSCOSetupTimeAuto = 3,///<Auto
} IdsNvMemDQSCOSetupTime;

///Addr/Cmd Fine Delay Control
///
typedef enum {
  IdsNvMemDQSAddrCmdFineDelayCtlAuto = 0,///<Auto
  IdsNvMemDQSAddrCmdFineDelayCtlManual = 1,///<Manual
} IdsNvMemDQSAddrCmdFineDelayCtl;

///Addr/Cmd Fine Delay
///Set how long the Addr/Cmd pins are delayed from the default setup time
#define IdsNvMemDQSAddrCmdFnDlMin 0
#define IdsNvMemDQSAddrCmdFnDlMax 31

///Addr/Cmd Setup Time
///This bit selects the default setup time for the address and command pins versus MEMCLK
typedef enum {
  IdsNvMemDQSAddrCmdSetupTime1_2_Clk = 0,///<1/2 Clk
  IdsNvMemDQSAddrCmdSetupTime1_Clk = 1,///<1 Clk
  IdsNvMemDQSAddrCmdSetupTimeAuto = 3,///<Auto
} IdsNvMemDQSAddrCmdSetupTime;

///Write ODT on duration control
///Specifies the number of memory clock cycles that ODT is asserted for writes.
typedef enum {
  IdsNvMemDQSWriODTDurCtlDisabled = 0,///<Disabled
  IdsNvMemDQSWriODTDurCtlManual = 1,///<Manual
  IdsNvMemDQSWriODTDurCtlAuto = 3,///<Auto
} IdsNvMemDQSWriODTDurCtl;

///Write ODT on duration
///D18F2x240_dct[1:0]_mp[1:0][14:12]
#define IdsNvMemDQSWriODTDurMin 0
#define IdsNvMemDQSWriODTDurMax 7

///Write ODT Turn on Delay control
///Specifies the number of memory clock cycles that ODT assertion is delayed relative to write CAS
typedef enum {
  IdsNvMemDQSWriODTTurnOnDlyCtlDisabled = 0,///<Disabled
  IdsNvMemDQSWriODTTurnOnDlyCtlManual = 1,///<Manual
  IdsNvMemDQSWriODTTurnOnDlyCtlAuto = 3,///<Auto
} IdsNvMemDQSWriODTTurnOnDlyCtl;

///Write ODT Turn on Delay
///D18F2x240_dct[1:0]_mp[1:0][10:8]
#define IdsNvMemDQSWriODTTurnOnDlyMin 0
#define IdsNvMemDQSWriODTTurnOnDlyMax 7

///Read ODT On Duration Control
///Specifies the number of memory clock cycles that ODT is asserted for read
typedef enum {
  IdsNvMemDQSReadODTDurCtlDisabled = 0,///<Disabled
  IdsNvMemDQSReadODTDurCtlManual = 1,///<Manual
  IdsNvMemDQSReadODTDurCtlAuto = 3,///<Auto
} IdsNvMemDQSReadODTDurCtl;

///Read ODT On Duration
///D18F2x240_dct[1:0]_mp[1:0][6:4]
#define IdsNvMemDQSReadODTDurMin 0
#define IdsNvMemDQSReadODTDurMax 9

///Read ODT Turn On Delay Control
///Specifies the number of clock cycles that ODT assertion is delayed relative to read CAS.
typedef enum {
  IdsNvMemDQSReadODTTurnDlyCtlManual = 1,///<Manual
  IdsNvMemDQSReadODTTurnDlyCtlDisabled = 0,///<Disabled
  IdsNvMemDQSReadODTTurnDlyCtlAuto = 3,///<Auto
} IdsNvMemDQSReadODTTurnDlyCtl;

///Read ODT Turn On Delay
///D18F2x240_dct[1:0]_mp[3:0]
#define IdsNvMemDQSReadODTTurnDlyMin 0
#define IdsNvMemDQSReadODTTurnDlyMax 0xF

///Desired Load Card Slots
///Desired Load Card Slots
typedef enum {
  IdsNvMemDiagLoadCardModeDisabled = 0,///<Disabled
  IdsNvMemDiagLoadCardModeChannel_A___Unganged = 1,///<Channel A - Unganged
  IdsNvMemDiagLoadCardModeNear_slot___Ganged = 2,///<Near slot - Ganged
  IdsNvMemDiagLoadCardModeOther_configurations = 3,///<Other configurations
} IdsNvMemDiagLoadCardMode;

///Swap in Load Cards
///Swap in Load Cards
typedef enum {
  IdsNvMemDiagLoadCardActiveNo = 0,///<No
  IdsNvMemDiagLoadCardActiveYes = 1,///<Yes
} IdsNvMemDiagLoadCardActive;

///Error Recovery Mode
///Error Recovery Mode
typedef enum {
  IdsNvMemDiagErrorRecoveryModeDisabled = 0,///<Disabled
  IdsNvMemDiagErrorRecoveryModeEnabled = 1,///<Enabled
} IdsNvMemDiagErrorRecoveryMode;

///Times to Retrain
///Times to Retrain
typedef enum {
  IdsNvMemDiagRetrainTimes0 = 0,///<0
  IdsNvMemDiagRetrainTimes1 = 1,///<1
  IdsNvMemDiagRetrainTimes2 = 2,///<2
  IdsNvMemDiagRetrainTimes3 = 3,///<3
} IdsNvMemDiagRetrainTimes;

///Ignore Memory Errors
///Ignore Memory Errors
typedef enum {
  IdsNvMemDiagIgnoreErrDisabled = 0,///<Disabled
  IdsNvMemDiagIgnoreErrEnabled = 1,///<Enabled
} IdsNvMemDiagIgnoreErr;

///GRA Group 1
///Enable or disable GRA Group 1
typedef enum {
  IdsNvMiscGRA1UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA1UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA1UsrCtl;

///GRA1 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA1RegTypePCI = 0,///<PCI
  IdsNvMiscGRA1RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA1RegType;

///GRA1 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA1TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA1TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA1TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA1TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA1TimePoint;

///GRA1 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA1GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA1GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA1GenRegNode;

///GRA1 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA1BusMin 0
#define IdsNvMiscGRA1BusMax 255

///GRA1 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA1DevMin 0
#define IdsNvMiscGRA1DevMax 31

///GRA1 Function
///This is to select the function of register to set
#define IdsNvMiscGRA1FunMin 0
#define IdsNvMiscGRA1FunMax 7

///GRA1 Offset
///Offset
#define IdsNvMiscGRA1OffsetMin 0
#define IdsNvMiscGRA1OffsetMax 0xFFF

///GRA1 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA1CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA1CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA1Cores;

///GRA1 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA1NodeMin 0
#define IdsNvMiscGRA1NodeMax 255

///GRA1 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA1CoreMin 0
#define IdsNvMiscGRA1CoreMax 127

///GRA1 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA1MsrHighMin 0
#define IdsNvMiscGRA1MsrHighMax 0xFFFF

///GRA1 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA1MsrLowMin 0
#define IdsNvMiscGRA1MsrLowMax 0xFFFF

///GRA1 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA1AndHighestMin 0
#define IdsNvMiscGRA1AndHighestMax 0xFFFF

///GRA1 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA1AndHigherMin 0
#define IdsNvMiscGRA1AndHigherMax 0xFFFF

///GRA1 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA1AndLowerMin 0
#define IdsNvMiscGRA1AndLowerMax 0xFFFF

///GRA1 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA1AndLowestMin 0
#define IdsNvMiscGRA1AndLowestMax 0xFFFF

///GRA1 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA1OrHighestMin 0
#define IdsNvMiscGRA1OrHighestMax 0xFFFF

///GRA1 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA1OrHigherMin 0
#define IdsNvMiscGRA1OrHigherMax 0xFFFF

///GRA1 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA1OrLowerMin 0
#define IdsNvMiscGRA1OrLowerMax 0xFFFF

///GRA1 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA1OrLowestMin 0
#define IdsNvMiscGRA1OrLowestMax 0xFFFF

///GRA Group 2
///Enable or disable GRA Group 2
typedef enum {
  IdsNvMiscGRA2UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA2UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA2UsrCtl;

///GRA2 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA2RegTypePCI = 0,///<PCI
  IdsNvMiscGRA2RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA2RegType;

///GRA2 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA2TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA2TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA2TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA2TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA2TimePoint;

///GRA2 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA2GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA2GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA2GenRegNode;

///GRA2 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA2BusMin 0
#define IdsNvMiscGRA2BusMax 255

///GRA2 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA2DevMin 0
#define IdsNvMiscGRA2DevMax 31

///GRA2 Function
///This is to select the function of register to set
#define IdsNvMiscGRA2FunMin 0
#define IdsNvMiscGRA2FunMax 7

///GRA2 Offset
///Offset
#define IdsNvMiscGRA2OffsetMin 0
#define IdsNvMiscGRA2OffsetMax 0xFFF

///GRA2 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA2CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA2CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA2Cores;

///GRA2 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA2NodeMin 0
#define IdsNvMiscGRA2NodeMax 255

///GRA2 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA2CoreMin 0
#define IdsNvMiscGRA2CoreMax 127

///GRA2 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA2MsrHighMin 0
#define IdsNvMiscGRA2MsrHighMax 0xFFFF

///GRA2 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA2MsrLowMin 0
#define IdsNvMiscGRA2MsrLowMax 0xFFFF

///GRA2 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA2AndHighestMin 0
#define IdsNvMiscGRA2AndHighestMax 0xFFFF

///GRA2 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA2AndHigherMin 0
#define IdsNvMiscGRA2AndHigherMax 0xFFFF

///GRA2 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA2AndLowerMin 0
#define IdsNvMiscGRA2AndLowerMax 0xFFFF

///GRA2 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA2AndLowestMin 0
#define IdsNvMiscGRA2AndLowestMax 0xFFFF

///GRA2 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA2OrHighestMin 0
#define IdsNvMiscGRA2OrHighestMax 0xFFFF

///GRA2 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA2OrHigherMin 0
#define IdsNvMiscGRA2OrHigherMax 0xFFFF

///GRA2 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA2OrLowerMin 0
#define IdsNvMiscGRA2OrLowerMax 0xFFFF

///GRA2 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA2OrLowestMin 0
#define IdsNvMiscGRA2OrLowestMax 0xFFFF

///GRA Group 3
///Enable or disable GRA Group 3
typedef enum {
  IdsNvMiscGRA3UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA3UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA3UsrCtl;

///GRA3 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA3RegTypePCI = 0,///<PCI
  IdsNvMiscGRA3RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA3RegType;

///GRA3 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA3TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA3TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA3TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA3TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA3TimePoint;

///GRA3 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA3GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA3GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA3GenRegNode;

///GRA3 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA3BusMin 0
#define IdsNvMiscGRA3BusMax 255

///GRA3 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA3DevMin 0
#define IdsNvMiscGRA3DevMax 31

///GRA3 Function
///This is to select the function of register to set
#define IdsNvMiscGRA3FunMin 0
#define IdsNvMiscGRA3FunMax 7

///GRA3 Offset
///Offset
#define IdsNvMiscGRA3OffsetMin 0
#define IdsNvMiscGRA3OffsetMax 0xFFF

///GRA3 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA3CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA3CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA3Cores;

///GRA3 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA3NodeMin 0
#define IdsNvMiscGRA3NodeMax 255

///GRA3 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA3CoreMin 0
#define IdsNvMiscGRA3CoreMax 127

///GRA3 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA3MsrHighMin 0
#define IdsNvMiscGRA3MsrHighMax 0xFFFF

///GRA3 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA3MsrLowMin 0
#define IdsNvMiscGRA3MsrLowMax 0xFFFF

///GRA3 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA3AndHighestMin 0
#define IdsNvMiscGRA3AndHighestMax 0xFFFF

///GRA3 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA3AndHigherMin 0
#define IdsNvMiscGRA3AndHigherMax 0xFFFF

///GRA3 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA3AndLowerMin 0
#define IdsNvMiscGRA3AndLowerMax 0xFFFF

///GRA3 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA3AndLowestMin 0
#define IdsNvMiscGRA3AndLowestMax 0xFFFF

///GRA3 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA3OrHighestMin 0
#define IdsNvMiscGRA3OrHighestMax 0xFFFF

///GRA3 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA3OrHigherMin 0
#define IdsNvMiscGRA3OrHigherMax 0xFFFF

///GRA3 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA3OrLowerMin 0
#define IdsNvMiscGRA3OrLowerMax 0xFFFF

///GRA3 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA3OrLowestMin 0
#define IdsNvMiscGRA3OrLowestMax 0xFFFF

///GRA Group 4
///Enable or disable GRA Group 4
typedef enum {
  IdsNvMiscGRA4UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA4UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA4UsrCtl;

///GRA4 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA4RegTypePCI = 0,///<PCI
  IdsNvMiscGRA4RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA4RegType;

///GRA4 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA4TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA4TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA4TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA4TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA4TimePoint;

///GRA4 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA4GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA4GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA4GenRegNode;

///GRA4 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA4BusMin 0
#define IdsNvMiscGRA4BusMax 255

///GRA4 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA4DevMin 0
#define IdsNvMiscGRA4DevMax 31

///GRA4 Function
///This is to select the function of register to set
#define IdsNvMiscGRA4FunMin 0
#define IdsNvMiscGRA4FunMax 7

///GRA4 Offset
///Offset
#define IdsNvMiscGRA4OffsetMin 0
#define IdsNvMiscGRA4OffsetMax 0xFFF

///GRA4 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA4CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA4CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA4Cores;

///GRA4 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA4NodeMin 0
#define IdsNvMiscGRA4NodeMax 255

///GRA4 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA4CoreMin 0
#define IdsNvMiscGRA4CoreMax 127

///GRA4 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA4MsrHighMin 0
#define IdsNvMiscGRA4MsrHighMax 0xFFFF

///GRA4 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA4MsrLowMin 0
#define IdsNvMiscGRA4MsrLowMax 0xFFFF

///GRA4 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA4AndHighestMin 0
#define IdsNvMiscGRA4AndHighestMax 0xFFFF

///GRA4 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA4AndHigherMin 0
#define IdsNvMiscGRA4AndHigherMax 0xFFFF

///GRA4 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA4AndLowerMin 0
#define IdsNvMiscGRA4AndLowerMax 0xFFFF

///GRA4 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA4AndLowestMin 0
#define IdsNvMiscGRA4AndLowestMax 0xFFFF

///GRA4 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA4OrHighestMin 0
#define IdsNvMiscGRA4OrHighestMax 0xFFFF

///GRA4 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA4OrHigherMin 0
#define IdsNvMiscGRA4OrHigherMax 0xFFFF

///GRA4 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA4OrLowerMin 0
#define IdsNvMiscGRA4OrLowerMax 0xFFFF

///GRA4 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA4OrLowestMin 0
#define IdsNvMiscGRA4OrLowestMax 0xFFFF

///GRA Group 5
///Enable or disable GRA Group 5
typedef enum {
  IdsNvMiscGRA5UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA5UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA5UsrCtl;

///GRA5 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA5RegTypePCI = 0,///<PCI
  IdsNvMiscGRA5RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA5RegType;

///GRA5 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA5TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA5TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA5TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA5TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA5TimePoint;

///GRA5 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA5GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA5GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA5GenRegNode;

///GRA5 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA5BusMin 0
#define IdsNvMiscGRA5BusMax 255

///GRA5 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA5DevMin 0
#define IdsNvMiscGRA5DevMax 31

///GRA5 Function
///This is to select the function of register to set
#define IdsNvMiscGRA5FunMin 0
#define IdsNvMiscGRA5FunMax 7

///GRA5 Offset
///Offset
#define IdsNvMiscGRA5OffsetMin 0
#define IdsNvMiscGRA5OffsetMax 0xFFF

///GRA5 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA5CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA5CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA5Cores;

///GRA5 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA5NodeMin 0
#define IdsNvMiscGRA5NodeMax 255

///GRA5 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA5CoreMin 0
#define IdsNvMiscGRA5CoreMax 127

///GRA5 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA5MsrHighMin 0
#define IdsNvMiscGRA5MsrHighMax 0xFFFF

///GRA5 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA5MsrLowMin 0
#define IdsNvMiscGRA5MsrLowMax 0xFFFF

///GRA5 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA5AndHighestMin 0
#define IdsNvMiscGRA5AndHighestMax 0xFFFF

///GRA5 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA5AndHigherMin 0
#define IdsNvMiscGRA5AndHigherMax 0xFFFF

///GRA5 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA5AndLowerMin 0
#define IdsNvMiscGRA5AndLowerMax 0xFFFF

///GRA5 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA5AndLowestMin 0
#define IdsNvMiscGRA5AndLowestMax 0xFFFF

///GRA5 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA5OrHighestMin 0
#define IdsNvMiscGRA5OrHighestMax 0xFFFF

///GRA5 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA5OrHigherMin 0
#define IdsNvMiscGRA5OrHigherMax 0xFFFF

///GRA5 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA5OrLowerMin 0
#define IdsNvMiscGRA5OrLowerMax 0xFFFF

///GRA5 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA5OrLowestMin 0
#define IdsNvMiscGRA5OrLowestMax 0xFFFF

///GRA Group 6
///Enable or disable GRA Group 6
typedef enum {
  IdsNvMiscGRA6UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA6UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA6UsrCtl;

///GRA6 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA6RegTypePCI = 0,///<PCI
  IdsNvMiscGRA6RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA6RegType;

///GRA6 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA6TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA6TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA6TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA6TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA6TimePoint;

///GRA6 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA6GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA6GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA6GenRegNode;

///GRA6 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA6BusMin 0
#define IdsNvMiscGRA6BusMax 255

///GRA6 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA6DevMin 0
#define IdsNvMiscGRA6DevMax 31

///GRA6 Function
///This is to select the function of register to set
#define IdsNvMiscGRA6FunMin 0
#define IdsNvMiscGRA6FunMax 7

///GRA6 Offset
///Offset
#define IdsNvMiscGRA6OffsetMin 0
#define IdsNvMiscGRA6OffsetMax 0xFFF

///GRA6 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA6CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA6CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA6Cores;

///GRA6 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA6NodeMin 0
#define IdsNvMiscGRA6NodeMax 255

///GRA6 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA6CoreMin 0
#define IdsNvMiscGRA6CoreMax 127

///GRA6 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA6MsrHighMin 0
#define IdsNvMiscGRA6MsrHighMax 0xFFFF

///GRA6 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA6MsrLowMin 0
#define IdsNvMiscGRA6MsrLowMax 0xFFFF

///GRA6 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA6AndHighestMin 0
#define IdsNvMiscGRA6AndHighestMax 0xFFFF

///GRA6 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA6AndHigherMin 0
#define IdsNvMiscGRA6AndHigherMax 0xFFFF

///GRA6 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA6AndLowerMin 0
#define IdsNvMiscGRA6AndLowerMax 0xFFFF

///GRA6 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA6AndLowestMin 0
#define IdsNvMiscGRA6AndLowestMax 0xFFFF

///GRA6 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA6OrHighestMin 0
#define IdsNvMiscGRA6OrHighestMax 0xFFFF

///GRA6 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA6OrHigherMin 0
#define IdsNvMiscGRA6OrHigherMax 0xFFFF

///GRA6 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA6OrLowerMin 0
#define IdsNvMiscGRA6OrLowerMax 0xFFFF

///GRA6 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA6OrLowestMin 0
#define IdsNvMiscGRA6OrLowestMax 0xFFFF

///GRA Group 7
///Enable or disable GRA Group 7
typedef enum {
  IdsNvMiscGRA7UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA7UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA7UsrCtl;

///GRA7 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA7RegTypePCI = 0,///<PCI
  IdsNvMiscGRA7RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA7RegType;

///GRA7 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA7TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA7TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA7TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA7TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA7TimePoint;

///GRA7 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA7GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA7GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA7GenRegNode;

///GRA7 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA7BusMin 0
#define IdsNvMiscGRA7BusMax 255

///GRA7 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA7DevMin 0
#define IdsNvMiscGRA7DevMax 31

///GRA7 Function
///This is to select the function of register to set
#define IdsNvMiscGRA7FunMin 0
#define IdsNvMiscGRA7FunMax 7

///GRA7 Offset
///Offset
#define IdsNvMiscGRA7OffsetMin 0
#define IdsNvMiscGRA7OffsetMax 0xFFF

///GRA7 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA7CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA7CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA7Cores;

///GRA7 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA7NodeMin 0
#define IdsNvMiscGRA7NodeMax 255

///GRA7 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA7CoreMin 0
#define IdsNvMiscGRA7CoreMax 127

///GRA7 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA7MsrHighMin 0
#define IdsNvMiscGRA7MsrHighMax 0xFFFF

///GRA7 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA7MsrLowMin 0
#define IdsNvMiscGRA7MsrLowMax 0xFFFF

///GRA7 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA7AndHighestMin 0
#define IdsNvMiscGRA7AndHighestMax 0xFFFF

///GRA7 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA7AndHigherMin 0
#define IdsNvMiscGRA7AndHigherMax 0xFFFF

///GRA7 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA7AndLowerMin 0
#define IdsNvMiscGRA7AndLowerMax 0xFFFF

///GRA7 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA7AndLowestMin 0
#define IdsNvMiscGRA7AndLowestMax 0xFFFF

///GRA7 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA7OrHighestMin 0
#define IdsNvMiscGRA7OrHighestMax 0xFFFF

///GRA7 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA7OrHigherMin 0
#define IdsNvMiscGRA7OrHigherMax 0xFFFF

///GRA7 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA7OrLowerMin 0
#define IdsNvMiscGRA7OrLowerMax 0xFFFF

///GRA7 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA7OrLowestMin 0
#define IdsNvMiscGRA7OrLowestMax 0xFFFF

///GRA Group 8
///Enable or disable GRA Group 8
typedef enum {
  IdsNvMiscGRA8UsrCtlDisabled = 0,///<Disabled
  IdsNvMiscGRA8UsrCtlEnabled = 1,///<Enabled
} IdsNvMiscGRA8UsrCtl;

///GRA8 Register Type
///Type of register PCI or MSR
typedef enum {
  IdsNvMiscGRA8RegTypePCI = 0,///<PCI
  IdsNvMiscGRA8RegTypeMSR = 1,///<MSR
} IdsNvMiscGRA8RegType;

///GRA8 When to Write
///When to modify the value specified by this option
typedef enum {
  IdsNvMiscGRA8TimePointVery_Early___After_Reset = 0,///<Very Early - After Reset
  IdsNvMiscGRA8TimePointEarly___before_Memory_Init = 1,///<Early - before Memory Init
  IdsNvMiscGRA8TimePointLate___Before_PCI_Init = 2,///<Late - Before PCI Init
  IdsNvMiscGRA8TimePointVery_Late___Before_OS_Boot = 3,///<Very Late - Before OS Boot
} IdsNvMiscGRA8TimePoint;

///GRA8 Gen Reg Node
///Applied to all nodes or a single node
typedef enum {
  IdsNvMiscGRA8GenRegNodeAll_Nodes = 0,///<All Nodes
  IdsNvMiscGRA8GenRegNodeEnter_Bus_Device = 1,///<Enter Bus_Device
} IdsNvMiscGRA8GenRegNode;

///GRA8 Bus
///Decide whether this generic option should be applied to all buses or a single bus
#define IdsNvMiscGRA8BusMin 0
#define IdsNvMiscGRA8BusMax 255

///GRA8 Device
///Applied to all devices or a single device
#define IdsNvMiscGRA8DevMin 0
#define IdsNvMiscGRA8DevMax 31

///GRA8 Function
///This is to select the function of register to set
#define IdsNvMiscGRA8FunMin 0
#define IdsNvMiscGRA8FunMax 7

///GRA8 Offset
///Offset
#define IdsNvMiscGRA8OffsetMin 0
#define IdsNvMiscGRA8OffsetMax 0xFFF

///GRA8 Gen Reg Core
///Decide whether this generic option should be applied to all cores or a single core
typedef enum {
  IdsNvMiscGRA8CoresAll_Cores = 0,///<All Cores
  IdsNvMiscGRA8CoresSpecify_Node_Core = 1,///<Specify Node_Core
} IdsNvMiscGRA8Cores;

///GRA8 Node
///Decide whether this generic option should be applied to specify node
#define IdsNvMiscGRA8NodeMin 0
#define IdsNvMiscGRA8NodeMax 255

///GRA8 Core
///Decide whether this generic option should be applied to specify core
#define IdsNvMiscGRA8CoreMin 0
#define IdsNvMiscGRA8CoreMax 127

///GRA8 MSR Address High
///This is to input the MSR address of register to set [63:32]
#define IdsNvMiscGRA8MsrHighMin 0
#define IdsNvMiscGRA8MsrHighMax 0xFFFF

///GRA8 MSR Address Low
///This is to input the MSR address of register to set [31:0]
#define IdsNvMiscGRA8MsrLowMin 0
#define IdsNvMiscGRA8MsrLowMax 0xFFFF

///GRA8 Mask Value [63:48]
///This is to input the mask value of register to setMask Value [63:48]
#define IdsNvMiscGRA8AndHighestMin 0
#define IdsNvMiscGRA8AndHighestMax 0xFFFF

///GRA8 Mask Value [47:32]
///This is to input the mask value of register to set [47:32]
#define IdsNvMiscGRA8AndHigherMin 0
#define IdsNvMiscGRA8AndHigherMax 0xFFFF

///GRA8 Mask Value [31:16]
///This is to input the mask value of register to set [31:16]
#define IdsNvMiscGRA8AndLowerMin 0
#define IdsNvMiscGRA8AndLowerMax 0xFFFF

///GRA8 Mask Value [15:0]
///This is to input the mask value of register to set [15:0]
#define IdsNvMiscGRA8AndLowestMin 0
#define IdsNvMiscGRA8AndLowestMax 0xFFFF

///GRA8 Or Value [63:48]
///This is to input the Or value of register to set [63:48]
#define IdsNvMiscGRA8OrHighestMin 0
#define IdsNvMiscGRA8OrHighestMax 0xFFFF

///GRA8 Or Value [47:32]
///This is to input the Or value of register to set [47:32]
#define IdsNvMiscGRA8OrHigherMin 0
#define IdsNvMiscGRA8OrHigherMax 0xFFFF

///GRA8 Or Value [31:16]
///This is to input the Or value of register to set [31:16]
#define IdsNvMiscGRA8OrLowerMin 0
#define IdsNvMiscGRA8OrLowerMax 0xFFFF

///GRA8 Or Value [15:0]
///This is to input the Or value of register to set [15:0]
#define IdsNvMiscGRA8OrLowestMin 0
#define IdsNvMiscGRA8OrLowestMax 0xFFFF

///PCIE Posted Pass NP
///PCIE posted pass non-posted (PCIE reordering rule)
typedef enum {
  IdsNvFchGppPcieOrderRuleDisabled = 0,///<Disabled
  IdsNvFchGppPcieOrderRuleHudson_1 = 1,///<Hudson-1
  IdsNvFchGppPcieOrderRuleHudson_2 = 2,///<Hudson-2
} IdsNvFchGppPcieOrderRule;

///GPP MEM WR IMPROVE
///Select whether or not Enable GPP MEM WR IMPROVE
typedef enum {
  IdsNvFchGppMemWrImproveDisabled = 0,///<Disabled
  IdsNvFchGppMemWrImproveEnabled = 1,///<Enabled
} IdsNvFchGppMemWrImprove;

///UMI Dynamic Speed Change
///Enable or disable UMI dynamic speed change
typedef enum {
  IdsNvFchGppUmiDynamicSpeedChangeDisabled = 0,///<Disabled
  IdsNvFchGppUmiDynamicSpeedChangeEnabled = 1,///<Enabled
} IdsNvFchGppUmiDynamicSpeedChange;

///UMI ASPM L1 Timer Auto Override
///Enable or disable UMI ASPM L1 timer auto override
typedef enum {
  IdsNvFchGppUmiL1TimerOverrideDisabled = 0,///<Disabled
  IdsNvFchGppUmiL1TimerOverride1us = 1,///<1us
  IdsNvFchGppUmiL1TimerOverride2us = 2,///<2us
  IdsNvFchGppUmiL1TimerOverride4us = 3,///<4us
  IdsNvFchGppUmiL1TimerOverride10us = 4,///<10us
  IdsNvFchGppUmiL1TimerOverride20us = 5,///<20us
  IdsNvFchGppUmiL1TimerOverride40us = 6,///<40us
  IdsNvFchGppUmiL1TimerOverride100us = 7,///<100us
} IdsNvFchGppUmiL1TimerOverride;

///UMI GPP TX Drive Strength
///Enable or disable UMI GPP TX drive strength
typedef enum {
  IdsNvFchGppUmiGppTxDriverStrengthDisabled = 0,///<Disabled
  IdsNvFchGppUmiGppTxDriverStrength18mA = 1,///<18mA
  IdsNvFchGppUmiGppTxDriverStrength20mA = 2,///<20mA
  IdsNvFchGppUmiGppTxDriverStrength22mA = 3,///<22mA
  IdsNvFchGppUmiGppTxDriverStrength24mA = 4,///<24mA
} IdsNvFchGppUmiGppTxDriverStrength;

///AB Clock Gating
///Enable or disable AB clock gating
typedef enum {
  IdsNvFchGppAbClockGatingDisabled = 0,///<Disabled
  IdsNvFchGppAbClockGatingEnabled = 1,///<Enabled
} IdsNvFchGppAbClockGating;

///UMI/GPP Clock Gating
///Enable or disable UMI/GPP clock gating
typedef enum {
  IdsNvFchGppClockGatingDisabled = 1,///<Disabled
  IdsNvFchGppClockGatingEnabled = 0,///<Enabled
} IdsNvFchGppClockGating;

///Toggle SB GPP Reset
///Enable or disable toggling of SB GPP PCIE reset
typedef enum {
  IdsNvFchGppToggleResetDisabled = 0,///<Disabled
  IdsNvFchGppToggleResetEnabled = 1,///<Enabled
} IdsNvFchGppToggleReset;

///UMI PHY PLL Power Down
///Select whether or not Power Down the UMI link  PHY PLL
typedef enum {
  IdsNvFchGppUmiPhyPllPowerDownDisabled = 0,///<Disabled
  IdsNvFchGppUmiPhyPllPowerDownEnabled = 1,///<Enabled
} IdsNvFchGppUmiPhyPllPowerDown;

///GPP Dynamic Power Saving
///Select whether or not Gpp Dynamic Power Saving
typedef enum {
  IdsNvFchGppDynamicPowerSavingEnabled = 1,///<Enabled
  IdsNvFchGppDynamicPowerSavingDisabled = 0,///<Disabled
} IdsNvFchGppDynamicPowerSaving;

///GPP Function
///Enable or disable the SB GPP Function
typedef enum {
  IdsNvFchGppEnableDisabled = 0,///<Disabled
  IdsNvFchGppEnableEnabled = 1,///<Enabled
} IdsNvFchGppEnable;

///GPP Port Link Configuration
///Select the GPP Port Link Configuration
typedef enum {
  IdsNvFchGppLinkConfigX4_______mode = 0,///<X4       mode
  IdsNvFchGppLinkConfigReserved = 1,///<Reserved
  IdsNvFchGppLinkConfig2_2_______mode = 2,///<2:2       mode
  IdsNvFchGppLinkConfig2_1_1____mode = 3,///<2:1:1    mode
  IdsNvFchGppLinkConfig1_1_1_1_mode = 4,///<1:1:1:1 mode
} IdsNvFchGppLinkConfig;

///Unhide unused GPP ports
///Select whether or not unhide the unused GPP ports
typedef enum {
  IdsNvFchGppUnhidePortsDisabled = 0,///<Disabled
  IdsNvFchGppUnhidePortsEnabled = 1,///<Enabled
} IdsNvFchGppUnhidePorts;

///GPP Hotplug
///Select Enable or disable SB GPP hotplug attribute
typedef enum {
  IdsNvFchGppPortHotplugDisabled = 0,///<Disabled
  IdsNvFchGppPortHotplugEnabled = 1,///<Enabled
} IdsNvFchGppPortHotplug;

///GPP Link ASPM
///Select GPP Link ASPM
typedef enum {
  IdsNvFchGppPortAspmDisabled = 0,///<Disabled
  IdsNvFchGppPortAspmL0s = 1,///<L0s
  IdsNvFchGppPortAspmL1 = 2,///<L1
  IdsNvFchGppPortAspmL0s___L1 = 3,///<L0s + L1
} IdsNvFchGppPortAspm;

///GPP GEN2
///Enable or disable GPP Gen2
typedef enum {
  IdsNvFchGppGen2Disabled = 0,///<Disabled
  IdsNvFchGppGen2Enabled = 1,///<Enabled
} IdsNvFchGppGen2;

///GPP Hardware Compliance Mode
///Select GPP hardware compliance mode
typedef enum {
  IdsNvFchGppHardwareDownGradeDisabled = 0,///<Disabled
  IdsNvFchGppHardwareDownGradePort_A = 1,///<Port A
  IdsNvFchGppHardwareDownGradePort_B = 2,///<Port B
  IdsNvFchGppHardwareDownGradePort_C = 3,///<Port C
  IdsNvFchGppHardwareDownGradePort_D = 4,///<Port D
} IdsNvFchGppHardwareDownGrade;

///GPP Lane Reversal
///Select whether or not Reverse the GPP Lane
typedef enum {
  IdsNvFchGppLaneReversalDisabled = 0,///<Disabled
  IdsNvFchGppLaneReversalEnabled = 1,///<Enabled
} IdsNvFchGppLaneReversal;

///GPP PHY PLL Power Down
///
typedef enum {
  IdsNvFchGppPhyPllPowerDownDisabled = 0,///<Disabled
  IdsNvFchGppPhyPllPowerDownEnabled = 1,///<Enabled
} IdsNvFchGppPhyPllPowerDown;

///Advanced Error Reporting
///Enable or disable PCIE AER support
typedef enum {
  IdsNvFchGppPcieAerDisabled = 0,///<Disabled
  IdsNvFchGppPcieAerEnabled = 1,///<Enabled
} IdsNvFchGppPcieAer;

///RAS Function
///Enable or disable PCIE RAS support
typedef enum {
  IdsNvFchGppPcieRasDisabled = 0,///<Disabled
  IdsNvFchGppPcieRasEnabled = 1,///<Enabled
} IdsNvFchGppPcieRas;

///In-Chip NIC
///Select Disable or Enable the In-Chip NIC
typedef enum {
  IdsNvFchGecEnableEnabled = 0,///<Enabled
  IdsNvFchGecEnableDisabled = 1,///<Disabled
} IdsNvFchGecEnable;

///GEC Phy Status
///Select GEC PHY mode
typedef enum {
  IdsNvFchGecPhyStatusGb_PHY_mode = 0,///<Gb PHY mode
  IdsNvFchGecPhyStatus100_10_PHY_mode = 1,///<100/10 PHY mode
} IdsNvFchGecPhyStatus;

///FCH GEC Powered down Policy
///Select FCH GEC Powered down Policy
typedef enum {
  IdsNvFchGecPowerPolicyS3_and_S5 = 0,///<S3 and S5
  IdsNvFchGecPowerPolicyS5 = 1,///<S5
  IdsNvFchGecPowerPolicyS3 = 2,///<S3
  IdsNvFchGecPowerPolicyNever_Powered_down = 3,///<Never Powered down
} IdsNvFchGecPowerPolicy;

///GEC Debug Bus
///Select whether or not Enable GEC Debug Bus
typedef enum {
  IdsNvFchGecDebugBusDisabled = 0,///<Disabled
  IdsNvFchGecDebugBusEnabled = 1,///<Enabled
} IdsNvFchGecDebugBus;

///SATA 6 AHCI Support
///Enable/disable support of SATA 6-AHCI port support
typedef enum {
  IdsNvFchSata6AhciCapDisabled = 0,///<Disabled
  IdsNvFchSata6AhciCapEnabled = 1,///<Enabled
} IdsNvFchSata6AhciCap;

///Internal Clock Differential
///Internal clock differential 100MHz spread
typedef enum {
  IdsNvFchSataInternal100SpreadDisabled = 0,///<Disabled
  IdsNvFchSataInternal100SpreadEnabled = 1,///<Enabled
} IdsNvFchSataInternal100Spread;

///SATA Set MaxGen2 Capability
///Select whether or not Enable SATA MaxGen2 Capability
typedef enum {
  IdsNvFchSataSetMaxGen2Enabled = 0,///<Enabled
  IdsNvFchSataSetMaxGen2Disabled = 1,///<Disabled
} IdsNvFchSataSetMaxGen2;

///SATA PHY Ref Clock
///Select SATA PHY Ref Clock
typedef enum {
  IdsNvFchSataClkModeExt_25MHz = 0,///<Ext 25MHz
  IdsNvFchSataClkModeInt_25MHz = 1,///<Int 25MHz
  IdsNvFchSataClkModeInt_100MHz = 9,///<Int 100MHz
  IdsNvFchSataClkModeExt_100MHz = 10,///<Ext 100MHz
} IdsNvFchSataClkMode;

///Aggressive Link PM Capability
///Select Enable or Disable Aggressive Link PM Capability
typedef enum {
  IdsNvFchSataAggrLinkPmCapDisabled = 0,///<Disabled
  IdsNvFchSataAggrLinkPmCapEnabled = 1,///<Enabled
} IdsNvFchSataAggrLinkPmCap;

///Port Multiplier Capability
///Select Enable or Disable Port Multiplier Capability
typedef enum {
  IdsNvFchSataPortMultCapDisabled = 0,///<Disabled
  IdsNvFchSataPortMultCapEnabled = 1,///<Enabled
} IdsNvFchSataPortMultCap;

///Partial State Capability
///Select Enable or Disable Partial State Capability
typedef enum {
  IdsNvFchSataPscCapDisabled = 0,///<Disabled
  IdsNvFchSataPscCapEnabled = 1,///<Enabled
} IdsNvFchSataPscCap;

///Slumber State Capability
///Select Enable or Disable Slumber State Capability
typedef enum {
  IdsNvFchSataSscCapDisabled = 0,///<Disabled
  IdsNvFchSataSscCapEnabled = 1,///<Enabled
} IdsNvFchSataSscCap;

///RAID Type
///Select Raid Type
typedef enum {
  IdsNvFchSataForceRaidRAID = 0,///<RAID
  IdsNvFchSataForceRaidRAID5 = 1,///<RAID5
} IdsNvFchSataForceRaid;

///SATA Ports Hot Plug Capability
///Select Enable or Disable SATA Ports Hot Plug Capability
typedef enum {
  IdsNvFchSataHotPlugCapDisabled = 0,///<Disabled
  IdsNvFchSataHotPlugCapEnabled = 1,///<Enabled
} IdsNvFchSataHotPlugCap;

///Unused SATA Port Auto Shut Down
///Select Unused SATA Port Auto Shut Down or not
typedef enum {
  IdsNvFchSataClkAutoOffDisabled = 0,///<Disabled
  IdsNvFchSataClkAutoOffEnabled = 1,///<Enabled
} IdsNvFchSataClkAutoOff;

///Options for FIS-based Switching Support
///Select Enable or Disable FIS-based Switching Support
typedef enum {
  IdsNvFchSataFisBasedSwitchingDisabled = 0,///<Disabled
  IdsNvFchSataFisBasedSwitchingEnabled = 1,///<Enabled
} IdsNvFchSataFisBasedSwitching;

///Command Completion Coalescing Support
///Select Enable or Disable Command Completion Coalescing Support
typedef enum {
  IdsNvFchSataCccSuppportDisabled = 0,///<Disabled
  IdsNvFchSataCccSuppportEnabled = 1,///<Enabled
} IdsNvFchSataCccSuppport;

///SATA MSI Capability
///Select Enable or Disable SATA MSI Capability
typedef enum {
  IdsNvFchSataMsiCapabilityDisabled = 0,///<Disabled
  IdsNvFchSataMsiCapabilityEnabled = 1,///<Enabled
} IdsNvFchSataMsiCapability;

///SATA Target Support 8 Devices
///Enable/disable SATA 8-device target support
typedef enum {
  IdsNvFchSataTargetSupport8DeviceDisabled = 0,///<Disabled
  IdsNvFchSataTargetSupport8DeviceEnabled = 1,///<Enabled
} IdsNvFchSataTargetSupport8Device;

///Generic Mode
///Enable/disable SATA generic mode
typedef enum {
  IdsNvFchSataDisableGenericModeEnabled = 0,///<Enabled
  IdsNvFchSataDisableGenericModeDisabled = 1,///<Disabled
} IdsNvFchSataDisableGenericMode;

///SATA AHCI Enclosure
///Enable/disable SATA AHCI enclosure management
typedef enum {
  IdsNvFchSataAhciEnclosureManagementDisabled = 0,///<Disabled
  IdsNvFchSataAhciEnclosureManagementEnabled = 1,///<Enabled
} IdsNvFchSataAhciEnclosureManagement;

///SATA SGPIO0
///Enable/disable SATA Sgpio0
typedef enum {
  IdsNvFchSataSgpio0Disabled = 0,///<Disabled
  IdsNvFchSataSgpio0Enabled = 1,///<Enabled
} IdsNvFchSataSgpio0;

///SATA SGPIO1
///Enable/disable SATA Sgpio1
typedef enum {
  IdsNvFchSataSgpio1Disabled = 0,///<Disabled
  IdsNvFchSataSgpio1Enabled = 1,///<Enabled
} IdsNvFchSataSgpio1;

///SATA PHY PLL
///Enable/disable SATA PHY PLL shutdown
typedef enum {
  IdsNvFchSataPhyPllShutDownDisabled = 0,///<Disabled
  IdsNvFchSataPhyPllShutDownEnabled = 1,///<Enabled
} IdsNvFchSataPhyPllShutDown;

///SATA Pri. Ch.
///Enable/disable Sata Primary Channel
typedef enum {
  IdsNvFchSataDisUnusedIdePChannelEnabled = 0,///<Enabled
  IdsNvFchSataDisUnusedIdePChannelDisabled = 1,///<Disabled
} IdsNvFchSataDisUnusedIdePChannel;

///SATA Sec. Ch.
///Enable/disable Sata Secondary Channel
typedef enum {
  IdsNvFchSataDisUnusedIdeSChannelEnabled = 0,///<Enabled
  IdsNvFchSataDisUnusedIdeSChannelDisabled = 1,///<Disabled
} IdsNvFchSataDisUnusedIdeSChannel;

///IDE Pri. Ch.
///Enable/disable IDE Primary Channel
typedef enum {
  IdsNvFchIdeDisUnusedIdePChannelEnabled = 0,///<Enabled
  IdsNvFchIdeDisUnusedIdePChannelDisabled = 1,///<Disabled
} IdsNvFchIdeDisUnusedIdePChannel;

///IDE Sec. Ch.
///Enable/disable Ide Secondary Channel
typedef enum {
  IdsNvFchIdeDisUnusedIdeSChannelEnabled = 0,///<Enabled
  IdsNvFchIdeDisUnusedIdeSChannelDisabled = 1,///<Disabled
} IdsNvFchIdeDisUnusedIdeSChannel;

///HRE Port0
///SATA Hot-Removal Enhancement function for port 0
typedef enum {
  IdsNvFchSataHotRemovalEnhPort0Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort0Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort0;

///HRE Port1
///SATA Hot-Removal Enhancement function for port 1
typedef enum {
  IdsNvFchSataHotRemovalEnhPort1Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort1Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort1;

///HRE Port2
///SATA Hot-Removal Enhancement function for port 2
typedef enum {
  IdsNvFchSataHotRemovalEnhPort2Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort2Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort2;

///HRE Port3
///SATA Hot-Removal Enhancement function for port 3
typedef enum {
  IdsNvFchSataHotRemovalEnhPort3Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort3Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort3;

///HRE Port4
///SATA Hot-Removal Enhancement function for port 4
typedef enum {
  IdsNvFchSataHotRemovalEnhPort4Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort4Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort4;

///HRE Port5
///SATA Hot-Removal Enhancement function for port 5
typedef enum {
  IdsNvFchSataHotRemovalEnhPort5Enabled = 1,///<Enabled
  IdsNvFchSataHotRemovalEnhPort5Disabled = 0,///<Disabled
} IdsNvFchSataHotRemovalEnhPort5;

///HRE Port6
///SATA Hot-Removal Enhancement function for port 6
typedef enum {
  IdsNvFchSataHotRemovalEnhPort6Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort6Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort6;

///HRE Port7
///SATA Hot-Removal Enhancement function for port 7
typedef enum {
  IdsNvFchSataHotRemovalEnhPort7Disabled = 0,///<Disabled
  IdsNvFchSataHotRemovalEnhPort7Enabled = 1,///<Enabled
} IdsNvFchSataHotRemovalEnhPort7;

///Sata mode for Port0
///SATA port mode for port 0
typedef enum {
  IdsNvFchSataPortMdPort0Auto = 0,///<Auto
  IdsNvFchSataPortMdPort0Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort0Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort0;

///Sata mode for Port1
///SATA port mode for port 1
typedef enum {
  IdsNvFchSataPortMdPort1Auto = 0,///<Auto
  IdsNvFchSataPortMdPort1Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort1Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort1;

///Sata mode for Port2
///SATA port mode for port 2
typedef enum {
  IdsNvFchSataPortMdPort2Auto = 0,///<Auto
  IdsNvFchSataPortMdPort2Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort2Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort2;

///Sata mode for Port3
///SATA port mode for port 3
typedef enum {
  IdsNvFchSataPortMdPort3Auto = 0,///<Auto
  IdsNvFchSataPortMdPort3Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort3Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort3;

///Sata mode for Port4
///SATA port mode for port 4
typedef enum {
  IdsNvFchSataPortMdPort4Auto = 0,///<Auto
  IdsNvFchSataPortMdPort4Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort4Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort4;

///Sata mode for Port5
///SATA port mode for port 5
typedef enum {
  IdsNvFchSataPortMdPort5Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort5Auto = 0,///<Auto
  IdsNvFchSataPortMdPort5Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort5;

///Sata mode for Port6
///SATA port mode for port 6
typedef enum {
  IdsNvFchSataPortMdPort6Auto = 0,///<Auto
  IdsNvFchSataPortMdPort6Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort6Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort6;

///Sata mode for Port7
///SATA port mode for port 7
typedef enum {
  IdsNvFchSataPortMdPort7Auto = 0,///<Auto
  IdsNvFchSataPortMdPort7Gen1 = 1,///<Gen1
  IdsNvFchSataPortMdPort7Gen2 = 2,///<Gen2
} IdsNvFchSataPortMdPort7;

///AC/DC Message
///Select AC/DC Change message delivery
typedef enum {
  IdsNvFchApuAcDcMsgDisabled = 0,///<Disabled
  IdsNvFchApuAcDcMsgEnabled = 1,///<Enabled
} IdsNvFchApuAcDcMsg;

///Timer Tick Tracking
///Enable/disable timer tick tracking
typedef enum {
  IdsNvFchApuTimerTickTrackDisabled = 0,///<Disabled
  IdsNvFchApuTimerTickTrackEnabled = 1,///<Enabled
} IdsNvFchApuTimerTickTrack;

///Clock Interrupt Tag
///Enable/disable tag on clock interrupt
typedef enum {
  IdsNvFchApuClockInterruptTagDisabled = 0,///<Disabled
  IdsNvFchApuClockInterruptTagEnabled = 1,///<Enabled
} IdsNvFchApuClockInterruptTag;

///OHCI Traffic
///Enable/disable OHCI traffic handling
typedef enum {
  IdsNvFchApuOhciTrafficHandingDisabled = 0,///<Disabled
  IdsNvFchApuOhciTrafficHandingEnabled = 1,///<Enabled
} IdsNvFchApuOhciTrafficHanding;

///EHCI Traffic
///Enable/disable EHCI traffic handling
typedef enum {
  IdsNvFchApuEhciTrafficHandingDisabled = 0,///<Disabled
  IdsNvFchApuEhciTrafficHandingEnabled = 1,///<Enabled
} IdsNvFchApuEhciTrafficHanding;

///Apu MC Support
///Enable/disable Apu multi-core support
typedef enum {
  IdsNvFchApuMsgCMultiCoreDisabled = 0,///<Disabled
  IdsNvFchApuMsgCMultiCoreEnabled = 1,///<Enabled
} IdsNvFchApuMsgCMultiCore;

///Apu MSG C State
///Enable/disable Apu message C state
typedef enum {
  IdsNvFchApuMsgCStageDisabled = 0,///<Disabled
  IdsNvFchApuMsgCStageEnabled = 1,///<Enabled
} IdsNvFchApuMsgCStage;

///SPI Read Mode
///Select SPI read mode
typedef enum {
  IdsNvFchSpiModeQual_IO_1_4_4 = 5,///<Qual IO 1-4-4
  IdsNvFchSpiModeQual_IO_1_2_2 = 4,///<Qual IO 1-2-2
  IdsNvFchSpiModeQual_IO_1_1_4 = 3,///<Qual IO 1-1-4
  IdsNvFchSpiModeQual_IO_1_1_2 = 2,///<Qual IO 1-1-2
  IdsNvFchSpiModeFast_Read = 7,///<Fast Read
  IdsNvFchSpiModeDefault = 0,///<Default
  IdsNvFchSpiModeNormal = 6,///<Normal
} IdsNvFchSpiMode;

///SPI Normal Speed
///Select SPI Normal speed
typedef enum {
  IdsNvFchSpiSpeedDefault = 0,///<Default
  IdsNvFchSpiSpeed66MHz = 1,///<66MHz
  IdsNvFchSpiSpeed33MHz = 2,///<33MHz
  IdsNvFchSpiSpeed22MHz = 3,///<22MHz
  IdsNvFchSpiSpeed16_5MHz = 4,///<16.5MHz
} IdsNvFchSpiSpeed;

///SPI Fast Read Speed
///Select SPI fast read speed
typedef enum {
  IdsNvFchSpiFastSpeedDefault = 0,///<Default
  IdsNvFchSpiFastSpeed66MHz = 1,///<66MHz
  IdsNvFchSpiFastSpeed33MHz = 2,///<33MHz
  IdsNvFchSpiFastSpeed22MHz = 3,///<22MHz
  IdsNvFchSpiFastSpeed16_5MHz = 4,///<16.5MHz
} IdsNvFchSpiFastSpeed;

///SPI Burst Write
///Select to enable or disable SPI Burst Write
typedef enum {
  IdsNvFchSpiBurstWriteDisabled = 0,///<Disabled
  IdsNvFchSpiBurstWriteEnabled = 1,///<Enabled
} IdsNvFchSpiBurstWrite;

///SB-TSI Auto Poll
///
typedef enum {
  IdsNvFchHwmSbtsiAutoPollDisabled = 0,///<Disabled
  IdsNvFchHwmSbtsiAutoPollEnabled = 1,///<Enabled
} IdsNvFchHwmSbtsiAutoPoll;

///Sample Rate of Fan Speed
///
typedef enum {
  IdsNvFchHwmFanSampleFreqDiv_1 = 0,///</1
  IdsNvFchHwmFanSampleFreqDiv_2 = 1,///</2
  IdsNvFchHwmFanSampleFreqDiv_4 = 2,///</4
  IdsNvFchHwmFanSampleFreqDiv_8 = 3,///</8
} IdsNvFchHwmFanSampleFreqDiv;

///Fan0-FanInputControl
///
#define IdsNvFchHwmFan0InputControlReg00Min 0
#define IdsNvFchHwmFan0InputControlReg00Max 7

///Fan0-FanControl
///
#define IdsNvFchHwmFan0ControlReg01Min 0
#define IdsNvFchHwmFan0ControlReg01Max 0xff

///Fan0-FanFreq
///
#define IdsNvFchHwmFan0FreqReg02Min 0
#define IdsNvFchHwmFan0FreqReg02Max 0xff

///Fan0-LowDuty
///
#define IdsNvFchHwmFan0LowDutyReg03Min 0
#define IdsNvFchHwmFan0LowDutyReg03Max 0x64

///Fan0-MedDuty
///
#define IdsNvFchHwmFan0MedDutyReg04Min 0
#define IdsNvFchHwmFan0MedDutyReg04Max 0x64

///Fan0-LowTempThreshold
///
#define IdsNvFchHwmFan0LowTempReg06Min 0
#define IdsNvFchHwmFan0LowTempReg06Max 0x80

///Fan0-MedTempThreshold
///
#define IdsNvFchHwmFan0MedTempReg08Min 0
#define IdsNvFchHwmFan0MedTempReg08Max 0xff

///Fan0-HighTempThreshold
///
#define IdsNvFchHwmFan0HighTempReg0AMin 0
#define IdsNvFchHwmFan0HighTempReg0AMax 0x80

///Fan0-LinearRange
///
#define IdsNvFchHwmFan0LinearRangeReg0CMin 0
#define IdsNvFchHwmFan0LinearRangeReg0CMax 0xff

///Fan0-LinearHoldCount
///
#define IdsNvFchHwmFan0LinearHoldCountReg0DMin 0
#define IdsNvFchHwmFan0LinearHoldCountReg0DMax 0xff

///Fan1-FanInputControl
///
#define IdsNvFchHwmFan1InputControlReg00Min 0
#define IdsNvFchHwmFan1InputControlReg00Max 7

///Fan1-FanControl
///
#define IdsNvFchHwmFan1ControlReg01Min 0
#define IdsNvFchHwmFan1ControlReg01Max 0xff

///Fan1-FanFreq
///
#define IdsNvFchHwmFan1FreqReg02Min 0
#define IdsNvFchHwmFan1FreqReg02Max 0xff

///Fan1-LowDuty
///
#define IdsNvFchHwmFan1LowDutyReg03Min 0
#define IdsNvFchHwmFan1LowDutyReg03Max 0x64

///Fan1-MedDuty
///
#define IdsNvFchHwmFan1MedDutyReg04Min 0
#define IdsNvFchHwmFan1MedDutyReg04Max 0x64

///Fan1-LowTempThreshold
///
#define IdsNvFchHwmFan1LowTempReg06Min 0
#define IdsNvFchHwmFan1LowTempReg06Max 0x80

///Fan1-MedTempThreshold
///
#define IdsNvFchHwmFan1MedTempReg08Min 0
#define IdsNvFchHwmFan1MedTempReg08Max 0xff

///Fan1-HighTempThreshold
///
#define IdsNvFchHwmFan1HighTempReg0AMin 0
#define IdsNvFchHwmFan1HighTempReg0AMax 0x80

///Fan1-LinearRange
///
#define IdsNvFchHwmFan1LinearRangeReg0CMin 0
#define IdsNvFchHwmFan1LinearRangeReg0CMax 0xff

///Fan1-LinearHoldCount
///
#define IdsNvFchHwmFan1LinearHoldCountReg0DMin 0
#define IdsNvFchHwmFan1LinearHoldCountReg0DMax 0xff

///Fan2-FanInputControl
///
#define IdsNvFchHwmFan2InputControlReg00Min 0
#define IdsNvFchHwmFan2InputControlReg00Max 7

///Fan2-FanControl
///
#define IdsNvFchHwmFan2ControlReg01Min 0
#define IdsNvFchHwmFan2ControlReg01Max 0xff

///Fan2-FanFreq
///
#define IdsNvFchHwmFan2FreqReg02Min 0
#define IdsNvFchHwmFan2FreqReg02Max 0xff

///Fan2-LowDuty
///
#define IdsNvFchHwmFan2LowDutyReg03Min 0
#define IdsNvFchHwmFan2LowDutyReg03Max 0x64

///Fan2-MedDuty
///
#define IdsNvFchHwmFan2MedDutyReg04Min 0
#define IdsNvFchHwmFan2MedDutyReg04Max 0x64

///Fan2-LowTempThreshold
///
#define IdsNvFchHwmFan2LowTempReg06Min 0
#define IdsNvFchHwmFan2LowTempReg06Max 0x80

///Fan2-MedTempThreshold
///
#define IdsNvFchHwmFan2MedTempReg08Min 0
#define IdsNvFchHwmFan2MedTempReg08Max 0xff

///Fan2-HighTempThreshold
///
#define IdsNvFchHwmFan2HighTempReg0AMin 0
#define IdsNvFchHwmFan2HighTempReg0AMax 0x80

///Fan2-LinearRange
///
#define IdsNvFchHwmFan2LinearRangeReg0CMin 0
#define IdsNvFchHwmFan2LinearRangeReg0CMax 0xff

///Fan2-LinearHoldCount
///
#define IdsNvFchHwmFan2LinearHoldCountReg0DMin 0
#define IdsNvFchHwmFan2LinearHoldCountReg0DMax 0xff

///Fan3-FanInputControl
///
#define IdsNvFchHwmFan3InputControlReg00Min 0
#define IdsNvFchHwmFan3InputControlReg00Max 7

///Fan3-FanControl
///
#define IdsNvFchHwmFan3ControlReg01Min 0
#define IdsNvFchHwmFan3ControlReg01Max 0xff

///Fan3-FanFreq
///
#define IdsNvFchHwmFan3FreqReg02Min 0
#define IdsNvFchHwmFan3FreqReg02Max 0xff

///Fan3-LowDuty
///
#define IdsNvFchHwmFan3LowDutyReg03Min 0
#define IdsNvFchHwmFan3LowDutyReg03Max 0x64

///Fan3-MedDuty
///
#define IdsNvFchHwmFan3MedDutyReg04Min 0
#define IdsNvFchHwmFan3MedDutyReg04Max 0x64

///Fan3-LowTempThreshold
///
#define IdsNvFchHwmFan3LowTempReg06Min 0
#define IdsNvFchHwmFan3LowTempReg06Max 0x80

///Fan3-MedTempThreshold
///
#define IdsNvFchHwmFan3MedTempReg08Min 0
#define IdsNvFchHwmFan3MedTempReg08Max 0xff

///Fan3-HighTempThreshold
///
#define IdsNvFchHwmFan3HighTempReg0AMin 0
#define IdsNvFchHwmFan3HighTempReg0AMax 0x80

///Fan3-LinearRange
///
#define IdsNvFchHwmFan3LinearRangeReg0CMin 0
#define IdsNvFchHwmFan3LinearRangeReg0CMax 0xff

///Fan3-LinearHoldCount
///
#define IdsNvFchHwmFan3LinearHoldCountReg0DMin 0
#define IdsNvFchHwmFan3LinearHoldCountReg0DMax 0xff

///Fan4-FanInputControl
///
#define IdsNvFchHwmFan4InputControlReg00Min 0
#define IdsNvFchHwmFan4InputControlReg00Max 7

///Fan4-FanControl
///
#define IdsNvFchHwmFan4ControlReg01Min 0
#define IdsNvFchHwmFan4ControlReg01Max 0xff

///Fan4-FanFreq
///
#define IdsNvFchHwmFan4FreqReg02Min 0
#define IdsNvFchHwmFan4FreqReg02Max 0xff

///Fan4-LowDuty
///
#define IdsNvFchHwmFan4LowDutyReg03Min 0
#define IdsNvFchHwmFan4LowDutyReg03Max 0x64

///Fan4-MedDuty
///
#define IdsNvFchHwmFan4MedDutyReg04Min 0
#define IdsNvFchHwmFan4MedDutyReg04Max 0x64

///Fan4-LowTempThreshold
///
#define IdsNvFchHwmFan4LowTempReg06Min 0
#define IdsNvFchHwmFan4LowTempReg06Max 0x80

///Fan4-MedTempThreshold
///
#define IdsNvFchHwmFan4MedTempReg08Min 0
#define IdsNvFchHwmFan4MedTempReg08Max 0xff

///Fan4-HighTempThreshold
///
#define IdsNvFchHwmFan4HighTempReg0AMin 0
#define IdsNvFchHwmFan4HighTempReg0AMax 0x80

///Fan4-LinearRange
///
#define IdsNvFchHwmFan4LinearRangeReg0CMin 0
#define IdsNvFchHwmFan4LinearRangeReg0CMax 0xff

///Fan4-LinearHoldCount
///
#define IdsNvFchHwmFan4LinearHoldCountReg0DMin 0
#define IdsNvFchHwmFan4LinearHoldCountReg0DMax 0xff

///Speed
///Select SD Speed
typedef enum {
  IdsNvFchSdSpeedLow = 0,///<Low
  IdsNvFchSdSpeedHigh = 1,///<High
} IdsNvFchSdSpeed;

///BitWidth
///Select SD Bit Width
typedef enum {
  IdsNvFchSdBitWidth32Bit = 0,///<32Bit
  IdsNvFchSdBitWidth64Bit = 1,///<64Bit
} IdsNvFchSdBitWidth;

///Azalia Snoop
///Select whether or not Enable the Azalia Snoop feature
typedef enum {
  IdsNvFchAzaliaSnoopDisabled = 0,///<Disabled
  IdsNvFchAzaliaSnoopEnabled = 1,///<Enabled
} IdsNvFchAzaliaSnoop;

///AB MSI Function
///Select whether or not set AB MSI Function
typedef enum {
  IdsNvFchSystemAbMsiEnableDisabled = 0,///<Disabled
  IdsNvFchSystemAbMsiEnableEnabled = 1,///<Enabled
} IdsNvFchSystemAbMsiEnable;

///PCIB MSI Function
///Select whether or not set PCIB MSI Function
typedef enum {
  IdsNvFchSystemPcibMsiEnableDisabled = 0,///<Disabled
  IdsNvFchSystemPcibMsiEnableEnabled = 1,///<Enabled
} IdsNvFchSystemPcibMsiEnable;

///LPC MSI Function
///Select whether or not set USB MSI Function
typedef enum {
  IdsNvFchSystemLpcMsiEnableDisabled = 0,///<Disabled
  IdsNvFchSystemLpcMsiEnableEnabled = 1,///<Enabled
} IdsNvFchSystemLpcMsiEnable;

///Hd Audio MSI Function
///Select whether or not set Hd Audio MSI Function
typedef enum {
  IdsNvFchSystemHdAudioMsiEnableDisabled = 0,///<Disabled
  IdsNvFchSystemHdAudioMsiEnableEnabled = 1,///<Enabled
} IdsNvFchSystemHdAudioMsiEnable;

///Slow A/B Link Clock
///Select whether or not set SlowSpeedABlinkClock
typedef enum {
  IdsNvFchSystemSlowSpeedAblinkClockEnabled = 1,///<Enabled
  IdsNvFchSystemSlowSpeedAblinkClockDisabled = 0,///<Disabled
} IdsNvFchSystemSlowSpeedAblinkClock;

///Usb MSI Function
///Select whether or not set USB MSI Function
typedef enum {
  IdsNvFchSystemUsbMsiEnableEnabled = 1,///<Enabled
  IdsNvFchSystemUsbMsiEnableDisabled = 0,///<Disabled
} IdsNvFchSystemUsbMsiEnable;

///SDB
///Select whether or not set FCH debug function
typedef enum {
  IdsNvFchSystemSerialDebugBusEnableEnabled = 1,///<Enabled
  IdsNvFchSystemSerialDebugBusEnableDisabled = 0,///<Disabled
} IdsNvFchSystemSerialDebugBusEnable;

///A-Link Clock Gate-Off
///Select whether or not set ALinkClkGateOff
typedef enum {
  IdsNvFchSystemALinkClkGateOffDisabled = 0,///<Disabled
  IdsNvFchSystemALinkClkGateOffEnabled = 1,///<Enabled
} IdsNvFchSystemALinkClkGateOff;

///B-Link Clock Gate-Off
///Select whether or not set BLinkClkGateOff
typedef enum {
  IdsNvFchSystemBLinkClkGateOffDisabled = 0,///<Disabled
  IdsNvFchSystemBLinkClkGateOffEnabled = 1,///<Enabled
} IdsNvFchSystemBLinkClkGateOff;

///USB Phy Power Down
///Select whether or not set USB Phy Power Down
typedef enum {
  IdsNvFchSystemUsbPhyPowerDownEnabled = 0,///<Enabled
  IdsNvFchSystemUsbPhyPowerDownDisabled = 1,///<Disabled
} IdsNvFchSystemUsbPhyPowerDown;

///HPET in FCH
///Select whether or not Enable HPET in FCH
typedef enum {
  IdsNvFchSystemHpetEnableDisabled = 0,///<Disabled
  IdsNvFchSystemHpetEnableEnabled = 1,///<Enabled
} IdsNvFchSystemHpetEnable;

///_OSC for PCI0
///Select whether or not Enable _OSC for PCI0
typedef enum {
  IdsNvFchSystemNativePcieSupportDisabled = 0,///<Disabled
  IdsNvFchSystemNativePcieSupportEnabled = 1,///<Enabled
} IdsNvFchSystemNativePcieSupport;

///FCH Spread Spectrum
///Select whether or not Enable the Spread Spectrum Feature
typedef enum {
  IdsNvFchSystemSpreadSpectrumDisabled = 0,///<Disabled
  IdsNvFchSystemSpreadSpectrumEnabled = 1,///<Enabled
} IdsNvFchSystemSpreadSpectrum;

///Enable CG2_PLL Clock
///Enable/disable SB spread spectrum
typedef enum {
  IdsNvFchSystemCg2PllDisabled = 0,///<Disabled
  IdsNvFchSystemCg2PllEnabled = 1,///<Enabled
} IdsNvFchSystemCg2Pll;

///Ac Loss Control
///Select Ac Loss Control Method
typedef enum {
  IdsNvFchSystemPwrFailShadowAlways_Off = 0,///<Always Off
  IdsNvFchSystemPwrFailShadowAlways_On = 1,///<Always On
  IdsNvFchSystemPwrFailShadowReserved = 2,///<Reserved
  IdsNvFchSystemPwrFailShadowPrevious = 3,///<Previous
} IdsNvFchSystemPwrFailShadow;

///Stress Reset Mode
///Select stress reset mode
typedef enum {
  IdsNvFchStressResetModeDisabled = 0,///<Disabled
  IdsNvFchStressResetModeIO_WRITE_64_FE = 1,///<IO_WRITE_64_FE
  IdsNvFchStressResetModeIO_WRITE_CF9_06 = 2,///<IO_WRITE_CF9_06
  IdsNvFchStressResetModeIO_WRITE_CF9_0E = 3,///<IO_WRITE_CF9_0E
} IdsNvFchStressResetMode;

///Primary Graphics Adapter
///Primary Graphics Adapter
typedef enum {
  IdsNvGnbGfxPrimaryGraphicsAdaptoriGfx_dGfx = 0,///<iGfx/dGfx
  IdsNvGnbGfxPrimaryGraphicsAdaptordGfx_iGfx = 1,///<dGfx/iGfx
} IdsNvGnbGfxPrimaryGraphicsAdaptor;

///USWC Steering
///Set USWC Steering
typedef enum {
  IdsNvGnbGfxUSWCSteeringGarlic = 0,///<Garlic
  IdsNvGnbGfxUSWCSteeringOnion = 1,///<Onion
} IdsNvGnbGfxUSWCSteering;

///UMA Steering
///Set UMA Steering
typedef enum {
  IdsNvGnbGfxUMASteeringSnoopedOnion = 0,///< Snooped Onion
  IdsNvGnbGfxUMASteeringSystemTrafficOnion = 1,///<System Traffic Onion
  IdsNvGnbGfxUMASteeringOnion = 2, ///<Onion
  IdsNvGnbGfxUMASteeringGarlic = 3,///<Garlic
  IdsNvGnbGfxUMASteeringAuto = 0xf,///<Auto
} IdsNvGnbGfxUMASteering;

///Down Configure GPU
///Reduce number of SIMD
typedef enum {
  IdsNvGnbGfxDownGPUDisabled = 0,///<Disabled
  IdsNvGnbGfxDownGPUEnabled = 1,///<Enabled
} IdsNvGnbGfxDownGPU;

///SIMD/QP Control
///SIMD/QP Control
typedef enum {
  IdsNvGnbGfxSimdQpCtlAuto = 0,///<Auto
  IdsNvGnbGfxSimdQpCtlManual = 1,///<Manual
} IdsNvGnbGfxSimdQpCtl;

///InactiveSimds
///InactiveSimds
#define IdsNvGnbGfxInactiveSimdMin 0
#define IdsNvGnbGfxInactiveSimdMax 0x1f

///InactiveQdPipes
///InactiveQdPipes
#define IdsNvGnbGfxInactiveQPMin 0
#define IdsNvGnbGfxInactiveQPMax 0xf

///UMA Address Swizzle Control
///UMA address swizzle control
typedef enum {
  IdsNvGnbGfxUMASwizzCtlAuto = 0,///<Auto
  IdsNvGnbGfxUMASwizzCtlManual = 1,///<Manual
} IdsNvGnbGfxUMASwizzCtl;

///UMA Address Swizzle 0 Low
///UMA address swizzle 0 Low
#define IdsNvGnbGfxUMASwizzle0LowMin 0
#define IdsNvGnbGfxUMASwizzle0LowMax 0xffff

///UMA Address Swizzle 0 High
///UMA address swizzle 0 High
#define IdsNvGnbGfxUMASwizzle0HighMin 0
#define IdsNvGnbGfxUMASwizzle0HighMax 0xffff

///UMA Address Swizzle 1 Low
///UMA address swizzle 1 Low
#define IdsNvGnbGfxUMASwizzle1LowMin 0
#define IdsNvGnbGfxUMASwizzle1LowMax 0xffff

///UMA Address Swizzle 1 High
///UMA address swizzle 1 High
#define IdsNvGnbGfxUMASwizzle1HighMin 0
#define IdsNvGnbGfxUMASwizzle1HighMax 0xffff

///GMC Power Gating Data
///GMC Power Gating Data
typedef enum {
  IdsNvGnbGfxGmcPowerGatingDataAuto = 0,///<Auto
  IdsNvGnbGfxGmcPowerGatingDataManual = 1,///<Manual
} IdsNvGnbGfxGmcPowerGatingData;

///GMC Power Gating
///GMC Power Gating
typedef enum {
  IdsNvGnbGfxGmcPowerGatingStutter_Disabled = 0,///<Stutter Disabled
  IdsNvGnbGfxGmcPowerGatingStutter_Only = 1,///<Stutter Only
  IdsNvGnbGfxGmcPowerGatingStutter_and_Powergate = 2,///<Stutter and Powergate
} IdsNvGnbGfxGmcPowerGating;

///Isolation Timer
///Isolation Timer
#define IdsNvGnbGfxGmcIsolationTimerMin 0
#define IdsNvGnbGfxGmcIsolationTimerMax 0xffff

///Reset Timer
///Reset Timer
#define IdsNvGnbGfxGmcResetTimerMin 0
#define IdsNvGnbGfxGmcResetTimerMax 0xffff

///Daughter Timer Powerup
///Daughter Timer Powerup
#define IdsNvGnbGfxGmcDaughterTimerPowerupMin 0
#define IdsNvGnbGfxGmcDaughterTimerPowerupMax 0xffff

///Daughter Timer Powerdown
///Daughter Timer Powerdown
#define IdsNvGnbGfxGmcDaughterTimerPowerdownMin 0
#define IdsNvGnbGfxGmcDaughterTimerPowerdownMax 0xffff

///Mother Timer Powerup
///Mother Timer Powerup
#define IdsNvGnbGfxGmcMotherTimerPowerupMin 0
#define IdsNvGnbGfxGmcMotherTimerPowerupMax 0xffff

///Mother Timer Powerdown
///Mother Timer Powerdown
#define IdsNvGnbGfxGmcMotherTimerPowerdownMin 0
#define IdsNvGnbGfxGmcMotherTimerPowerdownMax 0xffff

///GFX  Idle Voltage Change
///GFX  Idle Voltage mode
typedef enum {
  IdsNvGnbGfxIdleVoltChangeDisabled = 0,///<Disabled
  IdsNvGnbGfxIdleVoltChangeEnabled = 1,///<Enabled
} IdsNvGnbGfxIdleVoltChange;

///DP PLL mode
///Dp PLL mode
typedef enum {
  IdsNvGnbGfxDpPllModecascaded_PLL = 0,///<cascaded PLL
  IdsNvGnbGfxDpPllModeSingle_PLL = 1,///<Single PLL
} IdsNvGnbGfxDpPllMode;

///TMDS PLL mode
///TMDS PLL mode
typedef enum {
  IdsNvGnbGfxTMDSPllmodecascaded_PLL = 0,///<cascaded PLL
  IdsNvGnbGfxTMDSPllmodeSingle_PLL = 1,///<Single PLL
} IdsNvGnbGfxTMDSPllmode;

///TMDS Jitter type
///TMDS Jitter type
typedef enum {
  IdsNvGnbGfxTMDSJittertypeCoherent = 0,///<Coherent
  IdsNvGnbGfxTMDSJittertypeNoncoherent = 1,///<Noncoherent
} IdsNvGnbGfxTMDSJittertype;

///Power Play Table Override
///Override Power play table fuses
typedef enum {
  IdsNvGnbGfxPowerPlayOverrideDisabled = 0,///<Disabled
  IdsNvGnbGfxPowerPlayOverrideSimple = 1,///<Simple
  IdsNvGnbGfxPowerPlayOverrideAdvanced = 2,///<Advanced
} IdsNvGnbGfxPowerPlayOverride;

///ABM Support
///ABM support flag in PP table
typedef enum {
  IdsNvGnbGfxABMSupportDisabled = 0,///<Disabled
  IdsNvGnbGfxABMSupportEnabled = 1,///<Enabled
  IdsNvGnbGfxABMSupportAuto = 0xf,///<Auto
} IdsNvGnbGfxABMSupport;

///LCD backlight control
///LCD backlight control
typedef enum {
  IdsNvGnbGfxLcdBackLightCtlAuto = 0,///<Auto
  IdsNvGnbGfxLcdBackLightCtlManual = 1,///<Manual
} IdsNvGnbGfxLcdBackLightCtl;

///LCD backlight
///LCD backlight
#define IdsNvGnbGfxLcdBackLightCtlMin 0
#define IdsNvGnbGfxLcdBackLightCtlMax 0xffff

///Platform Type
///Platform type for PP
typedef enum {
  IdsNvGnbGfxPlatformTypeMobile = 1,///<Mobile
  IdsNvGnbGfxPlatformTypeDesktop = 2,///<Desktop
  IdsNvGnbGfxPlatformTypeAuto = 0xf,///<Auto
} IdsNvGnbGfxPlatformType;

///Load from Fuses
///Load option values from fuses
typedef enum {
  IdsNvGnbGfxLoadFromFusesNone = 0,///<None
  IdsNvGnbGfxLoadFromFusesLoad = 1,///<Load
} IdsNvGnbGfxLoadFromFuses;

///VID 0
///VID 0
typedef enum {
  IdsNvGnbGfxPpVID01_5500V = 0,///<1.5500V
  IdsNvGnbGfxPpVID01_5375V = 1,///<1.5375V
  IdsNvGnbGfxPpVID01_5250V = 2,///<1.5250V
  IdsNvGnbGfxPpVID01_5125V = 3,///<1.5125V
  IdsNvGnbGfxPpVID00_0125V = 0x7B,///<0.0125V
  IdsNvGnbGfxPpVID0OFF = 0x7C,///<OFF
} IdsNvGnbGfxPpVID0;

///VID 1
///VID 1
typedef enum {
  IdsNvGnbGfxPpVID11_5500V = 0,///<1.5500V
  IdsNvGnbGfxPpVID11_5375V = 1,///<1.5375V
  IdsNvGnbGfxPpVID11_5250V = 2,///<1.5250V
  IdsNvGnbGfxPpVID11_5125V = 3,///<1.5125V
  IdsNvGnbGfxPpVID10_0125V = 0x7B,///<0.0125V
  IdsNvGnbGfxPpVID1OFF = 0x7C,///<OFF
} IdsNvGnbGfxPpVID1;

///VID 2
///VID 2
typedef enum {
  IdsNvGnbGfxPpVID21_5500V = 0,///<1.5500V
  IdsNvGnbGfxPpVID21_5375V = 1,///<1.5375V
  IdsNvGnbGfxPpVID21_5250V = 2,///<1.5250V
  IdsNvGnbGfxPpVID21_5125V = 3,///<1.5125V
  IdsNvGnbGfxPpVID20_0125V = 0x7B,///<0.0125V
  IdsNvGnbGfxPpVID2OFF = 0x7C,///<OFF
} IdsNvGnbGfxPpVID2;

///VID 3
///VID 3
typedef enum {
  IdsNvGnbGfxPpVID31_5500V = 0,///<1.5500V
  IdsNvGnbGfxPpVID31_5375V = 1,///<1.5375V
  IdsNvGnbGfxPpVID31_5250V = 2,///<1.5250V
  IdsNvGnbGfxPpVID31_5125V = 3,///<1.5125V
  IdsNvGnbGfxPpVID30_0125V = 0x7B,///<0.0125V
  IdsNvGnbGfxPpVID3OFF = 0x7C,///<OFF
} IdsNvGnbGfxPpVID3;

///DPM #0 State Definition
///DPM State 0
typedef enum {
  IdsNvGnbGfxDPM0Disabled = 0,///<Disabled
  IdsNvGnbGfxDPM0Enabled = 1,///<Enabled
} IdsNvGnbGfxDPM0;

///DPM #0 VID
///DPM State 0  VID
typedef enum {
  IdsNvGnbGfxDpm0VIDVoltage_from_present_setting_on_VID_0 = 0,///<Voltage from present setting on VID 0
  IdsNvGnbGfxDpm0VIDVoltage_from_present_setting_on_VID_1 = 1,///<Voltage from present setting on VID 1
  IdsNvGnbGfxDpm0VIDVoltage_from_present_setting_on_VID_2 = 2,///<Voltage from present setting on VID 2
  IdsNvGnbGfxDpm0VIDVoltage_from_present_setting_on_VID_3 = 3,///<Voltage from present setting on VID 3
} IdsNvGnbGfxDpm0VID;

///DPM #0 SCLK
///DPM State 0  SCLK
#define IdsNvGnbGfxDpm0SclkMin 25
#define IdsNvGnbGfxDpm0SclkMax 700

///DPM #0 SCLK Cac
///DPM State 0  Cac
#define IdsNvGnbGfxDpm0CacMin 0
#define IdsNvGnbGfxDpm0CacMax 0x3f

///DPM #1 State Definition
///DPM State 1
typedef enum {
  IdsNvGnbGfxDPM1Disabled = 0,///<Disabled
  IdsNvGnbGfxDPM1Enabled = 1,///<Enabled
} IdsNvGnbGfxDPM1;

///DPM #1 SCLK
///DPM State 1  SCLK
#define IdsNvGnbGfxDpm1SclkMin 25
#define IdsNvGnbGfxDpm1SclkMax 700

///DPM #1 VID
///DPM State 1  VID
typedef enum {
  IdsNvGnbGfxDpm1VIDVoltage_from_present_setting_on_VID_0 = 0,///<Voltage from present setting on VID 0
  IdsNvGnbGfxDpm1VIDVoltage_from_present_setting_on_VID_1 = 1,///<Voltage from present setting on VID 1
  IdsNvGnbGfxDpm1VIDVoltage_from_present_setting_on_VID_2 = 2,///<Voltage from present setting on VID 2
  IdsNvGnbGfxDpm1VIDVoltage_from_present_setting_on_VID_3 = 3,///<Voltage from present setting on VID 3
} IdsNvGnbGfxDpm1VID;

///DPM #1 SCLK Cac
///DPM State 1  Cac
#define IdsNvGnbGfxDpm1CacMin 0
#define IdsNvGnbGfxDpm1CacMax 0x3f

///DPM #2 State Definition
///DPM State 2
typedef enum {
  IdsNvGnbGfxDPM2Disabled = 0,///<Disabled
  IdsNvGnbGfxDPM2Enabled = 1,///<Enabled
} IdsNvGnbGfxDPM2;

///DPM #2 SCLK
///DPM State 2  SCLK
#define IdsNvGnbGfxDpm2SclkMin 25
#define IdsNvGnbGfxDpm2SclkMax 700

///DPM #2 VID
///DPM State 2  VID
typedef enum {
  IdsNvGnbGfxDpm2VIDVoltage_from_present_setting_on_VID_0 = 0,///<Voltage from present setting on VID 0
  IdsNvGnbGfxDpm2VIDVoltage_from_present_setting_on_VID_1 = 1,///<Voltage from present setting on VID 1
  IdsNvGnbGfxDpm2VIDVoltage_from_present_setting_on_VID_2 = 2,///<Voltage from present setting on VID 2
  IdsNvGnbGfxDpm2VIDVoltage_from_present_setting_on_VID_3 = 3,///<Voltage from present setting on VID 3
} IdsNvGnbGfxDpm2VID;

///DPM #2 SCLK Cac
///DPM State 2  Cac
#define IdsNvGnbGfxDpm2CacMin 0
#define IdsNvGnbGfxDpm2CacMax 0x3f

///DPM #3 State Definition
///DPM State 3
typedef enum {
  IdsNvGnbGfxDPM3Disabled = 0,///<Disabled
  IdsNvGnbGfxDPM3Enabled = 1,///<Enabled
} IdsNvGnbGfxDPM3;

///DPM #3 SCLK
///DPM State 3  SCLK
#define IdsNvGnbGfxDpm3SclkMin 25
#define IdsNvGnbGfxDpm3SclkMax 700

///DPM #3 VID
///DPM State 3  VID
typedef enum {
  IdsNvGnbGfxDpm3VIDVoltage_from_present_setting_on_VID_0 = 0,///<Voltage from present setting on VID 0
  IdsNvGnbGfxDpm3VIDVoltage_from_present_setting_on_VID_1 = 1,///<Voltage from present setting on VID 1
  IdsNvGnbGfxDpm3VIDVoltage_from_present_setting_on_VID_2 = 2,///<Voltage from present setting on VID 2
  IdsNvGnbGfxDpm3VIDVoltage_from_present_setting_on_VID_3 = 3,///<Voltage from present setting on VID 3
} IdsNvGnbGfxDpm3VID;

///DPM #3 SCLK Cac
///DPM State 3  Cac
#define IdsNvGnbGfxDpm3CacMin 0
#define IdsNvGnbGfxDpm3CacMax 0x3f

///DPM #4 State Definition
///DPM State 4
typedef enum {
  IdsNvGnbGfxDPM4Disabled = 0,///<Disabled
  IdsNvGnbGfxDPM4Enabled = 1,///<Enabled
} IdsNvGnbGfxDPM4;

///DPM #4 SCLK
///DPM State 4  SCLK
#define IdsNvGnbGfxDpm4SclkMin 25
#define IdsNvGnbGfxDpm4SclkMax 700

///DPM #4 VID
///DPM State 4  VID
typedef enum {
  IdsNvGnbGfxDpm4VIDVoltage_from_present_setting_on_VID_0 = 0,///<Voltage from present setting on VID 0
  IdsNvGnbGfxDpm4VIDVoltage_from_present_setting_on_VID_1 = 1,///<Voltage from present setting on VID 1
  IdsNvGnbGfxDpm4VIDVoltage_from_present_setting_on_VID_2 = 2,///<Voltage from present setting on VID 2
  IdsNvGnbGfxDpm4VIDVoltage_from_present_setting_on_VID_3 = 3,///<Voltage from present setting on VID 3
} IdsNvGnbGfxDpm4VID;

///DPM #4 SCLK Cac
///DPM State 4  Cac
#define IdsNvGnbGfxDpm4CacMin 0
#define IdsNvGnbGfxDpm4CacMax 0xff

///VCLK 0
///VCLK 0
#define IdsNvGnbGfxPpVCLK0Min 25
#define IdsNvGnbGfxPpVCLK0Max 800

///DCLK 0
///DCLK 0
#define IdsNvGnbGfxPpDCLK0Min 25
#define IdsNvGnbGfxPpDCLK0Max 700

///VCLK 1
///VCLK 1
#define IdsNvGnbGfxPpVCLK1Min 25
#define IdsNvGnbGfxPpVCLK1Max 800

///DCLK 1
///DCLK 1
#define IdsNvGnbGfxPpDCLK1Min 25
#define IdsNvGnbGfxPpDCLK1Max 700

///VCLK 2
///VCLK 2
#define IdsNvGnbGfxPpVCLK2Min 25
#define IdsNvGnbGfxPpVCLK2Max 800

///DCLK 2
///DCLK 2
#define IdsNvGnbGfxPpDCLK2Min 25
#define IdsNvGnbGfxPpDCLK2Max 700

///VCLK 3
///VCLK 3
#define IdsNvGnbGfxPpVCLK3Min 25
#define IdsNvGnbGfxPpVCLK3Max 800

///DCLK 3
///DCLK 3
#define IdsNvGnbGfxPpDCLK3Min 25
#define IdsNvGnbGfxPpDCLK3Max 700

///PCIe Gen2 VID
///PCIe Gen2 VID
typedef enum {
  IdsNvGnbGfxPcieGen2VIDVoltage_from_present_setting_on_VID_0 = 0,///<Voltage from present setting on VID 0
  IdsNvGnbGfxPcieGen2VIDVoltage_from_present_setting_on_VID_1 = 1,///<Voltage from present setting on VID 1
  IdsNvGnbGfxPcieGen2VIDVoltage_from_present_setting_on_VID_2 = 2,///<Voltage from present setting on VID 2
  IdsNvGnbGfxPcieGen2VIDVoltage_from_present_setting_on_VID_3 = 3,///<Voltage from present setting on VID 3
} IdsNvGnbGfxPcieGen2VID;

///DispCLK0 (VID 0)
///DispCLK0 (VID 0)
#define IdsNvGnbGfxDispCLK0Min 25
#define IdsNvGnbGfxDispCLK0Max 600

///DispCLK1 (VID 1)
///DispCLK1 (VID 1)
#define IdsNvGnbGfxDispCLK1Min 25
#define IdsNvGnbGfxDispCLK1Max 600

///DispCLK2 (VID 2)
///DispCLK2 (VID 2)
#define IdsNvGnbGfxDispCLK2Min 25
#define IdsNvGnbGfxDispCLK2Max 600

///DispCLK3 (VID 3)
///DispCLK3 (VID 3)
#define IdsNvGnbGfxDispCLK3Min 25
#define IdsNvGnbGfxDispCLK3Max 600

///SCLK Thermal (VID 3)
///SCLK Thermal
#define IdsNvGnbGfxSclkThermalMin 25
#define IdsNvGnbGfxSclkThermalMax 600

///DPM LCLK #0 State Definition
///LCLK State Definition #0
typedef enum {
  IdsNvGnbGfxDpmLclkState0Disabled = 0,///<Disabled
  IdsNvGnbGfxDpmLclkState0Enabled = 1,///<Enabled
} IdsNvGnbGfxDpmLclkState0;

///LCLK #0
///LCLK #0
#define IdsNvGnbGfxDpmLclk0Min 25
#define IdsNvGnbGfxDpmLclk0Max 700

///DPM LCLK #1 State Definition
///LCLK State Definition #1
typedef enum {
  IdsNvGnbGfxDpmLclkState1Disabled = 0,///<Disabled
  IdsNvGnbGfxDpmLclkState1Enabled = 1,///<Enabled
} IdsNvGnbGfxDpmLclkState1;

///DPM LCLK #2 State Definition
///LCLK State Definition #2
typedef enum {
  IdsNvGnbGfxDpmLclkState2Disabled = 0,///<Disabled
  IdsNvGnbGfxDpmLclkState2Enabled = 1,///<Enabled
} IdsNvGnbGfxDpmLclkState2;

///LCLK #1
///LCLK #1
#define IdsNvGnbGfxDpmLclk1Min 25
#define IdsNvGnbGfxDpmLclk1Max 700

///LCLK #2
///LCLK #2
#define IdsNvGnbGfxDpmLclk2Min 25
#define IdsNvGnbGfxDpmLclk2Max 700

///DPM LCLK #3 State Definition
///LCLK State Definition #3
typedef enum {
  IdsNvGnbGfxDpmLclkState3Disabled = 0,///<Disabled
  IdsNvGnbGfxDpmLclkState3Enabled = 1,///<Enabled
} IdsNvGnbGfxDpmLclkState3;

///LCLK #3
///LCLK #3
#define IdsNvGnbGfxDpmLclk3Min 25
#define IdsNvGnbGfxDpmLclk3Max 700

///Power Play Table Rev
///Power Play table Rev
#define IdsNvGnbGfxPpTableRevMin 0
#define IdsNvGnbGfxPpTableRevMax 15

///SW State #0
///SW State #0
typedef enum {
  IdsNvGnbGfxSwState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSwState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSwState0;

///SW #0 Policy Label
///Policy Label
typedef enum {
  IdsNvGnbGfxSw0PolicyLabelNon_Labeled = 0,///<Non-Labeled
  IdsNvGnbGfxSw0PolicyLabelBattery = 1,///<Battery
  IdsNvGnbGfxSw0PolicyLabelPerformance = 2,///<Performance
} IdsNvGnbGfxSw0PolicyLabel;

///SW #0 Classification
///Classification
typedef enum {
  IdsNvGnbGfxSw0ClassificationNon_UVD = 0,///<Non-UVD
  IdsNvGnbGfxSw0ClassificationUVD_Performance = 1,///<UVD Performance
  IdsNvGnbGfxSw0ClassificationReserved___2_ = 2,///<Reserved (#2)
  IdsNvGnbGfxSw0ClassificationHD_State = 3,///<HD State
  IdsNvGnbGfxSw0ClassificationSD_State = 4,///<SD State
  IdsNvGnbGfxSw0ClassificationReserved___5_ = 5,///<Reserved (#5)
  IdsNvGnbGfxSw0ClassificationReserved___6_ = 6,///<Reserved (#6)
  IdsNvGnbGfxSw0ClassificationReserved___7_ = 7,///<Reserved (#7)
} IdsNvGnbGfxSw0Classification;

///SW #0 VCLK/DCK Index
///VCLK/DCK Index
typedef enum {
  IdsNvGnbGfxSw0VclkDclkIndexVCLK0_DCLK0 = 0,///<VCLK0/DCLK0
  IdsNvGnbGfxSw0VclkDclkIndexVCLK1_DCLK1 = 1,///<VCLK1/DCLK1
  IdsNvGnbGfxSw0VclkDclkIndexVCLK2_DCLK2 = 2,///<VCLK2/DCLK2
  IdsNvGnbGfxSw0VclkDclkIndexVCLK3_DCLK4 = 3,///<VCLK3/DCLK4
} IdsNvGnbGfxSw0VclkDclkIndex;

///SW #0 DPM State #0
///SW #0 DPM State #0
typedef enum {
  IdsNvGnbGfxSw0DpmState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSw0DpmState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSw0DpmState0;

///SW #0 DPM State #1
///SW #0 DPM State #1
typedef enum {
  IdsNvGnbGfxSw0DpmState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSw0DpmState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSw0DpmState1;

///SW #0 DPM State #2
///SW #0 DPM State #2
typedef enum {
  IdsNvGnbGfxSw0DpmState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSw0DpmState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSw0DpmState2;

///SW #0 DPM State #3
///SW #0 DPM State #3
typedef enum {
  IdsNvGnbGfxSw0DpmState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSw0DpmState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSw0DpmState3;

///SW #0 DPM State #4
///SW #0 DPM State #4
typedef enum {
  IdsNvGnbGfxSw0DpmState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSw0DpmState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSw0DpmState4;

///SW State #1
///SW State #1
typedef enum {
  IdsNvGnbGfxSwState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSwState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSwState1;

///SW #1 Policy Label
///Policy Label
typedef enum {
  IdsNvGnbGfxSw1PolicyLabelNon_Labeled = 0,///<Non-Labeled
  IdsNvGnbGfxSw1PolicyLabelBattery = 1,///<Battery
  IdsNvGnbGfxSw1PolicyLabelPerformance = 2,///<Performance
} IdsNvGnbGfxSw1PolicyLabel;

///SW #1 Classification
///SW #1 Classification
typedef enum {
  IdsNvGnbGfxSw1ClassificationNon_UVD = 0,///<Non-UVD
  IdsNvGnbGfxSw1ClassificationUVD_Performance = 1,///<UVD Performance
  IdsNvGnbGfxSw1ClassificationReserved___2_ = 2,///<Reserved (#2)
  IdsNvGnbGfxSw1ClassificationHD_State = 3,///<HD State
  IdsNvGnbGfxSw1ClassificationSD_State = 4,///<SD State
  IdsNvGnbGfxSw1ClassificationReserved___5_ = 5,///<Reserved (#5)
  IdsNvGnbGfxSw1ClassificationReserved___6_ = 6,///<Reserved (#6)
  IdsNvGnbGfxSw1ClassificationReserved___7_ = 7,///<Reserved (#7)
} IdsNvGnbGfxSw1Classification;

///SW #1 VCLK/DCK Index
///VCLK/DCK Index
typedef enum {
  IdsNvGnbGfxSw1VclkDclkIndexVCLK0_DCLK0 = 0,///<VCLK0/DCLK0
  IdsNvGnbGfxSw1VclkDclkIndexVCLK1_DCLK1 = 1,///<VCLK1/DCLK1
  IdsNvGnbGfxSw1VclkDclkIndexVCLK2_DCLK2 = 2,///<VCLK2/DCLK2
  IdsNvGnbGfxSw1VclkDclkIndexVCLK3_DCLK4 = 3,///<VCLK3/DCLK4
} IdsNvGnbGfxSw1VclkDclkIndex;

///SW #1 DPM State #0
///SW #1 DPM State #0
typedef enum {
  IdsNvGnbGfxSw1DpmState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSw1DpmState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSw1DpmState0;

///SW #1 DPM State #1
///SW #1 DPM State #1
typedef enum {
  IdsNvGnbGfxSw1DpmState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSw1DpmState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSw1DpmState1;

///SW #1 DPM State #2
///SW #1 DPM State #2
typedef enum {
  IdsNvGnbGfxSw1DpmState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSw1DpmState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSw1DpmState2;

///SW #1 DPM State #3
///SW #1 DPM State #3
typedef enum {
  IdsNvGnbGfxSw1DpmState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSw1DpmState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSw1DpmState3;

///SW #1 DPM State #4
///SW #1 DPM State #4
typedef enum {
  IdsNvGnbGfxSw1DpmState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSw1DpmState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSw1DpmState4;

///SW #2 Classification
///Classification
typedef enum {
  IdsNvGnbGfxSw2ClassificationNon_UVD = 0,///<Non-UVD
  IdsNvGnbGfxSw2ClassificationUVD_Performance = 1,///<UVD Performance
  IdsNvGnbGfxSw2ClassificationReserved___2_ = 2,///<Reserved (#2)
  IdsNvGnbGfxSw2ClassificationHD_State = 3,///<HD State
  IdsNvGnbGfxSw2ClassificationSD_State = 4,///<SD State
  IdsNvGnbGfxSw2ClassificationReserved___5_ = 5,///<Reserved (#5)
  IdsNvGnbGfxSw2ClassificationReserved___6_ = 6,///<Reserved (#6)
  IdsNvGnbGfxSw2ClassificationReserved___7_ = 7,///<Reserved (#7)
} IdsNvGnbGfxSw2Classification;

///SW State #2
///SW State #2
typedef enum {
  IdsNvGnbGfxSwState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSwState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSwState2;

///SW #2 Policy Label
///Policy Label
typedef enum {
  IdsNvGnbGfxSw2PolicyLabelNon_Labeled = 0,///<Non-Labeled
  IdsNvGnbGfxSw2PolicyLabelBattery = 1,///<Battery
  IdsNvGnbGfxSw2PolicyLabelPerformance = 2,///<Performance
} IdsNvGnbGfxSw2PolicyLabel;

///SW #2 VCLK/DCK Index
///VCLK/DCK Index
typedef enum {
  IdsNvGnbGfxSw2VclkDclkIndexVCLK0_DCLK0 = 0,///<VCLK0/DCLK0
  IdsNvGnbGfxSw2VclkDclkIndexVCLK1_DCLK1 = 1,///<VCLK1/DCLK1
  IdsNvGnbGfxSw2VclkDclkIndexVCLK2_DCLK2 = 2,///<VCLK2/DCLK2
  IdsNvGnbGfxSw2VclkDclkIndexVCLK3_DCLK4 = 3,///<VCLK3/DCLK4
} IdsNvGnbGfxSw2VclkDclkIndex;

///SW #2 DPM State #0
///SW #2 DPM State #0
typedef enum {
  IdsNvGnbGfxSw2DpmState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSw2DpmState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSw2DpmState0;

///SW #2 DPM State #1
///SW #2 DPM State #1
typedef enum {
  IdsNvGnbGfxSw2DpmState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSw2DpmState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSw2DpmState1;

///SW #2 DPM State #2
///SW #2 DPM State #2
typedef enum {
  IdsNvGnbGfxSw2DpmState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSw2DpmState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSw2DpmState2;

///SW #2 DPM State #3
///SW #2 DPM State #3
typedef enum {
  IdsNvGnbGfxSw2DpmState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSw2DpmState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSw2DpmState3;

///SW #2 DPM State #4
///SW #2 DPM State #4
typedef enum {
  IdsNvGnbGfxSw2DpmState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSw2DpmState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSw2DpmState4;

///SW State #3
///SW State #3
typedef enum {
  IdsNvGnbGfxSwState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSwState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSwState3;

///SW #3 Policy Label
///Policy Label
typedef enum {
  IdsNvGnbGfxSw3PolicyLabelNon_Labeled = 0,///<Non-Labeled
  IdsNvGnbGfxSw3PolicyLabelBattery = 1,///<Battery
  IdsNvGnbGfxSw3PolicyLabelPerformance = 2,///<Performance
} IdsNvGnbGfxSw3PolicyLabel;

///SW #3 Classification
///Classification
typedef enum {
  IdsNvGnbGfxSw3ClassificationNon_UVD = 0,///<Non-UVD
  IdsNvGnbGfxSw3ClassificationUVD_Performance = 1,///<UVD Performance
  IdsNvGnbGfxSw3ClassificationReserved___2_ = 2,///<Reserved (#2)
  IdsNvGnbGfxSw3ClassificationHD_State = 3,///<HD State
  IdsNvGnbGfxSw3ClassificationSD_State = 4,///<SD State
  IdsNvGnbGfxSw3ClassificationReserved___5_ = 5,///<Reserved (#5)
  IdsNvGnbGfxSw3ClassificationReserved___6_ = 6,///<Reserved (#6)
  IdsNvGnbGfxSw3ClassificationReserved___7_ = 7,///<Reserved (#7)
} IdsNvGnbGfxSw3Classification;

///SW #3 VCLK/DCK Index
///VCLK/DCK Index
typedef enum {
  IdsNvGnbGfxSw3VclkDclkIndexVCLK0_DCLK0 = 0,///<VCLK0/DCLK0
  IdsNvGnbGfxSw3VclkDclkIndexVCLK1_DCLK1 = 1,///<VCLK1/DCLK1
  IdsNvGnbGfxSw3VclkDclkIndexVCLK2_DCLK2 = 2,///<VCLK2/DCLK2
  IdsNvGnbGfxSw3VclkDclkIndexVCLK3_DCLK4 = 3,///<VCLK3/DCLK4
} IdsNvGnbGfxSw3VclkDclkIndex;

///SW #3 DPM State #0
///SW #3 DPM State #0
typedef enum {
  IdsNvGnbGfxSw3DpmState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSw3DpmState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSw3DpmState0;

///SW #3 DPM State #1
///SW #3 DPM State #1
typedef enum {
  IdsNvGnbGfxSw3DpmState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSw3DpmState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSw3DpmState1;

///SW #3 DPM State #2
///SW #3 DPM State #2
typedef enum {
  IdsNvGnbGfxSw3DpmState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSw3DpmState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSw3DpmState2;

///SW #3 DPM State #3
///SW #3 DPM State #3
typedef enum {
  IdsNvGnbGfxSw3DpmState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSw3DpmState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSw3DpmState3;

///SW #3 DPM State #4
///SW #3 DPM State #4
typedef enum {
  IdsNvGnbGfxSw3DpmState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSw3DpmState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSw3DpmState4;

///SW State #4
///SW State #4
typedef enum {
  IdsNvGnbGfxSwState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSwState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSwState4;

///SW #4 Policy Label
///Policy Label
typedef enum {
  IdsNvGnbGfxSw4PolicyLabelNon_Labeled = 0,///<Non-Labeled
  IdsNvGnbGfxSw4PolicyLabelBattery = 1,///<Battery
  IdsNvGnbGfxSw4PolicyLabelPerformance = 2,///<Performance
} IdsNvGnbGfxSw4PolicyLabel;

///SW #4 Classification
///SW #4 Classification
typedef enum {
  IdsNvGnbGfxSw4ClassificationNon_UVD = 0,///<Non-UVD
  IdsNvGnbGfxSw4ClassificationUVD_Performance = 1,///<UVD Performance
  IdsNvGnbGfxSw4ClassificationReserved___2_ = 2,///<Reserved (#2)
  IdsNvGnbGfxSw4ClassificationHD_State = 3,///<HD State
  IdsNvGnbGfxSw4ClassificationSD_State = 4,///<SD State
  IdsNvGnbGfxSw4ClassificationReserved___5_ = 5,///<Reserved (#5)
  IdsNvGnbGfxSw4ClassificationReserved___6_ = 6,///<Reserved (#6)
  IdsNvGnbGfxSw4ClassificationReserved___7_ = 7,///<Reserved (#7)
} IdsNvGnbGfxSw4Classification;

///SW #4 VCLK/DCK Index
///SW #4 VCLK/DCK Index
typedef enum {
  IdsNvGnbGfxSw4VclkDclkIndexVCLK0_DCLK0 = 0,///<VCLK0/DCLK0
  IdsNvGnbGfxSw4VclkDclkIndexVCLK1_DCLK1 = 1,///<VCLK1/DCLK1
  IdsNvGnbGfxSw4VclkDclkIndexVCLK2_DCLK2 = 2,///<VCLK2/DCLK2
  IdsNvGnbGfxSw4VclkDclkIndexVCLK3_DCLK4 = 3,///<VCLK3/DCLK4
} IdsNvGnbGfxSw4VclkDclkIndex;

///SW #4 DPM State #0
///SW #4 DPM State #0
typedef enum {
  IdsNvGnbGfxSw4DpmState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSw4DpmState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSw4DpmState0;

///SW #4 DPM State #1
///SW #4 DPM State #1
typedef enum {
  IdsNvGnbGfxSw4DpmState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSw4DpmState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSw4DpmState1;

///SW #4 DPM State #2
///SW #4 DPM State #2
typedef enum {
  IdsNvGnbGfxSw4DpmState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSw4DpmState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSw4DpmState2;

///SW #4 DPM State #3
///SW #4 DPM State #3
typedef enum {
  IdsNvGnbGfxSw4DpmState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSw4DpmState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSw4DpmState3;

///SW #4 DPM State #4
///SW #4 DPM State #4
typedef enum {
  IdsNvGnbGfxSw4DpmState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSw4DpmState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSw4DpmState4;

///SW State #5
///SW State #5
typedef enum {
  IdsNvGnbGfxSwState5Disabled = 0,///<Disabled
  IdsNvGnbGfxSwState5Enabled = 1,///<Enabled
} IdsNvGnbGfxSwState5;

///SW #5 Policy Label
///SW #5 Policy Label
typedef enum {
  IdsNvGnbGfxSw5PolicyLabelNon_Labeled = 0,///<Non-Labeled
  IdsNvGnbGfxSw5PolicyLabelBattery = 1,///<Battery
  IdsNvGnbGfxSw5PolicyLabelPerformance = 2,///<Performance
} IdsNvGnbGfxSw5PolicyLabel;

///SW #5 Classification
///Classification
typedef enum {
  IdsNvGnbGfxSw5ClassificationNon_UVD = 0,///<Non-UVD
  IdsNvGnbGfxSw5ClassificationUVD_Performance = 1,///<UVD Performance
  IdsNvGnbGfxSw5ClassificationReserved___2_ = 2,///<Reserved (#2)
  IdsNvGnbGfxSw5ClassificationHD_State = 3,///<HD State
  IdsNvGnbGfxSw5ClassificationSD_State = 4,///<SD State
  IdsNvGnbGfxSw5ClassificationReserved___5_ = 5,///<Reserved (#5)
  IdsNvGnbGfxSw5ClassificationReserved___6_ = 6,///<Reserved (#6)
  IdsNvGnbGfxSw5ClassificationReserved___7_ = 7,///<Reserved (#7)
} IdsNvGnbGfxSw5Classification;

///SW #5 VCLK/DCK Index
///VCLK/DCK Index
typedef enum {
  IdsNvGnbGfxSw5VclkDclkIndexVCLK0_DCLK0 = 0,///<VCLK0/DCLK0
  IdsNvGnbGfxSw5VclkDclkIndexVCLK1_DCLK1 = 1,///<VCLK1/DCLK1
  IdsNvGnbGfxSw5VclkDclkIndexVCLK2_DCLK2 = 2,///<VCLK2/DCLK2
  IdsNvGnbGfxSw5VclkDclkIndexVCLK3_DCLK4 = 3,///<VCLK3/DCLK4
} IdsNvGnbGfxSw5VclkDclkIndex;

///SW #5 DPM State #0
///SW #5 DPM State #0
typedef enum {
  IdsNvGnbGfxSw5DpmState0Disabled = 0,///<Disabled
  IdsNvGnbGfxSw5DpmState0Enabled = 1,///<Enabled
} IdsNvGnbGfxSw5DpmState0;

///SW #5 DPM State #1
///SW #5 DPM State #1
typedef enum {
  IdsNvGnbGfxSw5DpmState1Disabled = 0,///<Disabled
  IdsNvGnbGfxSw5DpmState1Enabled = 1,///<Enabled
} IdsNvGnbGfxSw5DpmState1;

///SW #5 DPM State #2
///SW #5 DPM State #2
typedef enum {
  IdsNvGnbGfxSw5DpmState2Disabled = 0,///<Disabled
  IdsNvGnbGfxSw5DpmState2Enabled = 1,///<Enabled
} IdsNvGnbGfxSw5DpmState2;

///SW #5 DPM State #3
///SW #5 DPM State #3
typedef enum {
  IdsNvGnbGfxSw5DpmState3Disabled = 0,///<Disabled
  IdsNvGnbGfxSw5DpmState3Enabled = 1,///<Enabled
} IdsNvGnbGfxSw5DpmState3;

///SW #5 DPM State #4
///SW #5 DPM State #4
typedef enum {
  IdsNvGnbGfxSw5DpmState4Disabled = 0,///<Disabled
  IdsNvGnbGfxSw5DpmState4Enabled = 1,///<Enabled
} IdsNvGnbGfxSw5DpmState4;

///#1
///#1
#define IdsNvGnbGfxPpTableNo1Min 0
#define IdsNvGnbGfxPpTableNo1Max 0xffff

///#2
///#2
#define IdsNvGnbGfxPpTableNo2Min 0
#define IdsNvGnbGfxPpTableNo2Max 0xffff

///#3
///#3
#define IdsNvGnbGfxPpTableNo3Min 0
#define IdsNvGnbGfxPpTableNo3Max 0xffff

///#4
///#4
#define IdsNvGnbGfxPpTableNo4Min 0
#define IdsNvGnbGfxPpTableNo4Max 0xffff

///#5
///#5
#define IdsNvGnbGfxPpTableNo5Min 0
#define IdsNvGnbGfxPpTableNo5Max 0xffff

///#6
///#6
#define IdsNvGnbGfxPpTableNo6Min 0
#define IdsNvGnbGfxPpTableNo6Max 0xffff

///#7
///#7
#define IdsNvGnbGfxPpTableNo7Min 0
#define IdsNvGnbGfxPpTableNo7Max 0xffff

///#8
///#8
#define IdsNvGnbGfxPpTableNo8Min 0
#define IdsNvGnbGfxPpTableNo8Max 0xffff

///#9
///#9
#define IdsNvGnbGfxPpTableNo9Min 0
#define IdsNvGnbGfxPpTableNo9Max 0xffff

///#10
///#10
#define IdsNvGnbGfxPpTableNo10Min 0
#define IdsNvGnbGfxPpTableNo10Max 0xffff

///#11
///#11
#define IdsNvGnbGfxPpTableNo11Min 0
#define IdsNvGnbGfxPpTableNo11Max 0xffff

///#12
///#12
#define IdsNvGnbGfxPpTableNo12Min 0
#define IdsNvGnbGfxPpTableNo12Max 0xffff

///#13
///#13
#define IdsNvGnbGfxPpTableNo13Min 0
#define IdsNvGnbGfxPpTableNo13Max 0xffff

///#14
///#14
#define IdsNvGnbGfxPpTableNo14Min 0
#define IdsNvGnbGfxPpTableNo14Max 0xffff

///#15
///#15
#define IdsNvGnbGfxPpTableNo15Min 0
#define IdsNvGnbGfxPpTableNo15Max 0xffff

///#16
///#16
#define IdsNvGnbGfxPpTableNo16Min 0
#define IdsNvGnbGfxPpTableNo16Max 0xffff

///#17
///#17
#define IdsNvGnbGfxPpTableNo17Min 0
#define IdsNvGnbGfxPpTableNo17Max 0xffff

///#18
///#18
#define IdsNvGnbGfxPpTableNo18Min 0
#define IdsNvGnbGfxPpTableNo18Max 0xffff

///#19
///#19
#define IdsNvGnbGfxPpTableNo19Min 0
#define IdsNvGnbGfxPpTableNo19Max 0xffff

///#20
///#20
#define IdsNvGnbGfxPpTableNo20Min 0
#define IdsNvGnbGfxPpTableNo20Max 0xffff

///#21
///#21
#define IdsNvGnbGfxPpTableNo21Min 0
#define IdsNvGnbGfxPpTableNo21Max 0xffff

///PCIe lanes topology
///Set PCIe lanes Topology
typedef enum {
  IdsNvGnbPcieLanesTopoAuto = 0,///<Auto
  IdsNvGnbPcieLanesTopoManual = 1,///<Manual
} IdsNvGnbPcieLanesTopo;

///Port Training
///Set PCIe port Training
typedef enum {
  IdsNvGnbPciePortxTrainingEnabled = 0,///<Enabled
  IdsNvGnbPciePortxTrainingDisabled = 1,///<Disabled
} IdsNvGnbPciePortxTraining;

///Hotplug Support
///Set Hotplug
typedef enum {
  IdsNvGnbPciePortxHotplugDisabled = 0,///<Disabled
  IdsNvGnbPciePortxHotplugBasic = 1,///<Basic
  IdsNvGnbPciePortxHotplugEnhanced = 2,///<Enhanced
} IdsNvGnbPciePortxHotplug;

///Speed Mode
///Set PCIe port speed and capability
typedef enum {
  IdsNvGnbPciePortxSpeedModeAuto = 0,///<Auto
  IdsNvGnbPciePortxSpeedModeGEN1 = 1,///<GEN1
  IdsNvGnbPciePortxSpeedModeGEN2 = 2,///<GEN2
} IdsNvGnbPciePortxSpeedMode;

///Compliance Mode
///Enable compliance mode
typedef enum {
  IdsNvGnbPciePortxComplianceModeDisabled = 0,///<Disabled
  IdsNvGnbPciePortxComplianceModeEnabled = 1,///<Enabled
} IdsNvGnbPciePortxComplianceMode;

///Link ASPM
///Enable port ASPM
typedef enum {
  IdsNvGnbPciePortxLinkASPMDisabled = 0,///<Disabled
  IdsNvGnbPciePortxLinkASPML0s = 1,///<L0s
  IdsNvGnbPciePortxLinkASPML1 = 2,///<L1
  IdsNvGnbPciePortxLinkASPML0s_and_L1 = 3,///<L0s and L1
} IdsNvGnbPciePortxLinkASPM;

///DFE Control
///Link DFE control
typedef enum {
  IdsNvGnbPciePortxDFEControl25mV = 0,///<25mV
  IdsNvGnbPciePortxDFEControl0mV = 1,///<0mV
  IdsNvGnbPciePortxDFEControl12_5mV = 2,///<12.5mV
  IdsNvGnbPciePortxDFEControl31_25mV = 3,///<31.25mV
} IdsNvGnbPciePortxDFEControl;

///Channel Type
///Channel type
typedef enum {
  IdsNvGnbPciePortxChannelTypelow_loss__3_5db_ = 0,///<low-loss(-3.5db)
  IdsNvGnbPciePortxChannelTypehigh_loss___6db_ = 1,///<high loss (-6db)
  IdsNvGnbPciePortxChannelTypehalf_swing__0db_ = 2,///<half-swing (0db)
  IdsNvGnbPciePortxChannelTypehalf_swing___3_5db_ = 3,///<half-swing (-3.5db)
  IdsNvGnbPciePortxChannelTypeextended_length___6db_ = 4,///<extended length (-6db)
  IdsNvGnbPciePortxChannelTypeextended_length___8db_ = 5,///<extended length (-8db)
} IdsNvGnbPciePortxChannelType;

///Slot Power Limit,W
///Slot Power Limit in Watt
#define IdsNvGnbPciePortxSlotPowerLimitMin 0
#define IdsNvGnbPciePortxSlotPowerLimitMax 255

///L1 Immediate ACK
///L1 Immediate ACK
typedef enum {
  IdsNvGnbPciePortxL1ImmACKDisabled = 0,///<Disabled
  IdsNvGnbPciePortxL1ImmACKEnabled = 1,///<Enabled
} IdsNvGnbPciePortxL1ImmACK;

///TX De-emphasis
///TX De-emphasis
typedef enum {
  IdsNvGnbPciePortxTxDeemphasisAuto = 0,///<Auto
  IdsNvGnbPciePortxTxDeemphasis3_5_dB = 1,///<3.5 dB
  IdsNvGnbPciePortxTxDeemphasis6_0_dB = 2,///<6.0 dB
} IdsNvGnbPciePortxTxDeemphasis;

///TX Margin
///TX Margin
typedef enum {
  IdsNvGnbPciePortxTxMargin0 = 0,///<0
  IdsNvGnbPciePortxTxMargin1 = 1,///<1
  IdsNvGnbPciePortxTxMargin2 = 2,///<2
  IdsNvGnbPciePortxTxMargin3 = 3,///<3
  IdsNvGnbPciePortxTxMargin4 = 4,///<4
  IdsNvGnbPciePortxTxMargin5 = 5,///<5
  IdsNvGnbPciePortxTxMargin6 = 6,///<6
  IdsNvGnbPciePortxTxMargin7 = 7,///<7
  IdsNvGnbPciePortxTxMarginAuto = 8,///<Auto
} IdsNvGnbPciePortxTxMargin;

///Scrambler control
///Scrambler control
typedef enum {
  IdsNvGnbPciePortxScramblerCntlDisabled = 0,///<Disabled
  IdsNvGnbPciePortxScramblerCntlEnabled = 1,///<Enabled
  IdsNvGnbPciePortxScramblerCntlAuto = 3,///<Auto
} IdsNvGnbPciePortxScramblerCntl;

///PCIE Core Power Gating Data(GFX Wrapper)
///PCIE Core Power Gating Data
typedef enum {
  IdsNvGnbPCIeCorePowerGatingDataAuto = 0,///<Auto
  IdsNvGnbPCIeCorePowerGatingDataManual = 1,///<Manual
} IdsNvGnbPCIeCorePowerGatingData;

///PCIe Core Power Gating(GFX Wrapper)
///Endbale PCIe Core power gating
typedef enum {
  IdsNvGnbPCIeCorePowerGatingDisabled = 0,///<Disabled
  IdsNvGnbPCIeCorePowerGatingEnabled = 1,///<Enabled
  IdsNvGnbPCIeCorePowerGatingAuto = 0xf,///<Auto
} IdsNvGnbPCIeCorePowerGating;

///PCIe Phy Power Gating(GFX Wrapper)
///Endbale PCIe Phy power gating
typedef enum {
  IdsNvGnbPCIePhyPowerGatingDisabled = 0,///<Disabled
  IdsNvGnbPCIePhyPowerGatingEnabled = 1,///<Enabled
  IdsNvGnbPCIePhyPowerGatingAuto = 0xf,///<Auto
} IdsNvGnbPCIePhyPowerGating;

///Isolation Timer(GFX Wrapper)
///Isolation Timer
#define IdsNvGnbPCIeIsolationTimerMin 0
#define IdsNvGnbPCIeIsolationTimerMax 0xffff

///Reset Timer(GFX Wrapper)
///Reset Timer
#define IdsNvGnbPCIeResetTimerMin 0
#define IdsNvGnbPCIeResetTimerMax 0xffff

///Daughter Timer Powerup(GFX Wrapper)
///Daughter Timer Powerup
#define IdsNvGnbPCIeDaughterTimerPowerupMin 0
#define IdsNvGnbPCIeDaughterTimerPowerupMax 0xffff

///Daughter Timer Powerdown(GFX Wrapper)
///Daughter Timer Powerdown
#define IdsNvGnbPCIeDaughterTimerPowerdownMin 0
#define IdsNvGnbPCIeDaughterTimerPowerdownMax 0xffff

///Mother Timer Powerup(GFX Wrapper)
///Mother Timer Powerup
#define IdsNvGnbPCIeMotherTimerPowerupMin 0
#define IdsNvGnbPCIeMotherTimerPowerupMax 0xffff

///Mother Timer Powerdown(GFX Wrapper)
///Mother Timer Powerdown
#define IdsNvGnbPCIeMotherTimerPowerdownMin 0
#define IdsNvGnbPCIeMotherTimerPowerdownMax 0xffff

///Receiver Detection Pooling
///Receiver detection pooling time
#define IdsNvGnbPCIeReceiverDetectPoolingMin 2
#define IdsNvGnbPCIeReceiverDetectPoolingMax 255

///Reset To training Delay
///Reset To training Delay
#define IdsNvGnbPCIeResetToTrainingDelayMin 0
#define IdsNvGnbPCIeResetToTrainingDelayMax 65535

///OSC mode
///Osc mode
typedef enum {
  IdsNvGnbPCIeOscSwitchFuses = 0,///<Fuses
  IdsNvGnbPCIeOscSwitchR0 = 1,///<R0
  IdsNvGnbPCIeOscSwitchLC = 2,///<LC
  IdsNvGnbPCIeOscSwitchDefault = 3,///<Default
} IdsNvGnbPCIeOscSwitch;

///RV370/RV380 Card Workaround
///RV370/RV380 workaround
typedef enum {
  IdsNvGnbPCIeRV370RV380WorkaroundDisabled = 0,///<Disabled
  IdsNvGnbPCIeRV370RV380WorkaroundEnabled = 1,///<Enabled
} IdsNvGnbPCIeRV370RV380Workaround;

///Native PCIe Support
///Native PCIe Support
typedef enum {
  IdsNvGnbPCIeNativePCIeSupportDisabled = 0,///<Disabled
  IdsNvGnbPCIeNativePCIeSupportEnabled = 1,///<Enabled
} IdsNvGnbPCIeNativePCIeSupport;

///PSPP Power Mode Override
///PCIe speed power policy mode override
typedef enum {
  IdsNvGnbPCIePsppPowerModeOverrideAC = 0,///<AC
  IdsNvGnbPCIePsppPowerModeOverrideDC = 1,///<DC
} IdsNvGnbPCIePsppPowerModeOverride;

///Unit ID Clumping
///Enable clumping of internal unit IDs 2 and 3
typedef enum {
  IdsNvGnbPCIeUnitIdClumpingDisabled = 0,///<Disabled
  IdsNvGnbPCIeUnitIdClumpingEnabled = 1,///<Enabled
} IdsNvGnbPCIeUnitIdClumping;

///Garlic Credit Override
///Garlic Credit Override
#define IdsNvGnbPmmGarlicCreditMin 0
#define IdsNvGnbPmmGarlicCreditMax 0xf

///ORB Dynamic Wake
///ORB Dynamic Wake
typedef enum {
  IdsNvGnbPmmOrbDynamicWakeDisabled = 0,///<Disabled
  IdsNvGnbPmmOrbDynamicWakeEnabled = 1,///<Enabled
  IdsNvGnbPmmOrbDynamicWakeAuto = 0xf,///<Auto
} IdsNvGnbPmmOrbDynamicWake;

///ORB Wake Hysteresis
///ORB Wake Hysteresis
typedef enum {
  IdsNvGnbPmmOrbWakeHysteresisAuto = 0,///<Auto
  IdsNvGnbPmmOrbWakeHysteresisManual = 1,///<Manual
} IdsNvGnbPmmOrbWakeHysteresis;

///ORB Wake Hysteresis Value
///ORB Wake Hysteresis Value
#define IdsNvGnbPmmOrbWakeHysteresisValMin 0
#define IdsNvGnbPmmOrbWakeHysteresisValMax 0xffff

///SCLK Boot Speed
///SCLK Boot Speed
#define IdsNvGnbPmmSclkBootSpeedMin 25
#define IdsNvGnbPmmSclkBootSpeedMax 700

///LCLK Boot Speed
///LCLK Boot Speed
#define IdsNvGnbPmmLclkBootSpeedMin 25
#define IdsNvGnbPmmLclkBootSpeedMax 700

///LCLK Gating (SMU)
///LCLK Gating (SMU)
typedef enum {
  IdsNvGnbPmmLclkGatingSmuDisabled = 0,///<Disabled
  IdsNvGnbPmmLclkGatingSmuEnabled = 1,///<Enabled
} IdsNvGnbPmmLclkGatingSmu;

///LCLK Gating (ORB)
///LCLK Gating (ORB)
typedef enum {
  IdsNvGnbPmmLclkGatingOrbDisabled = 0,///<Disabled
  IdsNvGnbPmmLclkGatingOrbEnabled = 1,///<Enabled
} IdsNvGnbPmmLclkGatingOrb;

///SCLK/NCLK GMC Gating
///SCLK/NCLK GMC Gating
typedef enum {
  IdsNvGnbPmmSclkGmcGatingDisabled = 0,///<Disabled
  IdsNvGnbPmmSclkGmcGatingEnabled = 1,///<Enabled
  IdsNvGnbPmmSclkGmcGatingAuto = 0xf,///<Auto
} IdsNvGnbPmmSclkGmcGating;

///SCLK Gating (ORB)
///SCLK Gating (ORB)
typedef enum {
  IdsNvGnbPmmSclkGatingOrbDisabled = 0,///<Disabled
  IdsNvGnbPmmSclkGatingOrbEnabled = 1,///<Enabled
} IdsNvGnbPmmSclkGatingOrb;

///SCLK Gating (DCE)
///SCLK Gating (DCE)
typedef enum {
  IdsNvGnbPmmSclkGatingDceDisabled = 0,///<Disabled
  IdsNvGnbPmmSclkGatingDceEnabled = 1,///<Enabled
} IdsNvGnbPmmSclkGatingDce;

///SCLK Gating (BIF)
///SCLK Gating (BIF)
typedef enum {
  IdsNvGnbPmmSclkGatingBifDisabled = 0,///<Disabled
  IdsNvGnbPmmSclkGatingBifEnabled = 1,///<Enabled
} IdsNvGnbPmmSclkGatingBif;

///SCLK Gating (SMU)
///SCLK Gating (SMU)
typedef enum {
  IdsNvGnbPmmSclkGatingSmuDisabled = 0,///<Disabled
  IdsNvGnbPmmSclkGatingSmuEnabled = 1,///<Enabled
} IdsNvGnbPmmSclkGatingSmu;

///SCLK Gating (IOC)
///SCLK Gating (IOC)
typedef enum {
  IdsNvGnbPmmSclkGatingIocDisabled = 0,///<Disabled
  IdsNvGnbPmmSclkGatingIocEnabled = 1,///<Enabled
} IdsNvGnbPmmSclkGatingIoc;

///DISPCLK Gating (DCE)
///DISPCLK Gating (DCE)
typedef enum {
  IdsNvGnbPmmDispClkGatingDceDisabled = 0,///<Disabled
  IdsNvGnbPmmDispClkGatingDceEnabled = 1,///<Enabled
} IdsNvGnbPmmDispClkGatingDce;

///DeadLockWidgetDis
///Enable or disable DeadLockWidgetDis
typedef enum {
  IDS_NV_DEADLOCK_WIDGET_DIS_0 = 0,              ///< 0
  IDS_NV_DEADLOCK_WIDGET_DIS_1 = 1,              ///< 1
  IDS_NV_DEADLOCK_WIDGET_DIS_AUTO = 3,              ///< Auto
} IDS_NV_DEADLOCK_WIDGET;

///Gnb SwTjOffset Control
///
typedef enum {
  IdsNvGnbPmmGnbSwTjOffsetCtlManual = 0,///<Manual
  IdsNvGnbPmmGnbSwTjOffsetCtlAuto = 1,///<Auto
} IdsNvGnbPmmGnbSwTjOffsetCtl;

///Gnb SwTjOffset
///
#define IdsNvGnbPmmGnbSwTjOffsetMin 0
#define IdsNvGnbPmmGnbSwTjOffsetMax 0xff

///Loadline Optimization
///Loadline Optimization
typedef enum {
  IdsNvGnbPmmLoadlineOptimizationDisabled = 0,///<Disabled
  IdsNvGnbPmmLoadlineOptimizationEnabled = 1,///<Enabled
  IdsNvGnbPmmLoadlineOptimizationAuto = 0xf,///<Auto
} IdsNvGnbPmmLoadlineOptimization;

///SMU SCLK Clock Gating
///SMU SCLK Clock Gating
typedef enum {
  IdsNvGnbPmmSMUSclkClockGatingDisabled = 0,///<Disabled
  IdsNvGnbPmmSMUSclkClockGatingEnabled = 1,///<Enabled
  IdsNvGnbPmmSMUSclkClockGatingAuto = 0xf,///<Auto
} IdsNvGnbPmmSMUSclkClockGating;

///SW Device ID Remapping
///SW Device ID Remapping
typedef enum {
  IdsNvGnbNbSWDeviceIDRemappingAuto = 0,///<Auto
  IdsNvGnbNbSWDeviceIDRemappingManual = 1,///<Manual
} IdsNvGnbNbSWDeviceIDRemapping;

///Hybrid Boost Control
///Hybrid Boost Control
typedef enum {
  IdsNvGnbPmmHybridBoostCtlDisabled = 0,///<Disabled
  IdsNvGnbPmmHybridBoostCtlEnabled = 1,///<Enabled
  IdsNvGnbPmmHybridBoostCtlAuto = 0xf,///<Auto
} IdsNvGnbPmmHybridBoostCtl;

///Hybrid Boost Threshold hyst Control
///Hybrid Boost Threshold hyst Control, "Hybrid Boost Threshold hyst Control" should be "Auto" if "Hybrid Boost Scalar Control" is "Auto"
typedef enum {
  IdsNvGnbPmmHybridBoostThresholdhystCtlAuto = 0,///<Auto
  IdsNvGnbPmmHybridBoostThresholdhystCtlManual = 1,///<Manual
} IdsNvGnbPmmHybridBoostThresholdhystCtl;

///Hybrid Boost Threshold hyst
///Hybrid Boost Threshold hyst
#define IdsNvGnbPmmHybridBoostThresholdhystMin 0
#define IdsNvGnbPmmHybridBoostThresholdhystMax 63

///Hybrid Boost Threshold offset Control
///Hybrid Boost Threshold offset Control, "Hybrid Boost Threshold offset Control" should be "Auto" if "Hybrid Boost Scalar Control" is "Auto"
typedef enum {
  IdsNvGnbPmmHybridBoostThresholdoffsetCtlAuto = 0,///<Auto
  IdsNvGnbPmmHybridBoostThresholdoffsetCtlManual = 1,///<Manual
} IdsNvGnbPmmHybridBoostThresholdoffsetCtl;

///Hybrid Boost Threshold offset
///Hybrid Boost Threshold offset, Sets the hybrid boost threshold temperature to (Tjmax - (user input/4)). Allowable values: 0 to 63
#define IdsNvGnbPmmHybridBoostThresholdoffsetMin 0
#define IdsNvGnbPmmHybridBoostThresholdoffsetMax 0xff

///Hybrid Boost Scalar Control
///Hybrid Boost Scalar Control
typedef enum {
  IdsNvGnbPmmHybridBoostScalarCtlAuto = 0,///<Auto
  IdsNvGnbPmmHybridBoostScalarCtlManual = 1,///<Manual
} IdsNvGnbPmmHybridBoostScalarCtl;

///Hybrid Boost Scalar
///Hybrid Boost Scalar, Sets the hybrid boost power scalar to (user input/255), Allowable values: 190 to 255
#define IdsNvGnbPmmHybridBoostScalarMin 190
#define IdsNvGnbPmmHybridBoostScalarMax 255

///PhyAdaptFomBer
///
typedef enum {
  IdsNvGnbPhyAdaptFomBerAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptFomBerManual = 1  ///<Manual
} IdsNvGnbPhyAdaptFomBer;

///PhyAdaptOffCalTime
///
typedef enum {
  IdsNvGnbPhyAdaptOffCalTimeAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptOffCalTimeManual = 1  ///<Manual
} IdsNvGnbPhyAdaptOffCalTime;

///PhyAdaptCDRTime
///
typedef enum {
  IdsNvGnbPhyAdaptCDRTimeAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptCDRTimeManual = 1  ///<Manual
} IdsNvGnbPhyAdaptCDRTime;

///PhyAdaptLEPhyAdaptLEQTime
///
typedef enum {
  IdsNvGnbPhyAdaptLEQTimeAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptLEQTimeManual = 1  ///<Manual
} IdsNvGnbPhyAdaptLEQTime;

///PhyAdaptDFETime
///
typedef enum {
  IdsNvGnbPhyAdaptDFETimeAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptDFETimeManual = 1  ///<Manual
} IdsNvGnbPhyAdaptDFETime;

///PhyAdaptFOMTime
///
typedef enum {
  IdsNvGnbPhyAdaptFOMTimeAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptFOMTimeManual = 1  ///<Manual
} IdsNvGnbPhyAdaptFOMTime;

///PhyAdaptPiOffsetbypVal
///
typedef enum {
  IdsNvGnbPhyAdaptPiOffsetbypValAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptPiOffsetbypValManual = 1  ///<Manual
} IdsNvGnbPhyAdaptPiOffsetbypVal;

///PhyAdaptPiOffsetBypEn
///
typedef enum {
  IdsNvGnbPhyAdaptPiOffsetBypEnAuto = 0,      ///<Auto
  IdsNvGnbPhyAdaptPiOffsetBypEnDisabled,      ///<Disabled
  IdsNvGnbPhyAdaptPiOffsetBypEnEnabled        ///<Enabled
} IdsNvGnbPhyAdaptPiOffsetBypEn;

///PhyAdaptThLoopGain
///
typedef enum {
  IdsNvGnbPhyAdaptThLoopGainAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptThLoopGainManual = 1  ///<Manual
} IdsNvGnbPhyAdaptThLoopGain;

///PhyAdaptLEQLoopGain
///
typedef enum {
  IdsNvGnbPhyAdaptLEQLoopGainAuto = 0,   ///<Auto
  IdsNvGnbPhyAdaptLEQLoopGainManual = 1  ///<Manual
} IdsNvGnbPhyAdaptLEQLoopGain;

///PhyRxAdaptTrack
///
typedef enum {
  IdsNvGnbPhyRxAdaptTrackAuto = 0,   ///<Auto
  IdsNvGnbPhyRxAdaptTrackManual = 1  ///<Manual
} IdsNvGnbPhyRxAdaptTrack;

///PhyTxDiff
///
typedef enum {
  IdsNvGnbPhyTxDiffAuto = 0,   ///<Auto
  IdsNvGnbPhyTxDiffManual = 1  ///<Manual
} IdsNvGnbPhyTxDiff;

///PhyDfrTimerCtl
///
typedef enum {
  IdsNvGnbPhyDfrTimerCtlAuto = 0,   ///<Auto
  IdsNvGnbPhyDfrTimerCtlManual = 1  ///<Manual
} IdsNvGnbPhyDfrTimerCtl;

///PhyGen12PiOffsetBypVal
///
typedef enum {
  IdsNvGnbPhyGen12PiOffsetBypValAuto = 0,   ///<Auto
  IdsNvGnbPhyGen12PiOffsetBypValManual = 1  ///<Manual
} IdsNvGnbPhyGen12PiOffsetBypVal;

///PhyGen12PiOffsetBypEn
///
typedef enum {
  IdsNvGnbPhyGen12PiOffsetBypEnAuto = 0,  ///<Auto
  IdsNvGnbPhyGen12PiOffsetBypEnDisabled,  ///<Disabled
  IdsNvGnbPhyGen12PiOffsetBypEnEnabled    ///<Enabled
} IdsNvGnbPhyGen12PiOffsetBypEn;

///PhyAnalogWaitTime
///
typedef enum {
  IdsNvGnbPhyAnalogWaitTimeAuto = 0,   ///<Auto
  IdsNvGnbPhyAnalogWaitTimeManual = 1  ///<Manual
} IdsNvGnbPhyAnalogWaitTime;

///PhyPhaseGainGen12Sel
///
typedef enum {
  IdsNvGnbPhyPhaseGainGen12SelAuto = 0,   ///<Auto
  IdsNvGnbPhyPhaseGainGen12SelManual = 1  ///<Manual
} IdsNvGnbPhyPhaseGainGen12Sel;

///PhyPhaseGainGen3Sel
///
typedef enum {
  IdsNvGnbPhyPhaseGainGen3SelAuto = 0,   ///<Auto
  IdsNvGnbPhyPhaseGainGen3SelManual = 1  ///<Manual
} IdsNvGnbPhyPhaseGainGen3Sel;

///ACP Power Gating
typedef enum {
  IdsNvGnbAcpPowerGatingDisabled = 0,///<Disabled
  IdsNvGnbAcpPowerGatingEnabled = 1,///<Enabled
  IdsNvGnbAcpPowerGatingAuto = 0xf,///<Auto
} IdsNvGnbAcpPowerGating;

///DSM low power
///
typedef enum {
  IdsNvDsmLpDisabled = 0,                 ///<Disabled
  IdsNvDsmLpEnabled = 1,                  ///<Enabled
  IdsNvDsmLpAuto = 3                      ///<Auto
} IdsNvDsmLpSelect;

///Gen1 loop back Mode
typedef enum {
  IdsNvGnbGen1LoopbackDisabled = 0,///<Disabled
  IdsNvGnbGen1LoopbackEnabled = 1,///<Enabled
  IdsNvGnbGen1LoopbackAuto = 0xf,///<Auto
} IdsNvGnbGen1Loopback;

#endif // _IDSINTF15StNVDEF_H_

