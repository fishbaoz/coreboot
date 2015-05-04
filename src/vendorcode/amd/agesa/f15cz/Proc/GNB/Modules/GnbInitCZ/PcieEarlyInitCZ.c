/* $NoKeywords:$ */
/**
 * @file
 *
 * PCIe early post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 311183 $   @e \$Date: 2015-01-19 08:51:58 -0600 (Mon, 19 Jan 2015) $
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
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbPcieTrainingV2.h"
#include  "GnbPcieInitLibV1.h"
#include  "GnbPcieInitLibV4.h"
#include  "GnbPcieInitLibV5.h"
#include  "PcieLibCZ.h"
#include  "PcieComplexDataCZ.h"
#include  "GnbRegistersCZ.h"
#include  "GnbRegisterAccCZ.h"
#include  "OptionGnb.h"
#include  "GnbPcieFamServices.h"
#include  "GnbSmuInitLibV8.h"
#include  "heapManager.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_PCIEEARLYINITCZ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA CoreInitTableCZ;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PcieInitEarlyTableCZ;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PhyLaneInitEarlyTableCZ;
extern CONST PCIE_HOST_REGISTER_TABLE_HEADER ROMDATA PhyWrapperInitEarlyTableCZ;
extern CONST PCIE_PORT_REGISTER_TABLE_HEADER ROMDATA PortInitEarlyTableCZ;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
PcieEarlyInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Prepare for reconfiguration
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
STATIC
PcieTopologyPrepareForReconfigCZ (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_CORE_0101_STRUCT  D0F0xE4_CORE_0101;
  UINT8                     CoreId;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyPrepareForReconfigCZ Enter\n");
  if (PcieLibIsPcieWrapper (Wrapper)) {
    for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {

      D0F0xE4_CORE_0101.Value = PcieRegisterRead (
                                   Wrapper,
                                   CORE_SPACE (CoreId, D0F0xE4_CORE_0101_ADDRESS),
                                   Pcie
                                   );

      D0F0xE4_CORE_0101.Field.CONFIG_XFER_MODE = 0x0;
      D0F0xE4_CORE_0101.Field.BLOCK_ON_IDLE = 0x1;
      PcieRegisterWrite (
        Wrapper,
        CORE_SPACE (CoreId, D0F0xE4_CORE_0101_ADDRESS),
        D0F0xE4_CORE_0101.Value,
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyPrepareForReconfigCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set core configuration according to PCIe port topology
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[out] ConfigChanged       Pointer to boolean indicator that configuration was changed
 * @param[in]  Pcie                Pointer to global PCIe configuration
 * @retval     AGESA_SUCCESS       Topology successfully mapped
 * @retval     AGESA_ERROR         Topology can not be mapped
 */

AGESA_STATUS
STATIC
PcieTopologySetCoreConfigCZ (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
     OUT   BOOLEAN               *ConfigChanged,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                     CoreId;
  AGESA_STATUS              Status;
  D0F0xE4_WRAP_0080_STRUCT  D0F0xE4_WRAP_0080;

  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologySetCoreConfigCZ Enter\n");
  if (PcieLibIsPcieWrapper (Wrapper)) {
    for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {
      UINT64  ConfigurationSignature;
      UINT8   NewConfigurationValue;
      ConfigurationSignature = PcieConfigGetConfigurationSignature (Wrapper, CoreId);
      Status = PcieFmGetCoreConfigurationValue (Wrapper, CoreId, ConfigurationSignature, &NewConfigurationValue);
      if (Status == AGESA_SUCCESS) {
        D0F0xE4_WRAP_0080.Value = PcieRegisterRead (
                                    Wrapper,
                                    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0080_ADDRESS),
                                    Pcie
                                    );
        IDS_HDT_CONSOLE (PCIE_MISC, "  Core Configuration: Wrapper [%s], CoreID [%d] - %s Original configuration - %s\n",
          PcieFmDebugGetWrapperNameString (Wrapper),
          CoreId,
          PcieFmDebugGetCoreConfigurationString (Wrapper, NewConfigurationValue),
          PcieFmDebugGetCoreConfigurationString (Wrapper, (UINT8) D0F0xE4_WRAP_0080.Field.StrapBifLinkConfig)
          );
        if (ConfigChanged != NULL) {
          if (D0F0xE4_WRAP_0080.Field.StrapBifLinkConfig != NewConfigurationValue) {
            *ConfigChanged = TRUE;
          }
        }
        D0F0xE4_WRAP_0080.Field.StrapBifLinkConfig = NewConfigurationValue;
        PcieRegisterWrite (
          Wrapper,
          WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0080_ADDRESS),
          D0F0xE4_WRAP_0080.Value,
          FALSE,
          Pcie
          );
      } else {
        IDS_HDT_CONSOLE (PCIE_MISC, "  ERROR! Core Configuration : Wrapper [%s], Signature [0x%x, 0x%x]\n",
          PcieFmDebugGetWrapperNameString (Wrapper),
          ((UINT32*)&ConfigurationSignature)[1],
          ((UINT32*)&ConfigurationSignature)[0]
          );
        PcieConfigDisableAllEngines (PciePortEngine | PcieDdiEngine, Wrapper);
      }
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologySetCoreConfigCZ Exit\n");
  return Status;
}

UINT8 LaneMuxSelectorArrayCZ[] = { 0, 1, 2, 3, 4, 5, 6, 7 };

/*----------------------------------------------------------------------------------------*/
/**
 * Locate mux array index
 *
 *
 *
 * @param[in, out]  LaneMuxSelectorArrayPtr     Pointer to mux selector array
 * @param[in]       LaneMuxValue                The value that match to array
 * @retval          Index                       Index successfully mapped
 */
UINT8
STATIC
PcieTopologyLocateMuxIndexCZ (
  IN OUT   UINT8                *LaneMuxSelectorArrayPtr,
  IN       UINT8                LaneMuxValue
  )
{
  UINT8  Index;
  for (Index = 0; Index < sizeof (LaneMuxSelectorArrayCZ); Index++ ) {
    if (LaneMuxSelectorArrayPtr [Index] ==  LaneMuxValue) {
      return Index;
    }
  }
  return 0;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Apply lane mux
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
STATIC
PcieTopologyApplyLaneMuxCZ (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_ENGINE_CONFIG  *EngineList;
  UINT32              Index;
  UINT8               RxLaneMuxSelectorArray [sizeof (LaneMuxSelectorArrayCZ)];
  UINT8               TxLaneMuxSelectorArray [sizeof (LaneMuxSelectorArrayCZ)];

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyApplyLaneMuxCZ Enter\n");
  if (PcieLibIsPcieWrapper (Wrapper)) {
    LibAmdMemCopy (
      &TxLaneMuxSelectorArray[0],
      &LaneMuxSelectorArrayCZ[0],
      sizeof (LaneMuxSelectorArrayCZ),
      GnbLibGetHeader (Pcie)
      );
    LibAmdMemCopy (
      &RxLaneMuxSelectorArray[0],
      &LaneMuxSelectorArrayCZ[0],
      sizeof (LaneMuxSelectorArrayCZ),
      GnbLibGetHeader (Pcie)
      );
    EngineList = PcieConfigGetChildEngine (Wrapper);
    while (EngineList != NULL) {
      if (PcieLibIsPcieEngine (EngineList) && PcieLibIsEngineAllocated (EngineList)) {
        UINT32  CoreLaneBitmap;
        UINT32  PifLaneBitmap;
        UINT8   CurrentCoreLane;
        UINT8   CurrentPifLane;

        CoreLaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_CORE_ALLOC, 0, EngineList);
        PifLaneBitmap = PcieUtilGetEngineLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, EngineList);
        IDS_HDT_CONSOLE (GNB_TRACE, "CoreLaneBitmap - %x, CurrentPifLane - %x\n", CoreLaneBitmap, PifLaneBitmap);


        while (CoreLaneBitmap != 0) {
          CurrentCoreLane = LibAmdBitScanForward (CoreLaneBitmap);
          CurrentPifLane = LibAmdBitScanForward (PifLaneBitmap);
          if (TxLaneMuxSelectorArray[CurrentPifLane] != CurrentCoreLane) {
            TxLaneMuxSelectorArray[PcieTopologyLocateMuxIndexCZ (TxLaneMuxSelectorArray, CurrentCoreLane)] = TxLaneMuxSelectorArray[CurrentPifLane];
            TxLaneMuxSelectorArray[CurrentPifLane] = CurrentCoreLane;
          }

          if (RxLaneMuxSelectorArray[CurrentCoreLane] != CurrentPifLane) {
            RxLaneMuxSelectorArray[PcieTopologyLocateMuxIndexCZ (RxLaneMuxSelectorArray, CurrentPifLane)] = RxLaneMuxSelectorArray[CurrentCoreLane];
            RxLaneMuxSelectorArray[CurrentCoreLane] = CurrentPifLane;
          }

          CoreLaneBitmap &= (~ (1 << CurrentCoreLane));
          PifLaneBitmap &= (~ (1 << CurrentPifLane));
        }
      }
      EngineList = PcieLibGetNextDescriptor (EngineList);
    }
    for (Index = 0; Index < 2; ++Index) {
      PcieRegisterWrite (
        Wrapper,
        CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0121_ADDRESS + Index),
        ((UINT32 *) TxLaneMuxSelectorArray) [Index],
        FALSE,
        Pcie
        );
      PcieRegisterWrite (
        Wrapper,
        CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0125_ADDRESS + Index),
        ((UINT32 *) RxLaneMuxSelectorArray) [Index],
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyApplyLaneMuxCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Select master PLL
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[out] ConfigChanged       Pointer to boolean indicator that configuration was changed
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */

VOID
STATIC
PcieTopologySelectMasterPllCZ (
  IN       PCIe_WRAPPER_CONFIG   *Wrapper,
     OUT   BOOLEAN               *ConfigChanged,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_CORE_0118_STRUCT  D0F0xE4_CORE_0118;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologySelectMasterPllCZ Enter\n");

  D0F0xE4_CORE_0118.Value = PcieRegisterRead (
                              Wrapper,
                              CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0118_ADDRESS),
                              Pcie
                              );

  //
  // CZ PCIE master pll always select PHY0 (0 <= lane <= 7)
  //
  if (Wrapper->MasterPll == GNB_PCIE_MASTERPLL_A) {
    D0F0xE4_CORE_0118.Field.MASTER_PCIE_PLL_SELECT = 0x0;
  } else {
    D0F0xE4_CORE_0118.Field.MASTER_PCIE_PLL_SELECT = 0x1;
  }

  if (ConfigChanged != NULL) {
    *ConfigChanged = (D0F0xE4_CORE_0118.Field.MASTER_PCIE_PLL_SELECT == 0) ? FALSE : TRUE;
  }
  PcieRegisterWrite (
    Wrapper,
    CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0118_ADDRESS),
    D0F0xE4_CORE_0118.Value,
    FALSE,
    Pcie
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologySelectMasterPllCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Execute/clean up reconfiguration
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
STATIC
PcieTopologyExecuteReconfigCZ (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_CORE_0101_STRUCT  D0F0xE4_CORE_0101;
  PCIe_SILICON_CONFIG       *Silicon;
  DEV_OBJECT                DevObject;
  UINT32                    SmuArg[6];

  if (PcieLibIsPcieWrapper (Wrapper)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyExecuteReconfigCZ Enter\n");

    D0F0xE4_CORE_0101.Value = PcieRegisterRead (
                                Wrapper,
                                CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0101_ADDRESS),
                                Pcie
                                );

    D0F0xE4_CORE_0101.Field.RECONFIGURE_EN = 0x1;
    D0F0xE4_CORE_0101.Field.RESET_PERIOD = 0x2;

    PcieRegisterWrite (
      Wrapper,
      CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0101_ADDRESS),
      D0F0xE4_CORE_0101.Value,
      FALSE,
      Pcie
      );

    Silicon = PcieConfigGetParentSilicon (Wrapper);
    DevObject.StdHeader = GnbLibGetHeader (Pcie);
    DevObject.GnbHandle = GnbGetHandle (GnbLibGetHeader (Pcie));
    DevObject.DevPciAddress.AddressValue = Silicon->Address.AddressValue;
    LibAmdMemFill (SmuArg, 0x00, sizeof (SmuArg), GnbLibGetHeader (Pcie));
    SmuArg [0] = Wrapper->WrapId;
    GnbSmuServiceRequestV8 (
      &DevObject,
      SMC_MSG_RECONFIGURE_SB,
      SmuArg,
      0
      );

    D0F0xE4_CORE_0101.Value = PcieRegisterRead (
                                Wrapper,
                                CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0101_ADDRESS),
                                Pcie
                                );

    D0F0xE4_CORE_0101.Field.RECONFIGURE_EN = 0x0;
    PcieRegisterWrite (
      Wrapper,
      CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_0101_ADDRESS),
      D0F0xE4_CORE_0101.Value,
      FALSE,
      Pcie
      );

    IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyExecuteReconfigCZ Exit\n");
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Cleanup reconfig
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
VOID
STATIC
PcieTopologyCleanUpReconfigCZ (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  D0F0xE4_CORE_0101_STRUCT  D0F0xE4_CORE_0101;
  UINT8                     CoreId;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyCleanUpReconfigCZ Enter\n");
  if (PcieLibIsPcieWrapper (Wrapper)) {
    for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {

      D0F0xE4_CORE_0101.Value = PcieRegisterRead (
                                   Wrapper,
                                   CORE_SPACE (CoreId, D0F0xE4_CORE_0101_ADDRESS),
                                   Pcie
                                   );

      D0F0xE4_CORE_0101.Field.CONFIG_XFER_MODE = 0x1;
      PcieRegisterWrite (
        Wrapper,
        CORE_SPACE (CoreId, D0F0xE4_CORE_0101_ADDRESS),
        D0F0xE4_CORE_0101.Value,
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieTopologyCleanUpReconfigCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set SSID
 *
 *
 * @param[in]  Ssid            SSID
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
STATIC
PcieSetSsidCZ (
  IN       UINT32                        Ssid,
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  if (PcieLibIsPcieWrapper (Wrapper)) {
    PcieRegisterWrite (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0046_ADDRESS),
      Ssid,
      FALSE,
      Pcie
      );
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Set port device/function mapping
 *
 *
 *
 * @param[in]  Descriptor          Silicon descriptor
 * @param[in]  Buffer              Pointer to buffer
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PciePortMapInitCallbackCZ (
  IN       PCIe_DESCRIPTOR_HEADER            *Descriptor,
  IN OUT   VOID                              *Buffer,
  IN       PCIe_PLATFORM_CONFIG              *Pcie
  )
{
  PcieSetPortPciAddressMapCZ ((PCIe_SILICON_CONFIG *) Descriptor);
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PHY lane parameter Init
 *
 *
 *
 * @param[in]  Wrapper             Pointer to wrapper config descriptor
 * @param[in]  Buffer              Pointer to buffer
 * @param[in]  Pcie                Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PciePhyLaneInitInitCallbackCZ (
  IN      PCIe_WRAPPER_CONFIG   *Wrapper,
  IN      VOID                  *Buffer,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8                   Phy;
  UINT8                   PhyLaneIndex;
  UINT8                   Lane;
  UINT32                  LaneBitmap;
  UINTN                   Index;

  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyLaneInitInitCallbackCZ Enter\n");

  LaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_CORE_ALLOC, 0, Wrapper);
  if (LaneBitmap == 0) {
    IDS_HDT_CONSOLE (GNB_TRACE, "No device allocated in this wrapper\n");
    return AGESA_SUCCESS;
  }

  LaneBitmap = PcieUtilGetWrapperLaneBitMap (LANE_TYPE_PCIE_PHY_NATIVE, 0, Wrapper);
  for (Lane = 0; Lane < Wrapper->NumberOfLanes; ++Lane) {
    Phy = Lane / MAX_NUM_LANE_PER_PHY;
    PhyLaneIndex = Lane - Phy * MAX_NUM_LANE_PER_PHY;
    if ((LaneBitmap & (1 << Lane)) != 0) {
      for (Index = 0; Index < PhyLaneInitEarlyTableCZ.Length; Index++) {
        UINT32 Value;
        Value = PcieRegisterRead (
                  Wrapper,
                  PHY_SPACE (Wrapper->WrapId, Phy, PhyLaneInitEarlyTableCZ.Table[Index].Reg + (PhyLaneIndex * 0x100)),
                  Pcie
                  );
        Value &= (~PhyLaneInitEarlyTableCZ.Table[Index].Mask);
        Value |= PhyLaneInitEarlyTableCZ.Table[Index].Data;
        PcieRegisterWrite (
          Wrapper,
          PHY_SPACE (Wrapper->WrapId, Phy, PhyLaneInitEarlyTableCZ.Table[Index].Reg + (PhyLaneIndex * 0x100)),
          Value,
          FALSE,
          Pcie
          );
      }
    }
  }
  for (Lane = 0; Lane < Wrapper->NumberOfLanes; Lane += MAX_NUM_LANE_PER_PHY) {
    Phy = Lane / MAX_NUM_LANE_PER_PHY;
    for (Index = 0; Index < PhyWrapperInitEarlyTableCZ.Length; Index++) {
      UINT32 Value;
      Value = PcieRegisterRead (
                Wrapper,
                PHY_SPACE (Wrapper->WrapId, Phy, PhyWrapperInitEarlyTableCZ.Table[Index].Reg),
                Pcie
                );
      Value &= (~PhyWrapperInitEarlyTableCZ.Table[Index].Mask);
      Value |= PhyWrapperInitEarlyTableCZ.Table[Index].Data;
      PcieRegisterWrite (
        Wrapper,
        PHY_SPACE (Wrapper->WrapId, Phy, PhyWrapperInitEarlyTableCZ.Table[Index].Reg),
        Value,
        FALSE,
        Pcie
        );
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PciePhyLaneInitInitCallbackCZ Exit\n");
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init core registers.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
STATIC
PcieEarlyCoreInitCZ (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  UINT8   CoreId;
  UINTN   Index;

  if (PcieLibIsPcieWrapper (Wrapper)) {
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyCoreInitCZ Enter\n");
    for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {
      for (Index = 0; Index < CoreInitTableCZ.Length; Index++) {
        UINT32 Value;
        Value = PcieRegisterRead (
                  Wrapper,
                  CORE_SPACE (CoreId, CoreInitTableCZ.Table[Index].Reg),
                  Pcie
                  );
        Value &= (~CoreInitTableCZ.Table[Index].Mask);
        Value |= CoreInitTableCZ.Table[Index].Data;
        PcieRegisterWrite (
          Wrapper,
          CORE_SPACE (CoreId, CoreInitTableCZ.Table[Index].Reg),
          Value,
          FALSE,
          Pcie
          );
      }
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyCoreInitCZ Exit\n");
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize hardware for Power Gating
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */

VOID
STATIC
PcieHwInitPowerGatingCZ (
  IN       PCIe_WRAPPER_CONFIG     *Wrapper,
  IN       PCIe_PLATFORM_CONFIG    *Pcie
  )
{
  UINT8   Pif;
  UINT32  Value;
  D0F0xE4_PIF_0008_STRUCT D0F0xE4_PIF_0008;
  D0F0xE4_PIF_000A_STRUCT D0F0xE4_PIF_000A;
  D0F0xE4_CORE_012A_STRUCT  D0F0xE4_CORE_012A;
  D0F0xE4_CORE_012C_STRUCT  D0F0xE4_CORE_012C;
  D0F0xE4_CORE_012D_STRUCT  D0F0xE4_CORE_012D;
  GNB_BUILD_OPTIONS_CZ    *GnbBuildOptionData;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieHwInitPowerGatingCZ Enter\n");

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, GnbLibGetHeader (Pcie));
  ASSERT (GnbBuildOptionData != NULL);

  Value = 0x0;
  if ((GnbBuildOptionData->CfgPcieHwInitPwerGating & PcieHwInitPwrGatingL1Pg) == PcieHwInitPwrGatingL1Pg) {
    Value = 0x1;
  }
  PcieRegisterWriteField (
    Wrapper,
    CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_003D_ADDRESS),
    D0F0xE4_CORE_003D_LC_L1_POWER_GATING_EN_OFFSET,
    D0F0xE4_CORE_003D_LC_L1_POWER_GATING_EN_WIDTH,
    Value,
    TRUE,
    Pcie
    );

  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    D0F0xE4_PIF_0008.Value = PcieRegisterRead (
                                 Wrapper,
                                 PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0008_ADDRESS),
                                 Pcie
                                 );
    D0F0xE4_PIF_000A.Value = PcieRegisterRead (
                                 Wrapper,
                                 PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_000A_ADDRESS),
                                 Pcie
                                 );
    D0F0xE4_PIF_0008.Field.TxpwrInOff = GnbBuildOptionData->CfgPcieTxpwrInOff;
    D0F0xE4_PIF_000A.Field.RxpwrInOff = GnbBuildOptionData->CfgPcieRxpwrInOff;

    D0F0xE4_PIF_000A.Field.RxEiDetInPs2Degrade = 0x0;
    D0F0xE4_PIF_0008.Field.TxpwrGatingInL1 = 0x0;
    D0F0xE4_PIF_000A.Field.RxpwrGatingInL1 = 0x0;
    if ((GnbBuildOptionData->CfgPcieHwInitPwerGating & PcieHwInitPwrGatingL1Pg) == PcieHwInitPwrGatingL1Pg) {
      D0F0xE4_PIF_0008.Field.TxpwrGatingInL1 = 0x1;
      D0F0xE4_PIF_000A.Field.RxpwrGatingInL1 = 0x1;
    }

    D0F0xE4_PIF_0008.Field.TxpwrGatingInUnused = 0x0;
    D0F0xE4_PIF_000A.Field.RxpwrGatingInUnused = 0x0;
    if ((GnbBuildOptionData->CfgPcieHwInitPwerGating & PcieHwInitPwrGatingOffPg) == PcieHwInitPwrGatingOffPg) {
      D0F0xE4_PIF_0008.Field.TxpwrGatingInUnused = 0x1;
      D0F0xE4_PIF_000A.Field.RxpwrGatingInUnused = 0x1;
    }

    PcieRegisterWrite (
          Wrapper,
          PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0008_ADDRESS),
          D0F0xE4_PIF_0008.Value,
          TRUE,
          Pcie
        );
    PcieRegisterWrite (
          Wrapper,
          PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_000A_ADDRESS),
          D0F0xE4_PIF_000A.Value,
          TRUE,
          Pcie
        );
  }

  D0F0xE4_CORE_012A.Value = PcieRegisterRead (
                                Wrapper,
                                CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_012A_ADDRESS),
                                Pcie
                                );
  D0F0xE4_CORE_012C.Value = PcieRegisterRead (
                                Wrapper,
                                CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_012C_ADDRESS),
                                Pcie
                                );
  D0F0xE4_CORE_012D.Value = PcieRegisterRead (
                                Wrapper,
                                CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_012D_ADDRESS),
                                Pcie
                                );

  D0F0xE4_CORE_012A.Field.LMLaneDegrade0 = 1;
  D0F0xE4_CORE_012A.Field.LMLaneDegrade1 = 1;
  D0F0xE4_CORE_012A.Field.LMLaneDegrade2 = 1;
  D0F0xE4_CORE_012A.Field.LMLaneDegrade3 = 1;

  D0F0xE4_CORE_012C.Field.LMLaneUnused0 = 1;
  D0F0xE4_CORE_012C.Field.LMLaneUnused1 = 1;
  D0F0xE4_CORE_012C.Field.LMLaneUnused2 = 1;
  D0F0xE4_CORE_012D.Field.LMLaneUnused3 = 1;

  PcieRegisterWrite (
      Wrapper,
      CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_012A_ADDRESS),
      D0F0xE4_CORE_012A.Value,
      TRUE,
      Pcie
      );
  PcieRegisterWrite (
      Wrapper,
      CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_012C_ADDRESS),
      D0F0xE4_CORE_012C.Value,
      TRUE,
      Pcie
      );
  PcieRegisterWrite (
      Wrapper,
      CORE_SPACE (Wrapper->StartPcieCoreId, D0F0xE4_CORE_012D_ADDRESS),
      D0F0xE4_CORE_012D.Value,
      TRUE,
      Pcie
      );

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieHwInitPowerGatingCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pcie TxPreset loading sequence
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
VOID
STATIC
PcieEarlyWrapperTxPresetLoadingSequenceCZ (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  UINT8   Pif;
  UINT8   CoreId;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyWrapperTxPresetLoadingSequenceCZ Enter\n");

  // Step 1:  program  TX preset value of PCIE_WRAPPER:PSX80/81_WRP_BIF_LANE_EQUALIZATION_CNTL  to 0x7 ( from h/w default 0xF )
  PcieRegisterRMW (
    Wrapper,
    WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0050_ADDRESS),
    D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_MASK | D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_MASK,
    (7 << D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlDsPortTxPreset_OFFSET) | (7 << D0F0xE4_WRAP_0050_StrapBifPcieLaneEqCntlUsPortTxPreset_OFFSET),
    TRUE,
    Pcie
    );

  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_BEFORE_TXPRESET_LOADING, Pcie, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);

  for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {
    // Step 2:  program TOGGLESTRAP bit of PCIE_WRAPPER:PSX80/81_BIF_SWRST_COMMAND_1  to 0x1
    PcieRegisterRMW (
      Wrapper,
      CORE_SPACE (CoreId, D0F0xE4_CORE_0103_ADDRESS),
      D0F0xE4_CORE_0103_Togglestrap_MASK,
      (1 << D0F0xE4_CORE_0103_Togglestrap_OFFSET),
      TRUE,
      Pcie
      );
    // Wait for ~50ns
    GnbLibStall (1, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
    // program TOGGLESTRAP bit of PCIE_WRAPPER:PSX80/81_BIF_SWRST_COMMAND_1  to 0x0
    PcieRegisterRMW (
      Wrapper,
      CORE_SPACE (CoreId, D0F0xE4_CORE_0103_ADDRESS),
      D0F0xE4_CORE_0103_Togglestrap_MASK,
      (0 << D0F0xE4_CORE_0103_Togglestrap_OFFSET),
      TRUE,
      Pcie
      );
  }

  for (Pif = 0; Pif < Wrapper->NumberOfPIFs; Pif++) {
    // Step 3:  program  TXPWR_IN_INIT bit of PCIE_WRAPPER:PSX80/81_PIF0_TX_CTRL  to  0x1 ( from h/w default 0x2 )
    //          program  RXPWR_IN_INIT bit of PCIE_WRAPPER:PSX80/81_PIF0_RX_CTRL to 0x1 ( from h/w default 0x2 )
    PcieRegisterRMW (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0008_ADDRESS),
      D0F0xE4_PIF_0008_TxpwrInInit_MASK,
      (1 << D0F0xE4_PIF_0008_TxpwrInInit_OFFSET),
      TRUE,
      Pcie
      );
    PcieRegisterRMW (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_000A_ADDRESS),
      D0F0xE4_PIF_000A_RxpwrInInit_MASK,
      (1 << D0F0xE4_PIF_000A_RxpwrInInit_OFFSET),
      TRUE,
      Pcie
      );
    // Wait for ~1ns
    GnbLibStall (1, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
    //Step 5:  program TXPWR_IN_INIT bit of PCIE_WRAPPER:PSX80/81_PIF0_TX_CTRL back to 0x2
    //         program RXPWR_IN_INIT bit of PCIE_WRAPPER:PSX80/81_PIF0_RX_CTRL back to 0x2
    PcieRegisterRMW (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_0008_ADDRESS),
      D0F0xE4_PIF_0008_TxpwrInInit_MASK,
      (2 << D0F0xE4_PIF_0008_TxpwrInInit_OFFSET),
      TRUE,
      Pcie
      );
    PcieRegisterRMW (
      Wrapper,
      PIF_SPACE (Wrapper->WrapId, Pif, D0F0xE4_PIF_000A_ADDRESS),
      D0F0xE4_PIF_000A_RxpwrInInit_MASK,
      (2 << D0F0xE4_PIF_000A_RxpwrInInit_OFFSET),
      TRUE,
      Pcie
      );
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyWrapperTxPresetLoadingSequenceCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Per wrapper Pcie Init prior training.
 *
 *
 * @param[in]  Wrapper         Pointer to wrapper configuration descriptor
 * @param[in]  Buffer          Pointer buffer
 * @param[in]  Pcie            Pointer to global PCIe configuration
 */
AGESA_STATUS
STATIC
PcieEarlyInitCallbackCZ (
  IN       PCIe_WRAPPER_CONFIG           *Wrapper,
  IN OUT   VOID                          *Buffer,
  IN       PCIe_PLATFORM_CONFIG          *Pcie
  )
{
  AGESA_STATUS  Status;
  BOOLEAN       CoreConfigChanged;
  BOOLEAN       PllConfigChanged;
  BOOLEAN       AriSupportEnable;
  GNB_BUILD_OPTIONS_CZ      *GnbBuildOptionData;

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCallbackCZ Enter\n");
  CoreConfigChanged = FALSE;
  PllConfigChanged = FALSE;

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, GnbLibGetHeader (Pcie));
  ASSERT (GnbBuildOptionData != NULL);

  AriSupportEnable = GnbBuildOptionData->CfgAriSupport;
  if (AriSupportEnable == TRUE) {
    // Enable Alternative Routing-ID Interpretation
    GnbLibPciIndirectRMW (
      MAKE_SBDFO (0,0,0,0, D0F0x60_ADDRESS),
      D0F0x64_x46_ADDRESS,
      AccessWidth32,
      (UINT32)~D0F0x64_x46_IocAriSupported_MASK,
      (1 << D0F0x64_x46_IocAriSupported_OFFSET),
      GnbLibGetHeader (Pcie)
       );

    PcieRegisterRMW (
      Wrapper,
      WRAP_SPACE (Wrapper->WrapId, D0F0xE4_WRAP_0000_ADDRESS),
      D0F0xE4_WRAP_0000_StrapBif2AriEn_MASK,
      (1 << D0F0xE4_WRAP_0000_StrapBif2AriEn_OFFSET),
      TRUE,
      Pcie
      );
  }

  IDS_OPTION_HOOK (IDS_BEFORE_RECONFIGURATION, Pcie, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);
  PcieTopologyPrepareForReconfigCZ (Wrapper, Pcie); //step 2
  Status = PcieTopologySetCoreConfigCZ (Wrapper, &CoreConfigChanged, Pcie); //step 3
  ASSERT (Status == AGESA_SUCCESS);
  PcieTopologyApplyLaneMuxCZ (Wrapper, Pcie); //step 4
//  PciePifSetRxDetectPowerMode (Wrapper, Pcie);
//  PciePifSetLs2ExitTime (Wrapper, Pcie);
//  PciePifApplyGanging  (Wrapper, Pcie);
//  PciePhyApplyGangingCZ (Wrapper, Pcie);
  PcieTopologySelectMasterPllCZ (Wrapper, &PllConfigChanged, Pcie); //step 5
  if (CoreConfigChanged) {
    PcieTopologyExecuteReconfigCZ (Wrapper, Pcie); // step 6
  }
  PcieEarlyWrapperTxPresetLoadingSequenceCZ (Wrapper, Pcie);
  PcieTopologyCleanUpReconfigCZ (Wrapper, Pcie); // step 7
  PcieTopologySetLinkReversalV4 (Wrapper, Pcie); // step 8
//  PciePifPllConfigureCZ (Wrapper, Pcie);
  PcieTopologyLaneControlCZ (
    DisableLanes,
    PcieUtilGetWrapperLaneBitMap (LANE_TYPE_CORE_ALL, LANE_TYPE_PCIE_CORE_ALLOC, Wrapper),
    Wrapper,
    Pcie
    ); //step 9
//  PciePollPifForCompeletion (Wrapper, Pcie);
//  PciePhyAvertClockPickersCZ (Wrapper, Pcie);
  PcieEarlyCoreInitCZ (Wrapper, Pcie);
  PcieSetSsidCZ (UserOptions.CfgGnbPcieSSID, Wrapper, Pcie);
  PcieHwInitPowerGatingCZ (Wrapper, Pcie);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCallbackCZ Exit [%x]\n", Status);
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Pcie Init
 *
 *
 *
 * @param[in]  Pcie                Pointer to global PCIe configuration
 * @retval     AGESA_SUCCESS       Topology successfully mapped
 * @retval     AGESA_ERROR         Topology can not be mapped
 */

AGESA_STATUS
STATIC
PcieEarlyInitCZ (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS        Status;
  AGESA_STATUS        AgesaStatus;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCZ Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  Status = PcieConfigRunProcForAllDescriptors (DESCRIPTOR_SILICON, 0, DESCRIPTOR_TERMINATE_TOPOLOGY, PciePortMapInitCallbackCZ, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PciePhyLaneInitInitCallbackCZ, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = PcieConfigRunProcForAllWrappers (DESCRIPTOR_ALL_WRAPPERS, PcieEarlyInitCallbackCZ, NULL, Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInitCZ Exit [%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init hotplug port
 *
 *
 *
 * @param[in]  Engine          Pointer to engine config descriptor
 * @param[in]  Pcie            Pointer to global PCIe configuration
 *
 */

VOID
STATIC
PcieLinkInitHotplugCZ (
  IN      PCIe_ENGINE_CONFIG    *Engine,
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  DxFxxE4_xB5_STRUCT  DxFxxE4_xB5;
  UINT32              Value;
  if ((Engine->Type.Port.PortData.LinkHotplug == HotplugEnhanced) || (Engine->Type.Port.PortData.LinkHotplug == HotplugInboard)) {
    DxFxxE4_xB5.Value = PciePortRegisterRead (Engine, DxFxxE4_xB5_ADDRESS, Pcie);
    DxFxxE4_xB5.Field.LcEhpRxPhyCmd = 0x3;
    DxFxxE4_xB5.Field.LcEhpTxPhyCmd = 0x3;
    DxFxxE4_xB5.Field.LcEnhancedHotPlugEn = 0x1;
    DxFxxE4_xB5.Field.LcRcvrDetEnOverride = 0;
    PciePortRegisterWrite (
      Engine,
      DxFxxE4_xB5_ADDRESS,
      DxFxxE4_xB5.Value,
      TRUE,
      Pcie
      );
    PcieRegisterWriteField (
      PcieConfigGetParentWrapper (Engine),
      CORE_SPACE (Engine->Type.Port.CoreId, D0F0xE4_CORE_0010_ADDRESS),
      D0F0xE4_CORE_0010_LcHotPlugDelSel_OFFSET,
      D0F0xE4_CORE_0010_LcHotPlugDelSel_WIDTH,
      0x5,
      TRUE,
      Pcie
      );
    PcieRegisterWriteField (
      PcieConfigGetParentWrapper (Engine),
      CORE_SPACE (Engine->Type.Port.CoreId, D0F0xE4_CORE_0118_ADDRESS),
      D0F0xE4_CORE_0118_RCVR_DET_CLK_ENABLE_OFFSET,
      D0F0xE4_CORE_0118_RCVR_DET_CLK_ENABLE_WIDTH,
      0x1,
      TRUE,
      Pcie
      );
  }
  if (Engine->Type.Port.PortData.LinkHotplug != HotplugDisabled) {
    GnbLibPciRMW (
      Engine->Type.Port.Address.AddressValue | DxFxx6C_ADDRESS,
      AccessS3SaveWidth32,
      0xffffffff,
      1 << DxFxx6C_HotplugCapable_OFFSET,
      GnbLibGetHeader (Pcie)
      );
    PciePortRegisterWriteField (
      Engine,
      DxFxxE4_x20_ADDRESS,
      DxFxxE4_x20_TxFlushTlpDis_OFFSET,
      DxFxxE4_x20_TxFlushTlpDis_WIDTH,
      0x0,
      TRUE,
      Pcie
      );
    PciePortRegisterWriteField (
      Engine,
      DxFxxE4_x70_ADDRESS,
      DxFxxE4_x70_RxRcbCplTimeoutMode_OFFSET,
      DxFxxE4_x70_RxRcbCplTimeoutMode_WIDTH,
      0x1,
      FALSE,
      Pcie
      );
  }
  if (Engine->Type.Port.PortData.LinkHotplug == HotplugEnhanced) {
    Value = 1;
  } else {
    Value = 0;
  }
  PciePortRegisterWriteField (
    Engine,
    DxFxxE4_x10_ADDRESS,
    DxFxxE4_x10_NativePmeEn_OFFSET,
    DxFxxE4_x10_NativePmeEn_WIDTH,
    Value,
    TRUE,
    Pcie
    );

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
PcieEarlyPortInitCallbackCZ (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyPortInitCallbackCZ Enter\n");
  ASSERT (Engine->EngineData.EngineType == PciePortEngine);
  PciePortProgramRegisterTable (PortInitEarlyTableCZ.Table, PortInitEarlyTableCZ.Length, Engine, FALSE, Pcie);

  IDS_OPTION_HOOK (IDS_GNB_EQUAL_PRESET, Engine, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);

  PcieSetLinkSpeedCapV4 (PcieGen1, Engine, Pcie);
  PcieSetLinkWidthCap (Engine, Pcie);
  PcieCompletionTimeout (Engine, Pcie);
  PcieLinkSetSlotCap (Engine, Pcie);
  PcieLinkInitHotplugCZ (Engine, Pcie);

  //Move EXTENDED_FMT_SUPPORTED stting from Mid to here.
  PciePortRegisterRMW (
    Engine,
    DxFxxE4_xC1_ADDRESS,
    DxFxxE4_xC1_StrapE2EPrefixEn_MASK | DxFxxE4_xC1_StrapExtendedFmtSupported_MASK,
    (1 << DxFxxE4_xC1_StrapE2EPrefixEn_OFFSET) | (1 << DxFxxE4_xC1_StrapExtendedFmtSupported_OFFSET),
    FALSE,
    Pcie
    );

  if (Engine->Type.Port.PortData.PortPresent == PortDisabled ||
      (Engine->Type.Port.PortData.EndpointStatus == EndpointNotPresent &&
       Engine->Type.Port.PortData.LinkHotplug != HotplugEnhanced &&
       Engine->Type.Port.PortData.LinkHotplug != HotplugServer)) {
    ASSERT (!PcieConfigIsSbPcieEngine (Engine));
    //
    //  Pass endpoint status in scratch
    //
    PciePortRegisterRMW (
      Engine,
      DxFxxE4_x01_ADDRESS,
      0x1,
      0x1,
      FALSE,
      Pcie
      );
    PcieTrainingSetPortStateV2 (Engine, LinkStateDeviceNotPresent, FALSE, Pcie);
  }

  if (PcieConfigIsSbPcieEngine (Engine)) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateTrainingSuccess, FALSE, Pcie);
    PcieRegisterWriteField (
      PcieConfigGetParentWrapper (Engine),
      CORE_SPACE (Engine->Type.Port.CoreId, D0F0xE4_CORE_001C_ADDRESS),
      D0F0xE4_CORE_001C_TX_ATOMIC_OPS_DISABLE_OFFSET,
      D0F0xE4_CORE_001C_TX_ATOMIC_OPS_DISABLE_WIDTH,
      0x1,
      TRUE,
      Pcie
      );
  }
  if (Engine->Type.Port.PortData.MiscControls.LinkComplianceMode == 0x1) {
    PcieTrainingSetPortStateV2 (Engine, LinkStateTrainingCompleted, FALSE, Pcie);
  }
  IDS_OPTION_HOOK (IDS_GNB_GEN1_LOOPBACK, Engine, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyPortInitCallbackCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Master procedure to init various features on all active ports
 *
 *
 *
 *
 * @param[in]   Pcie            Pointer to global PCIe configuration
 * @retval      AGESA_STATUS
 *
 */

AGESA_STATUS
STATIC
PcieEarlyPortInitCZ (
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  AGESA_STATUS        Status;

  Status = AGESA_SUCCESS;
  // Leave all device in Presence Detect Presence state for distributed training will be completed at PciePortPostEarlyInit
  if (Pcie->TrainingAlgorithm == PcieTrainingDistributed) {
    Pcie->TrainingExitState = LinkStateResetExit;
  }

  PcieConfigRunProcForAllEngines (
    DESCRIPTOR_ALLOCATED | DESCRIPTOR_PCIE_ENGINE,
    PcieEarlyPortInitCallbackCZ,
    NULL,
    Pcie
    );

  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PCIE_EARLY_PORT_CONFIG, Pcie, (AMD_CONFIG_PARAMS *)Pcie->StdHeader);

  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * PCIe Early Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
PcieEarlyInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  PCIe_PLATFORM_CONFIG  *Pcie;
  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInterfaceCZ Enter\n");
  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    PciePortsVisibilityControlV5 (UnhidePorts, Pcie);

    Status = PcieEarlyInitCZ (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieEarlyPortInitCZ (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    Status = PcieTrainingV2 (Pcie);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    ASSERT (Status == AGESA_SUCCESS);

    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_PCIE_PHY_CONFIG, Pcie, StdHeader);
    PciePortsVisibilityControlV5 (HidePorts, Pcie);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "PcieEarlyInterfaceCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

