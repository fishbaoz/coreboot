/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB boot time calibration.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 311051 $   @e \$Date: 2015-01-16 10:18:15 +0800 (Fri, 16 Jan 2015) $
 *
 */
/*
*****************************************************************************
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
* ***************************************************************************
*
*/
/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include  "AGESA.h"
#include  "Ids.h"
#include  "amdlib.h"
#include  "S3SaveState.h"
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbFuseTable.h"
#include  "heapManager.h"
#include  "GnbCommonLib.h"
#include  "GnbRegisterAccCZ.h"
#include  "GnbRegistersCZ.h"
#include  "GnbFamServices.h"
#include  "OptionGnb.h"
#include  "GnbSmuInitLibV8.h"
#include  "cpuBtc.h"
#include  "cpuF15CzBtc.h"
#include  "cpuFamilyTranslation.h"
#include  "OptionGnb.h"
#include  "heapManager.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_GNBBOOTTIMECALCZ_FILECODE

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE           BtcFamilyServiceTable;

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------*/
/**
 * Read back and set SMU parameters for GBV
 *
 *
 *
 * @param[in]  GnbHandle  GNB_HANDLE
 * @param[in]  AccessFlags         Flag for S3
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
STATIC
BtcGetSetGbvCZ (
  IN      GNB_HANDLE          *GnbHandle,
  IN      UINT32              AccessFlags,
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  DEV_OBJECT             DevObject;
  UINT32                 SmuArg[6];

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);

  //BTC voltage guard-band for P-State 4 to P-State 7
  GnbSmuServiceRequestV8 (
     &DevObject,
     SMC_MSG_GetBtcGbvHi,
     SmuArg,
     0
     );

  if ((AccessFlags & GNB_REG_ACC_FLAG_S3SAVE) != 0) {
    SMU_MSG_CONTEXT_V8 SmuMsgContext;
    SmuMsgContext.GnbPciAddress.AddressValue = DevObject.DevPciAddress.AddressValue;
    SmuMsgContext.RequestId = SMC_MSG_SetBtcGbvHi;
    SmuMsgContext.RequestArgument[0] = SmuArg[0];
    SmuMsgContext.RequestArgument[1] = SmuArg[1];
    SmuMsgContext.RequestArgument[2] = SmuArg[2];
    SmuMsgContext.RequestArgument[3] = SmuArg[3];
    SmuMsgContext.RequestArgument[4] = 0;
    SmuMsgContext.RequestArgument[5] = 0;
    S3_SAVE_DISPATCH (DevObject.StdHeader, GnbSmuServiceRequestV8S3Script_ID, sizeof (SmuMsgContext), &SmuMsgContext);
  }

  //BTC voltage guard-band for P-State 0 to P-State 3
  GnbSmuServiceRequestV8 (
     &DevObject,
     SMC_MSG_GetBtcGbvLo,
     SmuArg,
     0
     );

  if ((AccessFlags & GNB_REG_ACC_FLAG_S3SAVE) != 0) {
    SMU_MSG_CONTEXT_V8 SmuMsgContext;
    SmuMsgContext.GnbPciAddress.AddressValue = DevObject.DevPciAddress.AddressValue;
    SmuMsgContext.RequestId = SMC_MSG_SetBtcGbvLo;
    SmuMsgContext.RequestArgument[0] = SmuArg[0];
    SmuMsgContext.RequestArgument[1] = SmuArg[1];
    SmuMsgContext.RequestArgument[2] = SmuArg[2];
    SmuMsgContext.RequestArgument[3] = SmuArg[3];
    SmuMsgContext.RequestArgument[4] = 0;
    SmuMsgContext.RequestArgument[5] = 0;
    S3_SAVE_DISPATCH (DevObject.StdHeader, GnbSmuServiceRequestV8S3Script_ID, sizeof (SmuMsgContext), &SmuMsgContext);
  }

}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Boot Time Cal Interface
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbBootTimeCalInterfaceCZ (
  IN      AMD_CONFIG_PARAMS   *StdHeader
  )
{
  AGESA_STATUS           AgesaStatus;
  GNB_HANDLE             *GnbHandle;
  DEV_OBJECT             DevObject;
  UINT8                  CalPgmIndex;
  UINT8                  NumberOfCalPgms;
  UINT32                 SmuArg[6];
  UINT32                 D0F0xBC_xC0016078;
  BTC_FAMILY_SERVICES   *BtcFamilyServices;
  GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBootTimeCalInterfaceCZ Enter\n");

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  if (!GnbBuildOptionData->GnbCommonOptions.CfgSmuMessageEnable) {
    IDS_HDT_CONSOLE (GNB_TRACE, "BIOS FORCE SKIP SMU SERVICE!!\n");
    return AgesaStatus;
  }

  GnbHandle = GnbGetHandle (StdHeader);
  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);

  NumberOfCalPgms = NUMBER_OF_CAL_PGMS;

  // Read fuse to see if this processor supports boot time calibration
  GnbRegisterReadCZ (GnbGetHandle (StdHeader), TYPE_D0F0xBC, 0xC0016078,
                     &D0F0xBC_xC0016078, 0, StdHeader);

  if ((GnbBuildOptionData->CfgBTCEnable == TRUE) && (((D0F0xBC_xC0016078 & BIT29) == BIT29) || ((D0F0xBC_xC0016078 & BIT28) == BIT28))) {
    GetFeatureServicesOfCurrentCore (&BtcFamilyServiceTable, (CONST VOID **)&BtcFamilyServices, StdHeader);

    // Step 1. Set the Pstate on all cores to Pmin(P7)
    AgesaStatus = BtcFamilyServices->BeforeBtc (BtcFamilyServices, StdHeader);
    if (AgesaStatus == AGESA_SUCCESS) {
      // Step 2. Send BTC_START. This initializes code and hardware (SPMI, PSMs, etc).
      // Step 3. Wait for SMU completion, SMU sets the register which is polled.
      SmuArg[0] = BTC_START;
      GnbSmuServiceRequestV8 (
         &DevObject,
         SMC_MSG_BC,
         SmuArg,
         0
         );

      // Step 4. Execute all the tests.
      for (CalPgmIndex = 0; CalPgmIndex < NumberOfCalPgms; CalPgmIndex++) {
        // Step 4.1 Execute calibration pgm.X
        // Don't run workload for now
        //BtcFamilyServices->RunBtc (BtcFamilyServices, CalPgmIndex, StdHeader);

        // Step 4.2 Send test number to SMU.
        // Step 4.3. Wait for SMU completion
        IDS_HDT_CONSOLE (GNB_TRACE, "Starting test number %d\n", FIRST_CAL_PGM + CalPgmIndex);
        SmuArg[0] = FIRST_CAL_PGM + CalPgmIndex;
        GnbSmuServiceRequestV8 (
           &DevObject,
           SMC_MSG_BC,
           SmuArg,
           0
           );
      }

      // Step 5. Restore Pstate on all cores.
      BtcFamilyServices->AfterBtc (BtcFamilyServices, StdHeader);

    }
  }

  // Both save and restore needs to happen even if BTC fuses are disabled
  // Step 6. Get GBV and set back for S3.
  BtcGetSetGbvCZ (GnbHandle, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

  // Step 7. Update Shadow VID tables
  // Call SMU service instead of CPU routine
  SmuArg[0] = 0;
  GnbSmuServiceRequestV8 (
     &DevObject,
     SMC_MSG_ApplyShadowPstates,
     SmuArg,
     GNB_REG_ACC_FLAG_S3SAVE
     );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbBootTimeCalInterfaceCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
