/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Integrated Debug Option Specific Routines for common F15
 *
 * Contains AMD AGESA debug macros and library functions
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  IDS
 * @e \$Revision: 311790 $   @e \$Date: 2015-01-27 13:03:49 +0800 (Tue, 27 Jan 2015) $
 */
/*****************************************************************************
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
#include "Ids.h"
#include "IdsLib.h"
#include "IdsInternal.h"
#include "IdsInternalLib.h"
#include "IdsIntF15CzNvDef.h"
#include "cpuFamilyTranslation.h"
#include "cpuFeatures.h"
#include "cpuLateInit.h"
#include "cpuIoCstate.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15CzPowerMgmt.h"
#include "GeneralServices.h"
#include "GnbRegistersCZ.h"
#include "IdsIntF15AllService.h"
#include "Gnb.h"
#include "GnbCommonLib.h"
#include "GnbGfx.h"
#include "GnbPcie.h"
#include "GnbPcieConfig.h"
#include "GnbPcieInitLibV1.h"
#include "GfxCardInfo.h"
#include "GnbBiosSmc.h"
#include "PcieComplexDataCZ.h"
#include "GnbRegisterAccCZ.h"
#include "mm.h"
#include "mn.h"
#include "mp.h"
#include "mnreg.h"
#include "IdsIntF15CzAllService.h"
#include "OptionGnb.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

///Please check IdsInternalLib.h for internal FILECODE
#define FILECODE PROC_IDS_INTERNAL_FAMILY_0X15_CZ_IDSINTF15CZALLSERVICE_FILECODE

// Stretch Max Freq for unbuffered dimm.
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC PSCFG_MAXFREQ_ENTRY ROMDATA MaxStretchFreqCZUDIMM[] = {
  {_2DIMM, 1, 1, 0, 0, DDR2400_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR2400_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_1DIMM, 1, 1, 0, 0, DDR2133_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR2133_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR2133_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR2133_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY}
};

STATIC PSC_TBL_ENTRY CZMaxStretchFreqTblEntU = {
   {PSCFG_MAXFREQ, UDIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_CZ, AMD_F15_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY},
   sizeof (MaxStretchFreqCZUDIMM) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxStretchFreqCZUDIMM
};

// Stretch Max Freq. for normal SODIMM configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC PSCFG_MAXFREQ_ENTRY ROMDATA MaxStretchFreqCZSODIMM[] = {
  {_1DIMM, 1, 1, 0, 0, DDR2133_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR2133_FREQUENCY, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR1866_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1600_FREQUENCY, DDR1600_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
};

STATIC PSC_TBL_ENTRY CZStretchMaxFreqTblEntSO = {
   {PSCFG_MAXFREQ, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_CZ, AMD_F15_ALL}, PT_DONT_CARE, DDR3_TECHNOLOGY},
   sizeof (MaxStretchFreqCZSODIMM) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxStretchFreqCZSODIMM
};

// DDR4 Stretch Max Freq. for normal SODIMM configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_2V,   Speed_TBD1,   Speed_TBD2
//
STATIC PSCFG_MAXFREQ_ENTRY ROMDATA MaxStretchFreqCZSODIMMFP4D4[] = {
  {_1DIMM, 1, 1, 0, 0, DDR2400_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR2400_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
};

STATIC PSC_TBL_ENTRY CZStretchMaxFreqTblEntSOFP4D4 = {
   {PSCFG_MAXFREQ, SODIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_CZ, AMD_F15_ALL}, CZ_SOCKET_FP4, DDR4_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (MaxStretchFreqCZSODIMMFP4D4) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxStretchFreqCZSODIMMFP4D4
};

// DDR4 Stretch Max Freq. for UDIMM configuration
// Format :
// DimmPerCh,   Dimms,   SR,   DR,   QR,   Speed1_5V,   Speed1_35V,   Speed1_25V
//
STATIC PSCFG_MAXFREQ_ENTRY ROMDATA MaxStretchFreqCZUDIMMFP4D4[] = {
  // DimmPerCh, Dimms, SR, DR, Speed1_2V
  {_1DIMM, 1, 1, 0, 0, DDR2400_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_1DIMM, 1, 0, 1, 0, DDR2400_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 1, 0, 0, DDR2400_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 1, 0, 1, 0, DDR2133_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 2, 0, 0, DDR2133_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 1, 1, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
  {_2DIMM, 2, 0, 2, 0, DDR1866_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY, UNSUPPORTED_DDR_FREQUENCY},
};
STATIC PSC_TBL_ENTRY CZStretchMaxFreqTblEntUFP4D4 = {
   {PSCFG_MAXFREQ, UDIMM_TYPE, NOD_DONT_CARE, {AMD_FAMILY_15_CZ, AMD_F15_ALL}, CZ_SOCKET_FP4, DDR4_TECHNOLOGY, MBL_DONT_CARE, MBP_DONT_CARE},
   sizeof (MaxStretchFreqCZUDIMMFP4D4) / sizeof (PSCFG_MAXFREQ_ENTRY),
   (VOID *)&MaxStretchFreqCZUDIMMFP4D4
};

STATIC PSC_TBL_ENTRY* memPSCTblMaxStretchFreqArrayCZ[] = {
  &CZMaxStretchFreqTblEntU,
  &CZStretchMaxFreqTblEntSO,
  &CZStretchMaxFreqTblEntSOFP4D4,
  &CZStretchMaxFreqTblEntUFP4D4,
  NULL
};

/*----------------------------------------------------------------------------------------
 *                             P R O C E S S O R     F E A T U R E
 *----------------------------------------------------------------------------------------
 */
/**
 *  IDS Backend Function for RdRand for F15
 *
 *
 *  Parameters:
 * @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 * @param[in,out]   StdHeader        AMD standard header config param
 * @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsIntFeatRdRandF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT32 MsrAddress;
  UINT64 MsrValue;
  IDS_STATUS NvValue;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDRAND, IdsNvPtr, StdHeader) {
    if (NvValue == 1) {
      // Disable RdRand
      MsrAddress = 0xC0011004;
      LibAmdMsrRead (MsrAddress, &MsrValue, StdHeader);
      MsrValue &= (~BIT62);
      LibAmdMsrWrite (MsrAddress, &MsrValue, StdHeader);
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for watchdog timer and sync flood MCA for F15
 *
 *
 *  Parameters:
 * @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 * @param[in,out]   StdHeader        AMD standard header config param
 * @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsIntFeatWatchdogTimerAndSyncFloodMcaF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT32 PciVar32;
  UINT32 MsrAddress;
  UINT64 MsrValue;
  PCI_ADDR PciAddr;
  IDS_STATUS NvValue;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_WATCHDOG_TIMER, IdsNvPtr, StdHeader) {
    if (IsBsp (StdHeader)) {
      PciAddr.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_3, 0x44);
      LibAmdPciRead (AccessWidth32, PciAddr, &PciVar32, StdHeader);
      if (NvValue == 1) {
        // Set WDTDis to 0x1
        PciVar32 |= (1 << 8);
        // Set WDTBaseSel to 0x11, as means WDT disabled
        PciVar32 |= (3 << 12);
      } else if (NvValue == 0) {
        // Clear WDTDis to 0
        PciVar32 &= ~(UINT32) (1 << 8);
        // Set WDTBaseSel to Reset Value (0)
        PciVar32 &= ~(UINT32) (3 << 12);
      }
      LibAmdPciWrite (AccessWidth32, PciAddr, &PciVar32, StdHeader);
    }

    MsrAddress = 0xC0010074;
    LibAmdMsrRead (MsrAddress, &MsrValue, StdHeader);
    switch (NvValue) {
    //Enabled
    case  0:
      MsrValue |= BIT0;
      break;
    //Disabled
    case  1:
      MsrValue &= ~BIT0;
      break;
    //Auto
    case  3:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
    LibAmdMsrWrite (MsrAddress, &MsrValue, StdHeader);
  }

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_SYNC_FLOOD_MCA, IdsNvPtr, StdHeader) {
    MsrAddress = 0xC0011000;
    LibAmdMsrRead (MsrAddress, &MsrValue, StdHeader);
    switch (NvValue) {
    //Enabled
    case  0:
      MsrValue &= ~BIT10;
      break;
    //Disabled
    case  1:
      MsrValue |= BIT10;
      break;
    //Auto
    case  3:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
    LibAmdMsrWrite (MsrAddress, &MsrValue, StdHeader);
  }

  return IDS_SUCCESS;

}

/**
 *  IDS Backend Function for exception breakpoint for F15
 *
 *
 *  Parameters:
 * @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 * @param[in,out]   StdHeader        AMD standard header config param
 * @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsIntFeatExceptionBreakpointF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT32 MsrAddress;
  UINT64 MsrValue;
  IDS_STATUS NvValue;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_EXCEPTION_BREAKPOINT_CTRL, IdsNvPtr, StdHeader) {
    if (NvValue == 1) {
      // Enable
      // MSRC001_100A[17] = 1
      MsrAddress = 0xC001100A;
      LibAmdMsrRead (MsrAddress, &MsrValue, StdHeader);
      MsrValue |= BIT17;
      LibAmdMsrWrite (MsrAddress, &MsrValue, StdHeader);

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_EXCEPTION_BREAKPOINT_VECTOR, IdsNvPtr, StdHeader) {
        // MSRC001_1018[8] = 0x1
        // MSRC001_1018[7:0] = input
        MsrAddress = 0xC0011018;
        LibAmdMsrRead (MsrAddress, &MsrValue, StdHeader);
        MsrValue &= ~(0xFF);
        MsrValue |= ((NvValue & 0xFF) | BIT8);
        LibAmdMsrWrite (MsrAddress, &MsrValue, StdHeader);
      }
    }
  }

  return IDS_SUCCESS;

}


/**
 *  IDS Backend Function for core leveling for F15
 *
 *
 *  Parameters:
 * @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 * @param[in,out]   StdHeader        AMD standard header config param
 * @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsIntSubCoreLevelingF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  UINT8 CoreLevelingMode;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  CoreLevelingMode = PlatformConfig->CoreLevelingMode;
  //Core Leveling
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CORE_LEVEL, IdsNvPtr, StdHeader) {
    if ( NvValue != IdsNvCpuGenCoreLevAuto) {
      PlatformConfig->CoreLevelingMode = (UINT8) NvValue;
    }
  }

  //Reserve system memory for CZ ACP engine
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ACP_ENGINE, IdsNvPtr, StdHeader) {
    if ( NvValue != IdsNvBYTEValueAuto) {
      PlatformConfig->AcpSize = (UINT8) NvValue;
    }
  }

  return IDS_SUCCESS;
}

/**
 * Change Pstate
 *
 * @param[in]  StdHeader          Config handle for library and services.
 * @param[in]  Pstate             Switch all cores to this Pstate
 *
 */
VOID
ChangePstateF15Cz (
  IN       VOID *Pstate,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  LibAmdMsrWrite (MSR_PSTATE_CTL, (UINT64*) Pstate, StdHeader);
}

/**
 * This function sets Fid & Vid & Did
 *
 * @param[in]  StdHeader          Config handle for library and services.
 * @param[in]  MsrAddr            Indicate which HW Pstate should be setup.
 *
 */
VOID
SetUnFusedPstateF15Cz (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       UINT32 MsrAddr
  )
{
  UINT64 MsrValue;

  // 1GHz, 1.0V
  LibAmdMsrRead (MsrAddr, &MsrValue, StdHeader);
  ((PSTATE_MSR *) &MsrValue)->PsEnable = 1;
  SetF15CzCpuFid (&MsrValue, 0x4);
  ((PSTATE_MSR *) &MsrValue)->CpuDid = 1;
  ((PSTATE_MSR *) &MsrValue)->CpuVid = 0x58;
  ((PSTATE_MSR *) &MsrValue)->NbPstate = 0;
  LibAmdMsrWrite (MsrAddr, &MsrValue, StdHeader);
}

/**
 * after warm reset, need to re-program P-State registers for unfused.
 *
 * This function sets Fid & Vid & Did and P-State register on the current core for
 * unfused parts.
 *
 * @param[in]  StdHeader          Config handle for library and services.
 * @param[in]  CpuEarlyParamsPtr  Service parameters
 *
 */
VOID
SetUnFusedFidVidRegsF15Cz (
  IN       AMD_CONFIG_PARAMS *StdHeader,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr
  )
{
  UINT32 MsrAddr;
  UINT64 MsrValue;
  PCI_ADDR PciAddress;
  D18F3xDC_STRUCT D18f3xdc;
  F15_CPB_CTRL_REGISTER CpbReg;
  AP_TASK TaskPtr;

  // Get NumBoostStates
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &CpbReg, StdHeader);

  // Set all boost Pstate[PstateEn]=1b
  // Set software P0[PstateEn]=1b
  // Set rest Pstate[PstateEn]=0b
  for (MsrAddr = MSR_PSTATE_0 + CpbReg.NumBoostStates + 1; MsrAddr <= MSR_PSTATE_7; MsrAddr++) {
    LibAmdMsrRead (MsrAddr, &MsrValue, StdHeader);
    ((PSTATE_MSR *) &MsrValue)->PsEnable = 0;
    LibAmdMsrWrite (MsrAddr, &MsrValue, StdHeader);
  }

  // Configure all boost Pstate and software P0[CpuDid and CpuVid] to reflect the default hardware behavior out of cold reset described by
  // D0F0xBC_xD8230F00[CofVidProg]=0b.
  for (MsrAddr = MSR_PSTATE_0; MsrAddr <= MSR_PSTATE_0 + CpbReg.NumBoostStates; MsrAddr++) {
    SetUnFusedPstateF15Cz (StdHeader, MsrAddr);
  }

  // Update PstateMaxVal to NumBoostStates
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_3, D18F3xDC_ADDRESS);
  LibAmdPciRead (AccessWidth32, PciAddress, &D18f3xdc, StdHeader);
  D18f3xdc.Field.PstateMaxVal = CpbReg.NumBoostStates;
  LibAmdPciWrite (AccessWidth32, PciAddress, &D18f3xdc, StdHeader);

  // Transition to P0.
  TaskPtr.FuncAddress.PfApTaskI = ChangePstateF15Cz;
  TaskPtr.DataTransfer.DataSizeInDwords = 2;
  TaskPtr.DataTransfer.DataPtr = &MsrValue;
  TaskPtr.DataTransfer.DataTransferFlags = 0;

  MsrValue = 0;
  ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, CpuEarlyParamsPtr);
}

/**
 *
 * This function performs the "BIOS Requirements for Unfused Parts" section of
 * the BKDG.  This code will run on the BSC.
 *
 *  @param[in]  FamilySpecificServices   - Pointer to CPU_SPECIFIC_SERVICES struct.
 *  @param[in]  CpuEarlyParamsPtr       Service parameters
 *  @param[in]  StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
SetUnFusedPartF15Cz (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  PCI_ADDR                 PciAddress;
  D0F0xBC_xD8230F00_STRUCT D0F0xBC_xD8230F00;
  UINT8                    i;
  NB_PSTATE_REGISTER       NbPstateX;
  NB_PSTATE_CTRL_REGISTER  NbPstateCtrl;
  GNB_HANDLE              *GnbHandle;

  GnbHandle = GnbGetHandle (StdHeader);
  GnbRegisterReadCZ (GnbHandle, D0F0xBC_xD8230F00_TYPE, D0F0xBC_xD8230F00_ADDRESS, &D0F0xBC_xD8230F00.Value, 0, StdHeader);


  if (D0F0xBC_xD8230F00.Field.COF_VID_PROG == 0) {
    //PCI register config
    //Configure NB P0[NbVid, NbDid, NbFid] to reflect the default hardware behavior out of cold reset described by D0F0xBC_xD8230F00[CofVidProg]=0b
    PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_5, 0);
    for (i = 0; i < NM_NB_PS_REG; i++) {
      PciAddress.Address.Register = NB_PSTATE_0 + i * 4;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateX, StdHeader);
      if (i == 0) {
        //NB P0 COF 800Mhz, Voltage 1.0v
        SetF15CzNbFid (&NbPstateX, 0x4);
        NbPstateX.NbDid = 0;
        NbPstateX.MemPstate = 0;
        NbPstateX.NbVid_6_0 = 0x58;
        NbPstateX.NbVid_7 = 0;
        NbPstateX.NbPstateEn = 1;
      } else {
        //Disable other Nb Pstates
        NbPstateX.NbPstateEn = 0;
      }
      LibAmdPciWrite (AccessWidth32, PciAddress, &NbPstateX, StdHeader);
    }

    // Set F5x170[NbPstateMaxVal] = 0
    PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NbPstateCtrl, StdHeader);
    NbPstateCtrl.NbPstateMaxVal = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, (VOID *) &NbPstateCtrl, StdHeader);

    SetUnFusedFidVidRegsF15Cz (StdHeader, CpuEarlyParamsPtr);
  }
}

/**
 *  Processor Power Planes and Voltage Controls for F15
 *
 *  This function will customize Processor Power Planes and Voltage Controls
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubVoltageCtrlF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  PCI_ADDR PciAddress;
  D18F3xD8_STRUCT D18f3xd8;

  //Fast Slam Time Down
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_FASTSLAMTIMEDOWN, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvCpuPwrFastSlamTimeDownDisabled:
    //Enabled
    case  IdsNvCpuPwrFastSlamTimeDownEnabled:
      PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_3, D18F3xD8_ADDRESS);
      LibAmdPciRead (AccessWidth32, PciAddress, &D18f3xd8, StdHeader);
      D18f3xd8.Field.SlamModeSelect = ((NvValue == IdsNvCpuPwrFastSlamTimeDownDisabled) ? 0 : 1);
      LibAmdPciWrite (AccessWidth32, PciAddress, &D18f3xd8, StdHeader);
      break;
    //Auto
    case  IdsNvCpuPwrFastSlamTimeDownAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  return IDS_SUCCESS;
}
/**
 *  Program and Enable P-State Register.
 *
 *  @param[in]     pstate       The P-State Register.
 *  @param[in]     pstatereg    The Pointer of P-State Setting.
 *  @param[in,out] StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsProgPstatesCommonF15Cz (
  IN       UINT8 pstate,
  IN       PSTATE_REG *pstatereg,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT64 MsrValue;
  PSTATE_MSR * PMsrValue;
  PMsrValue = (PSTATE_MSR *) &MsrValue;
  LibAmdMsrRead ((MSR_PSTATE_0 + pstate), (UINT64 *) PMsrValue, StdHeader);
  PMsrValue->CpuDid = pstatereg->coredid;
  SetF15CzCpuFid (PMsrValue, pstatereg->corefid);
  PMsrValue->CpuVid = pstatereg->corevid;
  PMsrValue->PsEnable = 1;
  LibAmdMsrWrite ((MSR_PSTATE_0 + pstate), (UINT64 *) PMsrValue, StdHeader);
}

/**
 *  Switch P-State.
 *
 *  @param[in]     dataptr        The Pointer of CUSTOM_PSTATE_STRUCT.
 *  @param[in,out] StdHeader      The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsSetCustomPstatesCommonCoreF15Cz (
  IN       VOID *dataptr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT8 PsNum;
  UINT8 BPsNum;
  UINT8 i;
  UINT8 CurPstate;
  UINT64 MsrValue;
  UINT32 MaxPstates;
  PCI_ADDR  PciAddress;
  D18F4x15C_STRUCT D18f4x15c;
  D18F3xDC_STRUCT D18f3xdc;
  CUSTOM_PSTATE_STRUCT *pcustompstate;
  CPU_SPECIFIC_SERVICES   *FamilySpecificServices;

  //Below doesn't contain requirement check of P state register
  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  pcustompstate = (CUSTOM_PSTATE_STRUCT *) dataptr;
  BPsNum = pcustompstate->bpsnum;
  PsNum = pcustompstate->psnum;
  //Make sure Pstate number less equal than MaxPstate, & large than 0
  ASSERT ((PsNum > 0) && (PsNum <= NM_PS_REG));
  ASSERT (PsNum > BPsNum);

  //Set Boost Pstate Number
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_4, D18F4x15C_ADDRESS);
  LibAmdPciRead (AccessWidth32, PciAddress, &D18f4x15c, StdHeader);
  D18f4x15c.Field.NumBoostStates = BPsNum;
  LibAmdPciWrite (AccessWidth32, PciAddress, &D18f4x15c, StdHeader);

  for (i = 0; i < PsNum; i++) {
    IdsProgPstatesCommonF15Cz (i, &(pcustompstate->pstatereg[i]), StdHeader);
  }

  MaxPstates = NM_PS_REG - 1;

  // Disable other un-set pstates
  for (i = (UINT8)PsNum; i <= MaxPstates; i++) {
    LibAmdMsrRead ((MSR_PSTATE_0 + i), &MsrValue, StdHeader);
    ((PSTATE_MSR *)&MsrValue)->PsEnable = 0;
    LibAmdMsrWrite ((MSR_PSTATE_0 + i), &MsrValue, StdHeader);
  }

  //Number of Pstates user request, record current P states
  LibAmdMsrRead (MSR_PSTATE_STS, &MsrValue, StdHeader);
  CurPstate = (UINT8) ((PSTATE_STS_MSR*) &MsrValue)->CurPstate;

  PciAddress.Address.Function = FUNC_3;
  PciAddress.Address.Register = D18F3xDC_ADDRESS;
  LibAmdPciRead (AccessWidth32, PciAddress, &D18f3xdc, StdHeader);
  // Translate to last restore P-state
  //Only one Pstate programed
  if ((PsNum - BPsNum) == 1) {
    //Copy fake new pstate to P1
    LibAmdMsrRead ((MSR_PSTATE_0 + BPsNum), &MsrValue, StdHeader);
    LibAmdMsrWrite ((MSR_PSTATE_0 + BPsNum + 1), &MsrValue, StdHeader);
    D18f3xdc.Field.PstateMaxVal = PsNum;
    LibAmdPciWrite (AccessWidth32, PciAddress, &D18f3xdc, StdHeader);
    if (CurPstate != 1) {
      FamilySpecificServices->TransitionPstate (FamilySpecificServices, 1, 0, StdHeader);
    }
    FamilySpecificServices->TransitionPstate (FamilySpecificServices, 0, 0, StdHeader);
    ((PSTATE_MSR *)&MsrValue)->PsEnable = 0;
    LibAmdMsrWrite ((MSR_PSTATE_0 + BPsNum + 1), &MsrValue, StdHeader);
    D18f3xdc.Field.PstateMaxVal = PsNum - 1;
    LibAmdPciWrite (AccessWidth32, PciAddress, &D18f3xdc, StdHeader);
  } else {
    //Check does the custom p-state contain curpstate
    CurPstate = CurPstate > (PsNum - BPsNum - 1)? (PsNum - BPsNum - 1) : CurPstate;
    //Update the F3xDC[PstateMaxVal]
    D18f3xdc.Field.PstateMaxVal = PsNum - 1;
    LibAmdPciWrite (AccessWidth32, PciAddress, &D18f3xdc, StdHeader);
  // if Current Pstate is not the highest Pstate
    if (CurPstate != (PsNum - BPsNum - 1)) {
      FamilySpecificServices->TransitionPstate (FamilySpecificServices, CurPstate + 1, 0, StdHeader);
      FamilySpecificServices->TransitionPstate (FamilySpecificServices, CurPstate, 0, StdHeader);
    } else {
      FamilySpecificServices->TransitionPstate (FamilySpecificServices, CurPstate - 1, 0, StdHeader);
      FamilySpecificServices->TransitionPstate (FamilySpecificServices, CurPstate, 0, StdHeader);
    }
  }
}


/**
 *  P_State Override Feature for F15
 *
 *  This function will customize the P-States in all cores.
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubPstateOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS idsvalue;
  IDS_STATUS psnum;
  IDS_STATUS bpsnum;
  PCI_ADDR  PciAddress;
  D18F4x15C_STRUCT D18f4x15c;
  UINT8 i;
  AP_TASK       TaskPtr;
  CUSTOM_PSTATE_STRUCT custompstate;
  AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr;

  CpuEarlyParamsPtr = (AMD_CPU_EARLY_PARAMS *) DataPtr;
  LibAmdMemFill (&custompstate, 0, sizeof (CUSTOM_PSTATE_STRUCT), StdHeader);

  // Custom Boost P_States
  IDS_NV_READ_SKIP (bpsnum, AGESA_IDS_NV_CPB_NUM_BOOST, IdsNvPtr, StdHeader) {
    PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_4, D18F4x15C_ADDRESS);
    LibAmdPciRead (AccessWidth32, PciAddress, &D18f4x15c, StdHeader);

    if (bpsnum != IdsNvBYTEValueAuto) {
      //Set Boost Pstate Number
      D18f4x15c.Field.NumBoostStates = bpsnum;
      LibAmdPciWrite (AccessWidth32, PciAddress, &D18f4x15c, StdHeader);
    } else {
      bpsnum = D18f4x15c.Field.NumBoostStates;
    }
    custompstate.bpsnum = (UINT8) bpsnum;
  }

  // Custom P_States Enable
  IDS_NV_READ_SKIP (idsvalue, AGESA_IDS_NV_CUSTOM_PSTATE, IdsNvPtr, StdHeader) {
    if (idsvalue == IdsNvCpuPstCustomPStateEnabled) {
      // Custom P_States Enable
      // Collect custom pstates info for core routine
      IDS_NV_READ_SKIP (psnum, AGESA_IDS_NV_NUMCUSTOMPSTATES, IdsNvPtr, StdHeader) {
        ASSERT (
          (psnum >= IdsNvCpuPstNumPState1) &&
          (psnum <= IdsNvCpuPstNumPState8)
        );
        custompstate.psnum = (UINT8) psnum;
        for (i = 0; i < custompstate.psnum; i++) {
          custompstate.pstatereg[i].corefid = (UINT8) GETPXCOREFID (i, IdsNvPtr, StdHeader);
          custompstate.pstatereg[i].corevid = (UINT8) GETPXCOREVID (i, IdsNvPtr, StdHeader);
          custompstate.pstatereg[i].coredid = (UINT8) GETPXCOREDID (i, IdsNvPtr, StdHeader);
          ASSERT (custompstate.pstatereg[i].corefid <= IdsNvCpuPstSta0CoreFidMax);
          ASSERT (custompstate.pstatereg[i].corevid <= IdsNvCpuPstSta0CoreVidMax);
          ASSERT (
            custompstate.pstatereg[i].coredid == IdsNvCpuPstSta0CoreDidDivide_by_1 ||
            custompstate.pstatereg[i].coredid == IdsNvCpuPstSta0CoreDidDivide_by_2 ||
            custompstate.pstatereg[i].coredid == IdsNvCpuPstSta0CoreDidDivide_by_4 ||
            custompstate.pstatereg[i].coredid == IdsNvCpuPstSta0CoreDidDivide_by_8 ||
            custompstate.pstatereg[i].coredid == IdsNvCpuPstSta0CoreDidDivide_by_16
          );
        }
        TaskPtr.FuncAddress.PfApTaskI = IdsSetCustomPstatesCommonCoreF15Cz;
        TaskPtr.ExeFlags = WAIT_FOR_CORE;
        TaskPtr.DataTransfer.DataSizeInDwords = SIZE_IN_DWORDS (CUSTOM_PSTATE_STRUCT);
        TaskPtr.DataTransfer.DataPtr = &custompstate;
        TaskPtr.DataTransfer.DataTransferFlags = 0;
        ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, CpuEarlyParamsPtr);
      }
    }
  }
  return IDS_SUCCESS;
}
/**
 *  NB P_State Override Feature for F15
 *
 *  This function will customize the P-States in all cores.
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubNbPstateDisOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  BOOLEAN *SkipHwCfg;
  SkipHwCfg = (BOOLEAN *) DataPtr;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CUSTOM_NB_PSTATE, IdsNvPtr, StdHeader) {
    if (NvValue == IdsNvCpuCustomNBPstatesManual) {
      *SkipHwCfg = TRUE;
    }
  }
  return IDS_SUCCESS;
}
/**
 *  Get Min Nb Freq when Custom NB Pstate is enable
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubNbPstateGetMinFreqCz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT32 *PMinFreqInMHz;
  IDS_STATUS NvValue;
  UINT32 MinFreqInMHz;
  UINT32 TmpFreqInMHz;
  UINT32 i;
  UINT32 FreqNumerator;
  UINT32 FreqDivisor;

  PMinFreqInMHz = (UINT32 *)DataPtr;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CUSTOM_NB_PSTATE, IdsNvPtr, StdHeader) {
    if (NvValue == IdsNvCpuCustomNBPstatesManual) {
      MinFreqInMHz = 0xFFFFFFFF;
      for (i = 0; i < 4; i++) {
        ASSERT (GETPXNBFID (i, IdsNvPtr, StdHeader) != IDS_UNSUPPORTED);
        ASSERT (GETPXNBDID (i, IdsNvPtr, StdHeader) != IDS_UNSUPPORTED);
        FreqNumerator = ((GETPXNBFID (i, IdsNvPtr, StdHeader) + 4) * 100);
        FreqDivisor   = (1 << (GETPXNBDID (i, IdsNvPtr, StdHeader)));
        TmpFreqInMHz = (FreqNumerator / FreqDivisor);
        MinFreqInMHz = ((TmpFreqInMHz < MinFreqInMHz) ? TmpFreqInMHz : MinFreqInMHz);
      }
      *PMinFreqInMHz = MinFreqInMHz;
      IDS_HDT_CONSOLE (IDS_TRACE,"IDS Override Min NB Freq %d Mhz\n",MinFreqInMHz);
    }
  }
  return IDS_SUCCESS;
}

/**
 *  P_State Override Feature for F15
 *
 *  This function will customize the P-States in all cores.
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubNbPstateOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  PCI_ADDR PciAddress;
  UINT8 i;
  AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr;
  NB_PSTATE_STS_REGISTER NbPsStsReg;
  NB_PSTATE_CTRL_REGISTER NbPsCtrlReg;
  NB_PSTATE_REGISTER NbPsReg;
  NB_CAPS_REGISTER NbCapsReg;
  IDS_STATUS LcMode;

  CpuEarlyParamsPtr = (AMD_CPU_EARLY_PARAMS *) DataPtr;

  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrlReg, StdHeader);

  //Memory P-state Disable
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_MEMPSTATEDIS, IdsNvPtr, StdHeader) {
    // Disable MemPstate when load card support is enabled
    IDS_NV_READ_SKIP (LcMode, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
      if (LcMode != 0) {
        NvValue = IdsNvCpuPstMemPstateDis1;
      }
    }
    switch (NvValue) {
    case  IdsNvCpuPstMemPstateDis0:
    case  IdsNvCpuPstMemPstateDis1:
      PciAddress.Address.Function = FUNC_3;
      PciAddress.Address.Register = NB_CAPS_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbCapsReg, StdHeader);
      if (NbCapsReg.MemPstateCap == 1) {
        NbPsCtrlReg.MemPstateDis = NvValue;
      } else {
        IDS_HDT_CONSOLE (IDS_TRACE,"MemPstateDis is Read-only.\n");
      }
      break;
    //Auto
    case  IdsNvCpuPstMemPstateDisAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //Software NB P-state low disable
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPUPSTSWNBPSTATELODIS, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case  IdsNvCpuPstSwNbPstateLoDis0:
    case  IdsNvCpuPstSwNbPstateLoDis1:
      PciAddress.Address.Function = FUNC_5;
      PciAddress.Address.Register = NB_PSTATE_STATUS;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbPsStsReg, StdHeader);
      if (NbPsStsReg.NbPstateDis == 0) {
        NbPsCtrlReg.SwNbPstateLoDis = NvValue;
      } else {
        IDS_HDT_CONSOLE (IDS_TRACE,"SwNbPstateLoDis is Read-only.\n");
      }
      break;
    //Auto
    case  IdsNvCpuPstSwNbPstateLoDisAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrlReg, StdHeader);

  //Config NB_PSTATE_X Memory P-state
  for (i = 0; i < 4; i++) {
    PciAddress.Address.Register = NB_PSTATE_0 + i * 4;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsReg, StdHeader);
    //NB P-State X Memory P-state
    IDS_NV_READ_SKIP (NvValue, (UINT16) (AGESA_IDS_NV_CPUNBPS0MEMPSTATE + i), IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //0, 1
      case  IdsNvCpuNBPS0MemPstate0:
      case  IdsNvCpuNBPS0MemPstate1:
        NbPsReg.MemPstate = NvValue;
        LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsReg, StdHeader);
        break;
      //Auto
      case  IdsNvCpuNBPS0MemPstateAuto:
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }
  }

  //Check NB pstate mode,Disable Auto Or Manual
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CUSTOM_NB_PSTATE, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvCpuCustomNBPstatesDisabled:
      //If Disable only set D18F5x170[NbPstateMaxVal] to 0, F15IsNbPstateEnabled use this bit to set Nb pstate diable
      PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrlReg, StdHeader);

      PciAddress.Address.Register = NB_PSTATE_STATUS;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbPsStsReg, StdHeader);
      if (NbPsStsReg.NbPstateDis == 0) {
        NbPsCtrlReg.NbPstateMaxVal = 0;
        NbPsCtrlReg.SwNbPstateLoDis = 1;
        PciAddress.Address.Register = NB_PSTATE_CTRL;
        LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrlReg, StdHeader);
      }
      break;

    case IdsNvCpuCustomNBPstatesManual:
      PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrlReg, StdHeader);
      //Set NbPstateMaxVal to 3
      NbPsCtrlReg.NbPstateMaxVal = 3;
      LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrlReg, StdHeader);

      //Config NB_PSTATE_X
      for (i = 0; i < 4; i++) {
        PciAddress.Address.Register = NB_PSTATE_0 + i * 4;
        ASSERT ((GETPXNBFID (i, IdsNvPtr, StdHeader) >= IdsNvCpuNBPS0FidMin) &&
                (GETPXNBFID (i, IdsNvPtr, StdHeader) <= IdsNvCpuNBPS0FidMax));
        ASSERT ((GETPXNBVID (i, IdsNvPtr, StdHeader) >= IdsNvCpuNBPS0VidMin) &&
                (GETPXNBVID (i, IdsNvPtr, StdHeader) <= IdsNvCpuNBPS0VidMax));
        ASSERT ((GETPXNBDID (i, IdsNvPtr, StdHeader) == IdsNvCpuNBPS0DidDivide_by_1) ||
                (GETPXNBDID (i, IdsNvPtr, StdHeader) == IdsNvCpuNBPS0DidDivide_by_2));
        LibAmdPciRead (AccessWidth32, PciAddress, &NbPsReg, StdHeader);
        SetF15CzNbFid (&NbPsReg, GETPXNBFID (i, IdsNvPtr, StdHeader));
        NbPsReg.NbVid_6_0 = GETPXNBVID (i, IdsNvPtr, StdHeader) & 0x7F;
        NbPsReg.NbVid_7 = (GETPXNBVID (i, IdsNvPtr, StdHeader) & 0x80) >> 7;
        NbPsReg.NbDid = GETPXNBDID (i, IdsNvPtr, StdHeader);
        NbPsReg.NbPstateEn = 1;
        LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsReg, StdHeader);
      }
      break;

    case IdsNvCpuCustomNBPstatesAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  DeadLockWidgetDis on each core
 *
 *  @param[in]     dataptr        The Pointer of DeadLockWidgetDis value
 *  @param[in,out] StdHeader      The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsProgDeadLockWidgetDisCommonCoreF15Cz (
  IN       VOID *dataptr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT64 *PMsr;
  UINT64 TmpU64;

  PMsr = (UINT64 *)dataptr;
  //MSRC001_0044 LS Machine Check Control Mask (MC0_CTL_MASK)
  LibAmdMsrRead (0xC0010044, &TmpU64, StdHeader);
  //BIT 8 DeadLock: deadlock detection.
  TmpU64 &= ~BIT8;
  TmpU64 |= *PMsr << 8;
  LibAmdMsrWrite (0xC0010044, &TmpU64, StdHeader);

}
/**
 *  Program DeadLockWidgetDis for F15
 *
 *  This function will set DeadLockWidgetDis on local cores
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *
 **/
IDS_STATUS
IdsIntSubProgDeadLockWidgetDisF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AP_TASK       TaskPtr;
  AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr;
  UINT32 NvValue;
  UINT64 MsrValue;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DEADLOCK_WIDGET_DIS, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IDS_NV_DEADLOCK_WIDGET_DIS_0:
    case IDS_NV_DEADLOCK_WIDGET_DIS_1:
      MsrValue = NvValue;
      CpuEarlyParamsPtr = (AMD_CPU_EARLY_PARAMS *) DataPtr;
      TaskPtr.FuncAddress.PfApTaskI = IdsProgDeadLockWidgetDisCommonCoreF15Cz;
      TaskPtr.ExeFlags = WAIT_FOR_CORE;
      TaskPtr.DataTransfer.DataSizeInDwords = SIZE_IN_DWORDS (UINT64);
      TaskPtr.DataTransfer.DataPtr = &MsrValue;
      TaskPtr.DataTransfer.DataTransferFlags = 0;
      ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, CpuEarlyParamsPtr);
      break;
    case IDS_NV_DEADLOCK_WIDGET_DIS_AUTO:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}

VOID
STATIC
IdsProgDsmLpDisCommonCoreF15Cz (
  IN       VOID *dataptr,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT64                LocalMsrRegister;
  UINT64                *NvValue;

  NvValue = (UINT64 *)dataptr;

  if (IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    LibAmdMsrRead (MSR_C001_1040, &LocalMsrRegister, StdHeader);
    LocalMsrRegister &= ~BIT51;
    LocalMsrRegister |= *NvValue << 51;
    LibAmdMsrWrite (MSR_C001_1040, &LocalMsrRegister, StdHeader);
  }
}

/**
 *  Write Gnb DSM Register
 *
 *  This function will write 64-bit data to a GNB DSM register
 *
 *  @param[in]     GnbHandle        The Pointer to the GNB_HANDLE
 *  @param[in]     RegisterAddress  The index address of the register to be written.
 *  @param[in]     RegisterData     The data to be written to the register
 *  @param[in,out] StdHeader        The Pointer of AGESA Header.
 *
 *
 **/
VOID
STATIC
IdsGnbDsmLpRegWriteCz (
  IN       GNB_HANDLE *GnbHandle,
  IN       UINT32 RegisterAddress,
  IN       UINT64 RegisterData,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
/// @todo
//  GMMx190_STRUCT        GMMx190;
//  GMMx194_STRUCT        GMMx194;
//  UINT32                DsmData;
//
//  GnbRegisterReadCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194, 0, StdHeader);
//  GMMx194.Field.DSM_SPR_WREN_D4 = 0;
//  GMMx194.Field.DSM_SPR_WREN_D5 = 0;
//  GMMx194.Field.DSM_SPR_RDEN_D5 = 0;
//  GMMx194.Field.DBG_EN = 1;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GnbRegisterReadCZ (GnbHandle, GMMx190_TYPE, GMMx190_ADDRESS, &GMMx190, 0, StdHeader);
//  GMMx190.Field.DRB_SEL = 0;
//  GMMx190.Field.DSM_SEL = 1;
//  GMMx190.Field.ADDR = RegisterAddress;
//  GnbRegisterWriteCZ (GnbHandle, GMMx190_TYPE, GMMx190_ADDRESS, &GMMx190.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  DsmData = (UINT32) ((RegisterData >> 32) & 0xFFFFFFFF);
//  GnbRegisterWriteCZ (GnbHandle, GMMx198_TYPE, GMMx198_ADDRESS, &DsmData, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GMMx194.Field.DSM_SPR_WREN_D5 = 1;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GMMx194.Field.DSM_SPR_WREN_D5 = 0;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  DsmData = (UINT32) (RegisterData & 0xFFFFFFFF);
//  GnbRegisterWriteCZ (GnbHandle, GMMx198_TYPE, GMMx198_ADDRESS, &DsmData, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GMMx194.Field.DSM_SPR_WREN_D4 = 1;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GMMx194.Field.DSM_SPR_WREN_D4 = 0;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  DsmData = 0;
//  GnbRegisterWriteCZ (GnbHandle, GMMx198_TYPE, GMMx198_ADDRESS, &DsmData, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
}

/**
 *  Read Gnb DSM Register
 *
 *  This function will read 64-bit data from a GNB DSM register
 *
 *  @param[in]     GnbHandle        The Pointer to the GNB_HANDLE
 *  @param[in]     RegisterAddress  The index address of the register to be written.
 *  @param[in,out] RegisterData     The Pointer to date read from the register
 *  @param[in,out] StdHeader        The Pointer of AGESA Header.
 *
 *
 **/
VOID
STATIC
IdsGnbDsmLpRegReadCz (
  IN       GNB_HANDLE *GnbHandle,
  IN       UINT32 RegisterAddress,
  IN       UINT64 *RegisterData,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
/// @todo
//  GMMx190_STRUCT        GMMx190;
//  GMMx194_STRUCT        GMMx194;
//  UINT32                DsmData;
//
//  *RegisterData = 0x0ull;
//
//  GnbRegisterReadCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194, 0, StdHeader);
//  GMMx194.Field.DSM_SPR_WREN_D4 = 0;
//  GMMx194.Field.DSM_SPR_WREN_D5 = 0;
//  GMMx194.Field.DSM_SPR_RDEN_D5 = 0;
//  GMMx194.Field.DBG_EN = 1;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GnbRegisterReadCZ (GnbHandle, GMMx190_TYPE, GMMx190_ADDRESS, &GMMx190, 0, StdHeader);
//  GMMx190.Field.DRB_SEL = 0;
//  GMMx190.Field.DSM_SEL = 1;
//  GMMx190.Field.ADDR = RegisterAddress;
//  GnbRegisterWriteCZ (GnbHandle, GMMx190_TYPE, GMMx190_ADDRESS, &GMMx190.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GnbRegisterReadCZ (GnbHandle, GMMx198_TYPE, GMMx198_ADDRESS, &DsmData, 0, StdHeader);
//  *RegisterData = (UINT64) DsmData;
//
//  GMMx194.Field.DSM_SPR_RDEN_D5 = 1;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
//
//  GnbRegisterReadCZ (GnbHandle, GMMx198_TYPE, GMMx198_ADDRESS, &DsmData, 0, StdHeader);
//  *RegisterData = ((UINT64) DsmData) << 32;
//
//  GMMx194.Field.DSM_SPR_RDEN_D5 = 0;
//  GnbRegisterWriteCZ (GnbHandle, GMMx194_TYPE, GMMx194_ADDRESS, &GMMx194.Value, GNB_REG_ACC_FLAG_S3SAVE, StdHeader);
}

/**
 *  Program Dsm mode
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/

IDS_STATUS
IdsIntSubDsmLpCfgF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AP_TASK                 TaskPtr;
  AMD_CPU_EARLY_PARAMS    *CpuEarlyParamsPtr;
  UINT32                  NvValue;
  PCI_ADDR                PciAddress;
  D18F5x130_STRUCT        D18F5x130;
  UINT32                  D18F5x138;
  UINT64                  DsmData;
  GNB_HANDLE              *GnbHandle;


  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DSM_LP_SEL_EN, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvDsmLpDisabled:
    case IdsNvDsmLpEnabled:
      CpuEarlyParamsPtr = (AMD_CPU_EARLY_PARAMS *) DataPtr;
      TaskPtr.FuncAddress.PfApTaskI = IdsProgDsmLpDisCommonCoreF15Cz;
      TaskPtr.ExeFlags = WAIT_FOR_CORE;
      TaskPtr.DataTransfer.DataSizeInDwords = SIZE_IN_DWORDS (UINT32);
      TaskPtr.DataTransfer.DataPtr = &NvValue;
      TaskPtr.DataTransfer.DataTransferFlags = 0;
      ApUtilRunCodeOnAllLocalCoresAtEarly (&TaskPtr, StdHeader, CpuEarlyParamsPtr);

      PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_5, D18F5x130_ADDRESS);
      D18F5x130.Field.Offset = 0;
      D18F5x130.Field.RegRd = 1;
      LibAmdPciWrite (AccessWidth32, PciAddress, &D18F5x130, StdHeader);
      PciAddress.Address.Register = D18F5x138_ADDRESS;
      LibAmdPciRead (AccessWidth32, PciAddress, &D18F5x138, StdHeader);
      D18F5x138 &= ~BIT19;                                  /// 51 - 32
      D18F5x138 |= NvValue << (51 - 32);
      PciAddress.Address.Register = D18F5x130_ADDRESS;
      D18F5x130.Field.RegRd = 0;
      LibAmdPciWrite (AccessWidth32, PciAddress, &D18F5x130, StdHeader);
      PciAddress.Address.Register = D18F5x138_ADDRESS;
      LibAmdPciWrite (AccessWidth32, PciAddress, &D18F5x138, StdHeader);

      GnbHandle = GnbGetHandle (StdHeader);
      IdsGnbDsmLpRegReadCz (GnbHandle, 0, &DsmData, StdHeader);
      DsmData &= ~BIT51;
      DsmData |= ((UINT64) NvValue) << 51;
      IdsGnbDsmLpRegWriteCz (GnbHandle, 0, DsmData, StdHeader);

      break;
    case IdsNvDsmLpAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *
 * This function performs IDS initial step of PM, such as config for unfused part
 * This code will run on the BSC.
 *
 *  @param[in]  FamilySpecificServices   - Pointer to CPU_SPECIFIC_SERVICES struct.
 *  @param[in]  CpuEarlyParamsPtr       Service parameters
 *  @param[in]  StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsIntPmPwrInitF15Cz (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{

  IDS_NV_ITEM *IdsNvPtr;

  //Step for unfuse part
  SetUnFusedPartF15Cz (FamilySpecificServices, CpuEarlyParamsPtr, StdHeader);

  AmdGetIdsNvTable (&IdsNvPtr, StdHeader);
  if (IdsNvPtr != NULL) {
    //Program DEADLOCK_WIDGET_DIS before PM init
    IdsIntSubProgDeadLockWidgetDisF15Cz (CpuEarlyParamsPtr, StdHeader, IdsNvPtr);

    IdsIntSubDsmLpCfgF15Cz  (CpuEarlyParamsPtr, StdHeader, IdsNvPtr);
  }
}

/**
 *
 * This function performs IDS Power Management function, such as custom p-state & requirement for unfuse part
 * This code will run on the BSC.
 *
 *  @param[in]  FamilySpecificServices   - Pointer to CPU_SPECIFIC_SERVICES struct.
 *  @param[in]  CpuEarlyParamsPtr       Service parameters
 *  @param[in]  StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *
 *
 **/
VOID
IdsIntPmCustomizeF15Cz (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS  *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_NV_ITEM *IdsNvPtr;

  AmdGetIdsNvTable (&IdsNvPtr, StdHeader);

  if (IdsNvPtr != NULL) {
    //Processor Power Planes and Voltage Controls
    IdsIntSubVoltageCtrlF15Cz (CpuEarlyParamsPtr, StdHeader, IdsNvPtr);
    //Custom P-state
    IdsIntSubPstateOverrideF15Cz (CpuEarlyParamsPtr, StdHeader, IdsNvPtr);
    // Custom NB P-state
    IdsIntSubNbPstateOverrideF15Cz (CpuEarlyParamsPtr, StdHeader, IdsNvPtr);
  }
}

/**
 *  IDS Backend Function for config CPB feature
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubCpbConfigF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  D18F4x15C_STRUCT D18f4x15c;
  D18F4x16C_STRUCT D18f4x16c;
  PCI_ADDR PciAddress;

  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_4, D18F4x15C_ADDRESS);
  LibAmdPciRead (AccessWidth32, PciAddress, &D18f4x15c, StdHeader);
  PciAddress.Address.Register = D18F4x16C_ADDRESS;
  LibAmdPciRead (AccessWidth32, PciAddress, &D18f4x16c, StdHeader);

  //Core Performance Boost Master Enable
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPBMASTEREN, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvCpuPstApmMasterEnDisabled:
      D18f4x15c.Field.ApmMasterEn = 0;
      break;
    //Enabled
    case  IdsNvCpuPstApmMasterEnEnabled:
      D18f4x15c.Field.ApmMasterEn = 1;
      D18f4x15c.Field.BoostSrc = 1;
      break;
    case  IdsNvCpuPstApmMasterEnManual:
      D18f4x15c.Field.ApmMasterEn = 1;
      D18f4x15c.Field.BoostSrc = 1;
      //Number of boosted states
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPB_NUM_BOOST, IdsNvPtr, StdHeader) {
        if (NvValue != 0xFF) {
          // Not 'Auto'
          ASSERT ((NvValue >= IdsNvCpuPstNumBoostStatesMin) && (NvValue <= IdsNvCpuPstNumBoostStatesMax));
          D18f4x15c.Field.NumBoostStates = NvValue;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_C_STATE_BOOSTCTL, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        case IdsNvCpuPstCstateBoostCtlDisabled:
          D18f4x16c.Field.CstateBoost = 0;
          break;
        case IdsNvCpuPstCstateBoostCtlManual:
          //C-state count
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTATECNT, IdsNvPtr, StdHeader) {
            ASSERT ((NvValue >= IdsNvCpuPstCstCntMin) && (NvValue <= IdsNvCpuPstCstCntMax));
            D18f4x16c.Field.CstateCnt = NvValue;
          }
          //CstateBoost
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTATEBOOST, IdsNvPtr, StdHeader) {
            ASSERT ((NvValue >= IdsNvCpuPstCstBoostMin) && (NvValue <= IdsNvCpuPstCstBoostMax));
            D18f4x16c.Field.CstateBoost = NvValue;
          }
          break;
        case IdsNvCpuPstCstateBoostCtlAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      break;
    //Auto
    case  IdsNvCpuPstApmMasterEnAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //APM TDP Control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TDPCTL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvCpuPstTDPCtlDisabled:
      D18f4x16c.Field.TdpLimitDis = 1;
      break;
    //Manual
    case  IdsNvCpuPstTDPCtlManual:
      //NodeTdpLimit Enable
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NODETDPLIMITEN, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Disabled
        case  IdsNvCpuPstNodeTdpLmtEnDisabled:
          D18f4x16c.Field.NodeTdpLimitEn = 0;
          break;
        //Enabled
        case  IdsNvCpuPstNodeTdpLmtEnEnabled:
          D18f4x16c.Field.NodeTdpLimitEn = 1;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      //ApmlSwTdpLimit Enable
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_APMLSWTDPLIMITEN, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Disabled
        case  IdsNvCpuPstApmlSwTdpLmtEnDisabled:
          D18f4x16c.Field.ApmlSwTdpLimitEn = 0;
          break;
        //Enabled
        case  IdsNvCpuPstApmlSwTdpLmtEnEnabled:
          D18f4x16c.Field.ApmlSwTdpLimitEn = 1;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      break;
    //Auto
    case  IdsNvCpuPstTDPCtlAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_4, D18F4x15C_ADDRESS);
  LibAmdPciWrite (AccessWidth32, PciAddress, &D18f4x15c, StdHeader);
  PciAddress.Address.Register = D18F4x16C_ADDRESS;
  LibAmdPciWrite (AccessWidth32, PciAddress, &D18f4x16c, StdHeader);

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Message trigger C state mode and CC6
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsSubCModeF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  PLATFORM_CONFIGURATION *PlatformConfig;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;
  // Override CStateMode
  //Core C6 State
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CC6CTL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvCpuCstCC6EnDisabled:
      PlatformConfig->CStateMode = CStateModeDisabled;
      break;
    //Enabled
    case  IdsNvCpuCstCC6EnEnabled:
      PlatformConfig->CStateMode = CStateModeC6;
      break;
    //Auto
    case  IdsNvCpuCstCC6EnAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  // Disable C6 when load card support is enabled
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_LOAD_CARD_MODE, IdsNvPtr, StdHeader) {
    if (NvValue != 0) {
      PlatformConfig->CStateMode = CStateModeDisabled;
    }
  }

  return IDS_SUCCESS;
}
/**
 *  IDS Backend Function for IO c state control
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 **/
IDS_STATUS
IdsIntSubConfigIoCstateF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS  NvValue;
  UINT32 CstateCtrl1Reg;
  UINT32 CstateCtrl2Reg;
  PCI_ADDR     PciAddress;
  UINT32 NumCst;

  NumCst = 0;
  ////C-state Control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTATECTL, IdsNvPtr, StdHeader) {
    PciAddress.AddressValue = CSTATE_CTRL1_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &CstateCtrl1Reg, StdHeader);
    PciAddress.Address.Register = CSTATE_CTRL2_REG;
    LibAmdPciRead (AccessWidth32, PciAddress, &CstateCtrl2Reg, StdHeader);

    switch (NvValue) {
    case IdsNvCpuCstCtlDisabled:
      CstateCtrl1Reg &= 0;
      CstateCtrl2Reg &= 0xFFFF0000;
      break;
    case IdsNvCpuCstCtlManual:
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NUMCSTATES, IdsNvPtr, StdHeader) {
        NumCst = NvValue;
      }

      //C-state Action1
      if ((NumCst == IdsNvCpuCstNumCStates1_C_State) ||
          (NumCst == IdsNvCpuCstNumCStates2_C_States) ||
          (NumCst == IdsNvCpuCstNumCStates3_C_States)) {
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTACT1, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvCpuCstAct1Min) && (NvValue <= IdsNvCpuCstAct1Max));
          CstateCtrl1Reg &= ~(UINT32) 0x3FFF;
          CstateCtrl1Reg |= NvValue;
        }
      }

      //C-state Action2
      if ((NumCst == IdsNvCpuCstNumCStates2_C_States) ||
          (NumCst == IdsNvCpuCstNumCStates3_C_States)) {
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTACT2, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvCpuCstAct2Min) && (NvValue <= IdsNvCpuCstAct2Max));
          CstateCtrl1Reg &= ~(UINT32) (0x3FFF << 16);
          CstateCtrl1Reg |= (NvValue << 16);
        }
      }
      //C-state Action3
      if ((NumCst == IdsNvCpuCstNumCStates3_C_States)) {
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTACT3, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvCpuCstAct3Min) && (NvValue <= IdsNvCpuCstAct3Max));
          CstateCtrl2Reg &= ~(UINT32) 0x3FFF;
          CstateCtrl2Reg |= NvValue;
        }
      }
       break;
    case IdsNvCpuCstCtlAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }

    PciAddress.Address.Register = CSTATE_CTRL1_REG;
    LibAmdPciWrite (AccessWidth32, PciAddress, &CstateCtrl1Reg, StdHeader);
    PciAddress.Address.Register = CSTATE_CTRL2_REG;
    LibAmdPciWrite (AccessWidth32, PciAddress, &CstateCtrl2Reg, StdHeader);
  }
  return IDS_SUCCESS;
}
/**
 *  IDS Backend Function for Create _CST
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of ACPI_CST_CREATE_INPUT
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     IDS service doesn't take action
 * @retval      AGESA_FALSE     IDS service take action
 *
 **/
IDS_STATUS
IdsIntSubCreateCstF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  IDS_STATUS NumCst;
  BOOLEAN IsSkip;
  IDS_STATUS RetVal;
  IDS_CST_HEADER_STRUCT     *CstHeaderPtr;
  IDS_CST_BODY_STRUCT       *CstBodyPtr;
  IDS_CSD_HEADER_STRUCT       *CsdHeaderPtr;
  IDS_CSD_BODY_STRUCT       *CsdBodyPtr;
  VOID **PstateAcpiBufferPtr;
  UINT8 CstCount;
  UINT64 MsrData;
  UINT8 i;
  BOOLEAN               GenerateCsdObj;
  ACPI_CST_CREATE_INPUT  *CstInputPtr;

  IsSkip = TRUE;
  RetVal = IDS_SUCCESS;
  NumCst = 0;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTATECTL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvCpuCstCtlDisabled:
      IsSkip = FALSE;
      NumCst = IdsNvCpuCstNumCStates1_C_State;   //IDS_NV_AMDNUMCSTATES_1C_STATE will not generate _CST
      break;
    case IdsNvCpuCstCtlManual:
      IsSkip = FALSE;
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NUMCSTATES, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue == IdsNvCpuCstNumCStates1_C_State) ||
                (NvValue == IdsNvCpuCstNumCStates2_C_States) ||
                (NvValue == IdsNvCpuCstNumCStates3_C_States));
        NumCst = NvValue;
      }
      break;
    case IdsNvCpuCstCtlAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  CstInputPtr = (ACPI_CST_CREATE_INPUT *) DataPtr;
  PstateAcpiBufferPtr = CstInputPtr->PstateAcpiBufferPtr;
  if (IsSkip == FALSE) {
    if (NumCst == IdsNvCpuCstNumCStates1_C_State) {
      RetVal = IDS_UNSUPPORTED;
    } else if (NumCst == IdsNvCpuCstNumCStates2_C_States) {
    //Do nothing, CPU code will cover this case
    } else if (NumCst == IdsNvCpuCstNumCStates3_C_States) {
      RetVal = IDS_UNSUPPORTED;
      CstCount = 2;
      // Read from MSR C0010073 to obtain CstateAddr
      LibAmdMsrRead (MSR_CSTATE_ADDRESS, &MsrData, StdHeader);

      // Typecast the pointer
      CstHeaderPtr = (IDS_CST_HEADER_STRUCT *) *PstateAcpiBufferPtr;

      // Set CST Header
      CstHeaderPtr->NameOpcode  = NAME_OPCODE;
      CstHeaderPtr->CstName_a__ = CST_NAME__;
      CstHeaderPtr->CstName_a_C = CST_NAME_C;
      CstHeaderPtr->CstName_a_S = CST_NAME_S;
      CstHeaderPtr->CstName_a_T = CST_NAME_T;
      CstHeaderPtr->PkgOpcode      = PACKAGE_OPCODE;
      //PkgLeadByte := <bit 7-6: ByteData count that follows (0-3)> <bit 5-4: Only used if PkgLength < 63> <bit 3-0: Least significant package length nybble>
      CstHeaderPtr->PkgLength      = (CstCount == 2)? 0x0449: 0x0246;  //length is 0x49
      CstHeaderPtr->PkgElements    = (CstCount == 2)? 3 : 2;
      CstHeaderPtr->BytePrefix     = BYTE_PREFIX_OPCODE;
      CstHeaderPtr->Count          = CstCount;
      // Typecast the pointer
      CstHeaderPtr++;
      CstBodyPtr = (IDS_CST_BODY_STRUCT *) CstHeaderPtr;

      // Set CST Body
      for (i = 0; i < CstCount; i++) {
        CstBodyPtr->PkgOpcode2     = PACKAGE_OPCODE;
        CstBodyPtr->PkgLength2     = CST_PKG_LENGTH;
        CstBodyPtr->PkgElements2   = CST_PKG_ELEMENTS;
        CstBodyPtr->BufferOpcode   = BUFFER_OPCODE;
        CstBodyPtr->BufferLength   = CST_SUBPKG_LENGTH;
        CstBodyPtr->BufferElements = CST_SUBPKG_ELEMENTS;
        CstBodyPtr->BufferOpcode2  = BUFFER_OPCODE;
        CstBodyPtr->GdrOpcode      = GENERIC_REG_DESCRIPTION;
        CstBodyPtr->GdrLength      = CST_GDR_LENGTH;
        CstBodyPtr->AddrSpaceId    = GDR_ASI_SYSTEM_IO;
        CstBodyPtr->RegBitWidth    = 0x08;
        CstBodyPtr->RegBitOffset   = 0x00;
        CstBodyPtr->AddressSize    = GDR_ASZ_BYTE_ACCESS;
        CstBodyPtr->RegisterAddr   = ((CSTATE_ADDRESS_MSR *) &MsrData)->CstateAddr + 1 + i;
        CstBodyPtr->EndTag         = 0x0079;
        CstBodyPtr->BytePrefix2    = BYTE_PREFIX_OPCODE;
        CstBodyPtr->Type           = i + 2;
        CstBodyPtr->WordPrefix     = WORD_PREFIX_OPCODE;
        CstBodyPtr->Latency        = ((i == 0) ? 99 : 999);
        CstBodyPtr->DWordPrefix    = DWORD_PREFIX_OPCODE;
        CstBodyPtr->Power          = 0;

        CstBodyPtr++;
      }
      //Update the pointer
      *PstateAcpiBufferPtr = CstBodyPtr;

    // Check whether CSD object should be generated
      GenerateCsdObj = CstInputPtr->IoCstateServices->IsCsdObjGenerated (CstInputPtr->IoCstateServices, StdHeader);

      if (GenerateCsdObj) {
        CsdHeaderPtr = (IDS_CSD_HEADER_STRUCT *) *PstateAcpiBufferPtr;

        // Set CSD Header
        CsdHeaderPtr->NameOpcode  = NAME_OPCODE;
        CsdHeaderPtr->CsdName_a__ = CST_NAME__;
        CsdHeaderPtr->CsdName_a_C = CST_NAME_C;
        CsdHeaderPtr->CsdName_a_S = CST_NAME_S;
        CsdHeaderPtr->CsdName_a_D = CSD_NAME_D;
        CsdHeaderPtr->PkgOpcode         = PACKAGE_OPCODE;
        CsdHeaderPtr->PkgLength         = 0x38;
        CsdHeaderPtr->PkgElements       = 2;
        CsdHeaderPtr++;
        CsdBodyPtr = (IDS_CSD_BODY_STRUCT *) CsdHeaderPtr;

        // Set CSD Body
        for (i = 0; i < CstCount; i++) {
          CsdBodyPtr->PkgOpcode2        = PACKAGE_OPCODE;
          CsdBodyPtr->PkgLength2        = CSD_BODY_SIZE - 4; // CSD_BODY_SIZE - Package() - Package Opcode
          CsdBodyPtr->PkgElements2      = 6;
          CsdBodyPtr->BytePrefix        = BYTE_PREFIX_OPCODE;
          CsdBodyPtr->NumEntries        = 6;
          CsdBodyPtr->BytePrefix2       = BYTE_PREFIX_OPCODE;
          CsdBodyPtr->Revision          = 0;
          CsdBodyPtr->DWordPrefix       = DWORD_PREFIX_OPCODE;
          CsdBodyPtr->Domain            = (CstInputPtr->LocalApicId & 0xFE) >> 1;
          CsdBodyPtr->DWordPrefix2      = DWORD_PREFIX_OPCODE;
          CsdBodyPtr->CoordType         = CSD_COORD_TYPE_HW_ALL;
          CsdBodyPtr->DWordPrefix3      = DWORD_PREFIX_OPCODE;
          CsdBodyPtr->NumProcessors     = 0x2;
          CsdBodyPtr->DWordPrefix4      = DWORD_PREFIX_OPCODE;
          CsdBodyPtr->Index             = i;

          CsdBodyPtr++;
        }
        // Update the pointer
        *PstateAcpiBufferPtr = CsdBodyPtr;
      }
    } else {
      ASSERT (FALSE);
    }
  }
  return RetVal;
}
/**
 *  IDS Backend Function for get _CST size
 *
 *
 *  Parameters:
 *  @param[in,out]   DataPtr  The Pointer of ACPI_CST_GET_INPUT
 *  @param[in,out]   StdHeader        AMD standard header config param
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     IDS service doesn't take action
 * @retval      AGESA_FALSE     IDS service take action
 *
 **/
IDS_STATUS
IdsIntSubGetCstF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NumCst;
  BOOLEAN IsSkip;
  ACPI_CST_GET_INPUT *CstGetInputPtr;
  IDS_STATUS RetVal;
  IDS_STATUS NvValue;
  UINT32 *CStateAcpiObjSizePtr;
  BOOLEAN               GenerateCsdObj;
  UINT8 CstCount;

  RetVal = IDS_SUCCESS;
  IsSkip = TRUE;
  NumCst = 0;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CSTATECTL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvCpuCstCtlDisabled:
      IsSkip = FALSE;
      NumCst = IdsNvCpuCstNumCStates1_C_State;   //IDS_NV_AMDNUMCSTATES_1C_STATE will not generate _CST
    case IdsNvCpuCstCtlManual:
      IsSkip = FALSE;
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NUMCSTATES, IdsNvPtr, StdHeader) {
        NumCst = NvValue;
      }
      break;
    case IdsNvCpuCstCtlAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  CstGetInputPtr = (ACPI_CST_GET_INPUT *) DataPtr;
  CStateAcpiObjSizePtr = CstGetInputPtr->CStateAcpiObjSizePtr;
  if (IsSkip == FALSE) {
    if (NumCst == IdsNvCpuCstNumCStates1_C_State) {
      *CStateAcpiObjSizePtr = 0;
      RetVal = IDS_UNSUPPORTED;
    } else if (NumCst == IdsNvCpuCstNumCStates2_C_States) {
    //Do nothing, CPU code will cover this case
    } else if (NumCst == IdsNvCpuCstNumCStates3_C_States) {
      CstCount = 2;
      RetVal = IDS_UNSUPPORTED;
      *CStateAcpiObjSizePtr = sizeof (IDS_CST_HEADER_STRUCT) + CstCount * sizeof (IDS_CST_BODY_STRUCT);

      // If CSD Object is generated, add the size of CSD Object to the total size of
      // CState ACPI Object size
      GenerateCsdObj = CstGetInputPtr->IoCstateServices->IsCsdObjGenerated (CstGetInputPtr->IoCstateServices, StdHeader);

      if (GenerateCsdObj) {
        *CStateAcpiObjSizePtr += sizeof (IDS_CSD_HEADER_STRUCT) + CstCount * sizeof (IDS_CSD_BODY_STRUCT);
      }
    }
  }
  return RetVal;
}
/**
 *  IDS Backend Function for IDS Option Hook Point: IDS_CACHE_CONTROL. F15
 *
 *  This function is used to set cache control registers which are CPU specifically.
 *  It includes the following fields now: CacheFlushHltTmr and CacheFlushOnHaltCtl.
 *
 *  @param[in,out]   DataPtr      The Pointer of Data to Override.
 *  @param[in,out]   StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubCacheControlF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  IDS_STATUS CacheFlushSuccessMonitorCtrl;
  PCI_ADDR PciAddress;
  CSTATE_CTRL1_REGISTER CstateCtrl1Reg;
  CSTATE_CTRL2_REGISTER CstateCtrl2Reg;
  F15_CZ_CLK_PWR_TIMING_CTRL2_REGISTER ClkPwrTimingCtrl2Reg;
  CSTATE_POLICY_CTRL1_REGISTER CstatePolicyCtrl1Reg;

  //Cache Flush on Halt Control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CACHEFLUSHHLT, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvCpuCstCacheFlushHltCtrlDisabled:
      //D18F4x118
      PciAddress.AddressValue = CSTATE_CTRL1_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &CstateCtrl1Reg, StdHeader);
      //D18F4x11C
      PciAddress.Address.Register = CSTATE_CTRL2_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &CstateCtrl2Reg, StdHeader);

      CstateCtrl1Reg.CacheFlushEnCstAct0 = 0;
      CstateCtrl1Reg.CacheFlushEnCstAct1 = 0;
      CstateCtrl2Reg.CacheFlushEnCstAct2 = 0;

      //D18F4x11C
      LibAmdPciWrite (AccessWidth32, PciAddress, &CstateCtrl2Reg, StdHeader);
      //D18F4x118
      PciAddress.AddressValue = CSTATE_CTRL1_PCI_ADDR;
      LibAmdPciWrite (AccessWidth32, PciAddress, &CstateCtrl1Reg, StdHeader);
      break;
    //Manual
    case  IdsNvCpuCstCacheFlushHltCtrlManual:
      //D18F3xDC
      PciAddress.AddressValue = CPTC2_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &ClkPwrTimingCtrl2Reg, StdHeader);
      //D18F4x128
      PciAddress.Address.Function = FUNC_4;
      PciAddress.Address.Register = CSTATE_POLICY_CTRL1_REG;
      LibAmdPciRead (AccessWidth32, PciAddress, &CstatePolicyCtrl1Reg, StdHeader);

      //Cache Flush on Halt Clock Timer
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CACHEFLUSHHLTTMR, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvCpuCstCacheFlushHltTmrMin) && (NvValue <= IdsNvCpuCstCacheFlushHltTmrMax));
        ClkPwrTimingCtrl2Reg.CacheFlushOnHaltTmr = NvValue;
      }

      //Cache Flush on Halt Clock divisor
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CACHEFLUSHHLTDIV, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvCpuCstCacheFlushHltDivMin) && (NvValue <= IdsNvCpuCstCacheFlushHltDivMax));
        ClkPwrTimingCtrl2Reg.CacheFlushOnHaltCtl = NvValue;
      }

      //Cache Flush Timer
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CACHEFLUSHTMR, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvCpuCstCacheFlushTmrMin) && (NvValue <= IdsNvCpuCstCacheFlushTmrMax));
        CstatePolicyCtrl1Reg.CacheFlushTmr = NvValue;
      }
      //D18F4x128
      LibAmdPciWrite (AccessWidth32, PciAddress, &CstatePolicyCtrl1Reg, StdHeader);

      //D18F3xDC
      PciAddress.Address.Function = FUNC_3;
      PciAddress.Address.Register = CPTC2_REG;
      LibAmdPciWrite (AccessWidth32, PciAddress, &ClkPwrTimingCtrl2Reg, StdHeader);
      break;
    //Auto
    case  IdsNvCpuCstCacheFlushHltCtrlAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }

    //Cache Flush Success Monitor Control
    IDS_NV_READ_SKIP (CacheFlushSuccessMonitorCtrl, AGESA_IDS_NV_CFSUCCESSMONCTL, IdsNvPtr, StdHeader) {
      PciAddress.AddressValue = CSTATE_POLICY_CTRL1_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &CstatePolicyCtrl1Reg, StdHeader);
      switch (CacheFlushSuccessMonitorCtrl) {
      //Disabled
      case  IdsNvCpuCacheFlushSuccMonCtlDisabled:
        CstatePolicyCtrl1Reg.CacheFlushSucMonThreshold = 0;
        break;
      //Manual
      case  IdsNvCpuCacheFlushSuccMonCtlManual:
        //Cache Flush Success Monitor Threshold
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CACHEFLUSHSUCMONTHRESHOLD, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvCpuCacheFlushSuccMonThresholdMin) && (NvValue <= IdsNvCpuCacheFlushSuccMonThresholdMax));
          CstatePolicyCtrl1Reg.CacheFlushSucMonThreshold = NvValue;
        }
        break;
      //Auto
      case  IdsNvCpuCacheFlushSuccMonCtlAuto:
        break;
      default:
        ASSERT (FALSE);
        break;
      }
      if (CacheFlushSuccessMonitorCtrl == IdsNvCpuCacheFlushSuccMonCtlDisabled ||
          CacheFlushSuccessMonitorCtrl == IdsNvCpuCacheFlushSuccMonCtlManual) {
        LibAmdPciWrite (AccessWidth32, PciAddress, &CstatePolicyCtrl1Reg, StdHeader);
      }
    }
  }

  return IDS_SUCCESS;
}

/*----------------------------------------------------------------------------------------
 *                             M E M O R Y     F E A T U R E
 *----------------------------------------------------------------------------------------
 */
/**
 *  Init some ids internal bit field of memory register table
 *
 *  This function is used to add some reserved bit field to memory table
 *
 *  @param[in,out] DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubInitMemRegTblF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  MEM_NB_BLOCK *NBPtr;
  NBPtr = (MEM_NB_BLOCK *) DataPtr;

  _BF_DEF (NBPtr->NBRegTable, RegDramCtlrMisc2, BFOppWrEnhDis, 23, 23);
  _BF_DEF (NBPtr->NBRegTable, RegDramMRS, BFDramTerm_DDR3, 9, 7);
  _BF_DEF (NBPtr->NBRegTable, RegDramMRS, BFDramTermDyn, 11, 10);
  _BF_DEF (NBPtr->NBRegTable, RegDramConfigHi, BFDisSimulRdWr, 13, 13);
  _BF_DEF (NBPtr->NBRegTable, RegGmcToDctCtl2, BFDisHalfNclkPwrGate, 31, 31);

  return IDS_SUCCESS;
}

/**
 *  DRAM Timing Override for DDR3 Memory
 *
 *  This function is used to fill memory table to override drive strength.
 *  DRAM setting.
 *
 *  @param[in,out] MemTblPtr    The Pointer of Memory Table.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *  @param[in,out] NBPtr        The Pointer of MEM_NB_BLOCK.
 *
 **/
VOID
IdsMemTblDramTimingDdr3F15Cz (
  IN OUT   MEM_TABLE_ALIAS **MemTblPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_STATUS NvValue;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMTIME, IdsNvPtr, StdHeader) {
    ASSERT (
      NvValue == IdsNvMemTimingCtlAuto ||
      NvValue == IdsNvMemTimingCtlManual
    );
    if (NvValue == IdsNvMemTimingCtlManual) {
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTRDDR3, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        case IdsNvMemTimingRdPtrReserved_01b:
        case IdsNvMemTimingRdPtr3_Clk:
        case IdsNvMemTimingRdPtr2_5_Clk:
        case IdsNvMemTimingRdPtr2_Clk:
        case IdsNvMemTimingRdPtr1_5_Clk:
        case IdsNvMemTimingRdPtr1_Clk:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, MTDcts, MTDIMMs, MTBLs, BFRdPtrInit, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFreqChg, MTNodes, MTDcts, MTDIMMs, MTBLs, BFRdPtrInit, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterNbPstateChange, MTNodes, MTDcts, MTDIMMs, MTBLs, BFRdPtrInit, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        case IdsNvMemTimingRdPtrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      //Data transmit FIFO write delay
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DATATXFIFOWRDLY, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //0 MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly0_MemClk:
        //0.5 MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly0_5_MemClk:
        //1.0 MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly1_0_MemClk:
        //1.5 MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly1_5_MemClk:
        //2.0 MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly2_0_MemClk:
        //2.5 MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly2_5_MemClk:
        //3.0  MemClk
        case  IdsNvMemCtrllerDataTransFIFOWriDly3_0__MemClk:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterNbPstateChange, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly0, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterNbPstateChange, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly1, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterNbPstateChange, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly2, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterNbPstateChange, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly3, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly0, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly1, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly2, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataTxFifoWrDly3, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemCtrllerDataTransFIFOWriDlyAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TCL, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTcl5_Clk &&
                 NvValue <= IdsNvMemTimingTcl13h_Clk) ||
                 NvValue == IdsNvMemTimingTclAuto);
        if (NvValue != IdsNvMemTimingTclAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTcl, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTcl, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRCD, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTrcd5_Clk &&
                 NvValue <= IdsNvMemTimingTrcd1Ah_Clk) ||
                 NvValue == IdsNvMemTimingTrcdAuto);
        if (NvValue != IdsNvMemTimingTrcdAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrcd, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrcd, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRP, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTrp5_Clk &&
                 NvValue <= IdsNvMemTimingTrp1Ah_Clk) ||
                 NvValue == IdsNvMemTimingTrpAuto);
        if (NvValue != IdsNvMemTimingTrpAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrp, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrp, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRTP, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTrtp4_Clk &&
                 NvValue <= IdsNvMemTimingTrtp0Eh_Clk) ||
                 NvValue == IdsNvMemTimingTrtpAuto);
        if (NvValue != IdsNvMemTimingTrtpAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrtp, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrtp, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRAS, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTras8_Clk &&
                 NvValue <= IdsNvMemTimingTras36h_Clk) ||
                 NvValue == IdsNvMemTimingTrasAuto);
        if (NvValue != IdsNvMemTimingTrasAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTras, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTras, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRRD, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTrrd1_Clk &&
                 NvValue <= IdsNvMemTimingTrrd0Dh_Clk) ||
                 NvValue == IdsNvMemTimingTrrdAuto);
        if (NvValue != IdsNvMemTimingTrrdAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrrd, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrrd, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRC, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTrc0Ah_Clk &&
                 NvValue <= IdsNvMemTimingTrc4Eh_Clk) ||
                 NvValue == IdsNvMemTimingTrcAuto);
        if (NvValue != IdsNvMemTimingTrcAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrc, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrc, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRWTTO, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTrwtTo2_Clk &&
                 NvValue <= IdsNvMemTimingTrwtTo1Bh_Clk) ||
                 NvValue == IdsNvMemTimingTrwtToAuto);
        if (NvValue != IdsNvMemTimingTrwtToAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrwtTO, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrwtTO, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TWR, IdsNvPtr, StdHeader) {
        ASSERT (
          (NvValue >= IdsNvMemTimingTwr5_Clk && NvValue <= IdsNvMemTimingTwr1Ah_Clk) ||
           NvValue == IdsNvMemTimingTwrAuto);
        if (NvValue != IdsNvMemTimingTwrAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwr, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwr, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TWRRD, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTwrrd1_Clk &&
                 NvValue <= IdsNvMemTimingTwrrd0Bh_Clk) ||
                 NvValue == IdsNvMemTimingTwrrdAuto);
        if (NvValue != IdsNvMemTimingTwrrdAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrrd, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrrd, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TWTR, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemTimingTwtr2_Clk &&
                 NvValue <= IdsNvMemTimingTwtr0Eh_Clk) ||
                 NvValue == IdsNvMemTimingTwtrAuto);
        if (NvValue != IdsNvMemTimingTwtrAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwtr, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwtr, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TWRWRSDSC, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTwrwrSdsc1_Clk &&
                 NvValue <= IdsNvMemtimingTwrwrSdsc0Bh_Clk) ||
                 NvValue == IdsNvMemtimingTwrwrSdscAuto);
        if (NvValue != IdsNvMemtimingTwrwrSdscAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrwrSdSc, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrwrSdSc, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TWRWRSDDC, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTwrwrSdDc2_Clk &&
                 NvValue <= IdsNvMemtimingTwrwrSdDc0Bh_Clk) ||
                 NvValue == IdsNvMemtimingTwrwrSdDcAuto);
        if (NvValue != IdsNvMemtimingTwrwrSdDcAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrwrSdDc, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrwrSdDc, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TWRWRDD, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTwrwrDd2_Clk &&
                 NvValue <= IdsNvMemtimingTwrwrDd0Bh_Clk) ||
                 NvValue == IdsNvMemtimingTwrwrDdAuto);
        if (NvValue != IdsNvMemtimingTwrwrDdAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrwrDd, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTwrwrDd, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRDRDSDSC, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTRdRdSdsc1_Clk &&
                 NvValue <= IdsNvMemtimingTRdRdSdsc0Bh_Clk) ||
                 NvValue == IdsNvMemtimingTRdRdSdscAuto);
        if (NvValue != IdsNvMemtimingTRdRdSdscAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrdrdSdSc, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrdrdSdSc, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRDRDSDDC, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTRdRdSdDc2_Clk &&
                 NvValue <= IdsNvMemtimingTRdRdSdDc0Bh_Clk) ||
                 NvValue == IdsNvMemtimingTRdRdSdDcAuto);
        if (NvValue != IdsNvMemtimingTRdRdSdDcAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrdrdSdDc, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrdrdSdDc, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRDRDDD, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTRdRdDd2_Clk &&
                 NvValue <= IdsNvMemtimingTRdRdDd0Bh_Clk) ||
                 NvValue == IdsNvMemtimingTRdRdDdAuto);
        if (NvValue != IdsNvMemtimingTRdRdDdAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrdrdDd, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrdrdDd, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TREFCTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingTrefCtlManual ||
                NvValue == IdsNvMemTimingTrefCtlAuto);
        if (NvValue == IdsNvMemTimingTrefCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TREF, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingTrefMin &&
                    NvValue <= IdsNvMemTimingTrefMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTref, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRWTWB, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemtimingTrwtWB3_Clk &&
                 NvValue <= IdsNvMemtimingTrwtWB1Ch_Clk) ||
                 NvValue == IdsNvMemtimingTrwtWBAuto);
        if (NvValue != IdsNvMemtimingTrwtWBAuto) {
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrwtWB, MTOverride, NvValue);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrwtWB, MTOverride, NvValue);
          (*MemTblPtr)++;
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRFC0CTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingTrfcxManual ||
                NvValue == IdsNvMemTimingTrfcxAuto);
        if (NvValue == IdsNvMemTimingTrfcxManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRFC0, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingTrfcxMin &&
                    NvValue <= IdsNvMemTimingTrfcxMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrfc0, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }

      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRFC1CTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingTrfcxManual ||
                NvValue == IdsNvMemTimingTrfcxAuto);
        if (NvValue == IdsNvMemTimingTrfcxManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TRFC1, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingTrfcxMin &&
                    NvValue <= IdsNvMemTimingTrfcxMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTrfc1, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }

      // tCKSRX
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tCKSRXCTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingTcksrxCtlManual ||
                NvValue == IdsNvMemTimingTcksrxCtlAuto);
        if (NvValue == IdsNvMemTimingTcksrxCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tCKSRX, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemtimingTcksrxMin &&
                    NvValue <= IdsNvMemtimingTcksrxMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTcksrx, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // tCKSRE
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tCKSRECTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingTcksreCtlManual ||
                NvValue == IdsNvMemTimingTcksreCtlAuto);
        if (NvValue == IdsNvMemTimingTcksreCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tCKSRE, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingTcksreMin &&
                    NvValue <= IdsNvMemTimingTcksreMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTcksre, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // tCKESR
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tCKESRCTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTckesrTimingCtlManual ||
                NvValue == IdsNvMemTckesrTimingCtlAuto);
        if (NvValue == IdsNvMemTckesrTimingCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tCKESR, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingTckesrMin &&
                    NvValue <= IdsNvMemTimingTckesrMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTckesr, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // tPD
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tPDCTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingTpdCtlManual ||
                NvValue == IdsNvMemTimingTpdCtlAuto);
        if (NvValue == IdsNvMemTimingTpdCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_tPD, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingTpdMin &&
                    NvValue <= IdsNvMemTimingTpdMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming, MTNodes, MTDcts, MTDIMMs, MTBLs, BFTpd, MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // RdPtrVal NBP0
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP0_CTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingRdPtrCtlManual ||
                NvValue == IdsNvMemTimingRdPtrCtlAuto);
        if (NvValue == IdsNvMemTimingRdPtrCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP0, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingRdPtrMin &&
                    NvValue <= IdsNvMemTimingRdPtrMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_NB_PS (0, RegDataRdPtrInitVal), MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // RdPtrVal NBP1
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP1_CTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingRdPtrCtlManual ||
                NvValue == IdsNvMemTimingRdPtrCtlAuto);
        if (NvValue == IdsNvMemTimingRdPtrCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP1, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingRdPtrMin &&
                    NvValue <= IdsNvMemTimingRdPtrMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_NB_PS (1, RegDataRdPtrInitVal), MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // RdPtrVal NBP2
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP2_CTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingRdPtrCtlManual ||
                NvValue == IdsNvMemTimingRdPtrCtlAuto);
        if (NvValue == IdsNvMemTimingRdPtrCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP2, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingRdPtrMin &&
                    NvValue <= IdsNvMemTimingRdPtrMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_NB_PS (2, RegDataRdPtrInitVal), MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
      // RdPtrVal NBP3
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP3_CTL, IdsNvPtr, StdHeader) {
        ASSERT (NvValue == IdsNvMemTimingRdPtrCtlManual ||
                NvValue == IdsNvMemTimingRdPtrCtlAuto);
        if (NvValue == IdsNvMemTimingRdPtrCtlManual) {
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDPTR_NBP3, IdsNvPtr, StdHeader) {
            ASSERT (NvValue >= IdsNvMemTimingRdPtrMin &&
                    NvValue <= IdsNvMemTimingRdPtrMax);
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_NB_PS (3, RegDataRdPtrInitVal), MTOverride, NvValue);
            (*MemTblPtr)++;
          }
        }
      }
    }
  }
}

/**
 *  DRAM Controller Override for DDR3 Memory
 *
 *  This function is used to fill memory table to override DRAM Controller
 *  settings.
 *
 *  @param[in,out] MemTblPtr    The Pointer of Memory Table.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *  @param[in,out] NBPtr        The Pointer of MEM_NB_BLOCK.
 *
 **/
VOID
IdsMemTblDramControllerDdr3F15Cz (
  IN OUT   MEM_TABLE_ALIAS **MemTblPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  MTDct DRAMContrSelDct;
  IDS_STATUS NvValue;
  IDS_STATUS PchgPDModeSel;

  DRAMContrSelDct = 0xFE;   //Not valid value in MTDct, only used to check data validity
  PchgPDModeSel = 0;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMCON, IdsNvPtr, StdHeader) {
    ASSERT (
      NvValue == IdsNvMemCtrllerCtlAuto ||
      NvValue == IdsNvMemCtrllerCtlManual
    );
    if (NvValue == IdsNvMemCtrllerCtlManual) {
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMCONTRSEL, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        case IdsNvMemCtrllerSelAll:
        case IdsNvMemCtrllerSelAuto:
          DRAMContrSelDct = MTDcts;
          break;
        case IdsNvMemCtrllerSelDCT0:
          DRAMContrSelDct = MTDct0;
          break;
        case IdsNvMemCtrllerSelDCT1:
          DRAMContrSelDct = MTDct1;
        case IdsNvMemCtrllerSelDCT2:
          DRAMContrSelDct = MTDct2;
          break;
        case IdsNvMemCtrllerSelDCT3:
          DRAMContrSelDct = MTDct3;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
        if (DRAMContrSelDct != 0xFE) {
          //Precharge power down mode select
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PCHGPDMODESEL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerPrechargePDModeSelSlow:
              PchgPDModeSel = 0;
              break;
            case  IdsNvMemCtrllerPrechargePDModeSelFast:
              PchgPDModeSel = 0;
              break;
            case  IdsNvMemCtrllerPrechargePDModeSelDynamic:
              PchgPDModeSel = 1;
              break;
            case  IdsNvMemCtrllerPrechargePDModeSelAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
            MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFPchgPDModeSel, MTOverride, PchgPDModeSel);
            (*MemTblPtr)++;
          }

          //Precharge Power Down Entry Delay Control
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PCHGPDENDELAYCTL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerPrechargePDDlyCtlManual:
              //Precharge Power Down Entry Delay
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PCHGPDENDELAY, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerPrechargePDDlyMin) && (NvValue <= IdsNvMemCtrllerPrechargePDDlyMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFPchgPDEnDelay, MTOverride, NvValue);
                (*MemTblPtr)++;
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterSettingMemoryPstate1, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFPchgPDEnDelay, MTOverride, NvValue);
                (*MemTblPtr)++;
              }
              break;
            case  IdsNvMemCtrllerPrechargePDDlyCtlAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Power Down phy power save disable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PDPHYPSDIS, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerPDPhyPowerSaveDis0:
            case  IdsNvMemCtrllerPDPhyPowerSaveDis1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFPDPhyPSDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            case  IdsNvMemCtrllerPDPhyPowerSaveDisAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

            //Refresh channel Command Management Disable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_REFCHCMDMGTDIS, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerRefreshChlCmdManageDis0:
            case  IdsNvMemCtrllerRefreshChlCmdManageDis1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFRefChCmdMgtDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            case  IdsNvMemCtrllerRefreshChlCmdManageDisAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
          //Fast self refresh entry disable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_FASTSELFREFENTRYDIS, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerFastSelfRefreshEntryDis0:
            case  IdsNvMemCtrllerFastSelfRefreshEntryDis1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFFastSelfRefEntryDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerFastSelfRefreshEntryDisAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

         //Opportunistic write enh disable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_OPPWRENHDIS, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerOpportunisticWriEnhDis0:
            case  IdsNvMemCtrllerOpportunisticWriEnhDis1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFOppWrEnhDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerOpportunisticWriEnhDisAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Partial channel power down enh Enable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PRTLCHPDENHEN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Partial Chl Power down delay
            case  IdsNvMemCtrllerPartialChlPDEnhEnPartial_Chl_Power_down_delay:
            //256 clocks
            case  IdsNvMemCtrllerPartialChlPDEnhEn256_clocks:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFPrtlChPDEnhEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerPartialChlPDEnhEnAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Aggressive power down enable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_AGGRPDEN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Disabled
            case  IdsNvMemCtrllerAggressivePwrDownEnDisabled:
            //Enabled
            case  IdsNvMemCtrllerAggressivePwrDownEnEnabled:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFAggrPDEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerAggressivePwrDownEnAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Address command tristate enable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ADDRCMDTRIEN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Disabled
            case  IdsNvMemCtrllerAddrCmdTriEnDisabled:
            //Enabled
            case  IdsNvMemCtrllerAddrCmdTriEnEnabled:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFAddrCmdTriEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerAddrCmdTriEnAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Partial chl power down dynamic delay control
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PRTLCHPDDYNDLYCTL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Manual
            case  IdsNvMemCtrllerPartialChlPDDynDlyCtlManual:
              //Partial chl power down dynamic delay
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PRTLCHPDDYNDLY, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerPartialChlPDDynDlyMin) && (NvValue <= IdsNvMemCtrllerPartialChlPDDynDlyMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFPrtlChPDDynDly, MTOverride, NvValue);
                (*MemTblPtr)++;
              }
              break;
            //Auto
            case  IdsNvMemCtrllerPartialChlPDDynDlyCtlAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Aggressive power down delay control
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_AGGRPDDELAYCTL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Manual
            case  IdsNvMemCtrllerAggrePDDlyCtlManual:
              //Aggressive power down delay
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_AGGRPDDELAY, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerAggrePDDlyMin) && (NvValue <= IdsNvMemCtrllerAggrePDDlyMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFAggrPDDelay, MTOverride, NvValue);
                (*MemTblPtr)++;
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterSettingMemoryPstate1, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFAggrPDDelay, MTOverride, NvValue);
                (*MemTblPtr)++;
              }
              break;
            //Auto
            case  IdsNvMemCtrllerAggrePDDlyCtlAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Slow Access Mode
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CMDMOTIME, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //1T
            case  IdsNvMemCtrller2TMode1T:
            //2T
            case  IdsNvMemCtrller2TMode2T:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFSlowAccessMode, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrller2TModeAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Data Scramble
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DATASCRAMBLE, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Enable
            case  IdsNvMemCtrllerScrambleEn:
            //Disable
            case  IdsNvMemCtrllerScrambleDis:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDataScrambleEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerScrambleAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //BankGroupSwap
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_BANKGROUPSWAP, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Enable
            case  IdsNvMemCtrllerBankGroupSwapEn:
            //Disable
            case  IdsNvMemCtrllerBankGroupSwapDis:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFBankGroupSwap, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerBankGroupSwapAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //WrCrcEn
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_WRCRCEN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Enable
            case  IdsNvMemCtrllerWrCrcEn:
            //Disable
            case  IdsNvMemCtrllerWrCrcDis:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFWrCrcEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerWrCrcAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //IntLvRgnSwapMode
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_INTSWAPEN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Disabled
            case  IdsNvMemCtrllerIntLvRgnSwapModeDisabled:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterInterleave, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFIntlvRegionEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Manual
            case  IdsNvMemCtrllerIntLvRgnSwapModeManual:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterInterleave, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFIntlvRegionEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              //IntLvRgnBaseAddr
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_SWAPBASE, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerIntLvRgnBaseAddrMin) && (NvValue <= IdsNvMemCtrllerIntLvRgnBaseAddrMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterInterleave, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFIntlvRegionBase, MTOverride, NvValue);
                (*MemTblPtr)++;
              }

              //IntLvRgnLmtAddr
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_SWAPLIMIT, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerIntLvRgnLmtAddrMin) && (NvValue <= IdsNvMemCtrllerIntLvRgnLmtAddrMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterInterleave, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFIntlvRegionLimit, MTOverride, NvValue);
                (*MemTblPtr)++;
              }

              break;
            //Auto
            case  IdsNvMemCtrllerIntLvRgnSwapModeAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //DramTermDyn
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMTERMDYN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Disabled
            case  IdsNvMemCtrllerDramTermDynDisabled:
            //RZQ/4
            case  IdsNvMemCtrllerDramTermDynRZQ_4:
            //RZQ/2
            case  IdsNvMemCtrllerDramTermDynRZQ_2:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDramTermDyn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerDramTermDynAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //DramTerm
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMTERM, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Disabled
            case  IdsNvMemCtrllerDramTermDisabled:
            //RZQ/4
            case  IdsNvMemCtrllerDramTermRZQ_4:
            //RZQ/2
            case  IdsNvMemCtrllerDramTermRZQ_2:
            //RZQ/6
            case  IdsNvMemCtrllerDramTermRZQ_6:
            //RZQ/12
            case  IdsNvMemCtrllerDramTermRZQ_12:
            //RZQ/8
            case  IdsNvMemCtrllerDramTermRZQ_8:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDramTerm_DDR3, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerDramTermAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //On DIMM temperature sensor enable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ODTSEN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            //Disabled
            case  IdsNvMemCtrllerOnDIMMTempSensorEnDisabled:
            //Enabled
            case  IdsNvMemCtrllerOnDIMMTempSensorEnEnabled:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFODTSEn, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerOnDIMMTempSensorEnAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
          //Disable DLL shutdown in self-refresh mode
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DLL_SHUT_DOWN, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerDisDllShutdownSR0:
            case  IdsNvMemCtrllerDisDllShutdownSR1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterInterleave, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDisDllShutdownSR, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerDisDllShutdownSRAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
          //Fast M-state change disable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_FASTMSTATECHGDIS, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerFastMstateDis0:
            case  IdsNvMemCtrllerFastMstateDis1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFFastMstateDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            case  IdsNvMemCtrllerFastMstateDisAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
          //Disable simultaneous read and write
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DISSIMRDWR, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerDisSimulRdWr0:
            case  IdsNvMemCtrllerDisSimulRdWr1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDisSimulRdWr, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            case  IdsNvMemCtrllerDisSimulRdWrAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
          //Disable automatic refresh
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DISAUTOREFRESH, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsControlDisAutoRefresh0:
            case  IdsControlDisAutoRefresh1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDisAutoRefresh, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            case  IdsControlDisAutoRefreshAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Control command throttle mode.
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CMDTHROTTLEMODECTL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsControlCmdThrottleModeCtlManual:
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CMDTHROTTLEMODE, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerCmdThrottleModeMin) && (NvValue <= IdsNvMemCtrllerCmdThrottleModeMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFCmdThrottleMode, MTOverride, NvValue);
                (*MemTblPtr)++;
              }
              break;
            case  IdsControlCmdThrottleModeCtlAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          //Efficient arbitration disable
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_MEMCTRLLEREFFARBDIS, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerEffArbDis0:
            case  IdsNvMemCtrllerEffArbDis1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFEffArbDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFEffArbDis, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerEffArbDisAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          // Disable HalfNCLK power gate
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_MEMCTRLLERDISHALFNCLKPWRGATE, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsNvMemCtrllerDisHalfNclkPwrGate0:
            case  IdsNvMemCtrllerDisHalfNclkPwrGate1:
              MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDisHalfNclkPwrGate, MTOverride, NvValue);
              (*MemTblPtr)++;
              break;
            //Auto
            case  IdsNvMemCtrllerDisHalfNclkPwrGateAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }

          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DCTPWRGATEEN_CTRL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsControlDctPowerGateCtlManual:
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DCTPWRGATEEN, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsControlDctPowerGateEnMin) && (NvValue <= IdsControlDctPowerGateEnMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDctPwrGateEn, MTOverride, NvValue);
                (*MemTblPtr)++;
              }
              break;
            case  IdsControlDctPowerGateCtlAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
          IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DCTCLKGATEEN_CTRL, IdsNvPtr, StdHeader) {
            switch (NvValue) {
            case  IdsControlDctClkGateCtlManual:
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DCTCLKGATEEN, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsControlDctClkGateEnMin) && (NvValue <= IdsControlDctClkGateEnMax));
                MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, (UINT8)DRAMContrSelDct, MTDIMMs, MTBLs, BFDctClkGateEn, MTOverride, NvValue);
                (*MemTblPtr)++;
              }
              break;
            case  IdsControlDctClkGateCtlAuto:
              break;
            default:
              ASSERT (FALSE);
              break;
            }
          }
        }
      }
    }
  }
}
/**
 *  Drive Strength Override for DDR3 Memory
 *
 *  This function is used to fill memory table to override drive strength.
 *  DRAM setting. It includes CkeDrvStren, CsOdtDrvStren, AddrCmdDrvStren,
 *  ClkDrvStren, DataDrvStren, DqsDrvStren and ProcOdt.
 *
 *  @param[in,out] MemTblPtr    The Pointer of Memory Table.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *  @param[in,out] NBPtr        The Pointer of MEM_NB_BLOCK.
 *
 **/
VOID
IdsMemTblDriveStrengthDdr3F15Cz (
  IN OUT   MEM_TABLE_ALIAS **MemTblPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  IDS_STATUS NvValue;
  ///                             240, 120,  80, 69, 60, 53, 48,  44, 40,  37, 34,  27
  CONST UINT8 ODTStrenPTbl[] =  {   1, 0xC, 0xF,  5,  0,  1,  3, 0xC,  7, 0xD,  3, 0xF};
  CONST UINT8 EQStrenHiPTbl[] = {   0,   0,   0,  1,  3,  3,  3,   3,  3,   3,  7,   7};


  //Drive Strength User Controls
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRSTRCON, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Auto
    case  IdsNvMemDrvCtlAuto:
      break;
    //Manual
    case  IdsNvMemDrvCtlManual:
      //CKE Drive Strength
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CKEDRSTR, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //1.0x
        case  IdsNvMemDrvCKEDrStr1_0x:
        //1.25x
        case  IdsNvMemDrvCKEDrStr1_25x:
        //1.5x
        case  IdsNvMemDrvCKEDrStr1_5x:
        //2.0x
        case  IdsNvMemDrvCKEDrStr2_0x:
        //Disabled
        case  IdsNvMemDrvCKEDrStrDisabled:
        //0.25x
        case  IdsNvMemDrvCKEDrStr0_25x:
        //0.5x
        case  IdsNvMemDrvCKEDrStr0_5x:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, BFCkeDrvStren, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvCKEDrStrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      //CS/ODT Drive Strength
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CODRSTR, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //1.0x
        case  IdsNvMemDrvCODrStr1_0x:
        //1.25x
        case  IdsNvMemDrvCODrStr1_25x:
        //1.5x
        case  IdsNvMemDrvCODrStr1_5x:
        //2.0x
        case  IdsNvMemDrvCODrStr2_0x:
        //Disabled
        case  IdsNvMemDrvCODrStrDisabled:
        //0.25x
        case  IdsNvMemDrvCODrStr0_25x:
        //0.5x
        case  IdsNvMemDrvCODrStr0_5x:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, BFCsOdtDrvStren, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvCODrStrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      //Addr/Cmd Drive Strength
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ACDRSTR, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //1.0x
        case  IdsNvMemDrvACDrStr1_0x:
        //1.25x
        case  IdsNvMemDrvACDrStr1_25x:
        //1.5x
        case  IdsNvMemDrvACDrStr1_5x:
        //2.0x
        case  IdsNvMemDrvACDrStr2_0x:
        //Disabled
        case  IdsNvMemDrvACDrStrDisabled:
        //0.25x
        case  IdsNvMemDrvACDrStr0_25x:
        //0.5x
        case  IdsNvMemDrvACDrStr0_5x:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, BFAddrCmdDrvStren, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvACDrStrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      //Clock Drive Strength
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CLKDRSTR, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //1.0x
        case  IdsNvMemDrvClkDrStr1_0x:
        //1.25x
        case  IdsNvMemDrvClkDrStr1_25x:
        //1.5x
        case  IdsNvMemDrvClkDrStr1_5x:
        //2.0x
        case  IdsNvMemDrvClkDrStr2_0x:
        //Disabled
        case  IdsNvMemDrvClkDrStrDisabled:
        //0.25x
        case  IdsNvMemDrvClkDrStr0_25x:
        //0.5x
        case  IdsNvMemDrvClkDrStr0_5x:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, BFClkDrvStren, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvClkDrStrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      //Data Drive Strength
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DTDRSTR, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //0.75x
        case  IdsNvMemDrvDataDrStr0_75x:
        //1.0x
        case  IdsNvMemDrvDataDrStr1_0x:
        //1.25x
        case  IdsNvMemDrvDataDrStr1_25x:
        //1.5x
        case  IdsNvMemDrvDataDrStr1_5x:
        //Disabled
        case  IdsNvMemDrvDataDrStrDisabled:
        //0.25x
        case  IdsNvMemDrvDataDrStr0_25x:
        //0.5x
        case  IdsNvMemDrvDataDrStr0_5x:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDataDrvStren, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvDataDrStrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      //DQS Drive Strength
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DQSDRSTR, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //0.75x
        case  IdsNvMemDrvDQSDrStr0_75x:
        //1.0x
        case  IdsNvMemDrvDQSDrStr1_0x:
        //1.25x
        case  IdsNvMemDrvDQSDrStr1_25x:
        //1.5x
        case  IdsNvMemDrvDQSDrStr1_5x:
        //Disabled
        case  IdsNvMemDrvDQSDrStrDisabled:
        //0.25x
        case  IdsNvMemDrvDQSDrStr0_25x:
        //0.5x
        case  IdsNvMemDrvDQSDrStr0_5x:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, BFDqsDrvStren, MTOverride, NvValue);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvDQSDrStrAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      //Proc ODT
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DQSPROCODT, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //240 ohm
        case  IdsNvMemDrvProcODT240_ohm:
        //120 ohm
        case  IdsNvMemDrvProcODT120_ohm:
        //80 ohm
        case  IdsNvMemDrvProcODT80_ohm:
        //69 ohm
        case  IdsNvMemDrvProcODT69_ohm:
        //60 ohm
        case  IdsNvMemDrvProcODT60_ohm:
        //53 ohm
        case  IdsNvMemDrvProcODT53_ohm:
        //48 ohm
        case  IdsNvMemDrvProcODT48_ohm:
        //44 ohm
        case  IdsNvMemDrvProcODT44_ohm:
        //40 ohm
        case  IdsNvMemDrvProcODT40_ohm:
        //37 ohm
        case  IdsNvMemDrvProcODT37_ohm:
        //34 ohm
        case  IdsNvMemDrvProcODT34_ohm:
        //27 ohm
        case  IdsNvMemDrvProcODT27_ohm:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, RegDataRxImpedance), MTOverride, ODTStrenPTbl[NvValue] & 0xF);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, RegDataRxImpedance), MTOverride, ODTStrenPTbl[NvValue] & 0xF);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_DATA_BYTE (PAD_DQ, ALL_BYTES, BFEQStrenHiP), MTOverride, EQStrenHiPTbl[NvValue]);
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, MTDcts, MTDIMMs, MTBLs, PER_DATA_BYTE (PAD_DQS, ALL_BYTES, BFEQStrenHiP), MTOverride, EQStrenHiPTbl[NvValue]);
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvProcODTAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

      //Low power drive strength enable
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_LOWPOWERDRVSTRENGTHEN, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Disabled
        case  IdsNvMemDrvLowPwrDrvStrengthEnDisabled:
        //Enabled
        case  IdsNvMemDrvLowPwrDrvStrengthEnEnabled:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterFinalizeMCT, MTNodes, MTDcts, MTDIMMs, MTBLs, BFLowPowerDrvStrengthEn, MTOverride, (NvValue << 8));
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterSettingMemoryPstate1, MTNodes, MTDcts, MTDIMMs, MTBLs, BFLowPowerDrvStrengthEn, MTOverride, (NvValue << 8));
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvLowPwrDrvStrengthEnAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      //ProcOdt advance
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PROCODTADV, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Disabled
        case  IdsNvMemDrvProcOdtAdvDisabled:
        //Enabled
        case  IdsNvMemDrvProcOdtAdvEnabled:
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterNbPstateChange, MTNodes, MTDcts, MTDIMMs, MTBLs, BFProcOdtAdv, MTOverride, (NvValue << 14));
          (*MemTblPtr)++;
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, MTDcts, MTDIMMs, MTBLs, BFProcOdtAdv, MTOverride, (NvValue << 14));
          (*MemTblPtr)++;
          break;
        //Auto
        case  IdsNvMemDrvProcOdtAdvAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
}

/**
 *  DQS Timing Override for DDR3 Memory
 *
 *  This function is used to fill memory table to override DQS Timing
 *  settings.
 *
 *  @param[in,out] MemTblPtr    The Pointer of Memory Table.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *  @param[in,out] NBPtr        The Pointer of MEM_NB_BLOCK.
 *
 **/
VOID
IdsMemTblDqsTimingDdr3F15Cz (
  IN OUT   MEM_TABLE_ALIAS **MemTblPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr,
  IN OUT   MEM_NB_BLOCK *NBPtr
  )
{
  UINT32 WrByteDl;
  UINT32 WrByteDlg;
  UINT32 RdDQSBtDl;
  UINT32 TmgControlAndMask;
  UINT32 TmgControlOrMask;
  IDS_STATUS NvValue;
  UINT32  TmpValue;
  BOOLEAN IsSkip;
  UINT16 DqsCtrlId;
  UINT16 DqsDelayId;
  UINT16 DqsDelayGrossId;
  UINT16 ByteLane;


  TmpValue = 0;
  NvValue = 0;
  IsSkip = FALSE;

  if (AmdIdsNvReader (AGESA_IDS_NV_DQSDELAYS, IdsNvPtr, StdHeader) == IdsNvMemDQSCtlManual) {
    DqsCtrlId = AGESA_IDS_NV_MEMDQSWRBYTE0CTL;
    DqsDelayId = AGESA_IDS_NV_WRBYTEDL0;
    DqsDelayGrossId = AGESA_IDS_NV_WRBYTEDLG0;
    ByteLane = MTBL0;
    for (; DqsCtrlId <= AGESA_IDS_NV_MEMDQSWRBYTE7CTL; DqsCtrlId++, DqsDelayId += 2, DqsDelayGrossId += 2, ByteLane <<= 1) {
      //Write Data Byte Control
      IDS_NV_READ_SKIP (NvValue, DqsCtrlId, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Auto
        case  IdsNvMemDQSWrByte0CtlAuto:
          break;
        //Manual
        case  IdsNvMemDQSWrByte0CtlManual:
          WrByteDl = AmdIdsNvReader (DqsDelayId, IdsNvPtr, StdHeader);
          WrByteDlg = AmdIdsNvReader (DqsDelayGrossId, IdsNvPtr, StdHeader);
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8) MTDcts, MTDIMMs, ByteLane, BFWrDatDly, MTOverride, (WrByteDlg << 5 | WrByteDl));
          (*MemTblPtr)++;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
    }

    DqsCtrlId = AGESA_IDS_NV_MEMDQSRDDQSBT0CTL;
    DqsDelayId = AGESA_IDS_NV_RDDQSBTDL0;
    ByteLane = MTBL0;
    for (; DqsCtrlId <= AGESA_IDS_NV_MEMDQSRDDQSBT7CTL; DqsCtrlId++, DqsDelayId += 1, ByteLane <<= 1) {
      //Read Data Byte Control
      IDS_NV_READ_SKIP (NvValue, DqsCtrlId, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Auto
        case  IdsNvMemDQSRdDQSBt0CtlAuto:
          break;
        //Manual
        case  IdsNvMemDQSRdDQSBt0CtlManual:
          RdDQSBtDl = AmdIdsNvReader (DqsDelayId, IdsNvPtr, StdHeader);
          MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterTrn, MTNodes, (UINT8) MTDcts, MTDIMMs, ByteLane, BFRdDqsDly, MTOverride, RdDQSBtDl);
          (*MemTblPtr)++;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
    }

    TmgControlAndMask = 0xFFFFFFFF;
    TmgControlOrMask = 0;
    //CKE Fine Delay Control
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_MEMDQSCKEFINEDELAYCTL, IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //Auto
      case  IdsNvMemDQSCkeFineDelayCtlAuto:
        break;
      //Manual
      case  IdsNvMemDQSCkeFineDelayCtlManual:
        //CKE Fine Delay
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CKEFNDL, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvMemDQSCKEFnDlMin) && (NvValue <= IdsNvMemDQSCKEFnDlMax));
          TmgControlOrMask |= NvValue ;
          TmgControlAndMask |= IDS_GET_MASK32 (4, 0);
        }
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }

    //CKE Setup Time
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CKESETUPTIME, IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //1/2 Clk
      case  IdsNvMemDQSCKESetupTime1_2_Clk:
      case  IdsNvMemDQSCKESetupTime1_Clk:
        TmgControlOrMask |= (NvValue << 5);
        TmgControlAndMask |= IDS_GET_MASK32 (5, 5);
        break;
      //Auto
      case  IdsNvMemDQSCKESetupTimeAuto:
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }

    //CS/ODT Fine Delay Control
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_MEMDQSCSODTFINEDELAYCTL, IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //Auto
      case  IdsNvMemDQSCsOdtFineDelayCtlAuto:
        break;
      //Manual
      case  IdsNvMemDQSCsOdtFineDelayCtlManual:
        //CS/ODT Fine Delay
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_COFNDL, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvMemDQSCOFnDlMin) && (NvValue <= IdsNvMemDQSCOFnDlMax));
          TmgControlOrMask |= (NvValue << 8);
          TmgControlAndMask |= IDS_GET_MASK32 (12, 8);
        }
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }

    //CS/ODT Setup Time
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_COSETUPTIME, IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //1/2 Clk
      case  IdsNvMemDQSCOSetupTime1_2_Clk:
      case  IdsNvMemDQSCOSetupTime1_Clk:
        TmgControlOrMask |= (NvValue << 13);
        TmgControlAndMask |= IDS_GET_MASK32 (13, 13);
        break;
      //Auto
      case  IdsNvMemDQSCOSetupTimeAuto:
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }

    //Addr/Cmd Fine Delay Control
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_MEMDQSADDRCMDFINEDELAYCTL, IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //Auto
      case  IdsNvMemDQSAddrCmdFineDelayCtlAuto:
        break;
      //Manual
      case  IdsNvMemDQSAddrCmdFineDelayCtlManual:
        //Addr/Cmd Fine Delay
        IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ADDRCMDDNDL, IdsNvPtr, StdHeader) {
          ASSERT ((NvValue >= IdsNvMemDQSAddrCmdFnDlMin) && (NvValue <= IdsNvMemDQSAddrCmdFnDlMax));
          TmgControlOrMask |= (NvValue << 16);
          TmgControlAndMask |= IDS_GET_MASK32 (20, 16);
        }
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }

    //Addr/Cmd Setup Time
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ADDRCMDSETUPTIME, IdsNvPtr, StdHeader) {
      switch (NvValue) {
      //1/2 Clk
      case  IdsNvMemDQSAddrCmdSetupTime1_2_Clk:
      case  IdsNvMemDQSAddrCmdSetupTime1_Clk:
        TmgControlOrMask |= (NvValue << 21);
        TmgControlAndMask |= IDS_GET_MASK32 (21, 21);
        break;
      //Auto
      case  IdsNvMemDQSAddrCmdSetupTimeAuto:
        break;
      default:
        ASSERT (FALSE);
        break;
      }
    }

    MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFAddrTmgControl, MTAnd, ~TmgControlAndMask);
    (*MemTblPtr)++;
    MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterPlatformSpec, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFAddrTmgControl, MTOr, TmgControlOrMask);
    (*MemTblPtr)++;

  }

  //Write ODT on duration control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_WRODTONDURATIONCTL, IdsNvPtr, StdHeader) {
    IsSkip = FALSE;
    switch (NvValue) {
    //Disabled
    case  IdsNvMemDQSWriODTDurCtlDisabled:
      TmpValue = 0;
      break;
    //Manual
    case  IdsNvMemDQSWriODTDurCtlManual:
      //Write ODT on duration
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_WRODTONDURATION, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemDQSWriODTDurMin) && (NvValue <= IdsNvMemDQSWriODTDurMax));
        TmpValue = NvValue;
      }
      break;
    //Auto
    case  IdsNvMemDQSWriODTDurCtlAuto:
      IsSkip = TRUE;
      break;
    default:
      IsSkip = TRUE;
      ASSERT (FALSE);
      break;
    }
    if (IsSkip == FALSE) {
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFWrOdtOnDuration, MTOverride, TmpValue);
      (*MemTblPtr)++;
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFWrOdtOnDuration, MTOverride, TmpValue);
      (*MemTblPtr)++;
    }
  }

  //Write ODT Turn on Delay control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_WRODTTRNONDLYCTL, IdsNvPtr, StdHeader) {
    IsSkip = FALSE;
    switch (NvValue) {
    //Disabled
    case  IdsNvMemDQSWriODTTurnOnDlyCtlDisabled:
      TmpValue = 0;
      break;
    //Manual
    case  IdsNvMemDQSWriODTTurnOnDlyCtlManual:
      //Write ODT Turn on Delay
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_WRODTTRNONDLY, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemDQSWriODTTurnOnDlyMin) && (NvValue <= IdsNvMemDQSWriODTTurnOnDlyMax));
        TmpValue = NvValue;
      }
      break;
    //Auto
    case  IdsNvMemDQSWriODTTurnOnDlyCtlAuto:
      IsSkip = TRUE;
      break;
    default:
      IsSkip = TRUE;
      ASSERT (FALSE);
      break;
    }
    if (IsSkip == FALSE) {
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFWrOdtTrnOnDly, MTOverride, TmpValue);
      (*MemTblPtr)++;
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFWrOdtTrnOnDly, MTOverride, TmpValue);
      (*MemTblPtr)++;
    }
  }

  //Read ODT On Duration Control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDODTONDURATIONCTL, IdsNvPtr, StdHeader) {
    IsSkip = FALSE;
    switch (NvValue) {
    //Disabled
    case  IdsNvMemDQSReadODTDurCtlDisabled:
      TmpValue = 0;
      break;
    //Manual
    case  IdsNvMemDQSReadODTDurCtlManual:
      //Read ODT On Duration
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDODTONDURATION, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemDQSReadODTDurMin) && (NvValue <= IdsNvMemDQSReadODTDurMax));
        TmpValue = NvValue;
      }
      break;
    //Auto
    case  IdsNvMemDQSReadODTDurCtlAuto:
      IsSkip = TRUE;
      break;
    default:
      IsSkip = TRUE;
      ASSERT (FALSE);
      break;
    }
    if (IsSkip == FALSE) {
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTBeforeDInit, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFRdOdtOnDuration, MTOverride, TmpValue);
      (*MemTblPtr)++;
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn, MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFRdOdtOnDuration, MTOverride, TmpValue);
      (*MemTblPtr)++;
    }
  }

  //Read ODT Turn On Delay Control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDODTTRNONDLYCTL, IdsNvPtr, StdHeader) {
    IsSkip = FALSE;
    switch (NvValue) {
    //Manual
    case  IdsNvMemDQSReadODTTurnDlyCtlManual:
      //Read ODT Turn On Delay
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RDODTTRNONDLY, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvMemDQSReadODTTurnDlyMin) && (NvValue <= IdsNvMemDQSReadODTTurnDlyMax));
        TmpValue = NvValue;
      }
      break;
    //Disabled
    case  IdsNvMemDQSReadODTTurnDlyCtlDisabled:
      TmpValue = 0;
      break;
    //Auto
    case  IdsNvMemDQSReadODTTurnDlyCtlAuto:
      IsSkip = TRUE;
      break;
    default:
      IsSkip = TRUE;
      ASSERT (FALSE);
      break;
    }
    if (IsSkip == FALSE) {
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterAutoCycTiming , MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFRdOdtTrnOnDly, MTOverride, TmpValue);
      (*MemTblPtr)++;
      MAKE_MEM_TABLE_ALIAS ((*MemTblPtr), MTAfterMemPstate1PartialTrn , MTNodes, (UINT8) MTDcts, MTDIMMs, MTBLNOECC, BFRdOdtTrnOnDly, MTOverride, TmpValue);
      (*MemTblPtr)++;
    }
  }
}


/**
 *  DRAM Setting Override for DDR3 Memory
 *
 *  This function is used to override DRAM setting. It includes DRAM
 *  Timing, DRAM Controller, Drive Strength and DQS Timing.
 *
 *  @param[in,out] DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubDramSettingDdr3F15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  MEM_TABLE_ALIAS *pidsmemheap;
  MEM_NB_BLOCK *NBPtr;

  AGESA_STATUS status;

  NBPtr = (MEM_NB_BLOCK *) DataPtr;
  pidsmemheap = NULL;
  status = AmdGetIdsMemTable (&pidsmemheap, StdHeader);

  if (status == AGESA_SUCCESS) {
    *((UINT16 *)pidsmemheap) = 0x55AA;
    pidsmemheap = (MEM_TABLE_ALIAS *) ((UINT16 *)pidsmemheap + 1);
    IdsMemTblDramTimingDdr3F15Cz (&pidsmemheap, StdHeader, IdsNvPtr, NBPtr);
    IdsMemTblDramControllerDdr3F15Cz (&pidsmemheap, StdHeader, IdsNvPtr, NBPtr);
    IdsMemTblDriveStrengthDdr3F15Cz (&pidsmemheap, StdHeader, IdsNvPtr, NBPtr);
    IdsMemTblDqsTimingDdr3F15Cz (&pidsmemheap, StdHeader, IdsNvPtr, NBPtr);
    pidsmemheap ->time = 0xFF;
  }

  return IDS_SUCCESS;
}
/**
 *  DRAM Setting Override for DDR3 Memory
 *
 *  This function is used to override Mr0 register
 *
 *  @param[in,out] DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubMemOverrideMr0Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT32 * Ppd;
  IDS_STATUS NvValue;
//Currently only override Ppd, for there is no structure define of Mr0
  Ppd = (UINT32 *) DataPtr;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMCON, IdsNvPtr, StdHeader) {
    if (NvValue == IdsNvMemCtrllerCtlManual) {
      //Precharge Power down mode select
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PCHGPDMODESEL, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Slow
        case  IdsNvMemCtrllerPrechargePDModeSelSlow:
          *Ppd = 0;
          break;
        //Fast
        case  IdsNvMemCtrllerPrechargePDModeSelFast:
          *Ppd = 1;
          break;
        //Dynamic
        case  IdsNvMemCtrllerPrechargePDModeSelDynamic:
        //Auto
        case  IdsNvMemCtrllerPrechargePDModeSelAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
    }
  }
  return IDS_SUCCESS;
}

/**
 *  DRAM Phy Power Saving Override for DDR3 Memory
 *
 *  This function is used to bypass Phy Power Savings
 *
 *  @param[in,out] DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_UNSUPPORTED     Bypass Phy Power Saving
 *  @retval IDS_SUCCESS         Do Phy Phy Power Saving
 *
 **/
IDS_STATUS
IdsIntSubMemPhyPowerSavingCz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS MemPhyPowerSaving;

  IDS_NV_READ_SKIP (MemPhyPowerSaving, AGESA_IDS_NV_PHYPOWERSAVING, IdsNvPtr, StdHeader) {
    if (MemPhyPowerSaving == (IDS_STATUS) 0x0) {
      return IDS_UNSUPPORTED;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  Enforce MCLK above NCLK
 *
 *  This function is used to enforce MCLK above NCLK
 *
 *  @param[in,out] DataPtr      The Pointer of Data to Override.
 *  @param[in,out] StdHeader    The Pointer of AGESA Header.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_UNSUPPORTED     Bypass Phy Power Saving
 *  @retval IDS_SUCCESS         Do Phy Phy Power Saving
 *
 **/
IDS_STATUS
IdsIntSubMemEnforceMClkAboveNClkCz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS  *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS MemEnforceMClkAboveNClk;

  IDS_NV_READ_SKIP (MemEnforceMClkAboveNClk, AGESA_IDS_NV_MEMCLK_ABOVE_NCLK, IdsNvPtr, StdHeader) {
    if (MemEnforceMClkAboveNClk == (IDS_STATUS) 0x1) {
      return IDS_UNSUPPORTED;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Memory Mapping F15
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_POST_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubMemoryMappingF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  AMD_POST_PARAMS *PostParamsPtr;
  MEM_PARAMETER_STRUCT *RefPtr;
  IDS_STATUS NvValue;
  MEM_DATA_STRUCT * memdataptr;

  PostParamsPtr = (AMD_POST_PARAMS *)DataPtr;
  memdataptr = PostParamsPtr->MemConfig.MemData;
  RefPtr = memdataptr->ParameterListPtr;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMCON, IdsNvPtr, StdHeader) {
    if (NvValue == IdsNvMemCtrllerCtlManual) {
      //Bank Swizzle Mode
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_BANK_SWIZZLE, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Disabled
        case  IdsNvMemCtrllerBankSwizzleDisabled:
        //Enabled
        case  IdsNvMemCtrllerBankSwizzleEnabled:
          RefPtr->EnableBankSwizzle = (BOOLEAN) NvValue;
          break;
        //Auto
        case  IdsNvMemCtrllerBankSwizzleAuto:
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
    }
  }

  return IDS_SUCCESS;
}
/**
 *  IDS Backend Function
 *
 *  This function is used to override settings before DQS training
 *
 *  @param[in,out]   DataPtr      The Pointer of AMD_CPU_LATE_PARAMS.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsPhyDLLStandbyControlBeforeDramInitF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  MEM_NB_BLOCK  *NBPtr;
  UINT8         Dct;

  NBPtr = (MEM_NB_BLOCK *) DataPtr;
  // Clear the registers of PHY DLL standby feature before fence training start
  for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
    NBPtr->SwitchDCT (NBPtr, Dct);
    if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
      NBPtr->SetBitField (NBPtr, BFPhy0x0D080F0C, 0x00008000);
      NBPtr->SetBitField (NBPtr, BFRxDLLWakeupTime, 0);
      NBPtr->SetBitField (NBPtr, BFRxCPUpdPeriod, 0);
      NBPtr->SetBitField (NBPtr, BFRxMaxDurDllNoLock, 0);
      NBPtr->SetBitField (NBPtr, BFTxDLLWakeupTime, 0);
      NBPtr->SetBitField (NBPtr, BFTxCPUpdPeriod, 0);
      NBPtr->SetBitField (NBPtr, BFTxMaxDurDllNoLock, 0);
      NBPtr->SetBitField (NBPtr, BFPhy0x0D080F0C, 0);
    }
  }

  return IDS_SUCCESS;

}
/**
 *  IDS Backend Function for Option Hook Point IDS_PHY_DLL_STANDBY_CTRL
 *
 *  This function is used to enable/disable Phy DLL standby feature.
 *
 *  @param[in,out]   DataPtr      The Pointer to NB block.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsPhyDLLStandbyControlF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  UINT32 Value32;
  UINT8  Dct;
  IDS_STATUS NvValue;

  MEM_NB_BLOCK  *NBPtr;

  NBPtr = (MEM_NB_BLOCK *) DataPtr;
  //PHY DLL StandBy
  Value32 = 0;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMCON, IdsNvPtr, StdHeader) {
    if (NvValue == IdsNvMemCtrllerCtlManual) {
      //Phy DLL Standby Control
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PhyDLLStandby, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Auto
        case  IdsNvMemCtrllerDLLStandbyCtlAuto:
          break;
        //Manual
        case  IdsNvMemCtrllerDLLStandbyCtlManual:
          for (Dct = 0; Dct < NBPtr->DctCount; Dct++) {
            NBPtr->SwitchDCT (NBPtr, Dct);
            if (NBPtr->DCTPtr->Timings.DctMemSize != 0) {
              //Register programming for PHY DLL standby
              //If TxMaxDurDllNoLock!=0(standby is enabled),D18F2x[1,0]9C_x0D0F_0[F,7:0]0C[DllRstRelock]
              //must be set to 1 prior to writing this register and then DllRstRelock must be cleared after the register write.
              NBPtr->SetBitField (NBPtr, BFPhy0x0D080F0C, 0x00008000);

              //RxDllWakeupTime
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RxDLLWakeupTime, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerRxDllWakeupTimeMin) && (NvValue <= IdsNvMemCtrllerRxDllWakeupTimeMax));
                NBPtr->SetBitField (NBPtr, BFRxDLLWakeupTime, NvValue);
              }

              //RxCPUpdPeriod
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RxCPUpdPeriod, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerRxCPUpdPeriodMin) && (NvValue <= IdsNvMemCtrllerRxCPUpdPeriodMax));
                NBPtr->SetBitField (NBPtr, BFRxCPUpdPeriod, NvValue);
              }

              //RxMaxDurDllNoLock
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_RxMaxDurDllNoLock, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerRxMaxDurDllNoLockMin) && (NvValue <= IdsNvMemCtrllerRxMaxDurDllNoLockMax));
                NBPtr->SetBitField (NBPtr, BFRxMaxDurDllNoLock, NvValue);
              }

              //TxDllWakeupTime
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TxDLLWakeupTime, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerTxDllWakeupTimeMin) && (NvValue <= IdsNvMemCtrllerTxDllWakeupTimeMax));
                NBPtr->SetBitField (NBPtr, BFTxDLLWakeupTime, NvValue);
              }

              //TxCPUpdPeriod
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TxCPUpdPeriod, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerTxCPUpdPeriodMin) && (NvValue <= IdsNvMemCtrllerTxCPUpdPeriodMax));
                NBPtr->SetBitField (NBPtr, BFTxCPUpdPeriod, NvValue);
              }

              //TxMaxDurDllNoLock
              IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_TxMaxDurDllNoLock, IdsNvPtr, StdHeader) {
                ASSERT ((NvValue >= IdsNvMemCtrllerTxMaxDurDllNoLockMin) && (NvValue <= IdsNvMemCtrllerTxMaxDurDllNoLockMax));
                NBPtr->SetBitField (NBPtr, BFTxMaxDurDllNoLock, NvValue);
              }

              NBPtr->SetBitField (NBPtr, BFPhy0x0D080F0C, 0x00000000);
            }
          }
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }

    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for override GNB Platform config of midpost
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubGnbPlatformCfgF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  GFX_PLATFORM_CONFIG *PGfxPlatformCfg;
  IDS_STATUS NvValue;

  PGfxPlatformCfg = (GFX_PLATFORM_CONFIG *)DataPtr;
  //LCD backlight control
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_LCD_BACKLIGHT_CONTROL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Auto
    case  IdsNvGnbGfxLcdBackLightCtlAuto:
      break;
    //Manual
    case  IdsNvGnbGfxLcdBackLightCtlManual:
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_LCD_BACKLIGHT, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvGnbGfxLcdBackLightCtlMin) && (NvValue <= IdsNvGnbGfxLcdBackLightCtlMax));
        PGfxPlatformCfg->LcdBackLightControl = (UINT16)NvValue;
      }
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //Platform Type
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_PLATFORM_TYPE, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Mobile
    case  IdsNvGnbGfxPlatformTypeMobile:
      PGfxPlatformCfg->AmdPlatformType = AMD_PLATFORM_MOBILE;
      break;
    //Desktop
    case  IdsNvGnbGfxPlatformTypeDesktop:
      PGfxPlatformCfg->AmdPlatformType = AMD_PLATFORM_DESKTOP;
      break;
    //Auto
    case  IdsNvGnbGfxPlatformTypeAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //SCLK/NCLK GMC Gating
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_GMC_SCLK_GATING, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvGnbPmmSclkGmcGatingDisabled:
    //Enabled
    case  IdsNvGnbPmmSclkGmcGatingEnabled:
      PGfxPlatformCfg->GmcClockGating = (BOOLEAN)NvValue;
      break;
    case  IdsNvGnbPmmSclkGmcGatingAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  //GMC Power Gating Ctrl
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_GMC_POWG_GATING_CTRL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Auto
    case  IdsNvGnbGfxGmcPowerGatingDataAuto:
      break;
    //Manual
    case  IdsNvGnbGfxGmcPowerGatingDataManual:
      //GMC Power Gating
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_GMC_POWG_GATING, IdsNvPtr, StdHeader) {
        switch (NvValue) {
        //Stutter Disabled
        case  IdsNvGnbGfxGmcPowerGatingStutter_Disabled:
          PGfxPlatformCfg->GmcPowerGating = GmcPowerGatingDisabled;
          break;
        //Stutter Only
        case  IdsNvGnbGfxGmcPowerGatingStutter_Only:
          PGfxPlatformCfg->GmcPowerGating = GmcPowerGatingStutterOnly;
          break;
        //Stutter and Powergate
        case  IdsNvGnbGfxGmcPowerGatingStutter_and_Powergate:
          PGfxPlatformCfg->GmcPowerGating = GmcPowerGatingWithStutter;
          break;
        default:
          ASSERT (FALSE);
          break;
        }
      }
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  //UMA Steering
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_UMA_STEERING, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Garlic
    case  IdsNvGnbGfxUMASteeringSnoopedOnion:
      PGfxPlatformCfg->UmaSteering = DefaultGarlic;
      break;
    //Onion
    case  IdsNvGnbGfxUMASteeringSystemTrafficOnion:
      PGfxPlatformCfg->UmaSteering = SystemTrafficOnion;
      break;
    case  IdsNvGnbGfxUMASteeringOnion:
      PGfxPlatformCfg->UmaSteering = Onion;
      break;
    case  IdsNvGnbGfxUMASteeringGarlic:
      PGfxPlatformCfg->UmaSteering = Garlic;
      break;
    case  IdsNvGnbGfxUMASteeringAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}
/**
 *  IDS Backend Function for override GNB dynamic wake
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubGnbORBDynamicWakeF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  D0F0x98_x2C_STRUCT  *PD0F0x98_x2C_Reg;
  IDS_STATUS NvValue;
  PD0F0x98_x2C_Reg = (D0F0x98_x2C_STRUCT*) DataPtr;
  //ORB Dynamic Wake
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_EN, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Disabled
    case  IdsNvGnbPmmOrbDynamicWakeDisabled:
    //Enabled
    case  IdsNvGnbPmmOrbDynamicWakeEnabled:
      PD0F0x98_x2C_Reg->Field.DynWakeEn = NvValue;
      break;
    case  IdsNvGnbPmmOrbDynamicWakeAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //ORB Wake Hysteresis
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_HYSTERESIS, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Auto
    case  IdsNvGnbPmmOrbWakeHysteresisAuto:
      break;
    //Manual
    case  IdsNvGnbPmmOrbWakeHysteresisManual:
      //ORB Wake Hysteresis Value
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_ORB_DYNAMIC_WAKE_HYSTERESIS_VAL, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvGnbPmmOrbWakeHysteresisValMin) && (NvValue <= IdsNvGnbPmmOrbWakeHysteresisValMax));
        PD0F0x98_x2C_Reg->Field.WakeHysteresis = NvValue;
      }
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for override GNB Property
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubGnbPropertyOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for override GNB PCIe Power Gating
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubGnbPCIePowerGatingOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  UINT8  *PowerGatingFlags;

  PowerGatingFlags = (UINT8 *)DataPtr;
  //PCIe Core Power Gating
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_PCIE_CORE_POWER_GATING, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case  IdsNvGnbPCIeCorePowerGatingDisabled:
      *PowerGatingFlags |= PCIE_POWERGATING_SKIP_CORE;
      break;
    case  IdsNvGnbPCIeCorePowerGatingEnabled:
      *PowerGatingFlags &= (~PCIE_POWERGATING_SKIP_CORE);
      break;
    case  IdsNvGnbPCIeCorePowerGatingAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  //PCIe Phy Power Gating
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_PCIE_PHY_POWER_GATING, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case  IdsNvGnbPCIePhyPowerGatingDisabled:
      *PowerGatingFlags |= PCIE_POWERGATING_SKIP_PHY;
      break;
    case  IdsNvGnbPCIePhyPowerGatingEnabled:
      *PowerGatingFlags &= (~PCIE_POWERGATING_SKIP_PHY);
      break;
    case  IdsNvGnbPCIePhyPowerGatingAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function override GnbSwTjOffset
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubGnbSwTjOffsetOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  UINT8  *TjOffset;

  TjOffset = (UINT8 *)DataPtr;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_PMM_GNBSWTJOFFSETCTL, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Manual
    case  IdsNvGnbPmmGnbSwTjOffsetCtlManual:
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_PMM_GNBSWTJOFFSET, IdsNvPtr, StdHeader) {
        ASSERT ((NvValue >= IdsNvGnbPmmGnbSwTjOffsetMin) && (NvValue <= IdsNvGnbPmmGnbSwTjOffsetMax));
        *TjOffset = (UINT8) NvValue;
      }
      break;
  //Auto
    case  IdsNvGnbPmmGnbSwTjOffsetCtlAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function skip LockDramConfig
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSkipSubLockDramCfgF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS SkipLockDramConfig;

  IDS_NV_READ_SKIP (SkipLockDramConfig, AGESA_IDS_NV_MEM_LOCKDRAM_CONFIG, IdsNvPtr, StdHeader) {
    if (SkipLockDramConfig == (IDS_STATUS) 0xFF) {
      return IDS_UNSUPPORTED;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for Option Hook Point IDS_GET_STRETCH_FREQUENCY_LIMIT
 *
 *  This function is used to get stretch memory frequency limit.
 *
 *  @param[in,out]   DataPtr      The Pointer to NB block.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsGetStretchMemFreqF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  if (IdsEnforcePorMemFreq (DataPtr, StdHeader, IdsNvPtr) == IDS_UNSUPPORTED) {
    IDS_HDT_CONSOLE (MEM_FLOW, "\nUse Stretch memory frequency limit");
    *(PSC_TBL_ENTRY ***) DataPtr = memPSCTblMaxStretchFreqArrayCZ;
  }

  return IDS_SUCCESS;
}
/**
 *  IDS Backend Function for disable CPU feat
 *
 *
 *  @param[in,out]   DataPtr      Cpu Feat descriptor
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Do nothing
 *  @retval IDS_UNSUPPORTED     disable CPU feat
 *
 **/
IDS_STATUS
IdsDisableCpuFeatF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
//HDTOUT information is required here
  CPU_FEATURE_DESCRIPTOR *CpuFeatDescriptor;
  IDS_STATUS Status;
  IDS_STATUS NvValue;

  Status = IDS_SUCCESS;
  CpuFeatDescriptor = (CPU_FEATURE_DESCRIPTOR *) DataPtr;
  if (CpuFeatDescriptor->Feature == CoreBoost) {
    //Core Performance Boost Master Enable
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPBMASTEREN, IdsNvPtr, StdHeader) {
      if (NvValue == IdsNvCpuPstApmMasterEnDisabled) {
        IDS_HDT_CONSOLE (CPU_TRACE, "  CoreBoost Dis By IDS\n");
        Status = IDS_UNSUPPORTED;
      }
    }
  }
  return Status;
}

/**
 *  IDS Backend Function to enable or disable NB P-states
 *
 *  Parameters:
 *  @param[in,out]   DataPtr      The Pointer of PLATFORM_CONFIGURATION.
 *  @param[in,out]   StdHeader    AMD standard header config param
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 * @retval      AGESA_SUCCESS     Always Succeeds.
 *
 */
IDS_STATUS
IdsSubNBPStatesSupportOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  PLATFORM_CONFIGURATION *PlatformConfig;
  IDS_STATUS NvValue;

  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_PSTATES_SUPPORT, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvCpuNBPstatesEnable:
    case IdsNvCpuNBPstatesDisable:
      PlatformConfig->PlatformProfile.NbPstatesSupported = (BOOLEAN)NvValue;
      break;
    case IdsNvCpuNBPstatesAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function to override below platform configuration variables:
 *
 *  CPU:
 *     CurrentLimit
 *     MaxCurrentLimit
 *  NB:
 *     CurrentLimit
 *     MaxCurrentLimit
 *
 *  @param[in,out]   DataPtr      Cpu Feat descriptor
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Do nothing
 *  @retval IDS_UNSUPPORTED     disable CPU feat
 *
 **/
IDS_STATUS
IdsCurrentLimitOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  IDS_STATUS CPUCurrLmtCtl;
  IDS_STATUS NBCurrLmtCtl;
  IDS_STATUS Status;
  PLATFORM_CONFIGURATION *PlatformConfig;

  CPUCurrLmtCtl = 0;
  NBCurrLmtCtl = 0;
  Status = IDS_SUCCESS;
  IdsOutPort (0x99, 0x20, 0);
  PlatformConfig = (PLATFORM_CONFIGURATION *)DataPtr;

  // Override CPU Current Limit
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPUCURRENTLIMITCTL, IdsNvPtr, StdHeader) {
     CPUCurrLmtCtl = NvValue;
  }
  if (CPUCurrLmtCtl != 0) {
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPUCURRENTLIMIT, IdsNvPtr, StdHeader) {
      if (NvValue > 255 || NvValue == 0) {
        IDS_HDT_CONSOLE (IDS_TRACE, "CPU Current Limit: 0x%x -- Invalid Input!\n", NvValue);
        Status = IDS_UNSUPPORTED;
      } else {
        PlatformConfig->VrmProperties[CoreVrm].CurrentLimit = NvValue;
      }
    }
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_CPUMAXCURRENTLIMIT, IdsNvPtr, StdHeader) {
      if (NvValue > 0xFFFF || NvValue == 0) {
        IDS_HDT_CONSOLE (IDS_TRACE, "CPU Max Current Limit: 0x%x -- Invalid Input!\n", NvValue);
        Status = IDS_UNSUPPORTED;
      } else {
        PlatformConfig->VrmProperties[CoreVrm].MaximumCurrentLimit = NvValue;
      }
    }
  }

  // Override NB Current Limit
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NBCURRENTLIMITCTL, IdsNvPtr, StdHeader) {
    NBCurrLmtCtl = NvValue;
  }
  if (NBCurrLmtCtl != 0) {
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NBCURRENTLIMIT, IdsNvPtr, StdHeader)  {
      if (NvValue > 255 || NvValue == 0) {
        IDS_HDT_CONSOLE (IDS_TRACE, "NB Current Limit: 0x%x -- Invalid Input!\n", NvValue);
        Status = IDS_UNSUPPORTED;
      } else {
        PlatformConfig->VrmProperties[NbVrm].CurrentLimit = NvValue;
      }
    }
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_NBMAXCURRENTLIMIT, IdsNvPtr, StdHeader) {
      if (NvValue > 0xFFFF || NvValue == 0) {
        IDS_HDT_CONSOLE (IDS_TRACE, "NB Max Current Limit: 0x%x -- Invalid Input!\n", NvValue);
        Status = IDS_UNSUPPORTED;
      } else {
        PlatformConfig->VrmProperties[NbVrm].MaximumCurrentLimit = NvValue;
      }
    }
  }

  // Override Stapm config
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_STAPM_CONFIG_CONTROL, IdsNvPtr, StdHeader) {
    if (NvValue == 1) {
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_STAPM_SCALAR, IdsNvPtr, StdHeader)  {
        PlatformConfig->PlatStapmConfig.CfgStapmScalar = NvValue;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_STAPM_BOOST, IdsNvPtr, StdHeader)  {
        PlatformConfig->PlatStapmConfig.CfgStapmBoost = NvValue;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_STAPM_TIME_CONSTANT, IdsNvPtr, StdHeader)  {
        PlatformConfig->PlatStapmConfig.CfgStapmTimeConstant = NvValue;
      }
    }
  }
  return Status;
}

/**
 *  IDS Backend Function for APIC330 LVT Thermal Sensor
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntAPIC330MaskConfigF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  UINT64                 ApicBase;
  UINT32                 Register330;
  UINT64                 Address;

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_APIC330_MASK_CONFIG, IdsNvPtr, StdHeader) {
    if (NvValue != IdsNvCpuGenSMIMaskAuto) {
      LibAmdMsrRead (MSR_APIC_BAR, &ApicBase, StdHeader);
      ApicBase &= LAPIC_BASE_ADDR_MASK;
      Address = ApicBase + 0x330;
      LibAmdMemRead (AccessWidth32, Address, &Register330, StdHeader);
      Register330 &= ~(UINT32) (1 << 16);

      if (NvValue == IdsNvCpuGenSMIMaskMasked) {
        Register330 |= (1 << 16);
      }
      LibAmdMemWrite (AccessWidth32, Address, &Register330, StdHeader);
    }
  }
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for ECC Configuration F15 CZ
 *
 *
 *  @param[in,out]   DataPtr      The Pointer of ECC_OVERRIDE_STRUCT.
 *  @param[in,out]   StdHeader    The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]       IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS           Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsSubEccConfigurationF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  ECC_OVERRIDE_STRUCT * PeccOverStruct;
  IDS_STATUS NvValue;

  PeccOverStruct = (ECC_OVERRIDE_STRUCT *)DataPtr;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAMCON, IdsNvPtr, StdHeader) {
    if (NvValue != IdsControlManual) {
      return IDS_SUCCESS;
    }
  }

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_ECCEN, IdsNvPtr, StdHeader) {
    if (NvValue != IdsOptionEnabled) {
      return IDS_SUCCESS;
    }
  }

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_SCRUB_REDIRECTION, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvMemCtrllerEccScrubRedirectDis:
    case IdsNvMemCtrllerEccScrubRedirectEn:
      PeccOverStruct->CfgEccRedirection = (BOOLEAN) NvValue;
      break;
    case IdsNvMemCtrllerEccScrubRedirectAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_DRAM_SCRUB, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case IdsNvMemCtrllerEccDramScrub_Disable:
    case IdsNvMemCtrllerEccDramScrub_40ns:
    case IdsNvMemCtrllerEccDramScrub_80ns:
    case IdsNvMemCtrllerEccDramScrub_160ns:
    case IdsNvMemCtrllerEccDramScrub_320ns:
    case IdsNvMemCtrllerEccDramScrub_640ns:
    case IdsNvMemCtrllerEccDramScrub_1_28us:
    case IdsNvMemCtrllerEccDramScrub_2_56us:
    case IdsNvMemCtrllerEccDramScrub_5_12us:
    case IdsNvMemCtrllerEccDramScrub_10_2us:
    case IdsNvMemCtrllerEccDramScrub_20_5us:
    case IdsNvMemCtrllerEccDramScrub_41_0us:
    case IdsNvMemCtrllerEccDramScrub_81_9us:
    case IdsNvMemCtrllerEccDramScrub_163_8us:
    case IdsNvMemCtrllerEccDramScrub_327_7us:
    case IdsNvMemCtrllerEccDramScrub_655_4us:
    case IdsNvMemCtrllerEccDramScrub_1_31ms:
    case IdsNvMemCtrllerEccDramScrub_2_62ms:
    case IdsNvMemCtrllerEccDramScrub_5_24ms:
    case IdsNvMemCtrllerEccDramScrub_10_49ms:
    case IdsNvMemCtrllerEccDramScrub_20_97ms:
    case IdsNvMemCtrllerEccDramScrub_42ms:
    case IdsNvMemCtrllerEccDramScrub_84ms:
    case IdsNvMemCtrllerEccDramScrub_20ns:
      PeccOverStruct->CfgScrubDramRate = (UINT16) NvValue;
      break;
    case IdsNvMemCtrllerEccDramScrubAuto:
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for SW Device ID Remapping
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSubGnbPmmSWDeviceIDRemappingF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  PCIe_PORT_CONFIG  *Port;
  IDS_STATUS NvValue;
  Port = (PCIe_PORT_CONFIG*) DataPtr;

  //SW Device ID Remapping
  IDS_NV_READ (NvValue, AGESA_IDS_NV_GNB_NB_SWDEVICEIDREMAPPING, IdsNvPtr, StdHeader)
  if (NvValue != IdsNvGnbNbSWDeviceIDRemappingManual) {
    return IDS_SUCCESS;
  }

  switch (Port->PcieBridgeId) {
  case  0:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR0DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  1:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR1DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  2:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR2DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  3:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR3DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  4:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR4DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  5:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR5DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  6:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR6DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  7:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR7DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  8:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR8DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  9:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR9DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  10:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR10DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  case  11:
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_NB_PBR11DEVFNMAP, IdsNvPtr, StdHeader) {
      Port->PortData.FunctionNumber = (UINT8) (NvValue & 0x7);
      Port->PortData.DeviceNumber = (UINT8) ((NvValue & 0xF8) >> 3);
    }
    break;
  default:
    ASSERT (FALSE);
    break;
  }

  return IDS_SUCCESS;
}


/**
 *  IDS Backend Function skip MSR Access Override
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSkipSubMSRAccessOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  IDS_STATUS Status;

  Status = IDS_SUCCESS;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_MSR_ACCESS, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    case  IdsNvCpuCacDbgLockPolling:
      Status = IDS_SUCCESS;
      break;
    case  IdsNvCpuCacDbgLockDoNotPolling:
      Status = IDS_UNSUPPORTED;
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  return Status;
}

/**
 *  IDS Backend Function skip cTDP Nb Pstate Selection Table feature
 *
 *  @param[in,out]   DataPtr      NULL
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *
 **/
IDS_STATUS
IdsIntSkipSubNpstF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS NvValue;
  IDS_STATUS Status;

  Status = IDS_SUCCESS;
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_CTDP_NPST, IdsNvPtr, StdHeader) {
    if (NvValue == 1) {
      Status = IDS_UNSUPPORTED;
    }
  }

  return Status;
}

/**
 *  IDS Backend Function for override PCIe PHY Config
 *
 *  @param[in,out]   DataPtr      The Pointer of BOOLEAN.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/

IDS_STATUS
IdsIntSubGnbPciePhyCfgF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  /// @todo
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function for set equalization presets
 *
 *  @param[in,out]   DataPtr      The Pointer to wrapper struct.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/

IDS_STATUS
IdsIntSubGnbEqualizationPresetF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS              NvValue;
  PCIe_PLATFORM_CONFIG    *Pcie;
  PCIe_WRAPPER_CONFIG     *Wrapper;
  PCIe_ENGINE_CONFIG      *EnginesList;

  IDS_HDT_CONSOLE (GNB_TRACE, "IdsSubGnbEqualizationPreset Enter\n");
  EnginesList = (PCIe_ENGINE_CONFIG *) DataPtr;
  Wrapper = PcieConfigGetParentWrapper (EnginesList);
  Pcie = PcieConfigGetPlatform (Wrapper);

  if ((EnginesList->Type.Port.StartCoreLane != UNUSED_LANE_ID) && (EnginesList->Type.Port.EndCoreLane != UNUSED_LANE_ID)) {
    // Search Mode
    IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_EQUALIZATION_SEARCH_MODE0 + EnginesList->Type.Port.PortId, IdsNvPtr, StdHeader) {
    PciePortRegisterRMW (
      EnginesList,
      DxFxxE4_xB6_ADDRESS,
      DxFxxE4_xB6_LcEqSearchMode_MASK,
      ((UINT32) NvValue << DxFxxE4_xB6_LcEqSearchMode_OFFSET),
      FALSE,
      Pcie
      );
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "IdsSubGnbEqualizationPreset Exit\n");
  return IDS_SUCCESS;
}

/**
 *  IDS Backend Function to override Gen1 loop back mode
 *
 *  @param[in,out]   DataPtr      The Pointer to BOOLEAN type.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/

IDS_STATUS
IdsIntSubGnbGen1LoopbackOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS              NvValue;
  PCIe_PLATFORM_CONFIG    *Pcie;
  PCIe_WRAPPER_CONFIG     *Wrapper;
  PCIe_ENGINE_CONFIG      *Engine;
  UINT8                   CoreId;

  IDS_HDT_CONSOLE (GNB_TRACE, "IdsIntSubGnbGen1LoopbackOverride Enter\n");
  Engine = (PCIe_ENGINE_CONFIG *) DataPtr;
  Wrapper = PcieConfigGetParentWrapper (Engine);
  Pcie = PcieConfigGetPlatform (Wrapper);

  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GEN1_LOOPBACK_MODE, IdsNvPtr, StdHeader) {
    if (NvValue == IdsNvGnbGen1LoopbackEnabled) {
      PciePortRegisterRMW (
        Engine,
        DxFxxE4_xB1_ADDRESS,
        DxFxxE4_xB1_LcElecIdleMode_MASK,
        0 << DxFxxE4_xB1_LcElecIdleMode_OFFSET,
        FALSE,
        Pcie
        );

      for (CoreId = Wrapper->StartPcieCoreId; CoreId <= Wrapper->EndPcieCoreId; CoreId++) {
        PcieRegisterRMW (
          Wrapper,
          CORE_SPACE (CoreId, D0F0xE4_CORE_0040_ADDRESS),
          D0F0xE4_CORE_0040_PElecIdleMode_MASK,
          0 << D0F0xE4_CORE_0040_PElecIdleMode_OFFSET,
          FALSE,
          Pcie
          );
      }
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "IdsIntSubGnbGen1LoopbackOverride Exit\n");
  return IDS_SUCCESS;
}


/**
 *  IDS Backend Function to configure gnb smu port80 monitor
 *
 *  @param[in,out]   DataPtr      The Pointer to BOOLEAN type.
 *  @param[in,out]   StdHeader The Pointer of AMD_CONFIG_PARAMS.
 *  @param[in]     IdsNvPtr     The Pointer of NV Table.
 *
 *  @retval IDS_SUCCESS         Backend function is called successfully.
 *  @retval IDS_UNSUPPORTED     No Backend function is found.
 *
 **/
IDS_STATUS
IdsIntSubGnbSmuPort80ParamsOverrideF15Cz (
  IN OUT   VOID *DataPtr,
  IN OUT   AMD_CONFIG_PARAMS *StdHeader,
  IN       IDS_NV_ITEM *IdsNvPtr
  )
{
  IDS_STATUS           NvValue;
  GNB_SMU_PORT80_PARAMS *SmuPort80;
  GNB_HANDLE           *GnbHandle;

  IDS_HDT_CONSOLE (GNB_TRACE, "IdsIntSubGnbSmuPort80ParamsF15Cz Enter\n");

  GnbHandle = GnbGetHandle (StdHeader);
  SmuPort80 = (GNB_SMU_PORT80_PARAMS *)DataPtr;

  //Port 80
  IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_EN, IdsNvPtr, StdHeader) {
    switch (NvValue) {
    //Auto
    case  0:
      break;
    //Manual
    case  1:
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_MEMLO, IdsNvPtr, StdHeader) {
        SmuPort80->MemoryBaseLo = (UINT32) (NvValue << 16) ;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_MEMHI, IdsNvPtr, StdHeader) {
        SmuPort80->MemoryBaseHi = (UINT32) NvValue;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_MEMSIZE, IdsNvPtr, StdHeader) {
        SmuPort80->MemoryBufferSize = (UINT16) NvValue;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_POLL, IdsNvPtr, StdHeader) {
        SmuPort80->PollingInterval = (UINT16) NvValue;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_CSR, IdsNvPtr, StdHeader) {
        SmuPort80->EnableCsrShadow = (UINT8) NvValue;
      }
      IDS_NV_READ_SKIP (NvValue, AGESA_IDS_NV_GNB_SMU_PORT80_DRAM, IdsNvPtr, StdHeader) {
        SmuPort80->EnableDramShadow = (UINT8) NvValue;
      }
      break;
    default:
      ASSERT (FALSE);
      break;
    }
  }

  IDS_HDT_CONSOLE (GNB_TRACE, "IdsIntSubGnbSmuPort80ParamsF15Cz Exit\n");
  return IDS_SUCCESS;
}

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatRdRandBlockF15Cz =
{
  IDS_INT_FEAT_WATCHDOG_TIMER,
  IDS_ALL_CORES,
  IDS_CPU_OVERRIDE_REG_AFTER_AP_LAUNCH,
  AMD_FAMILY_15_CZ,
  IdsIntFeatRdRandF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatWatchdogTimerAndSyncFloodMcaBlockF15Cz =
{
  IDS_INT_FEAT_WATCHDOG_TIMER,
  IDS_ALL_CORES,
  IDS_CPU_OVERRIDE_REG_AFTER_AP_LAUNCH,
  AMD_FAMILY_15_CZ,
  IdsIntFeatWatchdogTimerAndSyncFloodMcaF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatExceptionBreakpointBlockF15Cz =
{
  IDS_INT_FEAT_EXCEPTION_BP,
  IDS_ALL_CORES,
  IDS_CPU_OVERRIDE_REG_AFTER_AP_LAUNCH,
  AMD_FAMILY_15_CZ,
  IdsIntFeatExceptionBreakpointF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCoreLevelingBlockF15Cz =
{
  IDS_INT_FEAT_CORE_LEVELING,
  IDS_ALL_CORES,
  IDS_PLATFORMCFG_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsIntSubCoreLevelingF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatNbPsDisOverrideBlockF15Cz =
{
  IDS_FEAT_POSTPSTATE,
  IDS_ALL_CORES,
  IDS_NBPSDIS_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsIntSubNbPstateDisOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntSubNbPstateGetMinFreqBlockF15Cz =
{
  IDS_FEAT_POSTPSTATE,
  IDS_ALL_CORES,
  IDS_NBPS_MIN_FREQ,
  AMD_FAMILY_15_CZ,
  IdsIntSubNbPstateGetMinFreqCz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatCpbCtrlBlockF15Cz =
{
  IDS_FEAT_CPB_CTRL,
  IDS_BSP_ONLY,
  IDS_AFTER_FEAT_MID_INIT,
  AMD_FAMILY_15_CZ,
  IdsIntSubCpbConfigF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCStateModeBlockF15Cz =
{
  IDS_INT_FEAT_CSTATE_MODE,
  IDS_ALL_CORES,
  IDS_PLATFORMCFG_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsSubCModeF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatConfigIOCstatePostAfterBlockF15Cz =
{
  IDS_INT_FEAT_CSTATE_MODE,
  IDS_BSP_ONLY,
  IDS_INIT_POST_AFTER,
  AMD_FAMILY_15_CZ,
  IdsIntSubConfigIoCstateF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGetCstBlockF15Cz =
{
  IDS_INT_FEAT_CSTATE_MODE,
  IDS_ALL_CORES,
  IDS_CST_SIZE,
  AMD_FAMILY_15_CZ,
  IdsIntSubGetCstF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCreateCstBlockF15Cz =
{
  IDS_INT_FEAT_CSTATE_MODE,
  IDS_ALL_CORES,
  IDS_CST_CREATE,
  AMD_FAMILY_15_CZ,
  IdsIntSubCreateCstF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCacheFlushCtrlBlockF15Cz =
{
  IDS_INT_FEAT_CACHE_FLUSHCTRL,
  IDS_ALL_CORES,
  IDS_CACHE_FLUSH_HLT,
  AMD_FAMILY_15_CZ,
  IdsIntSubCacheControlF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryInitRegTblBlockF15Cz =
{
  IDS_INT_FEAT_MEMORY_TBL_DRV_SUPPORT,
  IDS_ALL_CORES,
  IDS_INIT_MEM_REG_TABLE,
  AMD_FAMILY_15_CZ,
  IdsIntSubInitMemRegTblF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryTblDrvInitBlockF15Cz =
{
  IDS_INT_FEAT_MEMORY_TBL_DRV_SUPPORT,
  IDS_ALL_CORES,
  IDS_INIT_DRAM_TABLE,
  AMD_FAMILY_15_CZ,
  IdsIntSubDramSettingDdr3F15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryOverrideMr0F15Cz =
{
  IDS_INT_FEAT_MEMORY_TBL_DRV_SUPPORT,
  IDS_ALL_CORES,
  IDS_MEM_MR0,
  AMD_FAMILY_15_CZ,
  IdsIntSubMemOverrideMr0Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyPowerSavingF15Cz =
{
  IDS_INT_FEAT_MEMORY_PHY_POWER_SAVING,
  IDS_ALL_CORES,
  IDS_MEM_PHY_POWER_SAVING,
  AMD_FAMILY_15_CZ,
  IdsIntSubMemPhyPowerSavingCz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryEnforceMClkAboveNClkF15Cz =
{
  IDS_INT_FEAT_MEMORY_MCLK_ABOVE_NCLK,
  IDS_ALL_CORES,
  IDS_MEM_MCLK_ABOVE_NCLK,
  AMD_FAMILY_15_CZ,
  IdsIntSubMemEnforceMClkAboveNClkCz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatMemoryMappingPostBeforeBlockF15Cz =
{
  IDS_FEAT_MEMORY_MAPPING,
  IDS_ALL_CORES,
  IDS_INIT_POST_BEFORE,
  AMD_FAMILY_15_CZ,
  IdsIntSubMemoryMappingF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyDllStandbyCtrlBeforeDramInitBlockF15Cz =
{
  IDS_INT_FEAT_MEMORY_PHYDLL_STANDBY_CTRL,
  IDS_ALL_CORES,
  IDS_BEFORE_DRAM_INIT,
  AMD_FAMILY_15_CZ,
  IdsPhyDLLStandbyControlBeforeDramInitF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatMemoryPhyDllStandbyCtrlBlockF15Cz =
{
  IDS_INT_FEAT_MEMORY_PHYDLL_STANDBY_CTRL,
  IDS_ALL_CORES,
  IDS_PHY_DLL_STANDBY_CTRL,
  AMD_FAMILY_15_CZ,
  IdsPhyDLLStandbyControlF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGNBORBDynamicWakeBlockF15Cz =
{
  IDS_INT_FEAT_GNB_ORBDYNAMIC_WAKE,
  IDS_ALL_CORES,
  IDS_GNB_ORBDYNAMIC_WAKE,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbORBDynamicWakeF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbPlatformCfgExBlockF15Cz =
{
  IDS_FEAT_GNB_PLATFORMCFG,
  IDS_ALL_CORES,
  IDS_GNB_PLATFORMCFG_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbPlatformCfgF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbPropertyOverrideBlockF15Cz =
{
  IDS_INT_FEAT_GNB_PROPERTY,
  IDS_ALL_CORES,
  IDS_GNB_PROPERTY,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbPropertyOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbPCIePowerGatingOverrideBlockF15Cz =
{
  IDS_INT_FEAT_GNB_PCIE_POWER_GATING,
  IDS_ALL_CORES,
  IDS_GNB_PCIE_POWER_GATING,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbPCIePowerGatingOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbSWTJOFFSETOverrideBlockF15Cz =
{
  IDS_INT_FEAT_GNB_PMM_SWTJOFFSET,
  IDS_ALL_CORES,
  IDS_GNB_PMM_SWTJOFFSET,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbSwTjOffsetOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatSkipLockDramCfgF15Cz =
{
  IDS_INT_FEAT_LOCK_DRAM_CFG,
  IDS_ALL_CORES,
  IDS_LOCK_DRAM_CFG,
  AMD_FAMILY_15_CZ,
  IdsIntSkipSubLockDramCfgF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGetStretchFreqBlockF15Cz =
{
  IDS_INT_FEAT_ENFORCE_FREQ,
  IDS_ALL_CORES,
  IDS_GET_STRETCH_FREQUENCY_LIMIT,
  AMD_FAMILY_15_CZ,
  IdsGetStretchMemFreqF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatDisableCpuFeatF15Cz =
{
  IDS_FEAT_COMMON,
  IDS_ALL_CORES,
  IDS_CPU_FEAT,
  AMD_FAMILY_15_CZ,
  IdsDisableCpuFeatF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatNBPStatesSupportBlockF15Cz =
{
  IDS_INT_FEAT_NB_PSTATES,
  IDS_ALL_CORES,
  IDS_PLATFORMCFG_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsSubNBPStatesSupportOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatCurrentLimitOverrideF15Cz =
{
  IDS_INT_FEAT_CURRENT_LIMIT,
  IDS_ALL_CORES,
  IDS_PLATFORMCFG_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsCurrentLimitOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatAPIC330MaskConfigF15Cz =
{
  IDS_FEAT_COMMON,
  IDS_ALL_CORES,
  IDS_AFTER_EARLY_INIT_ONCORE,
  AMD_FAMILY_15_CZ,
  IdsIntAPIC330MaskConfigF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsIntFeatEccConfigurationF15Cz =
{
  IDS_FEAT_MEMORY_MAPPING,
  IDS_ALL_CORES,
  IDS_ECC,
  AMD_FAMILY_15_CZ,
  IdsSubEccConfigurationF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbPmmSWDeviceIDRemappingF15Cz =
{
  IDS_INT_FEAT_GNB_PMM_PORT_REMAP,
  IDS_ALL_CORES,
  IDS_GNB_PCIE_PORT_REMAP,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbPmmSWDeviceIDRemappingF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatSkipMsrAccessOverrideF15Cz =
{
  IDS_INT_FEAT_MSR_ACCESS_OVERRIDE,
  IDS_ALL_CORES,
  IDS_MSR_ACCESS_OVERRIDE,
  AMD_FAMILY_15_CZ,
  IdsIntSkipSubMSRAccessOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatSkipNpstF15Cz =
{
  IDS_INT_FEAT_CTDP_NPST,
  IDS_BSP_ONLY,
  IDS_NPST,
  AMD_FAMILY_15_CZ,
  IdsIntSkipSubNpstF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbPciePhyCfgF15Cz =
{
  IDS_INT_FEAT_PCIE_PHYLANE_CONFIG,
  IDS_ALL_CORES,
  IDS_GNB_PCIE_PHYLANE_CONFIG,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbPciePhyCfgF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatDsmLpCfgF15Cz =
{
  IDS_INT_FEAT_DSM_LP,
  IDS_ALL_CORES,
  IDS_DSM_LP_SELECTION,
  AMD_FAMILY_15_CZ,
  IdsIntSubDsmLpCfgF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbEqualizationPresetF15Cz =
{
  IDS_INT_FEAT_GNB_EQUAL_PRESET,
  IDS_ALL_CORES,
  IDS_GNB_EQUAL_PRESET,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbEqualizationPresetF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbSmuPort80ParamsOverrideF15Cz =
{
  IDS_INT_FEAT_GNB_SMU_PORT80_PARAMS,
  IDS_ALL_CORES,
  IDS_GNB_SMU_PORT80_PARAMS,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbSmuPort80ParamsOverrideF15Cz
};

CONST IDS_FAMILY_FEAT_STRUCT ROMDATA IdsFeatGnbGen1LoopbackOverrideF15Cz =
{
  IDS_INT_FEAT_GNB_GEN1_LOOPBACK,
  IDS_ALL_CORES,
  IDS_GNB_GEN1_LOOPBACK,
  AMD_FAMILY_15_CZ,
  IdsIntSubGnbGen1LoopbackOverrideF15Cz
};
