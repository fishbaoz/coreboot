/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Cache Flush On Halt Function for Family 16h Mullins.
 *
 * Contains code to initialize Cache Flush On Halt feature for Family 16h Mullins.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x16/ML
 * @e \$Revision: 281178 $   @e \$Date: 2013-12-18 02:14:15 -0600 (Wed, 18 Dec 2013) $
 *
 */
/*
 ******************************************************************************
 *
 * Copyright 2008 - 2014 ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
 *----------------------------------------------------------------------------
 */


/*
 *----------------------------------------------------------------------------
 *                                MODULES USED
 *
 *----------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "cpuRegisters.h"
#include "cpuServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuPostInit.h"
#include "cpuF16PowerMgmt.h"
#include "F16MlPowerMgmt.h"
#include "cpuFeatures.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X16_ML_F16MLCACHEFLUSHONHALT_FILECODE

/*----------------------------------------------------------------------------
 *                          DEFINITIONS AND MACROS
 *
 *----------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------
 *                           TYPEDEFS AND STRUCTURES
 *
 *----------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------
 *                        PROTOTYPES OF LOCAL FUNCTIONS
 *
 *----------------------------------------------------------------------------
 */
VOID
SetF16MlCacheFlushOnHaltRegister (
  IN       CPU_CFOH_FAMILY_SERVICES     *FamilySpecificServices,
  IN       UINT64                       EntryPoint,
  IN       PLATFORM_CONFIGURATION       *PlatformConfig,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          P U B L I C     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *    Enable Cpu Cache Flush On Halt Function
 *
 *    @param[in]       FamilySpecificServices   The current Family Specific Services.
 *    @param[in]       EntryPoint               Timepoint designator.
 *    @param[in]       PlatformConfig           Contains the runtime modifiable feature input data.
 *    @param[in]       StdHeader                Config Handle for library, services.
 */
VOID
SetF16MlCacheFlushOnHaltRegister (
  IN       CPU_CFOH_FAMILY_SERVICES     *FamilySpecificServices,
  IN       UINT64                       EntryPoint,
  IN       PLATFORM_CONFIGURATION       *PlatformConfig,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  PCI_ADDR                              PciAddress;
  CSTATE_CTRL1_REGISTER                 CstateCtrl1;

  if ((EntryPoint & (CPU_FEAT_AFTER_POST_MTRR_SYNC | CPU_FEAT_AFTER_RESUME_MTRR_SYNC)) != 0) {
    // Set F4x118
    PciAddress.AddressValue = CSTATE_CTRL1_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &CstateCtrl1, StdHeader);
    // Set C-state Action Field 0
    // bits[10]      NbPwrGate0          = 0x1
    // bits[11]      NbClkGate0          = 0x1
    // bits[12]      SelfRefr0           = 0x1
    CstateCtrl1.NbPwrGate0 = 1;
    CstateCtrl1.NbClkGate0 = 1;
    CstateCtrl1.SelfRefr0 = 1;
    // Set C-state Action Field 1
    // bits[26]      NbPwrGate1          = 0x1
    // bits[27]      NbClkGate1          = 0x1
    // bits[28]      SelfRefr1           = 0x1
    CstateCtrl1.NbPwrGate1 = 1;
    CstateCtrl1.NbClkGate1 = 1;
    CstateCtrl1.SelfRefr1 = 1;
    LibAmdPciWrite (AccessWidth32, PciAddress, &CstateCtrl1, StdHeader);
  }
}

CONST CPU_CFOH_FAMILY_SERVICES ROMDATA F16MlCacheFlushOnHalt =
{
  0,
  SetF16MlCacheFlushOnHaltRegister
};
