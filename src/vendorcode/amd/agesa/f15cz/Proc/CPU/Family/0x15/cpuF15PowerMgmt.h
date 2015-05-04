/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Power Management related registers defination
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15
 * @e \$Revision: 314610 $   @e \$Date: 2015-03-11 23:48:42 -0500 (Wed, 11 Mar 2015) $
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

#ifndef _CPUF15POWERMGMT_H_
#define _CPUF15POWERMGMT_H_

/*
 * Family 15h CPU Power Management MSR definitions
 *
 */

/* P-state Current Limit Register 0xC0010061 */
#define MSR_PSTATE_CURRENT_LIMIT 0xC0010061ul // F15 Shared

/// Pstate Current Limit MSR Register
typedef struct {
  UINT64 CurPstateLimit:3;           ///< Current Pstate Limit
  UINT64 :1;                         ///< Reserved
  UINT64 PstateMaxVal:3;             ///< Pstate Max Value
  UINT64 :57;                        ///< Reserved
} PSTATE_CURLIM_MSR;


/* P-state Control Register 0xC0010062 */
#define MSR_PSTATE_CTL 0xC0010062ul    // F15 Shared

/// Pstate Control MSR Register
typedef struct {
  UINT64 PstateCmd:3;                ///< Pstate change command
  UINT64 :61;                        ///< Reserved
} PSTATE_CTRL_MSR;


/* P-state Status Register 0xC0010063 */
#define MSR_PSTATE_STS 0xC0010063ul

/// Pstate Status MSR Register
typedef struct {
  UINT64 CurPstate:3;                ///< Current Pstate
  UINT64 :61;                        ///< Reserved
} PSTATE_STS_MSR;


/* P-state Registers 0xC001006[B:4] */
#define MSR_PSTATE_0 0xC0010064ul
#define MSR_PSTATE_1 0xC0010065ul
#define MSR_PSTATE_2 0xC0010066ul
#define MSR_PSTATE_3 0xC0010067ul
#define MSR_PSTATE_4 0xC0010068ul
#define MSR_PSTATE_5 0xC0010069ul
#define MSR_PSTATE_6 0xC001006Aul
#define MSR_PSTATE_7 0xC001006Bul

#define PS_REG_BASE MSR_PSTATE_0     /* P-state Register base */
#define PS_MAX_REG  MSR_PSTATE_7     /* Maximum P-State Register */
#define PS_MIN_REG  MSR_PSTATE_0     /* Minimum P-State Register */
#define NM_PS_REG 8                  /* number of P-state MSR registers */

/// P-state MSR with common field
typedef struct {
  UINT64 :63;                        ///< Other Pstate bitfields
  UINT64 PsEnable:1;                 ///< Pstate Enable
} F15_PSTATE_MSR;


/* C-state Address Register 0xC0010073 */
#define MSR_CSTATE_ADDRESS 0xC0010073ul

/// C-state Address MSR Register
typedef struct {
  UINT64 CstateAddr:16;              ///< C-state address
  UINT64 :48;                        ///< Reserved
} CSTATE_ADDRESS_MSR;


/*
 * Family 15h CPU Power Management PCI definitions
 *
 */

/* Hardware thermal control register */
#define HTC_REG 0xD8200C64ul

/// Hardware Thermal Control PCI Register
typedef struct {
  UINT32 HtcEn:1;                    ///< HTC Enable
  UINT32 :3;                         ///< Reserved
  UINT32 HtcAct:1;                   ///< HTC Active State
  UINT32 HtcActLog:1;                ///< HTC Active Log
  UINT32 HtcApicHiEn:1;              ///< P-state limit higher APIC interrupt enable
  UINT32 HtcApicLoEn:1;              ///< P-state limit lower APIC interrupt enable
  UINT32 :2;                         ///< Reserved
  UINT32 HtcToGnbEn:1;               ///< HtcToGnbEn
  UINT32 ProcHotToGnbEn:1;           ///< ProcHotToGnbEn
  UINT32 :4;                         ///< Reserved
  UINT32 HtcTmpLmt:7;                ///< HTC temperature limit
  UINT32 HtcSlewSel:1;               ///< HTC slew-controlled temp select
  UINT32 HtcHystLmt:4;               ///< HTC hysteresis
  UINT32 HtcPstateLimit:3;           ///< HTC P-state limit select
  UINT32 :1;                         ///< Reserved
} HTC_REGISTER;


/* Software P-state limit register F3x68 */
#define SW_PS_LIMIT_REG 0x68
#define SW_PS_LIMIT_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_3, SW_PS_LIMIT_REG))

/// Software P-state Limit PCI Register
typedef struct {
  UINT32 :5;                         ///< Reserved
  UINT32 SwPstateLimitEn:1;          ///< Software P-state limit enable
  UINT32 :22;                        ///< Reserved
  UINT32 SwPstateLimit:3;            ///< HTC P-state limit select
  UINT32 :1;                         ///< Reserved
} SW_PS_LIMIT_REGISTER;

/* Popup P-state Register F3xA8 */
#define POPUP_PSTATE_REG 0xA8
#define POPUP_PSTATE_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_3, POPUP_PSTATE_REG))

/// Popup P-state Register
typedef struct {
  UINT32 :29;                        ///< Reserved
  UINT32 PopDownPstate:3;            ///< PopDownPstate
} POPUP_PSTATE_REGISTER;

/* Core Performance Boost Control Register D18F4x15C */
#define CPB_CTRL_REG 0x15C
#define CPB_CTRL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_4, CPB_CTRL_REG))

/// Core Performance Boost Control Register of Family 15h common aceess
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
} F15_CPB_CTRL_REGISTER;

#define NM_NB_PS_REG 4               /* Number of NB P-state registers */

/* Northbridge P-state */
#define NB_PSTATE_0 0x160
#define NB_PSTATE_0_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_PSTATE_0))

#define NB_PSTATE_1 0x164
#define NB_PSTATE_1_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_PSTATE_1))

#define NB_PSTATE_2 0x168
#define NB_PSTATE_2_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_PSTATE_2))

#define NB_PSTATE_3 0x16C
#define NB_PSTATE_3_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, NB_PSTATE_3))

/* Northbridge P-state Status */
#define F15_NB_PSTATE_CTRL 0x170
#define F15_NB_PSTATE_CTRL_PCI_ADDR (MAKE_SBDFO (0, 0, 0x18, FUNC_5, F15_NB_PSTATE_CTRL))

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
} F15_NB_PSTATE_CTRL_REGISTER;


#endif /* _CPUF15POWERMGMT_H */
