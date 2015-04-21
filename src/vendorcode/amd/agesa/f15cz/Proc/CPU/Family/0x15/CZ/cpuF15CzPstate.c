/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD Family 15h Carrizo Pstate feature support functions.
 *
 * Provides the functions necessary to initialize the Pstate feature.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/CZ
 * @e \$Revision: 309090 $   @e \$Date: 2014-12-10 02:28:05 +0800 (Wed, 10 Dec 2014) $
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
#include "GeneralServices.h"
#include "cpuPstateTables.h"
#include "cpuRegisters.h"
#include "Table.h"
#include "cpuFamilyTranslation.h"
#include "cpuFamRegisters.h"
#include "cpuF15Utilities.h"
#include "cpuF15CzUtilities.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15CzPowerMgmt.h"
#include "CommonReturns.h"
#include "Gnb.h"
#include "GnbLib.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_CZ_CPUF15CZPSTATE_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
AGESA_STATUS
F15CzGetPstateTransLatency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       PSTATE_INFO            *PStateInfoBufferStructPtr,
  IN       PCI_ADDR               *PciAddress,
     OUT   UINT32                 *TransitionLatency,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  );

AGESA_STATUS
F15CzGetPstateFrequency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
F15CzGetPstatePower (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *PowerInMw,
  IN       AMD_CONFIG_PARAMS *StdHeader
  );

AGESA_STATUS
F15CzGetPstateMaxState (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
     OUT   UINT32              *MaxPStateNumber,
     OUT   UINT8               *NumberOfBoostStates,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );

AGESA_STATUS
F15CzGetPstateRegisterInfo (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT32              PState,
     OUT   BOOLEAN             *PStateEnabled,
  IN OUT   UINT32              *IddVal,
  IN OUT   UINT32              *IddDiv,
     OUT   UINT32              *SwPstateNumber,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  );



/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to check if Pstate PSD is dependent.
 *
 * @param[in]     PstateCpuServices  Pstate CPU services.
 * @param[in,out] PlatformConfig     Contains the runtime modifiable feature input data.
 * @param[in]     StdHeader          Config Handle for library, services.
 *
 * @retval       TRUE               PSD is dependent.
 * @retval       FALSE              PSD is independent.
 *
 */
BOOLEAN
STATIC
F15CzIsPstatePsdDependent (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN OUT   PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  BOOLEAN                PsdIsDependent;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzIsPstatePsdDependent\n");

  // Family 15h Carrizo defaults to dependent PSD; allow Platform Configuration to
  // overwrite the default setting.
  PsdIsDependent = TRUE;
  if (PlatformConfig->ForcePstateIndependent == TRUE) {
    PsdIsDependent = FALSE;
  }

  IDS_HDT_CONSOLE (CPU_TRACE, "    P-state PSD is dependent: %d\n", PsdIsDependent);
  return PsdIsDependent;
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get Pstate Transition Latency.
 *
 *  Calculate TransitionLatency by Gaters On/Off Time value and pll value.
 *
 *  @param[in]     PstateCpuServices                 Pstate CPU services.
 *  @param[in]     PStateInfoBufferStructPtr         Pstate row data buffer pointer
 *  @param[in]     PciAddress                        Pci address
 *  @param[out]    TransitionLatency                 The transition latency.
 *  @param[in]     StdHeader                         Header for library and services
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15CzGetPstateTransLatency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       PSTATE_INFO            *PStateInfoBufferStructPtr,
  IN       PCI_ADDR               *PciAddress,
     OUT   UINT32                 *TransitionLatency,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{

  UINT32               CpuFid;
  UINT32               HwP0CpuFid;
  UINT32               GatersOffTime;
  UINT32               GatersOnTime;
  UINT32               PllLockTimer;
  UINT32               LocalPciRegister;
  UINT32               k;
  UINT32               PStateMaxValueOnCurrentCore;
  UINT32               TransAndBusMastLatency;
  UINT64               MsrData;
  BOOLEAN              CpuFidSameFlag;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetPstateTransLatency\n");

  CpuFidSameFlag = TRUE;
  HwP0CpuFid = 0;

  // Get PStateMaxValue
  PciAddress->Address.Register = CPTC2_REG;
  PciAddress->Address.Function = FUNC_3;
  LibAmdPciRead (AccessWidth32, *PciAddress, &LocalPciRegister, StdHeader);
  PStateMaxValueOnCurrentCore = ((F15_CZ_CLK_PWR_TIMING_CTRL2_REGISTER *) &LocalPciRegister)->HwPstateMaxVal;

  //
  //Check if MSRC001_00[6B:64][CpuFid] is the same value for all P-states
  //
  for (k = 0; k <= PStateMaxValueOnCurrentCore; k++) {
    LibAmdMsrRead ((MSR_PSTATE_0 + k), &MsrData, StdHeader);
    if (((PSTATE_MSR *) &MsrData)->PsEnable == 1) {
      CpuFid = (UINT32) GetF15CzCpuFid (&MsrData);
      if (k == 0) {
        HwP0CpuFid = CpuFid;
      }
      if (HwP0CpuFid != CpuFid) {
        CpuFidSameFlag = FALSE;
        break;
      }
    }
  }

  GnbLibPciIndirectRead (MAKE_SBDFO (0, 0, 0, 0, 0xB8), 0xD8222114, AccessWidth32, &LocalPciRegister, StdHeader);

  // Refclock is 100MHz, so multiply by 10 is ns
  GatersOnTime = (LocalPciRegister & 0xFF) * 10;
  GatersOffTime = ((LocalPciRegister >> 8) & 0xFF) * 10;

  //
  //(15 * GatersOffTime / 1000) + (15 * GatersOnTime / 1000) us
  //
  TransAndBusMastLatency =
  ((15 * GatersOffTime + 999) / 1000) +
  ((15 * GatersOnTime + 999) / 1000);

  if (!CpuFidSameFlag) {
    //
    // +PllLockTimer
    //
    GnbLibPciIndirectRead (MAKE_SBDFO (0, 0, 0, 0, 0xB8), 0xD82220B8, AccessWidth32, &LocalPciRegister, StdHeader);
    PllLockTimer = ((LocalPciRegister & 0x1FFF) + 99) / 100;
    TransAndBusMastLatency += PllLockTimer;
  }

  *TransitionLatency = TransAndBusMastLatency;

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to calculates the frequency in megahertz of the desired P-state.
 *
 *  @param[in]   PstateCpuServices        Pstate CPU services.
 *  @param[in]   StateNumber              The P-State to analyze.
 *  @param[out]  FrequencyInMHz           The P-State's frequency in MegaHertz
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      AGESA_SUCCESS   Always Succeeds.
 */
AGESA_STATUS
F15CzGetPstateFrequency (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *FrequencyInMHz,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32 TempValue;
  UINT32 CpuDid;
  UINT32 CpuFid;
  UINT64 LocalMsrRegister;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetPstateFrequency - P%d\n", StateNumber);

  ASSERT (StateNumber < NM_PS_REG);
  LibAmdMsrRead (PS_REG_BASE + (UINT32) StateNumber, &LocalMsrRegister, StdHeader);
  ASSERT (((PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1);
  CpuDid = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->CpuDid);
  CpuFid = (UINT32) GetF15CzCpuFid (&LocalMsrRegister);

  switch (CpuDid) {
  case 0:
    TempValue = 1;
    break;
  case 1:
    TempValue = 2;
    break;
  case 2:
    TempValue = 4;
    break;
  case 3:
    TempValue = 8;
    break;
  case 4:
    TempValue  =  16;
    break;
  default:
    // CpuDid is set to an undefined value.  This is due to either a misfused CPU, or
    // an invalid P-state MSR write.
    IDS_HDT_CONSOLE (CPU_TRACE, "    Undefined CpuDid %d used. Force it to zero.\n", CpuDid);
    ASSERT (FALSE);
    TempValue = 1;
    break;
  }
  // 100 * (MSRC001_00[6B:64][CpuFid] + 0x10) / (2^MSRC001_00[6B:64][CpuDid]).
  *FrequencyInMHz = (100 * (CpuFid + 0x10) / TempValue);
  IDS_HDT_CONSOLE (CPU_TRACE, "    FrequencyInMHz=%d, CpuFid=%d, CpuDid=%d\n", *FrequencyInMHz, CpuFid, CpuDid);

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to calculates the power in milliWatts of the desired P-state.
 *
 *  @param[in]     PstateCpuServices      Pstate CPU services.
 *  @param[in]   StateNumber              Which P-state to analyze
 *  @param[out]  PowerInMw                The Power in milliWatts of that P-State
 *  @param[in]   StdHeader                Header for library and services
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15CzGetPstatePower (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT8 StateNumber,
     OUT   UINT32 *PowerInMw,
  IN       AMD_CONFIG_PARAMS *StdHeader
  )
{
  UINT32  CpuVid;
  UINT32  IddValue;
  UINT32  IddDiv;
  UINT32  V_x100000;
  UINT32  Power;
  UINT64  LocalMsrRegister;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetPstatePower - P%d\n", StateNumber);

  ASSERT (StateNumber < NM_PS_REG);
  LibAmdMsrRead (PS_REG_BASE + (UINT32) StateNumber, &LocalMsrRegister, StdHeader);
  ASSERT (((PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1);
  CpuVid = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->CpuVid);
  IddValue = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->IddValue);
  IddDiv = (UINT32) (((PSTATE_MSR *) &LocalMsrRegister)->IddDiv);

  if (CpuVid >= 0xF8) {
    V_x100000 = 0;
  } else {
    V_x100000 = 155000L - (625L * CpuVid);
  }

  Power = V_x100000 * IddValue;

  switch (IddDiv) {
  case 0:
    *PowerInMw = Power / 100L;
    break;
  case 1:
    *PowerInMw = Power / 1000L;
    break;
  case 2:
    *PowerInMw = Power / 10000L;
    break;
  default:
    // IddDiv is set to an undefined value.  This is due to either a misfused CPU, or
    // an invalid P-state MSR write.
    ASSERT (FALSE);
    *PowerInMw = 0;
    break;
  }
  IDS_HDT_CONSOLE (CPU_TRACE, "  PowerInMw=%d, CpuVid=%d, IddValue=%d, IddDiv=%d\n", *PowerInMw, CpuVid, IddValue, IddDiv);

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get CPU pstate max state.
 *
 *  @param[in]     PstateCpuServices      Pstate CPU services.
 *  @param[out]    MaxPStateNumber        The max hw pstate value on the current socket.
 *  @param[out]    NumberOfBoostStates    The number of boosted P-states on the current socket.
 *  @param[in]     StdHeader              Handle of Header for calling lib functions and services.
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15CzGetPstateMaxState (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
     OUT   UINT32              *MaxPStateNumber,
     OUT   UINT8               *NumberOfBoostStates,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32               NumBoostStates;
  UINT64               MsrValue;
  UINT32               LocalPciRegister;
  PCI_ADDR             PciAddress;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetPstateMaxState\n");

  LocalPciRegister = 0;

  // For F15 Carrizo CPU, skip boosted p-state. The boosted p-state number = F4x15C[NumBoostStates].
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader); // F4x15C

  NumBoostStates = ((CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates;
  *NumberOfBoostStates = (UINT8) NumBoostStates;

  //
  // Read PstateMaxVal [6:4] from MSR C001_0061
  // So, we will know the max pstate state in this socket.
  //
  LibAmdMsrRead (MSR_PSTATE_CURRENT_LIMIT, &MsrValue, StdHeader);
  *MaxPStateNumber = (UINT32) (((PSTATE_CURLIM_MSR *) &MsrValue)->PstateMaxVal) + NumBoostStates;
  IDS_HDT_CONSOLE (CPU_TRACE, "    MaxPStateNumber=%d, NumBoostStates=%d\n", *MaxPStateNumber, NumBoostStates);

  return (AGESA_SUCCESS);
}

/*---------------------------------------------------------------------------------------*/
/**
 *  Family specific call to get CPU pstate register information.
 *
 *  @param[in]     PstateCpuServices         Pstate CPU services.
 *  @param[in]     PState                    Input Pstate number for query.
 *  @param[out]    PStateEnabled             Boolean flag return pstate enable.
 *  @param[in,out] IddVal                    Pstate current value.
 *  @param[in,out] IddDiv                    Pstate current divisor.
 *  @param[out]    SwPstateNumber            Software P-state number.
 *  @param[in]     StdHeader                 Handle of Header for calling lib functions and services.
 *
 *  @retval      AGESA_SUCCESS Always succeeds.
 */
AGESA_STATUS
F15CzGetPstateRegisterInfo (
  IN       PSTATE_CPU_FAMILY_SERVICES *PstateCpuServices,
  IN       UINT32              PState,
     OUT   BOOLEAN             *PStateEnabled,
  IN OUT   UINT32              *IddVal,
  IN OUT   UINT32              *IddDiv,
     OUT   UINT32              *SwPstateNumber,
  IN       AMD_CONFIG_PARAMS   *StdHeader
  )
{
  UINT32    LocalPciRegister;
  UINT64    LocalMsrRegister;
  PCI_ADDR  PciAddress;

  IDS_HDT_CONSOLE (CPU_TRACE, "  F15CzGetPstateRegisterInfo - P%d\n", PState);

  ASSERT (PState < NM_PS_REG);

  // For F15 Carrizo CPU, skip boosted p-state. The boosted p-state number = F4x15C[NumBoostStates].
  PciAddress.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, PciAddress, &LocalPciRegister, StdHeader); // F4x15C

  // Read PSTATE MSRs
  LibAmdMsrRead (PS_REG_BASE + (UINT32) PState, &LocalMsrRegister, StdHeader);

  *SwPstateNumber = PState;

  if (((PSTATE_MSR *) &LocalMsrRegister)->PsEnable == 1) {
    // PState enable = bit 63
    *PStateEnabled = TRUE;
    //
    // Check input pstate belongs to Boosted-Pstate, if yes, return *PStateEnabled = FALSE.
    //
    if (PState < ((CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates) {
      *PStateEnabled = FALSE;
    } else {
      *SwPstateNumber = PState - ((CPB_CTRL_REGISTER *) &LocalPciRegister)->NumBoostStates;
    }
    IDS_HDT_CONSOLE (CPU_TRACE, "  Pstate %d is enabled. SwPstateNumber=%d\n", PState, *SwPstateNumber);
  } else {
    *PStateEnabled = FALSE;
  }

  // Bits 39:32 (high 32 bits [7:0])
  *IddVal = (UINT32) ((PSTATE_MSR *) &LocalMsrRegister)->IddValue;
  // Bits 41:40 (high 32 bits [9:8])
  *IddDiv = (UINT32) ((PSTATE_MSR *) &LocalMsrRegister)->IddDiv;

  IDS_HDT_CONSOLE (CPU_TRACE, "    IddVal=%d, IddDiv=%d\n", *IddVal, *IddDiv);
  return (AGESA_SUCCESS);
}


CONST PSTATE_CPU_FAMILY_SERVICES ROMDATA F15CzPstateServices =
{
  0,
  (PF_PSTATE_PSD_IS_NEEDED) CommonReturnTrue,
  F15CzIsPstatePsdDependent,
  F15CzGetPstateTransLatency,
  F15CzGetPstateFrequency,
  F15CzGetPstatePower,
  F15CzGetPstateMaxState,
  F15CzGetPstateRegisterInfo
};


/*---------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */

