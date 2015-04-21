/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of build option: GNB
 *
 * Contains AMD AGESA install macros and test conditions. Output is the
 * defaults tables reflecting the User's build options selection.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Options
 * @e \$Revision: 312286 $   @e \$Date: 2015-02-04 14:11:11 +0800 (Wed, 04 Feb 2015) $
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

#ifndef _OPTION_GNB_INSTALL_H_
#define _OPTION_GNB_INSTALL_H_

#include "S3SaveState.h"
/*  This option is designed to be included into the platform solution install
 *  file. The platform solution install file will define the options status.
 *  Check to validate the definition
 */

//---------------------------------------------------------------------------------------------------
// Family installation
//---------------------------------------------------------------------------------------------------

#define GNB_TYPE_CZ   FALSE
#define GNB_TYPE_ST   FALSE

#if (OPTION_FAMILY15H_CZ == TRUE)
  #undef  GNB_TYPE_CZ
  #define GNB_TYPE_CZ  TRUE
#endif

#if (OPTION_FAMILY15H_ST == TRUE)
  #undef  GNB_TYPE_ST
  #define GNB_TYPE_ST  TRUE
#endif

#if (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
//---------------------------------------------------------------------------------------------------
// Service installation
//---------------------------------------------------------------------------------------------------

  #include "Gnb.h"
  #include "GnbPcie.h"
  #include "GnbGfx.h"

  #define SERVICES_POINTER  NULL
  #if (GNB_TYPE_CZ == TRUE)
    #include "GnbInitCZInstall.h"
  #endif
  #if (GNB_TYPE_ST == TRUE)
    #include "GnbInitSTInstall.h"
  #endif
  GNB_SERVICE   *ServiceTable = SERVICES_POINTER;

//---------------------------------------------------------------------------------------------------
// Common BUILD options
// These Common Build options apply to all families
//---------------------------------------------------------------------------------------------------

  #ifndef CFG_IGFX_AS_PCIE_EP
    #define CFG_IGFX_AS_PCIE_EP                           TRUE
  #endif

  #ifndef CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING
    #define CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING  (60 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_L0_POOLING
    #define CFG_GNB_PCIE_LINK_L0_POOLING                  (60 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME
    #define CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME      (2 * 1000)
  #endif

  #ifndef CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME
    #define CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME      (2 * 1000)
  #endif

  #ifdef BLDCFG_PCIE_TRAINING_ALGORITHM
    #define CFG_GNB_PCIE_TRAINING_ALGORITHM               BLDCFG_PCIE_TRAINING_ALGORITHM
  #else
    #define CFG_GNB_PCIE_TRAINING_ALGORITHM               PcieTrainingStandard
  #endif

  #ifndef CFG_GNB_PCIE_POWERGATING_FLAGS
    #define CFG_GNB_PCIE_POWERGATING_FLAGS                0
  #endif

  #ifndef CFG_PCIE_ASPM_BLACK_LIST_ENABLE
    #define CFG_PCIE_ASPM_BLACK_LIST_ENABLE               TRUE
  #endif

  #ifndef CFG_MAX_PAYLOAD_ENABLE
    #define CFG_MAX_PAYLOAD_ENABLE                        TRUE
  #endif

  #ifndef CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT
    #define CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT      FALSE
  #endif

  #ifndef CFG_GNB_LOAD_REAL_FUSE
    #define  CFG_GNB_LOAD_REAL_FUSE                       TRUE
  #endif

  GNB_BUILD_OPTIONS ROMDATA GnbBuildOptions = {
    CFG_IGFX_AS_PCIE_EP,
    CFG_GNB_PCIE_LINK_RECEIVER_DETECTION_POOLING,
    CFG_GNB_PCIE_LINK_L0_POOLING,
    CFG_GNB_PCIE_LINK_GPIO_RESET_ASSERT_TIME,
    CFG_GNB_PCIE_LINK_RESET_TO_TRAINING_TIME,
    CFG_GNB_PCIE_TRAINING_ALGORITHM,
    CFG_GNB_PCIE_POWERGATING_FLAGS,
    CFG_PCIE_ASPM_BLACK_LIST_ENABLE,
    CFG_MAX_PAYLOAD_ENABLE,
    CFG_GNB_IVRS_RELATIVE_ADDR_NAMES_SUPPORT,
    CFG_ACPI_SET_OEM_ID,
    CFG_ACPI_SET_OEM_TABLE_ID,
    CFG_GNB_LOAD_REAL_FUSE
  };

//---------------------------------------------------------------------------------------------------
// Family-specific common BUILD options
// The build options are used in all families, but may be configured differently for different families
// CFG_XXXX will override default value if CFG_XXXX is defined
// CFG_XXXX_FM will override CFG_XXXX if CFG_XXXX_FM is defined (Where _FM is family, _CZ, _ST, ...
// If neither CFG_XXXX_FM or CFG_XXXX is defined, then the family specific default will be defined
// within the family-specific definition areas below
//---------------------------------------------------------------------------------------------------

  #ifdef CFG_SCS_SUPPORT
    #ifndef CFG_SCS_SUPPORT_CZ
      #define CFG_SCS_SUPPORT_CZ                            CFG_SCS_SUPPORT
    #endif
    #ifndef CFG_SCS_SUPPORT_ST
      #define CFG_SCS_SUPPORT_ST                            CFG_SCS_SUPPORT
    #endif
  #endif

  #ifndef CFG_SMU_MESSAGE_ENABLE
    #define CFG_SMU_MESSAGE_ENABLE                          TRUE
  #endif

//---------------------------------------------------------------------------------------------------
// CZ-specific common BUILD options
//---------------------------------------------------------------------------------------------------
  #if (GNB_TYPE_CZ == TRUE)

    // Common configuration defaults for CZ
    #ifndef CFG_SCS_SUPPORT_CZ
      #define CFG_SCS_SUPPORT_CZ                        TRUE
    #endif

    #ifndef CFG_UMA_STEERING_CZ
      #define CFG_UMA_STEERING_CZ                       DefaultGarlic
    #endif

    #ifndef CFG_GMC_POWER_GATING_CZ
      #define CFG_GMC_POWER_GATING_CZ                   GmcPowerGatingWithStutter
    #endif

    #ifndef CFG_GMC_CLOCK_GATING_CZ
      #define CFG_GMC_CLOCK_GATING_CZ                   TRUE
    #endif

    #ifndef CFG_ORB_DYN_WAKE_ENABLE_CZ
      #define CFG_ORB_DYN_WAKE_ENABLE_CZ                TRUE
    #endif

    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_CZ
      #define CFG_ORB_CLOCK_GATING_ENABLE_CZ            TRUE
    #endif

    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_CZ
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_CZ       TRUE
    #endif

    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_CZ
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_CZ       TRUE
    #endif

//    #ifndef CFG_LCLK_DEEP_SLEEP_EN_CZ
//      #define CFG_LCLK_DEEP_SLEEP_EN_CZ                 TRUE
//    #endif

//    #ifndef CFG_LCLK_DPM_EN_CZ
//      #define CFG_LCLK_DPM_EN_CZ                        TRUE
//    #endif

    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_CZ
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_CZ       TRUE
    #endif

    #ifndef CFG_GNB_BAPM_SUPPORT_CZ
      #define CFG_GNB_BAPM_SUPPORT_CZ                   CFG_BAPM_ENABLE
    #endif

    // CZ-only configuration defaults
//    #ifndef CFG_NBDPM_ENABLE_CZ
//      #define CFG_NBDPM_ENABLE_CZ                       FALSE
//    #endif

    #ifndef CFG_CHUB_CLOCK_GATING_CZ
      #define CFG_CHUB_CLOCK_GATING_CZ                  TRUE
    #endif

    #ifndef CFG_ACP_CLOCK_GATING_CZ
      #define CFG_ACP_CLOCK_GATING_CZ                   TRUE
    #endif


//    #ifndef CFG_SMU_SERVICE_BIT_MAP_CZ
//      #define CFG_SMU_SERVICE_BIT_MAP_CZ                0xFFFFFFFFul
//    #endif

    #ifndef CFG_AXG_DISABLE_CZ
      #define CFG_AXG_DISABLE_CZ                        FALSE
    #endif

    #ifndef CFG_GNB_NUM_DISPLAY_STREAM_PIPES_CZ
      #define  CFG_GNB_NUM_DISPLAY_STREAM_PIPES_CZ         3
    #endif

    #ifndef CFG_PCIE_ARI_SUPPORT_CZ
      #define CFG_PCIE_ARI_SUPPORT_CZ                      FALSE
    #endif

    #ifndef CFG_PCIE_HW_INIT_PWR_GATING_CZ
      #define CFG_PCIE_HW_INIT_PWR_GATING_CZ            PcieHwInitPwrGatingEnableAll
    #endif

    #ifndef CFG_SPG_CLOCK_GATING_ENABLE_CZ
      #define CFG_SPG_CLOCK_GATING_ENABLE_CZ               TRUE
    #endif

    #ifndef CFG_ACG_AZ_CLOCK_GATING_ENABLE_CZ
      #define CFG_ACG_AZ_CLOCK_GATING_ENABLE_CZ               TRUE
    #endif

    #ifndef CFG_IOMMU_L1_MEM_POWER_GATING_CZ
      #define CFG_IOMMU_L1_MEM_POWER_GATING_CZ               TRUE
    #endif

    #ifndef CFG_IOMMU_L2_MEM_POWER_GATING_CZ
      #define CFG_IOMMU_L2_MEM_POWER_GATING_CZ               FALSE
    #endif

    #ifndef CFG_ORB_TX_MEM_POWER_GATING_CZ
      #define CFG_ORB_TX_MEM_POWER_GATING_CZ               OrbMemPowerGatingLS
    #endif

    #ifndef CFG_ORB_RX_MEM_POWER_GATING_CZ
      #define CFG_ORB_RX_MEM_POWER_GATING_CZ               OrbMemPowerGatingLS
    #endif

    #ifndef CFG_SSTUNL_CLK_GATING_CZ
      #define CFG_SSTUNL_CLK_GATING_CZ                     TRUE
    #endif

    #ifndef CFG_PARITY_ERROR_CONFIGURATION_CZ
      #define CFG_PARITY_ERROR_CONFIGURATION_CZ            FALSE
    #endif

    #ifndef CFG_SPG_MEM_POWER_GATING_CZ
      #define CFG_SPG_MEM_POWER_GATING_CZ            SPGACGAZMemPowerGatingLS
    #endif

    #ifndef CFG_ACG_AZ_MEM_POWER_GATING_CZ
      #define CFG_ACG_AZ_MEM_POWER_GATING_CZ            SPGACGAZMemPowerGatingLS
    #endif

    #ifndef CFG_BIF_MEM_SD_EN_CZ
      #define CFG_BIF_MEM_SD_EN_CZ                     TRUE
    #endif

    #ifndef CFG_SDMA_MEM_POWER_GATING_CZ
      #define CFG_SDMA_MEM_POWER_GATING_CZ             TRUE
    #endif

    #ifndef CFG_PCIE_TX_POWER_INOFF_CZ
      #define CFG_PCIE_TX_POWER_INOFF_CZ               3
    #endif

    #ifndef CFG_PCIE_RX_POWER_INOFF_CZ
      #define CFG_PCIE_RX_POWER_INOFF_CZ               3
    #endif

    #ifndef CFG_SMU_DETERMINISM_AMBIENT_CZ
      #define CFG_SMU_DETERMINISM_AMBIENT_CZ               0
    #endif

    #ifndef CFG_BTC_ENABLE_CZ
      #define CFG_BTC_ENABLE_CZ                     TRUE
    #endif

    #ifndef CFG_PSI_ENABLE_CZ
      #define CFG_PSI_ENABLE_CZ                     TRUE
    #endif

    GNB_BUILD_OPTIONS_CZ ROMDATA GnbBuildOptionsCZ = {
      {                                                 // GNB_BUILD_OPTIONS_COMMON
        CFG_SCS_SUPPORT_CZ,
        CFG_UMA_STEERING_CZ,
        CFG_GMC_POWER_GATING_CZ,
        CFG_GMC_CLOCK_GATING_CZ,
        CFG_ORB_DYN_WAKE_ENABLE_CZ,
        CFG_ORB_CLOCK_GATING_ENABLE_CZ,
        CFG_IOMMU_L1_CLOCK_GATING_ENABLE_CZ,
        CFG_IOMMU_L2_CLOCK_GATING_ENABLE_CZ,
//        CFG_LCLK_DEEP_SLEEP_EN_CZ,
//        CFG_LCLK_DPM_EN_CZ,
        CFG_IOC_LCLK_CLOCK_GATING_ENABLE_CZ,
        CFG_GNB_BAPM_SUPPORT_CZ,
        FALSE,
        CFG_GNB_NUM_DISPLAY_STREAM_PIPES_CZ,
        CFG_SMU_MESSAGE_ENABLE
      },
//      CFG_NBDPM_ENABLE_CZ,
      CFG_CHUB_CLOCK_GATING_CZ,
      CFG_ACP_CLOCK_GATING_CZ,
      {
        CFG_GNB_BAPM_SUPPORT_CZ,  ///<               BAPM
        1,  ///<    POWER_ESTIMATOR
        1,  ///< THERMAL_CONTROLLER
        1,  ///<       TDC_LIMITING
        1,  ///<   PKG_PWR_LIMITING
        1,  ///<           LCLK_DPM
        1,  ///< VOLTAGE_CONTROLLER
        1,  ///<              GEAPM
        1,  ///<                BBB
        1,  ///<                IBS
        1,  ///<     PSTATE_ARBITER
        1,  ///<             NB_DPM
        1,  ///<    LCLK_DEEP_SLEEP
        1,  ///<                ULV
        1,  ///<          GFX_CU_PG
        0,  ///<               AVFS
        0,//1,  ///<            UVD_DPM
        1,  ///<      VQ_CONTROLLER
        0,//1,  ///<            ACP_DPM
        0,//1,  ///<           SCLK_DPM
        1,  ///<     PRE_FETCH_DATA
        1,  ///<         DFS_BYPASS
        1,  ///<                HTC
        0,//1,  ///<             VDDGFX
        1,  ///<              LEAPM
        1,  ///<         CONN_STDBY
        0,//1,  ///<     PORT80_MONITOR
        1,  ///<         PSPCLK_DPM
        1,  ///<              STAPM
        0,//1,  ///<    DETERMINISM_MASK
        1,  ///<      CPU_STRETCHER
        0,//1,  ///< SCLK_STRETCHER_MASK
      },
      CFG_AXG_DISABLE_CZ,
      CFG_PCIE_HW_INIT_PWR_GATING_CZ,
      CFG_PCIE_ARI_SUPPORT_CZ,
      CFG_SPG_CLOCK_GATING_ENABLE_CZ,
      CFG_ACG_AZ_CLOCK_GATING_ENABLE_CZ,
      CFG_IOMMU_L1_MEM_POWER_GATING_CZ,
      CFG_IOMMU_L2_MEM_POWER_GATING_CZ,
      CFG_ORB_TX_MEM_POWER_GATING_CZ,
      CFG_ORB_RX_MEM_POWER_GATING_CZ,
      CFG_SSTUNL_CLK_GATING_CZ,
      CFG_PARITY_ERROR_CONFIGURATION_CZ,
      CFG_SPG_MEM_POWER_GATING_CZ,
      CFG_ACG_AZ_MEM_POWER_GATING_CZ,
      CFG_BIF_MEM_SD_EN_CZ,
      CFG_SDMA_MEM_POWER_GATING_CZ,
      CFG_PCIE_TX_POWER_INOFF_CZ,
      CFG_PCIE_RX_POWER_INOFF_CZ,
      CFG_SMU_DETERMINISM_AMBIENT_CZ,
      CFG_BTC_ENABLE_CZ,
      CFG_PSI_ENABLE_CZ
    };
  #endif

//---------------------------------------------------------------------------------------------------
// ST-specific common BUILD options
//---------------------------------------------------------------------------------------------------
  #if (GNB_TYPE_ST == TRUE)

    // Common configuration defaults for ST
    #ifndef CFG_SCS_SUPPORT_ST
      #define CFG_SCS_SUPPORT_ST                        TRUE
    #endif

    #ifndef CFG_UMA_STEERING_ST
      #define CFG_UMA_STEERING_ST                       DefaultGarlic
    #endif

    #ifndef CFG_GMC_POWER_GATING_ST
      #define CFG_GMC_POWER_GATING_ST                   GmcPowerGatingWithStutter
    #endif

    #ifndef CFG_GMC_CLOCK_GATING_ST
      #define CFG_GMC_CLOCK_GATING_ST                   TRUE
    #endif

    #ifndef CFG_ORB_DYN_WAKE_ENABLE_ST
      #define CFG_ORB_DYN_WAKE_ENABLE_ST                TRUE
    #endif

    #ifndef CFG_ORB_CLOCK_GATING_ENABLE_ST
      #define CFG_ORB_CLOCK_GATING_ENABLE_ST            TRUE
    #endif

    #ifndef CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ST
      #define CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ST       TRUE
    #endif

    #ifndef CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ST
      #define CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ST       TRUE
    #endif

//    #ifndef CFG_LCLK_DEEP_SLEEP_EN_ST
//      #define CFG_LCLK_DEEP_SLEEP_EN_ST                 TRUE
//    #endif

//    #ifndef CFG_LCLK_DPM_EN_ST
//      #define CFG_LCLK_DPM_EN_ST                        TRUE
//    #endif

    #ifndef CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ST
      #define CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ST       TRUE
    #endif

    #ifndef CFG_GNB_BAPM_SUPPORT_ST
      #define CFG_GNB_BAPM_SUPPORT_ST                   CFG_BAPM_ENABLE
    #endif

    // CZ-only configuration defaults
//    #ifndef CFG_NBDPM_ENABLE_ST
//      #define CFG_NBDPM_ENABLE_ST                       FALSE
//    #endif

    #ifndef CFG_CHUB_CLOCK_GATING_ST
      #define CFG_CHUB_CLOCK_GATING_ST                  TRUE
    #endif

    #ifndef CFG_ACP_CLOCK_GATING_ST
      #define CFG_ACP_CLOCK_GATING_ST                   TRUE
    #endif


//    #ifndef CFG_SMU_SERVICE_BIT_MAP_ST
//      #define CFG_SMU_SERVICE_BIT_MAP_ST                0xFFFFFFFFul
//    #endif

    #ifndef CFG_AXG_DISABLE_ST
      #define CFG_AXG_DISABLE_ST                        FALSE
    #endif

    #ifndef CFG_GNB_NUM_DISPLAY_STREAM_PIPES_ST
      #define  CFG_GNB_NUM_DISPLAY_STREAM_PIPES_ST         3
    #endif

    #ifndef CFG_PCIE_ARI_SUPPORT_ST
      #define CFG_PCIE_ARI_SUPPORT_ST                      FALSE
    #endif

    #ifndef CFG_PCIE_HW_INIT_PWR_GATING_ST
      #define CFG_PCIE_HW_INIT_PWR_GATING_ST            PcieHwInitPwrGatingEnableAll
    #endif

    #ifndef CFG_SPG_CLOCK_GATING_ENABLE_ST
      #define CFG_SPG_CLOCK_GATING_ENABLE_ST               TRUE
    #endif

    #ifndef CFG_ACG_AZ_CLOCK_GATING_ENABLE_ST
      #define CFG_ACG_AZ_CLOCK_GATING_ENABLE_ST               TRUE
    #endif

    #ifndef CFG_IOMMU_L1_MEM_POWER_GATING_ST
      #define CFG_IOMMU_L1_MEM_POWER_GATING_ST               TRUE
    #endif

    #ifndef CFG_IOMMU_L2_MEM_POWER_GATING_ST
      #define CFG_IOMMU_L2_MEM_POWER_GATING_ST               FALSE
    #endif

    #ifndef CFG_ORB_TX_MEM_POWER_GATING_ST
      #define CFG_ORB_TX_MEM_POWER_GATING_ST               OrbMemPowerGatingLS
    #endif

    #ifndef CFG_ORB_RX_MEM_POWER_GATING_ST
      #define CFG_ORB_RX_MEM_POWER_GATING_ST               OrbMemPowerGatingLS
    #endif

    #ifndef CFG_SSTUNL_CLK_GATING_ST
      #define CFG_SSTUNL_CLK_GATING_ST                     TRUE
    #endif

    #ifndef CFG_PARITY_ERROR_CONFIGURATION_ST
      #define CFG_PARITY_ERROR_CONFIGURATION_ST            FALSE
    #endif

    #ifndef CFG_SPG_MEM_POWER_GATING_ST
      #define CFG_SPG_MEM_POWER_GATING_ST            SPGACGAZMemPowerGatingLS
    #endif

    #ifndef CFG_ACG_AZ_MEM_POWER_GATING_ST
      #define CFG_ACG_AZ_MEM_POWER_GATING_ST            SPGACGAZMemPowerGatingLS
    #endif

    #ifndef CFG_BIF_MEM_SD_EN_ST
      #define CFG_BIF_MEM_SD_EN_ST                     TRUE
    #endif

    #ifndef CFG_SDMA_MEM_POWER_GATING_ST
      #define CFG_SDMA_MEM_POWER_GATING_ST             TRUE
    #endif

    #ifndef CFG_PCIE_TX_POWER_INOFF_ST
      #define CFG_PCIE_TX_POWER_INOFF_ST               3
    #endif

    #ifndef CFG_PCIE_RX_POWER_INOFF_ST
      #define CFG_PCIE_RX_POWER_INOFF_ST               3
    #endif

    #ifndef CFG_SMU_DETERMINISM_AMBIENT_ST
      #define CFG_SMU_DETERMINISM_AMBIENT_ST               0
    #endif

    #ifndef CFG_BTC_ENABLE_ST
      #define CFG_BTC_ENABLE_ST                     TRUE
    #endif

    GNB_BUILD_OPTIONS_ST ROMDATA GnbBuildOptionsST = {
      {                                                 // GNB_BUILD_OPTIONS_COMMON
        CFG_SCS_SUPPORT_ST,
        CFG_UMA_STEERING_ST,
        CFG_GMC_POWER_GATING_ST,
        CFG_GMC_CLOCK_GATING_ST,
        CFG_ORB_DYN_WAKE_ENABLE_ST,
        CFG_ORB_CLOCK_GATING_ENABLE_ST,
        CFG_IOMMU_L1_CLOCK_GATING_ENABLE_ST,
        CFG_IOMMU_L2_CLOCK_GATING_ENABLE_ST,
//        CFG_LCLK_DEEP_SLEEP_EN_ST,
//        CFG_LCLK_DPM_EN_ST,
        CFG_IOC_LCLK_CLOCK_GATING_ENABLE_ST,
        CFG_GNB_BAPM_SUPPORT_ST,
        FALSE,
        CFG_GNB_NUM_DISPLAY_STREAM_PIPES_ST,
        CFG_SMU_MESSAGE_ENABLE
      },
//      CFG_NBDPM_ENABLE_ST,
      CFG_CHUB_CLOCK_GATING_ST,
      CFG_ACP_CLOCK_GATING_ST,
      {
        CFG_GNB_BAPM_SUPPORT_ST,  ///<               BAPM
        1,  ///<    POWER_ESTIMATOR
        1,  ///< THERMAL_CONTROLLER
        1,  ///<       TDC_LIMITING
        1,  ///<   PKG_PWR_LIMITING
        1,  ///<           LCLK_DPM
        1,  ///< VOLTAGE_CONTROLLER
        1,  ///<              GEAPM
        1,  ///<                BBB
        1,  ///<                IBS
        1,  ///<     PSTATE_ARBITER
        1,  ///<             NB_DPM
        1,  ///<    LCLK_DEEP_SLEEP
        1,  ///<                ULV
        1,  ///<          GFX_CU_PG
        1,  ///<               AVFS
        0,//1,  ///<            UVD_DPM
        0,//1,  ///<               CHTC
        0,//1,  ///<            ACP_DPM
        0,//1,  ///<           SCLK_DPM
        1,  ///<     PRE_FETCH_DATA
        1,  ///<         DFS_BYPASS
        1,  ///<                HTC
        0,//1,  ///<             VDDGFX
        1,  ///<              LEAPM
        1,  ///<         CONN_STDBY
        0,//1,  ///<     PORT80_MONITOR
        1,  ///<         PSPCLK_DPM
        1,  ///<              STAPM
        0,//1,  ///<    DETERMINISM_MASK
        1,  ///<      CPU_STRETCHER
        0,//1,  ///< SCLK_STRETCHER_MASK
      },
      CFG_AXG_DISABLE_ST,
      CFG_PCIE_HW_INIT_PWR_GATING_ST,
      CFG_PCIE_ARI_SUPPORT_ST,
      CFG_SPG_CLOCK_GATING_ENABLE_ST,
      CFG_ACG_AZ_CLOCK_GATING_ENABLE_ST,
      CFG_IOMMU_L1_MEM_POWER_GATING_ST,
      CFG_IOMMU_L2_MEM_POWER_GATING_ST,
      CFG_ORB_TX_MEM_POWER_GATING_ST,
      CFG_ORB_RX_MEM_POWER_GATING_ST,
      CFG_SSTUNL_CLK_GATING_ST,
      CFG_PARITY_ERROR_CONFIGURATION_ST,
      CFG_SPG_MEM_POWER_GATING_ST,
      CFG_ACG_AZ_MEM_POWER_GATING_ST,
      CFG_BIF_MEM_SD_EN_ST,
      CFG_SDMA_MEM_POWER_GATING_ST,
      CFG_PCIE_TX_POWER_INOFF_ST,
      CFG_PCIE_RX_POWER_INOFF_ST,
      CFG_SMU_DETERMINISM_AMBIENT_ST,
      CFG_BTC_ENABLE_ST
    };
  #endif

  //---------------------------------------------------------------------------------------------------
  // Module entries
  //---------------------------------------------------------------------------------------------------

  #if (AGESA_ENTRY_INIT_EARLY == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_EARLY_INIT
      #define OPTION_NB_EARLY_INIT TRUE
    #endif
    #if (OPTION_NB_EARLY_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlyInterfaceCZ;
      #define OPTION_GNBEARLYINTERFACECZ_ENTRY            {AMD_FAMILY_CZ, GnbEarlyInterfaceCZ, TpGnbEarlyInterface},
    #else
      #define OPTION_GNBEARLYINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_EARLY_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlyInterfaceST;
      #define OPTION_GNBEARLYINTERFACEST_ENTRY            {AMD_FAMILY_ST, GnbEarlyInterfaceST, TpGnbEarlyInterface},
    #else
      #define OPTION_GNBEARLYINTERFACEST_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CONFIG_MAP
      #define OPTION_PCIE_CONFIG_MAP TRUE
    #endif
    #if (OPTION_PCIE_CONFIG_MAP == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                 PcieConfigurationMap;
      #define OPTION_PCIECONFIGURATIONMAP_ENTRY          {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieConfigurationMap, TpGnbPcieConfigurationMap},
    #else
      #define OPTION_PCIECONFIGURATIONMAP_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_EARLY_INIT
      #define OPTION_PCIE_EARLY_INIT TRUE
    #endif
    #if (OPTION_PCIE_EARLY_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  PcieEarlyInterfaceCZ;
      #define OPTION_PCIEEARLYINTERFACECZ_ENTRY           {AMD_FAMILY_CZ, PcieEarlyInterfaceCZ, TpGnbPcieEarlyInterface},
    #else
      #define OPTION_PCIEEARLYINTERFACECZ_ENTRY
    #endif
    #if (OPTION_PCIE_EARLY_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieEarlyInterfaceST;
      #define OPTION_PCIEEARLYINTERFACEST_ENTRY           {AMD_FAMILY_ST, PcieEarlyInterfaceST, TpGnbPcieEarlyInterface},
    #else
      #define OPTION_PCIEEARLYINTERFACEST_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbEarlyFeatureTable[] = {
      OPTION_GNBEARLYINTERFACECZ_ENTRY
      OPTION_GNBEARLYINTERFACEST_ENTRY
      OPTION_PCIECONFIGURATIONMAP_ENTRY
      OPTION_PCIEEARLYINTERFACECZ_ENTRY
      OPTION_PCIEEARLYINTERFACEST_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CONFIG_INIT
      #define OPTION_PCIE_CONFIG_INIT TRUE
    #endif
    #if (OPTION_PCIE_CONFIG_INIT == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieConfigurationInit;
      #define OPTION_PCIECONFIGURATIONINIT_ENTRY          {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieConfigurationInit, TpGnbEarlierPcieConfigurationInit},
    #else
      #define OPTION_PCIECONFIGURATIONINIT_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_EARLIER_INIT
      #define OPTION_NB_EARLIER_INIT TRUE
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlierInterfaceCZ;
      #define OPTION_GNBEARLIERINTERFACECZ_ENTRY          {AMD_FAMILY_CZ, GnbEarlierInterfaceCZ, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBEARLIERINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbEarlierInterfaceST;
      #define OPTION_GNBEARLIERINTERFACEST_ENTRY          {AMD_FAMILY_ST, GnbEarlierInterfaceST, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBEARLIERINTERFACEST_ENTRY
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuEarlyInterfaceCZ;
      #define OPTION_GNBSMUEARLYINTERFACECZ_ENTRY              {AMD_FAMILY_CZ, GnbSmuEarlyInterfaceCZ, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBSMUEARLYINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuEarlyInterfaceST;
      #define OPTION_GNBSMUEARLYINTERFACEST_ENTRY              {AMD_FAMILY_ST, GnbSmuEarlyInterfaceST, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBSMUEARLYINTERFACEST_ENTRY
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbScsInterfaceCZ;
      #define OPTION_GNBSCSINTERFACECZ_ENTRY              {AMD_FAMILY_CZ, GnbScsInterfaceCZ, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBSCSINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_EARLIER_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbScsInterfaceST;
      #define OPTION_GNBSCSINTERFACEST_ENTRY              {AMD_FAMILY_ST, GnbScsInterfaceST, TpGnbEarlierInterface},
    #else
      #define OPTION_GNBSCSINTERFACEST_ENTRY
    #endif

    OPTION_GNB_CONFIGURATION  GnbEarlierFeatureTable[] = {
      OPTION_PCIECONFIGURATIONINIT_ENTRY
      OPTION_GNBEARLIERINTERFACECZ_ENTRY
      OPTION_GNBEARLIERINTERFACEST_ENTRY
      OPTION_GNBSMUEARLYINTERFACECZ_ENTRY
      OPTION_GNBSMUEARLYINTERFACEST_ENTRY
      OPTION_GNBSCSINTERFACECZ_ENTRY
      OPTION_GNBSCSINTERFACEST_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_POST == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_POST_INIT
      #define OPTION_GFX_CONFIG_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_POST_INIT == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxConfigPostInterface;
      #define OPTION_GFXCONFIGPOSTINTERFACE_ENTRY         {AMD_FAMILY_CZ | AMD_FAMILY_ST, GfxConfigPostInterface, TpGnbGfxConfigPostInterface},
    #else
      #define OPTION_GFXCONFIGPOSTINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_POST_INIT
      #define OPTION_GFX_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_POST_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GfxPostInterfaceCZ;
      #define OPTION_GFXPOSTINTERFACECZ_ENTRY             {AMD_FAMILY_CZ, GfxPostInterfaceCZ, TpGnbGfxPostInterface},
    #else
      #define OPTION_GFXPOSTINTERFACECZ_ENTRY
    #endif
    #if (OPTION_GFX_POST_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxPostInterfaceST;
      #define OPTION_GFXPOSTINTERFACEST_ENTRY             {AMD_FAMILY_ST, GfxPostInterfaceST, TpGnbGfxPostInterface},
    #else
      #define OPTION_GFXPOSTINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_POST_INIT
      #define OPTION_NB_POST_INIT TRUE
    #endif
    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbPostInterfaceCZ;
      #define OPTION_GNBPOSTINTERFACECZ_ENTRY             {AMD_FAMILY_CZ, GnbPostInterfaceCZ, TpGnbPostInterface},
    #else
      #define OPTION_GNBPOSTINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbPostInterfaceST;
      #define OPTION_GNBPOSTINTERFACEST_ENTRY             {AMD_FAMILY_ST, GnbPostInterfaceST, TpGnbPostInterface},
    #else
      #define OPTION_GNBPOSTINTERFACEST_ENTRY
    #endif

    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuPostInterfaceCZ;
      #define OPTION_GNBSMUPOSTINTERFACECZ_ENTRY             {AMD_FAMILY_CZ, GnbSmuPostInterfaceCZ, TpGnbPostInterface},
    #else
      #define OPTION_GNBSMUPOSTINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_POST_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuPostInterfaceST;
      #define OPTION_GNBSMUPOSTINTERFACEST_ENTRY             {AMD_FAMILY_ST, GnbSmuPostInterfaceST, TpGnbPostInterface},
    #else
      #define OPTION_GNBSMUPOSTINTERFACEST_ENTRY
    #endif
    //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_POST_EARLY_INIT
      #define OPTION_PCIE_POST_EARLY_INIT TRUE
    #endif
    #if (OPTION_PCIE_POST_EARLY_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  PciePostEarlyInterfaceCZ;
      #define OPTION_PCIEPOSTEARLYINTERFACECZ_ENTRY       {AMD_FAMILY_CZ, PciePostEarlyInterfaceCZ, TpGnbPciePostEarlyInterface},
    #else
      #define OPTION_PCIEPOSTEARLYINTERFACECZ_ENTRY
    #endif
    #if (OPTION_PCIE_POST_EARLY_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PciePostEarlyInterfaceST;
      #define OPTION_PCIEPOSTEARLYINTERFACEST_ENTRY       {AMD_FAMILY_ST, PciePostEarlyInterfaceST, TpGnbPciePostEarlyInterface},
    #else
      #define OPTION_PCIEPOSTEARLYINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_POST_INIT
      #define OPTION_PCIE_POST_INIT TRUE
    #endif
    #if (OPTION_PCIE_POST_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  PciePostInterfaceCZ;
      #define OPTION_PCIEPOSTINTERFACECZ_ENTRY            {AMD_FAMILY_CZ, PciePostInterfaceCZ, TpGnbPciePostInterface},
    #else
      #define OPTION_PCIEPOSTINTERFACECZ_ENTRY
    #endif
    #if (OPTION_PCIE_POST_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PciePostInterfaceST;
      #define OPTION_PCIEPOSTINTERFACEST_ENTRY            {AMD_FAMILY_ST, PciePostInterfaceST, TpGnbPciePostInterface},
    #else
      #define OPTION_PCIEPOSTINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbPostFeatureTable[] = {
      OPTION_PCIEPOSTEARLYINTERFACECZ_ENTRY
      OPTION_PCIEPOSTEARLYINTERFACEST_ENTRY
      OPTION_GFXCONFIGPOSTINTERFACE_ENTRY
      OPTION_GFXPOSTINTERFACECZ_ENTRY
      OPTION_GFXPOSTINTERFACEST_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

    OPTION_GNB_CONFIGURATION  GnbPostAfterDramFeatureTable[] = {
      OPTION_GNBPOSTINTERFACECZ_ENTRY
      OPTION_GNBPOSTINTERFACEST_ENTRY
      OPTION_GNBSMUPOSTINTERFACECZ_ENTRY
      OPTION_GNBSMUPOSTINTERFACEST_ENTRY
      OPTION_PCIEPOSTINTERFACECZ_ENTRY
      OPTION_PCIEPOSTINTERFACEST_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_ENV == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_ENV_INIT
      #define OPTION_NB_ENV_INIT TRUE
    #endif
    #if (OPTION_NB_ENV_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbEnvInterfaceCZ;
      #define OPTION_GNBENVINTERFACECZ_ENTRY              {AMD_FAMILY_CZ, GnbEnvInterfaceCZ, TpGnbEnvInterface},
    #else
      #define OPTION_GNBENVINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_ENV_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbEnvInterfaceST;
      #define OPTION_GNBENVINTERFACEST_ENTRY              {AMD_FAMILY_ST, GnbEnvInterfaceST, TpGnbEnvInterface},
    #else
      #define OPTION_GNBENVINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_ENV_INIT
      #define OPTION_GFX_CONFIG_ENV_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_ENV_INIT == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                   GfxConfigEnvInterface;
      #define OPTION_GFXCONFIGENVINTERFACE_ENTRY          {AMD_FAMILY_CZ | AMD_FAMILY_ST, GfxConfigEnvInterface, TpGnbGfxConfigEnvInterface},
    #else
      #define  OPTION_GFXCONFIGENVINTERFACE_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_ENV_INIT
      #define OPTION_GFX_ENV_INIT TRUE
    #endif
    #if (OPTION_GFX_ENV_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GfxEnvInterfaceCZ;
      #define OPTION_GFXENVINTERFACECZ_ENTRY              {AMD_FAMILY_CZ, GfxEnvInterfaceCZ, TpGnbGfxEnvInterface},
    #else
      #define OPTION_GFXENVINTERFACECZ_ENTRY
    #endif
    #if (OPTION_GFX_ENV_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxEnvInterfaceST;
      #define OPTION_GFXENVINTERFACEST_ENTRY              {AMD_FAMILY_ST, GfxEnvInterfaceST, TpGnbGfxEnvInterface},
    #else
      #define OPTION_GFXENVINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_ENV_INIT
      #define OPTION_PCIE_ENV_INIT TRUE
    #endif
    #if (OPTION_PCIE_ENV_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  PcieEnvInterfaceCZ;
      #define OPTION_PCIEENVINTERFACECZ_ENTRY             {AMD_FAMILY_CZ, PcieEnvInterfaceCZ, TpGnbPcieEnvInterface},
    #else
      #define OPTION_PCIEENVINTERFACECZ_ENTRY
    #endif
    #if (OPTION_PCIE_ENV_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieEnvInterfaceST;
      #define OPTION_PCIEENVINTERFACEST_ENTRY             {AMD_FAMILY_ST, PcieEnvInterfaceST, TpGnbPcieEnvInterface},
    #else
      #define OPTION_PCIEENVINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------

    OPTION_GNB_CONFIGURATION  GnbEnvFeatureTable[] = {
      OPTION_GNBENVINTERFACECZ_ENTRY
      OPTION_GNBENVINTERFACEST_ENTRY
      OPTION_PCIEENVINTERFACECZ_ENTRY
      OPTION_PCIEENVINTERFACEST_ENTRY
      OPTION_GFXCONFIGENVINTERFACE_ENTRY
      OPTION_GFXENVINTERFACECZ_ENTRY
      OPTION_GFXENVINTERFACEST_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_MID == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_MID_INIT
      #define OPTION_GFX_MID_INIT TRUE
    #endif
    #if (OPTION_GFX_MID_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GfxMidInterfaceCZ;
      #define OPTION_GFXMIDINTERFACECZ_ENTRY              {AMD_FAMILY_CZ, GfxMidInterfaceCZ, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXMIDINTERFACECZ_ENTRY
    #endif
    #if (OPTION_GFX_MID_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxMidInterfaceST;
      #define OPTION_GFXMIDINTERFACEST_ENTRY              {AMD_FAMILY_ST, GfxMidInterfaceST, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXMIDINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_INTEGRATED_TABLE_INIT
      #define OPTION_GFX_INTEGRATED_TABLE_INIT TRUE
    #endif
    #if (OPTION_GFX_INTEGRATED_TABLE_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GfxIntInfoTableInterfaceCZ;
      #define OPTION_GFXINTINFOTABLEINTERFACECZ_ENTRY     {AMD_FAMILY_CZ, GfxIntInfoTableInterfaceCZ, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXINTINFOTABLEINTERFACECZ_ENTRY
    #endif
    #if (OPTION_GFX_INTEGRATED_TABLE_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxIntInfoTableInterfaceST;
      #define OPTION_GFXINTINFOTABLEINTERFACEST_ENTRY     {AMD_FAMILY_ST, GfxIntInfoTableInterfaceST, TpGnbGfxMidInterface},
    #else
      #define OPTION_GFXINTINFOTABLEINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIe_MID_INIT
      #define OPTION_PCIe_MID_INIT TRUE
    #endif
    #if (OPTION_PCIe_MID_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  PcieMidInterfaceCZ;
      #define OPTION_PCIEMIDINTERFACECZ_ENTRY             {AMD_FAMILY_CZ, PcieMidInterfaceCZ, TpPcieMidInterface},
    #else
      #define OPTION_PCIEMIDINTERFACECZ_ENTRY
    #endif
    #if (OPTION_PCIe_MID_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieMidInterfaceST;
      #define OPTION_PCIEMIDINTERFACEST_ENTRY             {AMD_FAMILY_ST, PcieMidInterfaceST, TpPcieMidInterface},
    #else
      #define OPTION_PCIEMIDINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_NB_MID_INIT
      #define OPTION_NB_MID_INIT TRUE
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_CONFIG_POST_INIT
      #define OPTION_GFX_CONFIG_POST_INIT TRUE
    #endif
    #if (OPTION_GFX_CONFIG_POST_INIT == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxConfigMidInterface;
      #define OPTION_GFXCONFIGMIDINTERFACE_ENTRY         {AMD_FAMILY_CZ | AMD_FAMILY_ST, GfxConfigMidInterface, TpGnbGfxConfigMidInterface},
    #else
      #define OPTION_GFXCONFIGMIDINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbMidInterfaceCZ;
      #define OPTION_GNBMIDINTERFACECZ_ENTRY              {AMD_FAMILY_CZ, GnbMidInterfaceCZ, TpGnbMidInterface},
    #else
      #define OPTION_GNBMIDINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbMidInterfaceST;
      #define OPTION_GNBMIDINTERFACEST_ENTRY              {AMD_FAMILY_ST, GnbMidInterfaceST, TpGnbMidInterface},
    #else
      #define OPTION_GNBMIDINTERFACEST_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuMidInterfaceCZ;
      #define OPTION_GNBSMUMIDINTERFACECZ_ENTRY           {AMD_FAMILY_CZ, GnbSmuMidInterfaceCZ, TpGnbSmuMidInterface},
    #else
      #define OPTION_GNBSMUMIDINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuMidInterfaceST;
      #define OPTION_GNBSMUMIDINTERFACEST_ENTRY           {AMD_FAMILY_ST, GnbSmuMidInterfaceST, TpGnbSmuMidInterface},
    #else
      #define OPTION_GNBSMUMIDINTERFACEST_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_MAXPAYLOAD_INTERFACE
      #define OPTION_PCIE_MAXPAYLOAD_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_MAXPAYLOAD_INTERFACE == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieMaxPayloadInterface;
      #define OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY        {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieMaxPayloadInterface, TpGnbPcieMaxPayloadInterface},
    #else
      #define OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_CLK_PM_INTERFACE
      #define OPTION_PCIE_CLK_PM_INTERFACE FALSE
      #if (GNB_TYPE_CZ == TRUE)
        #undef  OPTION_PCIE_CLK_PM_INTERFACE
        #define OPTION_PCIE_CLK_PM_INTERFACE TRUE
      #endif
      #if (GNB_TYPE_ST == TRUE)
        #undef  OPTION_PCIE_CLK_PM_INTERFACE
        #define OPTION_PCIE_CLK_PM_INTERFACE TRUE
      #endif
    #endif

    #if (OPTION_PCIE_CLK_PM_INTERFACE == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieClkPmInterface;
      #define OPTION_PCIECLKPMINTERFACE_ENTRY             {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieClkPmInterface, TpGnbPcieClkPmInterface},
    #else
      #define OPTION_PCIECLKPMINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_ASPM_INTERFACE
      #define OPTION_PCIE_ASPM_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_ASPM_INTERFACE == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieAspmInterface;
      #define OPTION_PCIEASPMINTERFACE_ENTRY              {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieAspmInterface, TpGnbPcieAspmInterface},
    #else
      #define OPTION_PCIEASPMINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GNB_IOAPIC_INTERFACE
      #define OPTION_GNB_IOAPIC_INTERFACE TRUE
    #endif
    #if (OPTION_GNB_IOAPIC_INTERFACE == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbNbIoapicInterface;
      #define OPTION_GNBNBIOAPICINTERFACE_ENTRY          {AMD_FAMILY_CZ | AMD_FAMILY_ST, GnbNbIoapicInterface, TpGnbNbIoapicInterface},
    #else
      #define OPTION_GNBNBIOAPICINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE
      #define OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE TRUE
    #endif
    #if (OPTION_PCIE_PROGRAM_COMMON_CLOCK_INTERFACE == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  PcieCommClkCfgInterface;
      #define OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY              {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieCommClkCfgInterface, TpGnbPcieClkPmInterface},
    #else
      #define OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_BTC
      #define OPTION_BTC FALSE
    #endif
    #if (OPTION_BTC == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbBootTimeCalInterfaceCZ;
      #define OPTIONBOOTTIMECALLATECZ_ENTRY                 {AMD_FAMILY_CZ, GnbBootTimeCalInterfaceCZ, TpGnbBtcRun},
    #else
      #define OPTIONBOOTTIMECALLATECZ_ENTRY
    #endif
    #if (OPTION_BTC == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbBootTimeCalInterfaceST;
      #define OPTIONBOOTTIMECALLATEST_ENTRY                 {AMD_FAMILY_ST, GnbBootTimeCalInterfaceST, TpGnbBtcRun},
    #else
      #define OPTIONBOOTTIMECALLATEST_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbMidFeatureTable[] = {
      OPTION_GFXCONFIGMIDINTERFACE_ENTRY
      OPTION_GFXMIDINTERFACECZ_ENTRY
      OPTION_GFXMIDINTERFACEST_ENTRY
      OPTION_GFXINTINFOTABLEINTERFACECZ_ENTRY
      OPTION_GFXINTINFOTABLEINTERFACEST_ENTRY
      OPTION_PCIEMIDINTERFACECZ_ENTRY
      OPTION_PCIEMIDINTERFACEST_ENTRY
      OPTION_GNBMIDINTERFACECZ_ENTRY
      OPTION_GNBMIDINTERFACEST_ENTRY
      OPTIONBOOTTIMECALLATECZ_ENTRY
      OPTIONBOOTTIMECALLATEST_ENTRY
      OPTION_GNBSMUMIDINTERFACECZ_ENTRY
      OPTION_GNBSMUMIDINTERFACEST_ENTRY
      OPTION_PCIEMAXPAYLOADINTERFACE_ENTRY
      OPTION_PCIECLKPMINTERFACE_ENTRY
      OPTION_PCIEASPMINTERFACE_ENTRY
      OPTION_GNBNBIOAPICINTERFACE_ENTRY
      OPTION_PCIECOMMCLKCFGINTERFACE_ENTRY
      {0, NULL, EndGnbTestPoints}
    };

  //---------------------------------------------------------------------------------------------------
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_CZ == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuMidLateInterfaceCZ;
      #define OPTION_GNBSMUMIDLATEINTERFACECZ_ENTRY           {AMD_FAMILY_CZ, GnbSmuMidLateInterfaceCZ, TpGnbSmuMidInterface},
    #else
      #define OPTION_GNBSMUMIDLATEINTERFACECZ_ENTRY
    #endif
    #if (OPTION_NB_MID_INIT == TRUE) && (GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbSmuMidLateInterfaceST;
      #define OPTION_GNBSMUMIDLATEINTERFACEST_ENTRY           {AMD_FAMILY_ST, GnbSmuMidLateInterfaceST, TpGnbSmuMidInterface},
    #else
      #define OPTION_GNBSMUMIDLATEINTERFACEST_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbMidLateFeatureTable[] = {
      OPTION_GNBSMUMIDLATEINTERFACECZ_ENTRY
      OPTION_GNBSMUMIDLATEINTERFACEST_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif
  //--------------------------------------------------------------------------------------------------

  #if (AGESA_ENTRY_INIT_LATE == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_ALIB
      #define OPTION_ALIB FALSE
    #endif
    #if (OPTION_ALIB == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE_V2     PcieAlibUpdateVoltageData;
      #define OPTION_ALIB_PCIEUPDATEVOLTAGEDATA     {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieAlibUpdateVoltageData, TpGnbAlibDispatchFeature},
      OPTION_GNB_FEATURE_V2     PcieAlibUpdatePcieData;
      #define OPTION_ALIB_PCIEUPDATEPCIEDATA     {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieAlibUpdatePcieData, TpGnbAlibDispatchFeature},

      OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [] = {
        OPTION_ALIB_PCIEUPDATEVOLTAGEDATA
        OPTION_ALIB_PCIEUPDATEPCIEDATA
        {0, NULL, EndGnbTestPoints}
      };

      #if (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
        OPTION_GNB_FEATURE                                  PcieAlibV2Feature;
        #define OPTION_PCIEALIBV2FEATURE_ENTRY              {AMD_FAMILY_CZ | AMD_FAMILY_ST, PcieAlibV2Feature, TpGnbPcieAlibFeature},
      #endif
    #else
      OPTION_GNB_CONFIGURATION_V2 AlibDispatchTableV2 [] = {
        {0, NULL, EndGnbTestPoints}
      };
      #define OPTION_PCIEALIBV2FEATURE_ENTRY
    #endif
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_IOMMU_ACPI_IVRS
      #if (CFG_IOMMU_SUPPORT == TRUE)
        #define OPTION_IOMMU_ACPI_IVRS TRUE
      #else
        #define OPTION_IOMMU_ACPI_IVRS FALSE
      #endif
    #endif
    #if (OPTION_IOMMU_ACPI_IVRS == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbIommuIvrsTable;
      #define OPTIONIOMMUACPIIVRSLATE_ENTRY               {AMD_FAMILY_CZ | AMD_FAMILY_ST, GnbIommuIvrsTable, TpGnbIommuIvrsTable},
    #else
      #define OPTIONIOMMUACPIIVRSLATE_ENTRY
    #endif
    #if (CFG_IOMMU_SUPPORT == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GnbIommuScratchMemoryRangeInterface;
      #define OPTIONIOMMUSCRATCHMEMORYLATE_ENTRY          {AMD_FAMILY_CZ | AMD_FAMILY_ST, GnbIommuScratchMemoryRangeInterface, TpGnbIommuIvrsTable},
    #else
      #define OPTIONIOMMUSCRATCHMEMORYLATE_ENTRY
    #endif

  //---------------------------------------------------------------------------------------------------
    OPTION_GNB_CONFIGURATION  GnbLateFeatureTable[] = {
      OPTION_PCIEALIBV2FEATURE_ENTRY
      OPTIONIOMMUSCRATCHMEMORYLATE_ENTRY
      OPTIONIOMMUACPIIVRSLATE_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if (AGESA_ENTRY_INIT_RTB == TRUE)
  //---------------------------------------------------------------------------------------------------
    #ifndef OPTION_GFX_INIT_SVIEW
      #define OPTION_GFX_INIT_SVIEW TRUE
    #endif
    #if (OPTION_GFX_INIT_SVIEW == TRUE) && (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      OPTION_GNB_FEATURE                                  GfxInitSview;
      #define OPTION_GFXINITSVIEW_ENTRY                   {AMD_FAMILY_CZ | AMD_FAMILY_ST, GfxInitSview, TpGnbGfxInitSview},
    #else
      #define OPTION_GFXINITSVIEW_ENTRY
    #endif

    OPTION_GNB_CONFIGURATION  GnbRtbFeatureTable[] = {
      OPTION_GFXINITSVIEW_ENTRY
      {0, NULL, EndGnbTestPoints}
    };
  #endif

  #if  (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
    #define GNBS3RESTOREV8
    #if (GNB_TYPE_CZ == TRUE || GNB_TYPE_ST == TRUE)
      S3_DISPATCH_FUNCTION  GnbSmuServiceRequestV8S3Script;
      #undef  GNBS3RESTOREV8
      #define GNBS3RESTOREV8 {GnbSmuServiceRequestV8S3Script_ID, GnbSmuServiceRequestV8S3Script},
    #endif

    #define GNBS3WAITFORCACLOCKCZ
    #if (GNB_TYPE_CZ == TRUE)
      S3_DISPATCH_FUNCTION  GnbSmuWaitForCacLockCZS3Script;
      #undef  GNBS3WAITFORCACLOCKCZ
      #define GNBS3WAITFORCACLOCKCZ {GnbSmuWaitForCacLockS3Script_ID, GnbSmuWaitForCacLockCZS3Script},
    #endif
    #define GNBS3WAITFORCACLOCKST
    #if (GNB_TYPE_ST == TRUE)
      S3_DISPATCH_FUNCTION  GnbSmuWaitForCacLockSTS3Script;
      #undef  GNBS3WAITFORCACLOCKST
      #define GNBS3WAITFORCACLOCKST {GnbSmuWaitForCacLockS3Script_ID, GnbSmuWaitForCacLockSTS3Script},
    #endif

    S3_DISPATCH_FUNCTION  GnbLibStallS3Script;
    #define PCIELATERESTORECZ
    #if (GNB_TYPE_CZ == TRUE)
      S3_DISPATCH_FUNCTION  PcieLateRestoreInitCZS3Script;
      #undef  PCIELATERESTORECZ
      #define PCIELATERESTORECZ {PcieLateRestoreCZS3Script_ID, PcieLateRestoreInitCZS3Script},
    #endif
    #define PCIELATERESTOREST
    #if (GNB_TYPE_ST == TRUE)
      S3_DISPATCH_FUNCTION  PcieLateRestoreInitSTS3Script;
      #undef  PCIELATERESTOREST
      #define PCIELATERESTOREST {PcieLateRestoreCZS3Script_ID, PcieLateRestoreInitSTS3Script},
    #endif
    #define GNB_S3_DISPATCH_FUNCTION_TABLE \
    GNBS3RESTOREV8      \
    PCIELATERESTORECZ \
    PCIELATERESTOREST \
    GNBS3WAITFORCACLOCKCZ \
    GNBS3WAITFORCACLOCKST \
    {GnbLibStallS3Script_ID, GnbLibStallS3Script},
  #endif

#endif
#endif  // _OPTION_GNB_INSTALL_H_
