/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 15h CZ models 60h - 6Fh specific utility functions.
 *
 * Provides numerous utility functions specific to family 15h CZ.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 309899 $   @e \$Date: 2014-12-23 02:21:13 -0600 (Tue, 23 Dec 2014) $
 *
 */
/*
 ******************************************************************************
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
 ******************************************************************************
 */

/*----------------------------------------------------------------------------------------
 *                             M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15CzPowerMgmt.h"
#include "cpuEarlyInit.h"
#include "cpuF15CzUtilities.h"
#include "Gnb.h"
#include "GnbLib.h"
#include "GnbSmuInitLibV8.h"
#include "heapManager.h"
#include "Filecode.h"
CODE_GROUP (G2_PEI)
RDATA_GROUP (G2_PEI)
#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZUTILITIES_FILECODE

/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern REGISTER_TABLE_AT_GIVEN_TP F15CzRegTableListAtGivenTP[];

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
#define MAX_NB_PSTATE_FUSE 8
#define NPST_NUM 4

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

VOID
STATIC
F15CzNbPstateDisCore (
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
WaitForNbTransitionToComplete (
  IN       UINT8              NbPsHiOrLow,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

VOID
STATIC
F15CzGetNbFreqNumeratorInMHz (
  IN       UINT32                NbFid,
     OUT   UINT32                *FreqNumeratorInMHz,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F15CzGetNbFreqDivisor (
  IN       UINT32                NbDid,
     OUT   UINT32                *FreqDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

AGESA_STATUS
STATIC
F15CzCalculateNbFrequencyInMHz (
  IN       UINT32                NbFid,
  IN       UINT32                NbDid,
     OUT   UINT32                *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

VOID
STATIC
F15CzCovertVidInuV (
  IN       UINT32                Vid,
     OUT   UINT32                *VoltageInuV,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F15CzCmnGetIddDivisor (
  IN       UINT32                IddDiv,
     OUT   UINT32                *IddDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F15CzCmnCalculateCurrentInmA (
  IN       UINT32                IddValue,
  IN       UINT32                IddDiv,
     OUT   UINT32                *CurrentInmA,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  );

VOID
STATIC
F15CzModifyNbPstate (
  IN       NB_PSTATE_REGISTER  *NbFuse,
  IN       UINT8                NbPstateNum,
  IN       UINT8                Selection,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

VOID
STATIC
F15CzGetNbPstateFromFuse (
     OUT   NB_PSTATE_REGISTER  *NbFuse,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

VOID
STATIC
F15CzGetNbPstateSelectionTable (
     OUT   NB_PSTATE_SELECTION_TABLE  *NbSelTbl,
  IN       PLATFORM_CONFIGURATION     *PlatformConfig,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  );

BOOLEAN
F15CzSetDownCoreRegister (
  IN       CPU_CORE_LEVELING_FAMILY_SERVICES *FamilySpecificServices,
  IN       UINT32 *Socket,
  IN       UINT32 *Module,
  IN       UINT32 *LeveledCores,
  IN       CORE_LEVELING_TYPE CoreLevelMode,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

REGISTER_TABLE_AT_GIVEN_TP *
F15CzGetRegisterTableList (
  IN       CPU_SPECIFIC_SERVICES                *FamilyServices,
  IN       AMD_CONFIG_PARAMS                    *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Get CPU pstate current.
 *
 *  @CpuServiceMethod{::F_CPU_GET_IDD_MAX}.
 *
 *    This function returns the ProcIddMax.
 *
 *  @param[in]     FamilySpecificServices    The current Family Specific Services.
 *  @param[in]     Pstate                    The P-state to check.
 *  @param[out]    ProcIddMax                P-state current in mA.
 *  @param[in]     StdHeader                 Handle of Header for calling lib functions and services.
 *
 *  @retval        TRUE                      P-state is enabled
 *  @retval        FALSE                     P-state is disabled
 */
BOOLEAN
F15CzGetProcIddMax (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       UINT8                  Pstate,
     OUT   UINT32                 *ProcIddMax,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32       MsrAddress;
  PSTATE_MSR   PstateMsr;
  BOOLEAN      IsPstateEnabled;
  PCI_ADDR     PciAddress;
  NB_CAPS_2_REGISTER  NbCap2;
  UINT32       ProcIddMaxPerCore;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetProcIddMax - P%d\n", Pstate);

  IsPstateEnabled = FALSE;

  MsrAddress = (UINT32) (Pstate + PS_REG_BASE);
  ASSERT (MsrAddress <= PS_MAX_REG);

  LibAmdMsrRead (MsrAddress, (UINT64 *) &PstateMsr, StdHeader);
  if (PstateMsr.PsEnable == 1) {
    F15CzCmnCalculateCurrentInmA ((UINT32) PstateMsr.IddValue, (UINT32) PstateMsr.IddDiv, &ProcIddMaxPerCore, StdHeader);
    PciAddress.AddressValue = NB_CAPS_REG2_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbCap2, StdHeader);
    *ProcIddMax = (UINT32) ProcIddMaxPerCore * (NbCap2.CmpCap + 1);
    IDS_HDT_CONSOLE (CPU_TRACE, "    Pstate %d ProcIddMax %d CmpCap %d\n", Pstate, *ProcIddMax, NbCap2.CmpCap);
    IsPstateEnabled = TRUE;
  }
  return IsPstateEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set down core register on Carrizo
 *
 * This function set F3x190 Downcore Control Register[5:0]
 *
 * @param[in]   FamilySpecificServices   The current Family Specific Services.
 * @param[in]   Socket                   Socket ID.
 * @param[in]   Module                   Module ID in socket.
 * @param[in]   LeveledCores             Number of core.
 * @param[in]   CoreLevelMode            Core level mode.
 * @param[in]   StdHeader                Header for library and services.
 *
 * @retval      TRUE                     Down Core register is updated.
 * @retval      FALSE                    Down Core register is not updated.
 */
BOOLEAN
F15CzSetDownCoreRegister (
  IN       CPU_CORE_LEVELING_FAMILY_SERVICES *FamilySpecificServices,
  IN       UINT32 *Socket,
  IN       UINT32 *Module,
  IN       UINT32 *LeveledCores,
  IN       CORE_LEVELING_TYPE CoreLevelMode,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32    LocalPciRegister;
  UINT32    CoreDisableBits;
  PCI_ADDR  PciAddress;
  BOOLEAN   IsUpdated;

  IsUpdated = FALSE;

  switch (CoreLevelMode) {
  case CORE_LEVEL_ONE:
    CoreDisableBits = DOWNCORE_MASK_SINGLE;
    break;
  case CORE_LEVEL_TWO:
    CoreDisableBits = DOWNCORE_MASK_DUAL;
    break;
  default:
    CoreDisableBits = 0;
  }

  if (CoreDisableBits != 0) {
    PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_5, NORTH_BRIDGE_CAPABILITIES_2_REG);
    LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
    LocalPciRegister = (LocalPciRegister & 0xFF) + 1;
    LocalPciRegister = (1 << LocalPciRegister) - 1;
    CoreDisableBits &= LocalPciRegister;

    PciAddress.Address.Function = FUNC_3;
    PciAddress.Address.Register = DOWNCORE_CTRL;
    LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
    if ((LocalPciRegister | CoreDisableBits) != LocalPciRegister) {
      LocalPciRegister |= CoreDisableBits;
      LibAmdPciWrite (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader);
      IsUpdated = TRUE;
    }
  }

  return IsUpdated;
}


CONST CPU_CORE_LEVELING_FAMILY_SERVICES ROMDATA F15CzCoreLeveling =
{
  0,
  F15CzSetDownCoreRegister
};


/*---------------------------------------------------------------------------------------*/
/**
 *  Determines the NB clock on the desired node.
 *
 *  @CpuServiceMethod{::F_CPU_GET_NB_FREQ}.
 *
 *  @param[in]   FamilySpecificServices   The current Family Specific Services.
 *  @param[out]  FrequencyInMHz           Northbridge clock frequency in MHz.
 *  @param[in]   StdHeader                Header for library and services.
 *
 *  @return      AGESA_SUCCESS            FrequencyInMHz is valid.
 */
AGESA_STATUS
F15CzGetCurrentNbFrequency (
  IN       CPU_SPECIFIC_SERVICES *FamilySpecificServices,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  NB_PSTATE_STS_REGISTER NbPstateStsReg;
  PCI_ADDR     PciAddress;
  AGESA_STATUS ReturnCode;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetCurrentNbFrequency\n");

  PciAddress.AddressValue = NB_PSTATE_STATUS_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateStsReg, StdHeader);
  ReturnCode = F15CzCalculateNbFrequencyInMHz (
                 GetF15CzCurNbFid (&NbPstateStsReg),
                 NbPstateStsReg.CurNbDid,
                 FrequencyInMHz,
                 StdHeader
                 );
  return ReturnCode;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Returns the node's minimum and maximum northbridge frequency.
 *
 *  @CpuServiceMethod{::F_CPU_GET_MIN_MAX_NB_FREQ}.
 *
 *  @param[in]     FamilySpecificServices  The current Family Specific Services.
 *  @param[in]     PlatformConfig          Platform profile/build option config structure.
 *  @param[in]     PciAddress              The segment, bus, and device numbers of the CPU in question.
 *  @param[out]    MinFreqInMHz            The node's minimum northbridge frequency.
 *  @param[out]    MaxFreqInMHz            The node's maximum northbridge frequency.
 *  @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 *  @retval        AGESA_SUCCESS           Northbridge frequency is valid
 */
AGESA_STATUS
F15CzGetMinMaxNbFrequency (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       PCI_ADDR               *PciAddress,
     OUT   UINT32                 *MinFreqInMHz,
     OUT   UINT32                 *MaxFreqInMHz,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  INT8         NbPsMaxVal;
  UINT8        i;
  UINT32       LocalPciRegister;
  AGESA_STATUS AgesaStatus;

  AgesaStatus = AGESA_ERROR;

  // Obtain the max NB frequency on the node
  PciAddress->Address.Function = FUNC_5;
  PciAddress->Address.Register = NB_PSTATE_CTRL;
  LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
  NbPsMaxVal = (INT8) ((NB_PSTATE_CTRL_REGISTER *) &LocalPciRegister)->NbPstateMaxVal;

  // Starting from NB Pmax
  for (i = 0; i <= NbPsMaxVal; i++) {
    PciAddress->Address.Function = FUNC_5;
    PciAddress->Address.Register = (NB_PSTATE_0 + (4 * i));
    LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);

    // Ensure that the NB Pstate is enabled
    if (((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbPstateEn == 1) {
      AgesaStatus = F15CzCalculateNbFrequencyInMHz (GetF15CzNbFid (&LocalPciRegister),
                                                    ((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbDid,
                                                    MaxFreqInMHz,
                                                    StdHeader);
      break;
    }
  }
  // If all of NbPstates are disabled, get MaxFreqInMHz from CurNbPstate
  if (i > NbPsMaxVal) {
    PciAddress->Address.Register = NB_PSTATE_STATUS;
    LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
    F15CzCalculateNbFrequencyInMHz (GetF15CzCurNbFid (&LocalPciRegister),
                                      ((NB_PSTATE_STS_REGISTER *) &LocalPciRegister)->CurNbDid,
                                      MaxFreqInMHz,
                                      StdHeader);
    // No one NbPstate is enabled, so set Min = Max
    *MinFreqInMHz = *MaxFreqInMHz;
    ASSERT (FALSE);
  } else {
    // If NB P-states are disabled, return the NB P0 frequency
    // as both the min and max frequency on the node.
    if (!FamilySpecificServices->IsNbPstateEnabled (FamilySpecificServices, PlatformConfig, StdHeader)) {
      *MinFreqInMHz = *MaxFreqInMHz;
    } else {
      PciAddress->Address.Function = FUNC_5;
      PciAddress->Address.Register = NB_PSTATE_CTRL;
      LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
      NbPsMaxVal = (INT8) ((NB_PSTATE_CTRL_REGISTER *) &LocalPciRegister)->NbPstateMaxVal;

      // Obtain the min NB frequency on the node, starting from NB Pmin
      for (NbPsMaxVal; NbPsMaxVal >= 0; NbPsMaxVal--) {
        PciAddress->Address.Function = FUNC_5;
        PciAddress->Address.Register = (NB_PSTATE_0 + (4 * NbPsMaxVal));
        LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);

        // Ensure that the NB Pstate is enabled
        if (((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbPstateEn == 1) {
          AgesaStatus = F15CzCalculateNbFrequencyInMHz (GetF15CzNbFid (&LocalPciRegister),
                                                        ((NB_PSTATE_REGISTER *) &LocalPciRegister)->NbDid,
                                                        MinFreqInMHz,
                                                        StdHeader);
          break;
        }
      }
    }
  }
  IDS_OPTION_HOOK (IDS_NBPS_MIN_FREQ, MinFreqInMHz, StdHeader);

  return AgesaStatus;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Determines the NB clock on the desired node.
 *
 *  @CpuServiceMethod{::F_CPU_GET_NB_PSTATE_INFO}.
 *
 *  @param[in]     FamilySpecificServices  The current Family Specific Services.
 *  @param[in]     PlatformConfig          Platform profile/build option config structure.
 *  @param[in]     PciAddress              The segment, bus, and device numbers of the CPU in question.
 *  @param[in]     NbPstate                The NB P-state number to check.
 *  @param[out]    FreqNumeratorInMHz      The desired node's frequency numerator in megahertz.
 *  @param[out]    FreqDivisor             The desired node's frequency divisor.
 *  @param[out]    VoltageInuV             The desired node's voltage in microvolts.
 *  @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 *  @retval        TRUE                    NbPstate is valid
 *  @retval        FALSE                   NbPstate is disabled or invalid
 */
BOOLEAN
F15CzGetNbPstateInfo (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       PCI_ADDR               *PciAddress,
  IN       UINT32                 NbPstate,
     OUT   UINT32                 *FreqNumeratorInMHz,
     OUT   UINT32                 *FreqDivisor,
     OUT   UINT32                 *VoltageInuV,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32   NbVid;
  BOOLEAN  PstateIsValid;
  NB_PSTATE_CTRL_REGISTER   NbPstateCtrlReg;
  NB_PSTATE_REGISTER        NbPstateReg;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetNbPstateInfo - NB P%d\n", NbPstate);

  ASSERT ((PciAddress->Address.Segment == 0) && (PciAddress->Address.Bus == 0) && (PciAddress->Address.Device == 0x18));

  PstateIsValid = FALSE;

  // If NB P1, P2, or P3 is requested, make sure that NB Pstate is enabled
  if ((NbPstate == 0) || (FamilySpecificServices->IsNbPstateEnabled (FamilySpecificServices, PlatformConfig, StdHeader))) {
    PciAddress->AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, *PciAddress, &NbPstateCtrlReg, StdHeader);

    ASSERT ((NbPstate < NM_NB_PS_REG) && (NbPstateCtrlReg.NbPstateMaxVal < NM_NB_PS_REG));
    if (NbPstate <= NbPstateCtrlReg.NbPstateMaxVal) {
      PciAddress->Address.Register = (NB_PSTATE_0 + (sizeof (NB_PSTATE_REGISTER) * NbPstate));
      LibAmdPciRead (AccessWidth32, *PciAddress, &NbPstateReg, StdHeader);
      IDS_HDT_CONSOLE (CPU_TRACE, "    En:%d Fid:%x Did:%x Vid:%x\n", NbPstateReg.NbPstateEn, GetF15CzNbFid (&NbPstateReg), NbPstateReg.NbDid, GetF15CzNbVid (&NbPstateReg));

      // Check if at least NB P0 is enabled.
      ASSERT ((NbPstate == 0) ? (NbPstateReg.NbPstateEn == 1) : TRUE);
      // Ensure that requested NbPstate is enabled
      if (NbPstateReg.NbPstateEn == 1) {
        // Check for P-state Bandwidth Requirements on
        // "All NB P-states must be defined such that D18F5x1[6C:60][NbFid] <= 2Eh"
        ASSERT (GetF15CzNbFid (&NbPstateReg) <= 0x2E);
        F15CzGetNbFreqNumeratorInMHz (GetF15CzNbFid (&NbPstateReg), FreqNumeratorInMHz, StdHeader);
        F15CzGetNbFreqDivisor (NbPstateReg.NbDid, FreqDivisor, StdHeader);
        // Check for P-state Bandwidth Requirements on
        // "400MHz <= NBCOF <= 1600MHz"
        ASSERT ((*FreqNumeratorInMHz / *FreqDivisor) >= 400);
        ASSERT ((*FreqNumeratorInMHz / *FreqDivisor) <= 1600);

        NbVid = GetF15CzNbVid (&NbPstateReg);
        F15CzCovertVidInuV  (NbVid, VoltageInuV, StdHeader);
        PstateIsValid = TRUE;
        IDS_HDT_CONSOLE (CPU_TRACE, "    NB Pstate %d is Valid. NbVid=%d VoltageInuV=%d\n", NbPstate, NbVid, *VoltageInuV);
      }
    }
  }
  return PstateIsValid;
}


/*---------------------------------------------------------------------------------------*/
/**
 *  Get NB pstate current.
 *
 *  @CpuServiceMethod{::F_CPU_GET_NB_IDD_MAX}.
 *
 *    This function returns the NbIddMax.
 *
 *  @param[in]     FamilySpecificServices    The current Family Specific Services.
 *  @param[in]     NbPstate                  The NB P-state to check.
 *  @param[out]    NbIddMax                  NB P-state current in mA.
 *  @param[in]     StdHeader                 Handle of Header for calling lib functions and services.
 *
 *  @retval        TRUE                      NB P-state is enabled, and NbIddMax is valid.
 *  @retval        FALSE                     NB P-state is disabled
 */
BOOLEAN
F15CzGetNbIddMax (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       UINT8                  NbPstate,
     OUT   UINT32                 *NbIddMax,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  BOOLEAN      IsNbPsEnabled;
  PCI_ADDR     PciAddress;
  NB_PSTATE_CTRL_REGISTER   NbPstateCtrlReg;
  NB_PSTATE_REGISTER        NbPstateReg;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetNbIddMax - NB P%d\n", NbPstate);

  IsNbPsEnabled = FALSE;

  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateCtrlReg, StdHeader);

  ASSERT (NbPstate < NM_NB_PS_REG);
  if (NbPstate <= NbPstateCtrlReg.NbPstateMaxVal) {
    PciAddress.Address.Register = (NB_PSTATE_0 + (sizeof (NB_PSTATE_REGISTER) * NbPstate));
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateReg, StdHeader);

    // Ensure that requested NbPstate is enabled
    if (NbPstateReg.NbPstateEn == 1) {
      F15CzCmnCalculateCurrentInmA (NbPstateReg.NbIddValue, NbPstateReg.NbIddDiv, NbIddMax, StdHeader);
      IsNbPsEnabled = TRUE;
      IDS_HDT_CONSOLE (CPU_TRACE, "  NB Pstate %d is Valid. NbIddMax %d\n", NbPstate, *NbIddMax);
    }
  }
  return IsNbPsEnabled;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get the number of physical cores of current processor.
 *
 * @CpuServiceMethod{::F_CPU_NUMBER_OF_PHYSICAL_CORES}.
 *
 * @param[in]      FamilySpecificServices         The current Family Specific Services.
 * @param[in]      StdHeader                      Handle of Header for calling lib functions and services.
 *
 * @return         The number of physical cores.
 */
UINT8
F15CzGetNumberOfPhysicalCores (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CPUID_DATA    CpuId;

  //
  //CPUID.80000008h.ECX.NC + 1, 000b = 1, 001b = 2, etc.
  //
  LibAmdCpuidRead (CPUID_LONG_MODE_ADDR, &CpuId, StdHeader);
  return ((UINT8) ((CpuId.ECX_Reg & 0xff) + 1));
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Get this AP's system core number from hardware.
 *
 *  @CpuServiceMethod{::F_CPU_GET_AP_CORE_NUMBER}.
 *
 *  Returns the system core number from the scratch MSR, where
 *  it was saved at heap initialization.
 *
 *  @param[in]     FamilySpecificServices  The current Family Specific Services.
 *  @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 *  @return        The AP's unique core number
 */
UINT32
F15CzGetApCoreNumber (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CPUID_DATA Cpuid;

  LibAmdCpuidRead (0x1, &Cpuid, StdHeader);
  return ((Cpuid.EBX_Reg >> 24) & 0xFF);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Is the Northbridge PState feature enabled?
 *
 * @CpuServiceMethod{::F_IS_NB_PSTATE_ENABLED}.
 *
 * @param[in]      FamilySpecificServices         The current Family Specific Services.
 * @param[in]      PlatformConfig                 Platform profile/build option config structure.
 * @param[in]      StdHeader                      Handle of Header for calling lib functions and services.
 *
 * @retval         TRUE                           The NB PState feature is enabled.
 * @retval         FALSE                          The NB PState feature is not enabled.
 */
BOOLEAN
F15CzIsNbPstateEnabled (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  PCI_ADDR PciAddress;
  BOOLEAN  NbPstatesSupported;
  BOOLEAN  SkipHwCfg;
  NB_PSTATE_STS_REGISTER   NbPstateSts;
  NB_PSTATE_CTRL_REGISTER  NbPstateCtrl;


  PciAddress.AddressValue = NB_PSTATE_STATUS_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NbPstateSts, StdHeader);
  if (NbPstateSts.NbPstateDis == 1) {
    return FALSE;
  }

  SkipHwCfg = FALSE;
  IDS_OPTION_HOOK (IDS_NBPSDIS_OVERRIDE, &SkipHwCfg, StdHeader);

  // NbPstates is supported by default
  NbPstatesSupported = TRUE;

  // Get user options
  if (!PlatformConfig->PlatformProfile.NbPstatesSupported) {
    NbPstatesSupported = FALSE;
  }

  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NbPstateCtrl, StdHeader);
  if (((NbPstateCtrl.NbPstateMaxVal != 0) || SkipHwCfg) && (NbPstatesSupported)) {
    return TRUE;
  }
  return FALSE;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Is the NPST feature enabled?
 *
 * @param[in]      StdHeader                      Handle of Header for calling lib functions and services.
 *
 * @retval         TRUE                           The NPST feature is enabled.
 * @retval         FALSE                          The NPST feature is not enabled.
*/
BOOLEAN
F15CzIsNPstEnabled (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT16                    ConfigID;
  UINT32                    SmuIndex;
  UINT32                    SmuData;
  PCI_ADDR                  PciAddress;
  BOOLEAN                   IsCzA0;
  NATIVE_DIE_CPUID_REGISTER NativeDieCpuId;

  IsCzA0 = FALSE;
  // Get CPU Stepping from D18F4x160
  PciAddress.AddressValue = NATIVE_DIE_CPUID_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, (VOID *) &NativeDieCpuId, StdHeader);
  if ((NativeDieCpuId.ExtFamily == 0x6) && (NativeDieCpuId.ExtModel == 0x6) &&
      (NativeDieCpuId.BaseFamily == 0xF) && (NativeDieCpuId.BaseModel == 0x0) && (NativeDieCpuId.Stepping == 0x0)) {
    IsCzA0 = TRUE;
  }

  // Continue checking Config ID only if this is a CZ A0 part
  if (IsCzA0) {
    // Get Config ID from 0xC0016024
    SmuIndex = 0xC0016024;
    GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
    ConfigID = (UINT16) ((SmuData >> 15) & 0xFFF);

    // Disable NPST for these CZ A0 specific Config ID
    if ((ConfigID == 0x14) || (ConfigID == 0x17) || (ConfigID == 0x18) || (ConfigID == 0x1C)) {
      return FALSE;
    }
  }

  return TRUE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Config Nb-Pstate based on selection table
 *
 * @param[in]     FamilySpecificServices  The current Family Specific Services
 * @param[in]     CpuEarlyParamsPtr       Service Parameters
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 */
VOID
F15CzNbPstateConfig (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS   *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT8 i;
  UINT8 CurrentNbPstate;
  UINT8 SelForCurrentNb;
  UINT8 *Selection;
  PCI_ADDR PciAddress;
  NB_PSTATE_CTRL_REGISTER NbCtrlOnEntry;
  NB_PSTATE_STS_REGISTER  NbSts;
  NB_PSTATE_REGISTER NbFuse[MAX_NB_PSTATE_FUSE];
  NB_PSTATE_SELECTION_TABLE NbSelectionTable;
  ALLOCATE_HEAP_PARAMS AllocateHeap;

  IDS_SKIP_HOOK (IDS_NPST, NULL, StdHeader) {
    if (F15CzIsNPstEnabled (StdHeader)) {
      IDS_HDT_CONSOLE (CPU_TRACE, "  NPST is enabled\n");

      // Get Nb-Pstate selection table based on TDP
      F15CzGetNbPstateSelectionTable (&NbSelectionTable, &(CpuEarlyParamsPtr->PlatformConfig), StdHeader);

      // Collect all Nb-Pstate setting from FUSE
      F15CzGetNbPstateFromFuse (&NbFuse[0], StdHeader);

      IDS_HDT_CONSOLE (CPU_TRACE, "    MaxTdp: %d, NbP0Sel: %d, NbP1Sel: %d, NbP2Sel: %d, NbP3Sel: %d, DDrMaxRate: %x\n",
                                       NbSelectionTable.MaxTdp,
                                       NbSelectionTable.NbP0Sel,
                                       NbSelectionTable.NbP1Sel,
                                       NbSelectionTable.NbP2Sel,
                                       NbSelectionTable.NbP3Sel,
                                       NbSelectionTable.DdrMaxRate);


      PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbCtrlOnEntry, StdHeader);

      PciAddress.AddressValue = NB_PSTATE_STATUS_PCI_ADDR;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbSts, StdHeader);

      // We can't rely on D18F5x174[CurNbPstateLo] until a transition occurs
      if ((NbSts.StartupNbPstate != NbCtrlOnEntry.NbPstateHi) &&
          (NbSts.StartupNbPstate != NbCtrlOnEntry.NbPstateLo)) {
        TransitionToNbLow (StdHeader);
        LibAmdPciRead (AccessWidth32, PciAddress, &NbSts, StdHeader);
      }

      if (NbSts.CurNbPstateLo == 0) {
        // Current NbPstate is NbHi
        CurrentNbPstate = (UINT8) NbCtrlOnEntry.NbPstateHi;
        IDS_HDT_CONSOLE (CPU_TRACE, "    Current in NbHi, which is NbP%d\n", CurrentNbPstate);
      } else {
        // Current NbPstate is NbLo
        CurrentNbPstate = (UINT8) NbCtrlOnEntry.NbPstateLo;
        IDS_HDT_CONSOLE (CPU_TRACE, "    Current in NbLo, which is NbP%d\n", CurrentNbPstate);
      }

      Selection = &(NbSelectionTable.NbP0Sel);
      SelForCurrentNb = 0xFF;

      IDS_HDT_CONSOLE (CPU_TRACE, "    Begin to modify all Nb Pstates\n");
      PciAddress.AddressValue = MAKE_SBDFO (0, 0, 0x18, FUNC_5, 0);
      // Program Nb-Pstate based on selection table
      for (i = 0; i < NM_NB_PS_REG; i++) {
        ASSERT (*Selection < MAX_NB_PSTATE_FUSE);
        if (i == CurrentNbPstate) {
          SelForCurrentNb = *Selection;
          Selection++;
          continue;
        }
        F15CzModifyNbPstate (&NbFuse[0], i, *Selection, StdHeader);
        Selection++;
      }

      ASSERT (SelForCurrentNb != 0xFF);
      if (NbSts.CurNbPstateLo == 0) {
        // Current NbPstate is NbHi
        TransitionToNbLow (StdHeader);
        F15CzModifyNbPstate (&NbFuse[0], CurrentNbPstate, SelForCurrentNb, StdHeader);
        TransitionToNbHigh (StdHeader);
      } else {
        TransitionToNbHigh (StdHeader);
        F15CzModifyNbPstate (&NbFuse[0], CurrentNbPstate, SelForCurrentNb, StdHeader);
        TransitionToNbLow (StdHeader);
      }
      // Restore D18F5x170
      LibAmdPciWrite (AccessWidth32, PciAddress, &NbCtrlOnEntry, StdHeader);

      // Save DdrMaxRate to heap
      AllocateHeap.BufferHandle = AMD_MEM_DDR_MAX_RATE_HANDLE;
      AllocateHeap.RequestedBufferSize = sizeof (UINT32);
      AllocateHeap.Persist = HEAP_SYSTEM_MEM;
      if (HeapAllocateBuffer (&AllocateHeap, StdHeader) == AGESA_SUCCESS) {
        *((UINT32 *) AllocateHeap.BufferPtr) = NbSelectionTable.DdrMaxRate;
      }
    }
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Modify Nb Pstate registers
 *
 * @param[in]   NbFuse       All Nb-Pstate setting from FUSE.
 * @param[in]   NbPstateNum  Which NbPstate you want to modify.
 * @param[in]   Selection    Modify NbPstate based on Selection.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
STATIC
F15CzModifyNbPstate (
  IN       NB_PSTATE_REGISTER  *NbFuse,
  IN       UINT8                NbPstateNum,
  IN       UINT8                Selection,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  PCI_ADDR PciAddress;
  NB_PSTATE_REGISTER NbPstateX;

  PciAddress.AddressValue = NB_PSTATE_0_PCI_ADDR;

  PciAddress.Address.Register = NB_PSTATE_0 + NbPstateNum * 4;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPstateX, StdHeader);

  NbPstateX.NbFid_5_0 = NbFuse[Selection].NbFid_5_0;
  NbPstateX.NbDid = NbFuse[Selection].NbDid;
  NbPstateX.NbVid_6_0 = NbFuse[Selection].NbVid_6_0;
  NbPstateX.NbVid_7 = NbFuse[Selection].NbVid_7;
  NbPstateX.NbIddDiv = NbFuse[Selection].NbIddDiv;
  NbPstateX.NbIddValue = NbFuse[Selection].NbIddValue;
  NbPstateX.MemPstate = NbFuse[Selection].MemPstate;

  LibAmdPciWrite (AccessWidth32, PciAddress, &NbPstateX, StdHeader);
  IDS_HDT_CONSOLE (CPU_TRACE, "    Nb P%d: Fid %x, Did %x, Vid %x, IddDiv %x, IddValue %x, MemPstate %d\n",
                                      NbPstateNum,
                                      NbFuse[Selection].NbFid_5_0,
                                      NbFuse[Selection].NbDid,
                                      NbFuse[Selection].NbVid_6_0,
                                      NbFuse[Selection].NbIddDiv,
                                      NbFuse[Selection].NbIddValue,
                                      NbFuse[Selection].MemPstate);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get Nb-Pstates setting from FUSE
 *
 * @param[out]  NbFuse       All Nb-Pstate setting from FUSE.
 * @param[in]   StdHeader    Config handle for library and services.
 *
 */
VOID
STATIC
F15CzGetNbPstateFromFuse (
     OUT   NB_PSTATE_REGISTER  *NbFuse,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32 SmuIndex;
  UINT32 SmuData;

  SmuIndex = 0xC0016000;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[0].NbVid_6_0 = (SmuData >> 16) & 0x7F;
  NbFuse[0].NbVid_7 = (SmuData >> 23) & 0x1;
  NbFuse[1].NbVid_6_0 = (SmuData >> 24) & 0x7F;
  NbFuse[1].NbVid_7 = (SmuData >> 31) & 0x1;

  SmuIndex = 0xC0016004;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[2].NbVid_6_0 = SmuData & 0x7F;
  NbFuse[2].NbVid_7 = (SmuData >> 7) & 0x1;
  NbFuse[3].NbVid_6_0 = (SmuData >> 8) & 0x7F;
  NbFuse[3].NbVid_7 = (SmuData >> 15) & 0x1;

  SmuIndex = 0xC0016010;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);

  NbFuse[0].MemPstate = (SmuData >> 23) & 1;
  NbFuse[1].MemPstate = (SmuData >> 24) & 1;
  NbFuse[2].MemPstate = (SmuData >> 25) & 1;
  NbFuse[3].MemPstate = (SmuData >> 26) & 1;

  SmuIndex = 0xC0017008;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[0].NbFid_5_0 = (SmuData >> 26) & 0x3F;

  SmuIndex = 0xC001700C;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[1].NbFid_5_0 = SmuData & 0x3F;
  NbFuse[2].NbFid_5_0 = (SmuData >> 6) & 0x3F;
  NbFuse[3].NbFid_5_0 = (SmuData >> 12) & 0x3F;
  NbFuse[0].NbDid = (SmuData >> 18) & 0x1;
  NbFuse[1].NbDid = (SmuData >> 19) & 0x1;
  NbFuse[2].NbDid = (SmuData >> 20) & 0x1;
  NbFuse[3].NbDid = (SmuData >> 21) & 0x1;

  SmuIndex = 0xC0017018;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[0].NbIddDiv = (SmuData >> 18) & 0x3;
  NbFuse[1].NbIddDiv = (SmuData >> 20) & 0x3;
  NbFuse[2].NbIddDiv = (SmuData >> 22) & 0x3;
  NbFuse[3].NbIddDiv = (SmuData >> 24) & 0x3;
  NbFuse[0].NbIddValue = (SmuData >> 26) & 0x3F;

  SmuIndex = 0xC001701C;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[0].NbIddValue |= (SmuData & 0x3) << 6;
  NbFuse[1].NbIddValue = (SmuData >> 2) & 0xFF;
  NbFuse[2].NbIddValue = (SmuData >> 10) & 0xFF;
  NbFuse[3].NbIddValue = (SmuData >> 18) & 0xFF;


  SmuIndex = 0xC00160B8;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[4].MemPstate = (SmuData >> 10) & 1;
  NbFuse[5].MemPstate = (SmuData >> 11) & 1;
  NbFuse[6].MemPstate = (SmuData >> 12) & 1;
  NbFuse[7].MemPstate = (SmuData >> 13) & 1;

  NbFuse[4].NbDid = (SmuData >> 14) & 0x1;
  NbFuse[5].NbDid = (SmuData >> 15) & 0x1;
  NbFuse[6].NbDid = (SmuData >> 16) & 0x1;
  NbFuse[7].NbDid = (SmuData >> 17) & 0x1;

  NbFuse[4].NbFid_5_0 = (SmuData >> 18) & 0x3F;
  NbFuse[5].NbFid_5_0 = (SmuData >> 24) & 0x3F;
  NbFuse[6].NbFid_5_0 = (SmuData >> 30) & 0x3;

  SmuIndex = 0xC00160BC;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[6].NbFid_5_0 |= (SmuData & 0xF) << 2;
  NbFuse[7].NbFid_5_0 = (SmuData >> 4) & 0x3F;

  NbFuse[4].NbVid_6_0 = (SmuData >> 10) & 0x7F;
  NbFuse[4].NbVid_7 = (SmuData >> 17) & 0x1;
  NbFuse[5].NbVid_6_0 = (SmuData >> 18) & 0x7F;
  NbFuse[5].NbVid_7 = (SmuData >> 25) & 0x1;
  NbFuse[6].NbVid_6_0 = (SmuData >> 26) & 0x3F;

  SmuIndex = 0xC00160C0;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[6].NbVid_6_0 |= (SmuData & 0x1) << 6;
  NbFuse[6].NbVid_7 = (SmuData >> 1) & 0x1;
  NbFuse[7].NbVid_6_0 = (SmuData >> 2) & 0x7F;
  NbFuse[7].NbVid_7 = (SmuData >> 9) & 0x1;

  NbFuse[4].NbIddDiv = (SmuData >> 10) & 0x3;
  NbFuse[5].NbIddDiv = (SmuData >> 12) & 0x3;
  NbFuse[6].NbIddDiv = (SmuData >> 14) & 0x3;
  NbFuse[7].NbIddDiv = (SmuData >> 16) & 0x3;

  NbFuse[4].NbIddValue = (SmuData >> 18) & 0xFF;
  NbFuse[5].NbIddValue = (SmuData >> 26) & 0x3F;

  SmuIndex = 0xC00160C4;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  NbFuse[5].NbIddValue |= (SmuData & 0x3) << 6;
  NbFuse[6].NbIddValue = (SmuData >> 2) & 0xFF;
  NbFuse[7].NbIddValue = (SmuData >> 10) & 0xFF;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get Nb-Pstates selection table
 *
 * @param[in]  NbSelTbl        Nb-Pstate selcetion table.
 * @param[in]  PlatformConfig  Platform operational characteristics; power cap.
 * @param[in]  StdHeader       Config handle for library and services.
 *
 */
VOID
STATIC
F15CzGetNbPstateSelectionTable (
     OUT   NB_PSTATE_SELECTION_TABLE  *NbSelTbl,
  IN       PLATFORM_CONFIGURATION     *PlatformConfig,
  IN       AMD_CONFIG_PARAMS          *StdHeader
  )
{
  UINT8  i;
  UINT32 SmuIndex;
  UINT32 SmuData;
  UINT32 MaxTdp;
  UINT32 TDP;
  LOCATE_HEAP_PTR LocateHeapParams;
  NB_PSTATE_SELECTION_TABLE Npst[NPST_NUM];

  SmuIndex = 0xC0016114;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  Npst[0].MaxTdp = (UINT16) ((SmuData >> 11) & 0xFFFF);
  Npst[1].MaxTdp = (UINT16) ((SmuData >> 27) & 0x1F);

  SmuIndex = 0xC0016118;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  Npst[1].MaxTdp |= (SmuData & 0x7FF) << 5;
  Npst[2].MaxTdp = (UINT16) ((SmuData >> 11) & 0xFFFF);
  Npst[3].MaxTdp = (UINT16) ((SmuData >> 27) & 0x1F);

  SmuIndex = 0xC001611C;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  Npst[3].MaxTdp |= (SmuData & 0x7FF) << 5;
  Npst[0].NbP0Sel = (UINT8) ((SmuData >> 11) & 0x7);
  Npst[1].NbP0Sel = (UINT8) ((SmuData >> 14) & 0x7);
  Npst[2].NbP0Sel = (UINT8) ((SmuData >> 17) & 0x7);
  Npst[3].NbP0Sel = (UINT8) ((SmuData >> 20) & 0x7);
  Npst[0].NbP1Sel = (UINT8) ((SmuData >> 23) & 0x7);
  Npst[1].NbP1Sel = (UINT8) ((SmuData >> 26) & 0x7);
  Npst[2].NbP1Sel = (UINT8) ((SmuData >> 29) & 0x7);

  SmuIndex = 0xC0016120;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  Npst[3].NbP1Sel = (UINT8) (SmuData & 0x7);
  Npst[0].NbP2Sel = (UINT8) ((SmuData >> 3) & 0x7);
  Npst[1].NbP2Sel = (UINT8) ((SmuData >> 6) & 0x7);
  Npst[2].NbP2Sel = (UINT8) ((SmuData >> 9) & 0x7);
  Npst[3].NbP2Sel = (UINT8) ((SmuData >> 12) & 0x7);
  Npst[0].NbP3Sel = (UINT8) ((SmuData >> 15) & 0x7);
  Npst[1].NbP3Sel = (UINT8) ((SmuData >> 18) & 0x7);
  Npst[2].NbP3Sel = (UINT8) ((SmuData >> 21) & 0x7);
  Npst[3].NbP3Sel = (UINT8) ((SmuData >> 24) & 0x7);
  Npst[0].DdrMaxRate = (UINT8) ((SmuData >> 27) & 0x1F);

  SmuIndex = 0xC0016124;
  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndex, AccessWidth32, &SmuData, StdHeader);
  Npst[1].DdrMaxRate = (UINT8) (SmuData & 0x1F);
  Npst[2].DdrMaxRate = (UINT8) ((SmuData >> 5) & 0x1F);
  Npst[3].DdrMaxRate = (UINT8) ((SmuData >> 10) & 0x1F);

  LocateHeapParams.BufferHandle = AMD_GNB_TDP_HANDLE;
  if (HeapLocateBuffer (&LocateHeapParams, StdHeader) == AGESA_SUCCESS) {
    if (((SMU_TDP_INFO *) (LocateHeapParams.BufferPtr))->cTdp != 0) {
      TDP = ((SMU_TDP_INFO *) (LocateHeapParams.BufferPtr))->cTdp;
    } else {
      TDP = ((SMU_TDP_INFO *) (LocateHeapParams.BufferPtr))->SmuTdp;
    }
  } else {
    TDP = PlatformConfig->PowerCeiling;
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "    cTDP: %d\n", TDP);

  for (i = 0; i < NPST_NUM; i++) {
    // [15:8] - integer; [7:0] - decimals
    // convert watt to milliwatt by multiply it by 1000
    MaxTdp = Npst[i].MaxTdp * 1000 / 0x100;
    if (TDP <= MaxTdp) {
      break;
    }
  }

  if (i < NPST_NUM) {
    *NbSelTbl = Npst[i];
  } else {
    *NbSelTbl = Npst[NPST_NUM - 1];
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Disable NB P-state.
 *   - clear F5x1[6C:64]
 *   - clear F5x170[NbPstateMaxVal]
 *   - set   F5x170[SwNbPstateLoDis]
 *   - clear MSRC001_00[6B:64][NbPstate]
 *
 * @param[in]     FamilySpecificServices  The current Family Specific Services
 * @param[in]     CpuEarlyParamsPtr       Service Parameters
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
F15CzNbPstateDis (
  IN       CPU_SPECIFIC_SERVICES  *FamilySpecificServices,
  IN       AMD_CPU_EARLY_PARAMS   *CpuEarlyParamsPtr,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32       i;
  UINT32       PciData;
  UINT32       DataMask;
  PCI_ADDR     PciAddress;
  NB_PSTATE_CTRL_REGISTER NbPsCtrl;
  NB_PSTATE_CTRL_REGISTER NbPsCtrlOrg;
  NB_PSTATE_STS_REGISTER  NbPsSts;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzNbPstateDis\n");

  // Check whether NB P-state is disabled
  if (!FamilySpecificServices->IsNbPstateEnabled (FamilySpecificServices, &CpuEarlyParamsPtr->PlatformConfig, StdHeader)) {

    IDS_HDT_CONSOLE (CPU_TRACE, "  NB Pstates disabled\n");

    PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
    NbPsCtrlOrg = NbPsCtrl;

    PciAddress.Address.Register = NB_PSTATE_STATUS;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsSts, StdHeader);

    // Make sure NbPstateHi = NbPstateLo = 0
    if (NbPsCtrl.NbPstateHi != 0) {
      if (NbPsSts.CurNbPstateLo == 0) {
        // Switch to Nb Low if we're in Nb High
        TransitionToNbLow (StdHeader);
      }

      // NbPstateHi = 0
      PciAddress.Address.Register = NB_PSTATE_CTRL;
      LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
      NbPsCtrl.NbPstateHi = 0;
      LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
    }

    PciAddress.Address.Register = NB_PSTATE_STATUS;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsSts, StdHeader);
    if (NbPsSts.CurNbPstateLo == 1) {
      // Switch to Nb High if we're in Nb Low
      TransitionToNbHigh (StdHeader);
    }
    // NbPstateLo = 0
    // Clear F5x170[NbPstateMaxVal]
    PciAddress.Address.Register = NB_PSTATE_CTRL;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
    NbPsCtrl.NbPstateLo = 0;
    NbPsCtrl.SwNbPstateLoDis = 1;
    NbPsCtrl.NbPstateMaxVal = 0;
    LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);

    // Clear F5x1[6C:64]
    DataMask = 0xFFFFFFFF;
    PciData = 0;
    for (i = 1; i < NM_NB_PS_REG; i++) {
      PciAddress.Address.Register = NB_PSTATE_0 + (i * 4);
      LibAmdPciRMW (AccessWidth32, PciAddress, &PciData, &DataMask, StdHeader);
    }

    // Clear MSRC001_00[6B:64][NbPstate] on BSC.
    F15CzNbPstateDisCore (StdHeader);

    // BIOS performs the following to release the NB P-state force:
    // 1. Restore the initial D18F5x170[NbPstateDisOnP0, NbPstateThreshold] values.
    PciAddress.Address.Register = NB_PSTATE_CTRL;
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
    NbPsCtrl.NbPstateDisOnP0 = NbPsCtrlOrg.NbPstateDisOnP0;
    NbPsCtrl.NbPstateThreshold = NbPsCtrlOrg.NbPstateThreshold;
    LibAmdPciWrite (AccessWidth32, PciAddress, &PciData, StdHeader);
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Disable NB P-state on core.
 *   - clear MSRC001_00[6B:64][NbPstate].
 *
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F15CzNbPstateDisCore (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 i;
  UINT64 MsrData;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzNbPstateDisCore\n");

  for (i = MSR_PSTATE_0; i <= MSR_PSTATE_7; i++) {
    LibAmdMsrRead (i, &MsrData, StdHeader);
    ((PSTATE_MSR *) &MsrData)->NbPstate = 0;
    LibAmdMsrWrite (i, &MsrData, StdHeader);
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Transition to the low NB P-state.
 *
 *
 * @param[in]  StdHeader          Config handle for library and services.
 *
 */
VOID
TransitionToNbLow (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  PCI_ADDR                  PciAddress;
  NB_PSTATE_CTRL_REGISTER   NbPsCtrl;

  IDS_HDT_CONSOLE (CPU_TRACE, "  TransitionToNbLow\n");

  // Write 0 to D18F5x170[SwNbPstateLoDis, NbPstateDisOnP0, NbPstateThreshold].
  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
  NbPsCtrl.SwNbPstateLoDis = 0;
  NbPsCtrl.NbPstateDisOnP0 = 0;
  NbPsCtrl.NbPstateThreshold = 0;
  LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);

  // Wait for D18F5x174[CurNbPstateLo] = 1
  WaitForNbTransitionToComplete (NB_PS_LOW, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Transition to the high NB P-state.
 *
 *
 * @param[in]  StdHeader          Config handle for library and services.
 *
 */
VOID
TransitionToNbHigh (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  PCI_ADDR                  PciAddress;
  NB_PSTATE_CTRL_REGISTER   NbPsCtrl;

  IDS_HDT_CONSOLE (CPU_TRACE, "  TransitionToNbHigh\n");

  // Set D18F5x170[SwNbPstateLoDis] = 1.
  PciAddress.AddressValue = NB_PSTATE_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);
  NbPsCtrl.SwNbPstateLoDis = 1;
  LibAmdPciWrite (AccessWidth32, PciAddress, &NbPsCtrl, StdHeader);

  // Wait for D18F5x174[CurNbPstateLo] = 0
  WaitForNbTransitionToComplete (NB_PS_HIGH, StdHeader);
}

/*---------------------------------------------------------------------------------------*/
/**
 * Wait for CurNbPstateLo to match requirment
 *
 *
 * @param[in]  NbPsHiOrLow        Switch to Nb Pstate High or Nb Low
 * @param[in]  StdHeader          Config handle for library and services.
 *
 */
VOID
STATIC
WaitForNbTransitionToComplete (
  IN       UINT8              NbPsHiOrLow,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  PCI_ADDR                 PciAddress;
  NB_PSTATE_STS_REGISTER   NbPsSts;

  IDS_HDT_CONSOLE (CPU_TRACE, "  WaitForNbTransitionToComplete\n");

  PciAddress.AddressValue = NB_PSTATE_STATUS_PCI_ADDR;
  do {
    LibAmdPciRead (AccessWidth32, PciAddress, &NbPsSts, StdHeader);
  } while (((UINT8) NbPsSts.CurNbPstateLo != NbPsHiOrLow) || (NbPsSts.Reserved == 1));
}

/*---------------------------------------------------------------------------------------*/
/**
 * Get NB Frequency Numerator in MHz
 *
 * @param[in]     NbFid                   NB Frequency ID to convert
 * @param[out]    FreqNumeratorInMHz      The desire NB FID's frequency numerator in megahertz.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F15CzGetNbFreqNumeratorInMHz (
  IN       UINT32                NbFid,
     OUT   UINT32                *FreqNumeratorInMHz,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetNbFreqNumeratorInMHz - NbFid=%d\n", NbFid);
  // 100 * (D18F5x16[C:0][NbFid] + 4)
  *FreqNumeratorInMHz = (NbFid + 4) * 100;
  IDS_HDT_CONSOLE (CPU_TRACE, "    FreqNumeratorInMHz=%d\n", *FreqNumeratorInMHz);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get NB Frequency Divisor
 *
 * @param[in]     NbDid                   NB Divisor ID to convert.
 * @param[out]    FreqDivisor             The desire NB DID's frequency divisor.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F15CzGetNbFreqDivisor (
  IN       UINT32                NbDid,
     OUT   UINT32                *FreqDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetNbFreqDivisor - NbDid=%d\n", NbDid);
  *FreqDivisor = (1 << NbDid);
  IDS_HDT_CONSOLE (CPU_TRACE, "    FreqDivisor=%d\n", *FreqDivisor);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Calculate NB Frequency in MHz
 *
 * @param[in]     NbFid                   NB Frequency ID to convert
 * @param[in]     NbDid                   NB Divisor ID to convert.
 * @param[out]    FrequencyInMHz          The Northbridge clock frequency in megahertz.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 * @return        AGESA_SUCCESS           FrequencyInMHz is valid.
 */
AGESA_STATUS
STATIC
F15CzCalculateNbFrequencyInMHz (
  IN       UINT32                NbFid,
  IN       UINT32                NbDid,
     OUT   UINT32                *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32        FreqNumeratorInMHz;
  UINT32        FreqDivisor;
  AGESA_STATUS  ReturnStatus;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzCalculateNbFrequencyInMHz - NbFid=%x, NbDid=%x\n", NbFid, NbDid);

  ReturnStatus = AGESA_SUCCESS;
  F15CzGetNbFreqNumeratorInMHz (NbFid, &FreqNumeratorInMHz, StdHeader);
  F15CzGetNbFreqDivisor (NbDid, &FreqDivisor, StdHeader);
  *FrequencyInMHz = FreqNumeratorInMHz / FreqDivisor;
  IDS_HDT_CONSOLE (CPU_TRACE, "    FrequencyInMHz=%d\n", *FrequencyInMHz);

  return ReturnStatus;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Convert VID to microvolts(uV)
 *
 * @param[in]     Vid                     The voltage ID of SVI2 encoding to be converted.
 * @param[out]    VoltageInuV             The voltage in microvolts.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
VOID
STATIC
F15CzCovertVidInuV (
  IN       UINT32                Vid,
     OUT   UINT32                *VoltageInuV,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzCovertVidInuV\n");
  // Maximum 1.55V, 6.25mV per stpe
  *VoltageInuV = ConvertVidInuV(Vid);
  IDS_HDT_CONSOLE (CPU_TRACE, "    Vid=%x, VoltageInuV=%d\n", Vid, *VoltageInuV);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Get Core/NB Idd Divisor
 *
 * @param[in]     IddDiv                  Core/NB current divisor to convert.
 * @param[out]    IddDivisor              The desire Core/NB current divisor.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 */
VOID
STATIC
F15CzCmnGetIddDivisor (
  IN       UINT32                IddDiv,
     OUT   UINT32                *IddDivisor,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzCmnGetIddDivisor - IddDiv=%d\n", IddDiv);

  switch (IddDiv) {
  case 0:
    *IddDivisor = 1000;
    break;
  case 1:
    *IddDivisor = 100;
    break;
  case 2:
    *IddDivisor = 10;
    break;
  default:  // IddDiv = 3 is reserved. Use 10
    *IddDivisor = 10;
    ASSERT (FALSE);
    break;
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "    IddDivisor=%d\n", *IddDivisor);
}


/*---------------------------------------------------------------------------------------*/
/**
 * Calculate Core/NB current in mA
 *
 * @param[in]     IddValue                Core/NB current value.
 * @param[in]     IddDiv                  Core/NB current divisor.
 * @param[out]    CurrentInmA             The Core/NB current in milliampere.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 */
VOID
STATIC
F15CzCmnCalculateCurrentInmA (
  IN       UINT32                IddValue,
  IN       UINT32                IddDiv,
     OUT   UINT32                *CurrentInmA,
  IN       AMD_CONFIG_PARAMS     *StdHeader
  )
{
  UINT32        IddDivisor;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzCmnCalculateCurrentInmA - IddValue=%x, IddDiv=%x\n", IddValue, IddDiv);

  F15CzCmnGetIddDivisor (IddDiv, &IddDivisor, StdHeader);
  *CurrentInmA = IddValue * IddDivisor;

  IDS_HDT_CONSOLE (CPU_TRACE, "    CurrentInmA=%d\n", *CurrentInmA);
}

/*---------------------------------------------------------------------------------------*/
/**
 *
 * Is it SecureS3
 *
 * @param[in]    StdHeader        Header for library and services
 *
 * @retval       TRUE             It's SecureS3
 * @retval       FALSE            It's NOT SecureS3
 *
 */
BOOLEAN
F15CzIsSecureS3 (
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32       SystemCoreNumber;
  UINT64       BaseAddress;
  CPU_SPECIFIC_SERVICES  *IgnoreService;

  // The best way to check IsSecureS3 is locating a PPI which is published by PSP driver
  // But APs don't have PEI service, and can't locate PPI
  // During SecureS3 resume path, PSP driver will relocate heap (including BSP and APs)
  // Below is an alternate solution: checking BSP's heap address
  BaseAddress = HeapGetBaseAddress (StdHeader);
  IgnoreService = NULL;
  SystemCoreNumber = F15CzGetApCoreNumber (IgnoreService, StdHeader);
  BaseAddress = BaseAddress - (SystemCoreNumber * AMD_HEAP_SIZE_PER_CORE);
  if ((BaseAddress != AMD_HEAP_START_ADDRESS) && ((StdHeader->HeapStatus == HEAP_DO_NOT_EXIST_YET) || (StdHeader->HeapStatus == HEAP_LOCAL_CACHE))) {
    return TRUE;
  }

  return FALSE;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Returns the register tables list
 *
 * @param[in]     FamilyServices          The current Family Specific Services
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 */
REGISTER_TABLE_AT_GIVEN_TP *
F15CzGetRegisterTableList (
  IN       CPU_SPECIFIC_SERVICES                *FamilyServices,
  IN       AMD_CONFIG_PARAMS                    *StdHeader
  )
{
  return (REGISTER_TABLE_AT_GIVEN_TP *) &F15CzRegTableListAtGivenTP;
}
