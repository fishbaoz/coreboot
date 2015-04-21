/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD DMI Record Creation API, and related functions for Family15h Stoney.
 *
 * Contains code that produce the DMI related information.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU/Family/0x15/ST
 * @e \$Revision: 312756 $   @e \$Date: 2015-02-11 11:00:50 +0800 (Wed, 11 Feb 2015) $
 *
 */
/*****************************************************************************
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
#include "cpuPstateTables.h"
#include "cpuLateInit.h"
#include "cpuF15Dmi.h"
#include "cpuF15PowerMgmt.h"
#include "cpuF15StPowerMgmt.h"
#include "cpuServices.h"
#include "Filecode.h"
CODE_GROUP (G3_DXE)
RDATA_GROUP (G3_DXE)

#define FILECODE PROC_CPU_FAMILY_0X15_ST_CPUF15STDMI_FILECODE


/*----------------------------------------------------------------------------------------
 *                   D E F I N I T I O N S    A N D    M A C R O S
 *----------------------------------------------------------------------------------------
 */
extern CPU_FAMILY_SUPPORT_TABLE            PstateFamilyServiceTable;

/*----------------------------------------------------------------------------------------
 *                  T Y P E D E F S     A N D     S T R U C T U R E S
 *----------------------------------------------------------------------------------------
 */
CONST CHAR8 ROMDATA str_A10_ST[] = "AMD A10-";
CONST CHAR8 ROMDATA str_A8_ST[]  = "AMD A8-";
CONST CHAR8 ROMDATA str_A6_ST[]  = "AMD A6-";
CONST CHAR8 ROMDATA str_FX_ST[]  = "AMD FX-";
CONST CHAR8 ROMDATA str_PRO_A10_ST[] = "AMD PRO A10-";
CONST CHAR8 ROMDATA str_PRO_A8_ST[]  = "AMD PRO A8-";
CONST CHAR8 ROMDATA str_PRO_A6_ST[]  = "AMD PRO A6-";
CONST CHAR8 ROMDATA str_PRO_FX_ST[]  = "AMD PRO FX-";
/*---------------------------------------------------------------------------------------
 * Processor Family Table

 *-------------------------------------------------------------------------------------*/

CONST CPU_T4_PROC_FAMILY ROMDATA F15StFP4T4ProcFamily[] =
{
  {str_FX_ST,  0x3F},
  {str_A10_ST, 0x48},
  {str_A8_ST,  0x48},
  {str_A6_ST,  0x48},
  {str_PRO_FX_ST, 0x3F},
  {str_PRO_A10_ST, 0x48},
  {str_PRO_A8_ST,  0x48},
  {str_PRO_A6_ST,  0x48},
};

CONST CPU_T4_PROC_FAMILY ROMDATA F15StFT4T4ProcFamily[] =
{
  {str_FX_ST,  0x3F},
  {str_A10_ST, 0x48},
  {str_A8_ST,  0x48},
  {str_A6_ST,  0x48},
  {str_PRO_FX_ST, 0x3F},
  {str_PRO_A10_ST, 0x48},
  {str_PRO_A8_ST,  0x48},
  {str_PRO_A6_ST,  0x48},
};

/*----------------------------------------------------------------------------------------
 *           P R O T O T Y P E S     O F     L O C A L     F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
VOID
DmiF15StGetInfo (
  IN OUT   CPU_TYPE_INFO *CpuInfoPtr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

VOID
DmiF15StGetT4ProcFamily (
  IN OUT   UINT8 *T4ProcFamily,
  IN       PROC_FAMILY_TABLE *CpuDmiProcFamilyTable,
  IN       CPU_TYPE_INFO *CpuInfo,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

UINT8
DmiF15StGetVoltage (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

VOID
DmiF15StGetMemInfo (
  IN OUT   CPU_GET_MEM_INFO  *CpuGetMemInfoPtr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

UINT16
DmiF15StGetExtClock (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  );

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------*/
/**
 *
 *  DmiF15StGetInfo
 *
 *    Get CPU type information
 *
 *    @param[in,out]  CpuInfoPtr     Pointer to CPU_TYPE_INFO struct.
 *    @param[in]      StdHeader      Standard Head Pointer
 *
 */
VOID
DmiF15StGetInfo (
  IN OUT   CPU_TYPE_INFO *CpuInfoPtr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT8 NumOfCoresPerCU;
  CPUID_DATA CpuId;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;

  LibAmdCpuidRead (AMD_CPUID_FMF, &CpuId, StdHeader);
  CpuInfoPtr->ExtendedFamily = (UINT8) (CpuId.EAX_Reg >> 20) & 0xFF; // bit 27:20
  CpuInfoPtr->ExtendedModel = (UINT8) (CpuId.EAX_Reg >> 16) & 0xF; // bit 19:16
  CpuInfoPtr->BaseFamily = (UINT8) (CpuId.EAX_Reg >> 8) & 0xF; // bit 11:8
  CpuInfoPtr->BaseModel = (UINT8) (CpuId.EAX_Reg >> 4)  & 0xF; // bit 7:4
  CpuInfoPtr->Stepping = (UINT8) (CpuId.EAX_Reg & 0xF); // bit 3:0

  CpuInfoPtr->PackageType = (UINT8) (CpuId.EBX_Reg >> 28) & 0xF; // bit 31:28

  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  CpuInfoPtr->TotalCoreNumber = FamilySpecificServices->GetNumberOfPhysicalCores (FamilySpecificServices, StdHeader);
  CpuInfoPtr->TotalCoreNumber--;

  LibAmdCpuidRead (AMD_CPUID_ASIZE_PCCOUNT, &CpuId, StdHeader);
  CpuInfoPtr->EnabledCoreNumber = (UINT8) (CpuId.ECX_Reg & 0xFF); // bit 7:0

  switch (CpuInfoPtr->PackageType) {
  case ST_SOCKET_FP4:
    CpuInfoPtr->ProcUpgrade = P_UPGRADE_NONE;
    break;
  case ST_SOCKET_FT4:
    CpuInfoPtr->ProcUpgrade = P_UPGRADE_NONE;
    break;
  default:
    CpuInfoPtr->ProcUpgrade = P_UPGRADE_UNKNOWN;
    break;
  }

  switch (GetComputeUnitMapping (StdHeader)) {
  case AllCoresMapping:
    NumOfCoresPerCU = 1;
    break;
  case EvenCoresMapping:
    NumOfCoresPerCU = 2;
    break;
  default:
    NumOfCoresPerCU = 2;
  }
  // L1 Size & Associativity
  LibAmdCpuidRead (AMD_CPUID_TLB_L1Cache, &CpuId, StdHeader);
  CpuInfoPtr->CacheInfo.L1CacheSize = (UINT32) (((UINT8) ((CpuId.ECX_Reg >> 24) * NumOfCoresPerCU) + (UINT8) (CpuId.EDX_Reg >> 24)) * (CpuInfoPtr->EnabledCoreNumber + 1) / NumOfCoresPerCU);

  CpuInfoPtr->CacheInfo.L1CacheAssoc = DMI_ASSOCIATIVE_2_WAY; // Per the BKDG, this is hard-coded to 2-Way.

  // L2 Size & Associativity
  LibAmdCpuidRead (AMD_CPUID_L2L3Cache_L2TLB, &CpuId, StdHeader);
  CpuInfoPtr->CacheInfo.L2CacheSize = (UINT32) ((UINT16) (CpuId.ECX_Reg >> 16) * (CpuInfoPtr->EnabledCoreNumber + 1) / NumOfCoresPerCU);

  CpuInfoPtr->CacheInfo.L2CacheAssoc = DMI_ASSOCIATIVE_16_WAY; // Per the BKDG, this is hard-coded to 16-Way.

  // L3 Size & Associativity
  CpuInfoPtr->CacheInfo.L3CacheSize = 0;
  CpuInfoPtr->CacheInfo.L3CacheAssoc = DMI_ASSOCIATIVE_UNKNOWN;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  DmiF15StGetT4ProcFamily
 *
 *    Get type 4 processor family information
 *
 *    @param[in,out]  T4ProcFamily   Pointer to type 4 processor family information.
 *    @param[in]      *CpuDmiProcFamilyTable  Pointer to DMI family special service
 *    @param[in]      *CpuInfo       Pointer to CPU_TYPE_INFO struct
 *    @param[in]      StdHeader      Standard Head Pointer
 *
 */
VOID
DmiF15StGetT4ProcFamily (
  IN OUT   UINT8 *T4ProcFamily,
  IN       PROC_FAMILY_TABLE *CpuDmiProcFamilyTable,
  IN       CPU_TYPE_INFO *CpuInfo,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  CHAR8 NameString[49];
  CONST CHAR8 *DmiString;
  CONST VOID  *DmiStringTable;
  UINT8 NumberOfDmiString;
  UINT8 i;

  // Get name string from MSR_C001_00[30:35]
  GetNameString (NameString, StdHeader);
  // Get DMI String
  DmiStringTable = NULL;
  switch (CpuInfo->PackageType) {
  case ST_SOCKET_FP4:
    DmiStringTable = (CONST VOID *) &F15StFP4T4ProcFamily[0];
    NumberOfDmiString = sizeof (F15StFP4T4ProcFamily) / sizeof (CPU_T4_PROC_FAMILY);
    break;
  case ST_SOCKET_FT4:
    DmiStringTable = (CONST VOID *) &F15StFT4T4ProcFamily[0];
    NumberOfDmiString = sizeof (F15StFT4T4ProcFamily) / sizeof (CPU_T4_PROC_FAMILY);
    break;
  default:
    DmiStringTable = NULL;
    NumberOfDmiString = 0;
    break;
  }

  // Find out which DMI string matches current processor's name string
  *T4ProcFamily = P_FAMILY_UNKNOWN;
  if ((DmiStringTable != NULL) && (NumberOfDmiString != 0)) {
    for (i = 0; i < NumberOfDmiString; i++) {
      DmiString = (((CPU_T4_PROC_FAMILY *) DmiStringTable)[i]).Stringstart;
      if (IsSourceStrContainTargetStr (NameString, DmiString, StdHeader)) {
        *T4ProcFamily = (((CPU_T4_PROC_FAMILY *) DmiStringTable)[i]).T4ProcFamilySetting;
        break;
      }
    }
  }
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  DmiF15StGetVoltage
 *
 *    Get the voltage value according to SMBIOS SPEC's requirement.
 *
 *    @param[in]       StdHeader      Standard Head Pointer
 *
 *    @retval  Voltage   - CPU Voltage.
 *
 */
UINT8
DmiF15StGetVoltage (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  UINT8     MaxVid;
  UINT8     Voltage;
  UINT8     NumberBoostStates;
  UINT64    MsrData;
  PCI_ADDR  TempAddr;
  CPB_CTRL_REGISTER CpbCtrl;

  // Voltage = 0x80 + (voltage at boot time * 10)
  TempAddr.AddressValue = CPB_CTRL_PCI_ADDR;
  LibAmdPciRead (AccessWidth32, TempAddr, &CpbCtrl, StdHeader);  // F4x15C
  NumberBoostStates = (UINT8) CpbCtrl.NumBoostStates;

  LibAmdMsrRead ((MSR_PSTATE_0 + NumberBoostStates), &MsrData, StdHeader);
  MaxVid = (UINT8) (((PSTATE_MSR *)&MsrData)->CpuVid);

  if ((MaxVid >= 0xF8) && (MaxVid <= 0xFF)) {
    Voltage = 0;
  } else {
    Voltage = (UINT8) ((155000L - (625 * MaxVid) + 5000) / 10000);
  }

  Voltage += 0x80;
  return (Voltage);
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  DmiF15StGetMemInfo
 *
 *    Get memory information.
 *
 *    @param[in,out]  CpuGetMemInfoPtr      Pointer to CPU_GET_MEM_INFO struct.
 *    @param[in]      StdHeader             Standard Head Pointer
 *
 */
VOID
DmiF15StGetMemInfo (
  IN OUT   CPU_GET_MEM_INFO  *CpuGetMemInfoPtr,
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  CpuGetMemInfoPtr->EccCapable = FALSE;
}

/* -----------------------------------------------------------------------------*/
/**
 *
 *  DmiF15StGetExtClock
 *
 *    Get the external clock Speed
 *
 *    @param[in]      StdHeader      Standard Head Pointer
 *
 *    @retval  ExtClock   - CPU external clock Speed.
 *
 */
UINT16
DmiF15StGetExtClock (
  IN       AMD_CONFIG_PARAMS  *StdHeader
  )
{
  return (EXTERNAL_CLOCK_100MHZ);
}

CONST PROC_FAMILY_TABLE ROMDATA ProcFamily15StDmiTable =
{
// This table is for Processor family 15h Stoney
  AMD_FAMILY_15_ST,               // ID for Family 15h Stoney
  DmiF15StGetInfo,                // Transfer vectors for family
  DmiF15StGetT4ProcFamily,        // Get type 4 processor family information
  DmiF15StGetVoltage,             //   specific routines (above)
  DmiF15GetMaxSpeed,
  DmiF15StGetExtClock,
  DmiF15StGetMemInfo,             // Get memory information
  0,
  NULL
};


/*---------------------------------------------------------------------------------------
 *                          L O C A L    F U N C T I O N S
 *---------------------------------------------------------------------------------------
 */
