/* $NoKeywords:$ */
/**
 * @file
 *
 * Install of CPU specific services support
 *
 * This file resets and generates default services of CPU specific services.
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

//
// Undefine service table name
//
#undef CpuSrvcTableName

//
// Definitions types of a service to undefine
//
// * CpuSrvc<ServiceName>
// * DfltCpuSrvc<ServiceName>
// * DfltAssertCpuSrvc<ServiceName>
// * OvrdDfltCpuSrvc<ServiceName>
// * OvrdDfltAssertCpuSrvc<ServiceName>
// * FinalDfltCpuSrvc<ServiceName>
// * FinalDfltAssertCpuSrvc<ServiceName>
// * FinalCpuSrvc<ServiceName>
//

//
// Reset default services definitions
//
#undef CpuSrvcRevision
#undef CpuSrvcDisablePstate
#undef CpuSrvcTransitionPstate
#undef CpuSrvcGetProcIddMax
#undef CpuSrvcGetTscRate
#undef CpuSrvcGetCurrentNbFrequency
#undef CpuSrvcGetMinMaxNbFrequency
#undef CpuSrvcGetNbPstateInfo
#undef CpuSrvcIsNbCofInitNeeded
#undef CpuSrvcGetNbIddMax
#undef CpuSrvcLaunchApCore
#undef CpuSrvcGetNumberOfPhysicalCores
#undef CpuSrvcGetApCoreNumber
#undef CpuSrvcCoreIdPositionInInitialApicId
#undef CpuSrvcSetWarmResetFlag
#undef CpuSrvcGetWarmResetFlag
#undef CpuSrvcGetMicroCodePatchesStruct
#undef CpuSrvcGetMicrocodeEquivalenceTable
#undef CpuSrvcGetCacheInfo
#undef CpuSrvcGetSysPmTableStruct
#undef CpuSrvcGetWheaInitData
#undef CpuSrvcGetPlatformTypeSpecificInfo
#undef CpuSrvcIsNbPstateEnabled
#undef CpuSrvcDoEntryTypeMsrRegister
#undef CpuSrvcDoEntryTypePciRegister
#undef CpuSrvcDoEntryTypeFamSpecificWorkaround
#undef CpuSrvcDoEntryTypeProfileFixup
#undef CpuSrvcDoEntryTypeCoreCountsPciRegister
#undef CpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef CpuSrvcDoEntryTypeCompUnitCountsMsr
#undef CpuSrvcDoEntryTypeCpuRevPciRegister
#undef CpuSrvcDoEntryTypeCpuRevMsr
#undef CpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef CpuSrvcDoEntryTypeSmuIndexReg
#undef CpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef CpuSrvcDoEntryTypeCopyBitField
#undef CpuSrvcGetRegisterTableList
#undef CpuSrvcGetWorkaroundTable
#undef CpuSrvcComputeUnitMap
#undef CpuSrvcInitCacheDisabled
#undef CpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef CpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef CpuSrvcIsSecureS3
#undef CpuSrvcPatchLoaderIsSharedByCU


#undef DfltCpuSrvcRevision
#undef DfltCpuSrvcDisablePstate
#undef DfltCpuSrvcTransitionPstate
#undef DfltCpuSrvcGetProcIddMax
#undef DfltCpuSrvcGetTscRate
#undef DfltCpuSrvcGetCurrentNbFrequency
#undef DfltCpuSrvcGetMinMaxNbFrequency
#undef DfltCpuSrvcGetNbPstateInfo
#undef DfltCpuSrvcIsNbCofInitNeeded
#undef DfltCpuSrvcGetNbIddMax
#undef DfltCpuSrvcLaunchApCore
#undef DfltCpuSrvcGetNumberOfPhysicalCores
#undef DfltCpuSrvcGetApCoreNumber
#undef DfltCpuSrvcCoreIdPositionInInitialApicId
#undef DfltCpuSrvcSetWarmResetFlag
#undef DfltCpuSrvcGetWarmResetFlag
#undef DfltCpuSrvcGetMicroCodePatchesStruct
#undef DfltCpuSrvcGetMicrocodeEquivalenceTable
#undef DfltCpuSrvcGetCacheInfo
#undef DfltCpuSrvcGetSysPmTableStruct
#undef DfltCpuSrvcGetWheaInitData
#undef DfltCpuSrvcGetPlatformTypeSpecificInfo
#undef DfltCpuSrvcIsNbPstateEnabled
#undef DfltCpuSrvcDoEntryTypeMsrRegister
#undef DfltCpuSrvcDoEntryTypePciRegister
#undef DfltCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef DfltCpuSrvcDoEntryTypeProfileFixup
#undef DfltCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef DfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef DfltCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef DfltCpuSrvcDoEntryTypeCpuRevPciRegister
#undef DfltCpuSrvcDoEntryTypeCpuRevMsr
#undef DfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef DfltCpuSrvcDoEntryTypeSmuIndexReg
#undef DfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef DfltCpuSrvcDoEntryTypeCopyBitField
#undef DfltCpuSrvcGetRegisterTableList
#undef DfltCpuSrvcGetWorkaroundTable
#undef DfltCpuSrvcComputeUnitMap
#undef DfltCpuSrvcInitCacheDisabled
#undef DfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef DfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef DfltCpuSrvcIsSecureS3
#undef DfltCpuSrvcPatchLoaderIsSharedByCU


#undef DfltAssertCpuSrvcRevision
#undef DfltAssertCpuSrvcDisablePstate
#undef DfltAssertCpuSrvcTransitionPstate
#undef DfltAssertCpuSrvcGetProcIddMax
#undef DfltAssertCpuSrvcGetTscRate
#undef DfltAssertCpuSrvcGetCurrentNbFrequency
#undef DfltAssertCpuSrvcGetMinMaxNbFrequency
#undef DfltAssertCpuSrvcGetNbPstateInfo
#undef DfltAssertCpuSrvcIsNbCofInitNeeded
#undef DfltAssertCpuSrvcGetNbIddMax
#undef DfltAssertCpuSrvcLaunchApCore
#undef DfltAssertCpuSrvcGetNumberOfPhysicalCores
#undef DfltAssertCpuSrvcGetApCoreNumber
#undef DfltAssertCpuSrvcCoreIdPositionInInitialApicId
#undef DfltAssertCpuSrvcSetWarmResetFlag
#undef DfltAssertCpuSrvcGetWarmResetFlag
#undef DfltAssertCpuSrvcGetMicroCodePatchesStruct
#undef DfltAssertCpuSrvcGetMicrocodeEquivalenceTable
#undef DfltAssertCpuSrvcGetCacheInfo
#undef DfltAssertCpuSrvcGetSysPmTableStruct
#undef DfltAssertCpuSrvcGetWheaInitData
#undef DfltAssertCpuSrvcGetPlatformTypeSpecificInfo
#undef DfltAssertCpuSrvcIsNbPstateEnabled
#undef DfltAssertCpuSrvcDoEntryTypeMsrRegister
#undef DfltAssertCpuSrvcDoEntryTypePciRegister
#undef DfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef DfltAssertCpuSrvcDoEntryTypeProfileFixup
#undef DfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef DfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef DfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef DfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
#undef DfltAssertCpuSrvcDoEntryTypeCpuRevMsr
#undef DfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef DfltAssertCpuSrvcDoEntryTypeSmuIndexReg
#undef DfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef DfltAssertCpuSrvcDoEntryTypeCopyBitField
#undef DfltAssertCpuSrvcGetRegisterTableList
#undef DfltAssertCpuSrvcGetWorkaroundTable
#undef DfltAssertCpuSrvcComputeUnitMap
#undef DfltAssertCpuSrvcInitCacheDisabled
#undef DfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef DfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef DfltAssertCpuSrvcIsSecureS3
#undef DfltAssertCpuSrvcPatchLoaderIsSharedByCU


#undef OvrdDfltCpuSrvcRevision
#undef OvrdDfltCpuSrvcDisablePstate
#undef OvrdDfltCpuSrvcTransitionPstate
#undef OvrdDfltCpuSrvcGetProcIddMax
#undef OvrdDfltCpuSrvcGetTscRate
#undef OvrdDfltCpuSrvcGetCurrentNbFrequency
#undef OvrdDfltCpuSrvcGetMinMaxNbFrequency
#undef OvrdDfltCpuSrvcGetNbPstateInfo
#undef OvrdDfltCpuSrvcIsNbCofInitNeeded
#undef OvrdDfltCpuSrvcGetNbIddMax
#undef OvrdDfltCpuSrvcLaunchApCore
#undef OvrdDfltCpuSrvcGetNumberOfPhysicalCores
#undef OvrdDfltCpuSrvcGetApCoreNumber
#undef OvrdDfltCpuSrvcCoreIdPositionInInitialApicId
#undef OvrdDfltCpuSrvcSetWarmResetFlag
#undef OvrdDfltCpuSrvcGetWarmResetFlag
#undef OvrdDfltCpuSrvcGetMicroCodePatchesStruct
#undef OvrdDfltCpuSrvcGetMicrocodeEquivalenceTable
#undef OvrdDfltCpuSrvcGetCacheInfo
#undef OvrdDfltCpuSrvcGetSysPmTableStruct
#undef OvrdDfltCpuSrvcGetWheaInitData
#undef OvrdDfltCpuSrvcGetPlatformTypeSpecificInfo
#undef OvrdDfltCpuSrvcIsNbPstateEnabled
#undef OvrdDfltCpuSrvcDoEntryTypeMsrRegister
#undef OvrdDfltCpuSrvcDoEntryTypePciRegister
#undef OvrdDfltCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef OvrdDfltCpuSrvcDoEntryTypeProfileFixup
#undef OvrdDfltCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef OvrdDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef OvrdDfltCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef OvrdDfltCpuSrvcDoEntryTypeCpuRevPciRegister
#undef OvrdDfltCpuSrvcDoEntryTypeCpuRevMsr
#undef OvrdDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef OvrdDfltCpuSrvcDoEntryTypeSmuIndexReg
#undef OvrdDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef OvrdDfltCpuSrvcDoEntryTypeCopyBitField
#undef OvrdDfltCpuSrvcGetRegisterTableList
#undef OvrdDfltCpuSrvcGetWorkaroundTable
#undef OvrdDfltCpuSrvcComputeUnitMap
#undef OvrdDfltCpuSrvcInitCacheDisabled
#undef OvrdDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef OvrdDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef OvrdDfltCpuSrvcIsSecureS3
#undef OvrdDfltCpuSrvcPatchLoaderIsSharedByCU


#undef OvrdDfltAssertCpuSrvcRevision
#undef OvrdDfltAssertCpuSrvcDisablePstate
#undef OvrdDfltAssertCpuSrvcTransitionPstate
#undef OvrdDfltAssertCpuSrvcGetProcIddMax
#undef OvrdDfltAssertCpuSrvcGetTscRate
#undef OvrdDfltAssertCpuSrvcGetCurrentNbFrequency
#undef OvrdDfltAssertCpuSrvcGetMinMaxNbFrequency
#undef OvrdDfltAssertCpuSrvcGetNbPstateInfo
#undef OvrdDfltAssertCpuSrvcIsNbCofInitNeeded
#undef OvrdDfltAssertCpuSrvcGetNbIddMax
#undef OvrdDfltAssertCpuSrvcLaunchApCore
#undef OvrdDfltAssertCpuSrvcGetNumberOfPhysicalCores
#undef OvrdDfltAssertCpuSrvcGetApCoreNumber
#undef OvrdDfltAssertCpuSrvcCoreIdPositionInInitialApicId
#undef OvrdDfltAssertCpuSrvcSetWarmResetFlag
#undef OvrdDfltAssertCpuSrvcGetWarmResetFlag
#undef OvrdDfltAssertCpuSrvcGetMicroCodePatchesStruct
#undef OvrdDfltAssertCpuSrvcGetMicrocodeEquivalenceTable
#undef OvrdDfltAssertCpuSrvcGetCacheInfo
#undef OvrdDfltAssertCpuSrvcGetSysPmTableStruct
#undef OvrdDfltAssertCpuSrvcGetWheaInitData
#undef OvrdDfltAssertCpuSrvcGetPlatformTypeSpecificInfo
#undef OvrdDfltAssertCpuSrvcIsNbPstateEnabled
#undef OvrdDfltAssertCpuSrvcDoEntryTypeMsrRegister
#undef OvrdDfltAssertCpuSrvcDoEntryTypePciRegister
#undef OvrdDfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef OvrdDfltAssertCpuSrvcDoEntryTypeProfileFixup
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevMsr
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef OvrdDfltAssertCpuSrvcDoEntryTypeSmuIndexReg
#undef OvrdDfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef OvrdDfltAssertCpuSrvcDoEntryTypeCopyBitField
#undef OvrdDfltAssertCpuSrvcGetRegisterTableList
#undef OvrdDfltAssertCpuSrvcGetWorkaroundTable
#undef OvrdDfltAssertCpuSrvcComputeUnitMap
#undef OvrdDfltAssertCpuSrvcInitCacheDisabled
#undef OvrdDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef OvrdDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef OvrdDfltAssertCpuSrvcIsSecureS3
#undef OvrdDfltAssertCpuSrvcPatchLoaderIsSharedByCU


#undef FinalDfltCpuSrvcRevision
#undef FinalDfltCpuSrvcDisablePstate
#undef FinalDfltCpuSrvcTransitionPstate
#undef FinalDfltCpuSrvcGetProcIddMax
#undef FinalDfltCpuSrvcGetTscRate
#undef FinalDfltCpuSrvcGetCurrentNbFrequency
#undef FinalDfltCpuSrvcGetMinMaxNbFrequency
#undef FinalDfltCpuSrvcGetNbPstateInfo
#undef FinalDfltCpuSrvcIsNbCofInitNeeded
#undef FinalDfltCpuSrvcGetNbIddMax
#undef FinalDfltCpuSrvcLaunchApCore
#undef FinalDfltCpuSrvcGetNumberOfPhysicalCores
#undef FinalDfltCpuSrvcGetApCoreNumber
#undef FinalDfltCpuSrvcCoreIdPositionInInitialApicId
#undef FinalDfltCpuSrvcSetWarmResetFlag
#undef FinalDfltCpuSrvcGetWarmResetFlag
#undef FinalDfltCpuSrvcGetMicroCodePatchesStruct
#undef FinalDfltCpuSrvcGetMicrocodeEquivalenceTable
#undef FinalDfltCpuSrvcGetCacheInfo
#undef FinalDfltCpuSrvcGetSysPmTableStruct
#undef FinalDfltCpuSrvcGetWheaInitData
#undef FinalDfltCpuSrvcGetPlatformTypeSpecificInfo
#undef FinalDfltCpuSrvcIsNbPstateEnabled
#undef FinalDfltCpuSrvcDoEntryTypeMsrRegister
#undef FinalDfltCpuSrvcDoEntryTypePciRegister
#undef FinalDfltCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef FinalDfltCpuSrvcDoEntryTypeProfileFixup
#undef FinalDfltCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef FinalDfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef FinalDfltCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef FinalDfltCpuSrvcDoEntryTypeCpuRevPciRegister
#undef FinalDfltCpuSrvcDoEntryTypeCpuRevMsr
#undef FinalDfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef FinalDfltCpuSrvcDoEntryTypeSmuIndexReg
#undef FinalDfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef FinalDfltCpuSrvcDoEntryTypeCopyBitField
#undef FinalDfltCpuSrvcGetRegisterTableList
#undef FinalDfltCpuSrvcGetWorkaroundTable
#undef FinalDfltCpuSrvcComputeUnitMap
#undef FinalDfltCpuSrvcInitCacheDisabled
#undef FinalDfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef FinalDfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef FinalDfltCpuSrvcIsSecureS3
#undef FinalDfltCpuSrvcPatchLoaderIsSharedByCU


#undef FinalDfltAssertCpuSrvcRevision
#undef FinalDfltAssertCpuSrvcDisablePstate
#undef FinalDfltAssertCpuSrvcTransitionPstate
#undef FinalDfltAssertCpuSrvcGetProcIddMax
#undef FinalDfltAssertCpuSrvcGetTscRate
#undef FinalDfltAssertCpuSrvcGetCurrentNbFrequency
#undef FinalDfltAssertCpuSrvcGetMinMaxNbFrequency
#undef FinalDfltAssertCpuSrvcGetNbPstateInfo
#undef FinalDfltAssertCpuSrvcIsNbCofInitNeeded
#undef FinalDfltAssertCpuSrvcGetNbIddMax
#undef FinalDfltAssertCpuSrvcLaunchApCore
#undef FinalDfltAssertCpuSrvcGetNumberOfPhysicalCores
#undef FinalDfltAssertCpuSrvcGetApCoreNumber
#undef FinalDfltAssertCpuSrvcCoreIdPositionInInitialApicId
#undef FinalDfltAssertCpuSrvcSetWarmResetFlag
#undef FinalDfltAssertCpuSrvcGetWarmResetFlag
#undef FinalDfltAssertCpuSrvcGetMicroCodePatchesStruct
#undef FinalDfltAssertCpuSrvcGetMicrocodeEquivalenceTable
#undef FinalDfltAssertCpuSrvcGetCacheInfo
#undef FinalDfltAssertCpuSrvcGetSysPmTableStruct
#undef FinalDfltAssertCpuSrvcGetWheaInitData
#undef FinalDfltAssertCpuSrvcGetPlatformTypeSpecificInfo
#undef FinalDfltAssertCpuSrvcIsNbPstateEnabled
#undef FinalDfltAssertCpuSrvcDoEntryTypeMsrRegister
#undef FinalDfltAssertCpuSrvcDoEntryTypePciRegister
#undef FinalDfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef FinalDfltAssertCpuSrvcDoEntryTypeProfileFixup
#undef FinalDfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef FinalDfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef FinalDfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef FinalDfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
#undef FinalDfltAssertCpuSrvcDoEntryTypeCpuRevMsr
#undef FinalDfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef FinalDfltAssertCpuSrvcDoEntryTypeSmuIndexReg
#undef FinalDfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef FinalDfltAssertCpuSrvcDoEntryTypeCopyBitField
#undef FinalDfltAssertCpuSrvcGetRegisterTableList
#undef FinalDfltAssertCpuSrvcGetWorkaroundTable
#undef FinalDfltAssertCpuSrvcComputeUnitMap
#undef FinalDfltAssertCpuSrvcInitCacheDisabled
#undef FinalDfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef FinalDfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef FinalDfltAssertCpuSrvcIsSecureS3
#undef FinalDfltAssertCpuSrvcPatchLoaderIsSharedByCU


#undef FinalCpuSrvcRevision
#undef FinalCpuSrvcDisablePstate
#undef FinalCpuSrvcTransitionPstate
#undef FinalCpuSrvcGetProcIddMax
#undef FinalCpuSrvcGetTscRate
#undef FinalCpuSrvcGetCurrentNbFrequency
#undef FinalCpuSrvcGetMinMaxNbFrequency
#undef FinalCpuSrvcGetNbPstateInfo
#undef FinalCpuSrvcIsNbCofInitNeeded
#undef FinalCpuSrvcGetNbIddMax
#undef FinalCpuSrvcLaunchApCore
#undef FinalCpuSrvcGetNumberOfPhysicalCores
#undef FinalCpuSrvcGetApCoreNumber
#undef FinalCpuSrvcCoreIdPositionInInitialApicId
#undef FinalCpuSrvcSetWarmResetFlag
#undef FinalCpuSrvcGetWarmResetFlag
#undef FinalCpuSrvcGetMicroCodePatchesStruct
#undef FinalCpuSrvcGetMicrocodeEquivalenceTable
#undef FinalCpuSrvcGetCacheInfo
#undef FinalCpuSrvcGetSysPmTableStruct
#undef FinalCpuSrvcGetWheaInitData
#undef FinalCpuSrvcGetPlatformTypeSpecificInfo
#undef FinalCpuSrvcIsNbPstateEnabled
#undef FinalCpuSrvcDoEntryTypeMsrRegister
#undef FinalCpuSrvcDoEntryTypePciRegister
#undef FinalCpuSrvcDoEntryTypeFamSpecificWorkaround
#undef FinalCpuSrvcDoEntryTypeProfileFixup
#undef FinalCpuSrvcDoEntryTypeCoreCountsPciRegister
#undef FinalCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#undef FinalCpuSrvcDoEntryTypeCompUnitCountsMsr
#undef FinalCpuSrvcDoEntryTypeCpuRevPciRegister
#undef FinalCpuSrvcDoEntryTypeCpuRevMsr
#undef FinalCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#undef FinalCpuSrvcDoEntryTypeSmuIndexReg
#undef FinalCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#undef FinalCpuSrvcDoEntryTypeCopyBitField
#undef FinalCpuSrvcGetRegisterTableList
#undef FinalCpuSrvcGetWorkaroundTable
#undef FinalCpuSrvcComputeUnitMap
#undef FinalCpuSrvcInitCacheDisabled
#undef FinalCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#undef FinalCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#undef FinalCpuSrvcIsSecureS3
#undef FinalCpuSrvcPatchLoaderIsSharedByCU

//
// Define null CPU specific services - no redefinition is expected.
//
#define NullCpuSrvcRevision                                       (UINT16)                                      0
#define NullCpuSrvcDisablePstate                                  (PF_CPU_DISABLE_PSTATE)                       CommonReturnAgesaSuccess
#define NullCpuSrvcTransitionPstate                               (PF_CPU_TRANSITION_PSTATE)                    CommonReturnAgesaSuccess
#define NullCpuSrvcGetProcIddMax                                  (PF_CPU_GET_IDD_MAX)                          CommonReturnFalse
#define NullCpuSrvcGetTscRate                                     (PF_CPU_GET_TSC_RATE)                         CommonReturnAgesaSuccess
#define NullCpuSrvcGetCurrentNbFrequency                          (PF_CPU_GET_NB_FREQ)                          CommonReturnAgesaError
#define NullCpuSrvcGetMinMaxNbFrequency                           (PF_CPU_GET_MIN_MAX_NB_FREQ)                  CommonReturnAgesaError
#define NullCpuSrvcGetNbPstateInfo                                (PF_CPU_GET_NB_PSTATE_INFO)                   CommonReturnFalse
#define NullCpuSrvcIsNbCofInitNeeded                              (PF_CPU_IS_NBCOF_INIT_NEEDED)                 CommonReturnAgesaSuccess
#define NullCpuSrvcGetNbIddMax                                    (PF_CPU_GET_NB_IDD_MAX)                       CommonReturnFalse
#define NullCpuSrvcLaunchApCore                                   (PF_CPU_AP_INITIAL_LAUNCH)                    CommonReturnFalse
#define NullCpuSrvcGetNumberOfPhysicalCores                       (PF_CPU_NUMBER_OF_PHYSICAL_CORES)             CommonReturnZero8
#define NullCpuSrvcGetApCoreNumber                                (PF_CPU_GET_AP_CORE_NUMBER)                   CommonReturnOne32
#define NullCpuSrvcCoreIdPositionInInitialApicId                  (PF_CORE_ID_POSITION_IN_INITIAL_APIC_ID)      CommonReturnAgesaSuccess
#define NullCpuSrvcSetWarmResetFlag                               (PF_CPU_SET_WARM_RESET_FLAG)                  CommonReturnAgesaSuccess
#define NullCpuSrvcGetWarmResetFlag                               (PF_CPU_GET_WARM_RESET_FLAG)                  CommonReturnAgesaSuccess
#define NullCpuSrvcGetMicroCodePatchesStruct                      (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            GetEmptyArray
#define NullCpuSrvcGetMicrocodeEquivalenceTable                   (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            GetEmptyArray
#define NullCpuSrvcGetCacheInfo                                   (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            GetEmptyArray
#define NullCpuSrvcGetSysPmTableStruct                            (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            GetEmptyArray
#define NullCpuSrvcGetWheaInitData                                (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            GetEmptyArray
#define NullCpuSrvcGetPlatformTypeSpecificInfo                    (PF_CPU_GET_PLATFORM_TYPE_SPECIFIC_INFO)      CommonReturnAgesaSuccess
#define NullCpuSrvcIsNbPstateEnabled                              (PF_IS_NB_PSTATE_ENABLED)                     CommonReturnFalse
#define NullCpuSrvcDoEntryTypeMsrRegister                         (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypePciRegister                         (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeFamSpecificWorkaround               (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeProfileFixup                        (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCoreCountsPciRegister               (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCompUnitCountsPciRegister           (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCompUnitCountsMsr                   (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCpuRevPciRegister                   (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCpuRevMsr                           (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround         (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeSmuIndexReg                         (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeProfileFixupSmuIndexReg             (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcDoEntryTypeCopyBitField                        (PF_DO_TABLE_ENTRY)                           CommonAssert
#define NullCpuSrvcGetRegisterTableList                           (PF_GET_REGISTER_TABLE_LIST)                  CommonReturnNULL
#define NullCpuSrvcGetWorkaroundTable                             (PF_GET_WORKAROUND_TABLE)                     NULL
#define NullCpuSrvcComputeUnitMap                                 (COMPUTE_UNIT_MAP *)                          NULL
#define NullCpuSrvcInitCacheDisabled                              (FAMILY_CACHE_INIT_POLICY)                    InitCacheDisabled
#define NullCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable          (PF_GET_EARLY_INIT_TABLE)                     CommonVoid
#define NullCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable           (PF_GET_EARLY_INIT_TABLE)                     CommonVoid
#define NullCpuSrvcIsSecureS3                                     (PF_IS_SECURE_S3)                             CommonReturnFalse
#define NullCpuSrvcPatchLoaderIsSharedByCU                        (BOOLEAN)                                     FALSE



//
// Define default cpu specific services assertion if possible
//
#define DfltAssertCpuSrvcRevision                                 NullCpuSrvcRevision
#define DfltAssertCpuSrvcDisablePstate                            (PF_CPU_DISABLE_PSTATE)                       CommonAssert
#define DfltAssertCpuSrvcTransitionPstate                         (PF_CPU_TRANSITION_PSTATE)                    CommonAssert
#define DfltAssertCpuSrvcGetProcIddMax                            (PF_CPU_GET_IDD_MAX)                          CommonAssert
#define DfltAssertCpuSrvcGetTscRate                               (PF_CPU_GET_TSC_RATE)                         CommonAssert
#define DfltAssertCpuSrvcGetCurrentNbFrequency                    (PF_CPU_GET_NB_FREQ)                          CommonAssert
#define DfltAssertCpuSrvcGetMinMaxNbFrequency                     (PF_CPU_GET_MIN_MAX_NB_FREQ)                  CommonAssert
#define DfltAssertCpuSrvcGetNbPstateInfo                          (PF_CPU_GET_NB_PSTATE_INFO)                   CommonAssert
#define DfltAssertCpuSrvcIsNbCofInitNeeded                        (PF_CPU_IS_NBCOF_INIT_NEEDED)                 CommonAssert
#define DfltAssertCpuSrvcGetNbIddMax                              (PF_CPU_GET_NB_IDD_MAX)                       CommonAssert
#define DfltAssertCpuSrvcLaunchApCore                             (PF_CPU_AP_INITIAL_LAUNCH)                    CommonAssert
#define DfltAssertCpuSrvcGetNumberOfPhysicalCores                 (PF_CPU_NUMBER_OF_PHYSICAL_CORES)             CommonAssert
#define DfltAssertCpuSrvcGetApCoreNumber                          (PF_CPU_GET_AP_CORE_NUMBER)                   CommonAssert
#define DfltAssertCpuSrvcCoreIdPositionInInitialApicId            (PF_CORE_ID_POSITION_IN_INITIAL_APIC_ID)      CommonAssert
#define DfltAssertCpuSrvcSetWarmResetFlag                         (PF_CPU_SET_WARM_RESET_FLAG)                  CommonAssert
#define DfltAssertCpuSrvcGetWarmResetFlag                         (PF_CPU_GET_WARM_RESET_FLAG)                  CommonAssert
#define DfltAssertCpuSrvcGetMicroCodePatchesStruct                (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            CommonAssert
#define DfltAssertCpuSrvcGetMicrocodeEquivalenceTable             (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            CommonAssert
#define DfltAssertCpuSrvcGetCacheInfo                             (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            CommonAssert
#define DfltAssertCpuSrvcGetSysPmTableStruct                      (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            CommonAssert
#define DfltAssertCpuSrvcGetWheaInitData                          (PF_CPU_GET_FAMILY_SPECIFIC_ARRAY)            CommonAssert
#define DfltAssertCpuSrvcGetPlatformTypeSpecificInfo              (PF_CPU_GET_PLATFORM_TYPE_SPECIFIC_INFO)      CommonAssert
#define DfltAssertCpuSrvcIsNbPstateEnabled                        (PF_IS_NB_PSTATE_ENABLED)                     CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeMsrRegister                   (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypePciRegister                   (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround         (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeProfileFixup                  (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister         (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister     (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr             (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister             (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCpuRevMsr                     (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround   (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeSmuIndexReg                   (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg       (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcDoEntryTypeCopyBitField                  (PF_DO_TABLE_ENTRY)                           CommonAssert
#define DfltAssertCpuSrvcGetRegisterTableList                     NullCpuSrvcGetRegisterTableList
#define DfltAssertCpuSrvcGetWorkaroundTable                       NullCpuSrvcGetWorkaroundTable
#define DfltAssertCpuSrvcComputeUnitMap                           NullCpuSrvcComputeUnitMap
#define DfltAssertCpuSrvcInitCacheDisabled                        (FAMILY_CACHE_INIT_POLICY)                    InitCacheDisabled
#define DfltAssertCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable    (PF_GET_EARLY_INIT_TABLE)                     CommonAssert
#define DfltAssertCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable     (PF_GET_EARLY_INIT_TABLE)                     CommonAssert
#define DfltAssertCpuSrvcIsSecureS3                               (PF_IS_SECURE_S3)                             CommonAssert
#define DfltAssertCpuSrvcPatchLoaderIsSharedByCU                  NullCpuSrvcPatchLoaderIsSharedByCU



//
// Define family specific services default when feature is enabled in the build
//
#define DfltCpuSrvcRevision                                       NullCpuSrvcRevision
#define DfltCpuSrvcDisablePstate                                  DfltAssertCpuSrvcDisablePstate
#define DfltCpuSrvcTransitionPstate                               DfltAssertCpuSrvcTransitionPstate
#define DfltCpuSrvcGetProcIddMax                                  NullCpuSrvcGetProcIddMax
#define DfltCpuSrvcGetTscRate                                     DfltAssertCpuSrvcGetTscRate
#define DfltCpuSrvcGetCurrentNbFrequency                          DfltAssertCpuSrvcGetCurrentNbFrequency
#define DfltCpuSrvcGetMinMaxNbFrequency                           NullCpuSrvcGetMinMaxNbFrequency
#define DfltCpuSrvcGetNbPstateInfo                                NullCpuSrvcGetNbPstateInfo
#define DfltCpuSrvcIsNbCofInitNeeded                              (PF_CPU_IS_NBCOF_INIT_NEEDED)                 CommonReturnFalse
#define DfltCpuSrvcGetNbIddMax                                    DfltAssertCpuSrvcGetNbIddMax
#define DfltCpuSrvcLaunchApCore                                   NullCpuSrvcLaunchApCore
#define DfltCpuSrvcGetNumberOfPhysicalCores                       (PF_CPU_NUMBER_OF_PHYSICAL_CORES)             CommonReturnOne8
#define DfltCpuSrvcGetApCoreNumber                                NullCpuSrvcGetApCoreNumber
#define DfltCpuSrvcCoreIdPositionInInitialApicId                  (PF_CORE_ID_POSITION_IN_INITIAL_APIC_ID)      CommonReturnOne32
#define DfltCpuSrvcSetWarmResetFlag                               (PF_CPU_SET_WARM_RESET_FLAG)                  CommonVoid
#define DfltCpuSrvcGetWarmResetFlag                               DfltAssertCpuSrvcGetWarmResetFlag
#define DfltCpuSrvcGetMicroCodePatchesStruct                      NullCpuSrvcGetMicroCodePatchesStruct
#define DfltCpuSrvcGetMicrocodeEquivalenceTable                   NullCpuSrvcGetMicrocodeEquivalenceTable
#define DfltCpuSrvcGetCacheInfo                                   NullCpuSrvcGetCacheInfo
#define DfltCpuSrvcGetSysPmTableStruct                            NullCpuSrvcGetSysPmTableStruct
#define DfltCpuSrvcGetWheaInitData                                NullCpuSrvcGetWheaInitData
#define DfltCpuSrvcGetPlatformTypeSpecificInfo                    NullCpuSrvcGetPlatformTypeSpecificInfo
#define DfltCpuSrvcIsNbPstateEnabled                              NullCpuSrvcIsNbPstateEnabled
#define DfltCpuSrvcDoEntryTypeMsrRegister                         DfltAssertCpuSrvcDoEntryTypeMsrRegister
#define DfltCpuSrvcDoEntryTypePciRegister                         DfltAssertCpuSrvcDoEntryTypePciRegister
#define DfltCpuSrvcDoEntryTypeFamSpecificWorkaround               DfltAssertCpuSrvcDoEntryTypeFamSpecificWorkaround
#define DfltCpuSrvcDoEntryTypeProfileFixup                        DfltAssertCpuSrvcDoEntryTypeProfileFixup
#define DfltCpuSrvcDoEntryTypeCoreCountsPciRegister               DfltAssertCpuSrvcDoEntryTypeCoreCountsPciRegister
#define DfltCpuSrvcDoEntryTypeCompUnitCountsPciRegister           DfltAssertCpuSrvcDoEntryTypeCompUnitCountsPciRegister
#define DfltCpuSrvcDoEntryTypeCompUnitCountsMsr                   DfltAssertCpuSrvcDoEntryTypeCompUnitCountsMsr
#define DfltCpuSrvcDoEntryTypeCpuRevPciRegister                   DfltAssertCpuSrvcDoEntryTypeCpuRevPciRegister
#define DfltCpuSrvcDoEntryTypeCpuRevMsr                           DfltAssertCpuSrvcDoEntryTypeCpuRevMsr
#define DfltCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround         DfltAssertCpuSrvcDoEntryTypeCpuRevFamSpecificWorkaround
#define DfltCpuSrvcDoEntryTypeSmuIndexReg                         DfltAssertCpuSrvcDoEntryTypeSmuIndexReg
#define DfltCpuSrvcDoEntryTypeProfileFixupSmuIndexReg             DfltAssertCpuSrvcDoEntryTypeProfileFixupSmuIndexReg
#define DfltCpuSrvcDoEntryTypeCopyBitField                        DfltAssertCpuSrvcDoEntryTypeCopyBitField
#define DfltCpuSrvcGetRegisterTableList                           NullCpuSrvcGetRegisterTableList
#define DfltCpuSrvcGetWorkaroundTable                             NullCpuSrvcGetWorkaroundTable
#define DfltCpuSrvcComputeUnitMap                                 NullCpuSrvcComputeUnitMap
// NOTE: From CPUs with compute units and moving forward, we use InitCacheEnabled.
#define DfltCpuSrvcInitCacheDisabled                              (FAMILY_CACHE_INIT_POLICY)                    InitCacheEnabled
#define DfltCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable          NullCpuSrvcGetEarlyInitBeforeApLaunchOnCoreTable
#define DfltCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable           NullCpuSrvcGetEarlyInitAfterApLaunchOnCoreTable
#define DfltCpuSrvcIsSecureS3                                     (PF_IS_SECURE_S3)                             CommonReturnFalse
#define DfltCpuSrvcPatchLoaderIsSharedByCU                        NullCpuSrvcPatchLoaderIsSharedByCU


