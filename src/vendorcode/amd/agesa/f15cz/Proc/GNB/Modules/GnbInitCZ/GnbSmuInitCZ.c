/* $NoKeywords:$ */
/**
 * @file
 *
 * SMU mid post initialization.
 *
 *
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:     AGESA
 * @e sub-project: GNB
 * @e \$Revision: 313269 $   @e \$Date: 2015-02-18 18:19:37 -0600 (Wed, 18 Feb 2015) $
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
#include  "GnbNbInitLibV4.h"
#include  "GnbTable.h"
#include  "GnbRegisterAccCZ.h"
#include  "GnbRegistersCZ.h"
#include  "GnbFamServices.h"
#include  "OptionGnb.h"
#include  "GnbSmuInitLibV8.h"
#include  "GnbBiosSmc.h"
#include  "GnbPcieInitLibV1.h"
#include  "heapManager.h"
#include  "GnbPspCZ.h"
#include  "mm.h"
#include  "PspBaseLib.h"
#include  "Filecode.h"
#define FILECODE PROC_GNB_MODULES_GNBINITCZ_GNBSMUINITCZ_FILECODE
/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

extern BUILD_OPT_CFG        UserOptions;

#define GnbFpLibToUint32(Value)            (*((UINT32*) &Value))

#define MEMCLK_ID_TO_FREQ(Id)  ((((Id) + 6) * 33) + (((Id) + 6) / 3))


#define GnbFpLibGetExp(V)  ((INT32) ((*((UINT32*) &Value) >> 23) & 0xFF) - 127)
#define GnbFpLibGetFraction(V) (UINT32) ((*((UINT32*) &Value) & ((1ull << 23) - 1)) | (1ull << 23))

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U  R E S
 *----------------------------------------------------------------------------------------
 */
typedef float FLOAT;

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U  N C T I O N S
 *----------------------------------------------------------------------------------------
 */

AGESA_STATUS
GnbScsInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

AGESA_STATUS
GnbSmuEarlyInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

AGESA_STATUS
GnbSmuPostInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

AGESA_STATUS
GnbSmuMidInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  );

/*----------------------------------------------------------------------------------------*/
/**
 * Extract UINT32 TDP from Single Float
 *
 *
 *
 * @param[in]  Value  UINT32 value but IEEE sigle precision format
 * @retval     UINT32
 */
UINT32
GnbGetTdp (
  IN      UINT32  Value
  )
{
  UINT32  Fraction;
  INT32   Exponent;
  UINT32  Result;
  UINT32  Fp;

  Fraction = GnbFpLibGetFraction (Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "Fraction %x\n", Fraction);
  Exponent = GnbFpLibGetExp (Value);
  IDS_HDT_CONSOLE (GNB_TRACE, "Exponent %x\n", Exponent);
  Result = 0;

  if (Exponent <= 0) {
    Result = 0;
  } else {

    Result = Fraction >> (23 - Exponent);
    Result = Result * 1000;
    // Get TDP unit to milli-watt
    Fp = Fraction & ((1ull << (23 - Exponent)) - 1);
    IDS_HDT_CONSOLE (GNB_TRACE, "Fp =  %x\n", Fp);
    Result += ((Fp >> (23 - Exponent - 3)) & 0x1) * 125;
    Result += ((Fp >> (23 - Exponent - 2)) & 0x1) * 250;
    Result += ((Fp >> (23 - Exponent - 1)) & 0x1) * 500;
    IDS_HDT_CONSOLE (GNB_TRACE, "Result %x\n", Result);
  }
  return Result;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load System Configuration Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */
UINT32
GnbSmuLoadSystemConfigParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;
  UINT32                  ReturnValue;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadSystemConfigParamsCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = EarlyParams->PlatformConfig.SystemConfiguration;
  if (EarlyParams->PlatformConfig.SystemConfiguration == 0) {
    // 15w default config
    SmuArg[0] = 1;
  }

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadSystemConfigParameters,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "SMU with return value floating TDP formatt %x\n", SmuArg[0]);
  ReturnValue = GnbGetTdp (SmuArg[0]);
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadSystemConfigParamsCZ Exit with TDP in milli-watt %d\n", ReturnValue);

  return ReturnValue;
}
/*----------------------------------------------------------------------------------------*/
/**
 * Load TDC Limit Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
GnbSmuLoadEdcLimitParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadEdcLimitParamsCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = EarlyParams->PlatformConfig.VrmProperties[CoreVrm].MaximumCurrentLimit;
  SmuArg[1] = EarlyParams->PlatformConfig.VrmProperties[NbVrm].MaximumCurrentLimit;
  SmuArg[2] = EarlyParams->PlatformConfig.VrmProperties[GfxVrm].MaximumCurrentLimit;

  if ((SmuArg[0] == 0) && (SmuArg[1] == 0) && (SmuArg[2] == 0)) {
    return;
  }

  IDS_HDT_CONSOLE (NB_MISC, "  EdcLimitParams current amps %d, %d, %d\n", SmuArg[0], SmuArg[1], SmuArg[2]);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadEdcLimitParameters,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadEdcLimitParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load TDC Limit Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadTdcLimitParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT                DevObject;
  UINT32                    SmuArg[6];
  AMD_EARLY_PARAMS          *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadTdcLimitParamsCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = EarlyParams->PlatformConfig.VrmProperties[CoreVrm].CurrentLimit;
  SmuArg[1] = EarlyParams->PlatformConfig.VrmProperties[NbVrm].CurrentLimit;
  SmuArg[2] = EarlyParams->PlatformConfig.VrmProperties[GfxVrm].CurrentLimit;

  if ((SmuArg[0] == 0) && (SmuArg[1] == 0) && (SmuArg[2] == 0)) {
    return;
  }

  IDS_HDT_CONSOLE (NB_MISC, "  TdcLimitParams current apms %d, %d, %d\n", SmuArg[0], SmuArg[1], SmuArg[2]);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadTdcLimitParameters,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadTdcLimitParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load CTdp Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

UINT32
STATIC
GnbSmuLoadCtdpParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;
  UINT32                  ReturnValue;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;
  ReturnValue = 0;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadCtdpParamsCZ Enter\n");

  if (EarlyParams->PlatformConfig.PowerCeiling == 0) {
    return ReturnValue;
  }

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = EarlyParams->PlatformConfig.PowerCeiling;
  ReturnValue = SmuArg[0];

  IDS_HDT_CONSOLE (NB_MISC, "  CtdpParams milli-watts %d\n", SmuArg[0]);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadCTdpParameters,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadCtdpParamsCZ Exit\n");
  return ReturnValue;
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load Package Power Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadPkgPowerParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadPkgPowerParamsCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = EarlyParams->PlatformConfig.PkgPwrLimitAC;
  SmuArg[1] = EarlyParams->PlatformConfig.PkgPwrLimitDC;

  if ((SmuArg[0] == 0) && (SmuArg[1] == 0)) {
    return;
  }

  IDS_HDT_CONSOLE (NB_MISC, "  PkgPowerParams milli-watts AC %d, DC %d\n", SmuArg[0], SmuArg[1]);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadPkgPowerParameters,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadPkgPowerParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load Memory Config Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuPrepareMemoryParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DIE_STRUCT              *MCTPtr;
  MEM_DATA_STRUCT         *MemDataPtr;
  FLOAT                   VddIo;
  FLOAT                   Vddr;
  UINT8                   offset;
  UINT16                  DimmValid;
  D18F2x2E0_dct0_STRUCT   D18F2x2E0_dct0;
  D18F2x2E0_dct1_STRUCT   D18F2x2E0_dct1;
  UINT32                  Dimms;
  UINT32                  MemorySpeedHigh;
  UINT32                  MemorySpeedLow;
  UINT32                  *SmuArg;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuPrepareMemoryParamsCZ Enter\n");

  SmuArg = (UINT32 *) GnbAllocateHeapBufferAndClear (AMD_GNB_SMU_MEMORY_INFO_BUFFER_HANDLE, 24, StdHeader);
  ASSERT (SmuArg != NULL);

  MCTPtr = (DIE_STRUCT *) GnbLocateHeapBuffer (GENERATE_MEM_HANDLE (ALLOC_DIE_STRUCT_HANDLE, 0, 0, 0), StdHeader);
  ASSERT (MCTPtr != NULL);

  MemDataPtr = (MEM_DATA_STRUCT *) GnbLocateHeapBuffer (AMD_MEM_DATA_HANDLE, StdHeader);
  ASSERT (MemDataPtr != NULL);

  DimmValid = (UINT16) (MCTPtr->DimmValid & 0xFFFF);
  DimmValid |= (UINT16) (MCTPtr->DimmValid >> 16);

  IDS_HDT_CONSOLE (GNB_TRACE, "Valid Dimm Mask: %08x\n", MCTPtr->DimmValid);
  IDS_HDT_CONSOLE (GNB_TRACE, "Local Dimm Mask: %04x\n", DimmValid);
  Dimms = 0;

  // Get number of dimms
  for (offset = 0; offset < 16; offset ++) {
    if ((DimmValid & ((UINT32)1 << offset)) != 0) {
      Dimms ++;
    }
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Number of Dimms: %d\n", Dimms);
  SmuArg[4] = Dimms;

  // Get memory frequency
  GnbRegisterReadCZ (GnbHandle, D18F2x2E0_dct0_TYPE, D18F2x2E0_dct0_ADDRESS, &D18F2x2E0_dct0.Value, 0, StdHeader);
  GnbRegisterReadCZ (GnbHandle, D18F2x2E0_dct1_TYPE, D18F2x2E0_dct1_ADDRESS, &D18F2x2E0_dct1.Value, 0, StdHeader);
  MemorySpeedHigh = MCTPtr->DctData[0].Timings.Speed & 0xFFFF;

  if ((D18F2x2E0_dct0.Field.M1MemClkFreq != 0) || (D18F2x2E0_dct1.Field.M1MemClkFreq != 0)) {
    if (D18F2x2E0_dct0.Field.M1MemClkFreq > D18F2x2E0_dct1.Field.M1MemClkFreq) {
      IDS_HDT_CONSOLE (GNB_TRACE, "D18F2x2E0_dct0: %d\n", D18F2x2E0_dct0.Field.M1MemClkFreq);
      MemorySpeedLow = MEMCLK_ID_TO_FREQ (D18F2x2E0_dct0.Field.M1MemClkFreq);
    } else {
      IDS_HDT_CONSOLE (GNB_TRACE, "D18F2x2E0_dct1: %d\n", D18F2x2E0_dct1.Field.M1MemClkFreq);
      MemorySpeedLow = MEMCLK_ID_TO_FREQ (D18F2x2E0_dct1.Field.M1MemClkFreq);
    }
  } else {
    MemorySpeedLow = DDR667_FREQUENCY;
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Speed Low: %dMHz\n", MemorySpeedLow);
  SmuArg[3] = MemorySpeedLow;
  IDS_HDT_CONSOLE (GNB_TRACE, "Memory Speed High: %dMHz\n", MemorySpeedHigh);
  SmuArg[2] = MemorySpeedHigh;

  // Get VDDIO
  // Take the voltage in volt
  VddIo = 1.5;
  switch (MemDataPtr->ParameterListPtr->DDR3Voltage) {
  case VOLT1_5:
    VddIo = 1.5;
    break;
  case VOLT1_35:
    VddIo = 1.35;
    break;
  case VOLT1_25:
    VddIo = 1.25;
    break;
  default:
    ASSERT (FALSE);
  }

  SmuArg[0] = GnbFpLibToUint32 (VddIo);

  // Get VDDR
  // Format is the same as VDDIO
  Vddr = 1.05;
  ASSERT (UserOptions.CfgMemoryPhyVoltage < MAX_VDDR);
  SmuArg[1] = GnbFpLibToUint32 (Vddr);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuPrepareMemoryParamsCZ Exit\n");

}

/*----------------------------------------------------------------------------------------*/
/**
 * Load Memory Config Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadMemoryParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  *SmuArg;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadMemoryParamsCZ Enter\n");

  SmuArg = (UINT32 *) GnbLocateHeapBuffer (AMD_GNB_SMU_MEMORY_INFO_BUFFER_HANDLE, StdHeader);
  ASSERT (SmuArg != NULL);
  DevObject.GnbHandle = GnbHandle;
  DevObject.StdHeader = StdHeader;
  DevObject.DevPciAddress = GnbGetHostPciAddress (GnbHandle);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadMemoryParameters,
    SmuArg,
    GNB_REG_ACC_FLAG_S3SAVE
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadMemoryParamsCZ Exit\n");

}

/*----------------------------------------------------------------------------------------*/
/**
 * Load STAPM Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadStapmLimitParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadStapmLimitParamsCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);

  SmuArg[0] = EarlyParams->PlatformConfig.PlatStapmConfig.CfgStapmScalar;
  switch (EarlyParams->PlatformConfig.PlatStapmConfig.CfgStapmBoost) {
  case StapmBoostDisabled:
    SmuArg[1] = 1;
    break;
  case StapmBoostEnabled:
    SmuArg[1] = 2;
    break;
  case StapmBoostSystemConfig:
  default:
    SmuArg[1] = 0;
    break;
  }
  SmuArg[2] = EarlyParams->PlatformConfig.PlatStapmConfig.CfgStapmTimeConstant;

  IDS_HDT_CONSOLE (NB_MISC, "  StapmScalar %x\n", SmuArg[0]);
  IDS_HDT_CONSOLE (NB_MISC, "  StapmBoost %x\n", SmuArg[1]);
  IDS_HDT_CONSOLE (NB_MISC, "  StapmTimeConstant milli-seconds %d\n", SmuArg[2]);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadStapmParameters,
    SmuArg,
    0
    );
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadStapmLimitParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load CTdp Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadBbbParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadBbbParamsCZ Enter\n");

  if (EarlyParams->PlatformConfig.BatteryBoostTune != 0) {
    GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
    SmuArg[0] = EarlyParams->PlatformConfig.BatteryBoostTune;
    IDS_HDT_CONSOLE (NB_MISC, "  BbbParams %d\n", SmuArg[0]);

    GnbSmuServiceRequestV8 (
      &DevObject,
      SMC_MSG_LoadBbbParameters,
      SmuArg,
      0
      );
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadBbbParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load PSI current values to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadPsiParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT                DevObject;
  UINT32                    SmuArg[6];
  AMD_EARLY_PARAMS          *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadPsiParamsCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  SmuArg[0] = EarlyParams->PlatformConfig.VrmProperties[CoreVrm].LowPowerThreshold;
  SmuArg[1] = EarlyParams->PlatformConfig.VrmProperties[NbVrm].LowPowerThreshold;
  SmuArg[2] = EarlyParams->PlatformConfig.VrmProperties[GfxVrm].LowPowerThreshold;

  if ((SmuArg[0] == 0) && (SmuArg[1] == 0) && (SmuArg[2] == 0)) {
    return;
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "  PsiParams current values %d, %d, %d\n", SmuArg[0], SmuArg[1], SmuArg[2]);

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_LoadPsiParameters,
    SmuArg,
    0
    );

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadPsiParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU wait for Cac Lock
 *
 *
 *
 * @param[in]  AccessFlags         Flag for S3
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuWaitForCacLockCZ (
  IN       UINT32                AccessFlags,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT64                  MsrData;
  GNB_HANDLE              *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuWaitForCacLockCZ Enter\n");

  if ((AccessFlags & GNB_REG_ACC_FLAG_S3SAVE) != 0) {
    S3_SAVE_DISPATCH (StdHeader, GnbSmuWaitForCacLockS3Script_ID, 0, NULL);
  }

  GnbHandle = GnbGetHandle (StdHeader);

  do {
    GnbRegisterReadCZ (GnbHandle, MSRC001_1073_TYPE, MSRC001_1073_ADDRESS, &MsrData, 0, StdHeader);
  } while ((MsrData & BIT62) == 0);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuWaitForCacLockCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * S3 script
 *
 *
 *
 * @param[in]  StdHeader      Standard configuration header
 * @param[in]  ContextLength  Context Length (not used)
 * @param[in]  Context        pointer to UINT32 number of us
 */
VOID
GnbSmuWaitForCacLockCZS3Script (
  IN      AMD_CONFIG_PARAMS    *StdHeader,
  IN      UINT16               ContextLength,
  IN      VOID*                Context
  )
{
//  GnbSmuWaitForCacLockCZ (0, StdHeader);
}

/*----------------------------------------------------------------------------------------*/
/**
 * Program Apm Msr
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuProgramApmMsrCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  UINT32                  RequestId;
  UINT32                  AccessFlags;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuProgramApmMsrCZ Enter\n");

  GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
  RequestId = SMC_MSG_ProgramApmMSRs;
  AccessFlags = 0;

  if (!CheckPspPlatformSecureEnable ()) {
    AccessFlags = GNB_REG_ACC_FLAG_S3SAVE;
  }

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_ProgramApmMSRs,
    SmuArg,
    AccessFlags
    );

  if ((RequestId != 0xff) && GnbBuildOptionData->GnbCommonOptions.CfgSmuMessageEnable) {
    GnbSmuWaitForCacLockCZ (AccessFlags, StdHeader);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuProgramApmMsrCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Configure SMU Port 80 monitoring
 *
 * @param[in]  StdHeader  Standard configuration header
 */

VOID
GnbSmuPort80ConfigCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  GNB_HANDLE              *GnbHandle;
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  GNB_SMU_PORT80_PARAMS   SmuPort80;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuPort80ConfigCZ Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);
  LibAmdMemFill (&SmuPort80, 0x00, sizeof (SmuPort80), StdHeader);

  IDS_OPTION_HOOK (IDS_GNB_SMU_PORT80_PARAMS, &SmuPort80, StdHeader);

  if ((SmuPort80.EnableCsrShadow == 1) ||
      (SmuPort80.EnableDramShadow == 1)) {

    IDS_HDT_CONSOLE (GNB_TRACE, "<-------- P80 Config -------->\n");
    IDS_HDT_CONSOLE (GNB_TRACE,
      "  Mmio:        %08x\n"
      "  MemLo:       %08x\n"
      "  MemHi:       %08x\n"
      "  BufSize:     %04x\n"
      "  CSR Shadow:  %d\n"
      "  DRAM Shadow: %d\n"
      "  Polling:     %d\n",
      SmuPort80.MmioAddress,
      SmuPort80.MemoryBaseLo,
      SmuPort80.MemoryBaseHi,
      SmuPort80.MemoryBufferSize,
      SmuPort80.EnableCsrShadow,
      SmuPort80.EnableDramShadow,
      SmuPort80.PollingInterval
    );
    IDS_HDT_CONSOLE (GNB_TRACE, "<---------------------------->\n");

    SmuArg[0] = SmuPort80.MmioAddress;
    SmuArg[1] = SmuPort80.MemoryBaseHi;
    SmuArg[2] = SmuPort80.MemoryBaseLo;
    SmuArg[3] = SmuPort80.MemoryBufferSize;
    SmuArg[4] = (UINT32) ((((UINT16) SmuPort80.PollingInterval) << 16) | (((UINT8) SmuPort80.EnableCsrShadow) << 8) | ((UINT8) SmuPort80.EnableDramShadow));

    GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
    GnbSmuServiceRequestV8 (
      &DevObject,
      SMC_MSG_LoadPort80MonitorParameters,
      SmuArg,
      GNB_REG_ACC_FLAG_S3SAVE
      );

  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuPort80ConfigCZ Exit\n");

}

/*----------------------------------------------------------------------------------------*/
/**
 * Load TjMax Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadTjMaxParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT              DevObject;
  UINT32                  SmuArg[6];
  AMD_EARLY_PARAMS        *EarlyParams;

  EarlyParams = (AMD_EARLY_PARAMS *) StdHeader;

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadTjMaxParamsCZ Enter\n");

  if (EarlyParams->PlatformConfig.ThermCtlLimit != 0) {
    GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
    SmuArg[0] = EarlyParams->PlatformConfig.ThermCtlLimit;
    IDS_HDT_CONSOLE (NB_MISC, "  ThermCtlLimit %d\n", SmuArg[0]);

    GnbSmuServiceRequestV8 (
      &DevObject,
      SMC_MSG_LoadTjMaxParameters,
      SmuArg,
      0
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadTjMaxParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * Load Determinism Ambient Parameters to SMU
 *
 *
 *
 * @param[in]  GnbHandle           Pointer to GNB_HANDLE
 * @param[in]  GnbBuildOptionData  Pointer to Build Options
 * @param[in]  StdHeader           Standard configuration header
 */

VOID
STATIC
GnbSmuLoadDeterminsmTambientParamsCZ (
  IN       GNB_HANDLE            *GnbHandle,
  IN       GNB_BUILD_OPTIONS_CZ  *GnbBuildOptionData,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  DEV_OBJECT                DevObject;
  UINT32                    SmuArg[6];


  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadDeterminsmTambientParamsCZ Enter\n");

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  if (GnbBuildOptionData->CfgSmuDeterminismAmbient != 0) {
    GnbSmuServiceCommonInitArgumentsV8 (GnbHandle, StdHeader, &DevObject, SmuArg);
    SmuArg[0] = GnbBuildOptionData->CfgSmuDeterminismAmbient;
    IDS_HDT_CONSOLE (NB_MISC, "  DeterminismAmbient %x\n", SmuArg[0]);

    GnbSmuServiceRequestV8 (
      &DevObject,
      SMC_MSG_LoadDeterminismParameters,
      SmuArg,
      GNB_REG_ACC_FLAG_S3SAVE
      );
  }
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuLoadDeterminsmTambientParamsCZ Exit\n");
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU Early Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbSmuEarlyInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              AgesaStatus;
  GNB_HANDLE                *GnbHandle;
  GNB_BUILD_OPTIONS_CZ      *GnbBuildOptionData;
  SMU_TDP_INFO              *TdpInfo;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuEarlyInitCZ Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  LibAmdFinit ();

  TdpInfo = (SMU_TDP_INFO *) GnbAllocateHeapBuffer (AMD_GNB_TDP_HANDLE, sizeof (SMU_TDP_INFO), StdHeader);


  TdpInfo->SmuTdp = GnbSmuLoadSystemConfigParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "TdpInfo->SmuTdp = %d\n", TdpInfo->SmuTdp);
  GnbSmuLoadEdcLimitParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  GnbSmuLoadTdcLimitParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  TdpInfo->cTdp = GnbSmuLoadCtdpParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  IDS_HDT_CONSOLE (GNB_TRACE, "TdpInfo->cTdp = %d\n", TdpInfo->cTdp);

  GnbSmuLoadPkgPowerParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  GnbSmuLoadStapmLimitParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  GnbSmuLoadBbbParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  GnbSmuLoadTjMaxParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  if (GnbBuildOptionData->CfgPSIEnable) {
    GnbSmuLoadPsiParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuEarlyInitCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbSmuPostInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              AgesaStatus;
  GNB_HANDLE                *GnbHandle;
  GNB_BUILD_OPTIONS_CZ      *GnbBuildOptionData;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuPostInitCZ Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  GnbSmuPrepareMemoryParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuPostInitCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU Mid Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbSmuMidInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              AgesaStatus;
  GNB_HANDLE                *GnbHandle;
  GNB_BUILD_OPTIONS_CZ      *GnbBuildOptionData;

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuMidInterfaceCZ Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  GnbSmuLoadMemoryParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  GnbSmuPort80ConfigCZ (StdHeader);
  GnbSmuLoadDeterminsmTambientParamsCZ (GnbHandle, GnbBuildOptionData, StdHeader);
  // After BTC
//  GnbSmuProgramApmMsrCZ (GnbHandle, GnbBuildOptionData, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuMidInterfaceCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * GNB Scs interface for CZ
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */
AGESA_STATUS
GnbScsInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS      Status;
  AGESA_STATUS      AgesaStatus;
  GNB_HANDLE        *GnbHandle;
  UINTN             ScsDataAddress;
  UINTN             ScsDataSize;

  AgesaStatus = AGESA_SUCCESS;
  Status = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceCZ Enter\n");
  GnbHandle = GnbGetHandle (StdHeader);

  Status = GnbGetScsDataCZ (&ScsDataAddress, &ScsDataSize);
  IDS_HDT_CONSOLE (GNB_TRACE, "  ScsDataAddress = %08x\n", ScsDataAddress);
  IDS_HDT_CONSOLE (GNB_TRACE, "  ScsDataSize    = %08x\n", ScsDataSize);
  ASSERT (ScsDataAddress != NULL);

  Status = GnbSmuLoadScsDataV8 (GnbHandle, (UINT8 *) ScsDataAddress, SMC_MSG_ReadScs, StdHeader);

  AGESA_STATUS_UPDATE (Status, AgesaStatus);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbScsInterfaceCZ Exit [0x%x]\n", Status);
  return  AgesaStatus;
}

/*----------------------------------------------------------------------------------------*/
/**
 * SMU Mid Late Post Init
 *
 *
 *
 * @param[in]  StdHeader  Standard configuration header
 * @retval     AGESA_STATUS
 */

AGESA_STATUS
GnbSmuMidLateInterfaceCZ (
  IN      AMD_CONFIG_PARAMS               *StdHeader
  )
{
  AGESA_STATUS              AgesaStatus;
  GNB_HANDLE                *GnbHandle;
  DEV_OBJECT                DevObject;
  GNB_BUILD_OPTIONS_CZ      *GnbBuildOptionData;
  UINT32                    EnablementMask;
  UINT32                    SmuArg[6];

  AgesaStatus = AGESA_SUCCESS;
  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuMidLateInterfaceCZ Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);

  GnbBuildOptionData = GnbLocateHeapBuffer (AMD_GNB_BUILD_OPTIONS_HANDLE, StdHeader);
  ASSERT (GnbBuildOptionData != NULL);

  EnablementMask = GnbBuildOptionData->CfgSMUServiceEnablementBitMap.Value;

  IDS_OPTION_CALLOUT (IDS_CALLOUT_GNB_SMUFEATURES_MASK_OVERRIDE, &EnablementMask, StdHeader);

  IDS_HDT_CONSOLE (GNB_TRACE, "BAPM %s\n", ((EnablementMask & BAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "POWER_ESTIMATOR %s\n", ((EnablementMask & POWER_ESTIMATOR_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "THERMAL_CONTROLLER %s\n", ((EnablementMask & THERMAL_CONTROLLER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "TDC_LIMITING %s\n", ((EnablementMask & TDC_LIMITING_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PKG_PWR_LIMITING %s\n", ((EnablementMask & PKG_PWR_LIMITING_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LCLK_DPM %s\n", ((EnablementMask & LCLK_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "VOLTAGE_CONTROLLER %s\n", ((EnablementMask & VOLTAGE_CONTROLLER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "GLOBAL EAPM %s\n", ((EnablementMask & GEAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "BBB_MASK %s\n", ((EnablementMask & BBB_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "IBS %s\n", ((EnablementMask & IBS_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PSTATE_ARBITER %s\n", ((EnablementMask & PSTATE_ARBITER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "NB_DPM %s\n", ((EnablementMask & NB_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LCLK DEEP SLEEP %s\n", ((EnablementMask & LCLK_DEEP_SLEEP_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "ULV_MASK %s\n", ((EnablementMask & ULV_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "GFX_CU_PG %s\n", ((EnablementMask & GFX_CU_PG_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "AVFS_MASK %s\n", ((EnablementMask & AVFS_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "UVD_DPM_MASK %s\n", ((EnablementMask & UVD_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "VQ_CONTROLLER_MASK %s\n", ((EnablementMask & VQ_CONTROLLER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "ACP_DPM_MASK %s\n", ((EnablementMask & ACP_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "SCLK_DPM_MASK %s\n", ((EnablementMask & SCLK_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PRE_FETCH_DATA_MASK %s\n", ((EnablementMask & PRE_FETCH_DATA_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "DFS_BYPASS_MASK %s\n", ((EnablementMask & DFS_BYPASS_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "HTC_MASK %s\n", ((EnablementMask & HTC_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "VDDGFX_MASK %s\n", ((EnablementMask & VDDGFX_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "LOCAL_EAPM_MASK %s\n", ((EnablementMask & LEAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "CONN_STDBY %s\n", ((EnablementMask & CONN_STDBY_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PORT80_MONITOR_MASK %s\n", ((EnablementMask & PORT80_MONITOR_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "PSPCLK_DPM_MASK %s\n", ((EnablementMask & PSPCLK_DPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "STAPM_MASK %s\n", ((EnablementMask & STAPM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "DETERMINISM_MASK %s\n", ((EnablementMask & DETERMINISM_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "CPU_STRETCHER_MASK %s\n", ((EnablementMask & CPU_STRETCHER_MASK) != 0) ? "Enable" : "Disable");
  IDS_HDT_CONSOLE (GNB_TRACE, "SCLK_STRETCHER_MASK %s\n", ((EnablementMask & SCLK_STRETCHER_MASK) != 0) ? "Enable" : "Disable");

  WriteIo32 (0x84, 0xabcd);

  DevObject.DevPciAddress.AddressValue = MAKE_SBDFO (0, 0, 0, 0, 0);
  DevObject.GnbHandle = GnbHandle;
  DevObject.StdHeader = StdHeader;
  LibAmdMemFill (SmuArg, 0x00, sizeof (SmuArg), StdHeader);
  SmuArg[0] = EnablementMask;

  GnbSmuServiceRequestV8 (
    &DevObject,
    SMC_MSG_EnableAllSmuFeatures,
    SmuArg,
    GNB_REG_ACC_FLAG_S3SAVE
    );

  WriteIo32 (0x84, 0xabef);

  IDS_HDT_CONSOLE (GNB_TRACE, "GnbSmuMidLateInterfaceCZ Exit [0x%x]\n", AgesaStatus);
  return  AgesaStatus;
}
