/* $NoKeywords:$ */
/**
 * @file
 *
 * AMD CPU Register Table Related Functions
 *
 * Set registers according to a set of register tables
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  CPU
 * @e \$Revision: 313537 $   @e \$Date: 2015-02-24 10:57:13 -0600 (Tue, 24 Feb 2015) $
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
 *                            M O D U L E S    U S E D
 *----------------------------------------------------------------------------------------
 */
#include "AGESA.h"
#include "amdlib.h"
#include "Ids.h"
#include "Topology.h"
#include "cpuRegisters.h"
#include "cpuFamilyTranslation.h"
#include "Table.h"
#include "GeneralServices.h"
#include "cpuServices.h"
#include "Gnb.h"
#include "GnbLib.h"
#include "cpuFeatures.h"
#include "CommonReturns.h"
#include "Filecode.h"
CODE_GROUP (G1_PEICC)
RDATA_GROUP (G1_PEICC)

#define FILECODE PROC_CPU_TABLE_FILECODE

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

/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */
extern BUILD_OPT_CFG UserOptions;

/*---------------------------------------------------------------------------------------*/
/**
 * An iterator for all the Family and Model Register Tables.
 *
 * RegisterTableHandle should be set to NULL to begin iteration, the first time the method is
 * invoked.  Register tables can be processed, until this method returns NULL.  RegisterTableHandle
 * should simply be passed back to the method without modification or use by the caller.
 * The table selector allows the relevant tables for different cores to be iterated, if the family separates
 * tables.  For example, MSRs can be in a table processed by all cores and PCI registers in a table processed by
 * primary cores.
 *
 * @param[in]     Selector                Select whether to iterate over tables for either all cores, primary cores, bsp, ....
 * @param[in]     RegisterTableList       Register table list
 * @param[in,out] RegisterTableHandle     IN: The handle of the current register table, or NULL if Begin.
 *                                        OUT: The handle of the next register table, if not End.
 * @param[in]     StdHeader               Handle of Header for calling lib functions and services.
 *
 * @return        The pointer to the next Register Table, or NULL if End.
 */
REGISTER_TABLE
**GetNextRegisterTable (
  IN       UINT32                  Selector,
  IN       REGISTER_TABLE        **RegisterTableList,
  IN OUT   REGISTER_TABLE       ***RegisterTableHandle,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  REGISTER_TABLE **NextTable;

  ASSERT (StdHeader != NULL);
  ASSERT (Selector < TableCoreSelectorMax);

  NextTable = *RegisterTableHandle;
  if (NextTable == NULL) {
    // Begin
    NextTable = RegisterTableList;
    IDS_OPTION_HOOK (IDS_TRAP_TABLE, &NextTable, StdHeader);
  } else {
    NextTable++;
  }
  // skip if not selected
  while ((*NextTable != NULL) && (((*NextTable)->Selector != Selector))) {
    NextTable++;
  }
  if (*NextTable == NULL) {
    // End
    *RegisterTableHandle = NULL;
  } else {
    // Iterate next table
    *RegisterTableHandle = NextTable;
  }
  return NextTable;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Compare counts to a pair of ranges.
 *
 * @param[in]       FirstCount       The actual count to be compared to the first range.
 * @param[in]       SecondCount      The actual count to be compared to the second range.
 * @param[in]       Ranges           The ranges which the counts are compared to.
 *
 * @retval          TRUE             Either one, or both, of the counts is in the range given.
 * @retval          FALSE            Neither count is in the range given.
 */
BOOLEAN
IsEitherCountInRange (
  IN       UINTN                FirstCount,
  IN       UINTN                SecondCount,
  IN       COUNT_RANGE_FEATURE  Ranges
  )
{
  // Errors: Entire Range value is zero, Min and Max reversed or not <=, ranges overlap (OK if first range is all),
  // the real counts are too big.
  ASSERT ((Ranges.Range0Min <= Ranges.Range0Max) &&
          (Ranges.Range1Min <= Ranges.Range1Max) &&
          (Ranges.Range0Max != 0) &&
          (Ranges.Range1Max != 0) &&
          ((Ranges.Range0Max == COUNT_RANGE_HIGH) || (Ranges.Range0Max < Ranges.Range1Min)) &&
          ((FirstCount < COUNT_RANGE_HIGH) && (SecondCount < COUNT_RANGE_HIGH)));

  return (BOOLEAN) (((FirstCount <= Ranges.Range0Max) && (FirstCount >= Ranges.Range0Min)) ||
                   ((SecondCount <= Ranges.Range1Max) && (SecondCount >= Ranges.Range1Min)));
}

/*-------------------------------------------------------------------------------------*/
/**
 * Returns the performance profile features list of the currently running processor core.
 *
 * @param[out]      Features          The performance profile features supported by this platform
 * @param[in]       PlatformConfig    Config handle for platform specific information
 * @param[in]       StdHeader         Header for library and services
 *
 */
VOID
GetPerformanceFeatures (
     OUT   PERFORMANCE_PROFILE_FEATS    *Features,
  IN       PLATFORM_CONFIGURATION       *PlatformConfig,
  IN       AMD_CONFIG_PARAMS            *StdHeader
  )
{
  CPUID_DATA  CpuidDataStruct;
  CPU_SPECIFIC_SERVICES  *FamilySpecificServices;

  Features->PerformanceProfileValue = 0;

  // Reflect Display Refresh Requests use 32 bytes Configuration.
  Features->PerformanceProfileFeatures.RefreshRequest32Byte = 0;
  if (PlatformConfig->PlatformProfile.Use32ByteRefresh) {
    Features->PerformanceProfileFeatures.RefreshRequest32Byte = 1;
  }
  // Reflect Mct Isoc Read Priority set to variable Configuration.
  Features->PerformanceProfileFeatures.MctIsocVariable = 0;
  if (PlatformConfig->PlatformProfile.UseVariableMctIsocPriority) {
    Features->PerformanceProfileFeatures.MctIsocVariable = 1;
  }
  // Indicate if this boot is a warm reset.
  Features->PerformanceProfileFeatures.IsWarmReset = 0;
  if (IsWarmReset (StdHeader)) {
    Features->PerformanceProfileFeatures.IsWarmReset = 1;
  }

  // Get L3 Cache present as indicated by CPUID
  Features->PerformanceProfileFeatures.L3Cache = 0;
  Features->PerformanceProfileFeatures.NoL3Cache = 1;
  LibAmdCpuidRead (AMD_CPUID_L2L3Cache_L2TLB, &CpuidDataStruct, StdHeader);
  if (((CpuidDataStruct.EDX_Reg & 0xFFFC0000) >> 18) != 0) {
    Features->PerformanceProfileFeatures.L3Cache = 1;
    Features->PerformanceProfileFeatures.NoL3Cache = 0;
  }

  // Get VRM select high speed from build option.
  Features->PerformanceProfileFeatures.VrmHighSpeed = 0;
  if (PlatformConfig->VrmProperties[CoreVrm].HiSpeedEnable) {
    Features->PerformanceProfileFeatures.VrmHighSpeed = 1;
  }

  // Get some family, model specific performance type info.
  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  ASSERT (FamilySpecificServices != NULL);

  // Is the Northbridge P-State feature enabled
  Features->PerformanceProfileFeatures.NbPstates = 0;
  if (FamilySpecificServices->IsNbPstateEnabled (FamilySpecificServices, PlatformConfig, StdHeader)) {
    Features->PerformanceProfileFeatures.NbPstates = 1;
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the MSR Register Entry.
 *
 * @TableEntryTypeMethod{::MsrRegister}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits.
 *
 * @param[in]     Entry             The MSR register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForMsrEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT64          MsrData;
  MSR_TYPE_ENTRY_DATA *MsrEntry;

  MsrEntry = (MSR_TYPE_ENTRY_DATA *) (*Entry);
  // Even for only single bit fields, use those in the mask.  "Mask nothing" is a bug, even if just by policy.
  ASSERT (MsrEntry->Mask != 0);

  LibAmdMsrRead (MsrEntry->Address, &MsrData, StdHeader);
  MsrData = MsrData & (~(MsrEntry->Mask));
  MsrData = MsrData | MsrEntry->Data;
  LibAmdMsrWrite (MsrEntry->Address, &MsrData, StdHeader);
  // Entry MUST points to next register entry
  (*((MSR_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the PCI Register Entry.
 *
 * @TableEntryTypeMethod{::PciRegister}.
 *
 * Make the current core's PCI address with the function and register for the entry.
 * Read - Modify - Write the PCI register, clearing masked bits, and setting the data bits.
 *
 * @param[in]     Entry             The PCI register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForPciEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32               PciRegData;
  PCI_TYPE_ENTRY_DATA *PciEntry;

  PciEntry = (PCI_TYPE_ENTRY_DATA *) (*Entry);
  // "Mask nothing" is a bug, even if just by policy.
  ASSERT (PciEntry->Mask != 0);

  IDS_OPTION_HOOK (IDS_SET_PCI_REGISTER_ENTRY, PciEntry, StdHeader);

  LibAmdPciRead (AccessWidth32, PciEntry->Address, &PciRegData, StdHeader);
  PciRegData = PciRegData & (~(PciEntry->Mask));
  PciRegData = PciRegData | PciEntry->Data;
  LibAmdPciWrite (AccessWidth32, PciEntry->Address, &PciRegData, StdHeader);
  // Entry MUST points to next register entry
  (*((PCI_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Family Specific Workaround Register Entry.
 *
 * @TableEntryTypeMethod{::FamSpecificWorkaround}.
 *
 * Call the function, passing the data.
 *
 * See if you can use the other entries or make an entry that covers the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model code specific to one case.
 *
 * @param[in]     Entry             The Family Specific Workaround register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForFamSpecificWorkaroundEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT16 NumberOfWorkaroundTableEntries;
  FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA *FamWorkaroundEntry;
  F_FAM_SPECIFIC_WORKAROUND **FamWorkaroundTable;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;

  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  ASSERT (FamilySpecificServices != NULL);
  FamWorkaroundTable = FamilySpecificServices->GetWorkaroundTable (FamilySpecificServices, &NumberOfWorkaroundTableEntries, StdHeader);

  FamWorkaroundEntry = (FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA *) (*Entry);
  ASSERT (FamWorkaroundEntry->FunctionIndex < NumberOfWorkaroundTableEntries);

  (*FamWorkaroundTable[FamWorkaroundEntry->FunctionIndex]) (FamWorkaroundEntry->Data, StdHeader);
  // Entry MUST points to next register entry
  (*((FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Performance Profile PCI Register Entry.
 *
 * @TableEntryTypeMethod{::ProfileFixup}.
 *
 * Check the entry's performance profile features to the platform's and do the
 * PCI register entry if they match.
 *
 * @param[in]     Entry             The Performance Profile register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForPerformanceProfileEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  PERFORMANCE_PROFILE_FEATS      PlatformProfile;
  PROFILE_FIXUP_TYPE_ENTRY_DATA *ProfileFixupEntry;
  PCI_TYPE_ENTRY_DATA           *PciEntry;

  ProfileFixupEntry = (PROFILE_FIXUP_TYPE_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  ASSERT ((ProfileFixupEntry->TypeFeats.PerformanceProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  GetPerformanceFeatures (&PlatformProfile, PlatformConfig, StdHeader);
  if (DoesEntryTypeSpecificInfoMatch (PlatformProfile.PerformanceProfileValue,
                                      ProfileFixupEntry->TypeFeats.PerformanceProfileValue)) {
    PciEntry = &ProfileFixupEntry->PciEntry;
    SetRegisterForPciEntry ((UINT8 **) &PciEntry, PlatformConfig, StdHeader);
  }
  // Entry MUST points to next register entry
  (*((PROFILE_FIXUP_TYPE_ENTRY_DATA **)Entry))++;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Core Counts Performance PCI Register Entry.
 *
 * @TableEntryTypeMethod{::CoreCountsPciRegister}.
 *
 * Check the performance profile.
 * Check the actual core count to the range pair given, and apply if matched.
 *
 * @param[in]     Entry             The PCI register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForCoreCountsPerformanceEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CORE_COUNTS_PCI_TYPE_ENTRY_DATA *CoreCountsPciEntry;
  PERFORMANCE_PROFILE_FEATS        PlatformProfile;
  UINTN                            ActualCoreCount;
  PCI_TYPE_ENTRY_DATA             *PciEntry;

  CoreCountsPciEntry = (CORE_COUNTS_PCI_TYPE_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  ASSERT ((CoreCountsPciEntry->TypeFeats.PerformanceProfileValue & ~(PERFORMANCE_PROFILE_ALL | PERFORMANCE_AND)) == 0);

  GetPerformanceFeatures (&PlatformProfile, PlatformConfig, StdHeader);
  if (DoesEntryTypeSpecificInfoMatch (PlatformProfile.PerformanceProfileValue, CoreCountsPciEntry->TypeFeats.PerformanceProfileValue)) {
    ActualCoreCount = GetActiveCoresInCurrentModule (StdHeader);
    // Check if the actual core count is in either range.
    if (IsEitherCountInRange (ActualCoreCount, ActualCoreCount, CoreCountsPciEntry->CoreCounts.CoreRanges)) {
      PciEntry = &(CoreCountsPciEntry->PciEntry);
      SetRegisterForPciEntry ((UINT8 **) &PciEntry, PlatformConfig, StdHeader);
    }
  }
  // Entry MUST points to next register entry
  (*((CORE_COUNTS_PCI_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Compute Unit Counts PCI Register Entry.
 *
 * @TableEntryTypeMethod{::CompUnitCountsPciRegister}.
 *
 * Check the entry's performance profile features and the compute unit count
 * to the platform's and do the PCI register entry if they match.
 *
 * @param[in]     Entry             The PCI register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForComputeUnitCountsEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  PERFORMANCE_PROFILE_FEATS                PlatformProfile;
  UINTN                                    ComputeUnitCount;
  COMPUTE_UNIT_COUNTS_PCI_TYPE_ENTRY_DATA *CUCountsPciEntry;
  PCI_TYPE_ENTRY_DATA                     *PciEntry;

  CUCountsPciEntry = (COMPUTE_UNIT_COUNTS_PCI_TYPE_ENTRY_DATA *) (*Entry);

  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  ASSERT ((CUCountsPciEntry->TypeFeats.PerformanceProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  GetPerformanceFeatures (&PlatformProfile, PlatformConfig, StdHeader);
  if (DoesEntryTypeSpecificInfoMatch (PlatformProfile.PerformanceProfileValue, CUCountsPciEntry->TypeFeats.PerformanceProfileValue)) {
    ComputeUnitCount = GetNumberOfCompUnitsInCurrentModule (StdHeader);
    // Check if the actual compute unit count is in either range.
    if (IsEitherCountInRange (ComputeUnitCount, ComputeUnitCount, CUCountsPciEntry->ComputeUnitCounts.ComputeUnitRanges)) {
      PciEntry = &(CUCountsPciEntry->PciEntry);
      SetRegisterForPciEntry ((UINT8 **) &PciEntry, PlatformConfig, StdHeader);
    }
  }
  // Entry MUST points to next register entry
  (*((COMPUTE_UNIT_COUNTS_PCI_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Compute Unit Counts MSR Register Entry.
 *
 * @TableEntryTypeMethod{::CompUnitCountsMsr}.
 *
 * Check the entry's compute unit count to the platform's and do the
 * MSR entry if they match.
 *
 * @param[in]     Entry             The PCI register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetMsrForComputeUnitCountsEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINTN ComputeUnitCount;
  COMPUTE_UNIT_COUNTS_MSR_TYPE_ENTRY_DATA *CUCountsMsrEntry;
  MSR_TYPE_ENTRY_DATA *MsrEntry;

  CUCountsMsrEntry = (COMPUTE_UNIT_COUNTS_MSR_TYPE_ENTRY_DATA *) (*Entry);
  ComputeUnitCount = GetNumberOfCompUnitsInCurrentModule (StdHeader);
  // Check if the actual compute unit count is in either range.
  if (IsEitherCountInRange (ComputeUnitCount, ComputeUnitCount, CUCountsMsrEntry->ComputeUnitCounts.ComputeUnitRanges)) {
    MsrEntry = &(CUCountsMsrEntry->MsrEntry);
    SetRegisterForMsrEntry ((UINT8 **) &MsrEntry, PlatformConfig, StdHeader);
  }
  // Entry MUST points to next register entry
  (*((COMPUTE_UNIT_COUNTS_MSR_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Processor Revision Dependent PCI Register Entry.
 *
 * @TableEntryTypeMethod{::CpuRevPciRegister}.
 *
 * Read - Modify - Write the PCI register, clearing masked bits, and setting the data bits,
 * if the current core's revision matches the revision specified in the entry.
 *
 * @param[in]     Entry             The PCI register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForCpuRevPciEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32          PciRegData;
  CPU_LOGICAL_ID  CpuLogicalId;
  CPU_REV_PCI_TYPE_ENTRY_DATA *CpuRevPciEntry;

  CpuRevPciEntry = (CPU_REV_PCI_TYPE_ENTRY_DATA *) (*Entry);
  ASSERT (CpuRevPciEntry->Mask != 0);

  GetLogicalIdOfCurrentCore (&CpuLogicalId, StdHeader);

  if (((CpuLogicalId.Family & CpuRevPciEntry->CpuRevision.Family) != 0) &&
      ((CpuLogicalId.Revision & CpuRevPciEntry->CpuRevision.Revision) != 0)) {
    LibAmdPciRead (AccessWidth32, CpuRevPciEntry->Address, &PciRegData, StdHeader);
    PciRegData = PciRegData & (~(CpuRevPciEntry->Mask));
    PciRegData = PciRegData | CpuRevPciEntry->Data;
    LibAmdPciWrite (AccessWidth32, CpuRevPciEntry->Address, &PciRegData, StdHeader);
  }
  // Entry MUST points to next register entry
  (*((CPU_REV_PCI_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Processor Revision Dependent MSR Entry.
 *
 * @TableEntryTypeMethod{::CpuRevMsr}.
 *
 * Read - Modify - Write the MSR, clearing masked bits, and setting the data bits,
 * if the current core's revision matches the revision specified in the entry.
 *
 * @param[in]     Entry             The MSR entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForCpuRevMsrEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{

  UINT64 MsrData;
  CPU_LOGICAL_ID  CpuLogicalId;
  CPU_REV_MSR_TYPE_ENTRY_DATA *CpuRevMsrEntry;

  CpuRevMsrEntry = (CPU_REV_MSR_TYPE_ENTRY_DATA *) (*Entry);
  ASSERT (CpuRevMsrEntry->Mask != 0);

  GetLogicalIdOfCurrentCore (&CpuLogicalId, StdHeader);

  if (((CpuLogicalId.Family & CpuRevMsrEntry->CpuRevision.Family) != 0) &&
      ((CpuLogicalId.Revision & CpuRevMsrEntry->CpuRevision.Revision) != 0)) {

    LibAmdMsrRead (CpuRevMsrEntry->Address, &MsrData, StdHeader);
    MsrData = MsrData & (~(CpuRevMsrEntry->Mask));
    MsrData = MsrData | CpuRevMsrEntry->Data;
    LibAmdMsrWrite (CpuRevMsrEntry->Address, &MsrData, StdHeader);
  }
  // Entry MUST points to next register entry
  (*((CPU_REV_MSR_TYPE_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Processor Revision Dependent Family Specific Workaround Register Entry.
 *
 * @TableEntryTypeMethod{::CpuRevFamSpecificWorkaround}.
 *
 * Call the function, passing the data, if the current core's revision matches the revision
 * specified in the entry.
 *
 * See if you can use the other entries or make an entry that covers the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model code specific to one case.
 *
 * @param[in]     Entry             The Family Specific Workaround register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegisterForCpuRevFamSpecificWorkaroundEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT16 NumberOfWorkaroundTableEntries;
  CPU_LOGICAL_ID  CpuLogicalId;
  CPU_REV_FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA *CpuRevWorkaroundEntry;
  F_FAM_SPECIFIC_WORKAROUND **FamWorkaroundTable;
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;

  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  ASSERT (FamilySpecificServices != NULL);
  FamWorkaroundTable = FamilySpecificServices->GetWorkaroundTable (FamilySpecificServices, &NumberOfWorkaroundTableEntries, StdHeader);

  CpuRevWorkaroundEntry = (CPU_REV_FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA *) (*Entry);
  ASSERT (CpuRevWorkaroundEntry->FunctionIndex < NumberOfWorkaroundTableEntries);

  GetLogicalIdOfCurrentCore (&CpuLogicalId, StdHeader);

  if (((CpuLogicalId.Family & CpuRevWorkaroundEntry->CpuRevision.Family) != 0) &&
      ((CpuLogicalId.Revision & CpuRevWorkaroundEntry->CpuRevision.Revision) != 0)) {
    (*FamWorkaroundTable[CpuRevWorkaroundEntry->FunctionIndex]) (CpuRevWorkaroundEntry->Data, StdHeader);
  }
  // Entry MUST points to next register entry
  (*((CPU_REV_FAM_SPECIFIC_WORKAROUND_TYPE_ENTRY_DATA **)Entry))++;
}


/*---------------------------------------------------------------------------------------*/
/**
 * Perform the SMU Index/Data Register Entry.
 *
 * @TableEntryTypeMethod{::SmuIndexReg}.
 *
 * Read - Modify - Write the SMU Index/Data register, clearing masked bits, and setting the data bits.
 *
 * See if you can use the other entries or make an entry that covers the fix.
 * After all, the purpose of having a table entry is to @b NOT have code which
 * isn't generic feature code, but is family/model code specific to one case.
 *
 * @param[in]     Entry             The SMU Index Data entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetSmuIndexRegisterEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT32               SmuIndexReg;
  SMU_INDEX_ENTRY_DATA *SmuIndexEntry;

  SmuIndexEntry = (SMU_INDEX_ENTRY_DATA *) (*Entry);
  // "Mask nothing" is a bug, even if just by policy.
  ASSERT (SmuIndexEntry->Mask != 0);

  // Software must wait for 0xD8230000[31][SVIBUSY] to clear to 0 before writing any of the following fields
  if ((SmuIndexEntry->Index == 0xD8230000) || (SmuIndexEntry->Index == 0xD8230004) || (SmuIndexEntry->Index == 0xD823000C)) {
    do {
      GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, 0xD8230000, AccessWidth32, &SmuIndexReg, StdHeader);
    } while ((SmuIndexReg & BIT31) != 0);
  }

  GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, SmuIndexEntry->Index, AccessWidth32, &SmuIndexReg, StdHeader);
  SmuIndexReg = SmuIndexReg & (~(SmuIndexEntry->Mask));
  SmuIndexReg = SmuIndexReg | SmuIndexEntry->Data;
  GnbLibPciIndirectWrite (SMU_INDEX_ADDRESS, SmuIndexEntry->Index, AccessWidth32, &SmuIndexReg, StdHeader);
  // Entry MUST points to next register entry
  (*((SMU_INDEX_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Performance Profile SMU Index/Data Register Entry.
 *
 * @TableEntryTypeMethod{::ProfileFixupSmuIndexReg}.
 *
 * Check the entry's performance profile features to the platform's and do the
 * SMU Index/Data register entry if they match.
 *
 * @param[in]     Entry             The Performance Profile register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
SetSmuIndexRegisterForPerformanceEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  PERFORMANCE_PROFILE_FEATS          PlatformProfile;
  SMU_INDEX_ENTRY_DATA               *SmuIndexEntry;
  PROFILE_FIXUP_SMU_INDEX_ENTRY_DATA *ProfileFixupSmuIndexEntry;

  ProfileFixupSmuIndexEntry = (PROFILE_FIXUP_SMU_INDEX_ENTRY_DATA *) (*Entry);
  // Errors:  Possible values in unused entry space, extra type features, value range checks.
  // Check that the entry type is correct and the actual supplied entry data is appropriate for that entry.
  ASSERT ((ProfileFixupSmuIndexEntry->TypeFeats.PerformanceProfileValue & ~((PERFORMANCE_PROFILE_ALL) | (PERFORMANCE_AND))) == 0);

  GetPerformanceFeatures (&PlatformProfile, PlatformConfig, StdHeader);
  if (DoesEntryTypeSpecificInfoMatch (PlatformProfile.PerformanceProfileValue,
                                      ProfileFixupSmuIndexEntry->TypeFeats.PerformanceProfileValue)) {
    SmuIndexEntry = &(ProfileFixupSmuIndexEntry->SmuIndexEntry);
    SetSmuIndexRegisterEntry ((UINT8 **) &SmuIndexEntry, PlatformConfig, StdHeader);
  }
  // Entry MUST points to next register entry
  (*((PROFILE_FIXUP_SMU_INDEX_ENTRY_DATA **)Entry))++;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Perform the Copy Bitfield Entry.
 *
 * @TableEntryTypeMethod{::CopyBitField}.
 *
 * Copy bitfield from register A to register B
 *
 * @param[in]     Entry             The Performance Profile register entry to perform
 * @param[in]     PlatformConfig    Config handle for platform specific information
 * @param[in]     StdHeader         Config handle for library and services.
 *
 */
VOID
CopyBitFieldEntry (
  IN       UINT8                 **Entry,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  UINT8                    *DestEntry;
  UINT64                    SourceData;
  UINT64                    SourceMask;
  UINT64                    DestData;
  UINT64                    DestMask;
  PCI_ADDR                  PciAddress;
  TABLE_ENTRY_DATA          DestEntryStruct;
  COPY_BIT_FIELD_DESCRIPTION *Source;
  COPY_BIT_FIELD_DESCRIPTION *Destination;
  CPU_SPECIFIC_SERVICES    *FamilySpecificServices;

  Source = &(((COPY_BIT_FIELD_ENTRY_DATA *) (*Entry))->Source);
  Destination = &(((COPY_BIT_FIELD_ENTRY_DATA *) (*Entry))->Destination);
  // Get source data
  switch (Source->RegType) {
  case MsrRegister:
    LibAmdMsrRead (Source->Address, &SourceData, StdHeader);
    break;
  case PciRegister:
    PciAddress.AddressValue = Source->Address;
    LibAmdPciRead (AccessWidth32, PciAddress, &SourceData, StdHeader);
    break;
  case SmuIndexReg:
    GnbLibPciIndirectRead (SMU_INDEX_ADDRESS, Source->Address, AccessWidth32, &SourceData, StdHeader);
    break;
  default:
    ASSERT (FALSE);
    return;
  }

  // Check MSB, LSB, and get SourceMask, SourceData
  if (Source->MSB >= Source->LSB) {
    SourceMask = (1 << (Source->MSB - Source->LSB + 1)) - 1;
    SourceData  = (SourceData >> Source->LSB) & SourceMask;
  } else {
    // The input MSB LSB are wrong. exchange them
    ASSERT (FALSE);
    SourceMask = (1 << (Source->LSB - Source->MSB + 1)) - 1;
    SourceData  = (SourceData >> Source->MSB) & SourceMask;
  }

  // Check MSB, LSB, and get DestMask, DestData
  if (Destination->MSB >= Destination->LSB) {
    DestMask = (1 << (Destination->MSB - Destination->LSB + 1)) - 1;
    DestMask = DestMask << Destination->LSB;
    DestData = SourceData << Destination->LSB;
  } else {
    // The input MSB LSB are wrong. exchange them
    ASSERT (FALSE);
    DestMask = (1 << (Destination->LSB - Destination->MSB + 1)) - 1;
    DestMask = DestMask << Destination->MSB;
    DestData = SourceData << Destination->MSB;
  }
  if (SourceMask > DestMask) {
    ASSERT (FALSE);
    return;
  }

  // Set destination data
  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  ASSERT (FamilySpecificServices != NULL);

  switch (Destination->RegType) {
  case MsrRegister:
    DestEntryStruct.MsrEntry.Address = Destination->Address;
    DestEntryStruct.MsrEntry.Data = DestData;
    DestEntryStruct.MsrEntry.Mask = DestMask;
    break;
  case PciRegister:
    DestEntryStruct.PciEntry.Address.AddressValue = Destination->Address;
    DestEntryStruct.PciEntry.Data = (UINT32) DestData;
    DestEntryStruct.PciEntry.Mask = (UINT32) DestMask;
    break;
  case SmuIndexReg:
    DestEntryStruct.SmuIndexEntry.Index = Destination->Address;
    DestEntryStruct.SmuIndexEntry.Data = (UINT32) DestData;
    DestEntryStruct.SmuIndexEntry.Mask = (UINT32) DestMask;
    break;
  default:
    ASSERT (FALSE);
    return;
  }
  DestEntry = (UINT8 *) &DestEntryStruct;
  FamilySpecificServices->DoTableEntry[Destination->RegType] ((UINT8 **) &DestEntry, PlatformConfig, StdHeader);
  // Entry MUST points to next register entry
  (*((COPY_BIT_FIELD_ENTRY_DATA **)Entry))++;
}

/* -----------------------------------------------------------------------------*/
/**
 * Returns the platform features list of the currently running processor core.
 *
 * @param[out]      Features          The Features supported by this platform
 * @param[in]       PlatformConfig    Config handle for platform specific information
 * @param[in]       StdHeader         Header for library and services
 *
 */
VOID
GetPlatformFeatures (
     OUT   PLATFORM_FEATS         *Features,
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  PCI_ADDR  PciAddress;
  UINT32    CapabilityReg;
  UINT32     Link;
  CPU_SPECIFIC_SERVICES  *FamilySpecificServices;
  UINT32     CoreCount;

  // Start with none.
  Features->PlatformValue = 0;

  switch (PlatformConfig->PlatformProfile.PlatformControlFlowMode) {
  case Nfcm:
    Features->PlatformFeatures.PlatformNfcm = 1;
    break;
  case UmaDr:
    Features->PlatformFeatures.PlatformUma = 1;
    break;
  case UmaIfcm:
    Features->PlatformFeatures.PlatformUmaIfcm = 1;
    break;
  case Ifcm:
    Features->PlatformFeatures.PlatformIfcm = 1;
    break;
  case Iommu:
    Features->PlatformFeatures.PlatformIommu = 1;
    break;
  default:
    ASSERT (FALSE);
  }
  // Check - Single Link?
  // This is based on the implemented links on the package regardless of their
  // connection status.  All processors must match the BSP, so we only check it and
  // not the current node.  We don't care exactly how many links there are, as soon
  // as we find more than one we are done.
  Link = 0;
  PciAddress.AddressValue = MAKE_SBDFO (0, 0, PCI_DEV_BASE, FUNC_0, 0);
  // Until either all capabilities are done or until the desired link is found,
  // keep looking for HT Host Capabilities.
  while (Link < 2) {
    LibAmdPciFindNextCap (&PciAddress, StdHeader);
    if (PciAddress.AddressValue != ILLEGAL_SBDFO) {
      LibAmdPciRead (AccessWidth32, PciAddress, &CapabilityReg, StdHeader);
      if ((CapabilityReg & 0xE00000FF) == 0x20000008) {
        Link++;
      }
      // A capability other than an HT capability, keep looking.
    } else {
      // end of capabilities
      break;
    }
  }
  if (Link < 2) {
    Features->PlatformFeatures.PlatformSingleLink = 1;
  } else {
    Features->PlatformFeatures.PlatformMultiLink = 1;
  }

  // Set the legacy core count bits.
  GetActiveCoresInCurrentSocket (&CoreCount, StdHeader);
  switch (CoreCount) {
  case 1:
    Features->PlatformFeatures.PlatformSingleCore = 1;
    break;
  case 2:
    Features->PlatformFeatures.PlatformDualCore = 1;
    break;
  default:
    Features->PlatformFeatures.PlatformMultiCore = 1;
  }

  //
  // Get some specific platform type info, VC...etc.
  //
  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  ASSERT (FamilySpecificServices != NULL);
  FamilySpecificServices->GetPlatformTypeSpecificInfo (FamilySpecificServices, Features, StdHeader);

}

/*---------------------------------------------------------------------------------------*/
/**
 * Checks if a register table entry applies to the executing core.
 *
 * This function uses a platform features to determine whether or not a
 * register table entry applies to the executing core.
 *
 * @param[in]     PlatformFeatures        The platform features
 * @param[in]     EntryFeatures           The entry's desired platform features
 *
 * @retval        TRUE           This entry should be applied
 * @retval        FALSE          This entry does not apply
 *
 */
BOOLEAN
STATIC
DoesEntryMatchPlatform (
  IN       PLATFORM_FEATS   PlatformFeatures,
  IN       PLATFORM_FEATS   EntryFeatures
  )
{
  BOOLEAN Result;

  Result = FALSE;

  if (EntryFeatures.PlatformFeatures.AndPlatformFeats == 0) {
    // Match if ANY entry feats match a platform feat (an OR test)
    if ((EntryFeatures.PlatformValue & PlatformFeatures.PlatformValue) != 0) {
      Result = TRUE;
    }
  } else {
    // Match if ALL entry feats match a platform feat (an AND test)
    if ((EntryFeatures.PlatformValue & ~(AMD_PF_AND)) ==
        (EntryFeatures.PlatformValue & PlatformFeatures.PlatformValue)) {
      Result = TRUE;
    }
  }

  return Result;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Checks register table entry type specific criteria to the platform.
 *
 * Entry Data Type implementer methods can use this generically to check their own
 * specific criteria.  The method collects the actual platform characteristics and
 * provides them along with the table entry's criteria to this service.
 *
 * There are a couple considerations for any implementer method using this service.
 * The criteria value has to be representable as a UINT32.  The MSB, Bit 31, has to
 * be used as a AND test request if set in the entry.  (The platform value should never
 * have that bit set.)
 *
 * @param[in]     PlatformTypeSpecificFeatures        The platform features
 * @param[in]     EntryTypeFeatures                   The entry's desired platform features
 *
 * @retval        TRUE                                This entry should be applied
 * @retval        FALSE                               This entry does not apply
 *
 */
BOOLEAN
DoesEntryTypeSpecificInfoMatch (
  IN       UINT32   PlatformTypeSpecificFeatures,
  IN       UINT32   EntryTypeFeatures
  )
{
  BOOLEAN Result;

  Result = FALSE;

  if ((EntryTypeFeatures & BIT31) == 0) {
    // Match if ANY entry feats match a platform feat (an OR test)
    if ((EntryTypeFeatures & PlatformTypeSpecificFeatures) != 0) {
      Result = TRUE;
    }
  } else {
    // Match if ALL entry feats match a platform feat (an AND test)
    if ((EntryTypeFeatures & ~(BIT31)) == (EntryTypeFeatures & PlatformTypeSpecificFeatures)) {
      Result = TRUE;
    }
  }
  return Result;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Determine this core's Selector matches.
 *
 * @param[in]  Selector    Is the current core this selector type?
 * @param[in]  StdHeader   Config handle for library and services.
 *
 * @retval  TRUE           Yes, it is.
 * @retval  FALSE          No, it is not.
 */
BOOLEAN
IsCoreSelector (
  IN       UINT32                    Selector,
  IN       AMD_CONFIG_PARAMS        *StdHeader
  )
{
  BOOLEAN Result;

  Result = TRUE;
  ASSERT (Selector < TableCoreSelectorMax);

  if ((Selector == PrimaryCores) && !IsCurrentCorePrimary (StdHeader)) {
    Result = FALSE;
  }
  if ((Selector == ComputeUnitPrimary) && !IsCoreComputeUnitPrimary (FirstCoreIsComputeUnitPrimary, StdHeader)) {
    Result = FALSE;
  }
  if ((Selector == BscCore) && (!IsBsp (StdHeader))) {
    Result = FALSE;
  }
  return Result;
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the registers for this core based on entries in a list of Register Tables.
 *
 * Determine the platform features and this core's logical id.  Get the specific table
 * entry type implementations for the logical model, which may be either generic (the ones
 * in this file) or specific.
 *
 * Scan the tables starting the with ones for all cores and progressively narrowing the selection
 * based on this core's role (ex. primary core).  For a selected table, check for each entry
 * matching the current core and platform, and call the implementer method to perform the
 * register set operation if it matches.
 *
 * @param[in]  PlatformConfig    Config handle for platform specific information
 * @param[in]  RegisterEntry     RegisterEntry
 * @param[in]  StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegistersFromTable (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       UINT8                  *RegisterEntry,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  CPU_SPECIFIC_SERVICES *FamilySpecificServices;
  TABLE_ENTRY_FIELDS    *Entries;
  TABLE_ENTRY_DATA      *EntryData;
  UINT16                 EntryType;

  GetCpuServicesOfCurrentCore ((CONST CPU_SPECIFIC_SERVICES **) &FamilySpecificServices, StdHeader);
  ASSERT (FamilySpecificServices != NULL);

  // Entries Format:
  //
  // UINT16          EntryType \
  // VariableLength  EntryData / one entry
  // UINT16          EntryType \
  // VariableLength  EntryData / one entry
  // ...                       \
  // ...                       / more entries...
  Entries = (TABLE_ENTRY_FIELDS *) RegisterEntry;                    // Get the first entry
  EntryType = Entries->EntryType;                                    // Get EntryType
  EntryData = &(Entries->EntryData);                                 // Get EntryData block
  while (EntryType != TableTerminator) {
    if (EntryType < TableEntryTypeMax) {
      // EntryData will be added with correct size by DoTableEntry ()
      // After that, it points to the next entry
      FamilySpecificServices->DoTableEntry[EntryType] ((UINT8 **) &EntryData, PlatformConfig, StdHeader);
      Entries = (TABLE_ENTRY_FIELDS *) EntryData;                    // Get the next entry
      EntryType = Entries->EntryType;                                // Get EntryType
      EntryData = &(Entries->EntryData);                             // Get EntryData
    } else {
      ASSERT (FALSE);
    }
  }
}


/*---------------------------------------------------------------------------------------*/
/**
 * Set the registers for this core based on entries in a list of Register Tables.
 *
 * Determine the platform features and this core's logical id.  Get the specific table
 * entry type implementations for the logical model, which may be either generic (the ones
 * in this file) or specific.
 *
 * Scan the tables starting the with ones for all cores and progressively narrowing the selection
 * based on this core's role (ex. primary core).  For a selected table, check for each entry
 * matching the current core and platform, and call the implementer method to perform the
 * register set operation if it matches.
 *
 * @param[in]  PlatformConfig    Config handle for platform specific information
 * @param[in]  RegisterTableList RegisterTableList
 * @param[in]  StdHeader         Config handle for library and services.
 *
 */
VOID
SetRegistersFromTableList (
  IN       PLATFORM_CONFIGURATION *PlatformConfig,
  IN       REGISTER_TABLE         **RegisterTableList,
  IN       AMD_CONFIG_PARAMS      *StdHeader
  )
{
  TABLE_CORE_SELECTOR    Selector;
  REGISTER_TABLE       **TableHandle;
  REGISTER_TABLE       **RegisterTable;

  for (Selector = AllCores; Selector < TableCoreSelectorMax; Selector++) {
    if (IsCoreSelector (Selector, StdHeader)) {
      // If the current core is the selected type of core, work the table list for tables for that type of core.
      TableHandle = NULL;
      RegisterTable = GetNextRegisterTable (Selector, RegisterTableList, &TableHandle, StdHeader);
      while (*RegisterTable != NULL) {
        SetRegistersFromTable (PlatformConfig, (UINT8 *) ((*RegisterTable)->Table), StdHeader);
        RegisterTable = GetNextRegisterTable (Selector, RegisterTableList, &TableHandle, StdHeader);
      }
    } else {
      // Once a selector does not match the current core, quit looking.
      break;
    }
  }
}

/*---------------------------------------------------------------------------------------*/
/**
 * Set the registers for this core based on entries in a list of Register Tables.
 *
 * This function acts as a wrapper for calling the SetRegistersFromTable
 * routine at AmdInitEarly.
 *
 *  @param[in]   PlatformConfig      Config handle for platform specific information.
 *  @param[in]   StdHeader           Config handle for library and services.
 *  @param[in]   TimePoint           Point in time of execution.
 *
 */
AGESA_STATUS
SetRegistersFromTablesAtGivenTimePoint (
  IN       VOID                      *PlatformConfig,
  IN       REGISTER_TABLE_TIME_POINT  TimePoint,
  IN       AMD_CONFIG_PARAMS         *StdHeader
  )
{
  CPU_LOGICAL_ID              CpuLogicalId;
  CPU_SPECIFIC_SERVICES      *FamilySpecificServices;
  REGISTER_TABLE            **RegisterTableList;
  REGISTER_TABLE_AT_GIVEN_TP *RegTableListAtGivenTP;
  AGESA_STATUS                Status;

  if (TimePoint >= MaxAmdRegisterTableTps) {
    ASSERT (FALSE);
    return AGESA_BOUNDS_CHK;
  }

  GetLogicalIdOfCurrentCore (&CpuLogicalId, StdHeader);
  GetCpuServicesFromLogicalId (&CpuLogicalId, &FamilySpecificServices, StdHeader);

  RegTableListAtGivenTP = FamilySpecificServices->GetRegisterTableList (FamilySpecificServices, StdHeader);

  RegisterTableList = NULL;
  while (RegTableListAtGivenTP->TimePoint != MaxAmdRegisterTableTps) {
    if (RegTableListAtGivenTP->TimePoint == TimePoint) {
      RegisterTableList = (REGISTER_TABLE **) RegTableListAtGivenTP->TableList;
      break;
    }
    RegTableListAtGivenTP++;
  }
  if (RegisterTableList != NULL) {
    SetRegistersFromTableList (PlatformConfig, RegisterTableList, StdHeader);
    Status = AGESA_SUCCESS;
  } else {
    Status = AGESA_UNSUPPORTED;
  }
  return Status;
}

