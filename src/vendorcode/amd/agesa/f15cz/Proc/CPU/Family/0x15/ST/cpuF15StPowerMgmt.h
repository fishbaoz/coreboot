/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 15h Stoney Power Management related registers defination
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/ST
 * @e \$Revision: 312756 $   @e \$Date: 2015-02-11 11:00:50 +0800 (Wed, 11 Feb 2015) $
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

#ifndef _CPU_F15_ST_POWERMGMT_H_
#define _CPU_F15_ST_POWERMGMT_H_

/*
 * Family 15h Stoney CPU Power Management MSR definitions
 *
 */

/// NB Machine Check Misc 0, 0x00000403
typedef struct {
  UINT64 :24;                        ///< Reserved
  UINT64 BlkPtr:8;                   ///< Block pointer for additional MISC registers
  UINT64 ErrCnt:12;                  ///< Error counter
  UINT64 :4;                         ///< Reserved
  UINT64 Ovrflw:1;                   ///< Overflow
  UINT64 IntType:2;                  ///< Interrupt type
  UINT64 CntEn:1;                    ///< Counter enable
  UINT64 LvtOffset:4;                ///< LVT offset
  UINT64 :4;                         ///< Reserved
  UINT64 IntP:1;                     ///< Intp
  UINT64 Locked:1;                   ///< Locked
  UINT64 CntP:1;                     ///< Counter present
  UINT64 Valid:1;                    ///< Valid
} MC0_MISC_MSR;

/* P-state Registers 0xC00100[6B:64] */

/// P-state MSR
typedef struct {
  UINT64 CpuFid_5_0:6;               ///< CpuFid[5:0]
  UINT64 CpuDid:3;                   ///< CpuDid
  UINT64 CpuVid:8;                   ///< CpuVid
  UINT64 :5;                         ///< Reserved
  UINT64 NbPstate:1;                 ///< NbPstate
  UINT64 :9;                         ///< Reserved
  UINT64 IddValue:8;                 ///< IddValue
  UINT64 IddDiv:2;                   ///< IddDiv
  UINT64 :21;                        ///< Reserved
  UINT64 PsEnable:1;                 ///< Pstate Enable
} PSTATE_MSR;

#define GetF15StCpuVid(PstateMsr) (((PSTATE_MSR *) PstateMsr)->CpuVid)

#define PSTATE_MSR_CpuFid_5_0_OFFSET      0
#define PSTATE_MSR_CpuFid_5_0_WIDTH       6
#define PSTATE_MSR_CpuFid_5_0_MASK        0x0000003Ful

#define GetF15StCpuFid(PstateMsr) ( \
  ((PSTATE_MSR *) PstateMsr)->CpuFid_5_0)


#define SetF15StCpuFid(PstateMsr, NewCpuFid) { \
  ((PSTATE_MSR *) PstateMsr)->CpuFid_5_0 = NewCpuFid & ((1 << PSTATE_MSR_CpuFid_5_0_WIDTH) - 1); \
}


/* VID operation related macros */
#define ConvertVidInuV(Vid)   (1550000 - (6250 * Vid)) ///< Convert VID in uV.

/* COFVID Control Register 0xC0010070 */
#define MSR_COFVID_CTL 0xC0010070ul

/// COFVID Control MSR Register
typedef struct {
  UINT64 CpuFid_5_0:6;               ///< CpuFid[5:0]
  UINT64 CpuDid:3;                   ///< CpuDid
  UINT64 CpuVid_6_0:7;               ///< CpuVid[6:0]
  UINT64 PstateId:3;                 ///< Pstate ID
  UINT64 :1;                         ///< Reserved
  UINT64 CpuVid_7:1;                 ///< CpuVid[7]
  UINT64 :1;                         ///< Reserved
  UINT64 NbPstate:1;                 ///< Northbridge P-state
  UINT64 :1;                         ///< Reserved
  UINT64 NbVid:8;                    ///< NbVid
  UINT64 :32;                        ///< Reserved
} COFVID_CTRL_MSR;

#define COFVID_CTRL_MSR_CurCpuVid_6_0_OFFSET       9
#define COFVID_CTRL_MSR_CurCpuVid_6_0_WIDTH        7
#define COFVID_CTRL_MSR_CurCpuVid_6_0_MASK         0xFE00
#define COFVID_CTRL_MSR_CurCpuVid_7_OFFSET         20
#define COFVID_CTRL_MSR_CurCpuVid_7_WIDTH          1
#define COFVID_CTRL_MSR_CurCpuVid_7_MASK           0x100000ul

/* SVI VID Encoding */

///< Union structure of VID in SVI1/SVI2 modes
typedef union {
  UINT32     RawVid;                 ///< Raw VID value
  struct {                           ///< SVI2 mode VID structure
    UINT32   Vid_6_0:7;              ///< Vid[6:0] of SVI2 mode
    UINT32   Vid_7:1;                ///< Vid[7] of SVI2 mode
  } SVI2;
  struct {                           ///< SVI1 mode VID structure
    UINT32   Vid_LSB_Ignore:1;       ///< Ignored LSB of 8bit VID encoding in SVI1 mode
    UINT32   Vid_6_0:1;              ///< Vid[6:0] of SVI mode
  } SVI1;
} SVI_VID;


#define SetF15StCpuVid(CofVidCtrlMsr, NewCpuVid) ( \
  ((COFVID_CTRL_MSR *) CofVidCtrlMsr)->CurCpuVid_6_0) = ((SVI_VID *) NewCpuVid)->SVI2.Vid_6_0; \
  ((COFVID_CTRL_MSR *) CofVidCtrlMsr)->CurCpuVid_7) = ((SVI_VID *) NewCpuVid)->SVI2.Vid_7; \
)


/* COFVID Status Register 0xC0010071 */
#define MSR_COFVID_STS 0xC0010071ul

/// COFVID Status MSR Register
typedef struct {
  UINT64 CurCpuFid:6;                ///< Current CpuFid
  UINT64 CurCpuDid:3;                ///< Current CpuDid
  UINT64 CurCpuVid_6_0:7;            ///< Current CpuVid[6:0]
  UINT64 CurPstate:3;                ///< Current Pstate
  UINT64 :1;                         ///< Reserved
  UINT64 CurCpuVid_7:1;              ///< Current CpuVid[7]
  UINT64 :2;                         ///< Reserved
  UINT64 NbPstateDis:1;              ///< NbPstate Disable
  UINT64 CurNbVid:8;                 ///< Current NbVid[7:0]
  UINT64 StartupPstate:3;            ///< Startup Pstate
  UINT64 :14;                        ///< Reserved
  UINT64 MaxCpuCof:6;                ///< MaxCpuCof
  UINT64 :1;                         ///< Reserved
  UINT64 CurPstateLimit:3;           ///< Current Pstate Limit
  UINT64 MaxNbCof:5;                 ///< MaxNbCof
} COFVID_STS_MSR;

#define GetF15StCurCpuFid(CofVidStsMsr) ( \
  ((COFVID_STS_MSR *) CofVidStsMsr)->CurCpuFid)

#define COFVID_STS_MSR_CurCpuVid_6_0_OFFSET       9
#define COFVID_STS_MSR_CurCpuVid_6_0_WIDTH        7
#define COFVID_STS_MSR_CurCpuVid_6_0_MASK         0xFE00
#define COFVID_STS_MSR_CurCpuVid_7_OFFSET         20
#define COFVID_STS_MSR_CurCpuVid_7_WIDTH          1
#define COFVID_STS_MSR_CurCpuVid_7_MASK           0x100000ul

#define GetF15StCurCpuVid(CofVidStsMsr) ( \
  (((COFVID_STS_MSR *) CofVidStsMsr)->CurCpuVid_7 << COFVID_STS_MSR_CurCpuVid_6_0_WIDTH) \
  | ((COFVID_STS_MSR *) CofVidStsMsr)->CurCpuVid_6_0)


/* Floating Point Configuration Register 0xC0011028 */
#define MSR_FP_CFG 0xC0011028ul

/// Floating Point Configuration MSR Register
typedef struct {
  UINT64 :16;                        ///< Reserved
  UINT64 DiDtMode:1;                 ///< Di/Dt Mode
  UINT64 :1;                         ///< Reserved
  UINT64 DiDtCfg0:5;                 ///< Di/Dt Config 0
  UINT64 :2;                         ///< Reserved
  UINT64 DiDtCfg2:2;                 ///< Di/Dt Config 2
  UINT64 DiDtCfg1:8;                 ///< Di/Dt Config 1
  UINT64 :5;                         ///< Reserved
  UINT64 DiDtCfg3:1;                 ///< Di/Dt Config 3
  UINT64 DiDtCfg4:4;                 ///< Di/Dt Config 4
  UINT64 :19;                        ///< Reserved
} FP_CFG_MSR;


/*
 * Family 15h Stoney CPU Power Management PCI definitions
 *
 */

/* Clock Power/Timing Control 2 Register F3xDC */
#define CPTC2_REG 0xDC
#define CPTC2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_3, CPTC2_REG))

/// Clock Power Timing Control 2 PCI Register
typedef struct {
  UINT32 :7;                         ///< Reserved
  UINT32 FastSprSaveRestEn:1;        ///< FastSprSaveRestEn
  UINT32 HwPstateMaxVal:3;           ///< HW P-state maximum value
  UINT32 :1;                         ///< Reserved
  UINT32 NbsynPtrAdj:3;              ///< NB/Core sync FIFO ptr adjust
  UINT32 NbsynPtrAdjPstate_0:1;      ///< NB/core synchronization FIFO pointer adjust P-state[0]
  UINT32 CacheFlushOnHaltCtl:3;      ///< Cache flush on halt control
  UINT32 CacheFlushOnHaltTmr:7;      ///< Cache flush on halt timer
  UINT32 IgnCpuPrbEn:1;              ///< ignore CPU probe enable
  UINT32 NbsynPtrAdjLo:3;            ///< NB/core synchronization FIFO pointer adjust low
  UINT32 NbsynPtrAdjPstate_2_1:2;    ///< NB/core synchronization FIFO pointer adjust P-state[2:1]
} F15_ST_CLK_PWR_TIMING_CTRL2_REGISTER;

/* Northbridge Capabilities Register F3xE8 */
#define NB_CAPS_REG 0xE8
#define NB_CAPS_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_3, NB_CAPS_REG))

/// Northbridge Capabilities PCI Register
typedef struct {
  UINT32 :1;                         ///< Reserved
  UINT32 DualNode:1;                 ///< Dual-node multi-processor capable
  UINT32 EightNode:1;                ///< Eight-node multi-processor capable
  UINT32 Ecc:1;                      ///< ECC capable
  UINT32 Chipkill:1;                 ///< Chipkill ECC capable
  UINT32 :3;                         ///< Reserved
  UINT32 MctCap:1;                   ///< Memory controller capable
  UINT32 SvmCapable:1;               ///< SVM capable
  UINT32 HtcCapable:1;               ///< HTC capable
  UINT32 :3;                         ///< Reserved
  UINT32 MultVidPlane:1;             ///< Multiple VID plane capable
  UINT32 :4;                         ///< Reserved
  UINT32 x2Apic:1;                   ///< x2Apic capability
  UINT32 :4;                         ///< Reserved
  UINT32 MemPstateCap:1;             ///< Memory P-state capable
  UINT32 :3;                         ///< Reserved
  UINT32 SUCCOR:1;                   ///< SUCCOR
  UINT32 :3;                         ///< Reserved
} NB_CAPS_REGISTER;

/* C-state Control 1 Register D18F4x118 */
#define CSTATE_CTRL1_REG 0x118
#define CSTATE_CTRL1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_4, CSTATE_CTRL1_REG))

/// C-state Control 1 Register
typedef struct {
  UINT32 CpuPrbEnCstAct0:1;          ///< Core direct probe enable
  UINT32 CacheFlushEnCstAct0:1;      ///< Cache flush enable
  UINT32 CacheFlushTmrSelCstAct0:2;  ///< Cache flush timer select
  UINT32 :1;                         ///< Reserved
  UINT32 ClkDivisorCstAct0:3;        ///< Clock divisor
  UINT32 PwrGateEnCstAct0:1;         ///< Power gate enable
  UINT32 PwrOffEnCstAct0:1;          ///< C-state action field 3
  UINT32 NbPwrGate0:1;               ///< NB power-gating 0
  UINT32 NbClkGate0:1;               ///< NB clock-gating 0
  UINT32 SelfRefr0:1;                ///< Self-refresh 0
  UINT32 SelfRefrEarly0:1;           ///< Allow early self-refresh 0
  UINT32 :2;                         ///< Reserved
  UINT32 CpuPrbEnCstAct1:1;          ///< Core direct probe enable
  UINT32 CacheFlushEnCstAct1:1;      ///< Cache flush eable
  UINT32 CacheFlushTmrSelCstAct1:2;  ///< Cache flush timer select
  UINT32 :1;                         ///< Reserved
  UINT32 ClkDivisorCstAct1:3;        ///< Clock divisor
  UINT32 PwrGateEnCstAct1:1;         ///< Power gate enable
  UINT32 PwrOffEnCstAct1:1;          ///< C-state action field 3
  UINT32 NbPwrGate1:1;               ///< NB power-gating 1
  UINT32 NbClkGate1:1;               ///< NB clock-gating 1
  UINT32 SelfRefr1:1;                ///< Self-refresh 1
  UINT32 SelfRefrEarly1:1;           ///< Allow early self-refresh 1
  UINT32 :2;                         ///< Reserved
} CSTATE_CTRL1_REGISTER;


/* C-state Control 2 Register D18F4x11C */
#define CSTATE_CTRL2_REG 0x11C
#define CSTATE_CTRL2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_4, CSTATE_CTRL2_REG))

/// C-state Control 2 Register
typedef struct {
  UINT32 CpuPrbEnCstAct2:1;          ///< Core direct probe enable
  UINT32 CacheFlushEnCstAct2:1;      ///< Cache flush eable
  UINT32 CacheFlushTmrSelCstAct2:2;  ///< Cache flush timer select
  UINT32 :1;                         ///< Reserved
  UINT32 ClkDivisorCstAct2:3;        ///< Clock divisor
  UINT32 PwrGateEnCstAct2:1;         ///< Power gate enable
  UINT32 PwrOffEnCstAct2:1;          ///< C-state action field 3
  UINT32 NbPwrGate2:1;               ///< NB power-gating 2
  UINT32 NbClkGate2:1;               ///< NB clock-gating 2
  UINT32 SelfRefr2:1;                ///< Self-refresh 2
  UINT32 SelfRefrEarly2:1;           ///< Allow early self-refresh 2
  UINT32 :18;                        ///< Reserved
} CSTATE_CTRL2_REGISTER;


/* Cstate Policy Control 1 Register D18F4x128 */
#define CSTATE_POLICY_CTRL1_REG 0x128
#define CSTATE_POLICY_CTRL1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_4, CSTATE_POLICY_CTRL1_REG))

/// Cstate Policy Control 1 Register
typedef struct {
  UINT32 CoreCStateMode:1;            ///< Specifies C-State actions
  UINT32 CoreCstatePolicy:1;          ///< Specified processor arbitration of voltage and frequency
  UINT32 HaltCstateIndex:3;           ///< Specifies the IO-based C-state that is invoked by a HLT instruction
  UINT32 CacheFlushTmr:7;             ///< Cache flush timer
  UINT32 :6;                          ///< Reserved
  UINT32 CacheFlushSucMonThreshold:3; ///< Cache flush success monitor threshold
  UINT32 CacheFlushSucMonTmrSel:2;    ///< Cache flush success monitor timer select
  UINT32 CacheFlushSucMonMispredictAct:2; ///< Cache flush success monitor mispredict action
  UINT32 :6;                          ///< Reserved
  UINT32 CstateMsgDis:1;              ///< C-state messaging disable
} CSTATE_POLICY_CTRL1_REGISTER;


/* Core Performance Boost Control Register D18F4x15C */

/// Core Performance Boost Control Register
typedef struct {
  UINT32 BoostSrc:2;                 ///< Boost source
  UINT32 NumBoostStates:3;           ///< Number of boosted states
  UINT32 :2;                         ///< Reserved
  UINT32 ApmMasterEn:1;              ///< APM master enable
  UINT32 CstatePowerEn:1;            ///< C-state power enable
  UINT32 LpmOnVidNop:1;              ///< LpmOnVidNop
  UINT32 LpmTrigger:1;               ///< LpmTrigger
  UINT32 :20;                        ///< Reserved
  UINT32 BoostLock:1;                ///< BoostLock
} CPB_CTRL_REGISTER;


/* Northbridge Capabilities 2  F5x84*/
#define NB_CAPS_REG2 0x84
#define NB_CAPS_REG2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_CAPS_REG2))

/// Northbridge Capabilities 2 PCI Register
typedef struct {
  UINT32 CmpCap:8;                   ///< CMP capable
  UINT32 AddressBasedDramErrInj:1;   ///< AddressBasedDramErrInj
  UINT32 DramErrInformation:1;       ///< DramErrInformation
  UINT32 :2;                         ///< Reserved
  UINT32 DctEn:4;                    ///< DCT enabled
  UINT32 DdrMaxRate:5;               ///< maximum DDR rate
  UINT32 :3;                         ///< Reserved
  UINT32 DdrMaxRateEnf:5;            ///< enforced maximum DDR rate:
  UINT32 :3;                         ///< Reserved
} NB_CAPS_2_REGISTER;

/* Northbridge Configuration 4  F5x88*/
#define NB_CFG_REG4 0x88
#define NB_CFG_REG4_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_CFG_REG4))

/// Northbridge Configuration 4 PCI Register
typedef struct {
  UINT32 :14;                        ///< Reserved
  UINT32 DisHldRegRdRspChk:1;        ///< DisHldRegRdRspChk
  UINT32 :3;                         ///< Reserved
  UINT32 EnCstateBoostBlockCC6Exit:1;///< EnCstateBoostBlockCC6Exit
  UINT32 :5;                         ///< Reserved
  UINT32 DisHbNpReqBusLock:1;        ///< DisHbNpReqBusLock
  UINT32 :7;                         ///< Reserved
} NB_CFG_4_REGISTER;

/* Northbridge P-state [3:0] F5x1[6C:60]  */

/// Northbridge P-state Register
typedef struct {
  UINT32 NbPstateEn:1;               ///< NB P-state enable
  UINT32 NbFid_5_0:6;                ///< NB frequency ID[5:0]
  UINT32 NbDid:1;                    ///< NB divisor ID
  UINT32 :2;                         ///< Reserved
  UINT32 NbVid_6_0:7;                ///< NB VID[6:0]
  UINT32 :1;                         ///< Reserved
  UINT32 MemPstate:1;                ///< Memory P-State
  UINT32 :2;                         ///< Reserved
  UINT32 NbVid_7:1;                  ///< NB VID[7]
  UINT32 NbIddDiv:2;                 ///< northbridge current divisor
  UINT32 NbIddValue:8;               ///< northbridge current value
} NB_PSTATE_REGISTER;

#define NB_PSTATE_REGISTER_NbFid_5_0_OFFSET      1
#define NB_PSTATE_REGISTER_NbFid_5_0_WIDTH       6
#define NB_PSTATE_REGISTER_NbFid_5_0_MASK        0x0000007Eul

#define GetF15StNbFid(NbPstateRegister) ( \
  ((NB_PSTATE_REGISTER *) NbPstateRegister)->NbFid_5_0)

#define SetF15StNbFid(NbPstateRegister, NewNbFid) { \
  ((NB_PSTATE_REGISTER *) NbPstateRegister)->NbFid_5_0 = NewNbFid & ((1 << NB_PSTATE_REGISTER_NbFid_5_0_WIDTH) - 1); \
}

#define NB_PSTATE_REGISTER_NbVid_6_0_OFFSET      10
#define NB_PSTATE_REGISTER_NbVid_6_0_WIDTH       7
#define NB_PSTATE_REGISTER_NbVid_6_0_MASK        0x0001FC00ul
#define NB_PSTATE_REGISTER_NbVid_7_OFFSET        21
#define NB_PSTATE_REGISTER_NbVid_7_WIDTH         1
#define NB_PSTATE_REGISTER_NbVid_7_MASK          0x00200000ul

#define GetF15StNbVid(NbPstateRegister) ( \
  (((NB_PSTATE_REGISTER *) NbPstateRegister)->NbVid_7 << NB_PSTATE_REGISTER_NbVid_6_0_WIDTH) \
  | ((NB_PSTATE_REGISTER *) NbPstateRegister)->NbVid_6_0)

#define SetF15StNbVid(NbPstateRegister, NewNbVid) { \
  ((NB_PSTATE_REGISTER *) NbPstateRegister)->NbVid_6_0 = ((SVI_VID *) NewNbVid)->SVI2.Vid_6_0; \
  ((NB_PSTATE_REGISTER *) NbPstateRegister)->NbVid_7 = ((SVI_VID *) NewNbVid)->SVI2.Vid_7; \
}

/* Northbridge P-state Status */
#define NB_PSTATE_CTRL 0x170
#define NB_PSTATE_CTRL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_PSTATE_CTRL))

/// Northbridge P-state Control Register
typedef struct {
  UINT32 NbPstateMaxVal:2;           ///< NB P-state maximum value
  UINT32 :1;                         ///< Reserved
  UINT32 NbPstateLo:2;               ///< NB P-state low
  UINT32 :1;                         ///< Reserved
  UINT32 NbPstateHi:2;               ///< NB P-state high
  UINT32 :1;                         ///< Reserved
  UINT32 NbPstateThreshold:4;        ///< NB P-state threshold
  UINT32 NbPstateDisOnP0:1;          ///< NB P-state disable on P0
  UINT32 SwNbPstateLoDis:1;          ///< Software NB P-state low disable
  UINT32 :8;                         ///< Reserved
  UINT32 NbPstateGnbSlowDis:1;       ///< Disable NB P-state transition take GnbSlow into account.
  UINT32 NbPstateLoRes:3;            ///< NB P-state low residency timer
  UINT32 NbPstateHiRes:3;            ///< NB P-state high residency timer
  UINT32 NbPstateFidVidSbcEn:1;      ///< NbPstateFidVidSbcEn
  UINT32 MemPstateDis:1;             ///< Memory P-state disable
} NB_PSTATE_CTRL_REGISTER;


/* Northbridge P-state Status */
#define NB_PSTATE_STATUS 0x174
#define NB_PSTATE_STATUS_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_PSTATE_STATUS))

/// Northbridge P-state Status Register
typedef struct {
  UINT32 NbPstateDis:1;              ///< Nb pstate disable
  UINT32 StartupNbPstate:2;          ///< startup northbridge Pstate number
  UINT32 CurNbFid_5_0:6;             ///< Current NB FID[5:0]
  UINT32 CurNbDid:1;                 ///< Current NB DID
  UINT32 :2;                         ///< Reserved
  UINT32 CurNbVid_6_0:7;             ///< Current NB VID[6:0]
  UINT32 CurNbPstate:2;              ///< Current NB Pstate
  UINT32 :1;                         ///< Reserved
  UINT32 CurNbPstateLo:1;            ///< CurNbPstateLo
  UINT32 CurNbVid_7:1;               ///< Current NB VID[7]
  UINT32 CurMemPstate:1;             ///< Current memory P-state
  UINT32 :6;                         ///< Reserved
  UINT32 Reserved:1;                 ///< Reserved
} NB_PSTATE_STS_REGISTER;

#define NB_PSTATE_STS_REGISTER_CurNbFid_5_0_OFFSET      3
#define NB_PSTATE_STS_REGISTER_CurNbFid_5_0_WIDTH       6
#define NB_PSTATE_STS_REGISTER_CurNbFid_5_0_MASK        0x000001F8ul

#define GetF15StCurNbFid(NbPstateRegister) ( \
  ((NB_PSTATE_STS_REGISTER *) NbPstateRegister)->CurNbFid_5_0)

#define NB_PSTATE_STS_REGISTER_CurNbVid_6_0_OFFSET      12
#define NB_PSTATE_STS_REGISTER_CurNbVid_6_0_WIDTH       7
#define NB_PSTATE_STS_REGISTER_CurNbVid_6_0_MASK        0x0007F000ul
#define NB_PSTATE_STS_REGISTER_CurNbVid_7_OFFSET        23
#define NB_PSTATE_STS_REGISTER_CurNbVid_7_WIDTH         1
#define NB_PSTATE_STS_REGISTER_CurNbVid_7_MASK          0x00800000ul

#define GetF15StCurNbVid(NbPstateStsRegister) ( \
  (((NB_PSTATE_STS_REGISTER *) NbPstateStsRegister)->CurNbVid_7 << NB_PSTATE_STS_REGISTER_CurNbVid_6_0_WIDTH) \
  | ((NB_PSTATE_STS_REGISTER *) NbPstateStsRegister)->CurNbVid_6_0)

/* SMUSVI MISC VID STATUS D0F0xBC_xD823_0030 */
#define SMUSVI_MISC_VID_STATUS 0xD8230030ul

/// SMUSVI POWER CONTROL MISC Register
typedef struct {
  UINT32 MAX_VID:8;                   ///< MAX VID
  UINT32 :2;                          ///< Reserved
  UINT32 MIN_VID:8;                   ///< MIN VID
  UINT32 :5;                          ///< Reserved
  UINT32 NB_PSI_VID:8;                ///< NB PSI VID
  UINT32 NB_PSI_VID_EN:1;             ///< NB PSI VID EN
} SMUSVI_MISC_VID_STATUS_REGISTER;

/* SMUSVI POWER CONTROL MISC D0F0xBC_xD823_0034 */
#define SMUSVI_POWER_CONTROL_MISC 0xD8230034ul

/// SMUSVI POWER CONTROL MISC Register
typedef struct {
  UINT32 PSIVID:7;                   ///< PSI VID
  UINT32 PSIVIDEN:1;                 ///< PSI VID EN
  UINT32 PSIVID_HI:1;                ///< PSI VID HI
  UINT32 SVIHIGHFREQSEL:1;           ///< SVIHIGHFREQSEL
  UINT32 :4;                         ///< Reserved
  UINT32 SVI2HIGHFREQSEL:1;          ///< SVI2HIGHFREQSEL
  UINT32 :17;                        ///< Reserved
} SMUSVI_POWER_CONTROL_MISC_REGISTER;

#endif /* _CPU_F15_ST_POWERMGMT_H_ */
