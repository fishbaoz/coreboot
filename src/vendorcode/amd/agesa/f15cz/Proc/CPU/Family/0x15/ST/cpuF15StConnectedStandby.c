/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 Stoney Connected Standby Initialization
 *
 * Enables connected standby.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/0x15/ST
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

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "GeneralServices.h"
#include "cpuFamilyTranslation.h"
#include "cpuFeatures.h"
#include "cpuConnectedStandby.h"
#include "cpuApComm.h"
#include "PspBaseLib.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_ST_CPUF15STCONNECTEDSTANDBY_FILECODE

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
BOOLEAN
STATIC
F15StIsConnectedStandbySupported (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
STATIC
F15StInitializeConnectedStandby (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       UINT64                 EntryPoint,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

VOID
STATIC
F15StSetConnectedStandbyEnable (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );
/*---------------------------------------------------------------------------------------*/
/**
 * BSC entry point for checking whether or not Connected Standby is supported.
 *
 * @param[in]  ConnectedStandbyServices  The current CPU's family services.
 * @param[in]  PlatformConfig          Contains the runtime modifiable feature input data.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     TRUE                    Connected Standby is supported.
 * @retval     FALSE                   Connected Standby is not supported.
 *
 */
BOOLEAN
STATIC
F15StIsConnectedStandbySupported (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES    *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  if (CheckPspDevicePresent () && CheckPspPlatformSecureEnable ()) {
    return TRUE;
  }
  return FALSE;
}


/*---------------------------------------------------------------------------------------*/
/**
 * BSC entry point for for enabling Connected Standby.
 *
 * Set up MSRC001_0015[CSEnable] on each core
 *
 * @param[in]  ConnectedStandbyServices  The current CPU's family services.
 * @param[in]  PlatformConfig          Contains the runtime modifiable feature input data.
 * @param[in]  EntryPoint              Current CPU feature dispatch point.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
STATIC
F15StInitializeConnectedStandby (
  IN       CONNECTED_STANDBY_FAMILY_SERVICES *ConnectedStandbyServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       UINT64                 EntryPoint,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  AP_TASK  TaskPtr;

  if ((EntryPoint & CPU_FEAT_AFTER_PM_INIT) != 0) {
    // Initialize MSRC001_0015[CSEnable] on each core
    TaskPtr.FuncAddress.PfApTask = F15StSetConnectedStandbyEnable;
    TaskPtr.DataTransfer.DataSizeInDwords = 0;
    TaskPtr.ExeFlags = 0;
    ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, NULL);
  }
  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Enable CSEnable on a family 15h Stoney core.
 *
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 */
VOID
STATIC
F15StSetConnectedStandbyEnable (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64  LocalMsrRegister;

  // Initialize MSRC001_0015[CSEnable] on each core
  LibAmdMsrRead (0xC0010015, &LocalMsrRegister, StdHeader);
  LocalMsrRegister |= BIT29;
  LibAmdMsrWrite (0xC0010015, &LocalMsrRegister, StdHeader);
}

CONST CONNECTED_STANDBY_FAMILY_SERVICES ROMDATA F15StConnectedStandbySupport =
{
  0,
  F15StIsConnectedStandbySupported,
  F15StInitializeConnectedStandby
};
