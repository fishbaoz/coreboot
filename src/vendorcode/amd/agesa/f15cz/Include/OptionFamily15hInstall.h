/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of family 15h support
 *
 * This file generates the defaults tables for family 15h processors.
 *
 * @xrefitem bom "File Content Label" "Release Content"
 * @e project:      AGESA
 * @e sub-project:  Core
 * @e \$Revision: 313537 $   @e \$Date: 2015-02-24 10:57:13 -0600 (Tue, 24 Feb 2015) $
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
 *
 ***************************************************************************/

#ifndef _OPTION_FAMILY_15H_INSTALL_H_
#define _OPTION_FAMILY_15H_INSTALL_H_

#include "OptionFamily15hEarlySample.h"
#include "cpuFamilyTranslation.h"

/*
 * Pull in family specific services based on entry point
 */

/*
 * Common Family 15h routines
 */

/*
 * Install family 15h model 60h - 6Fh support
 */
#ifdef OPTION_FAMILY15H_CZ
  #if OPTION_FAMILY15H_CZ == TRUE
    extern CONST REGISTER_TABLE ROMDATA F15CzPrimaryCoreRegTableBeforeApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15CzPrimaryCoreRegTableAfterApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15CzAllCoreRegTableAfterApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15CzCURegTableBeforeApLaunch;
    extern CONST REGISTER_TABLE ROMDATA F15CzCURegTableAfterApLaunch;

    /**
     * Compute unit and Compute unit primary determination table.
     *
     * The two fields from the compute unit status hardware register can be used to determine whether
     * even number cores are primary or all cores are primary.  It can be extended if it is
     * decided to have other configs as well.  The other logically possible value sets are BitMapMapping,
     * but they are currently not supported by the processor.
     */
    CONST COMPUTE_UNIT_MAP ROMDATA TopologyFam15CzComputeUnitMapping[] =
    {
      {1, 1, 'x', 'x', EvenCoresMapping},                                     ///< 1 Compute Unit with 2 cores
      {3, 3, 'x', 'x', EvenCoresMapping},                                     ///< 2 Compute Units both with 2 Cores
      {1, 0, 'x', 'x', AllCoresMapping},                                      ///< 1 Compute Unit with 1 core
      {3, 0, 'x', 'x', AllCoresMapping},                                      ///< 2 Compute Units both with 1 Core
      {TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, TOPOLOGY_LIST_TERMINAL, MaxComputeUnitMapping}   ///< End
    };


    #if USES_REGISTER_TABLES == TRUE
      CONST REGISTER_TABLE ROMDATA *F15CzRegisterTablesAfterApLaunch[] =
      {
        &F15CzAllCoreRegTableAfterApLaunch,
        &F15CzCURegTableAfterApLaunch,
        &F15CzPrimaryCoreRegTableAfterApLaunch,
        // the end.
        NULL
      };

      CONST REGISTER_TABLE ROMDATA *F15CzRegisterTablesBeforeApLaunch[] =
      {
        &F15CzCURegTableBeforeApLaunch,
        &F15CzPrimaryCoreRegTableBeforeApLaunch,
        // the end.
        NULL
      };

      CONST REGISTER_TABLE ROMDATA *F15CzRegisterTablesAfterApLaunchSecureS3[] =
      {
        &F15CzPrimaryCoreRegTableAfterApLaunch,
        // the end.
        NULL
      };

      CONST REGISTER_TABLE ROMDATA *F15CzRegisterTablesBeforeApLaunchSecureS3[] =
      {
        &F15CzPrimaryCoreRegTableBeforeApLaunch,
        // the end.
        NULL
      };
    #endif

    CONST REGISTER_TABLE_AT_GIVEN_TP ROMDATA F15CzRegTableListAtGivenTP[] =
    {
      #if USES_REGISTER_TABLES == TRUE
        {AmdRegisterTableTpBeforeApLaunch, F15CzRegisterTablesBeforeApLaunch},
        {AmdRegisterTableTpAfterApLaunch, F15CzRegisterTablesAfterApLaunch},
        {AmdRegisterTableTpBeforeApLaunchSecureS3, F15CzRegisterTablesBeforeApLaunchSecureS3},
        {AmdRegisterTableTpAfterApLaunchSecureS3, F15CzRegisterTablesAfterApLaunchSecureS3},
      #endif
      // the end
      {MaxAmdRegisterTableTps, NULL}
    };

    #include "OptionCpuSpecificServicesInstallReset.h"
    #define CpuSrvcTableName                            cpuF15CzServices

    #define CpuSrvcDisablePstate                        F15DisablePstate
    #define CpuSrvcTransitionPstate                     F15TransitionPstate
    #define CpuSrvcGetProcIddMax                        F15CzGetProcIddMax
    #define CpuSrvcGetTscRate                           F15GetTscRate
    #define CpuSrvcGetCurrentNbFrequency                F15CzGetCurrentNbFrequency
    #define CpuSrvcGetMinMaxNbFrequency                 F15CzGetMinMaxNbFrequency
    #define CpuSrvcGetNbPstateInfo                      F15CzGetNbPstateInfo
    #define CpuSrvcIsNbCofInitNeeded                    F15CommonGetNbCofVidUpdate
    #define CpuSrvcGetNbIddMax                          F15CzGetNbIddMax
    #define CpuSrvcLaunchApCore                         F15LaunchApCore
    #define CpuSrvcGetNumberOfPhysicalCores             F15CzGetNumberOfPhysicalCores
    #define CpuSrvcGetApCoreNumber                      F15CzGetApCoreNumber
    #define CpuSrvcCoreIdPositionInInitialApicId        F15CpuAmdCoreIdPositionInInitialApicId
    #define CpuSrvcSetWarmResetFlag                     F15SetAgesaWarmResetFlag
    #define CpuSrvcGetWarmResetFlag                     F15GetAgesaWarmResetFlag
    #define CpuSrvcGetMicroCodePatchesStruct            GetF15CzMicroCodePatchesStruct
    #define CpuSrvcGetMicrocodeEquivalenceTable         GetF15CzMicrocodeEquivalenceTable
    #define CpuSrvcGetCacheInfo                         GetF15CacheInfo
    #define CpuSrvcGetSysPmTableStruct                  GetF15CzSysPmTable
    #define CpuSrvcGetWheaInitData                      GetF15WheaInitData
    #define CpuSrvcIsNbPstateEnabled                    F15CzIsNbPstateEnabled
    #define CpuSrvcIsSecureS3                           F15CzIsSecureS3
    #define CpuSrvcDoEntryTypeMsrRegister               SetRegisterForMsrEntry
    #define CpuSrvcDoEntryTypePciRegister               SetRegisterForPciEntry
    #define CpuSrvcDoEntryTypeCpuRevPciRegister         SetRegisterForCpuRevPciEntry
    #define CpuSrvcDoEntryTypeCpuRevMsr                 SetRegisterForCpuRevMsrEntry
    #define CpuSrvcDoEntryTypeFamSpecificWorkaround     SetRegisterForFamSpecificWorkaroundEntry
    #define CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround SetRegisterForCpuRevFamSpecificWorkaroundEntry
    #define CpuSrvcDoEntryTypeProfileFixup              SetRegisterForPerformanceProfileEntry
    #define CpuSrvcDoEntryTypeCoreCountsPciRegister     SetRegisterForCoreCountsPerformanceEntry
    #define CpuSrvcDoEntryTypeCompUnitCountsPciRegister SetRegisterForComputeUnitCountsEntry
    #define CpuSrvcDoEntryTypeCompUnitCountsMsr         SetMsrForComputeUnitCountsEntry
    #define CpuSrvcDoEntryTypeSmuIndexReg               SetSmuIndexRegisterEntry
    #define CpuSrvcDoEntryTypeProfileFixupSmuIndexReg   SetSmuIndexRegisterForPerformanceEntry
    #define CpuSrvcDoEntryTypeCopyBitField              CopyBitFieldEntry
    #define CpuSrvcGetRegisterTableList                 F15CzGetRegisterTableList
    #define CpuSrvcGetWorkaroundTable                   F15CzGetWorkaroundTable
    #define CpuSrvcComputeUnitMap                       (COMPUTE_UNIT_MAP *) &TopologyFam15CzComputeUnitMapping
    #define CpuSrvcInitCacheDisabled                    InitCacheEnabled
    #define CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable GetF15CzEarlyInitBeforeApLaunchOnCoreTable
    #define CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable GetF15CzEarlyInitAfterApLaunchOnCoreTable
    #define CpuSrvcPatchLoaderIsSharedByCU              TRUE

    #include "OptionCpuSpecificServicesInstall.h"
    INSTALL_CPU_SPECIFIC_SERVICES_TABLE (CpuSrvcTableName);

    #define CZ_SOCKETS 1
    #define CZ_MODULES 1
    #define CZ_RECOVERY_SOCKETS 1
    #define CZ_RECOVERY_MODULES 1
    extern F_CPU_GET_SUBFAMILY_ID_ARRAY GetF15CzLogicalIdAndRev;
    #define OPT_F15_CZ_ID (PF_CPU_GET_SUBFAMILY_ID_ARRAY) GetF15CzLogicalIdAndRev,
    #ifndef ADVCFG_PLATFORM_SOCKETS
      #define ADVCFG_PLATFORM_SOCKETS CZ_SOCKETS
    #else
      #if ADVCFG_PLATFORM_SOCKETS < CZ_SOCKETS
        #undef ADVCFG_PLATFORM_SOCKETS
        #define ADVCFG_PLATFORM_SOCKETS CZ_SOCKETS
      #endif
    #endif
    #ifndef ADVCFG_PLATFORM_MODULES
      #define ADVCFG_PLATFORM_MODULES CZ_MODULES
    #else
      #if ADVCFG_PLATFORM_MODULES < CZ_MODULES
        #undef ADVCFG_PLATFORM_MODULES
        #define ADVCFG_PLATFORM_MODULES CZ_MODULES
      #endif
    #endif

    #if (AGESA_ENTRY_INIT_RECOVERY == TRUE) || (AGESA_ENTRY_INIT_EARLY == TRUE) || (AGESA_ENTRY_INIT_POST == TRUE)
      #define F15_CZ_UCODE_6601
      #define F15_CZ_UCODE_6600

      #if AGESA_ENTRY_INIT_EARLY == TRUE
        #if OPTION_EARLY_SAMPLES == TRUE
        #endif
        extern  CONST UINT8 ROMDATA  CpuF15CzId6601MicrocodePatch[];
        #undef F15_CZ_UCODE_6601
        #define F15_CZ_UCODE_6601 CpuF15CzId6601MicrocodePatch,

        extern  CONST UINT8 ROMDATA  CpuF15CzId6600MicrocodePatch[];
        #undef F15_CZ_UCODE_6600
        #define F15_CZ_UCODE_6600 CpuF15CzId6600MicrocodePatch,
      #endif

      CONST UINT8 ROMDATA *CpuF15CzMicroCodePatchArray[] =
      {
        F15_CZ_UCODE_6601
        F15_CZ_UCODE_6600
        NULL
      };

      CONST UINT8 ROMDATA CpuF15CzNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF15CzMicroCodePatchArray) / sizeof (CpuF15CzMicroCodePatchArray[0])) - 1);
    #endif

    #if OPTION_EARLY_SAMPLES == TRUE
    #endif
    CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F15CzLogicalIdTable[] =
    {
      OPT_F15_CZ_ID
    };

    #define OPT_F15_CZ_ID_TABLE {0x00660F00ul, {AMD_FAMILY_15_CZ, AMD_F15_UNKNOWN}, F15CzLogicalIdTable, (sizeof (F15CzLogicalIdTable) / sizeof (F15CzLogicalIdTable[0]))},
    #define OPT_F15_CZ_CPU {AMD_FAMILY_15_CZ, &cpuF15CzServices},

  #else  //  OPTION_FAMILY15H_CZ == TRUE
    #define OPT_F15_CZ_CPU
    #define OPT_F15_CZ_ID
    #define OPT_F15_CZ_ID_TABLE
    #if defined (BLDOPT_REMOVE_FAMILY_15_MODEL_6x_SUPPORT) && (BLDOPT_REMOVE_FAMILY_15_MODEL_6x_SUPPORT)
      CONST UINT8 ROMDATA *CpuF15CzMicroCodePatchArray[] =
      {
        NULL
      };
      CONST UINT8 ROMDATA CpuF15CzNumberOfMicrocodePatches = (UINT8) ((sizeof (CpuF15CzMicroCodePatchArray) / sizeof (CpuF15CzMicroCodePatchArray[0])) - 1);
    #endif
  #endif  //  OPTION_FAMILY15H_CZ == TRUE
#else  //  defined (OPTION_FAMILY15H_CZ)
  #define OPT_F15_CZ_CPU
  #define OPT_F15_CZ_ID
  #define OPT_F15_CZ_ID_TABLE
#endif  //  defined (OPTION_FAMILY15H_CZ)

// Family 15h maximum base address is 48 bits. Limit BLDCFG to 48 bits, if appropriate.
#if (FAMILY_MMIO_BASE_MASK < 0xFFFF000000000000ull)
  #undef  FAMILY_MMIO_BASE_MASK
  #define FAMILY_MMIO_BASE_MASK (0xFFFF000000000000ull)
#endif


#undef OPT_F15_ID_TABLE
#define OPT_F15_ID_TABLE  OPT_F15_CZ_ID_TABLE

#undef OPT_F15_TABLE
#define OPT_F15_TABLE     OPT_F15_CZ_CPU


CONST PF_CPU_GET_SUBFAMILY_ID_ARRAY ROMDATA F15LogicalIdTable[] =
{
  OPT_F15_CZ_ID
};

#endif  // _OPTION_FAMILY_15H_INSTALL_H_
