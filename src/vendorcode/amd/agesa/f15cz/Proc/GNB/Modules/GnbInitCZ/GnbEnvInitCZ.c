/* $NoKeywords:$ */
/**
 * @file
 *
 * GNB env post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 314031 $   @e \$Date: 2015-03-03 06:06:56 -0600 (Tue, 03 Mar 2015) $
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
#include  "GnbCommonLib.h"
#include  "GnbTable.h"
#include  "GnbPcieConfig.h"
#include  "GnbNbInitLibV4.h"
#include  "GnbNbInitLibV5.h"
#include  "GnbRegisterAccCZ.h"
#include  "GnbRegistersCZ.h"
#include  "GnbFuseTableCZ.h"
#include  "PcieComplexDataCZ.h"
#include  "PcieLibCZ.h"
#include  "OptionGnb.h"
#include  "heapManager.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_GNBENVINITCZ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
//extern GNB_BUILD_OPTIONS_CZ   GnbBuildOptionsCZ;
extern GNB_TABLE ROMDATA      GnbEnvInitTableCZ [];
extern GNB_TABLE ROMDATA      GnbIommuEnvInitTableCZ [];
/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GnbEnvInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Registers need to be set if no GFX PCIe ports are used
 *
 *
 *
 * @param[in]  Pcie                Pointer to PCIe_PLATFORM_CONFIG
 * @retval     BOOLEAN
 */

BOOLEAN
STATIC
GnbIommuEnvInitCheckGfxPciePorts (
  IN      PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  PCIe_WRAPPER_CONFIG   *WrapperList;
  BOOLEAN               GfxPciePortUsed;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitCheckGfxPciePorts Enter\n");
  GfxPciePortUsed = FALSE;

  WrapperList = PcieConfigGetChildWrapper (Pcie);
  ASSERT (WrapperList != NULL);
  if (WrapperList->WrapId == GFX_WRAP_ID) {
    PCIe_ENGINE_CONFIG   *EngineList;
    EngineList = PcieConfigGetChildEngine (WrapperList);
    while (EngineList != NULL) {
      if (PcieConfigIsPcieEngine (EngineList)) {
        IDS_HDT_CONSOLE (GNB_TRACE, "Checking Gfx ports device number %x\n", EngineList->Type.Port.NativeDevNumber);
        if (PcieConfigCheckPortStatus (EngineList, INIT_STATUS_PCIE_TRAINING_SUCCESS) ||
           ((EngineList->Type.Port.PortData.LinkHotplug != HotplugDisabled) && (EngineList->Type.Port.PortData.LinkHotplug != HotplugInboard))) {
          // GFX PCIe ports are being used
          GfxPciePortUsed = TRUE;
          IDS_HDT_CONSOLE (GNB_TRACE, "GFX PCIe ports are being used\n");
          break;
        }
      }
      EngineList = PcieLibGetNextDescriptor (EngineList);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuMidInitCheckGfxPciePorts Exit\n");
  return GfxPciePortUsed;
}

/*----------------------------------------------------------------------------------------*/
/**
 * IOMMU Memory power gating
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
STATIC
GnbIommuEnvMemoryPowerGating (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              Status;
  GNB_HANDLE                *GnbHandle;
  GNB_BUILD_OPTIONS_CZ      *GnbBuildOptionData;
  D0F2xFC_x3A_L1_STRUCT     D0F2xFC_x3A_L1;
  D0F2xFC_x3B_L1_STRUCT     D0F2xFC_x3B_L1;
  D0F2xFC_x3C_L1_STRUCT     D0F2xFC_x3C_L1;
  D0F2xFC_x39_L1_STRUCT     D0F2xFC_x39_L1;
  UINT8                     i;
  D0F2xF4_x3B_STRUCT        D0F2xF4_x3B;
  D0F2xF4_x3C_STRUCT        D0F2xF4_x3C;
  D0F2xF4_x3D_STRUCT        D0F2xF4_x3D;
  D0F2xF4_x35_STRUCT        D0F2xF4_x35;
  D0F2xF4_x36_STRUCT        D0F2xF4_x36;
  D0F2xF4_x37_STRUCT        D0F2xF4_x37;
  D0F2xF4_x95_STRUCT        D0F2xF4_x95;
  D0F2xF4_x96_STRUCT        D0F2xF4_x96;
  D0F2xF4_x97_STRUCT        D0F2xF4_x97;


  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuEnvMemoryPowerGating Enter\n");
  Status = AGESA_SUCCESS;

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  if (GnbBuildOptionData->CfgIommuL2MemPowerGating) {
    //Disable PGFSM Memory Power Gating in IOMMU L2
    D0F2xF4_x3B.Value = 0x100020FF;
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x3B_TYPE, D0F2xF4_x3B_ADDRESS, &D0F2xF4_x3B.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x3D_TYPE, D0F2xF4_x3D_ADDRESS, &D0F2xF4_x3D.Value, 0, StdHeader);
    D0F2xF4_x3C.Value = (D0F2xF4_x3D.Field.ReadValue | 0x00080000);
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x3C_TYPE, D0F2xF4_x3C_ADDRESS, &D0F2xF4_x3C.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

    D0F2xF4_x3B.Value = 0x100010FF;
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x3B_TYPE, D0F2xF4_x3B_ADDRESS, &D0F2xF4_x3B.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);

    //Enabling PGMEM Memory Power Gating in IOMMU L2
    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x36_TYPE, D0F2xF4_x36_ADDRESS, &D0F2xF4_x36.Value, 0, StdHeader);
    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x37_TYPE, D0F2xF4_x37_ADDRESS, &D0F2xF4_x37.Value, 0, StdHeader);
    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x96_TYPE, D0F2xF4_x96_ADDRESS, &D0F2xF4_x96.Value, 0, StdHeader);
    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x97_TYPE, D0F2xF4_x97_ADDRESS, &D0F2xF4_x97.Value, 0, StdHeader);
    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x35_TYPE, D0F2xF4_x35_ADDRESS, &D0F2xF4_x35.Value, 0, StdHeader);
    GnbRegisterReadCZ (GnbHandle, D0F2xF4_x95_TYPE, D0F2xF4_x95_ADDRESS, &D0F2xF4_x95.Value, 0, StdHeader);

    D0F2xF4_x36.Field.L2aregLSThres = 0xC8;
    D0F2xF4_x37.Field.L2aregDSThres = 0xC8;
    D0F2xF4_x96.Field.L2bregLSThres = 0xC8;
    D0F2xF4_x97.Field.L2bregDSThres = 0xC8;
    D0F2xF4_x35.Field.L2aregLSEn = 1;
    D0F2xF4_x35.Field.L2aregDSEn = 1;
    D0F2xF4_x35.Field.L2aregSDEn = 1;
    D0F2xF4_x95.Field.L2bregLSEn = 1;
    D0F2xF4_x95.Field.L2bregDSEn = 1;
    D0F2xF4_x95.Field.L2bregSDEn = 1;

    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x36_TYPE, D0F2xF4_x36_ADDRESS, &D0F2xF4_x36.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x37_TYPE, D0F2xF4_x37_ADDRESS, &D0F2xF4_x37.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x96_TYPE, D0F2xF4_x96_ADDRESS, &D0F2xF4_x96.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x97_TYPE, D0F2xF4_x97_ADDRESS, &D0F2xF4_x97.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x35_TYPE, D0F2xF4_x35_ADDRESS, &D0F2xF4_x35.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    GnbRegisterWriteCZ (GnbHandle, D0F2xF4_x95_TYPE, D0F2xF4_x95_ADDRESS, &D0F2xF4_x95.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
  }



  if (GnbBuildOptionData->CfgIommuL1MemPowerGating) {
    for (i = 0; i < 4; i++) {
      GnbRegisterReadCZ (GnbHandle, D0F2xFC_x3A_L1_TYPE, D0F2xFC_x3A_L1_ADDRESS (i), &D0F2xFC_x3A_L1.Value, 0, StdHeader);
      GnbRegisterReadCZ (GnbHandle, D0F2xFC_x3B_L1_TYPE, D0F2xFC_x3B_L1_ADDRESS (i), &D0F2xFC_x3B_L1.Value, 0, StdHeader);
      GnbRegisterReadCZ (GnbHandle, D0F2xFC_x3C_L1_TYPE, D0F2xFC_x3C_L1_ADDRESS (i), &D0F2xFC_x3C_L1.Value, 0, StdHeader);
      GnbRegisterReadCZ (GnbHandle, D0F2xFC_x39_L1_TYPE, D0F2xFC_x39_L1_ADDRESS (i), &D0F2xFC_x39_L1.Value, 0, StdHeader);

      D0F2xFC_x3A_L1.Field.LSThres = 0xC8;
      D0F2xFC_x3B_L1.Field.DSThres = 0xC8;
      D0F2xFC_x3C_L1.Field.SDThres = 0x44C;
      D0F2xFC_x39_L1.Field.LSEn = 1;
      D0F2xFC_x39_L1.Field.DSEn = 1;
      D0F2xFC_x39_L1.Field.SDEn = 1;

      GnbRegisterWriteCZ (GnbHandle, D0F2xFC_x3A_L1_TYPE, D0F2xFC_x3A_L1_ADDRESS (i), &D0F2xFC_x3A_L1.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
      GnbRegisterWriteCZ (GnbHandle, D0F2xFC_x3B_L1_TYPE, D0F2xFC_x3B_L1_ADDRESS (i), &D0F2xFC_x3B_L1.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
      GnbRegisterWriteCZ (GnbHandle, D0F2xFC_x3C_L1_TYPE, D0F2xFC_x3C_L1_ADDRESS (i), &D0F2xFC_x3C_L1.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
      GnbRegisterWriteCZ (GnbHandle, D0F2xFC_x39_L1_TYPE, D0F2xFC_x39_L1_ADDRESS (i), &D0F2xFC_x39_L1.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
    }
  }



  IDS_HDT_CONSOLE (GNB_TRACE, "GnbIommuEnvMemoryPowerGating Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB init at env
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbEnvInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS          Status;
  AGESA_STATUS          AgesaStatus;
  AMD_ENV_PARAMS        *EnvParamsPtr;
  UINT32                Property;
  GNB_HANDLE            *GnbHandle;
  PCI_ADDR              GnbPciAddress;
//  D18F5x170_STRUCT      D18F5x170;
  D0F0x7C_STRUCT        D0F0x7C;
  PCIe_PLATFORM_CONFIG  *Pcie;
  GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnvInterfaceCZ Enter\n");
  Property = TABLE_PROPERTY_DEFAULT;
  AgesaStatus = AGESA_SUCCESS;
  EnvParamsPtr = (AMD_ENV_PARAMS *) StdHeader;

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  GnbLoadFuseTableCZ (StdHeader);

  Property |= GnbBuildOptionData->GnbCommonOptions.CfgIommuL1ClockGatingEnable ? TABLE_PROPERTY_IOMMU_L1_CLOCK_GATING : 0;
  Property |= GnbBuildOptionData->GnbCommonOptions.CfgIommuL2ClockGatingEnable ? TABLE_PROPERTY_IOMMU_L2_CLOCK_GATING : 0;
  Property |= GnbBuildOptionData->CfgSstunlClkGating ? TABLE_PROPERTY_IOMMU_L1_SST_CLOCK_GATING : 0;
  if (!EnvParamsPtr->GnbEnvConfiguration.IommuSupport) {
    Property |= TABLE_PROPERTY_IOMMU_DISABLED;
  }

  GnbLibPciRead (MAKE_SBDFO (0, 0, 0, 0, D0F0x7C_ADDRESS), AccessWidth32, &D0F0x7C.Value, StdHeader);
  Property |= (D0F0x7C.Field.ForceIntGfxDisable != 0) ? TABLE_PROPERTY_IGFX_DISABLED : 0;

  Status = PcieLocateConfigurationData (StdHeader, &Pcie);
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  if (Status == AGESA_SUCCESS) {
    Property |= GnbIommuEnvInitCheckGfxPciePorts (Pcie) ? 0 : TABLE_PROPERTY_DGFX_DISABLED;
  }
  GnbHandle = GnbGetHandle (StdHeader);
  GnbPciAddress = GnbGetHostPciAddress (GnbHandle);
  Status = GnbSetTomV5 (GnbPciAddress, StdHeader);
  GnbOrbDynamicWakeV5 (GnbPciAddress, StdHeader);
  if (GnbIsGnbConnectedToSb (GnbHandle)) {
    GnbLpcDmaDeadlockPreventionV5 (GnbHandle, StdHeader);
  }

  Status = GnbIommuEnvMemoryPowerGating (StdHeader);

//  if (GnbBuildOptionData->CfgNbdpmEnable) {
//    Property &= ~TABLE_PROPERTY_NBDPM;
//    GnbRegisterReadCZ (
//      GnbHandle,
//      TYPE_D18F5,
//      D18F5x170_ADDRESS,
//      &D18F5x170.Value,
//      0,
//      StdHeader
//    );
//    // Check if NbPstate enable
//    if ((D18F5x170.Field.SwNbPstateLoDis != 1) && (D18F5x170.Field.NbPstateMaxVal != 0)) {
//      Property |= TABLE_PROPERTY_NBDPM;
//    }
//  }
////  IDS_OPTION_HOOK (IDS_GNB_PROPERTY, &Property, StdHeader);

  Status = GnbProcessTable (
             GnbHandle,
             GnbEnvInitTableCZ,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  Status = GnbProcessTable (
             GnbHandle,
             GnbIommuEnvInitTableCZ,
             Property,
             GNB_TABLE_FLAGS_FORCE_S3_SAVE,
             StdHeader
             );
  AGESA_STATUS_UPDATE (Status, AgesaStatus);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbEnvInterfaceCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
