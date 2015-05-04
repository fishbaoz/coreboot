/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Prefetch Mode
 *
 * This funtion provides for performance tuning to optimize for specific
 * workloads. For general performance use the recommended settings.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
#include "cpuFamilyTranslation.h"
#include "cpuServices.h"
#include "GeneralServices.h"
#include "cpuPrefetchMode.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CPUF15PREFETCHMODE_FILECODE

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
AGESA_STATUS
CpuF15PrefetchMode (
  IN       PREFETCH_MODE_FAMILY_SERVICES    *PrefetchModeServices,
  IN       PLATFORM_CONFIGURATION           *PlatformConfig,
  IN       AMD_CONFIG_PARAMS                *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *  CpuF15PrefetchMode
 *
 *  Description:
 *    This funtion provides for performance tuning to optimize for specific
 *    workloads. For general performance use the recommended settings.
 *
 *  Parameters:
 *    @param[in]  PrefetchModeServices      The current Family Specific Services.
 *    @param[in]  PlatformConfig            Platform profile/build option config structure.
 *    @param[in]  StdHeader                 Config handle for library and services
 *
 *    @retval     AGESA_STATUS
 *
 *
 */
AGESA_STATUS
CpuF15PrefetchMode (
  IN       PREFETCH_MODE_FAMILY_SERVICES    *PrefetchModeServices,
  IN       PLATFORM_CONFIGURATION           *PlatformConfig,
  IN       AMD_CONFIG_PARAMS                *StdHeader
  )
{
  UINT64   LocalMsrRegister;

  ASSERT (PlatformConfig->PlatformProfile.AdvancedPerformanceProfile.HardwarePrefetchMode < MAX_HARDWARE_PREFETCH_MODE);

  switch (PlatformConfig->PlatformProfile.AdvancedPerformanceProfile.HardwarePrefetchMode) {
  case DISABLE_HARDWARE_PREFETCH:
    // DC_CFG (MSR_C001_1022)
    //  [13] = 1
    //  [15] = 1
    LibAmdMsrRead (MSR_DC_CFG, &LocalMsrRegister, StdHeader);
    LocalMsrRegister |= (BIT13 | BIT15);
    LibAmdMsrWrite (MSR_DC_CFG, &LocalMsrRegister, StdHeader);
    // CU_CFG3 (MSR_C001_102B)
    //  [3]  = 1
    //  [16] = 1
    //  [17] = 1
    //  [18] = 1
    if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
      LibAmdMsrRead (MSR_CU_CFG3, &LocalMsrRegister, StdHeader);
      LocalMsrRegister |= (BIT3 | BIT16 | BIT17 | BIT18);
      LibAmdMsrWrite (MSR_CU_CFG3, &LocalMsrRegister, StdHeader);
    }
    break;
  case DISABLE_L2_STRIDE_PREFETCHER:
    // For DISABLE_L2_STRIDE_PREFETCHER, also implement the following :
    // DISABLE_L1_PREFETCHER
    // DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES
    // Hence, no break to allow code to flow through.
    // CU_CFG3 (MSR_C001_102B)
    //  [17] = 1
    if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
      LibAmdMsrRead (MSR_CU_CFG3, &LocalMsrRegister, StdHeader);
      LocalMsrRegister |= BIT17;
      LibAmdMsrWrite (MSR_CU_CFG3, &LocalMsrRegister, StdHeader);
    }
  case DISABLE_L1_PREFETCHER:
    // For this setting, also implement DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES.
    // Hence, no break to allow code to flow through.
    // CU_CFG3 (MSR_C001_102B)
    //  [3] = 1
    if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
      LibAmdMsrRead (MSR_CU_CFG3, &LocalMsrRegister, StdHeader);
      LocalMsrRegister |= BIT3;
      LibAmdMsrWrite (MSR_CU_CFG3, &LocalMsrRegister, StdHeader);
    }
  case DISABLE_HW_PREFETCHER_TRAINING_ON_SOFTWARE_PREFETCHES:
    // DC_CFG (MSR_C001_1022)
    //  [15] = 1
    LibAmdMsrRead (MSR_DC_CFG, &LocalMsrRegister, StdHeader);
    LocalMsrRegister |= BIT15;
    LibAmdMsrWrite (MSR_DC_CFG, &LocalMsrRegister, StdHeader);
  }
  // DISABLE_SOFTWARE_PREFETCHES
  if (PlatformConfig->PlatformProfile.AdvancedPerformanceProfile.SoftwarePrefetchMode == DISABLE_SOFTWARE_PREFETCHES) {
    // MSR_DE_CFG (MSR_C001_1029)
    //  [7:2] = 0x3F
    if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
      LibAmdMsrRead (MSR_DE_CFG, &LocalMsrRegister, StdHeader);
      LocalMsrRegister |= 0xFC;
      LibAmdMsrWrite (MSR_DE_CFG, &LocalMsrRegister, StdHeader);
    }
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

CONST PREFETCH_MODE_FAMILY_SERVICES ROMDATA F15PrefetchModeSupport =
{
  0,
  CpuF15PrefetchMode
};