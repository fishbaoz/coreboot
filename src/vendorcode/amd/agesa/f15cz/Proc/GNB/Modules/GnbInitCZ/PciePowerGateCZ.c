/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe power gate initialization
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-09 12:28:05 -0600 (Tue, 09 Dec 2014) $
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
#include  "amdlib.h"
#include  "Ids.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "OptionGnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieFamServices.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbSmuInitLibV8.h"
#include  "GnbRegistersCZ.h"
#include  "GnbRegisterAccCZ.h"
#include  "PciePowerGateCZ.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_PCIEPOWERGATECZ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_BUILD_OPTIONS      GnbBuildOptions;

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
 * Power down unused lanes
 *
 *
 *
 *
 * @param[in]       Wrapper         Pointer to wrapper configuration
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 * @retval          AGESA_SUCCESS
 *
 */

AGESA_STATUS
STATIC
PciePowerGatePowerDownUnusedLanesCallbackCZ (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Index;
  UINTN                   State;
  UINT32                  LaneBitmap;
  UINT32                  LaneBitmapNative;
  UINT16                  StartLane;
  UINT16                  EndLane;
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  GNB_HANDLE              *GnbHandle;
  SMU_PCIE_PHY_LN_STRUCT  Phy_Data;
  UINT8                   PowerGatingFlags;


  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGatePowerDownUnusedLanesCallbackCZ Enter\n");

  if (Wrapper->Features.PowerOffUnusedLanes != 0) {
    PowerGatingFlags = *(UINT8 *) Buffer;
    Phy_Data.Value = 0;
    GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));
    GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, (AMD_CONFIG_PARAMS *)Pcie->StdHeader, &DevObject, SmuArg);

    LaneBitmapNative = PcieUtilGetWrapperLaneBitMap (
                        LANE_TYPE_PHY_NATIVE_ALL,
                        0,
                        Wrapper
                        );
    LaneBitmap = PcieUtilGetWrapperLaneBitMap (
                   LANE_TYPE_PHY_NATIVE_ALL,
                   LANE_TYPE_PCIE_PHY_NATIVE_ALLOC_ACTIVE | LANE_TYPE_DDI_PHY_NATIVE_ACTIVE | LANE_TYPE_PCIE_PHY_NATIVE_HOTPLUG,
                   Wrapper
                   );
    if (LaneBitmap == LaneBitmapNative) {
      State = 0;
      StartLane = 0;
      EndLane = 0;
      for (Index = 0; Index <= (LibAmdBitScanReverse (LaneBitmap) + 1); Index++) {
        if ((State == 0) && ((LaneBitmap & (1 << Index)) != 0)) {
          StartLane = Index;
          State = 1;
        } else if ((State == 1) && ((LaneBitmap & (1 << Index)) == 0)) {
          EndLane = Index - 1;
          State = 0;

          Phy_Data.Field.Tx = 1;
          Phy_Data.Field.Rx = 1;
          Phy_Data.Field.Core = 1;
          if ((PowerGatingFlags & PCIE_POWERGATING_SKIP_CORE) != 0) {
            Phy_Data.Field.SkipCore = 1;
          }
          if ((PowerGatingFlags & PCIE_POWERGATING_SKIP_PHY) != 0) {
            Phy_Data.Field.SkipPhy = 1;
          }
          Phy_Data.Field.LowerLaneID = StartLane + Wrapper->StartPhyLane;
          Phy_Data.Field.UpperLaneID = EndLane + Wrapper->StartPhyLane;
          IDS_HDT_CONSOLE (
            GNB_TRACE,
            "  LowerLaneID - %02d UpperLaneID - %02d Tx - %d  Rx - %d Core - %d Exit\n",
            Phy_Data.Field.LowerLaneID,
            Phy_Data.Field.UpperLaneID,
            Phy_Data.Field.Tx,
            Phy_Data.Field.Rx,
            Phy_Data.Field.Core
            );

          SmuArg[0] = Phy_Data.Value;
          GnbSmuServiceRequestV8 (
            &DevObject,
            SMC_MSG_PHY_LN_OFF,
            SmuArg,
            GNB_REG_ACC_FLAG_S3SAVE
            );
        }
      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGatePowerDownUnusedLanesCallbackCZ Exit\n");
  return AGESA_SUCCESS;
}




/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Power gate init
 *
 *   Late PCIe initialization
 *
 * @param[in]  Pcie                Pointer to global PCIe configuration
 * @retval     AGESA_SUCCESS       Topology successfully mapped
 */

AGESA_STATUS
PciePowerGateCZ (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   PowerGatingFlags;
  GNB_HANDLE              *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateCZ Enter\n");

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));

  PowerGatingFlags = GnbBuildOptions.CfgPciePowerGatingFlags;

  IDS_HDT_CONSOLE (GNB_TRACE, "  PowerGatingFlags = %02x\n", PowerGatingFlags);

  IDS_OPTION_HOOK (IDS_GNB_PCIE_POWER_GATING, &PowerGatingFlags, GnbLibGetHeader (Pcie));
  IDS_HDT_CONSOLE (GNB_TRACE, "  PowerGatingFlags = %02x\n", PowerGatingFlags);

  // Power down unused lanes
  PcieConfigRunProcForAllWrappers (
    DESCRIPTOR_PCIE_WRAPPER | DESCRIPTOR_DDI_WRAPPER,
    PciePowerGatePowerDownUnusedLanesCallbackCZ,
    &PowerGatingFlags,
    Pcie
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerGateCZ Exit\n");
  return  AGESA_SUCCESS;
}
