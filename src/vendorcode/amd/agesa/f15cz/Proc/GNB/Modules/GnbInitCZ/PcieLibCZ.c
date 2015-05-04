/* $NoKeywords:$ */
/**
 * @file
 *
 * CZ specific PCIe services
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 311871 $   @e \$Date: 2015-01-28 01:03:13 -0600 (Wed, 28 Jan 2015) $
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
#include  "heapManager.h"
#include  "Gnb.h"
#include  "GnbPcie.h"
#include  "GnbSbLib.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV5.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV5.h"
#include  "PcieComplexDataCZ.h"
#include  "PcieLibCZ.h"
#include  "GnbRegistersCZ.h"
#include  "GnbRegisterAccCZ.h"
#include  "GnbFuseTable.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_PCIELIBCZ_FILECODE
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
 * Power down inactive lanes
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
PciePowerDownPllInL1CZ (
  IN       PCIe_WRAPPER_CONFIG    *Wrapper,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  UINT32              LaneBitmapForPllOffInL1;
  UINT32              ActiveLaneBitmap;
  UINT8               PllPowerUpLatency;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerDownPllInL1CZ Enter\n");
  ActiveLaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE_ACTIVE | LANE_TYPE_PCIE_PHY_NATIVE_HOTPLUG, 0, Wrapper);
  if (ActiveLaneBitmap != 0) {
    PllPowerUpLatency = PciePifGetPllPowerUpLatencyCZ (Wrapper, Pcie);
    LaneBitmapForPllOffInL1 = PcieLanesToPowerDownPllInL1 (PllPowerUpLatency, Wrapper, Pcie);
    if ((LaneBitmapForPllOffInL1 != 0) && ((ActiveLaneBitmap & LaneBitmapForPllOffInL1) == ActiveLaneBitmap)) {
      LaneBitmapForPllOffInL1 &= PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PHY_NATIVE_ALL, 0, Wrapper);
      LaneBitmapForPllOffInL1 |= PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE_MASTER_PLL, 0, Wrapper);
      PciePifSetPllModeForL1 (LaneBitmapForPllOffInL1, Wrapper, Pcie);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePowerDownPllInL1CZ Exit\n");
}



/*----------------------------------------------------------------------------------------*/
/**
 * Request boot up voltage
 *
 *
 *
 * @param[in]  LinkCap             Global GEN capability
 * @param[in]  Pcie                Pointer to PCIe configuration data area
 */
VOID
PcieSetVoltageCZ (
  IN      PCIE_LINK_SPEED_CAP   LinkCap,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                      TargetVid;
  UINT8                      MinVidIndex;
  UINT8                      VddNbVid[8];
  UINT8                      Index;
  PP_FUSE_ARRAY_V2           *PpFuseArray;
  UINT32                     Millivolt[6];
  D0F0xBC_xC0014004_STRUCT   D0F0xBC_xC0014004;
  D0F0xBC_xC0014007_STRUCT   D0F0xBC_xC0014007;
  D0F0xBC_xC0014008_STRUCT   D0F0xBC_xC0014008;
  D0F0xBC_xC00160C4_STRUCT   D0F0xBC_xC00160C4;
  D0F0xBC_xC00160C7_STRUCT   D0F0xBC_xC00160C7;
  D0F0xBC_xC00160C8_STRUCT   D0F0xBC_xC00160C8;
  D0F0xBC_xC001706C_STRUCT   D0F0xBC_xC001706C;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieSetVoltageCZ Enter\n");
  Index = 0;

  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, GnbLibGetHeader (Pcie));
  if (PpFuseArray == NULL) {
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0014004_TYPE, D0F0xBC_xC0014004_ADDRESS, &D0F0xBC_xC0014004, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0014007_TYPE, D0F0xBC_xC0014007_ADDRESS, &D0F0xBC_xC0014007, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC0014008_TYPE, D0F0xBC_xC0014008_ADDRESS, &D0F0xBC_xC0014008, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC00160C4_TYPE, D0F0xBC_xC00160C4_ADDRESS, &D0F0xBC_xC00160C4, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC00160C7_TYPE, D0F0xBC_xC00160C7_ADDRESS, &D0F0xBC_xC00160C7, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC00160C8_TYPE, D0F0xBC_xC00160C8_ADDRESS, &D0F0xBC_xC00160C8, 0, GnbLibGetHeader (Pcie));
    GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), D0F0xBC_xC001706C_TYPE, D0F0xBC_xC001706C_ADDRESS, &D0F0xBC_xC001706C, 0, GnbLibGetHeader (Pcie));
    VddNbVid[0] = (UINT8) D0F0xBC_xC0014004.Field.GnbVid0;
    VddNbVid[1] = (UINT8) D0F0xBC_xC0014004.Field.GnbVid1;
    VddNbVid[2] = (UINT8) D0F0xBC_xC0014007.Field.GnbVid2;
    VddNbVid[3] = (UINT8) D0F0xBC_xC0014008.Field.GnbVid3;
    VddNbVid[4] = (UINT8) D0F0xBC_xC0014008.Field.GnbVid4;
    VddNbVid[5] = (UINT8) D0F0xBC_xC00160C4.Field.GnbVid5;
    VddNbVid[6] = (UINT8) D0F0xBC_xC00160C7.Field.GnbVid6;
    VddNbVid[7] = (UINT8) D0F0xBC_xC00160C8.Field.GnbVid7;
    if (LinkCap <= PcieGen2) {
      Index = (UINT8) D0F0xBC_xC001706C.Field.PcieGen2Vid;
    } else if (LinkCap >= PcieGen3) {
      Index = (UINT8) D0F0xBC_xC001706C.Field.PcieGen3Vid;
    }
  } else {
    VddNbVid[0] = PpFuseArray->VddNbVid[0];
    VddNbVid[1] = PpFuseArray->VddNbVid[1];
    VddNbVid[2] = PpFuseArray->VddNbVid[2];
    VddNbVid[3] = PpFuseArray->VddNbVid[3];
    VddNbVid[4] = PpFuseArray->VddNbVid[4];
    VddNbVid[5] = PpFuseArray->VddNbVid[5];
    VddNbVid[6] = PpFuseArray->VddNbVid[6];
    VddNbVid[7] = PpFuseArray->VddNbVid[7];
    if (LinkCap <= PcieGen2) {
      Index = PpFuseArray->PcieGen2Vid;
    } else if (LinkCap >= PcieGen3) {
      Index = PpFuseArray->PcieGen3Vid;
    }
  }

  if (LinkCap > PcieGen1) {

    ASSERT (VddNbVid[Index] != 0);
    TargetVid = VddNbVid[Index];

  } else {

    MinVidIndex = 0;
    for (Index = 0; Index < (sizeof (VddNbVid) / sizeof (VddNbVid[0])); Index++) {
      IDS_HDT_CONSOLE (PCIE_MISC, "  VddNbVid[%d] %x\n", Index, VddNbVid[Index]);
      if (VddNbVid[Index] > VddNbVid[MinVidIndex]) {
        MinVidIndex = (UINT8) Index;
      }
    }
    //ASSERT (VddNbVid[MinVidIndex] != 0);
    TargetVid = VddNbVid[MinVidIndex];

  }

  Millivolt[0] = GnbTranslateVidCodeToMillivoltV5 (TargetVid, GnbLibGetHeader (Pcie)) * 4 / 100;
  IDS_HDT_CONSOLE (PCIE_MISC, "  Set Voltage for Gen %d, Vid code %d, Millivolt with 2 fraction = %d\n", LinkCap, TargetVid, Millivolt[0]);

  Millivolt[1] = 0;
  Millivolt[2] = 0;
  Millivolt[3] = 0;
  Millivolt[4] = 0;
  Millivolt[5] = 0;

  if (TargetVid != 0) {
    GnbRegisterWriteCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), TYPE_SMU_MSG, SMC_MSG_VddNbRequest, Millivolt, 0, GnbLibGetHeader (Pcie));
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieSetVoltageCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * PLL power up latency
 *
 *
 * @param[in]  Wrapper     Pointer to Wrapper config descriptor
 * @param[in]  Pcie        Pointer to PICe configuration data area
 * @retval                 Pll wake up latency in us
 */
UINT8
PciePifGetPllPowerUpLatencyCZ (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  return 35;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get max link speed capability supported by this port
 *
 *
 *
 * @param[in]  Flags              See Flags PCIE_PORT_GEN_CAP_BOOT / PCIE_PORT_GEN_CAP_MAX
 * @param[in]  Engine             Pointer to engine config descriptor
 * @retval     PcieGen1/PcieGen2  Max supported link gen capability
 */
PCIE_LINK_SPEED_CAP
PcieGetLinkSpeedCapCZ (
  IN       UINT32                Flags,
  IN       PCIe_ENGINE_CONFIG    *Engine
  )
{
  PCIE_LINK_SPEED_CAP   LinkSpeedCapability;
  PCIe_WRAPPER_CONFIG   *Wrapper;
  PCIe_PLATFORM_CONFIG  *Pcie;

  Wrapper = PcieConfigGetParentWrapper (Engine);
  Pcie = PcieConfigGetPlatform (Wrapper);

  LinkSpeedCapability = PcieGen3;
  IDS_HDT_CONSOLE (GNB_TRACE, "Init LinkSpeedCapability = %x\n", LinkSpeedCapability);

  if (Engine->Type.Port.PortData.LinkSpeedCapability == PcieGenMaxSupported) {
    Engine->Type.Port.PortData.LinkSpeedCapability = (UINT8) LinkSpeedCapability;
    IDS_HDT_CONSOLE (GNB_TRACE, "  Change to PcieGenMaxSupported = %x\n", LinkSpeedCapability);
  }
  if (Pcie->PsppPolicy == PsppPowerSaving) {
    LinkSpeedCapability = PcieGen1;
    IDS_HDT_CONSOLE (GNB_TRACE, "  Change to PsppPowerSaving Gen1\n");
  }
  if (Engine->Type.Port.PortData.LinkSpeedCapability < LinkSpeedCapability) {
    LinkSpeedCapability = Engine->Type.Port.PortData.LinkSpeedCapability;
    IDS_HDT_CONSOLE (GNB_TRACE, "  Change to LinkSpeedCapability = %x\n", LinkSpeedCapability);
  }
  if ((Flags & PCIE_PORT_GEN_CAP_BOOT) != 0) {
    if ((Pcie->PsppPolicy == PsppBalanceLow || Engine->Type.Port.PortData.MiscControls.LinkSafeMode == PcieGen1) && !PcieConfigIsSbPcieEngine (Engine)) {
      LinkSpeedCapability = PcieGen1;
      IDS_HDT_CONSOLE (GNB_TRACE, "  Change to PCIE_PORT_GEN_CAP_BOOT with Gen1\n");
    }
  }
  return LinkSpeedCapability;
}


/*----------------------------------------------------------------------------------------*/
/**
 * Get timestamp
 *
 *
 *
 * @param[in]  StdHeader             Standard header
 * @retval     TimeStamp             Count
 */

UINT32
GnbTimeStampCZ (
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  UINT32 TimeStamp;

  TimeStamp = 0;

  GnbLibPciIndirectRead (
    MAKE_SBDFO (0, 0, 0, 0, 0xE0),
    0x13080F0,
    AccessWidth32,
    &TimeStamp,
    StdHeader
    );

  if (TimeStamp == 0) {
    GnbLibPciIndirectRead (
      MAKE_SBDFO (0, 0, 0, 0, 0xE0),
      0x13180F0,
      AccessWidth32,
      &TimeStamp,
      StdHeader
      );
  }

  return TimeStamp;

}

/*----------------------------------------------------------------------------------------*/
/**
 * Lanes enable/disable control
 *
 * @param[in]   Control             Lane control action
 * @param[in]   LaneBitMap          Core lanes bitmap
 * @param[in]   Wrapper             Pointer to Wrapper config descriptor
 * @param[in]   Pcie                Pointer to global PCIe configuration
 */
VOID
PcieTopologyLaneControlCZ (
  IN      LANE_CONTROL          Control,
  IN      UINT32                LaneBitMap,
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
    )
{
  D0F0xE4_CORE_0129_STRUCT  D0F0xE4_CORE_0129;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyLaneControlCZ Enter\n");
  D0F0xE4_CORE_0129.Value = PcieRegisterRead (
                            Wrapper,
                            CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0129_ADDRESS),
                            Pcie
                            );

  if (Control == EnableLanes) {
    D0F0xE4_CORE_0129.Value |= LaneBitMap;
  } else if (Control == DisableLanes) {
    D0F0xE4_CORE_0129.Value &= (~LaneBitMap);
  }
  D0F0xE4_CORE_0129.Value &= ((1 << Wrapper->NumberOfLanes) - 1);
  PcieRegisterWrite (
    Wrapper,
    CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0129_ADDRESS),
    D0F0xE4_CORE_0129.Value,
    TRUE,
    Pcie
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyLaneControlCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Power down unused lanes and plls
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
PciePwrPowerDownUnusedLanesCZ (
  IN       PCIe_WRAPPER_CONFIG    *Wrapper,
  IN       PCIe_PLATFORM_CONFIG   *Pcie
  )
{
  UINT32              UnusedLanes;
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownUnusedLanesCZ Enter\n");
  if (Wrapper->Features.PowerOffUnusedLanes != 0) {
    UnusedLanes = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_CORE_ALL, LANE_TYPE_PCIE_CORE_ALLOC_ACTIVE, Wrapper);
    PcieTopologyLaneControlCZ (
      DisableLanes,
      UnusedLanes,
      Wrapper,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePwrPowerDownUnusedLanesCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to init various features on all active ports
 *
 *
 *
 *
 * @param[in]       Engine          Pointer to engine config descriptor
 * @param[in, out]  Buffer          Not used
 * @param[in]       Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
DdiEdpPortDetectCallbackCZ (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS             Status;
  BOOLEAN                  *EdpPresent;

  Status = AGESA_SUCCESS;

  IDS_HDT_CONSOLE (GNB_TRACE, "DdiEdpPortDetectCallbackCZ Enter\n");
  if ((Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDP) ||
      (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeDpToLvds) ||
      (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDPToLvds) ||
      (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDPToLvdsSwInit)) {

    IDS_HDT_CONSOLE (GNB_TRACE, "Found eDP/LVDS Connector\n");
    EdpPresent = (BOOLEAN *) Buffer;
    *EdpPresent = TRUE;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "DdiEdpPortDetectCallbackCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe eDP port detection and poower down
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
PcieEdpPortPowerCheckCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  GMMx1206C_STRUCT      GMMx1206C;
  BOOLEAN               EdpPresent;
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  PCIe_PLATFORM_CONFIG  *Pcie;
  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEdpPortPowerCheckCZ Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    EdpPresent = FALSE;
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_DDI_ENGINE | DESCRIPTOR_VIRTUAL,
      DdiEdpPortDetectCallbackCZ,
      (VOID *)&EdpPresent,
      Pcie
      );
    if (EdpPresent == FALSE) {
      // Power off
      GnbRegisterReadCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), GMMx1206C_TYPE, GMMx1206C_ADDRESS, &GMMx1206C.Value, 0, GnbLibGetHeader (Pcie));
      GMMx1206C.Field.LVTMA_PWRSEQ_EN = 0;
      GMMx1206C.Field.LVTMA_PWRSEQ_TARGET_STATE = 0;
      GMMx1206C.Field.LVTMA_BLON_OVRD = 0;
      GnbRegisterWriteCZ (GnbGetHandle (GnbLibGetHeader (Pcie)), GMMx1206C_TYPE, GMMx1206C_ADDRESS, &GMMx1206C.Value, GNB_REG_ACC_FLAG_S3SAVE, GnbLibGetHeader (Pcie));
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieMidBeforeGfxInitCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}


