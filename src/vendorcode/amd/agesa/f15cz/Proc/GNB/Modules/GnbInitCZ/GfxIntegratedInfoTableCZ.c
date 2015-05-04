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
#include  "GfxPwrPlayTableV5.h"
#include  "GnbGfxFamServices.h"
#include  "GnbRegistersCZ.h"
#include  "GnbRegisterAccCZ.h"
#include  "GfxConfigLib.h"
#include  "GfxIntegratedInfoTableV5.h"
#include  "GnbGfxInitLibV1.h"
#include  "GfxEnumConnectors.h"
#include  "GfxLibCZ.h"
#include  "GfxLibV5.h"
#include  "OptionGnb.h"
#include  "GnbSmuInitLibV8.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_GFXINTEGRATEDINFOTABLECZ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

#define GFX_REFCLK                        100     // (in MHz) Reference clock is 100 MHz
#define GFX_NCLK_MIN                      700     // (in MHz) Minimum value for NCLK is 700 MHz

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GfxIntInfoTableInterfaceCZ (
  IN      AMD_CONFIG_PARAMS       *StdHeader
  );

AGESA_STATUS
GfxMapEngineToDisplayPathCZ (
  IN       PCIe_ENGINE_CONFIG          *Engine,
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  );



/*----------------------------------------------------------------------------------------*/
CONST UINT8  DdiLaneConfigArrayCZ [][4] = {
  {16, 19, 0, 0},
  {19, 16, 0, 0},
  {20, 23, 1, 1},
  {23, 20, 1, 1},
  {24, 27, 2, 2},
  {27, 24, 2, 2}
};

/*----------------------------------------------------------------------------------------*/
/**
 * Initialize display path for given engine
 *
 *
 *
 * @param[in]   Engine            Engine configuration info
 * @param[out]  DisplayPathList   Display path list
 * @param[in]   Gfx               Pointer to global GFX configuration
 */

AGESA_STATUS
GfxMapEngineToDisplayPathCZ (
  IN       PCIe_ENGINE_CONFIG          *Engine,
     OUT   EXT_DISPLAY_PATH            *DisplayPathList,
  IN       GFX_PLATFORM_CONFIG         *Gfx
  )
{
  AGESA_STATUS      Status;
  UINT8             PrimaryDisplayPathId;
  UINT8             SecondaryDisplayPathId;
  UINTN             DisplayPathIndex;
  PrimaryDisplayPathId = 0xff;
  SecondaryDisplayPathId = 0xff;
  for (DisplayPathIndex = 0; DisplayPathIndex <  (sizeof (DdiLaneConfigArrayCZ) / 4); DisplayPathIndex++) {
    if (DdiLaneConfigArrayCZ[DisplayPathIndex][0] == Engine->EngineData.StartLane &&
        DdiLaneConfigArrayCZ[DisplayPathIndex][1] == Engine->EngineData.EndLane) {
      PrimaryDisplayPathId = DdiLaneConfigArrayCZ[DisplayPathIndex][2];
      SecondaryDisplayPathId = DdiLaneConfigArrayCZ[DisplayPathIndex][3];
      break;
    }
  }
  if (PrimaryDisplayPathId != 0xff) {
    IDS_HDT_CONSOLE (GFX_MISC, "  Allocate Display Connector at Primary sPath[%d]\n", PrimaryDisplayPathId);
    Engine->InitStatus |= INIT_STATUS_DDI_ACTIVE;
    GfxIntegratedCopyDisplayInfo (
      Engine,
      &DisplayPathList[PrimaryDisplayPathId],
      (PrimaryDisplayPathId != SecondaryDisplayPathId) ? &DisplayPathList[SecondaryDisplayPathId] : NULL,
      Gfx
      );
    Status = AGESA_SUCCESS;
  } else {
    IDS_HDT_CONSOLE (GFX_MISC, "  Error!!! Map DDI lanes %d - %d to display path failed\n",
      Engine->EngineData.StartLane,
      Engine->EngineData.EndLane
      );
    PutEventLog (
      AGESA_ERROR,
      GNB_EVENT_INVALID_DDI_LINK_CONFIGURATION,
      Engine->EngineData.StartLane,
      Engine->EngineData.EndLane,
      0,
      0,
      GnbLibGetHeader (Gfx)
      );
    Status = AGESA_ERROR;
  }
  return Status;
}

/*----------------------------------------------------------------------------------------*/
/**
 *Init CZ Nb p-State MemclkFreq
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] PpFuseArray         Fuse array pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxFillNbPstateMemclkFreqCZ (
  IN OUT   ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  UINT8                           Channel;
  D18F2x94_dct0_STRUCT           D18F2x94_dct0;
  GNB_HANDLE                     *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxFillNbPstateMemclkFreqCZ Enter\n");
  // Suppose channel 0 present
  Channel = 0;
  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));

  GnbRegisterReadCZ (GnbHandle, D18F2x94_dct0_TYPE, D18F2x94_dct0_ADDRESS, &D18F2x94_dct0.Value, 0, GnbLibGetHeader (Gfx));

  // Set channel 1 present if cheannel 0 disabled
  if (D18F2x94_dct0.Field.DisDramInterface == 1) {
    Channel = 1;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "  Channel =  = %d\n", Channel);

  //For CZ, change ulNbpStateMemclkFreq[4] to ulNbpStateMemclkFreq[2], just store hi and lo
  IntegratedInfoTable->ulNbpStateMemclkFreq[0] = 100 * GfxLibExtractDramFrequencyV5 ((UINT8) PpFuseArray->MemClkFreq[Channel], GnbLibGetHeader (Gfx));
  IDS_HDT_CONSOLE (GNB_TRACE, "  Mclk P0 = %08x\n", IntegratedInfoTable->ulNbpStateMemclkFreq[0]);
  IntegratedInfoTable->ulNbpStateMemclkFreq[1] = 100 * GfxLibExtractDramFrequencyV5 ((UINT8) PpFuseArray->M1MemClkFreq[Channel], GnbLibGetHeader (Gfx));
  IDS_HDT_CONSOLE (GNB_TRACE, "  Mclk P1 = %08x\n", IntegratedInfoTable->ulNbpStateMemclkFreq[1]);

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxFillNbPstateMemclkFreqCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 *Calculate ulGMCRestoreResetTime
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 * @param[in] PpFuseArray         Fuse array pointer
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
GfxCalculateRestoreResetTimeCZ (
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx,
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray
  )
{
  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Populate memory channels
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 * @param[in] PpFuseArray         Fuse array pointer
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
GfxPopulateMemoryChannelsCZ (
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx,
  IN       PP_FUSE_ARRAY_V2                 *PpFuseArray
  )
{
  D18F2x94_dct0_STRUCT           D18F2x94_dct0;
  D18F2x94_dct0_STRUCT           D18F2x94_dct1;
  GNB_HANDLE                     *GnbHandle;

  GnbHandle = GnbGetHandle (GnbLibGetHeader (Gfx));
  IntegratedInfoTable->ucUMAChannelNumber = 2;

  GnbRegisterReadCZ (GnbHandle, D18F2x94_dct0_TYPE, D18F2x94_dct0_ADDRESS, &D18F2x94_dct0.Value, 0, GnbLibGetHeader (Gfx));
  GnbRegisterReadCZ (GnbHandle, D18F2x94_dct1_TYPE, D18F2x94_dct1_ADDRESS, &D18F2x94_dct1.Value, 0, GnbLibGetHeader (Gfx));

  if (D18F2x94_dct0.Field.DisDramInterface == 1) {
    IntegratedInfoTable->ucUMAChannelNumber--;
  }
  if (D18F2x94_dct1.Field.DisDramInterface == 1) {
    IntegratedInfoTable->ucUMAChannelNumber--;
  }

  return AGESA_SUCCESS;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Init TDP Configuration
 *
 *
 * @param[in] IntegratedInfoTable Integrated info table pointer
 * @param[in] Gfx                 Gfx configuration info
 */

VOID
STATIC
GfxIntInfoTableInitTDPConfigCZ (
  IN       ATOM_INTEGRATED_SYSTEM_INFO_V1_9 *IntegratedInfoTable,
  IN       GFX_PLATFORM_CONFIG              *Gfx
  )
{
  SMU_TDP_INFO      *TdpInfo;
  UINT32            TargetTDP;
  UINT32            SocketTDP;

  TdpInfo = (SMU_TDP_INFO *) GnbLocateHeapBuffer (AMD_GNB_TDP_HANDLE, GnbLibGetHeader (Gfx));

  if (TdpInfo == NULL) {
    IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntInfoTableInitTDPConfigCZ failed!!\n");
    return;
  }
  // TDP value in tens of milli watts
  SocketTDP = TdpInfo->SmuTdp / 10;
  TargetTDP = TdpInfo->cTdp / 10;

  if (TargetTDP == 0) {
    TargetTDP = SocketTDP;
  }
  IntegratedInfoTable->asTdpConfig.TDP_config.uTDP_Value = SocketTDP;
  IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Value = TargetTDP;

  if (TargetTDP != SocketTDP) {
    IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable = 1;
  } else {
    IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable = 0;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "Target TDP: %d\n", IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "Original TDP: %d\n", IntegratedInfoTable->asTdpConfig.TDP_config.uTDP_Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "uCTDP_Enable: %d\n", IntegratedInfoTable->asTdpConfig.TDP_config.uCTDP_Enable);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *
 *
 *
 * @param[in] Gfx             Gfx configuration info
 * @retval    AGESA_STATUS
 */
AGESA_STATUS
STATIC
GfxIntInfoTableInitCZ (
  IN      GFX_PLATFORM_CONFIG     *Gfx
  )
{
  AGESA_STATUS                    Status;
  AGESA_STATUS                    AgesaStatus;
  ATOM_FUSION_SYSTEM_INFO_V5      SystemInfoTableV5;
  PP_FUSE_ARRAY_V2                *PpFuseArray;
  ATOM_PPLIB_POWERPLAYTABLE5      *PpTable;
  ACP_ENGINE_T                    *AcpEnginePtr;
  GNB_BUILD_OPTIONS_CZ            *GnbBuildOptionData;

  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInitCZ Enter\n");

  AgesaStatus = AGESA_SUCCESS;
  PpFuseArray = GnbLocateHeapBuffer (AMD_PP_FUSE_TABLE_HANDLE, GnbLibGetHeader (Gfx));

  if (PpFuseArray != NULL) {
    LibAmdMemFill (&SystemInfoTableV5, 0x00, sizeof (ATOM_FUSION_SYSTEM_INFO_V5), GnbLibGetHeader (Gfx));

    // Use common initialization first
    Status = GfxIntInfoTableInitV5 (Gfx, &SystemInfoTableV5, PpFuseArray);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    // Complete table with CZ-specific fields

    // Populate memory channels
    Status = GfxPopulateMemoryChannelsCZ (&SystemInfoTableV5.sIntegratedSysInfo, Gfx, PpFuseArray);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    // Fill in Nb P-state MemclkFreq Data
    GfxFillNbPstateMemclkFreqCZ (&SystemInfoTableV5.sIntegratedSysInfo, PpFuseArray, Gfx);

    // Build PP table
    PpTable = (ATOM_PPLIB_POWERPLAYTABLE5*) &SystemInfoTableV5.ulPowerplayTable;
    Status = GfxPwrPlayBuildTable (PpTable,  Gfx);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    IDS_HDT_CONSOLE (GNB_TRACE, "CZ Pplay done\n");

    // Assign usFormatID to represent CZ
    PpTable->usFormatID = 0x17;
    PpTable->sThermalController.ucType = ATOM_PP_THERMALCONTROLLER_CZ;

    // Family specific data update
    // Determine ulGMCRestoreResetTime
    Status = GfxCalculateRestoreResetTimeCZ (&SystemInfoTableV5.sIntegratedSysInfo, Gfx, PpFuseArray);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);

    SystemInfoTableV5.sIntegratedSysInfo.ulDDR_DLL_PowerUpTime = 4940;
    SystemInfoTableV5.sIntegratedSysInfo.ulDDR_PLL_PowerUpTime = 2000;

    SystemInfoTableV5.sIntegratedSysInfo.ulGPUCapInfo = GPUCAPINFO_TMDS_HDMI_USE_CASCADE_PLL_MODE | GPUCAPINFO_DP_USE_SINGLE_PLL_MODE;
    GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, GnbLibGetHeader (Gfx));
    if (GnbBuildOptionData != NULL) {
      if (GnbBuildOptionData->CfgSMUServiceEnablementBitMap.Field.DFS_BYPASS == 1) {
        SystemInfoTableV5.sIntegratedSysInfo.ulGPUCapInfo |= GPUCAPINFO_DFS_BYPASS_ENABLE;
      }
      if (GnbBuildOptionData->CfgSMUServiceEnablementBitMap.Field.CONN_STDBY == 1) {
        SystemInfoTableV5.sIntegratedSysInfo.ulGPUCapInfo |= GPUCAPS__GNB_FAST_RESUME_CAPABLE;
      }
      if (GnbBuildOptionData->CfgSMUServiceEnablementBitMap.Field.NB_DPM == 1) {
        SystemInfoTableV5.sIntegratedSysInfo.ulSystemConfig |= BIT3;
      }
    }
    IDS_HDT_CONSOLE (GNB_TRACE, "ulGPUCapInfo = %x\n", SystemInfoTableV5.sIntegratedSysInfo.ulGPUCapInfo);
    IDS_HDT_CONSOLE (GNB_TRACE, "ulSystemConfig = %x\n", SystemInfoTableV5.sIntegratedSysInfo.ulSystemConfig);

    // Populate usGPUReservedSysMemSize, usGPUReservedSysMemBaseAddrLo and usGPUReservedSysMemBaseAddrHi
    AcpEnginePtr = (ACP_ENGINE_T *) GnbLocateHeapBuffer (AMD_GNB_ACP_ENGINE_HANDLE, GnbLibGetHeader (Gfx));
    if (AcpEnginePtr != NULL) {
      SystemInfoTableV5.sIntegratedSysInfo.usGPUReservedSysMemSize = (USHORT) AcpEnginePtr->AcpEngineSize;
      SystemInfoTableV5.sIntegratedSysInfo.ulGPUReservedSysMemBaseAddrLo = (ULONG) (AcpEnginePtr->AcpEngineBase & 0xFFFFFFFF);
      SystemInfoTableV5.sIntegratedSysInfo.ulGPUReservedSysMemBaseAddrHi = (ULONG) (AcpEnginePtr->AcpEngineBase >> 32);
    }

    GfxIntInfoTableInitTDPConfigCZ (&SystemInfoTableV5.sIntegratedSysInfo, Gfx);

    IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_INTEGRATED_TABLE_CONFIG, &SystemInfoTableV5.sIntegratedSysInfo, GnbLibGetHeader (Gfx));
    // Copy integrated info table to Frame Buffer. (Do not use LibAmdMemCopy, routine not guaranteed access to above 4G memory in 32 bit mode.)
    GfxIntInfoTablePostToFbV5 (&SystemInfoTableV5, Gfx);
  } else {
    IDS_HDT_CONSOLE (GNB_TRACE, "  ERROR! Cannot locate fuse table\n");
    Status = AGESA_ERROR;
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInitCZ Exit [0x%x]\n", Status);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Build integrated info table
 *  GMC FB access requred
 *
 *
 * @param[in]   StdHeader     Standard configuration header
 * @retval      AGESA_STATUS
 */
AGESA_STATUS
GfxIntInfoTableInterfaceCZ (
  IN      AMD_CONFIG_PARAMS       *StdHeader
  )
{
  AGESA_STATUS                    AgesaStatus;
  AGESA_STATUS                    Status;
  GFX_PLATFORM_CONFIG             *Gfx;
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInterfaceCZ Enter\n");
  AgesaStatus = AGESA_SUCCESS;
  if (GfxLibIsControllerPresent (StdHeader)) {
    Status = GfxLocateConfigData (StdHeader, &Gfx);
    AGESA_STATUS_UPDATE (Status, AgesaStatus);
    if (Status != AGESA_FATAL) {
      Status = GfxIntInfoTableInitCZ (Gfx);
      AGESA_STATUS_UPDATE (Status, AgesaStatus);
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GfxIntegratedInfoTableInterfaceCZ Exit[0x%x]\n", AgesaStatus);
  return AgesaStatus;
}
