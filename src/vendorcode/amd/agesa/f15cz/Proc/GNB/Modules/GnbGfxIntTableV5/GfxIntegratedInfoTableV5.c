/* $NoKeywords:$ */
/**
 * @file
 *
 * Integrated table info init
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#include  "heapManager.h"
#include  "GeneralServices.h"
#include  "Gnb.h"
#include  "GnbFuseTable.h"
#include  "GnbPcie.h"
#include  "GnbGfx.h"
#include  "GnbSbLib.h"
#include  "GnbCommonLib.h"
#include  "GnbPcieConfig.h"
#include  "GnbGfxConfig.h"
#include  "GnbGfxFamServices.h"
#include  "GnbNbInitLibV5.h"
#include  "GfxConfigLib.h"
#include  "GfxIntegratedInfoTableV5.h"
#include  "GfxPwrPlayTableV5.h"
#include  "OptionGnb.h"
#include  "GfxLibV5.h"
#include  "GnbGfxInitLibV1.h"
#include  "GfxEnumConnectors.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBGFXINTTABLEV5_GFXINTEGRATEDINFOTABLEV5_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern GNB_BUILD_OPTIONS  GnbBuildOptions;

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
 * Init V5 Support for eDP to Lvds translators
 *
 *
 * @param[in]     Engine  Engine configuration info
 * @param[in,out] Buffer  Buffer pointer
 * @param[in]     Pcie    PCIe configuration info
 */
VOID
STATIC
GfxIntegrateducEDPToLVDSRxIdCallback (
  IN       PCIe_ENGINE_CONFIG    *Engine,
  IN OUT   VOID                  *Buffer,
  IN       PCIe_PLATFORM_CONFIG  *Pcie
  )
{
  UINT8    *uceDPToLVDSRxId;
  uceDPToLVDSRxId = (UINT8*) Buffer;
  // APU output DP signal to a 3rd party DP translator chip (Analogix, Parade etc),
  // the chip is handled by the 3rd party DP Rx firmware and it does not require the AMD SW to have a special
  // initialize/enable/disable sequence to control this chip, the AMD SW just follows the eDP spec
  // to enable the LVDS panel through this chip.

  if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDPToLvds) {
    *uceDPToLVDSRxId = eDP_TO_LVDS_COMMON_ID;
    IDS_HDT_CONSOLE (GNB_TRACE, "Found 3rd party common EDPToLvds Connector\n");
  }
  // APU output DP signal to a 3rd party DP translator chip which requires a AMD SW one time initialization
  // to the chip based on the LVDS panel parameters ( such as power sequence time and panel SS parameter etc ).
  // After that, the AMD SW does not need any specific enable/disable sequences to control this chip and just
  // follows the eDP spec. to control the panel.
  if (Engine->Type.Ddi.DdiData.ConnectorType == ConnectorTypeEDPToLvdsSwInit) {
    *uceDPToLVDSRxId = eDP_TO_LVDS_SWINIT_ID;
    IDS_HDT_CONSOLE (GNB_TRACE, "Found EDPToLvds Connector requiring SW init\n");
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Calculate V5 NCLK clock
 *
 *
 *
 * @param[in] NbFid         NbFid
 * @param[in] NbDid         NbDid
 * @retval                  Clock in 10KHz
 */

UINT32
STATIC
GfxLibGetNclkV5 (
  IN      UINT8                   NbFid,
  IN      UINT8                   NbDid
  )
{
  UINT32  Divider;
  //i.e. NBCOF[0] = (100 * ([NbFid] + 4h) / (2^[NbDid])) Mhz
  if (NbDid == 1) {
    Divider = 2;
  } else if (NbDid == 0) {
    Divider = 1;
  } else {
    Divider = 1;
  }
  ASSERT (NbDid == 0 || NbDid == 1);
  return ((10000 * (NbFid + 4)) / Divider);
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init V5 Nb p-State MemclkFreq
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] PpFuseArray         PP_FUSE_ARRAY_V2 pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxFillNbPstateMemclkFreqV5 (
  IN OUT   ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT8                           i;
  UINT8                           Channel;
  ULONG                           memps0_freq;
  ULONG                           memps1_freq;

  Channel = 0;
  if ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT1) != 0) {
    Channel = 1;
  } else if ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT2) != 0) {
    Channel = 2;
  } else if ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT3) != 0) {
    Channel = 3;
  }


  memps0_freq = 100 * GfxLibExtractDramFrequencyV5 ((UINT8) PpFuseArray->MemClkFreq[Channel], GnbLibGetHeader (Gfx));
  memps1_freq = 100 * GfxLibExtractDramFrequencyV5 ((UINT8) PpFuseArray->M1MemClkFreq[Channel], GnbLibGetHeader (Gfx));

  for (i = 0; i < 2; i++) {
    if (PpFuseArray->NbPstateEn[i] == 1) {
      IntegratedInfoTable->ulNbpStateMemclkFreq[i] = (PpFuseArray->MemPstate[i] == 0) ? memps0_freq : memps1_freq;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init V5 NbPstateVid
 *
 *
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxFillNbPStateVidV5 (
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN OUT   ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT8                   i;

  for (i = 0; i < 4; i++) {
    IntegratedInfoTable->usNBPStateVoltage[i] = (USHORT) ((PpFuseArray->NbVid_7[i] << 7) | (PpFuseArray->NbVid_6_0[i]));
    IntegratedInfoTable->ulNbpStateNClkFreq[i] = GfxLibGetNclkV5 ((UINT8) (PpFuseArray->NbFid[i]), (UINT8) (PpFuseArray->NbDid[i]));
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Copy memory content to FB
 *
 *
 * @param[in] SystemInfoTableV5Ptr    Pointer to integrated info table
 * @param[in] Gfx                     Pointer to global GFX configuration
 *
 */
VOID
GfxIntInfoTablePostToFbV5 (
  IN       ATOM_FUSION_SYSTEM_INFO_V5     *SystemInfoTableV5Ptr,
  IN       GFX_PLATFORM_CONFIG            *Gfx
  )
{
  UINT32      Index;
  UINT32      TableOffset;
  UINT32      FbAddress;
  TableOffset = (UINT32) (Gfx->UmaInfo.UmaSize - sizeof (ATOM_FUSION_SYSTEM_INFO_V5)) | 0x80000000;
  for (Index = 0; Index < sizeof (ATOM_FUSION_SYSTEM_INFO_V5); Index = Index + 4 ) {
    FbAddress = TableOffset + Index;
    GnbLibMemWrite (Gfx->GmmBase + GMMx00_ADDRESS, AccessWidth32, &FbAddress, GnbLibGetHeader (Gfx));
    GnbLibMemWrite (Gfx->GmmBase + GMMx04_ADDRESS, AccessWidth32, (UINT8*) SystemInfoTableV5Ptr + Index, GnbLibGetHeader (Gfx));
  }
}


/*----------------------------------------------------------------------------------------*/
/**
 *Init Dispclk <-> VID table
 *
 *
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxIntInfoTableInitDispclkTableV5 (
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINTN   Index;
  for (Index = 0; Index < 4; Index++) {
    if (PpFuseArray->DispClkDid[Index] != 0) {
      IntegratedInfoTable->sDISPCLK_Voltage[Index].ulMaximumSupportedCLK = GfxFmCalculateClock (
                                                                             PpFuseArray->DispClkDid[Index],
                                                                             GnbLibGetHeader (Gfx)
                                                                             );
      IntegratedInfoTable->sDISPCLK_Voltage[Index].ulVoltageIndex = (ULONG) Index;
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init Dispclk <-> VID table
 *
 *
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxIntInfoTableInitDispclkV2TableV5 (
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINTN   Index;
  for (Index = 0; Index < 8; Index++) {
    if (PpFuseArray->DispClkDid[Index] != 0) {
      IntegratedInfoTable->sDispClkVoltageMapping[Index].ulMaximumSupportedCLK = GfxFmCalculateClock (
                                                                             PpFuseArray->DispClkDid[Index],
                                                                             GnbLibGetHeader (Gfx)
                                                                             );
      IntegratedInfoTable->sDispClkVoltageMapping[Index].usVoltageLevel = (USHORT) (GnbTranslateVidCodeToMillivoltV5 (PpFuseArray->VddNbVid[Index], GnbLibGetHeader (Gfx)) / 100);
    }
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init Sclk <-> VID table
 *
 *
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxIntInfoTableInitSclkTableV5 (
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT8                       Index;
  UINTN                       AvailSclkIndex;
  ATOM_AVAILABLE_SCLK_LIST    *AvailSclkList;
  BOOLEAN                     Sorting;
  AvailSclkList = &IntegratedInfoTable->sAvail_SCLK[0];

  AvailSclkIndex = 0;
  for (Index = 0; Index < MAX_NUM_OF_FUSED_DPM_STATES; Index++) {
    if (PpFuseArray->SclkDid[Index] != 0) {
      AvailSclkList[AvailSclkIndex].ulSupportedSCLK = GfxFmCalculateClock (PpFuseArray->SclkDid[Index], GnbLibGetHeader (Gfx));
      AvailSclkList[AvailSclkIndex].usVoltageIndex = Index;
      AvailSclkList[AvailSclkIndex].usVoltageID = PpFuseArray->VddNbVid[Index];
      AvailSclkIndex++;
    }
  }
  //Sort by VoltageIndex & ulSupportedSCLK
  if (AvailSclkIndex > 1) {
    do {
      Sorting = FALSE;
      for (Index = 0; Index < (AvailSclkIndex - 1); Index++) {
        ATOM_AVAILABLE_SCLK_LIST  Temp;
        BOOLEAN                   Exchange;
        Exchange = FALSE;
        if (AvailSclkList[Index].usVoltageIndex > AvailSclkList[Index + 1].usVoltageIndex) {
          Exchange = TRUE;
        }
        if ((AvailSclkList[Index].usVoltageIndex == AvailSclkList[Index + 1].usVoltageIndex) &&
            (AvailSclkList[Index].ulSupportedSCLK > AvailSclkList[Index + 1].ulSupportedSCLK)) {
          Exchange = TRUE;
        }
        if (Exchange) {
          Sorting = TRUE;
          LibAmdMemCopy (&Temp, &AvailSclkList[Index], sizeof (ATOM_AVAILABLE_SCLK_LIST), GnbLibGetHeader (Gfx));
          LibAmdMemCopy (&AvailSclkList[Index], &AvailSclkList[Index + 1], sizeof (ATOM_AVAILABLE_SCLK_LIST), GnbLibGetHeader (Gfx));
          LibAmdMemCopy (&AvailSclkList[Index + 1], &Temp, sizeof (ATOM_AVAILABLE_SCLK_LIST), GnbLibGetHeader (Gfx));
        }
      }
    } while (Sorting);
  }
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *
 *
 *
 * @param[in] Gfx                 Gfx configuration info
 * @param[in] SystemInfoTableV5   ATOM_FUSION_SYSTEM_INFO_V5 pointer
 * @param[in] PpFuseArray         PP_FUSE_ARRAY pointer
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
GfxIntInfoTableInitV5 (
  IN      GFX_PLATFORM_CONFIG         *Gfx,
  IN      ATOM_FUSION_SYSTEM_INFO_V5  *SystemInfoTableV5,
  IN      PP_FUSE_ARRAY_V2             *PpFuseArray
  )
{
  AGESA_STATUS                    Status;
  AGESA_STATUS                    AgesaStatus;
  PCIe_PLATFORM_CONFIG            *Pcie;
  UINT8                           Channel;
  UINT8                           Index;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInitV5 Enter\n");

  if (PpFuseArray != NULL) {

    Channel = 0;
    if ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT1) != 0) {
      Channel = 1;
    } else if ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT2) != 0) {
      Channel = 2;
    } else if ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_ON_DCT3) != 0) {
      Channel = 3;
    }
    SystemInfoTableV5->sIntegratedSysInfo.sHeader.usStructureSize = sizeof (ATOM_INTEGRATED_SYSTEM_INFO_V1_9);
    ASSERT (SystemInfoTableV5->sIntegratedSysInfo.sHeader.usStructureSize == 512);
    SystemInfoTableV5->sIntegratedSysInfo.sHeader.ucTableFormatRevision = 1;
    SystemInfoTableV5->sIntegratedSysInfo.sHeader.ucTableContentRevision = 9;
    SystemInfoTableV5->sIntegratedSysInfo.ulBootUpEngineClock = 200 * 100;    //Set default engine clock to 200MhZ
    SystemInfoTableV5->sIntegratedSysInfo.ulDentistVCOFreq = (PpFuseArray->MainPllOpFreqIdStartup + 0x10) * 10000;
    SystemInfoTableV5->sIntegratedSysInfo.ulBootUpUMAClock = Gfx->UmaInfo.MemClock * 100;

    SystemInfoTableV5->sIntegratedSysInfo.usRequestedPWMFreqInHz = Gfx->LcdBackLightControl;
//    SystemInfoTableV5->sIntegratedSysInfo.ucUMAChannelNumber = ((Gfx->UmaInfo.UmaAttributes & UMA_ATTRIBUTE_INTERLEAVE) == 0) ? 1 : 2;
    SystemInfoTableV5->sIntegratedSysInfo.ucMemoryType = Gfx->UmaInfo.MemType;
    SystemInfoTableV5->sIntegratedSysInfo.usBootUpNBVoltage = GnbLocateHighestVidIndexV5 (GnbLibGetHeader (Gfx));
    SystemInfoTableV5->sIntegratedSysInfo.usPanelRefreshRateRange = Gfx->DynamicRefreshRate;
    SystemInfoTableV5->sIntegratedSysInfo.usLvdsSSPercentage = Gfx->LvdsSpreadSpectrum;
    //Locate PCIe configuration data to get definitions of display connectors
    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.sHeader.usStructureSize = sizeof (ATOM_EXTERNAL_DISPLAY_CONNECTION_INFO);
    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.sHeader.ucTableFormatRevision = 1;
    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.sHeader.ucTableContentRevision = 1;
    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.uc3DStereoPinId = Gfx->Gnb3dStereoPinIndex;
    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.ucRemoteDisplayConfig = Gfx->GnbRemoteDisplaySupport;
    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.ucFixDPVoltageSwing = (UINT8) Gfx->DpFixedVoltSwingType;
    SystemInfoTableV5->sIntegratedSysInfo.usExtDispConnInfoOffset = offsetof (ATOM_INTEGRATED_SYSTEM_INFO_V1_9, sExtDispConnInfo);

    SystemInfoTableV5->sIntegratedSysInfo.ucEDPv1_4VSMode = (UINT8) Gfx->EDPv1_4VSMode;
    SystemInfoTableV5->sIntegratedSysInfo.ucExtHDMIReDrvSlvAddr = (UINT8) Gfx->ExtHDMIReDrvSlvAddr;
    SystemInfoTableV5->sIntegratedSysInfo.ucExtHDMIReDrvRegNum = (UINT8) Gfx->ExtHDMIReDrvRegNum;

    if ((HDMI_RE_DRIVER_I2C_REG_INFO *)Gfx->ExtHDMIRegSetting != NULL) {
      if (Gfx->ExtHDMIReDrvRegNum > 0) {
        for (Index = 0; Index < Gfx->ExtHDMIReDrvRegNum; Index++) {
          SystemInfoTableV5->sIntegratedSysInfo.asExtHDMIRegSetting[Index].ucI2cRegIndex = (UINT8) ((HDMI_RE_DRIVER_I2C_REG_INFO *)Gfx->ExtHDMIRegSetting)[Index].RegIndex;
          SystemInfoTableV5->sIntegratedSysInfo.asExtHDMIRegSetting[Index].ucI2cRegVal = (UINT8) ((HDMI_RE_DRIVER_I2C_REG_INFO *)Gfx->ExtHDMIRegSetting)[Index].RegVal;
        }
      }
    }

    SystemInfoTableV5->sIntegratedSysInfo.usPCIEClkSSPercentage = Gfx->PcieRefClkSpreadSpectrum;
    IDS_HDT_CONSOLE (GNB_TRACE, "PcieRefClkSpreadSpectrum : %d\n", Gfx->PcieRefClkSpreadSpectrum);
    SystemInfoTableV5->sIntegratedSysInfo.ucLvdsMisc = Gfx->LvdsMiscControl.Value;
    IDS_HDT_CONSOLE (GNB_TRACE, "Lvds Misc control : %x\n", Gfx->LvdsMiscControl.Value);
    if (Gfx->LvdsMiscControl.Field.LvdsVoltOverwriteEn) {
      SystemInfoTableV5->sIntegratedSysInfo.ucLVDSVoltAdjust = Gfx->LVDSVoltAdjust;
      IDS_HDT_CONSOLE (GNB_TRACE, "LVDSVoltAdjust : %x\n", Gfx->LVDSVoltAdjust);
    }

    SystemInfoTableV5->sIntegratedSysInfo.ulVBIOSMisc = Gfx->DisplayMiscControl.Value;
    IDS_HDT_CONSOLE (GNB_TRACE, "Display Misc control : %x\n", Gfx->DisplayMiscControl.Value);

    // LVDS
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSPwrOnSeqDIGONtoDE_in4Ms = Gfx->LvdsPowerOnSeqDigonToDe;
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSPwrOnSeqDEtoVARY_BL_in4Ms = Gfx->LvdsPowerOnSeqDeToVaryBl;
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSPwrOffSeqVARY_BLtoDE_in4Ms = Gfx->LvdsPowerOnSeqVaryBlToDe;
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSPwrOffSeqDEtoDIGON_in4Ms = Gfx->LvdsPowerOnSeqDeToDigon;
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSOffToOnDelay_in4Ms = Gfx->LvdsPowerOnSeqOnToOffDelay;
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSPwrOnSeqVARY_BLtoBLON_in4Ms = Gfx->LvdsPowerOnSeqVaryBlToBlon;
    SystemInfoTableV5->sIntegratedSysInfo.ucLVDSPwrOffSeqBLONtoVARY_BL_in4Ms = Gfx->LvdsPowerOnSeqBlonToVaryBl;
    SystemInfoTableV5->sIntegratedSysInfo.ulLCDBitDepthControlVal = Gfx->LcdBitDepthControlValue;
    SystemInfoTableV5->sIntegratedSysInfo.usMaxLVDSPclkFreqInSingleLink = Gfx->LvdsMaxPixelClockFreq;
    SystemInfoTableV5->sIntegratedSysInfo.ucMinAllowedBL_Level = Gfx->MinAllowedBLLevel;
    Status = PcieLocateConfigurationData (GnbLibGetHeader (Gfx), &Pcie);
    ASSERT (Status == AGESA_SUCCESS);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    if (Status == AGESA_SUCCESS) {
      Status = GfxIntegratedEnumerateAllConnectors (
                 &SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.sPath[0],
                 Pcie,
                 Gfx
                 );
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
    }

    SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.uceDPToLVDSRxId = eDP_TO_LVDS_RX_DISABLE;
    PcieConfigRunProcForAllEngines (
      DESCRIPTOR_ALLOCATED | DESCRIPTOR_VIRTUAL | DESCRIPTOR_DDI_ENGINE,
      GfxIntegrateducEDPToLVDSRxIdCallback,
      &SystemInfoTableV5->sIntegratedSysInfo.sExtDispConnInfo.uceDPToLVDSRxId,
      Pcie
      );

    // Build info from fuses
    // Build Display clock info
    GfxIntInfoTableInitDispclkV2TableV5 (PpFuseArray, &SystemInfoTableV5->sIntegratedSysInfo, Gfx);
    // Build Sclk info table - no longer used
    //GfxIntInfoTableInitSclkTableV5 (PpFuseArray, &SystemInfoTableV5->sIntegratedSysInfo, Gfx);
    // Fill in Nb P-state MemclkFreq Data
    GfxFillNbPstateMemclkFreqV5 (&SystemInfoTableV5->sIntegratedSysInfo, PpFuseArray, Gfx);
    // Fill in HTC Data
    if (PpFuseArray->HtcEn == 1) {
      SystemInfoTableV5->sIntegratedSysInfo.ucHtcTmpLmt = (UCHAR) (PpFuseArray->HtcTmpLmt / 2 + 52);
      SystemInfoTableV5->sIntegratedSysInfo.ucHtcHystLmt = (UCHAR) (PpFuseArray->HtcHystLmt / 2);
    } else {
      SystemInfoTableV5->sIntegratedSysInfo.ucHtcTmpLmt = 0;
      SystemInfoTableV5->sIntegratedSysInfo.ucHtcHystLmt = 0;
    }
    // Fill in NB P states VID & NCLK info
    GfxFillNbPStateVidV5 (PpFuseArray, &SystemInfoTableV5->sIntegratedSysInfo, Gfx);

    // Family specific data update - store default values to be updated by family specific code
    //GfxFmIntegratedInfoTableInit (&SystemInfoV1Table.sIntegratedSysInfo, Gfx);
    SystemInfoTableV5->sIntegratedSysInfo.ulDDR_DLL_PowerUpTime = 4940;
    SystemInfoTableV5->sIntegratedSysInfo.ulDDR_PLL_PowerUpTime = 2000;

    if (PpFuseArray->MemPhyPllPdMode[Channel] != 0) {
      SystemInfoTableV5->sIntegratedSysInfo.ulSystemConfig |= BIT2;
    }
    if (PpFuseArray->DisDllShutdownSR[Channel] == 0) {
      SystemInfoTableV5->sIntegratedSysInfo.ulSystemConfig |= BIT1;
    }
    if (GnbBuildOptions.CfgPciePowerGatingFlags != (PCIE_POWERGATING_SKIP_CORE | PCIE_POWERGATING_SKIP_PHY)) {
      SystemInfoTableV5->sIntegratedSysInfo.ulSystemConfig |= BIT0;
    }
    SystemInfoTableV5->sIntegratedSysInfo.ulGPUCapInfo = GPUCAPINFO_TMDS_HDMI_USE_CASCADE_PLL_MODE | GPUCAPINFO_DP_USE_SINGLE_PLL_MODE;

    IDS_HDT_CONSOLE (GNB_TRACE, "ulSystemConfig : %x\n", SystemInfoTableV5->sIntegratedSysInfo.ulSystemConfig);

  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR! Cannot locate fuse table\n");
    Status = AGESA_ERROR;
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInitV5 Exit [0x%x]\n", Status);
  return  Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Dump gfx integrated info table
 *
 *
 * @param[in] SystemInfoTableV5Ptr    Pointer to integrated info table
 * @param[in] Gfx                     Pointer to global GFX configuration
 *
 */
VOID
GfxIntInfoTableDebugDumpV5 (
  IN       ATOM_FUSION_SYSTEM_INFO_V5     *SystemInfoTableV5Ptr,
  IN       GFX_PLATFORM_CONFIG            *Gfx
  )
{
  ATOM_PPLIB_POWERPLAYTABLE5      *PpTable;
  ATOM_PPLIB_EXTENDEDHEADER       *ExtendedHeader;

  IDS_HDT_CONSOLE (GFX_MISC, "GfxIntInfoTableDebugDumpV5 Enter\n");

  PpTable = (ATOM_PPLIB_POWERPLAYTABLE5*) &SystemInfoTableV5Ptr->ulPowerplayTable;
  ExtendedHeader = (ATOM_PPLIB_EXTENDEDHEADER *) ((UINT8 *) (PpTable) + PpTable->usExtendendedHeaderOffset);
  IDS_HDT_CONSOLE (GFX_MISC, "    ExtendedHeader  usSize %d\n", ExtendedHeader->usSize);
  IDS_HDT_CONSOLE (GFX_MISC, "                    SizeOf %d\n", sizeof(ATOM_PPLIB_EXTENDEDHEADER));

  IDS_HDT_CONSOLE (GFX_MISC, "    ucHtcTmpLmt  0x%X\n", SystemInfoTableV5Ptr->sIntegratedSysInfo.ucHtcTmpLmt);
  IDS_HDT_CONSOLE (GFX_MISC, "    ucHtcHystLmt 0x%X\n", SystemInfoTableV5Ptr->sIntegratedSysInfo.ucHtcHystLmt);
  IDS_HDT_CONSOLE (GFX_MISC, "GfxIntInfoTableDebugDumpV5 Exit\n");
}

