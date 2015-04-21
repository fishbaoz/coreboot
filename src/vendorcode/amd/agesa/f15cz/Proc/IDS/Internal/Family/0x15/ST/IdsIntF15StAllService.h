/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD IDS Routines
 *
 * Contains AMD AGESA IDS Translation
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
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
#ifndef _IDS_INT_F15_ST_ALLSERVICE_H_
#define _IDS_INT_F15_ST_ALLSERVICE_H_
//#include "cpuF15PowerMgmt.h"
#ifndef NM_PS_REG
 #define  NM_PS_REG  8
#endif
#include "mm.h"
#include "mn.h"

#define IDS_INITIAL_F15_ST_PM_STEP {0, IdsIntPmPwrInitF15St},
#define IDS_F15_ST_PM_CUSTOM_STEP  {0, IdsIntPmCustomizeF15St},
#define BF_DQS_PROC_ODT           BFReserved01
#define BFOppWrEnhDis             BFReserved02
#define BFDisHalfNclkPwrGate      BFReserved03

/// Register Setting for One P-State
///
/// This data structure defines the register setting are specific to P-State.
///
typedef struct _PSTATE_REG {
  UINT8 corefid;                        ///< COREFID
  UINT8 corevid;                        ///< COREVID
  UINT8 coredid;                        ///< COREDID
} PSTATE_REG;

/// Custom P-State Definition Structure
///
/// This data structure defines entries that are specific to the custom P-State initialization.
///
typedef struct _CUSTOM_PSTATE_STRUCT {
  UINT8 psnum;                              ///< The number of P_State
  UINT8 bpsnum;                             ///< The number of Boost P_State
  PSTATE_REG pstatereg[NM_PS_REG];  ///< The P_State Register Setting
} CUSTOM_PSTATE_STRUCT;

VOID
IdsIntPmPwrInitF15St (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
IdsIntPmCustomizeF15St (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
IemEarlyDeviceInitD3ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
IemEarlyDeviceInitD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
IemBeforePmuSramMsgBlockWriteST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
IemLateDctConfigD3ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
IemLateDctConfigD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
IemLateMaxRdLatD3ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

VOID
IemLateMaxRdLatD4ST (
  IN OUT   MEM_NB_BLOCK *NBPtr
  );

// **** D18F3xD8 Register Definition ****
// Address
#define D18F3xD8_ADDRESS                                        0xd8

// Type
#define D18F3xD8_TYPE                                           TYPE_D18F3
// Field Data
#define D18F3xD8_VSSlamTime_OFFSET                              0
#define D18F3xD8_VSSlamTime_WIDTH                               3
#define D18F3xD8_VSSlamTime_MASK                                0x7
#define D18F3xD8_Reserved_3_3_OFFSET                            3
#define D18F3xD8_Reserved_3_3_WIDTH                             1
#define D18F3xD8_Reserved_3_3_MASK                              0x8
#define D18F3xD8_VSRampSlamTime_OFFSET                          4
#define D18F3xD8_VSRampSlamTime_WIDTH                           3
#define D18F3xD8_VSRampSlamTime_MASK                            0x70
#define D18F3xD8_Reserved_7_7_OFFSET                            7
#define D18F3xD8_Reserved_7_7_WIDTH                             1
#define D18F3xD8_Reserved_7_7_MASK                              0x80
#define D18F3xD8_NbAltVid_OFFSET                                8
#define D18F3xD8_NbAltVid_WIDTH                                 7
#define D18F3xD8_NbAltVid_MASK                                  0x7f00
#define D18F3xD8_NbAltVidEn_OFFSET                              15
#define D18F3xD8_NbAltVidEn_WIDTH                               1
#define D18F3xD8_NbAltVidEn_MASK                                0x8000
#define D18F3xD8_AltVidStart_OFFSET                             16
#define D18F3xD8_AltVidStart_WIDTH                              7
#define D18F3xD8_AltVidStart_MASK                               0x7f0000ul
#define D18F3xD8_Reserved_23_23_OFFSET                          23
#define D18F3xD8_Reserved_23_23_WIDTH                           1
#define D18F3xD8_Reserved_23_23_MASK                            0x800000ul
#define D18F3xD8_Reserved_27_24_OFFSET                          24
#define D18F3xD8_Reserved_27_24_WIDTH                           4
#define D18F3xD8_Reserved_27_24_MASK                            0xf000000ul
#define D18F3xD8_PwrPlanes_OFFSET                               28
#define D18F3xD8_PwrPlanes_WIDTH                                1
#define D18F3xD8_PwrPlanes_MASK                                 0x10000000ul
#define D18F3xD8_SlamModeSelect_OFFSET                          29
#define D18F3xD8_SlamModeSelect_WIDTH                           1
#define D18F3xD8_SlamModeSelect_MASK                            0x20000000ul
#define D18F3xD8_Reserved_30_30_OFFSET                          30
#define D18F3xD8_Reserved_30_30_WIDTH                           1
#define D18F3xD8_Reserved_30_30_MASK                            0x40000000ul
#define D18F3xD8_NbAltVidOnLdtStop_OFFSET                       31
#define D18F3xD8_NbAltVidOnLdtStop_WIDTH                        1
#define D18F3xD8_NbAltVidOnLdtStop_MASK                         0x80000000ul

/// D18F3xD8
typedef union {
  struct {                                                              ///<
    UINT32                                               VSSlamTime:3 ; ///<
    UINT32                                             Reserved_3_3:1 ; ///<
    UINT32                                           VSRampSlamTime:3 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                                 NbAltVid:7 ; ///<
    UINT32                                               NbAltVidEn:1 ; ///<
    UINT32                                              AltVidStart:7 ; ///<
    UINT32                                           Reserved_23_23:1 ; ///<
    UINT32                                           Reserved_27_24:4 ; ///<
    UINT32                                                PwrPlanes:1 ; ///<
    UINT32                                           SlamModeSelect:1 ; ///<
    UINT32                                           Reserved_30_30:1 ; ///<
    UINT32                                        NbAltVidOnLdtStop:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3xD8_STRUCT;

// **** D18F3xDC Register Definition ****
// Address
#define D18F3xDC_ADDRESS                                        0xdc

// Type
#define D18F3xDC_TYPE                                           TYPE_D18F3
// Field Data
#define D18F3xDC_AltVid_OFFSET                                  0
#define D18F3xDC_AltVid_WIDTH                                   7
#define D18F3xDC_AltVid_MASK                                    0x7f
#define D18F3xDC_Reserved_7_7_OFFSET                            7
#define D18F3xDC_Reserved_7_7_WIDTH                             1
#define D18F3xDC_Reserved_7_7_MASK                              0x80
#define D18F3xDC_PstateMaxVal_OFFSET                            8
#define D18F3xDC_PstateMaxVal_WIDTH                             3
#define D18F3xDC_PstateMaxVal_MASK                              0x700
#define D18F3xDC_DualSkipCClk_OFFSET                            11
#define D18F3xDC_DualSkipCClk_WIDTH                             1
#define D18F3xDC_DualSkipCClk_MASK                              0x800
#define D18F3xDC_NbsynPtrAdj_OFFSET                             12
#define D18F3xDC_NbsynPtrAdj_WIDTH                              3
#define D18F3xDC_NbsynPtrAdj_MASK                               0x7000
#define D18F3xDC_Reserved_15_15_OFFSET                          15
#define D18F3xDC_Reserved_15_15_WIDTH                           1
#define D18F3xDC_Reserved_15_15_MASK                            0x8000
#define D18F3xDC_CacheFlushOnHaltCtl_OFFSET                     16
#define D18F3xDC_CacheFlushOnHaltCtl_WIDTH                      3
#define D18F3xDC_CacheFlushOnHaltCtl_MASK                       0x70000ul
#define D18F3xDC_CacheFlushOnHaltTmr_OFFSET                     19
#define D18F3xDC_CacheFlushOnHaltTmr_WIDTH                      7
#define D18F3xDC_CacheFlushOnHaltTmr_MASK                       0x3f80000ul
#define D18F3xDC_IgnCpuPrbEn_OFFSET                             26
#define D18F3xDC_IgnCpuPrbEn_WIDTH                              1
#define D18F3xDC_IgnCpuPrbEn_MASK                               0x4000000ul
#define D18F3xDC_Reserved_31_27_OFFSET                          27
#define D18F3xDC_Reserved_31_27_WIDTH                           5
#define D18F3xDC_Reserved_31_27_MASK                            0xf8000000ul

/// D18F3xDC
typedef union {
  struct {                                                              ///<
    UINT32                                                   AltVid:7 ; ///<
    UINT32                                             Reserved_7_7:1 ; ///<
    UINT32                                             PstateMaxVal:3 ; ///<
    UINT32                                             DualSkipCClk:1 ; ///<
    UINT32                                              NbsynPtrAdj:3 ; ///<
    UINT32                                           Reserved_15_15:1 ; ///<
    UINT32                                      CacheFlushOnHaltCtl:3 ; ///<
    UINT32                                      CacheFlushOnHaltTmr:7 ; ///<
    UINT32                                              IgnCpuPrbEn:1 ; ///<
    UINT32                                           Reserved_31_27:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3xDC_STRUCT;

// **** D18F3x18C Register Definition ****
// Address
#define D18F3x18C_ADDRESS                                       0x18c

// Type
#define D18F3x18C_TYPE                                          TYPE_D18F3
// Field Data
#define D18F3x18C_DisLclSpqToSriPrbRsp_OFFSET                   0
#define D18F3x18C_DisLclSpqToSriPrbRsp_WIDTH                    1
#define D18F3x18C_DisLclSpqToSriPrbRsp_MASK                     0x1
#define D18F3x18C_DisDirImpUpdate_OFFSET                        1
#define D18F3x18C_DisDirImpUpdate_WIDTH                         1
#define D18F3x18C_DisDirImpUpdate_MASK                          0x2
#define D18F3x18C_DisDynEStateDataRet_OFFSET                    2
#define D18F3x18C_DisDynEStateDataRet_WIDTH                     1
#define D18F3x18C_DisDynEStateDataRet_MASK                      0x4
#define D18F3x18C_EnInvDirOnWrSz_OFFSET                         3
#define D18F3x18C_EnInvDirOnWrSz_WIDTH                          1
#define D18F3x18C_EnInvDirOnWrSz_MASK                           0x8
#define D18F3x18C_DisSrqReqCompOnWDT_OFFSET                     4
#define D18F3x18C_DisSrqReqCompOnWDT_WIDTH                      1
#define D18F3x18C_DisSrqReqCompOnWDT_MASK                       0x10
#define D18F3x18C_EnFidVidTargetBootCore_OFFSET                 5
#define D18F3x18C_EnFidVidTargetBootCore_WIDTH                  1
#define D18F3x18C_EnFidVidTargetBootCore_MASK                   0x20
#define D18F3x18C_Reserved_6_6_OFFSET                           6
#define D18F3x18C_Reserved_6_6_WIDTH                            1
#define D18F3x18C_Reserved_6_6_MASK                             0x40
#define D18F3x18C_DisWrSzUState_OFFSET                          7
#define D18F3x18C_DisWrSzUState_WIDTH                           1
#define D18F3x18C_DisWrSzUState_MASK                            0x80
#define D18F3x18C_DisPfcqBypass_OFFSET                          8
#define D18F3x18C_DisPfcqBypass_WIDTH                           1
#define D18F3x18C_DisPfcqBypass_MASK                            0x100
#define D18F3x18C_DisXdsDstLnkFastXfr_OFFSET                    9
#define D18F3x18C_DisXdsDstLnkFastXfr_WIDTH                     1
#define D18F3x18C_DisXdsDstLnkFastXfr_MASK                      0x200
#define D18F3x18C_DisEarlyDirRsp_OFFSET                         10
#define D18F3x18C_DisEarlyDirRsp_WIDTH                          1
#define D18F3x18C_DisEarlyDirRsp_MASK                           0x400
#define D18F3x18C_MTC1eHaltBrdcstToCht_OFFSET                   11
#define D18F3x18C_MTC1eHaltBrdcstToCht_WIDTH                    1
#define D18F3x18C_MTC1eHaltBrdcstToCht_MASK                     0x800
#define D18F3x18C_DisChkIntPendState_OFFSET                     12
#define D18F3x18C_DisChkIntPendState_WIDTH                      1
#define D18F3x18C_DisChkIntPendState_MASK                       0x1000
#define D18F3x18C_EnHwAssertProtErr_OFFSET                      13
#define D18F3x18C_EnHwAssertProtErr_WIDTH                       1
#define D18F3x18C_EnHwAssertProtErr_MASK                        0x2000
#define D18F3x18C_DcqDepthCtl_OFFSET                            14
#define D18F3x18C_DcqDepthCtl_WIDTH                             4
#define D18F3x18C_DcqDepthCtl_MASK                              0x3c000ul
#define D18F3x18C_DisWDTtickInC1_OFFSET                         18
#define D18F3x18C_DisWDTtickInC1_WIDTH                          1
#define D18F3x18C_DisWDTtickInC1_MASK                           0x40000ul
#define D18F3x18C_EnL3VicBlkBusLock_OFFSET                      19
#define D18F3x18C_EnL3VicBlkBusLock_WIDTH                       1
#define D18F3x18C_EnL3VicBlkBusLock_MASK                        0x80000ul
#define D18F3x18C_DisPwrMgtWaitPrbRspOnly_OFFSET                20
#define D18F3x18C_DisPwrMgtWaitPrbRspOnly_WIDTH                 1
#define D18F3x18C_DisPwrMgtWaitPrbRspOnly_MASK                  0x100000ul
#define D18F3x18C_FwdClVicBlkToMctEn_OFFSET                     21
#define D18F3x18C_FwdClVicBlkToMctEn_WIDTH                      1
#define D18F3x18C_FwdClVicBlkToMctEn_MASK                       0x200000ul
#define D18F3x18C_DisXcsScndPicker_OFFSET                       22
#define D18F3x18C_DisXcsScndPicker_WIDTH                        1
#define D18F3x18C_DisXcsScndPicker_MASK                         0x400000ul
#define D18F3x18C_DisXcs0BlockDis2ndPicker_OFFSET               23
#define D18F3x18C_DisXcs0BlockDis2ndPicker_WIDTH                1
#define D18F3x18C_DisXcs0BlockDis2ndPicker_MASK                 0x800000ul
#define D18F3x18C_DisMct2SriPrbByp_OFFSET                       24
#define D18F3x18C_DisMct2SriPrbByp_WIDTH                        1
#define D18F3x18C_DisMct2SriPrbByp_MASK                         0x1000000ul
#define D18F3x18C_DisSriToMctRlxdByp_OFFSET                     25
#define D18F3x18C_DisSriToMctRlxdByp_WIDTH                      1
#define D18F3x18C_DisSriToMctRlxdByp_MASK                       0x2000000ul
#define D18F3x18C_DisL3FillAntiStarvation_OFFSET                26
#define D18F3x18C_DisL3FillAntiStarvation_WIDTH                 1
#define D18F3x18C_DisL3FillAntiStarvation_MASK                  0x4000000ul
#define D18F3x18C_DisMultL3PrbHitDatMov_OFFSET                  27
#define D18F3x18C_DisMultL3PrbHitDatMov_WIDTH                   1
#define D18F3x18C_DisMultL3PrbHitDatMov_MASK                    0x8000000ul
#define D18F3x18C_DisSpqPrbByp_OFFSET                           28
#define D18F3x18C_DisSpqPrbByp_WIDTH                            1
#define D18F3x18C_DisSpqPrbByp_MASK                             0x10000000ul
#define D18F3x18C_DisErlyL3PrbHitDatRd_OFFSET                   29
#define D18F3x18C_DisErlyL3PrbHitDatRd_WIDTH                    1
#define D18F3x18C_DisErlyL3PrbHitDatRd_MASK                     0x20000000ul
#define D18F3x18C_DisParallelCpuPwrExit_OFFSET                  30
#define D18F3x18C_DisParallelCpuPwrExit_WIDTH                   1
#define D18F3x18C_DisParallelCpuPwrExit_MASK                    0x40000000ul
#define D18F3x18C_DisC6StpGntExit_OFFSET                        31
#define D18F3x18C_DisC6StpGntExit_WIDTH                         1
#define D18F3x18C_DisC6StpGntExit_MASK                          0x80000000ul

/// D18F3x18C
typedef union {
  struct {                                                              ///<
    UINT32                                     DisLclSpqToSriPrbRsp:1 ; ///<
    UINT32                                          DisDirImpUpdate:1 ; ///<
    UINT32                                      DisDynEStateDataRet:1 ; ///<
    UINT32                                           EnInvDirOnWrSz:1 ; ///<
    UINT32                                       DisSrqReqCompOnWDT:1 ; ///<
    UINT32                                   EnFidVidTargetBootCore:1 ; ///<
    UINT32                                             Reserved_6_6:1 ; ///<
    UINT32                                            DisWrSzUState:1 ; ///<
    UINT32                                            DisPfcqBypass:1 ; ///<
    UINT32                                      DisXdsDstLnkFastXfr:1 ; ///<
    UINT32                                           DisEarlyDirRsp:1 ; ///<
    UINT32                                     MTC1eHaltBrdcstToCht:1 ; ///<
    UINT32                                       DisChkIntPendState:1 ; ///<
    UINT32                                        EnHwAssertProtErr:1 ; ///<
    UINT32                                              DcqDepthCtl:4 ; ///<
    UINT32                                           DisWDTtickInC1:1 ; ///<
    UINT32                                        EnL3VicBlkBusLock:1 ; ///<
    UINT32                                  DisPwrMgtWaitPrbRspOnly:1 ; ///<
    UINT32                                       FwdClVicBlkToMctEn:1 ; ///<
    UINT32                                         DisXcsScndPicker:1 ; ///<
    UINT32                                 DisXcs0BlockDis2ndPicker:1 ; ///<
    UINT32                                         DisMct2SriPrbByp:1 ; ///<
    UINT32                                       DisSriToMctRlxdByp:1 ; ///<
    UINT32                                  DisL3FillAntiStarvation:1 ; ///<
    UINT32                                    DisMultL3PrbHitDatMov:1 ; ///<
    UINT32                                             DisSpqPrbByp:1 ; ///<
    UINT32                                     DisErlyL3PrbHitDatRd:1 ; ///<
    UINT32                                    DisParallelCpuPwrExit:1 ; ///<
    UINT32                                          DisC6StpGntExit:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3x18C_STRUCT;

// **** D18F3x2B4 Register Definition ****
// Address
#define D18F3x2B4_ADDRESS                                       0x2B4

// Type
#define D18F3x2B4_TYPE                                          TYPE_D18F3
// Field Data
#define D18F3x2B4_DctPwrGateEn_OFFSET                           0
#define D18F3x2B4_DctPwrGateEn_WIDTH                            4
#define D18F3x2B4_DctPwrGateEn_MASK                             0xF
#define D18F3x2B4_Reserved7_4_OFFSET                            4
#define D18F3x2B4_Reserved7_4_WIDTH                             4
#define D18F3x2B4_Reserved7_4_MASK                              0xF
#define D18F3x2B4_DctClkGateEn_OFFSET                           8
#define D18F3x2B4_DctClkGateEn_WIDTH                            4
#define D18F3x2B4_DctClkGateEn_MASK                             0xF
#define D18F3x2B4_Reserved15_12_OFFSET                          12
#define D18F3x2B4_Reserved15_12_WIDTH                           4
#define D18F3x2B4_Reserved15_12_MASK                            0xF
#define D18F3x2B4_PrePwrUpDelay_OFFSET                          16
#define D18F3x2B4_PrePwrUpDelay_WIDTH                           2
#define D18F3x2B4_PrePwrUpDelay_MASK                            0x3
#define D18F3x2B4_PostPwrUpDelay_OFFSET                         18
#define D18F3x2B4_PostPwrUpDelay_WIDTH                          2
#define D18F3x2B4_PostPwrUpDelay_MASK                           0x3
#define D18F3x2B4_Reserved21_20_OFFSET                          20
#define D18F3x2B4_Reserved21_20_WIDTH                           2
#define D18F3x2B4_Reserved21_20_MASK                            0x3
#define D18F3x2B4_PostPwrDnDelay_OFFSET                         22
#define D18F3x2B4_PostPwrDnDelay_WIDTH                          2
#define D18F3x2B4_PostPwrDnDelay_MASK                           0x3
#define D18F3x2B4_PwrDnHyst_OFFSET                              24
#define D18F3x2B4_PwrDnHyst_WIDTH                               2
#define D18F3x2B4_PwrDnHyst_MASK                                0x3
#define D18F3x2B4_FusePwrStatus_OFFSET                          26
#define D18F3x2B4_FusePwrStatus_WIDTH                           1
#define D18F3x2B4_FusePwrStatus_MASK                            0x1
#define D18F3x2B4_Reserved31_27_OFFSET                          27
#define D18F3x2B4_Reserved31_27_WIDTH                           5
#define D18F3x2B4_Reserved31_27_MASK                            0x1F

/// D18F3x2B4
typedef union {
  struct {                                                              ///<
    UINT32                                             DctPwrGateEn:2 ; ///<
    UINT32                                              Reserved7_2:6 ; ///<
    UINT32                                             DctClkGateEn:2 ; ///<
    UINT32                                            Reserved15_10:6 ; ///<
    UINT32                                            PrePwrUpDelay:2 ; ///<
    UINT32                                           PostPwrUpDelay:2 ; ///<
    UINT32                                            Reserved21_20:2 ; ///<
    UINT32                                    DisMultL3PrbHitDatMov:2 ; ///<
    UINT32                                             DisSpqPrbByp:2 ; ///<
    UINT32                                     DisErlyL3PrbHitDatRd:2 ; ///<
    UINT32                                    DisParallelCpuPwrExit:1 ; ///<
    UINT32                                            Reserved31_27:5 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F3x2B4_STRUCT;

// **** D18F4x15C Register Definition ****
// Address
#define D18F4x15C_ADDRESS                                       0x15c

// Type
#define D18F4x15C_TYPE                                          TYPE_D18F4
// Field Data
#define D18F4x15C_BoostSrc_OFFSET                               0
#define D18F4x15C_BoostSrc_WIDTH                                2
#define D18F4x15C_BoostSrc_MASK                                 0x3
#define D18F4x15C_NumBoostStates_OFFSET                         2
#define D18F4x15C_NumBoostStates_WIDTH                          3
#define D18F4x15C_NumBoostStates_MASK                           0x1c
#define D18F4x15C_Reserved_6_5_OFFSET                           5
#define D18F4x15C_Reserved_6_5_WIDTH                            2
#define D18F4x15C_Reserved_6_5_MASK                             0x60
#define D18F4x15C_ApmMasterEn_OFFSET                            7
#define D18F4x15C_ApmMasterEn_WIDTH                             1
#define D18F4x15C_ApmMasterEn_MASK                              0x80
#define D18F4x15C_Reserved_27_8_OFFSET                          8
#define D18F4x15C_Reserved_27_8_WIDTH                           20
#define D18F4x15C_Reserved_27_8_MASK                            0xfffff00ul
#define D18F4x15C_TdpLimitPstate_OFFSET                         28
#define D18F4x15C_TdpLimitPstate_WIDTH                          3
#define D18F4x15C_TdpLimitPstate_MASK                           0x70000000ul
#define D18F4x15C_BoostLock_OFFSET                              31
#define D18F4x15C_BoostLock_WIDTH                               1
#define D18F4x15C_BoostLock_MASK                                0x80000000ul

/// D18F4x15C
typedef union {
  struct {                                                              ///<
    UINT32                                                 BoostSrc:2 ; ///<
    UINT32                                           NumBoostStates:3 ; ///<
    UINT32                                             Reserved_6_5:2 ; ///<
    UINT32                                              ApmMasterEn:1 ; ///<
    UINT32                                            Reserved_27_8:20; ///<
    UINT32                                           TdpLimitPstate:3 ; ///<
    UINT32                                                BoostLock:1 ; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F4x15C_STRUCT;

// **** D18F4x16C Register Definition ****
// Address
#define D18F4x16C_ADDRESS                                       0x16c

// Type
#define D18F4x16C_TYPE                                          TYPE_D18F4
// Field Data
#define D18F4x16C_ApmlSwTdpLimitEn_OFFSET                       0
#define D18F4x16C_ApmlSwTdpLimitEn_WIDTH                        2
#define D18F4x16C_ApmlSwTdpLimitEn_MASK                         0x3
#define D18F4x16C_NodeTdpLimitEn_OFFSET                         2
#define D18F4x16C_NodeTdpLimitEn_WIDTH                          1
#define D18F4x16C_NodeTdpLimitEn_MASK                           0x4
#define D18F4x16C_TdpLimitDis_OFFSET                            3
#define D18F4x16C_TdpLimitDis_WIDTH                             1
#define D18F4x16C_TdpLimitDis_MASK                              0x8
#define D18F4x16C_ApmTdpLimitIntEn_OFFSET                       4
#define D18F4x16C_ApmTdpLimitIntEn_WIDTH                        1
#define D18F4x16C_ApmTdpLimitIntEn_MASK                         0x10
#define D18F4x16C_ApmTdpLimitSts_OFFSET                         5
#define D18F4x16C_ApmTdpLimitSts_WIDTH                          1
#define D18F4x16C_ApmTdpLimitSts_MASK                           0x20
#define D18F4x16C_CstateBoost_OFFSET                            6
#define D18F4x16C_CstateBoost_WIDTH                             3
#define D18F4x16C_CstateBoost_MASK                              0x1c0
#define D18F4x16C_CstateCnt_OFFSET                              9
#define D18F4x16C_CstateCnt_WIDTH                               3
#define D18F4x16C_CstateCnt_MASK                                0xe00
#define D18F4x16C_Reserved_31_12_OFFSET                         12
#define D18F4x16C_Reserved_31_12_WIDTH                          20
#define D18F4x16C_Reserved_31_12_MASK                           0xfffff000ul

/// D18F4x16C
typedef union {
  struct {                                                              ///<
    UINT32                                         ApmlSwTdpLimitEn:2 ; ///<
    UINT32                                           NodeTdpLimitEn:1 ; ///<
    UINT32                                              TdpLimitDis:1 ; ///<
    UINT32                                         ApmTdpLimitIntEn:1 ; ///<
    UINT32                                           ApmTdpLimitSts:1 ; ///<
    UINT32                                              CstateBoost:3 ; ///<
    UINT32                                                CstateCnt:3 ; ///<
    UINT32                                           Reserved_31_12:20; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F4x16C_STRUCT;

/* MSR Register 0xC0011040 */
#define MSR_C001_1040 0xC0011040ul

/// C001_1040 MSR Register
typedef struct {
  UINT64 :51;                        ///< Reserved
  UINT64 Lp:1;                       ///< Lp
  UINT64 :12;                        ///< Reserved
} C001_1040_MSR;

// **** D18F5x130 Register Definition ****
// Address
#define D18F5x130_ADDRESS                                       0x130

/// D18F5x130
typedef union {
  struct {                                                              ///<
    UINT32                                                   Offset:6 ; ///<
    UINT32                                                    RegRd:1 ; ///<
    UINT32                                            Reserved_31_7:25; ///<
  } Field;                                                              ///<
  UINT32 Value;                                                         ///<
} D18F5x130_STRUCT;

// **** D18F5x138 Register Definition ****
// Address
#define D18F5x138_ADDRESS                                       0x138


#endif  //_IDS_INT_F15_ST_ALLSERVICE_H_

