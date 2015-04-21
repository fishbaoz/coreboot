/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB mid post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313866 $   @e \$Date: 2015-03-02 16:06:17 +0800 (Mon, 02 Mar 2015) $
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
#include  "Gnb.h"
#include  "GnbPcieConfig.h"
#include  "GnbFuseTable.h"
#include  "heapManager.h"
#include  "GnbCommonLib.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbTable.h"
#include  "GnbRegisterAccST.h"
#include  "GnbRegistersST.h"
#include  "GnbFamServices.h"
#include  "OptionGnb.h"
#include  "GnbSmuInitLibV8.h"
#include  "GnbPcieInitLibV1.h"
#include  "heapManager.h"
#include  "GnbAzalialib.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITST_GNBMIDINITST_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern GNB_TABLE ROMDATA      GnbMidInitTableST[];

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GnbMidInterfaceST (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

/*----------------------------------------------------------------------------------------*/
/**
 * ORB TX/RX Memory power gating
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
STATIC
GnbOrbMidMemoryPowerGating (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_HANDLE                *GnbHandle;
  GNB_BUILD_OPTIONS_ST      *GnbBuildOptionData;
  D0F0x98_x02_STRUCT        D0F0x98_x02;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbOrbMidMemoryPowerGating Enter\n");
  Status = AGESA_SUCCESS;

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  GnbRegisterReadST (GnbHandle, D0F0x98_x02_TYPE, D0F0x98_x02_ADDRESS, &D0F0x98_x02.Value, 0, StdHeader);

  if (GnbBuildOptionData->CfgOrbTxMemPowerGating != OrbMemPowerGatingDisabled) {
    D0F0x98_x02.Field.OrbTxPgmemEn = 1;
    if (GnbBuildOptionData->CfgOrbTxMemPowerGating == OrbMemPowerGatingSD) {
      D0F0x98_x02.Field.TxPgmemStEn = 4;
    } else if (GnbBuildOptionData->CfgOrbTxMemPowerGating == OrbMemPowerGatingDS) {
      D0F0x98_x02.Field.TxPgmemStEn = 2;
    } else {
      D0F0x98_x02.Field.TxPgmemStEn = 1;
    }
  }

  if (GnbBuildOptionData->CfgOrbRxMemPowerGating != OrbMemPowerGatingDisabled) {
    D0F0x98_x02.Field.OrbRxPgmemEn = 1;
    if (GnbBuildOptionData->CfgOrbRxMemPowerGating == OrbMemPowerGatingSD) {
      D0F0x98_x02.Field.RxPgmemStEn = 4;
    } else if (GnbBuildOptionData->CfgOrbRxMemPowerGating == OrbMemPowerGatingDS) {
      D0F0x98_x02.Field.RxPgmemStEn = 2;
    } else {
      D0F0x98_x02.Field.RxPgmemStEn = 1;
    }
  }

  D0F0x98_x02.Field.PgmemHysteresis = 0x3F;

  GnbRegisterWriteST (GnbHandle, D0F0x98_x02_TYPE, D0F0x98_x02_ADDRESS, &D0F0x98_x02.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbOrbMidMemoryPowerGating Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * SPG ACGAZ Memory power gating
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
STATIC
GnbSPGACGAZMidMemoryPowerGating (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_HANDLE                *GnbHandle;
  GNB_BUILD_OPTIONS_ST      *GnbBuildOptionData;
  UINT32                    D9F2xEC_x50;
  UINT32                    D8F0xEC_x50;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSPGACGAZMidMemoryPowerGating Enter\n");
  Status = AGESA_SUCCESS;

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  GnbRegisterReadST (GnbHandle, D9F2xEC_x50_TYPE, D9F2xEC_x50_ADDRESS, &D9F2xEC_x50, 0, StdHeader);
  GnbRegisterReadST (GnbHandle, D8F0xEC_x50_TYPE, D8F0xEC_x50_ADDRESS, &D8F0xEC_x50, 0, StdHeader);

  D9F2xEC_x50 &= 0xFFFFFFFC;
  D8F0xEC_x50 &= 0xFFFFFFFC;
  D9F2xEC_x50 |= (GnbBuildOptionData->CfgSpgMemPowerGatingEnable & 0x3);
  D8F0xEC_x50 |= (GnbBuildOptionData->CfgAcgAzMemPowerGatingEnable & 0x3);

  GnbRegisterWriteST (GnbHandle, D9F2xEC_x50_TYPE, D9F2xEC_x50_ADDRESS, &D9F2xEC_x50, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  GnbRegisterWriteST (GnbHandle, D8F0xEC_x50_TYPE, D8F0xEC_x50_ADDRESS, &D8F0xEC_x50, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSPGACGAZMidMemoryPowerGating Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Program IOMMU MMIO registers
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
STATIC
GnbIommuMidInitProgramMmRegST (
  IN       GNB_HANDLE            *GnbHandle,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT16                  CapabilityOffset;
  UINT64                  BaseAddress;
  UINT32                  Value;
  PCI_ADDR                GnbIommuPciAddress;
  UINT8                   i;
  UINT32                  SupData;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitProgramMmRegST Enter\n");

  GnbIommuPciAddress = GnbGetIommuPciAddressV4 (GnbHandle, StdHeader);
  CapabilityOffset = GnbLibFindPciCapability (GnbIommuPciAddress.AddressValue, IOMMU_CAP_ID, StdHeader);

  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x8), AccessWidth32, &Value, StdHeader);
  BaseAddress = (UINT64) Value << 32;
  GnbLibPciRead (GnbIommuPciAddress.AddressValue | (CapabilityOffset + 0x4), AccessWidth32, &Value, StdHeader);
  BaseAddress |= Value;
  BaseAddress &= 0xfffffffffffffffe;
  IDS_HDT_CONSOLE (GNB_TRACE, "IOMMU base address %x\n", BaseAddress);
  // ISOC
  GnbLibMemRMW (BaseAddress + 0x18, AccessS3SaveWidth32, 0xFFFFF7FF, 0x800, StdHeader);
  // NXSUP
  SupData = 0;
  GnbLibMemRead (BaseAddress + 0x30, AccessWidth32, &Value, StdHeader);
  if ((Value & BIT3) != 0) {
    SupData |= BIT2;
  }
  // USSUP
  GnbLibMemRead (BaseAddress + 0x34, AccessWidth32, &Value, StdHeader);
  if ((Value & BIT5) != 0) {
    SupData |= BIT1;
  }
  for (i = 0; i < 4; i++) {
    GnbRegisterReadST (
      GnbHandle,
      D0F2xFC_x27_L1_TYPE,
      D0F2xFC_x27_L1_ADDRESS (i),
      &Value,
      0,
      StdHeader
      );
    Value |= SupData;
    GnbRegisterWriteST (
      GnbHandle,
      D0F2xFC_x27_L1_TYPE,
      D0F2xFC_x27_L1_ADDRESS (i),
      &Value,
      GNB_REG_ACC_FLAG_S3SAVE,
      StdHeader
      );
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitProgramMmRegST Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Override IOMMU PCIE Strap
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 */

VOID
STATIC
GnbIommuMidInitPcieStrapOverrideST (
  IN       GNB_HANDLE            *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG     *EngineList;
  AMD_CONFIG_PARAMS      *StdHeader;
  PCIe_PLATFORM_CONFIG   *Pcie;
  AGESA_STATUS           AgesaStatus;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitPcieStrapOverrideST Enter\n");

  StdHeader = PcieConfigGetStdHeader (GnbHandle);
  AgesaStatus = PcieLocateConfigurationData (StdHeader, &Pcie);
  if (AgesaStatus != AGESA_SUCCESS) {
    return;
  }

  EngineList = PcieConfigGetChildEngine (GnbHandle);
  while (EngineList != NULL) {
    if (PcieConfigIsPcieEngine (EngineList)) {

      if (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS)) {
        PciePortRegisterRMW (
          EngineList,
          DxFxxE4_xC1_ADDRESS,
          DxFxxE4_xC1_StrapE2EPrefixEn_MASK | DxFxxE4_xC1_StrapExtendedFmtSupported_MASK,
          (1 << DxFxxE4_xC1_StrapE2EPrefixEn_OFFSET) | (1 << DxFxxE4_xC1_StrapExtendedFmtSupported_OFFSET),
          TRUE,
          Pcie
          );
      }
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitPcieStrapOverrideST Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Registers needs to be set if no GFX PCIe ports being us
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 */

VOID
STATIC
GnbIommuMidInitPcieCoreUsageST (
  IN       GNB_HANDLE            *GnbHandle
  )
{
  PCIe_ENGINE_CONFIG    *EngineList;
  D0F2xF4_x57_STRUCT    D0F2xF4_x57;
  UINT32                AllCoreDis;
  AMD_CONFIG_PARAMS     *StdHeader;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitPcieCoreUsageST Enter\n");
  //
  // Initial asusmption that no wrapper/cores are booting used
  //
  AllCoreDis = 1;

  EngineList = PcieConfigGetChildEngine (GnbHandle);
  while (EngineList != NULL) {
    if (PcieConfigIsPcieEngine (EngineList)) {
      if (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
        ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) && (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard))) {
          //Wrap ID correspoing to bit in D0F2xF4_x57
          AllCoreDis = 0;
          break;
      }
    }
    EngineList = (PCIe_ENGINE_CONFIG *) PcieConfigGetNextTopologyDescriptor (EngineList, DESCRIPTOR_TERMINATE_GNB);
  }

  StdHeader = PcieConfigGetStdHeader (GnbHandle);
  GnbRegisterReadST (GnbHandle, D0F2xF4_x57_TYPE, D0F2xF4_x57_ADDRESS, &D0F2xF4_x57.Value, 0, StdHeader);
  D0F2xF4_x57.Value |= BIT1;
  if (AllCoreDis == 1) {
    D0F2xF4_x57.Value |= BIT0;
  }
  GnbRegisterWriteST (GnbHandle, D0F2xF4_x57_TYPE, D0F2xF4_x57_ADDRESS, &D0F2xF4_x57.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitPcieCoreUsageST Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to for each PCIe port
 *
 *
 *
 *
 * @param[in]       CoreId        CoreId
 *
 */

UINT8
STATIC
GnbIommuCoreIdToL1SelST (
  IN       UINT8                 CoreId
  )
{
  UINT8 Sel;
  Sel = 0;
  switch (CoreId) {
  // GPP
  case 0x0:
    Sel = L1_SEL_PPx40;
    break;

  default:
    ASSERT (FALSE);
  }
  return Sel;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Callback to for each PCIe port
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
GnbIommuMidInitOnPortCallbackST (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  GNB_TOPOLOGY_INFO       TopologyInfo;
  GNB_HANDLE              *GnbHandle;
  D0F2xFC_x07_L1_STRUCT   D0F2xFC_x07_L1;
  D0F2xFC_x0D_L1_STRUCT   D0F2xFC_x0D_L1;

  TopologyInfo.PhantomFunction = FALSE;
  TopologyInfo.PcieToPciexBridge = FALSE;
  GnbHandle = (GNB_HANDLE *) PcieConfigGetParentSilicon (Engine);
  if (Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) {
    TopologyInfo.PhantomFunction = TRUE;
    TopologyInfo.PcieToPciexBridge = TRUE;
  } else {
    if (PcieConfigIsSbPcieEngine (Engine)) {
      PCI_ADDR  StartSbPcieDev;
      PCI_ADDR  EndSbPcieDev;
      StartSbPcieDev.AddressValue = MAKE_SBDFO (0, 0, 0x15, 0, 0);
      EndSbPcieDev.AddressValue = MAKE_SBDFO (0, 0, 0x15, 7, 0);
      GnbGetTopologyInfoV4 (StartSbPcieDev, EndSbPcieDev, &TopologyInfo, GnbLibGetHeader (Pcie));
    } else {
      GnbGetTopologyInfoV4 (Engine->Type.Port.Address, Engine->Type.Port.Address, &TopologyInfo, GnbLibGetHeader (Pcie));
    }
  }
  if (TopologyInfo.PhantomFunction) {
    GnbRegisterReadST (
      GnbHandle,
      D0F2xFC_x07_L1_TYPE,
      D0F2xFC_x07_L1_ADDRESS (GnbIommuCoreIdToL1SelST (Engine->Type.Port.CoreId)),
      &D0F2xFC_x07_L1.Value,
      0,
      GnbLibGetHeader (Pcie)
      );
    D0F2xFC_x07_L1.Value |= BIT0;
    GnbRegisterWriteST (
      GnbHandle,
      D0F2xFC_x07_L1_TYPE,
      D0F2xFC_x07_L1_ADDRESS (GnbIommuCoreIdToL1SelST (Engine->Type.Port.CoreId)),
      &D0F2xFC_x07_L1.Value,
      GNB_REG_ACC_FLAG_S3SAVE,
      GnbLibGetHeader (Pcie)
      );
  }
  if (TopologyInfo.PcieToPciexBridge) {
    GnbRegisterReadST (
      GnbHandle,
      D0F2xFC_x0D_L1_TYPE,
      D0F2xFC_x0D_L1_ADDRESS (GnbIommuCoreIdToL1SelST (Engine->Type.Port.CoreId)),
      &D0F2xFC_x0D_L1.Value,
      0,
      GnbLibGetHeader (Pcie)
      );
    D0F2xFC_x0D_L1.Field.VOQPortBits = 0x7;
    GnbRegisterWriteST (
      GnbHandle,
      D0F2xFC_x0D_L1_TYPE,
      D0F2xFC_x0D_L1_ADDRESS (GnbIommuCoreIdToL1SelST (Engine->Type.Port.CoreId)),
      &D0F2xFC_x0D_L1.Value,
      GNB_REG_ACC_FLAG_S3SAVE,
      GnbLibGetHeader (Pcie)
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * IOMMU Mid Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
STATIC
GnbIommuMidInitST (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  PCIe_PLATFORM_CONFIG  *Pcie;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitST Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  if (Status == AGESA_SUCCESS) {
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
      GnbIommuMidInitOnPortCallbackST,
      NULL,
      Pcie
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitST Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Mid Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbMidInterfaceST (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  UINT32            Property;
  AGESA_STATUS      AgesaStatus;
  GNB_HANDLE        *GnbHandle;
  GNB_BUILD_OPTIONS_ST      *GnbBuildOptionData;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbMidInterfaceST Enter\n");

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  Status = GnbIommuMidInitST (StdHeader);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Property = TABLE_PROPERTY_DEFAULT;
  Property |= GnbBuildOptionData->GnbCommonOptions.CfgOrbClockGatingEnable ? TABLE_PROPERTY_ORB_CLK_GATING : 0;
  Property |= GnbBuildOptionData->GnbCommonOptions.CfgIocLclkClockGatingEnable ? TABLE_PROPERTY_IOC_LCLK_CLOCK_GATING : 0;
  Property |= GnbBuildOptionData->CfgSpgClockGatingEnable ? TABLE_PROPERTY_SPG_CLOCK_GATING : 0;
  Property |= GnbBuildOptionData->CfgAcgAzClockGatingEnable ? TABLE_PROPERTY_ACG_AZ_CLOCK_GATING : 0;

  GnbHandle = GnbGetHandle (StdHeader);
  if (GnbFmCheckIommuPresent (GnbHandle, StdHeader)) {
    Status = GnbEnableIommuMmioV4 (GnbHandle, StdHeader);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    if (Status == AGESA_SUCCESS) {
      GnbIommuMidInitProgramMmRegST (GnbHandle, StdHeader);
    }
    GnbIommuMidInitPcieCoreUsageST (GnbHandle);
  }

  Status = GnbProcessTable (
             GnbHandle,
             GnbMidInitTableST,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = GnbOrbMidMemoryPowerGating (StdHeader);
  Status = GnbSPGACGAZMidMemoryPowerGating (StdHeader);

  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbMidInterfaceST Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
