/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family_15 APM Initialization
 *
 * Enables Application Power Management feature
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x16
 * @e \$Revision: 315356 $   @e \$Date: 2015-03-25 14:29:02 +0800 (Wed, 25 Mar 2015) $
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
#include "cpuServices.h"
#include "cpuRegisters.h"
#include "cpuApComm.h"
#include "OptionApComm.h"
#include "cpuFamilyTranslation.h"
#include "cpuF15PowerMgmt.h"
#include "CommonReturns.h"
#include "cpuApm.h"
#include "S3SaveState.h"
#include "Gnb.h"
#include "GnbPcieConfig.h"
#include "GnbRegistersCZ.h"
#include "GnbHandleLib.h"
#include "GnbSmuInitLibV8.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CPUF15APM_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern OPTION_AP_COMMUNICATE_CONFIGURATION OptionApCommConfiguration;

#define MAX_APM_WTS_F15 136

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
BOOLEAN
F15IsApmSupported (
  IN       APM_FAMILY_SERVICES    *ApmServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
F15InitializeApm (
  IN       APM_FAMILY_SERVICES    *ApmServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );
/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
ProgramApmWeightsOnAps (
  IN       VOID *ApmWeigths,
  IN       AMD_CONFIG_PARAMS *StdHeader

  )
{
  UINT8   i;

  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    for (i = 0; ((APM_WEIGHTS_DATA *) ApmWeigths)[i].MsrAddr != 0; i++) {
      LibAmdMsrWrite (((APM_WEIGHTS_DATA *) ApmWeigths)[i].MsrAddr,
                      &((APM_WEIGHTS_DATA *) ApmWeigths)[i].Data,
                      StdHeader);
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to check if Application Power Management (APM) is supported.
 *
 * @param[in]    ApmServices        APM services.
 * @param[in]    PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]    StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               APM is supported.
 * @retval       FALSE              APM is not supported.
 *
 */
BOOLEAN
F15IsApmSupported (
  IN       APM_FAMILY_SERVICES    *ApmServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  F15_CPB_CTRL_REGISTER  CpbControl;
  PCI_ADDR               PciAddress;
  BOOLEAN                Result;

  Result = FALSE;
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &CpbControl, StdHeader);
  if (CpbControl.NumBoostStates != 0) {
    Result = TRUE;
  }
  return Result;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Entry point for programming APM weights
 *
 * This function must be run before enabling ApmMasterEn
 *
 * @param[in]  ApmServices             The current CPU's family services.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
F15InitializeApmWeights (
  IN       APM_FAMILY_SERVICES    *ApmServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT8             i;
  UINT32            Core;
  UINT32            NumberOfCores;
  UINT32            SmuArg[6];
  UINT32            AccessFlags;
  UINT64            Lock;
  AP_TASK           TaskPtr;
  DEV_OBJECT        DevObject;
  GNB_HANDLE       *GnbHandle;
  APM_WEIGHTS_DATA  ApmWeights[MAX_APM_WTS_F15 / 4 + 2];

  IDS_HDT_CONSOLE (CPU_TRACE, "F15InitializeApmWeights Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);
  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  AccessFlags = 0;

  //Get all APM weights data
  for (i = 0; i < MAX_APM_WTS_F15 / 4; i++) {
    SmuArg[0] = i;
    //call message 0x71 and pass in "i" as the ARG
    GnbSmuServiceRequestV8 (
      &DevObject,
      0x71, //call message 0x71 and pass in "i" as the ARG
      SmuArg,
      AccessFlags
      );
    ApmWeights[i].Data = ((UINT64) SmuArg[0]) | (((UINT64) SmuArg[1]) << 32);
    ApmWeights[i].MsrAddr = 0xC0011072;
  }
  //Get the energy count normalize
  GnbSmuServiceRequestV8 (
      &DevObject,
      0x72, //call message 0x72 with no ARG
      SmuArg,
      AccessFlags
      );

  LibAmdMsrRead (0xC0011073, &ApmWeights[i].Data, StdHeader);
  ApmWeights[i].Data &= 0xFFF003FFFFFFFFFF;
  ApmWeights[i].Data |= ((UINT64) SmuArg[0]  & 0x3FF) << 42;
  ApmWeights[i].Data |= BIT62 | BIT63;
  ApmWeights[i].MsrAddr = 0xC0011073;

  ApmWeights[i + 1].Data = 0x0;
  ApmWeights[i + 1].MsrAddr = 0x0;

  GetActiveCoresInCurrentSocket (&NumberOfCores, StdHeader);

  TaskPtr.FuncAddress.PfApTaskI = ProgramApmWeightsOnAps;
  TaskPtr.DataTransfer.DataSizeInDwords = (UINT16) (sizeof (ApmWeights) >> 2);
  TaskPtr.ExeFlags = WAIT_FOR_CORE;
  TaskPtr.DataTransfer.DataPtr = ApmWeights;
  TaskPtr.DataTransfer.DataTransferFlags = 0;

  for (Core = 1; Core < NumberOfCores; Core++) {
    OptionApCommConfiguration.ApRunCodeOnSocketCore (0, (UINT8) Core, &TaskPtr, StdHeader);
  }

  ProgramApmWeightsOnAps (ApmWeights, StdHeader);

  IDS_SKIP_HOOK (IDS_MSR_ACCESS_OVERRIDE, NULL, StdHeader) {
    do {
      LibAmdMsrRead (0xC0011073, &Lock, StdHeader);
    } while ((Lock & BIT62) != BIT62);
  }

  return AGESA_SUCCESS;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Entry point for enabling Application Power Management
 *
 * This function must be run after all P-State routines have been executed
 *
 * @param[in]  ApmServices             The current CPU's family services.
 * @param[in]  StdHeader               Config handle for library and services.
 *
 * @retval     AGESA_SUCCESS           Always succeeds.
 *
 */
AGESA_STATUS
F15InitializeApm (
  IN       APM_FAMILY_SERVICES    *ApmServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  F15_CPB_CTRL_REGISTER  CpbControl;
  PCI_ADDR               PciAddress;

  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &CpbControl, StdHeader);
  CpbControl.ApmMasterEn = 1;
  LibAmdPciWrite (AccessWidth32, PciAddress, &CpbControl, StdHeader);
  S3_SAVE_PCI_WRITE (StdHeader, PciAddress, AccessWidth32, &CpbControl);
  return AGESA_SUCCESS;
}



CONST APM_FAMILY_SERVICES ROMDATA F15ApmSupport =
{
  0,
  F15IsApmSupported,
  F15InitializeApmWeights,
  F15InitializeApm
};

