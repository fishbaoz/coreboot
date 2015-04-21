/* $NoKeywords:$ */
/**
 * @file
 *
 * IDS Option Install File
 *
 * This file generates the defaults tables for family 10h model 5 processors.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 311976 $   @e \$Date: 2015-01-29 13:34:44 +0800 (Thu, 29 Jan 2015) $
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
#ifndef _OPTION_IDS_INTERNAL_INSTALL_H_
#define _OPTION_IDS_INTERNAL_INSTALL_H_

#include IDS_EXT_INCLUDE (IdsInternalLib)

#ifdef AGESA_ENTRY_LATE_RUN_AP_TASK
  #undef AGESA_ENTRY_LATE_RUN_AP_TASK
#endif
#define AGESA_ENTRY_LATE_RUN_AP_TASK    TRUE

#if (IDSOPT_IDS_ENABLED == TRUE)
  #if ((IDSOPT_CONTROL_ENABLED == TRUE) && \
       ((AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_ENV == TRUE) || \
        (AGESA_ENTRY_INIT_MID == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE) || (AGESA_ENTRY_INIT_RTB == TRUE) || \
        (AGESA_ENTRY_INIT_RESUME == TRUE) || (AGESA_ENTRY_INIT_LATE_RESTORE == TRUE)))
    #define OPTION_IDS_NV_TO_CMOS_EXTEND_COMMON\
            {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_REDIRECT_SHUTDOWN_TO_HDT}, \
            {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_REDIRECT_INT1_INT3_TO_HDT}, \
            {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_DEBUGCTRL_LBR},

    #if ((OPTION_FAMILY15H_CZ == TRUE) || (OPTION_FAMILY15H_ST == TRUE))
        #define OPTION_IDS_NV_TO_CMOS_EXTEND_F15\
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_APIC330_MASK_CONFIG}, \
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_MSR_ACCESS}, \
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_WATCHDOG_TIMER}, \
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_SYNC_FLOOD_MCA}, \
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_EXCEPTION_BREAKPOINT_CTRL}, \
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_EXCEPTION_BREAKPOINT_VECTOR}, \
          {IDS_NV_TO_CMOS_LEN_BYTE, AGESA_IDS_NV_RDRAND},
    #endif


    #ifndef OPTION_IDS_NV_TO_CMOS_EXTEND_COMMON
      #define OPTION_IDS_NV_TO_CMOS_EXTEND_COMMON
    #endif

    #ifndef OPTION_IDS_NV_TO_CMOS_EXTEND_F15
      #define OPTION_IDS_NV_TO_CMOS_EXTEND_F15
    #endif

    #define OPTION_IDS_NV_TO_CMOS_EXTEND\
                OPTION_IDS_NV_TO_CMOS_EXTEND_COMMON \
                OPTION_IDS_NV_TO_CMOS_EXTEND_F15
  #endif
#endif

#if (IDSOPT_IDS_ENABLED == TRUE)
  #if ((IDSOPT_CONTROL_ENABLED == TRUE) && \
       ((AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_ENV == TRUE) || \
        (AGESA_ENTRY_INIT_MID == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE) || (AGESA_ENTRY_INIT_RTB == TRUE) || \
        (AGESA_ENTRY_INIT_RESUME == TRUE) || (AGESA_ENTRY_INIT_LATE_RESTORE == TRUE)))

    #define   OPTION_IDS_FEAT_INT_MEMORY_MAPPING\
                  OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_CZ \
                  OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_ST

    #define   OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK\
                  OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_CZ \
                  OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_ST

    #define OPTION_IDS_FEAT_CUSTOMPSTATE

    #define  OPTION_IDS_FEAT_CORE_LEVELING\
                OPTION_IDS_FEAT_CORE_LEVELING_F15_CZ \
                OPTION_IDS_FEAT_CORE_LEVELING_F15_ST

    #define OPTION_IDS_FEAT_RDRAND\
                OPTION_IDS_FEAT_RDRAND_F15_CZ \
                OPTION_IDS_FEAT_RDRAND_F15_ST

    #define  OPTION_IDS_FEAT_WATCHDOG_TIMER\
                OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_CZ \
                OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_ST

    #define  OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT\
                OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_CZ \
                OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_ST

    #define  OPTION_IDS_FEAT_MEMTBL_DRV\
                OPTION_IDS_FEAT_MEMTBL_DRV_F15_CZ \
                OPTION_IDS_FEAT_MEMTBL_DRV_F15_ST

    #define OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL\
                OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_CZ \
                OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_ST

    #define OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE\
                OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_CZ \
                OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_ST

    #define OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX\
                OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_CZ \
                OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_ST

    #define OPTION_IDS_FEAT_GNB_CLOCK_GATING

    #define OPTION_IDS_FEAT_GNB_LCLK_DPM

    #define OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK\
                OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_CZ \
                OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_ST

    #define OPTION_IDS_FEAT_GNB_LCLK_DEEPSLEEP

    #define OPTION_IDS_FEAT_MAIN_PLL

    #define OPTION_IDS_FEAT_NB_PSTATE_DIDVID

    #define OPTION_IDS_FEAT_APM_TDP_CTRL

    #define OPTION_IDS_FEAT_CSAMPLETIMER

    #define OPTION_IDS_FEAT_PIPETHROTTLE

    #define OPTION_IDS_FEAT_INT_CPB_CTRL\
                OPTION_IDS_FEAT_INT_CPB_CTRL_F15_CZ \
                OPTION_IDS_FEAT_INT_CPB_CTRL_F15_ST

    #define OPTION_IDS_FEAT_CACHE_FLUSHCTRL\
                OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_CZ \
                OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_ST

    #define OPTION_IDS_FEAT_CACHE_FLUSHMISCCTRL

    #define OPTION_IDS_FEAT_CACHE_FLUSHXC6CTRL

    #define OPTION_IDS_FEAT_CACHE_FLUSHRINSERCTRL

    #define OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE \
                OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_CZ \
                OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_ST

    #define OPTION_IDS_FEAT_CSTATE_MODE \
                OPTION_IDS_FEAT_CSTATE_MODE_F15_CZ \
                OPTION_IDS_FEAT_CSTATE_MODE_F15_ST

    #define OPTION_IDS_FEAT_DLL_CSR

    #define OPTION_IDS_FEAT_CC6_WORKAROUND

    #define OPTION_IDS_FEAT_REG_TABLE

    #define OPTION_IDS_FEAT_NB_BUFFER_ALLOCATION

    #define OPTION_IDS_FEAT_ERRATUM463_WORAROUND

    #define OPTION_IDS_FEAT_INT_NBPSTATE\
                OPTION_IDS_FEAT_INT_NBPSTATE_F15_CZ \
                OPTION_IDS_FEAT_INT_NBPSTATE_F15_ST

    #define OPTION_IDS_FEAT_GNB_FORCE_CABLESAFE

    #define OPTION_IDS_FEAT_GNB_PROPERTY\
                OPTION_IDS_FEAT_GNB_PROPERTY_F15_CZ \
                OPTION_IDS_FEAT_GNB_PROPERTY_F15_ST

    #define OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING\
                OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_CZ \
                OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_ST

    #define OPTION_IDS_FEAT_GNB_SWTJOFFSET\
                OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_CZ \
                OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_ST

    #define OPTION_IDS_FEAT_GNB_PMMPCIEPHYISOLATION

    #define OPTION_IDS_FEAT_GNB_GPPPCIEMASTERPLLSELECTION

    #define OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP\
                OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_CZ \
                OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_ST

    #define OPTION_IDS_FEAT_GNB_PMM_NATIVE_GEN1_PLL

    #define OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG\
                OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_CZ \
                OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_ST

    #define OPTION_IDS_FEAT_GNB_ALTVDDNB

    #define OPTION_IDS_FEAT_LATE_AFTER

    #define OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT \
                OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_CZ \
                OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_ST

    #define OPTION_IDS_FEAT_DIS_CPU_FEAT\
                OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_CZ \
                OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_ST

    #define OPTION_IDS_FEAT_AFTER_APIC_ENABLE\
                OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_CZ \
                OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_ST

    #define OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT\
                OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_CZ \
                OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_ST

    #define OPTION_IDS_FEAT_CTDP_NPST_FEAT\
                OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_CZ \
                OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_ST

    #define OPTION_IDS_FEAT_DLLSTAGGERDLY

    #define OPTION_IDS_FEAT_GNB_LOAD_SAMU_PATCH

    #define OPTION_IDS_FEAT_BANK_SWIZZLE

    #define OPTION_IDS_FEAT_ECC_CONFIGURATION\
                OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_CZ \
                OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_ST

    #define OPTION_IDS_FEAT_SMU_PORT80_LOGGING_DISABLE

    #define OPTION_IDS_FEAT_SMU_PORT80_LOGGING_ENABLE

    #define OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG\
                OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_CZ \
                OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_ST

    #define OPTION_IDS_FEAT_DSM_LP_CONFIG\
                OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_CZ \
                OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_ST

    #define OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG\
                OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_CZ \
                OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_ST

    #define OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG\
                OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_CZ \
                OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_ST

    #define OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN\
                OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_CZ \
                OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_ST

    #define OPTION_IDS_FEAT_TARGET_TDP\
                OPTION_IDS_FEAT_TARGET_TDP_F15_CZ \
                OPTION_IDS_FEAT_TARGET_TDP_F15_ST

    #define OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS\
                OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_CZ \
                OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_ST

    #define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET\
                OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_CZ \
                OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_ST

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatAcpiPssBlock =
    {
      IDS_INT_FEAT_ACPI_PSS,
      IDS_ALL_CORES,
      IDS_PLATFORMCFG_OVERRIDE,
      IDS_FAMILY_ALL,
      IdsSubPss
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatAcpiCstBlock =
    {
      IDS_INT_FEAT_ACPI_CST,
      IDS_ALL_CORES,
      IDS_PLATFORMCFG_OVERRIDE,
      IDS_FAMILY_ALL,
      IdsSubCst
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatAcpiWheaBlock =
    {
      IDS_INT_FEAT_ACPI_WHEA,
      IDS_ALL_CORES,
      IDS_PLATFORMCFG_OVERRIDE,
      IDS_FAMILY_ALL,
      IdsSubWhea
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatDmiBlock =
    {
      IDS_INT_FEAT_DMI_TBL,
      IDS_ALL_CORES,
      IDS_PLATFORMCFG_OVERRIDE,
      IDS_FAMILY_ALL,
      IdsSubDmi
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatPsiBlock =
    {
      IDS_INT_FEAT_PSI_EN,
      IDS_ALL_CORES,
      IDS_PLATFORMCFG_OVERRIDE,
      IDS_FAMILY_ALL,
      IdsSubPsi
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatDctConfigBlock =
    {
      IDS_INT_FEAT_DCT_CONFIG_CONTROL,
      IDS_ALL_CORES,
      IDS_INIT_POST_BEFORE,
      IDS_FAMILY_ALL,
      IdsIntSubDctConfigPostBefore
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryTblDrvGetBlock =
    {
      IDS_INT_FEAT_MEMORY_TBL_DRV_SUPPORT,
      IDS_ALL_CORES,
      IDS_GET_DRAM_TABLE,
      IDS_FAMILY_ALL,
      IdsSubGetDramTable
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatEnforceFreqBlock =
    {
      IDS_INT_FEAT_ENFORCE_FREQ,
      IDS_ALL_CORES,
      IDS_POR_MEM_FREQ,
      IDS_FAMILY_ALL,
      IdsEnforcePorMemFreq
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatEnforceStretchFreqBlock =
    {
      IDS_INT_FEAT_ENFORCE_FREQ,
      IDS_ALL_CORES,
      IDS_STRETCH_FREQUENCY_LIMIT,
      IDS_FAMILY_ALL,
      IdsEnforceStretchMemFreq
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryTimingBlock =
    {
      IDS_INT_FEAT_MEMORY_TIMING,
      IDS_ALL_CORES,
      IDS_INIT_POST_BEFORE,
      IDS_FAMILY_ALL,
      IdsIntSubMemoryTimingPostBefore
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryErrorRecoveryBlock =
    {
      IDS_INT_FEAT_MEM_ERROR_RECOVERY,
      IDS_ALL_CORES,
      IDS_MEM_ERROR_RECOVERY,
      IDS_FAMILY_ALL,
      IdsSubMemErrorRecovery
    };


    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryRetrainTimesBlock =
    {
      IDS_INT_FEAT_MEM_RETRAIN_TIMES,
      IDS_ALL_CORES,
      IDS_MEM_RETRAIN_TIMES,
      IDS_FAMILY_ALL,
      IdsSubMemRetrainTimes
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryMrlRetrainTimesBlock =
    {
      IDS_INT_FEAT_MEM_MRL_RETRAIN_TIMES,
      IDS_ALL_CORES,
      IDS_MEM_MRL_RETRAIN_TIMES,
      IDS_FAMILY_ALL,
      IdsSubMemMrlRetrainTimes
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfDramInitBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_DRAM_INIT,
      IDS_FAMILY_ALL,
      IdsIntHdtBreakPointBeforeDramInit
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfMemClrBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_MEMCLR,
      IDS_FAMILY_ALL,
      IdsIntHdtBreakPointBeforeMemClr
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfGen2InitBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_GEN2_INIT,
      IDS_FAMILY_ALL,
      IdsFeatHdtBreakPointBfGen2Init
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfGppTrainingBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_GPP_TRAINING,
      IDS_FAMILY_ALL,
      IdsFeatHdtBreakPointBfGppTraining
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfReConfigurationBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_RECONFIGURATION,
      IDS_FAMILY_ALL,
      IdsFeatHdtBreakPointBfReConfiguration
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfGen3InitBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_GEN3_INIT,
      IDS_FAMILY_ALL,
      IdsFeatHdtBreakPointBfGen3Init
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatHdtBreakPointBfPowerGatingBlock =
    {
      IDS_INT_FEAT_HDTBREAKPOINT,
      IDS_ALL_CORES,
      IDS_BEFORE_POWER_GATING,
      IDS_FAMILY_ALL,
      IdsFeatHdtBreakPointBfPowerGating
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatIgnoreMemErrorBlock =
    {
      IDS_INT_FEAT_IGNORE_MEM_ERROR,
      IDS_ALL_CORES,
      IDS_MEM_IGNORE_ERROR,
      IDS_FAMILY_ALL,
      IdsSubIgnoreMemError
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatEnforceVddIoBlock =
    {
      IDS_INT_FEAT_ENFORCE_VDDIO,
      IDS_ALL_CORES,
      IDS_ENFORCE_VDDIO,
      IDS_FAMILY_ALL,
      IdsEnforceVddIo
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemPowerPolicyBlock =
    {
      IDS_INT_FEAT_MEM_POWER_POLICY,
      IDS_ALL_CORES,
      IDS_MEMORY_POWER_POLICY,
      IDS_FAMILY_ALL,
      IdsMemPowerPolicy
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatSkipFusedMaxDdrRateBlock =
    {
      IDS_INT_FEAT_SKIP_FUSED_MAX_RATE,
      IDS_ALL_CORES,
      IDS_SKIP_FUSED_MAX_RATE,
      IDS_FAMILY_ALL,
      IdsSkipFusedMaxDdrRate
    };

    CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatRedirectShutDownToHdtBlock =
    {
      IDS_FEAT_COMMON,
      IDS_ALL_CORES,
      IDS_INIT_EARLY_BEFORE,
      IDS_FAMILY_ALL,
      IdsIntRedirectShutDownToHdt
    };

    #define OPTION_IDS_FEAT_LOADCARD
    #if ((AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_RTB == TRUE))
      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCPostBeforeBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_INIT_POST_BEFORE,
        IDS_FAMILY_ALL,
        IdsSubLoadCardSupport
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBeforeS3SaveBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_S3_SAVE,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBeforeS3Save
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCAfterS3SaveBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_AFTER_S3_SAVE,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardAfterS3Save
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCAfterDctPhyAccess =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_AFTER_DCT_PHY_ACCESS,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardAfterDctPhyAccess
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBeforeS3RestoreBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_S3_RESTORE,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBeforeS3Restore
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCAfterDqsTrainingBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_AFTER_DQS_TRAINING,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardAfterDqsTraining
      };


      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBeforeDramInitBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_DRAM_INIT,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBeforeDramInit
      };


      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBeforeFreqChgBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_MEM_FREQ_CHG,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBeforeMemFreqChg
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCErrorRecoveryBlock =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_LOADCARD_ERROR_RECOVERY,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardMemErrorRecovery
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBypassHook =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_S3_SPECIAL,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBypassHook
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCMemMappingOverride =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_HT_MEM_MAP,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardMemMappingOverride
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCForcePhyToM0 =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_FORCE_PHY_TO_M0,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardForcePhyToM0
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBeforePlatTable =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BEFORE_PLAT_TABLES,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBeforePlatTables
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCPlatTableOverride =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_ENFORCE_PLAT_TABLES,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardAfterPlatTables
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCPciRegRestore =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_AFTER_RESTORING_PCI_REG,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardAfterPciRegRestore
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCPlatCfgOverride =
      {
        IDS_INT_FEAT_CSTATE_MODE,
        IDS_ALL_CORES,
        IDS_PLATFORMCFG_OVERRIDE,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardPlatCfgOverride
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCDimmMaskOverride =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_OVERRIDE_DIMM_MASK,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardDimmMaskOverride
      };

      CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryLCBypassS3Registers =
      {
        IDS_INT_FEAT_MEMORY_LOADCARD_SUPPORT,
        IDS_ALL_CORES,
        IDS_BYPASS_S3_REGISTERS,
        IDS_FAMILY_ALL,
        IdsIntSubLoadCardBypassS3Registers
      };

      #undef OPTION_IDS_FEAT_LOADCARD
      #define OPTION_IDS_FEAT_LOADCARD \
              &IdsFeatMemoryLCErrorRecoveryBlock, \
              &IdsFeatMemoryLCPostBeforeBlock, \
              &IdsFeatMemoryLCBeforeS3SaveBlock, \
              &IdsFeatMemoryLCAfterS3SaveBlock, \
              &IdsFeatMemoryLCBeforeS3RestoreBlock, \
              &IdsFeatMemoryLCAfterDqsTrainingBlock, \
              &IdsFeatMemoryLCBeforeDramInitBlock, \
              &IdsFeatMemoryLCBeforeFreqChgBlock, \
              &IdsFeatMemoryLCBypassHook, \
              &IdsFeatMemoryLCMemMappingOverride, \
              &IdsFeatMemoryLCForcePhyToM0, \
              &IdsFeatMemoryLCAfterDctPhyAccess, \
              &IdsFeatMemoryLCBeforePlatTable, \
              &IdsFeatMemoryLCPlatTableOverride, \
              &IdsFeatMemoryLCPciRegRestore, \
              &IdsFeatMemoryLCPlatCfgOverride, \
              &IdsFeatMemoryLCDimmMaskOverride, \
              &IdsFeatMemoryLCBypassS3Registers,
    #endif


/*----------------------------------------------------------------------------
 *                        Family related
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        Family 15 CZ feat blocks
 *
 *----------------------------------------------------------------------------
 */
    #define OPTION_IDS_FEAT_RDRAND_F15_CZ
    #define OPTION_IDS_FEAT_CORE_LEVELING_F15_CZ
    #define OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_CZ
    #define OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_CZ
    #define OPTION_IDS_FEAT_INT_NBPSTATE_F15_CZ
    #define OPTION_IDS_FEAT_INT_CPB_CTRL_F15_CZ
    #define OPTION_IDS_FEAT_CSTATE_MODE_F15_CZ
    #define OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_CZ
    #define OPTION_IDS_FEAT_MEMTBL_DRV_F15_CZ
    #define OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_CZ
    #define OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_CZ
    #define OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_CZ
    #define OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_CZ
    #define OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_CZ
    #define OPTION_IDS_FEAT_GNB_PROPERTY_F15_CZ
    #define OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_CZ
    #define OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_CZ
    #define OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_CZ
    #define OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_CZ
    #define OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_CZ
    #define OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_CZ
    #define OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_CZ
    #define OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_CZ
    #define OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_CZ
    #define OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_CZ
    #define OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_CZ
    #define OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_CZ
    #define OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_CZ
    #define OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_CZ
    #define OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_CZ
    #define OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_CZ
    #define OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_CZ
    #define OPTION_IDS_FEAT_TARGET_TDP_F15_CZ
    #define OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_CZ
    #define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_CZ
    #define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_CZ

    #ifdef OPTION_FAMILY15H_CZ
      #if OPTION_FAMILY15H_CZ == TRUE
        #undef OPTION_IDS_FEAT_CORE_LEVELING_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCoreLevelingBlockF15Cz;
        #define OPTION_IDS_FEAT_CORE_LEVELING_F15_CZ &IdsFeatCoreLevelingBlockF15Cz,

        #undef OPTION_IDS_FEAT_RDRAND_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatRdRandBlockF15Cz;
        #define OPTION_IDS_FEAT_RDRAND_F15_CZ &IdsIntFeatRdRandBlockF15Cz,

        #undef OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatWatchdogTimerAndSyncFloodMcaBlockF15Cz;
        #define OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_CZ &IdsIntFeatWatchdogTimerAndSyncFloodMcaBlockF15Cz,

        #undef OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatExceptionBreakpointBlockF15Cz;
        #define OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_CZ &IdsIntFeatExceptionBreakpointBlockF15Cz,

        #undef OPTION_IDS_FEAT_INT_NBPSTATE_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatNBPStatesSupportBlockF15Cz;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntFeatNbPsDisOverrideBlockF15Cz;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntSubNbPstateGetMinFreqBlockF15Cz;
        #define OPTION_IDS_FEAT_INT_NBPSTATE_F15_CZ \
                 &IdsFeatNBPStatesSupportBlockF15Cz, \
                 &IdsIntFeatNbPsDisOverrideBlockF15Cz, \
                 &IdsIntSubNbPstateGetMinFreqBlockF15Cz,

        #undef OPTION_IDS_FEAT_INT_CPB_CTRL_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatCpbCtrlBlockF15Cz;
        #define OPTION_IDS_FEAT_INT_CPB_CTRL_F15_CZ  &IdsIntFeatCpbCtrlBlockF15Cz,

        #undef OPTION_IDS_FEAT_CSTATE_MODE_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCStateModeBlockF15Cz;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatConfigIOCstatePostAfterBlockF15Cz;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGetCstBlockF15Cz;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCreateCstBlockF15Cz;
        #define OPTION_IDS_FEAT_CSTATE_MODE_F15_CZ \
                    &IdsFeatCStateModeBlockF15Cz, \
                    &IdsFeatConfigIOCstatePostAfterBlockF15Cz, \
                    &IdsFeatGetCstBlockF15Cz, \
                    &IdsFeatCreateCstBlockF15Cz,

        #undef OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_CZ
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCacheFlushCtrlBlockF15Cz;
        #define OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_CZ &IdsFeatCacheFlushCtrlBlockF15Cz,

        #undef OPTION_IDS_FEAT_MEMTBL_DRV_F15_CZ
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryTblDrvInitBlockF15Cz;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryOverrideMr0F15Cz;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryInitRegTblBlockF15Cz;
        #define OPTION_IDS_FEAT_MEMTBL_DRV_F15_CZ \
                    &IdsFeatMemoryTblDrvInitBlockF15Cz, \
                    &IdsFeatMemoryOverrideMr0F15Cz, \
                    &IdsFeatMemoryInitRegTblBlockF15Cz,

        #undef OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatMemoryMappingPostBeforeBlockF15Cz;
        #define OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_CZ \
                    &IdsIntFeatMemoryMappingPostBeforeBlockF15Cz,

        #undef OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryEnforceMClkAboveNClkF15Cz;
        #define OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_CZ \
                    &IdsFeatMemoryEnforceMClkAboveNClkF15Cz,

        #undef OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_CZ
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyDllStandbyCtrlBlockF15Cz;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyDllStandbyCtrlBeforeDramInitBlockF15Cz;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyPowerSavingF15Cz;
        #define OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_CZ \
                    &IdsFeatMemoryPhyDllStandbyCtrlBlockF15Cz, \
                    &IdsFeatMemoryPhyDllStandbyCtrlBeforeDramInitBlockF15Cz, \
                    &IdsFeatMemoryPhyPowerSavingF15Cz,

        #undef OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGetStretchFreqBlockF15Cz;
        #define OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_CZ &IdsFeatGetStretchFreqBlockF15Cz,

        //GNB features
        #undef OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGNBORBDynamicWakeBlockF15Cz;
        #define OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_CZ &IdsFeatGNBORBDynamicWakeBlockF15Cz,

        #undef OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPlatformCfgExBlockF15Cz;
        #define OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_CZ &IdsFeatGnbPlatformCfgExBlockF15Cz,

        ///@todo check if CZ needs this
        //#undef OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_CZ
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbSmuServiceMskOverrideBlockF15Cz;
        //#define OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_CZ &IdsFeatGnbSmuServiceMskOverrideBlockF15Cz,

        #undef OPTION_IDS_FEAT_GNB_PROPERTY_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPropertyOverrideBlockF15Cz;
        #define OPTION_IDS_FEAT_GNB_PROPERTY_F15_CZ &IdsFeatGnbPropertyOverrideBlockF15Cz,

        #undef OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPCIePowerGatingOverrideBlockF15Cz;
        #define OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_CZ &IdsFeatGnbPCIePowerGatingOverrideBlockF15Cz,

        #undef OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbSWTJOFFSETOverrideBlockF15Cz;
        #define OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_CZ &IdsFeatGnbSWTJOFFSETOverrideBlockF15Cz,

        #undef OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatSkipLockDramCfgF15Cz;
        #define OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_CZ &IdsFeatSkipLockDramCfgF15Cz,

        #undef OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatDisableCpuFeatF15Cz;
        #define OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_CZ &IdsFeatDisableCpuFeatF15Cz,

        #undef OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_CZ
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatCurrentLimitOverrideF15Cz;
        #define OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_CZ &IdsFeatCurrentLimitOverrideF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatAPIC330MaskConfigF15Cz;
        #undef OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_CZ
        #define OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_CZ &IdsFeatAPIC330MaskConfigF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatEccConfigurationF15Cz;
        #undef OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_CZ
        #define OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_CZ &IdsIntFeatEccConfigurationF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPmmSWDeviceIDRemappingF15Cz;
        #undef OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_CZ
        #define OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_CZ &IdsFeatGnbPmmSWDeviceIDRemappingF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatSkipMsrAccessOverrideF15Cz;
        #undef OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_CZ
        #define OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_CZ &IdsFeatSkipMsrAccessOverrideF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatSkipNpstF15Cz;
        #undef OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_CZ
        #define OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_CZ &IdsFeatSkipNpstF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPciePhyCfgF15Cz;
        #undef OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_CZ
        #define OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_CZ &IdsFeatGnbPciePhyCfgF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatDsmLpCfgF15Cz;
        #undef OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_CZ
        #define OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_CZ &IdsFeatDsmLpCfgF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbEqualizationPresetF15Cz;
        #undef OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_CZ
        #define OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_CZ &IdsFeatGnbEqualizationPresetF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbGen1LoopbackOverrideF15Cz;
        #undef OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_CZ
        #define OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_CZ &IdsFeatGnbGen1LoopbackOverrideF15Cz,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbSmuPort80ParamsOverrideF15Cz;
        #undef OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_CZ
        #define OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_CZ &IdsFeatGnbSmuPort80ParamsOverrideF15Cz,

        ///@todo check if CZ needs this
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbOverrideAdaptPiOffsetBypF15Cz;
        ///#undef OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_CZ
        //#define OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_CZ &IdsFeatGnbOverrideAdaptPiOffsetBypF15Cz,

        ///@todo check if CZ needs this
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbTargetTDPOverrideF15Cz;
        //#undef OPTION_IDS_FEAT_TARGET_TDP_F15_CZ
        //#define OPTION_IDS_FEAT_TARGET_TDP_F15_CZ &IdsFeatGnbTargetTDPOverrideF15Cz,

        ///@todo check if CZ needs this
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatFixupCustomPstatesOnWarmResetCz;
        //#undef OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_CZ
        //#define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_CZ &IdsFeatFixupCustomPstatesOnWarmResetCz,

      #endif
    #endif

/*----------------------------------------------------------------------------
 *                        Family 15 ST feat blocks
 *
 *----------------------------------------------------------------------------
 */
    #define OPTION_IDS_FEAT_RDRAND_F15_ST
    #define OPTION_IDS_FEAT_CORE_LEVELING_F15_ST
    #define OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_ST
    #define OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_ST
    #define OPTION_IDS_FEAT_INT_NBPSTATE_F15_ST
    #define OPTION_IDS_FEAT_INT_CPB_CTRL_F15_ST
    #define OPTION_IDS_FEAT_CSTATE_MODE_F15_ST
    #define OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_ST
    #define OPTION_IDS_FEAT_MEMTBL_DRV_F15_ST
    #define OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_ST
    #define OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_ST
    #define OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_ST
    #define OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_ST
    #define OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_ST
    #define OPTION_IDS_FEAT_GNB_PROPERTY_F15_ST
    #define OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_ST
    #define OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_ST
    #define OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_ST
    #define OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_ST
    #define OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_ST
    #define OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_ST
    #define OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_ST
    #define OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_ST
    #define OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_ST
    #define OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_ST
    #define OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_ST
    #define OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_ST
    #define OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_ST
    #define OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_ST
    #define OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_ST
    #define OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_ST
    #define OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_ST
    #define OPTION_IDS_FEAT_TARGET_TDP_F15_ST
    #define OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_ST
    #define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_ST
    #define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_ST

    #ifdef OPTION_FAMILY15H_ST
      #if OPTION_FAMILY15H_ST == TRUE
        #undef OPTION_IDS_FEAT_CORE_LEVELING_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCoreLevelingBlockF15St;
        #define OPTION_IDS_FEAT_CORE_LEVELING_F15_ST &IdsFeatCoreLevelingBlockF15St,

        #undef OPTION_IDS_FEAT_RDRAND_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatRdRandBlockF15St;
        #define OPTION_IDS_FEAT_RDRAND_F15_ST &IdsIntFeatRdRandBlockF15St,

        #undef OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatWatchdogTimerAndSyncFloodMcaBlockF15St;
        #define OPTION_IDS_FEAT_WATCHDOG_TIMER_F15_ST &IdsIntFeatWatchdogTimerAndSyncFloodMcaBlockF15St,

        #undef OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatExceptionBreakpointBlockF15St;
        #define OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT_F15_ST &IdsIntFeatExceptionBreakpointBlockF15St,

        #undef OPTION_IDS_FEAT_INT_NBPSTATE_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatNBPStatesSupportBlockF15St;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntFeatNbPsDisOverrideBlockF15St;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntSubNbPstateGetMinFreqBlockF15St;
        #define OPTION_IDS_FEAT_INT_NBPSTATE_F15_ST \
                 &IdsFeatNBPStatesSupportBlockF15St, \
                 &IdsIntFeatNbPsDisOverrideBlockF15St, \
                 &IdsIntSubNbPstateGetMinFreqBlockF15St,

        #undef OPTION_IDS_FEAT_INT_CPB_CTRL_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatCpbCtrlBlockF15St;
        #define OPTION_IDS_FEAT_INT_CPB_CTRL_F15_ST  &IdsIntFeatCpbCtrlBlockF15St,

        #undef OPTION_IDS_FEAT_CSTATE_MODE_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCStateModeBlockF15St;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatConfigIOCstatePostAfterBlockF15St;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGetCstBlockF15St;
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCreateCstBlockF15St;
        #define OPTION_IDS_FEAT_CSTATE_MODE_F15_ST \
                    &IdsFeatCStateModeBlockF15St, \
                    &IdsFeatConfigIOCstatePostAfterBlockF15St, \
                    &IdsFeatGetCstBlockF15St, \
                    &IdsFeatCreateCstBlockF15St,

        #undef OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_ST
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCacheFlushCtrlBlockF15St;
        #define OPTION_IDS_FEAT_CACHE_FLUSHCTRL_F15_ST &IdsFeatCacheFlushCtrlBlockF15St,

        #undef OPTION_IDS_FEAT_MEMTBL_DRV_F15_ST
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryTblDrvInitBlockF15St;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryOverrideMr0F15St;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryInitRegTblBlockF15St;
        #define OPTION_IDS_FEAT_MEMTBL_DRV_F15_ST \
                    &IdsFeatMemoryTblDrvInitBlockF15St, \
                    &IdsFeatMemoryOverrideMr0F15St, \
                    &IdsFeatMemoryInitRegTblBlockF15St,

        #undef OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatMemoryMappingPostBeforeBlockF15St;
        #define OPTION_IDS_FEAT_INT_MEMORY_MAPPING_F15_ST \
                    &IdsIntFeatMemoryMappingPostBeforeBlockF15St,

        #undef OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryEnforceMClkAboveNClkF15St;
        #define OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK_F15_ST \
                    &IdsFeatMemoryEnforceMClkAboveNClkF15St,

        #undef OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_ST
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyDllStandbyCtrlBlockF15St;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyDllStandbyCtrlBeforeDramInitBlockF15St;
        extern  CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyPowerSavingF15St;
        #define OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL_F15_ST \
                    &IdsFeatMemoryPhyDllStandbyCtrlBlockF15St, \
                    &IdsFeatMemoryPhyDllStandbyCtrlBeforeDramInitBlockF15St, \
                    &IdsFeatMemoryPhyPowerSavingF15St,

        #undef OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGetStretchFreqBlockF15St;
        #define OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT_F15_ST &IdsFeatGetStretchFreqBlockF15St,

        //GNB features
        #undef OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGNBORBDynamicWakeBlockF15St;
        #define OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE_F15_ST &IdsFeatGNBORBDynamicWakeBlockF15St,

        #undef OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPlatformCfgExBlockF15St;
        #define OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX_F15_ST &IdsFeatGnbPlatformCfgExBlockF15St,

        ///@todo check if ST needs this
        //#undef OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_ST
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbSmuServiceMskOverrideBlockF15St;
        //#define OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK_F15_ST &IdsFeatGnbSmuServiceMskOverrideBlockF15St,

        #undef OPTION_IDS_FEAT_GNB_PROPERTY_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPropertyOverrideBlockF15St;
        #define OPTION_IDS_FEAT_GNB_PROPERTY_F15_ST &IdsFeatGnbPropertyOverrideBlockF15St,

        #undef OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPCIePowerGatingOverrideBlockF15St;
        #define OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING_F15_ST &IdsFeatGnbPCIePowerGatingOverrideBlockF15St,

        #undef OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbSWTJOFFSETOverrideBlockF15St;
        #define OPTION_IDS_FEAT_GNB_SWTJOFFSET_F15_ST &IdsFeatGnbSWTJOFFSETOverrideBlockF15St,

        #undef OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatSkipLockDramCfgF15St;
        #define OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG_F15_ST &IdsFeatSkipLockDramCfgF15St,

        #undef OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatDisableCpuFeatF15St;
        #define OPTION_IDS_FEAT_DIS_CPU_FEAT_F15_ST &IdsFeatDisableCpuFeatF15St,

        #undef OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_ST
        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatCurrentLimitOverrideF15St;
        #define OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE_F15_ST &IdsFeatCurrentLimitOverrideF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatAPIC330MaskConfigF15St;
        #undef OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_ST
        #define OPTION_IDS_FEAT_AFTER_APIC_ENABLE_F15_ST &IdsFeatAPIC330MaskConfigF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatEccConfigurationF15St;
        #undef OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_ST
        #define OPTION_IDS_FEAT_ECC_CONFIGURATION_F15_ST &IdsIntFeatEccConfigurationF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPmmSWDeviceIDRemappingF15St;
        #undef OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_ST
        #define OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP_F15_ST &IdsFeatGnbPmmSWDeviceIDRemappingF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatSkipMsrAccessOverrideF15St;
        #undef OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_ST
        #define OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT_F15_ST &IdsFeatSkipMsrAccessOverrideF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatSkipNpstF15St;
        #undef OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_ST
        #define OPTION_IDS_FEAT_CTDP_NPST_FEAT_F15_ST &IdsFeatSkipNpstF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbPciePhyCfgF15St;
        #undef OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_ST
        #define OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG_F15_ST &IdsFeatGnbPciePhyCfgF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatDsmLpCfgF15St;
        #undef OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_ST
        #define OPTION_IDS_FEAT_DSM_LP_CONFIG_F15_ST &IdsFeatDsmLpCfgF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbEqualizationPresetF15St;
        #undef OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_ST
        #define OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG_F15_ST &IdsFeatGnbEqualizationPresetF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatGnbGen1LoopbackOverrideF15St;
        #undef OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_ST
        #define OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG_F15_ST &IdsFeatGnbGen1LoopbackOverrideF15St,

        extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbSmuPort80ParamsOverrideF15St;
        #undef OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_ST
        #define OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS_F15_ST &IdsFeatGnbSmuPort80ParamsOverrideF15St,

        ///@todo check if ST needs this
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbOverrideAdaptPiOffsetBypF15St;
        ///#undef OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_ST
        //#define OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN_F15_ST &IdsFeatGnbOverrideAdaptPiOffsetBypF15St,

        ///@todo check if ST needs this
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbTargetTDPOverrideF15St;
        //#undef OPTION_IDS_FEAT_TARGET_TDP_F15_ST
        //#define OPTION_IDS_FEAT_TARGET_TDP_F15_ST &IdsFeatGnbTargetTDPOverrideF15St,

        ///@todo check if ST needs this
        //extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatFixupCustomPstatesOnWarmResetSt;
        //#undef OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_ST
        //#define OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET_F15_ST &IdsFeatFixupCustomPstatesOnWarmResetSt,

      #endif
    #endif

/*----------------------------------------------------------------------------
 *                        Internal common feat blocks
 *
 *----------------------------------------------------------------------------
 */

    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntBlockEarlyAfter;
    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntBlockLaterAfter;
    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsIntEarlyBefore;
    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatMemoryDramTermBlock;
    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA  IdsFeatMemoryDynDramTermBlock;

    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGraInitEarly;
    extern CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGraInitAfterPost;

    /// Define feats which used in optionidsinstall.h
    #define OPTION_IDS_EXTEND_FEATS\
        OPTION_IDS_FEAT_SMU_PORT80_LOGGING_DISABLE \
        OPTION_IDS_FEAT_SMU_PORT80_LOGGING_ENABLE \
        &IdsFeatGraInitEarly, \
        &IdsFeatGraInitAfterPost, \
        &IdsIntBlockEarlyAfter, \
        &IdsIntBlockLaterAfter, \
        OPTION_IDS_FEAT_LATE_AFTER \
        &IdsIntEarlyBefore, \
        &IdsFeatMemoryDramTermBlock, \
        &IdsFeatMemoryDynDramTermBlock, \
        OPTION_IDS_FEAT_DIS_CPU_FEAT \
        OPTION_IDS_FEAT_CORE_LEVELING \
        OPTION_IDS_FEAT_RDRAND \
        OPTION_IDS_FEAT_WATCHDOG_TIMER \
        OPTION_IDS_FEAT_EXCEPTION_BREAKPOINT \
        OPTION_IDS_FEAT_INT_NBPSTATE \
        &IdsFeatAcpiPssBlock, \
        &IdsFeatAcpiCstBlock, \
        &IdsFeatAcpiWheaBlock, \
        &IdsFeatDmiBlock, \
        &IdsFeatPsiBlock, \
        OPTION_IDS_FEAT_INT_MEMORY_MAPPING \
        OPTION_IDS_FEAT_INT_MEMCLK_ABOVE_NCLK \
        &IdsFeatDctConfigBlock, \
        &IdsFeatMemoryTblDrvGetBlock, \
        OPTION_IDS_FEAT_MEMTBL_DRV \
        OPTION_IDS_FEAT_BANK_SWIZZLE \
        OPTION_IDS_FEAT_ECC_CONFIGURATION \
        &IdsFeatEnforceFreqBlock, \
        &IdsFeatEnforceStretchFreqBlock, \
        &IdsFeatMemPowerPolicyBlock, \
        &IdsFeatMemoryTimingBlock, \
        OPTION_IDS_FEAT_LOADCARD \
        OPTION_IDS_FEAT_MEMORY_PHYDLL_CTRL \
        OPTION_IDS_FEAT_CUSTOMPSTATE \
        &IdsFeatMemoryErrorRecoveryBlock, \
        &IdsFeatMemoryRetrainTimesBlock, \
        &IdsFeatMemoryMrlRetrainTimesBlock, \
        &IdsFeatSkipFusedMaxDdrRateBlock, \
        &IdsFeatEnforceVddIoBlock, \
        &IdsFeatIgnoreMemErrorBlock, \
        &IdsFeatHdtBreakPointBfDramInitBlock, \
        &IdsFeatHdtBreakPointBfMemClrBlock, \
        &IdsFeatHdtBreakPointBfGen2InitBlock, \
        &IdsFeatHdtBreakPointBfGppTrainingBlock, \
        &IdsFeatHdtBreakPointBfReConfigurationBlock, \
        &IdsFeatHdtBreakPointBfGen3InitBlock, \
        &IdsFeatHdtBreakPointBfPowerGatingBlock, \
        &IdsFeatRedirectShutDownToHdtBlock, \
        OPTION_IDS_FEAT_MEM_LOCK_DRAM_CFG \
        OPTION_IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE \
        OPTION_IDS_FEAT_GNB_PLATFORMCFG_EX \
        OPTION_IDS_FEAT_GNB_CLOCK_GATING \
        OPTION_IDS_FEAT_GNB_LCLK_DPM \
        OPTION_IDS_FEAT_GNB_LCLK_DEEPSLEEP \
        OPTION_IDS_FEAT_MAIN_PLL \
        OPTION_IDS_FEAT_NB_PSTATE_DIDVID \
        OPTION_IDS_FEAT_APM_TDP_CTRL \
        OPTION_IDS_FEAT_CSAMPLETIMER \
        OPTION_IDS_FEAT_PIPETHROTTLE \
        OPTION_IDS_FEAT_INT_CPB_CTRL \
        OPTION_IDS_FEAT_CACHE_FLUSHCTRL \
        OPTION_IDS_FEAT_CACHE_FLUSHMISCCTRL \
        OPTION_IDS_FEAT_CACHE_FLUSHXC6CTRL \
        OPTION_IDS_FEAT_CACHE_FLUSHRINSERCTRL \
        OPTION_IDS_FEAT_CURRENT_LIMIT_OVERRIDE \
        OPTION_IDS_FEAT_CSTATE_MODE \
        OPTION_IDS_FEAT_CC6_WORKAROUND \
        OPTION_IDS_FEAT_REG_TABLE \
        OPTION_IDS_FEAT_NB_BUFFER_ALLOCATION \
        OPTION_IDS_FEAT_ERRATUM463_WORAROUND \
        OPTION_IDS_FEAT_GNB_FORCE_CABLESAFE \
        OPTION_IDS_FEAT_GNB_PROPERTY \
        OPTION_IDS_FEAT_GNB_PCIE_POWER_GATING \
        OPTION_IDS_FEAT_GNB_SWTJOFFSET \
        OPTION_IDS_FEAT_GNB_PMMPCIEPHYISOLATION \
        OPTION_IDS_FEAT_GNB_GPPPCIEMASTERPLLSELECTION \
        OPTION_IDS_FEAT_GNB_PMM_PORT_REMAP \
        OPTION_IDS_FEAT_GNB_PMM_NATIVE_GEN1_PLL \
        OPTION_IDS_FEAT_GNB_ALTVDDNB \
        OPTION_IDS_FEAT_DLL_CSR \
        OPTION_IDS_FEAT_MEMORY_GET_STRETCH_LIMIT \
        OPTION_IDS_FEAT_MSR_ACCESS_OVERRIDE_FEAT \
        OPTION_IDS_FEAT_CTDP_NPST_FEAT \
        OPTION_IDS_FEAT_DLLSTAGGERDLY \
        OPTION_IDS_FEAT_GNB_LOAD_SAMU_PATCH \
        OPTION_IDS_FEAT_AFTER_APIC_ENABLE \
        OPTION_IDS_FEAT_PCIE_PHYLANE_CONFIG \
        OPTION_IDS_FEAT_DSM_LP_CONFIG \
        OPTION_IDS_FEAT_EQUAL_PRESET_CONFIG \
        OPTION_IDS_FEAT_GEN1_LOOPBACK_CONFIG \
        OPTION_IDS_FEAT_GNB_SMU_SERVICE_MASK \
        OPTION_IDS_FEAT_ADAPT_PI_OFFSET_BYP_EN \
        OPTION_IDS_FEAT_TARGET_TDP \
        OPTION_IDS_FEAT_GNB_SMU_PORT80_PARAMS \
        OPTION_IDS_FIXUP_CUSTOM_PSTATES_ON_WARM_RESET
  #endif
#endif // IDSOPT_CONTROL_ENABLED == TRUE

#endif
