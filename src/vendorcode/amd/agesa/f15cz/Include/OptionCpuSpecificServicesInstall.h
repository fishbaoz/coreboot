/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of CPU specific services support
 *
 * This file generates the CPU specific services tables.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Include
 * @e \$Revision: 313528 $   @e \$Date: 2015-02-24 22:23:58 +0800 (Tue, 24 Feb 2015) $
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

/*

NOTE:

1. This file is designed to be included multiple times in one file. So there's no includsion guard like below.

#ifndef _OPTION_CPU_FAMILY_SERVICE_INSTALL_DEFAULT_H_
#define _OPTION_CPU_FAMILY_SERVICE_INSTALL_DEFAULT_H_
#endif // _OPTION_CPU_FAMILY_SERVICE_INSTALL_DEFAULT_H_


2. This file is designed to be included in below form.

//
// 1st family model specific definitions
//
#include "OptionCpuSpecificServicesInstallReset.h"
#define CpuSrvcTableName                                        cpuFamilyModelServices
// Put your service definitions "CpuSrvc<ServiceName>" here.
#define CpuSrvcDisablePstate                                    cpuF1stM1stDisablePstate
#include "OptionCpuSpecificServicesInstall.h"
INSTALL_CPU_SPECIFIC_SERVICES_TABLE (CpuSrvcTableName);

//
// 2nd family model specific definitions
//
#include "OptionCpuSpecificServicesInstallReset.h"
#define CpuSrvcTableName                                        cpuFamilyModelServices
// Put your service definitions "CpuSrvc<ServiceName>" here.
#define CpuSrvcDisablePstate                                    cpuF2ndM2ndDisablePstate
#include "OptionCpuSpecificServicesInstall.h"
INSTALL_CPU_SPECIFIC_SERVICES_TABLE (CpuSrvcTableName);


Example of CPU specific services definitions:

NOTE: Members with type casting should use OvrdDfltCpuSrvc<ServiceName> instead due to automatical "extern" limitation.
#define CpuSrvcRevision                                (UINT16)                                           Revision
#define CpuSrvcDisablePstate                           (PF_CPU_DISABLE_PSTATE)                            DisablePstate
#define CpuSrvcTransitionPstate                        (PF_CPU_TRANSITION_PSTATE)                         TransitionPstate
#define CpuSrvcGetProcIddMax                           (PF_CPU_GET_IDD_MAX)                               GetProcIddMax
#define CpuSrvcGetTscRate                              (PF_CPU_GET_TSC_RATE)                              GetTscRate
#define CpuSrvcGetCurrentNbFrequency                   (PF_CPU_GET_NB_FREQ)                               GetCurrentNbFrequency
#define CpuSrvcGetMinMaxNbFrequency                    (PF_CPU_GET_MIN_MAX_NB_FREQ)                       GetMinMaxNbFrequency
#define CpuSrvcGetNbPstateInfo                         (PF_CPU_GET_NB_PSTATE_INFO)                        GetNbPstateInfo
#define CpuSrvcIsNbCofInitNeeded                       (PF_CPU_IS_NBCOF_INIT_NEEDED)                      IsNbCofInitNeeded
#define CpuSrvcGetNbIddMax                             (PF_CPU_GET_NB_IDD_MAX)                            GetNbIddMax
#define CpuSrvcLaunchApCore                            (PF_CPU_AP_INITIAL_LAUNCH)                         LaunchApCore
#define CpuSrvcGetNumberOfPhysicalCores                (PF_CPU_NUMBER_OF_PHYSICAL_CORES)                  GetNumberOfPhysicalCores
#define CpuSrvcGetApCoreNumber                         (PF_CPU_GET_AP_CORE_NUMBER)                        GetApCoreNumber
#define CpuSrvcCoreIdPositionInInitialApicId           (PF_CORE_ID_POSITION_IN_INITIAL_APIC_ID)           CoreIdPositionInInitialApicId
#define CpuSrvcSetWarmResetFlag                        (PF_CPU_SET_WARM_RESET_FLAG)                       SetWarmResetFlag
#define CpuSrvcGetWarmResetFlag                        (PF_CPU_GET_WARM_RESET_FLAG)                       GetWarmResetFlag
#define CpuSrvcGetMicroCodePatchesStruct               (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)                 GetMicroCodePatchesStruct
#define CpuSrvcGetMicrocodeEquivalenceTable            (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)                 GetMicrocodeEquivalenceTable
#define CpuSrvcGetCacheInfo                            (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)                 GetCacheInfo
#define CpuSrvcGetSysPmTableStruct                     (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)                 GetSysPmTableStruct
#define CpuSrvcGetWheaInitData                         (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)                 GetWheaInitData
#define CpuSrvcGetPlatformTypeSpecificInfo             (PF_CPU_GET_PLATFORM_TYPE_SPECIFIC_INFO)           GetPlatformTypeSpecificInfo
#define CpuSrvcIsNbPstateEnabled                       (PF_IS_NB_PSTATE_ENABLED)                          IsNbPstateEnabled
// DoTableEntry[TableEntryTypeMax]
  #define CpuSrvcDoEntryTypeMsrRegister                (PF_DO_TABLE_ENTRY)                                DoTableEntry[MsrRegister]
  #define CpuSrvcDoEntryTypePciRegister                (PF_DO_TABLE_ENTRY)                                DoTableEntry[PciRegister]
  #define CpuSrvcDoEntryTypeFamSpecificWorkaround      (PF_DO_TABLE_ENTRY)                                DoTableEntry[FamSpecificWorkaround]
  #define CpuSrvcDoEntryTypeProfileFixup               (PF_DO_TABLE_ENTRY)                                DoTableEntry[ProfileFixup]
  #define CpuSrvcDoEntryTypeCoreCountsPciRegister      (PF_DO_TABLE_ENTRY)                                DoTableEntry[CoreCountsPciRegister]
  #define CpuSrvcDoEntryTypeCompUnitCountsPciRegister  (PF_DO_TABLE_ENTRY)                                DoTableEntry[CompUnitCountsPciRegister]
  #define CpuSrvcDoEntryTypeCompUnitCountsMsr          (PF_DO_TABLE_ENTRY)                                DoTableEntry[CompUnitCountsMsr]
  #define CpuSrvcDoEntryTypeCpuRevPciRegister          (PF_DO_TABLE_ENTRY)                                DoTableEntry[CpuRevPciRegister]
  #define CpuSrvcDoEntryTypeCpuRevMsr                  (PF_DO_TABLE_ENTRY)                                DoTableEntry[CpuRevMsr]
  #define CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround (PF_DO_TABLE_ENTRY)                               DoTableEntry[CpuRevFamSpecificWorkaround]
  #define CpuSrvcDoEntryTypeSmuIndexReg                (PF_DO_TABLE_ENTRY)                                DoTableEntry[SmuIndexReg]
  #define CpuSrvcDoEntryTypeProfileFixupSmuIndexReg    (PF_DO_TABLE_ENTRY)                                DoTableEntry[ProfileFixupSmuIndexReg]
  #define CpuSrvcDoEntryTypeCopyBitField               (PF_DO_TABLE_ENTRY)                                DoTableEntry[CopyBitField]
#define CpuSrvcGetRegisterTableList                    (PF_GET_REGISTER_TABLE_LIST)                       GetRegisterTableList
#define CpuSrvcGetWorkaroundTable                      (PF_GET_WORKAROUND_TABLE)                          GetWorkaroundTable
#define CpuSrvcComputeUnitMap                          (COMPUTE_UNIT_MAP *)                               ComputeUnitMap
#define CpuSrvcInitCacheDisabled                       (FAMILY_CACHE_INIT_POLICY)                         InitCacheDisabled
#define CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable   (PF_GET_EARLY_INIT_TABLE)                          GetEarlyInitBeforeApLaunchOnCoreTable
#define CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable    (PF_GET_EARLY_INIT_TABLE)                          GetEarlyInitAfterApLaunchOnCoreTable
#define CpuSrvcIsSecureS3                              (PF_IS_SECURE_S3)                                  IsSecureS3
#define CpuSrvcPatchLoaderIsSharedByCU                 (BOOLEAN)                                          PatchLoaderIsSharedByCU

  USAGE MODEL:

  //
  // If BUILD_CONFIG_SWITCH = TRUE
  //   If override service is defined, use it as the final service definition.
  //   If override service is not define, use default service (DlftCpuSrvc) as the final service definition.
  //
  // If BUILD_CONFIG_SWITCH = FALSE
  //   Use default assert service (AssertCpuSrvc) as the final service definition for making a assertion when unsupported service is called.
  //
  // "extern" will be used for function pointer (exclude data) type serive members automatically when CpuSrvc<ServiceName> is defined.
  //

// Member: (MEMBER_TYPE) ServiceName
#if BUILD_CONFIG_SWITCH == TRUE
  #ifdef CpuSrvcServiceName
    #define FinalCpuSrvcServiceName           CpuSrvcServiceName
    extern  MEMBER_TYPE                       FinalCpuSrvcServiceName;
  #else
    #define FinalCpuSrvcServiceName           DfltCpuSrvcServiceName
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcServiceName) )
  #endif
#else
  #define   FinalCpuSrvcServiceName           DfltAssertCpuSrvcServiceName
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcServiceName) )
#endif

*/


#define STRING2(x)                                #x
#define STRING(x)                                 STRING2(x)
#define CAT_STRING(a, b)                          a##b
#define STRING_ARROW                              " --> "

//
// Verbose control for messaging CPU services at build time
//
#ifndef VERBOSE_CPU_SERVICES
  #define VERBOSE_CPU_SERVICES                    0 // 0 - disable messaging, 1 - enable debugging.
#endif

#if VERBOSE_CPU_SERVICES
  #define STRING_MACRO_VALUE(Name)                " " #Name " = " STRING(Name)
  #define STRING_OVERRIDE_MACRO_VALUE(Name)       "  OVERRIDE: " #Name " : " STRING(Name) STRING_ARROW STRING(Ovrd##Name)
  #define STRING_MACRO_DEFAULT_USED(Name)         "  NOT DEFINED: "  #Name STRING_ARROW " DEFAULT: " STRING(FinalDflt##Name)
  #define STRING_MACRO_DEFAULT_ASSERT_USED(Name)  "  DISABLED: " #Name ", assertion is used."

  #define STRING_SEPARATOR1                       "-----------------------------------------------------------------------------------------------"
  #define STRING_SEPARATOR2                       "==============================================================================================="
  #define STRING_SEPARATOR_POUND                  "###############################################################################################"
#else // VERBOSE_CPU_SERVICES
  #define STRING_MACRO_VALUE(Name)                NULL_STRING
  #define STRING_OVERRIDE_MACRO_VALUE(Name)       NULL_STRING
  #define STRING_MACRO_DEFAULT_USED(Name)         NULL_STRING
  #define STRING_MACRO_DEFAULT_ASSERT_USED(Name)  NULL_STRING


  #define NULL_STRING                             ""
  #define STRING_SEPARATOR1                       NULL_STRING
  #define STRING_SEPARATOR2                       NULL_STRING
  #define STRING_SEPARATOR_POUND                  NULL_STRING
#endif // VERBOSE_CPU_SERVICES

//
// Handle override definitions on DfltAssertCpuSrvc<ServiceName>
//
#pragma message( STRING_SEPARATOR_POUND )
#pragma message( STRING_MACRO_VALUE(CpuSrvcTableName) )
#pragma message( STRING_SEPARATOR2 )

#ifdef OvrdDfltAssertCpuSrvcRevision
  #define FinalDfltAssertCpuSrvcRevision                              OvrdDfltAssertCpuSrvcRevision
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcRevision) )
#else
  #define FinalDfltAssertCpuSrvcRevision                              DfltAssertCpuSrvcRevision
#endif

#ifdef OvrdDfltAssertCpuSrvcDisablePstate
  #define FinalDfltAssertCpuSrvcDisablePstate                         OvrdDfltAssertCpuSrvcDisablePstate
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDisablePstate) )
#else
  #define FinalDfltAssertCpuSrvcDisablePstate                         DfltAssertCpuSrvcDisablePstate
#endif

#ifdef OvrdDfltAssertCpuSrvcTransitionPstate
  #define FinalDfltAssertCpuSrvcTransitionPstate                      OvrdDfltAssertCpuSrvcTransitionPstate
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcTransitionPstate) )
#else
  #define FinalDfltAssertCpuSrvcTransitionPstate                      DfltAssertCpuSrvcTransitionPstate
#endif

#ifdef OvrdDfltAssertCpuSrvcGetProcIddMax
  #define FinalDfltAssertCpuSrvcGetProcIddMax                         OvrdDfltAssertCpuSrvcGetProcIddMax
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetProcIddMax) )
#else
  #define FinalDfltAssertCpuSrvcGetProcIddMax                         DfltAssertCpuSrvcGetProcIddMax
#endif

#ifdef OvrdDfltAssertCpuSrvcGetTscRate
  #define FinalDfltAssertCpuSrvcGetTscRate                            OvrdDfltAssertCpuSrvcGetTscRate
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetTscRate) )
#else
  #define FinalDfltAssertCpuSrvcGetTscRate                            DfltAssertCpuSrvcGetTscRate
#endif

#ifdef OvrdDfltAssertCpuSrvcGetCurrentNbFrequency
  #define FinalDfltAssertCpuSrvcGetCurrentNbFrequency                 OvrdDfltAssertCpuSrvcGetCurrentNbFrequency
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetCurrentNbFrequency) )
#else
  #define FinalDfltAssertCpuSrvcGetCurrentNbFrequency                 DfltAssertCpuSrvcGetCurrentNbFrequency
#endif

#ifdef OvrdDfltAssertCpuSrvcGetMinMaxNbFrequency
  #define FinalDfltAssertCpuSrvcGetMinMaxNbFrequency                  OvrdDfltAssertCpuSrvcGetMinMaxNbFrequency
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetMinMaxNbFrequency) )
#else
  #define FinalDfltAssertCpuSrvcGetMinMaxNbFrequency                  DfltAssertCpuSrvcGetMinMaxNbFrequency
#endif

#ifdef OvrdDfltAssertCpuSrvcGetNbPstateInfo
  #define FinalDfltAssertCpuSrvcGetNbPstateInfo                       OvrdDfltAssertCpuSrvcGetNbPstateInfo
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetNbPstateInfo) )
#else
  #define FinalDfltAssertCpuSrvcGetNbPstateInfo                       DfltAssertCpuSrvcGetNbPstateInfo
#endif

#ifdef OvrdDfltAssertCpuSrvcIsNbCofInitNeeded
  #define FinalDfltAssertCpuSrvcIsNbCofInitNeeded                     OvrdDfltAssertCpuSrvcIsNbCofInitNeeded
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcIsNbCofInitNeeded) )
#else
  #define FinalDfltAssertCpuSrvcIsNbCofInitNeeded                     DfltAssertCpuSrvcIsNbCofInitNeeded
#endif

#ifdef OvrdDfltAssertCpuSrvcGetNbIddMax
  #define FinalDfltAssertCpuSrvcGetNbIddMax                           OvrdDfltAssertCpuSrvcGetNbIddMax
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetNbIddMax) )
#else
  #define FinalDfltAssertCpuSrvcGetNbIddMax                           DfltAssertCpuSrvcGetNbIddMax
#endif

#ifdef OvrdDfltAssertCpuSrvcLaunchApCore
  #define FinalDfltAssertCpuSrvcLaunchApCore                          OvrdDfltAssertCpuSrvcLaunchApCore
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcLaunchApCore) )
#else
  #define FinalDfltAssertCpuSrvcLaunchApCore                          DfltAssertCpuSrvcLaunchApCore
#endif

#ifdef OvrdDfltAssertCpuSrvcGetNumberOfPhysicalCores
  #define FinalDfltAssertCpuSrvcGetNumberOfPhysicalCores              OvrdDfltAssertCpuSrvcGetNumberOfPhysicalCores
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetNumberOfPhysicalCores) )
#else
  #define FinalDfltAssertCpuSrvcGetNumberOfPhysicalCores              DfltAssertCpuSrvcGetNumberOfPhysicalCores
#endif

#ifdef OvrdDfltAssertCpuSrvcGetApCoreNumber
  #define FinalDfltAssertCpuSrvcGetApCoreNumber                       OvrdDfltAssertCpuSrvcGetApCoreNumber
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetApCoreNumber) )
#else
  #define FinalDfltAssertCpuSrvcGetApCoreNumber                       DfltAssertCpuSrvcGetApCoreNumber
#endif

#ifdef OvrdDfltAssertCpuSrvcCoreIdPositionInInitialApicId
  #define FinalDfltAssertCpuSrvcCoreIdPositionInInitialApicId         OvrdDfltAssertCpuSrvcCoreIdPositionInInitialApicId
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcCoreIdPositionInInitialApicId) )
#else
  #define FinalDfltAssertCpuSrvcCoreIdPositionInInitialApicId         DfltAssertCpuSrvcCoreIdPositionInInitialApicId
#endif

#ifdef OvrdDfltAssertCpuSrvcSetWarmResetFlag
  #define FinalDfltAssertCpuSrvcSetWarmResetFlag                      OvrdDfltAssertCpuSrvcSetWarmResetFlag
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcSetWarmResetFlag) )
#else
  #define FinalDfltAssertCpuSrvcSetWarmResetFlag                      DfltAssertCpuSrvcSetWarmResetFlag
#endif

#ifdef OvrdDfltAssertCpuSrvcGetWarmResetFlag
  #define FinalDfltAssertCpuSrvcGetWarmResetFlag                      OvrdDfltAssertCpuSrvcGetWarmResetFlag
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetWarmResetFlag) )
#else
  #define FinalDfltAssertCpuSrvcGetWarmResetFlag                      DfltAssertCpuSrvcGetWarmResetFlag
#endif

#ifdef OvrdDfltAssertCpuSrvcGetMicroCodePatchesStruct
  #define FinalDfltAssertCpuSrvcGetMicroCodePatchesStruct             OvrdDfltAssertCpuSrvcGetMicroCodePatchesStruct
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetMicroCodePatchesStruct) )
#else
  #define FinalDfltAssertCpuSrvcGetMicroCodePatchesStruct             DfltAssertCpuSrvcGetMicroCodePatchesStruct
#endif

#ifdef OvrdDfltAssertCpuSrvcGetMicrocodeEquivalenceTable
  #define FinalDfltAssertCpuSrvcGetMicrocodeEquivalenceTable          OvrdDfltAssertCpuSrvcGetMicrocodeEquivalenceTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetMicrocodeEquivalenceTable) )
#else
  #define FinalDfltAssertCpuSrvcGetMicrocodeEquivalenceTable          DfltAssertCpuSrvcGetMicrocodeEquivalenceTable
#endif

#ifdef OvrdDfltAssertCpuSrvcGetCacheInfo
  #define FinalDfltAssertCpuSrvcGetCacheInfo                          OvrdDfltAssertCpuSrvcGetCacheInfo
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetCacheInfo) )
#else
  #define FinalDfltAssertCpuSrvcGetCacheInfo                          DfltAssertCpuSrvcGetCacheInfo
#endif

#ifdef OvrdDfltAssertCpuSrvcGetSysPmTableStruct
  #define FinalDfltAssertCpuSrvcGetSysPmTableStruct                   OvrdDfltAssertCpuSrvcGetSysPmTableStruct
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetSysPmTableStruct) )
#else
  #define FinalDfltAssertCpuSrvcGetSysPmTableStruct                   DfltAssertCpuSrvcGetSysPmTableStruct
#endif

#ifdef OvrdDfltAssertCpuSrvcGetWheaInitData
  #define FinalDfltAssertCpuSrvcGetWheaInitData                       OvrdDfltAssertCpuSrvcGetWheaInitData
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetWheaInitData) )
#else
  #define FinalDfltAssertCpuSrvcGetWheaInitData                       DfltAssertCpuSrvcGetWheaInitData
#endif

#ifdef OvrdDfltAssertCpuSrvcGetPlatformTypeSpecificInfo
  #define FinalDfltAssertCpuSrvcGetPlatformTypeSpecificInfo           OvrdDfltAssertCpuSrvcGetPlatformTypeSpecificInfo
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetPlatformTypeSpecificInfo) )
#else
  #define FinalDfltAssertCpuSrvcGetPlatformTypeSpecificInfo           DfltAssertCpuSrvcGetPlatformTypeSpecificInfo
#endif

#ifdef OvrdDfltAssertCpuSrvcIsNbPstateEnabled
  #define FinalDfltAssertCpuSrvcIsNbPstateEnabled                     OvrdDfltAssertCpuSrvcIsNbPstateEnabled
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcIsNbPstateEnabled) )
#else
  #define FinalDfltAssertCpuSrvcIsNbPstateEnabled                     DfltAssertCpuSrvcIsNbPstateEnabled
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeMsrRegister
  #define FinalDfltAssertCpuSrvcDoEntryTypeMsrRegister                OvrdDfltAssertCpuSrvcDoEntryTypeMsrRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeMsrRegister )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeMsrRegister                DfltAssertCpuSrvcDoEntryTypeMsrRegister
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypePciRegister
  #define FinalDfltAssertCpuSrvcDoEntryTypePciRegister                OvrdDfltAssertCpuSrvcDoEntryTypePciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypePciRegister )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypePciRegister                DfltAssertCpuSrvcDoEntryTypePciRegister
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
  #define FinalDfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround      OvrdDfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround      DfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeProfileFixup
  #define FinalDfltAssertCpuSrvcDoEntryTypeProfileFixup               OvrdDfltAssertCpuSrvcDoEntryTypeProfileFixup
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeProfileFixup )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeProfileFixup               DfltAssertCpuSrvcDoEntryTypeProfileFixup
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
  #define FinalDfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister      OvrdDfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister      DfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
  #define FinalDfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister  OvrdDfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister  DfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
  #define FinalDfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr          OvrdDfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr          DfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
  #define FinalDfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister          OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister          DfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevMsr
  #define FinalDfltAssertCpuSrvcDoEntryTypeCpuRevMsr                  OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevMsr
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCpuRevMsr )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCpuRevMsr                  DfltAssertCpuSrvcDoEntryTypeCpuRevMsr
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  #define FinalDfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround DfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeSmuIndexReg
  #define FinalDfltAssertCpuSrvcDoEntryTypeSmuIndexReg                OvrdDfltAssertCpuSrvcDoEntryTypeSmuIndexReg
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeSmuIndexReg )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeSmuIndexReg                DfltAssertCpuSrvcDoEntryTypeSmuIndexReg
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  #define FinalDfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg    OvrdDfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg    DfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#endif

#ifdef OvrdDfltAssertCpuSrvcDoEntryTypeCopyBitField
  #define FinalDfltAssertCpuSrvcDoEntryTypeCopyBitField               OvrdDfltAssertCpuSrvcDoEntryTypeCopyBitField
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcDoEntryTypeCopyBitField )
#else
  #define FinalDfltAssertCpuSrvcDoEntryTypeCopyBitField               DfltAssertCpuSrvcDoEntryTypeCopyBitField
#endif

#ifdef OvrdDfltAssertCpuSrvcGetRegisterTableList
  #define FinalDfltAssertCpuSrvcGetRegisterTableList                  OvrdDfltAssertCpuSrvcGetRegisterTableList
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetRegisterTableList) )
#else
  #define FinalDfltAssertCpuSrvcGetRegisterTableList                  DfltAssertCpuSrvcGetRegisterTableList
#endif

#ifdef OvrdDfltAssertCpuSrvcGetWorkaroundTable
  #define FinalDfltAssertCpuSrvcGetWorkaroundTable                    OvrdDfltAssertCpuSrvcGetWorkaroundTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetWorkaroundTable) )
#else
  #define FinalDfltAssertCpuSrvcGetWorkaroundTable                    DfltAssertCpuSrvcGetWorkaroundTable
#endif

#ifdef OvrdDfltAssertCpuSrvcComputeUnitMap
  #define FinalDfltAssertCpuSrvcComputeUnitMap                        OvrdDfltAssertCpuSrvcComputeUnitMap
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcComputeUnitMap) )
#else
  #define FinalDfltAssertCpuSrvcComputeUnitMap                        DfltAssertCpuSrvcComputeUnitMap
#endif

#ifdef OvrdDfltAssertCpuSrvcInitCacheDisabled
  #define FinalDfltAssertCpuSrvcInitCacheDisabled                     OvrdDfltAssertCpuSrvcInitCacheDisabled
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcInitCacheDisabled) )
#else
  #define FinalDfltAssertCpuSrvcInitCacheDisabled                     DfltAssertCpuSrvcInitCacheDisabled
#endif

#ifdef OvrdDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
  #define FinalDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable OvrdDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable) )
#else
  #define FinalDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable DfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#endif

#ifdef OvrdDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
  #define FinalDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable  OvrdDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable) )
#else
  #define FinalDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable  DfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#endif

#ifdef OvrdDfltAssertCpuSrvcIsSecureS3
  #define FinalDfltAssertCpuSrvcIsSecureS3                            OvrdDfltAssertCpuSrvcIsSecureS3
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcIsSecureS3) )
#else
  #define FinalDfltAssertCpuSrvcIsSecureS3                            DfltAssertCpuSrvcIsSecureS3
#endif

#ifdef OvrdDfltAssertCpuSrvcPatchLoaderIsSharedByCU
  #define FinalDfltAssertCpuSrvcPatchLoaderIsSharedByCU               OvrdDfltAssertCpuSrvcPatchLoaderIsSharedByCU
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltAssertCpuSrvcPatchLoaderIsSharedByCU) )
#else
  #define FinalDfltAssertCpuSrvcPatchLoaderIsSharedByCU               DfltAssertCpuSrvcPatchLoaderIsSharedByCU
#endif


//
// Handle override definitions on DfltCpuSrvc<ServiceName>
//
#pragma message( STRING_SEPARATOR1 )

#ifdef OvrdDfltCpuSrvcRevision
  #define FinalDfltCpuSrvcRevision                                    OvrdDfltCpuSrvcRevision
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcRevision) )
#else
  #define FinalDfltCpuSrvcRevision                                    DfltCpuSrvcRevision
#endif

#ifdef OvrdDfltCpuSrvcDisablePstate
  #define FinalDfltCpuSrvcDisablePstate                               OvrdDfltCpuSrvcDisablePstate
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDisablePstate) )
#else
  #define FinalDfltCpuSrvcDisablePstate                               DfltCpuSrvcDisablePstate
#endif

#ifdef OvrdDfltCpuSrvcTransitionPstate
  #define FinalDfltCpuSrvcTransitionPstate                            OvrdDfltCpuSrvcTransitionPstate
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcTransitionPstate) )
#else
  #define FinalDfltCpuSrvcTransitionPstate                            DfltCpuSrvcTransitionPstate
#endif

#ifdef OvrdDfltCpuSrvcGetProcIddMax
  #define FinalDfltCpuSrvcGetProcIddMax                               OvrdDfltCpuSrvcGetProcIddMax
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetProcIddMax) )
#else
  #define FinalDfltCpuSrvcGetProcIddMax                               DfltCpuSrvcGetProcIddMax
#endif

#ifdef OvrdDfltCpuSrvcGetTscRate
  #define FinalDfltCpuSrvcGetTscRate                                  OvrdDfltCpuSrvcGetTscRate
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetTscRate) )
#else
  #define FinalDfltCpuSrvcGetTscRate                                  DfltCpuSrvcGetTscRate
#endif

#ifdef OvrdDfltCpuSrvcGetCurrentNbFrequency
  #define FinalDfltCpuSrvcGetCurrentNbFrequency                       OvrdDfltCpuSrvcGetCurrentNbFrequency
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetCurrentNbFrequency) )
#else
  #define FinalDfltCpuSrvcGetCurrentNbFrequency                       DfltCpuSrvcGetCurrentNbFrequency
#endif

#ifdef OvrdDfltCpuSrvcGetMinMaxNbFrequency
  #define FinalDfltCpuSrvcGetMinMaxNbFrequency                        OvrdDfltCpuSrvcGetMinMaxNbFrequency
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetMinMaxNbFrequency) )
#else
  #define FinalDfltCpuSrvcGetMinMaxNbFrequency                        DfltCpuSrvcGetMinMaxNbFrequency
#endif

#ifdef OvrdDfltCpuSrvcGetNbPstateInfo
  #define FinalDfltCpuSrvcGetNbPstateInfo                             OvrdDfltCpuSrvcGetNbPstateInfo
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetNbPstateInfo) )
#else
  #define FinalDfltCpuSrvcGetNbPstateInfo                             DfltCpuSrvcGetNbPstateInfo
#endif

#ifdef OvrdDfltCpuSrvcIsNbCofInitNeeded
  #define FinalDfltCpuSrvcIsNbCofInitNeeded                           OvrdDfltCpuSrvcIsNbCofInitNeeded
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcIsNbCofInitNeeded) )
#else
  #define FinalDfltCpuSrvcIsNbCofInitNeeded                           DfltCpuSrvcIsNbCofInitNeeded
#endif

#ifdef OvrdDfltCpuSrvcGetNbIddMax
  #define FinalDfltCpuSrvcGetNbIddMax                                 OvrdDfltCpuSrvcGetNbIddMax
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetNbIddMax) )
#else
  #define FinalDfltCpuSrvcGetNbIddMax                                 DfltCpuSrvcGetNbIddMax
#endif

#ifdef OvrdDfltCpuSrvcLaunchApCore
  #define FinalDfltCpuSrvcLaunchApCore                                OvrdDfltCpuSrvcLaunchApCore
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcLaunchApCore) )
#else
  #define FinalDfltCpuSrvcLaunchApCore                                DfltCpuSrvcLaunchApCore
#endif

#ifdef OvrdDfltCpuSrvcGetNumberOfPhysicalCores
  #define FinalDfltCpuSrvcGetNumberOfPhysicalCores                    OvrdDfltCpuSrvcGetNumberOfPhysicalCores
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetNumberOfPhysicalCores) )
#else
  #define FinalDfltCpuSrvcGetNumberOfPhysicalCores                    DfltCpuSrvcGetNumberOfPhysicalCores
#endif

#ifdef OvrdDfltCpuSrvcGetApCoreNumber
  #define FinalDfltCpuSrvcGetApCoreNumber                             OvrdDfltCpuSrvcGetApCoreNumber
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetApCoreNumber) )
#else
  #define FinalDfltCpuSrvcGetApCoreNumber                             DfltCpuSrvcGetApCoreNumber
#endif

#ifdef OvrdDfltCpuSrvcCoreIdPositionInInitialApicId
  #define FinalDfltCpuSrvcCoreIdPositionInInitialApicId               OvrdDfltCpuSrvcCoreIdPositionInInitialApicId
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcCoreIdPositionInInitialApicId) )
#else
  #define FinalDfltCpuSrvcCoreIdPositionInInitialApicId               DfltCpuSrvcCoreIdPositionInInitialApicId
#endif

#ifdef OvrdDfltCpuSrvcSetWarmResetFlag
  #define FinalDfltCpuSrvcSetWarmResetFlag                            OvrdDfltCpuSrvcSetWarmResetFlag
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcSetWarmResetFlag) )
#else
  #define FinalDfltCpuSrvcSetWarmResetFlag                            DfltCpuSrvcSetWarmResetFlag
#endif

#ifdef OvrdDfltCpuSrvcGetWarmResetFlag
  #define FinalDfltCpuSrvcGetWarmResetFlag                            OvrdDfltCpuSrvcGetWarmResetFlag
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetWarmResetFlag) )
#else
  #define FinalDfltCpuSrvcGetWarmResetFlag                            DfltCpuSrvcGetWarmResetFlag
#endif

#ifdef OvrdDfltCpuSrvcGetMicroCodePatchesStruct
  #define FinalDfltCpuSrvcGetMicroCodePatchesStruct                   OvrdDfltCpuSrvcGetMicroCodePatchesStruct
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetMicroCodePatchesStruct) )
#else
  #define FinalDfltCpuSrvcGetMicroCodePatchesStruct                   DfltCpuSrvcGetMicroCodePatchesStruct
#endif

#ifdef OvrdDfltCpuSrvcGetMicrocodeEquivalenceTable
  #define FinalDfltCpuSrvcGetMicrocodeEquivalenceTable                OvrdDfltCpuSrvcGetMicrocodeEquivalenceTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetMicrocodeEquivalenceTable) )
#else
  #define FinalDfltCpuSrvcGetMicrocodeEquivalenceTable                DfltCpuSrvcGetMicrocodeEquivalenceTable
#endif

#ifdef OvrdDfltCpuSrvcGetCacheInfo
  #define FinalDfltCpuSrvcGetCacheInfo                                OvrdDfltCpuSrvcGetCacheInfo
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetCacheInfo) )
#else
  #define FinalDfltCpuSrvcGetCacheInfo                                DfltCpuSrvcGetCacheInfo
#endif

#ifdef OvrdDfltCpuSrvcGetSysPmTableStruct
  #define FinalDfltCpuSrvcGetSysPmTableStruct                         OvrdDfltCpuSrvcGetSysPmTableStruct
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetSysPmTableStruct) )
#else
  #define FinalDfltCpuSrvcGetSysPmTableStruct                         DfltCpuSrvcGetSysPmTableStruct
#endif

#ifdef OvrdDfltCpuSrvcGetWheaInitData
  #define FinalDfltCpuSrvcGetWheaInitData                             OvrdDfltCpuSrvcGetWheaInitData
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetWheaInitData) )
#else
  #define FinalDfltCpuSrvcGetWheaInitData                             DfltCpuSrvcGetWheaInitData
#endif

#ifdef OvrdDfltCpuSrvcGetPlatformTypeSpecificInfo
  #define FinalDfltCpuSrvcGetPlatformTypeSpecificInfo                 OvrdDfltCpuSrvcGetPlatformTypeSpecificInfo
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetPlatformTypeSpecificInfo) )
#else
  #define FinalDfltCpuSrvcGetPlatformTypeSpecificInfo                 DfltCpuSrvcGetPlatformTypeSpecificInfo
#endif

#ifdef OvrdDfltCpuSrvcIsNbPstateEnabled
  #define FinalDfltCpuSrvcIsNbPstateEnabled                           OvrdDfltCpuSrvcIsNbPstateEnabled
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcIsNbPstateEnabled) )
#else
  #define FinalDfltCpuSrvcIsNbPstateEnabled                           DfltCpuSrvcIsNbPstateEnabled
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeMsrRegister
  #define FinalDfltCpuSrvcDoEntryTypeMsrRegister                      OvrdDfltCpuSrvcDoEntryTypeMsrRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeMsrRegister )
#else
  #define FinalDfltCpuSrvcDoEntryTypeMsrRegister                      DfltCpuSrvcDoEntryTypeMsrRegister
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypePciRegister
  #define FinalDfltCpuSrvcDoEntryTypePciRegister                      OvrdDfltCpuSrvcDoEntryTypePciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypePciRegister )
#else
  #define FinalDfltCpuSrvcDoEntryTypePciRegister                      DfltCpuSrvcDoEntryTypePciRegister
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeFamSpecificWorkaround
  #define FinalDfltCpuSrvcDoEntryTypeFamSpecificWorkaround            OvrdDfltCpuSrvcDoEntryTypeFamSpecificWorkaround
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeFamSpecificWorkaround )
#else
  #define FinalDfltCpuSrvcDoEntryTypeFamSpecificWorkaround            DfltCpuSrvcDoEntryTypeFamSpecificWorkaround
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeProfileFixup
  #define FinalDfltCpuSrvcDoEntryTypeProfileFixup                     OvrdDfltCpuSrvcDoEntryTypeProfileFixup
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeProfileFixup )
#else
  #define FinalDfltCpuSrvcDoEntryTypeProfileFixup                     DfltCpuSrvcDoEntryTypeProfileFixup
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeCoreCountsPciRegister
  #define FinalDfltCpuSrvcDoEntryTypeCoreCountsPciRegister            OvrdDfltCpuSrvcDoEntryTypeCoreCountsPciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCoreCountsPciRegister )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCoreCountsPciRegister            DfltCpuSrvcDoEntryTypeCoreCountsPciRegister
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
  #define FinalDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister        OvrdDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister        DfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeCompUnitCountsMsr
  #define FinalDfltCpuSrvcDoEntryTypeCompUnitCountsMsr                OvrdDfltCpuSrvcDoEntryTypeCompUnitCountsMsr
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCompUnitCountsMsr )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCompUnitCountsMsr                DfltCpuSrvcDoEntryTypeCompUnitCountsMsr
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeCpuRevPciRegister
  #define FinalDfltCpuSrvcDoEntryTypeCpuRevPciRegister                OvrdDfltCpuSrvcDoEntryTypeCpuRevPciRegister
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCpuRevPciRegister )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCpuRevPciRegister                DfltCpuSrvcDoEntryTypeCpuRevPciRegister
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeCpuRevMsr
  #define FinalDfltCpuSrvcDoEntryTypeCpuRevMsr                        OvrdDfltCpuSrvcDoEntryTypeCpuRevMsr
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCpuRevMsr )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCpuRevMsr                        DfltCpuSrvcDoEntryTypeCpuRevMsr
#endif


#ifdef OvrdDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  #define FinalDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround      OvrdDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround      DfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeSmuIndexReg
  #define FinalDfltCpuSrvcDoEntryTypeSmuIndexReg                      OvrdDfltCpuSrvcDoEntryTypeSmuIndexReg
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeSmuIndexReg )
#else
  #define FinalDfltCpuSrvcDoEntryTypeSmuIndexReg                      DfltCpuSrvcDoEntryTypeSmuIndexReg
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  #define FinalDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg          OvrdDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg )
#else
  #define FinalDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg          DfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#endif

#ifdef OvrdDfltCpuSrvcDoEntryTypeCopyBitField
  #define FinalDfltCpuSrvcDoEntryTypeCopyBitField                     OvrdDfltCpuSrvcDoEntryTypeCopyBitField
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcDoEntryTypeCopyBitField )
#else
  #define FinalDfltCpuSrvcDoEntryTypeCopyBitField                     DfltCpuSrvcDoEntryTypeCopyBitField
#endif

#ifdef OvrdDfltCpuSrvcGetRegisterTableList
  #define FinalDfltCpuSrvcGetRegisterTableList                        OvrdDfltCpuSrvcGetRegisterTableList
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetRegisterTableList) )
#else
  #define FinalDfltCpuSrvcGetRegisterTableList                        DfltCpuSrvcGetRegisterTableList
#endif

#ifdef OvrdDfltCpuSrvcGetWorkaroundTable
  #define FinalDfltCpuSrvcGetWorkaroundTable                          OvrdDfltCpuSrvcGetWorkaroundTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetWorkaroundTable) )
#else
  #define FinalDfltCpuSrvcGetWorkaroundTable                          DfltCpuSrvcGetWorkaroundTable
#endif

#ifdef OvrdDfltCpuSrvcComputeUnitMap
  #define FinalDfltCpuSrvcComputeUnitMap                              OvrdDfltCpuSrvcComputeUnitMap
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcComputeUnitMap) )
#else
  #define FinalDfltCpuSrvcComputeUnitMap                              DfltCpuSrvcComputeUnitMap
#endif

#ifdef OvrdDfltCpuSrvcInitCacheDisabled
  #define FinalDfltCpuSrvcInitCacheDisabled                           OvrdDfltCpuSrvcInitCacheDisabled
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcInitCacheDisabled) )
#else
  #define FinalDfltCpuSrvcInitCacheDisabled                           DfltCpuSrvcInitCacheDisabled
#endif

#ifdef OvrdDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
  #define FinalDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable       OvrdDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable) )
#else
  #define FinalDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable       DfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#endif

#ifdef OvrdDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
  #define FinalDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable        OvrdDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable) )
#else
  #define FinalDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable        DfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#endif

#ifdef OvrdDfltCpuSrvcIsSecureS3
  #define FinalDfltCpuSrvcIsSecureS3                                  OvrdDfltCpuSrvcIsSecureS3
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcIsSecureS3) )
#else
  #define FinalDfltCpuSrvcIsSecureS3                                  DfltCpuSrvcIsSecureS3
#endif

#ifdef OvrdDfltCpuSrvcPatchLoaderIsSharedByCU
  #define FinalDfltCpuSrvcPatchLoaderIsSharedByCU                     OvrdDfltCpuSrvcPatchLoaderIsSharedByCU
  #pragma message( STRING_OVERRIDE_MACRO_VALUE(DfltCpuSrvcPatchLoaderIsSharedByCU) )
#else
  #define FinalDfltCpuSrvcPatchLoaderIsSharedByCU                     DfltCpuSrvcPatchLoaderIsSharedByCU
#endif


//
// Make final service definition with considerations of below:
// 1. Common build config swich control
// 2. Override service definition
//
#pragma message( STRING_SEPARATOR1 )

// Member: (UINT16) Revision
#ifdef CpuSrvcRevision
  #define FinalCpuSrvcRevision                                        CpuSrvcRevision
#else
  #define FinalCpuSrvcRevision                                        FinalDfltCpuSrvcRevision
#endif

// Member: (PF_CPU_DISABLE_PSTATE) DisablePstate
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcDisablePstate
    #define FinalCpuSrvcDisablePstate                                 CpuSrvcDisablePstate
    extern F_CPU_DISABLE_PSTATE                                       FinalCpuSrvcDisablePstate;
  #else
    #define FinalCpuSrvcDisablePstate                                 FinalDfltCpuSrvcDisablePstate
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDisablePstate) )
  #endif
#else
  #define FinalCpuSrvcDisablePstate                                   FinalDfltAssertCpuSrvcDisablePstate
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcDisablePstate) )
#endif

// Member: (PF_CPU_TRANSITION_PSTATE) TransitionPstate
#if (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  #ifdef CpuSrvcTransitionPstate
    #define FinalCpuSrvcTransitionPstate                              CpuSrvcTransitionPstate
    extern F_CPU_TRANSITION_PSTATE                                    FinalCpuSrvcTransitionPstate;
  #else
    #define FinalCpuSrvcTransitionPstate                              FinalDfltCpuSrvcTransitionPstate
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcTransitionPstate) )
  #endif
#else
  #define FinalCpuSrvcTransitionPstate                                FinalDfltAssertCpuSrvcTransitionPstate
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcTransitionPstate) )
#endif

// Member: (PF_CPU_GET_IDD_MAX) GetProcIddMax
#if (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE)
  #ifdef CpuSrvcGetProcIddMax
    #define FinalCpuSrvcGetProcIddMax                                 CpuSrvcGetProcIddMax
    extern F_CPU_GET_IDD_MAX                                          FinalCpuSrvcGetProcIddMax;
  #else
    #define FinalCpuSrvcGetProcIddMax                                 FinalDfltCpuSrvcGetProcIddMax
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetProcIddMax) )
  #endif
#else
  #define FinalCpuSrvcGetProcIddMax                                   FinalDfltAssertCpuSrvcGetProcIddMax
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetProcIddMax) )
#endif

// Member: (PF_CPU_GET_TSC_RATE) GetTscRate
#if (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  #ifdef CpuSrvcGetTscRate
    #define FinalCpuSrvcGetTscRate                                    CpuSrvcGetTscRate
    extern F_CPU_GET_TSC_RATE                                         FinalCpuSrvcGetTscRate;
  #else
    #define FinalCpuSrvcGetTscRate                                    FinalDfltCpuSrvcGetTscRate
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetTscRate) )
  #endif
#else
  #define FinalCpuSrvcGetTscRate                                      FinalDfltAssertCpuSrvcGetTscRate
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetTscRate) )
#endif

// Member: (PF_CPU_GET_NB_FREQ) GetCurrentNbFrequency
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetCurrentNbFrequency
    #define FinalCpuSrvcGetCurrentNbFrequency                         CpuSrvcGetCurrentNbFrequency
    extern F_CPU_GET_NB_FREQ                                          FinalCpuSrvcGetCurrentNbFrequency;
  #else
    #define FinalCpuSrvcGetCurrentNbFrequency                         FinalDfltCpuSrvcGetCurrentNbFrequency
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetCurrentNbFrequency) )
  #endif
#else
  #define FinalCpuSrvcGetCurrentNbFrequency                           FinalDfltAssertCpuSrvcGetCurrentNbFrequency
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetCurrentNbFrequency) )
#endif

// Member: (PF_CPU_GET_MIN_MAX_NB_FREQ) GetMinMaxNbFrequency
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetMinMaxNbFrequency
    #define FinalCpuSrvcGetMinMaxNbFrequency                          CpuSrvcGetMinMaxNbFrequency
    extern F_CPU_GET_MIN_MAX_NB_FREQ                                  FinalCpuSrvcGetMinMaxNbFrequency;
  #else
    #define FinalCpuSrvcGetMinMaxNbFrequency                          FinalDfltCpuSrvcGetMinMaxNbFrequency
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetMinMaxNbFrequency) )
  #endif
#else
  #define FinalCpuSrvcGetMinMaxNbFrequency                            FinalDfltAssertCpuSrvcGetMinMaxNbFrequency
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetMinMaxNbFrequency) )
#endif

// Member: (PF_CPU_GET_NB_PSTATE_INFO) GetNbPstateInfo
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetNbPstateInfo
    #define FinalCpuSrvcGetNbPstateInfo                               CpuSrvcGetNbPstateInfo
    extern F_CPU_GET_NB_PSTATE_INFO                                   FinalCpuSrvcGetNbPstateInfo;
  #else
    #define FinalCpuSrvcGetNbPstateInfo                               FinalDfltCpuSrvcGetNbPstateInfo
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetNbPstateInfo) )
  #endif
#else
  #define FinalCpuSrvcGetNbPstateInfo                                 FinalDfltAssertCpuSrvcGetNbPstateInfo
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetNbPstateInfo) )
#endif

// Member: (PF_CPU_IS_NBCOF_INIT_NEEDED) IsNbCofInitNeeded
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcIsNbCofInitNeeded
    #define FinalCpuSrvcIsNbCofInitNeeded                             CpuSrvcIsNbCofInitNeeded
    extern F_CPU_IS_NBCOF_INIT_NEEDED                                 FinalCpuSrvcIsNbCofInitNeeded;
  #else
    #define FinalCpuSrvcIsNbCofInitNeeded                             FinalDfltCpuSrvcIsNbCofInitNeeded
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcIsNbCofInitNeeded) )
  #endif
#else
  #define FinalCpuSrvcIsNbCofInitNeeded                               FinalDfltAssertCpuSrvcIsNbCofInitNeeded
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcIsNbCofInitNeeded) )
#endif

// Member: (PF_CPU_GET_NB_IDD_MAX) GetNbIddMax
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetNbIddMax
    #define FinalCpuSrvcGetNbIddMax                                   CpuSrvcGetNbIddMax
    extern F_CPU_GET_NB_IDD_MAX                                       FinalCpuSrvcGetNbIddMax;
  #else
    #define FinalCpuSrvcGetNbIddMax                                   FinalDfltCpuSrvcGetNbIddMax
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetNbIddMax) )
  #endif
#else
  #define FinalCpuSrvcGetNbIddMax                                     FinalDfltAssertCpuSrvcGetNbIddMax
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetNbIddMax) )
#endif

// Member: (PF_CPU_AP_INITIAL_LAUNCH) LaunchApCore
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcLaunchApCore
    #define FinalCpuSrvcLaunchApCore                                  CpuSrvcLaunchApCore
    extern F_CPU_AP_INITIAL_LAUNCH                                    FinalCpuSrvcLaunchApCore;
  #else
    #define FinalCpuSrvcLaunchApCore                                  FinalDfltCpuSrvcLaunchApCore
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcLaunchApCore) )
  #endif
#else
  #define FinalCpuSrvcLaunchApCore                                    FinalDfltAssertCpuSrvcLaunchApCore
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcLaunchApCore) )
#endif

// Member: (PF_CPU_NUMBER_OF_PHYSICAL_CORES) GetNumberOfPhysicalCores
#if (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  #ifdef CpuSrvcGetNumberOfPhysicalCores
    #define FinalCpuSrvcGetNumberOfPhysicalCores                      CpuSrvcGetNumberOfPhysicalCores
    extern F_CPU_NUMBER_OF_PHYSICAL_CORES                             FinalCpuSrvcGetNumberOfPhysicalCores;
  #else
    #define FinalCpuSrvcGetNumberOfPhysicalCores                      FinalDfltCpuSrvcGetNumberOfPhysicalCores
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetNumberOfPhysicalCores) )
  #endif
#else
  #define FinalCpuSrvcGetNumberOfPhysicalCores                        FinalDfltAssertCpuSrvcGetNumberOfPhysicalCores
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetNumberOfPhysicalCores) )
#endif

// Member: (PF_CPU_GET_AP_CORE_NUMBER) GetApCoreNumber
#if (AGESA_ENTRY_INIT_RESET == TRUE) || (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || \
    (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  #ifdef CpuSrvcGetApCoreNumber
    #define FinalCpuSrvcGetApCoreNumber                               CpuSrvcGetApCoreNumber
    extern F_CPU_GET_AP_CORE_NUMBER                                   FinalCpuSrvcGetApCoreNumber;
  #else
    #define FinalCpuSrvcGetApCoreNumber                               FinalDfltCpuSrvcGetApCoreNumber
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetApCoreNumber) )
  #endif
#else
  #define FinalCpuSrvcGetApCoreNumber                                 FinalDfltAssertCpuSrvcGetApCoreNumber
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetApCoreNumber) )
#endif

// Member: (PF_CORE_ID_POSITION_IN_INITIAL_APIC_ID) CoreIdPositionInInitialApicId
#if (AGESA_ENTRY_INIT_RESET == TRUE) || (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || \
    (AGESA_ENTRY_INIT_POST == TRUE) || (AGESA_ENTRY_INIT_GENERAL_SERVICES == TRUE)
  #ifdef CpuSrvcCoreIdPositionInInitialApicId
    #define FinalCpuSrvcCoreIdPositionInInitialApicId                 CpuSrvcCoreIdPositionInInitialApicId
    extern F_CORE_ID_POSITION_IN_INITIAL_APIC_ID                      FinalCpuSrvcCoreIdPositionInInitialApicId;
  #else
    #define FinalCpuSrvcCoreIdPositionInInitialApicId                 FinalDfltCpuSrvcCoreIdPositionInInitialApicId
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcCoreIdPositionInInitialApicId) )
  #endif
#else
  #define FinalCpuSrvcCoreIdPositionInInitialApicId                   FinalDfltAssertCpuSrvcCoreIdPositionInInitialApicId
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcCoreIdPositionInInitialApicId) )
#endif

// Member: (PF_CPU_SET_WARM_RESET_FLAG) SetWarmResetFlag
#if (AGESA_ENTRY_INIT_RESET == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE)
  #ifdef CpuSrvcSetWarmResetFlag
    #define FinalCpuSrvcSetWarmResetFlag                              CpuSrvcSetWarmResetFlag
    extern F_CPU_SET_WARM_RESET_FLAG                                  FinalCpuSrvcSetWarmResetFlag;
  #else
    #define FinalCpuSrvcSetWarmResetFlag                              FinalDfltCpuSrvcSetWarmResetFlag
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcSetWarmResetFlag) )
  #endif
#else
  #define FinalCpuSrvcSetWarmResetFlag                                FinalDfltAssertCpuSrvcSetWarmResetFlag
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcSetWarmResetFlag) )
#endif

// Member: (PF_CPU_GET_WARM_RESET_FLAG) GetWarmResetFlag
#if (AGESA_ENTRY_INIT_RESET == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE)
  #ifdef CpuSrvcGetWarmResetFlag
    #define FinalCpuSrvcGetWarmResetFlag                              CpuSrvcGetWarmResetFlag
    extern F_CPU_GET_WARM_RESET_FLAG                                  FinalCpuSrvcGetWarmResetFlag;
  #else
    #define FinalCpuSrvcGetWarmResetFlag                              FinalDfltCpuSrvcGetWarmResetFlag
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetWarmResetFlag) )
  #endif
#else
  #define FinalCpuSrvcGetWarmResetFlag                                FinalDfltAssertCpuSrvcGetWarmResetFlag
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetWarmResetFlag) )
#endif

// Member: (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY) GetMicroCodePatchesStruct
#if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
  #ifdef CpuSrvcGetMicroCodePatchesStruct
    #define FinalCpuSrvcGetMicroCodePatchesStruct                     CpuSrvcGetMicroCodePatchesStruct
    extern F_CPU_GET_FAMILY_SPECIFIC_ARRAY                            FinalCpuSrvcGetMicroCodePatchesStruct;
  #else
    #define FinalCpuSrvcGetMicroCodePatchesStruct                     FinalDfltCpuSrvcGetMicroCodePatchesStruct
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetMicroCodePatchesStruct) )
  #endif
#else
  #define FinalCpuSrvcGetMicroCodePatchesStruct                       FinalDfltAssertCpuSrvcGetMicroCodePatchesStruct
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetMicroCodePatchesStruct) )
#endif

// Member: (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY) GetMicrocodeEquivalenceTable
#if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
  #ifdef CpuSrvcGetMicrocodeEquivalenceTable
    #define FinalCpuSrvcGetMicrocodeEquivalenceTable                  CpuSrvcGetMicrocodeEquivalenceTable
    extern F_CPU_GET_FAMILY_SPECIFIC_ARRAY                            FinalCpuSrvcGetMicrocodeEquivalenceTable;
  #else
    #define FinalCpuSrvcGetMicrocodeEquivalenceTable                  FinalDfltCpuSrvcGetMicrocodeEquivalenceTable
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetMicrocodeEquivalenceTable) )
  #endif
#else
  #define FinalCpuSrvcGetMicrocodeEquivalenceTable                    FinalDfltAssertCpuSrvcGetMicrocodeEquivalenceTable
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetMicrocodeEquivalenceTable) )
#endif

// Member: (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY) GetCacheInfo
#if (AGESA_ENTRY_INIT_RESET == TRUE) || (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
  #ifdef CpuSrvcGetCacheInfo
    #define FinalCpuSrvcGetCacheInfo                                  CpuSrvcGetCacheInfo
    extern F_CPU_GET_FAMILY_SPECIFIC_ARRAY                            FinalCpuSrvcGetCacheInfo;
  #else
    #define FinalCpuSrvcGetCacheInfo                                  FinalDfltCpuSrvcGetCacheInfo
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetCacheInfo) )
  #endif
#else
  #define FinalCpuSrvcGetCacheInfo                                    FinalDfltAssertCpuSrvcGetCacheInfo
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetCacheInfo) )
#endif

// Member: (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY) GetSysPmTableStruct
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetSysPmTableStruct
    #define FinalCpuSrvcGetSysPmTableStruct                           CpuSrvcGetSysPmTableStruct
    extern F_CPU_GET_FAMILY_SPECIFIC_ARRAY                            FinalCpuSrvcGetSysPmTableStruct;
  #else
    #define FinalCpuSrvcGetSysPmTableStruct                           FinalDfltCpuSrvcGetSysPmTableStruct
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetSysPmTableStruct) )
  #endif
#else
  #define FinalCpuSrvcGetSysPmTableStruct                             FinalDfltAssertCpuSrvcGetSysPmTableStruct
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetSysPmTableStruct) )
#endif

// Member: (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY) GetWheaInitData
#if AGESA_ENTRY_INIT_LATE == TRUE
  #ifdef CpuSrvcGetWheaInitData
    #define FinalCpuSrvcGetWheaInitData                               CpuSrvcGetWheaInitData
    extern F_CPU_GET_FAMILY_SPECIFIC_ARRAY                            FinalCpuSrvcGetWheaInitData;
  #else
    #define FinalCpuSrvcGetWheaInitData                               FinalDfltCpuSrvcGetWheaInitData
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetWheaInitData) )
  #endif
#else
  #define FinalCpuSrvcGetWheaInitData                                 FinalDfltAssertCpuSrvcGetWheaInitData
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetWheaInitData) )
#endif

// Member: (PF_CPU_GET_PLATFORM_TYPE_SPECIFIC_INFO) GetPlatformTypeSpecificInfo
#if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_LATE == TRUE)
  #ifdef CpuSrvcGetPlatformTypeSpecificInfo
    #define FinalCpuSrvcGetPlatformTypeSpecificInfo                   CpuSrvcGetPlatformTypeSpecificInfo
    extern F_CPU_GET_PLATFORM_TYPE_SPECIFIC_INFO                      FinalCpuSrvcGetPlatformTypeSpecificInfo;
  #else
    #define FinalCpuSrvcGetPlatformTypeSpecificInfo                   FinalDfltCpuSrvcGetPlatformTypeSpecificInfo
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetPlatformTypeSpecificInfo) )
  #endif
#else
  #define FinalCpuSrvcGetPlatformTypeSpecificInfo                     FinalDfltAssertCpuSrvcGetPlatformTypeSpecificInfo
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetPlatformTypeSpecificInfo) )
#endif

// Member: (PF_IS_NB_PSTATE_ENABLED) IsNbPstateEnabled
#if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE)
  #ifdef CpuSrvcIsNbPstateEnabled
    #define FinalCpuSrvcIsNbPstateEnabled                             CpuSrvcIsNbPstateEnabled
    extern F_IS_NB_PSTATE_ENABLED                                     FinalCpuSrvcIsNbPstateEnabled;
  #else
    #define FinalCpuSrvcIsNbPstateEnabled                             FinalDfltCpuSrvcIsNbPstateEnabled
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcIsNbPstateEnabled) )
  #endif
#else
  #define FinalCpuSrvcIsNbPstateEnabled                               FinalDfltAssertCpuSrvcIsNbPstateEnabled
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcIsNbPstateEnabled) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[MsrRegister]
#ifdef CpuSrvcDoEntryTypeMsrRegister
  #define FinalCpuSrvcDoEntryTypeMsrRegister                          CpuSrvcDoEntryTypeMsrRegister
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeMsrRegister;
#else
  #define FinalCpuSrvcDoEntryTypeMsrRegister                          FinalDfltCpuSrvcDoEntryTypeMsrRegister
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeMsrRegister) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[PciRegister]
#ifdef CpuSrvcDoEntryTypePciRegister
  #define FinalCpuSrvcDoEntryTypePciRegister                          CpuSrvcDoEntryTypePciRegister
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypePciRegister;
#else
  #define FinalCpuSrvcDoEntryTypePciRegister                          FinalDfltCpuSrvcDoEntryTypePciRegister
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypePciRegister) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[FamSpecificWorkaround]
#ifdef CpuSrvcDoEntryTypeFamSpecificWorkaround
  #define FinalCpuSrvcDoEntryTypeFamSpecificWorkaround                CpuSrvcDoEntryTypeFamSpecificWorkaround
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeFamSpecificWorkaround;
#else
  #define FinalCpuSrvcDoEntryTypeFamSpecificWorkaround                FinalDfltCpuSrvcDoEntryTypeFamSpecificWorkaround
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeFamSpecificWorkaround) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[ProfileFixup]
#ifdef CpuSrvcDoEntryTypeProfileFixup
  #define FinalCpuSrvcDoEntryTypeProfileFixup                         CpuSrvcDoEntryTypeProfileFixup
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeProfileFixup;
#else
  #define FinalCpuSrvcDoEntryTypeProfileFixup                         FinalDfltCpuSrvcDoEntryTypeProfileFixup
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeProfileFixup) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CoreCountsPciRegister]
#ifdef CpuSrvcDoEntryTypeCoreCountsPciRegister
  #define FinalCpuSrvcDoEntryTypeCoreCountsPciRegister                CpuSrvcDoEntryTypeCoreCountsPciRegister
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCoreCountsPciRegister;
#else
  #define FinalCpuSrvcDoEntryTypeCoreCountsPciRegister                FinalDfltCpuSrvcDoEntryTypeCoreCountsPciRegister
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCoreCountsPciRegister) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CompUnitCountsPciRegister]
#ifdef CpuSrvcDoEntryTypeCompUnitCountsPciRegister
  #define FinalCpuSrvcDoEntryTypeCompUnitCountsPciRegister            CpuSrvcDoEntryTypeCompUnitCountsPciRegister
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCompUnitCountsPciRegister;
#else
  #define FinalCpuSrvcDoEntryTypeCompUnitCountsPciRegister            FinalDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCompUnitCountsPciRegister) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CompUnitCountsMsr]
#ifdef CpuSrvcDoEntryTypeCompUnitCountsMsr
  #define FinalCpuSrvcDoEntryTypeCompUnitCountsMsr                    CpuSrvcDoEntryTypeCompUnitCountsMsr
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCompUnitCountsMsr;
#else
  #define FinalCpuSrvcDoEntryTypeCompUnitCountsMsr                    FinalDfltCpuSrvcDoEntryTypeCompUnitCountsMsr
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCompUnitCountsMsr) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CpuRevPciRegister]
#ifdef CpuSrvcDoEntryTypeCpuRevPciRegister
  #define FinalCpuSrvcDoEntryTypeCpuRevPciRegister                    CpuSrvcDoEntryTypeCpuRevPciRegister
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCpuRevPciRegister;
#else
  #define FinalCpuSrvcDoEntryTypeCpuRevPciRegister                    FinalDfltCpuSrvcDoEntryTypeCpuRevPciRegister
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCpuRevPciRegister) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CpuRevMsr]
#ifdef CpuSrvcDoEntryTypeCpuRevMsr
  #define FinalCpuSrvcDoEntryTypeCpuRevMsr                            CpuSrvcDoEntryTypeCpuRevMsr
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCpuRevMsr;
#else
  #define FinalCpuSrvcDoEntryTypeCpuRevMsr                            FinalDfltCpuSrvcDoEntryTypeCpuRevMsr
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCpuRevMsr) )
#endif


// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CpuRevMsr]
#ifdef CpuSrvcDoEntryTypeCpuRevMsr
  #define FinalCpuSrvcDoEntryTypeCpuRevMsr                            CpuSrvcDoEntryTypeCpuRevMsr
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCpuRevMsr;
#else
  #define FinalCpuSrvcDoEntryTypeCpuRevMsr                            FinalDfltCpuSrvcDoEntryTypeCpuRevMsr
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCpuRevMsr) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CpuRevFamSpecificWorkaround]
#ifdef CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  #define FinalCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround          CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround;
#else
  #define FinalCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround          FinalDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[SmuIndexReg]
#ifdef CpuSrvcDoEntryTypeSmuIndexReg
  #define FinalCpuSrvcDoEntryTypeSmuIndexReg                          CpuSrvcDoEntryTypeSmuIndexReg
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeSmuIndexReg;
#else
  #define FinalCpuSrvcDoEntryTypeSmuIndexReg                          FinalDfltCpuSrvcDoEntryTypeSmuIndexReg
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeSmuIndexReg) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[ProfileFixupSmuIndexReg]
#ifdef CpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  #define FinalCpuSrvcDoEntryTypeProfileFixupSmuIndexReg              CpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeProfileFixupSmuIndexReg;
#else
  #define FinalCpuSrvcDoEntryTypeProfileFixupSmuIndexReg              FinalDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeProfileFixupSmuIndexReg) )
#endif

// Member: (PF_DO_TABLE_ENTRY) DoTableEntry[CopyBitField]
#ifdef CpuSrvcDoEntryTypeCopyBitField
  #define FinalCpuSrvcDoEntryTypeCopyBitField                         CpuSrvcDoEntryTypeCopyBitField
  extern F_DO_TABLE_ENTRY                                             FinalCpuSrvcDoEntryTypeCopyBitField;
#else
  #define FinalCpuSrvcDoEntryTypeCopyBitField                         FinalDfltCpuSrvcDoEntryTypeCopyBitField
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcDoEntryTypeCopyBitField) )
#endif

// Member: (PF_GET_REGISTER_TABLE_LIST) GetRegisterTableList
#ifdef CpuSrvcGetRegisterTableList
  #define FinalCpuSrvcGetRegisterTableList                            CpuSrvcGetRegisterTableList
  extern F_GET_REGISTER_TABLE_LIST                                    FinalCpuSrvcGetRegisterTableList;
#else
  #define FinalCpuSrvcGetRegisterTableList                            FinalDfltCpuSrvcGetRegisterTableList
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetRegisterTableList) )
#endif

// Member: (PF_GET_WORKAROUND_TABLE) GetWorkaroundTable
#ifdef CpuSrvcGetWorkaroundTable
  #define FinalCpuSrvcGetWorkaroundTable                              CpuSrvcGetWorkaroundTable
  extern F_GET_WORKAROUND_TABLE                                       FinalCpuSrvcGetWorkaroundTable;
#else
  #define FinalCpuSrvcGetWorkaroundTable                              FinalDfltCpuSrvcGetWorkaroundTable
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetWorkaroundTable) )
#endif

// Member: (COMPUTE_UNIT_MAP *) ComputeUnitMap
#ifdef CpuSrvcComputeUnitMap
  #define FinalCpuSrvcComputeUnitMap                                  CpuSrvcComputeUnitMap
#else
  #define FinalCpuSrvcComputeUnitMap                                  FinalDfltCpuSrvcComputeUnitMap
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcComputeUnitMap) )
#endif

// Member: (FAMILY_CACHE_INIT_POLICY) InitCacheDisabled
#ifdef CpuSrvcInitCacheDisabled
  #define FinalCpuSrvcInitCacheDisabled                               CpuSrvcInitCacheDisabled
#else
  #define FinalCpuSrvcInitCacheDisabled                               FinalDfltCpuSrvcInitCacheDisabled
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcInitCacheDisabled) )
#endif

// Member: (PF_GET_EARLY_INIT_TABLE) GetEarlyInitBeforeApLaunchOnCoreTable
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
    #define FinalCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable         CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
    extern F_GET_EARLY_INIT_TABLE                                     FinalCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable;
  #else
    #define FinalCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable         FinalDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable) )
  #endif
#else
  #define FinalCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable           FinalDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable) )
#endif

// Member: (PF_GET_EARLY_INIT_TABLE) GetEarlyInitAfterApLaunchOnCoreTable
#if AGESA_ENTRY_INIT_EARLY == TRUE
  #ifdef CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
    #define FinalCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable          CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
    extern F_GET_EARLY_INIT_TABLE                                     FinalCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable;
  #else
    #define FinalCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable          FinalDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable) )
  #endif
#else
  #define FinalCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable            FinalDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable) )
#endif

// Member: (PF_IS_SECURE_S3) IsSecureS3
#ifdef CpuSrvcIsSecureS3
  #define FinalCpuSrvcIsSecureS3                                      CpuSrvcIsSecureS3
  extern F_IS_SECURE_S3                                               FinalCpuSrvcIsSecureS3;
#else
  #define FinalCpuSrvcIsSecureS3                                      FinalDfltCpuSrvcIsSecureS3
  #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcIsSecureS3) )
#endif

// Member: (BOOLEAN) PatchLoaderIsSharedByCU
#if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE)
  #ifdef CpuSrvcPatchLoaderIsSharedByCU
    #define FinalCpuSrvcPatchLoaderIsSharedByCU                       CpuSrvcPatchLoaderIsSharedByCU
  #else
    #define FinalCpuSrvcPatchLoaderIsSharedByCU                       FinalDfltCpuSrvcPatchLoaderIsSharedByCU
    #pragma message( STRING_MACRO_DEFAULT_USED(CpuSrvcPatchLoaderIsSharedByCU) )
  #endif
#else
  #define FinalCpuSrvcPatchLoaderIsSharedByCU                         FinalDfltAssertCpuSrvcPatchLoaderIsSharedByCU
  #pragma message( STRING_MACRO_DEFAULT_ASSERT_USED(CpuSrvcPatchLoaderIsSharedByCU) )
#endif


//
// NOTE: All CPU family specific service members should be all defined now.
//

//
// Define CPU specific services installation macro
//
#ifndef _INSTALL_CPU_SPECIFIC_SERVICES_TABLE_NAME_
  #define _INSTALL_CPU_SPECIFIC_SERVICES_TABLE_NAME_

  #define INSTALL_CPU_SPECIFIC_SERVICES_TABLE_NAME(MacroPrefix, TableName) \
  CONST CPU_SPECIFIC_SERVICES ROMDATA TableName =                \
  {                                                              \
    MacroPrefix##CpuSrvcRevision,                                \
    MacroPrefix##CpuSrvcDisablePstate,                           \
    MacroPrefix##CpuSrvcTransitionPstate,                        \
    MacroPrefix##CpuSrvcGetProcIddMax,                           \
    MacroPrefix##CpuSrvcGetTscRate,                              \
    MacroPrefix##CpuSrvcGetCurrentNbFrequency,                   \
    MacroPrefix##CpuSrvcGetMinMaxNbFrequency,                    \
    MacroPrefix##CpuSrvcGetNbPstateInfo,                         \
    MacroPrefix##CpuSrvcIsNbCofInitNeeded,                       \
    MacroPrefix##CpuSrvcGetNbIddMax,                             \
    MacroPrefix##CpuSrvcLaunchApCore,                            \
    MacroPrefix##CpuSrvcGetNumberOfPhysicalCores,                \
    MacroPrefix##CpuSrvcGetApCoreNumber,                         \
    MacroPrefix##CpuSrvcCoreIdPositionInInitialApicId,           \
    MacroPrefix##CpuSrvcSetWarmResetFlag,                        \
    MacroPrefix##CpuSrvcGetWarmResetFlag,                        \
    MacroPrefix##CpuSrvcGetMicroCodePatchesStruct,               \
    MacroPrefix##CpuSrvcGetMicrocodeEquivalenceTable,            \
    MacroPrefix##CpuSrvcGetCacheInfo,                            \
    MacroPrefix##CpuSrvcGetSysPmTableStruct,                     \
    MacroPrefix##CpuSrvcGetWheaInitData,                         \
    MacroPrefix##CpuSrvcGetPlatformTypeSpecificInfo,             \
    MacroPrefix##CpuSrvcIsNbPstateEnabled,                       \
    {                                                            \
      MacroPrefix##CpuSrvcDoEntryTypeMsrRegister,                \
      MacroPrefix##CpuSrvcDoEntryTypePciRegister,                \
      MacroPrefix##CpuSrvcDoEntryTypeFamSpecificWorkaround,      \
      MacroPrefix##CpuSrvcDoEntryTypeProfileFixup,               \
      MacroPrefix##CpuSrvcDoEntryTypeCoreCountsPciRegister,      \
      MacroPrefix##CpuSrvcDoEntryTypeCompUnitCountsPciRegister,  \
      MacroPrefix##CpuSrvcDoEntryTypeCompUnitCountsMsr,          \
      MacroPrefix##CpuSrvcDoEntryTypeCpuRevPciRegister,          \
      MacroPrefix##CpuSrvcDoEntryTypeCpuRevMsr,                  \
      MacroPrefix##CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround, \
      MacroPrefix##CpuSrvcDoEntryTypeSmuIndexReg,                \
      MacroPrefix##CpuSrvcDoEntryTypeProfileFixupSmuIndexReg,    \
      MacroPrefix##CpuSrvcDoEntryTypeCopyBitField                \
    },                                                           \
    MacroPrefix##CpuSrvcGetRegisterTableList,                    \
    MacroPrefix##CpuSrvcGetWorkaroundTable,                      \
    MacroPrefix##CpuSrvcComputeUnitMap,                          \
    MacroPrefix##CpuSrvcInitCacheDisabled,                       \
    MacroPrefix##CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable,   \
    MacroPrefix##CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable,    \
    MacroPrefix##CpuSrvcIsSecureS3,                              \
    MacroPrefix##CpuSrvcPatchLoaderIsSharedByCU                  \
  }
#endif // _INSTALL_CPU_SPECIFIC_SERVICES_TABLE_NAME_

#ifndef _INSTALL_CPU_SPECIFIC_SERVICES_TABLE_
  #define _INSTALL_CPU_SPECIFIC_SERVICES_TABLE_

  #define INSTALL_CPU_SPECIFIC_SERVICES_TABLE(TableName) \
            INSTALL_CPU_SPECIFIC_SERVICES_TABLE_NAME (Final, TableName)
#endif // _INSTALL_CPU_SPECIFIC_SERVICES_TABLE_

//
// Message out the final table definitions
//
#pragma message( STRING_SEPARATOR2 )
#pragma message( STRING_MACRO_VALUE(CpuSrvcTableName) )
#pragma message( STRING_SEPARATOR1 )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcRevision) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDisablePstate) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcTransitionPstate) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetProcIddMax) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetTscRate) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetCurrentNbFrequency) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetMinMaxNbFrequency) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetNbPstateInfo) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcIsNbCofInitNeeded) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetNbIddMax) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcLaunchApCore) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetNumberOfPhysicalCores) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetApCoreNumber) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcCoreIdPositionInInitialApicId) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcSetWarmResetFlag) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetWarmResetFlag) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetMicroCodePatchesStruct) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetMicrocodeEquivalenceTable) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetCacheInfo) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetSysPmTableStruct) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetWheaInitData) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetPlatformTypeSpecificInfo) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcIsNbPstateEnabled) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeMsrRegister) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypePciRegister) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeFamSpecificWorkaround) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeProfileFixup) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCoreCountsPciRegister) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCompUnitCountsPciRegister) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCompUnitCountsMsr) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCpuRevPciRegister) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCpuRevMsr) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeSmuIndexReg) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeProfileFixupSmuIndexReg) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcDoEntryTypeCopyBitField) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetRegisterTableList) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetWorkaroundTable) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcComputeUnitMap) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcInitCacheDisabled) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcIsSecureS3) )
#pragma message( STRING_MACRO_VALUE(FinalCpuSrvcPatchLoaderIsSharedByCU) )
#pragma message( STRING_SEPARATOR_POUND )
