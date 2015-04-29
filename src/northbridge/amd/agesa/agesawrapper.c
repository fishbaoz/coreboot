/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011-2012 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>
#include <string.h>

#include <cpu/amd/agesa/s3_resume.h>
#include <northbridge/amd/agesa/agesawrapper.h>
#include <northbridge/amd/agesa/BiosCallOuts.h>
#include "amdlib.h"

#include "heapManager.h"

static const struct OEM_HOOK *OemHook = &OemCustomize;

#if defined(__PRE_RAM__)

AGESA_STATUS agesawrapper_amdinitreset(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;
	AMD_RESET_PARAMS AmdResetParams;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));
	memset(&AmdResetParams, 0, sizeof(AMD_RESET_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_RESET;
	AmdParamStruct.AllocationMethod = ByHost;
	AmdParamStruct.NewStructSize = sizeof(AMD_RESET_PARAMS);
	AmdParamStruct.NewStructPtr = &AmdResetParams;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;

	AmdCreateStruct(&AmdParamStruct);
//	AmdResetParams.HtConfig.Depth = 0;

	status = AmdInitReset((AMD_RESET_PARAMS *) AmdParamStruct.NewStructPtr);
	AGESA_EVENTLOG(status, &AmdParamStruct.StdHeader);
	AmdReleaseStruct(&AmdParamStruct);
	return status;
}

AGESA_STATUS agesawrapper_amdinitearly(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;
	AMD_EARLY_PARAMS *AmdEarlyParamsPtr;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_EARLY;
	AmdParamStruct.AllocationMethod = PreMemHeap;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;
	AmdCreateStruct(&AmdParamStruct);

	/* OEM Should Customize the defaults through this hook. */
	AmdEarlyParamsPtr = (AMD_EARLY_PARAMS *) AmdParamStruct.NewStructPtr;
	if (OemHook->InitEarly)
		OemHook->InitEarly(AmdEarlyParamsPtr);

	status = AmdInitEarly(AmdEarlyParamsPtr);
	AGESA_EVENTLOG(status, &AmdParamStruct.StdHeader);

	AmdReleaseStruct(&AmdParamStruct);

	return status;
}

AGESA_STATUS agesawrapper_amdinitpost(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;
	AMD_POST_PARAMS *PostParams;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_POST;
	AmdParamStruct.AllocationMethod = PreMemHeap;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;

	AmdCreateStruct(&AmdParamStruct);

	/* OEM Should Customize the defaults through this hook. */
	PostParams = (AMD_POST_PARAMS *) AmdParamStruct.NewStructPtr;
	if (OemHook->InitPost)
		OemHook->InitPost(PostParams);

	status = AmdInitPost(PostParams);
	AGESA_EVENTLOG(status, &PostParams->StdHeader);

	AmdReleaseStruct(&AmdParamStruct);

	/* Initialize heap space */
	EmptyHeap();

	return status;
}

AGESA_STATUS agesawrapper_amdinitresume(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;
	AMD_RESUME_PARAMS *AmdResumeParamsPtr;
	S3_DATA_TYPE S3DataType;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_RESUME;
	AmdParamStruct.AllocationMethod = PreMemHeap;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;
	AmdCreateStruct(&AmdParamStruct);

	AmdResumeParamsPtr = (AMD_RESUME_PARAMS *) AmdParamStruct.NewStructPtr;

	AmdResumeParamsPtr->S3DataBlock.NvStorageSize = 0;
	AmdResumeParamsPtr->S3DataBlock.VolatileStorageSize = 0;
	S3DataType = S3DataTypeNonVolatile;

	OemAgesaGetS3Info(S3DataType,
			  (u32 *) & AmdResumeParamsPtr->S3DataBlock.NvStorageSize,
			  (void **)&AmdResumeParamsPtr->S3DataBlock.NvStorage);

	status = AmdInitResume((AMD_RESUME_PARAMS *) AmdParamStruct.NewStructPtr);

	AGESA_EVENTLOG(status, &AmdParamStruct.StdHeader);
	AmdReleaseStruct(&AmdParamStruct);

	return status;
}

AGESA_STATUS agesawrapper_amdinitenv(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;
	AMD_ENV_PARAMS *EnvParam;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_ENV;
	AmdParamStruct.AllocationMethod = PostMemDram;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;
	AmdCreateStruct(&AmdParamStruct);
	EnvParam = (AMD_ENV_PARAMS *) AmdParamStruct.NewStructPtr;

	status = AmdInitEnv(EnvParam);
	AGESA_EVENTLOG(status, &EnvParam->StdHeader);

	AmdReleaseStruct(&AmdParamStruct);
	return status;
}

AGESA_STATUS agesawrapper_amds3laterestore(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdInterfaceParams;
	AMD_S3LATE_PARAMS AmdS3LateParams;
	AMD_S3LATE_PARAMS *AmdS3LateParamsPtr;
	S3_DATA_TYPE S3DataType;

	memset(&AmdS3LateParams, 0, sizeof(AMD_S3LATE_PARAMS));

	AmdInterfaceParams.StdHeader.ImageBasePtr = 0;
	AmdInterfaceParams.AllocationMethod = ByHost;
	AmdInterfaceParams.AgesaFunctionName = AMD_S3LATE_RESTORE;
	AmdInterfaceParams.NewStructPtr = &AmdS3LateParams;
	AmdInterfaceParams.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdS3LateParamsPtr = &AmdS3LateParams;
	AmdInterfaceParams.NewStructSize = sizeof(AMD_S3LATE_PARAMS);

	AmdCreateStruct(&AmdInterfaceParams);

	AmdS3LateParamsPtr->S3DataBlock.VolatileStorageSize = 0;
	S3DataType = S3DataTypeVolatile;

	OemAgesaGetS3Info(S3DataType,
			  (u32 *) & AmdS3LateParamsPtr->S3DataBlock.VolatileStorageSize,
			  (void **)&AmdS3LateParamsPtr->S3DataBlock.VolatileStorage);

	status = AmdS3LateRestore(AmdS3LateParamsPtr);
	AGESA_EVENTLOG(status, &AmdInterfaceParams.StdHeader);
	ASSERT(status == AGESA_SUCCESS);

	return status;
}

#else /* __PRE_RAM__ */

AGESA_STATUS agesawrapper_amdinitmid(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;
	AMD_MID_PARAMS *MidParam;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_MID;
	AmdParamStruct.AllocationMethod = PostMemDram;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;

	AmdCreateStruct(&AmdParamStruct);

	/* OEM Should Customize the defaults through this hook. */
	MidParam = (AMD_MID_PARAMS *) AmdParamStruct.NewStructPtr;
	if (OemHook->InitMid)
		OemHook->InitMid(MidParam);

	status = AmdInitMid(MidParam);
	AGESA_EVENTLOG(status, &MidParam->StdHeader);
	AmdReleaseStruct(&AmdParamStruct);

	return status;
}

AGESA_STATUS agesawrapper_amdS3Save(void)
{
	AGESA_STATUS status;
#if IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ)
	AMD_RTB_PARAMS *AmdS3SaveParamsPtr;
#else
	AMD_S3SAVE_PARAMS *AmdS3SaveParamsPtr;
#endif
	AMD_INTERFACE_PARAMS AmdInterfaceParams;
	S3_DATA_TYPE S3DataType;

	memset(&AmdInterfaceParams, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdInterfaceParams.StdHeader.ImageBasePtr = 0;
	AmdInterfaceParams.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
	AmdInterfaceParams.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdInterfaceParams.AllocationMethod = PostMemDram;
#if IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ)
	AmdInterfaceParams.AgesaFunctionName = AMD_INIT_RTB;
#else
	AmdInterfaceParams.AgesaFunctionName = AMD_S3_SAVE;
#endif
	AmdInterfaceParams.StdHeader.AltImageBasePtr = 0;
	AmdInterfaceParams.StdHeader.Func = 0;
	AmdCreateStruct(&AmdInterfaceParams);

#if IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ)
	AmdS3SaveParamsPtr = (AMD_RTB_PARAMS *) AmdInterfaceParams.NewStructPtr;
#else
	AmdS3SaveParamsPtr = (AMD_S3SAVE_PARAMS *) AmdInterfaceParams.NewStructPtr;
#endif
	AmdS3SaveParamsPtr->StdHeader = AmdInterfaceParams.StdHeader;

#if IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ)
	status = AmdInitRtb(AmdS3SaveParamsPtr);
#else
	status = AmdS3Save(AmdS3SaveParamsPtr);
#endif
	AGESA_EVENTLOG(status, &AmdInterfaceParams.StdHeader);
	ASSERT(status == AGESA_SUCCESS);

	S3DataType = S3DataTypeNonVolatile;

//	status = OemAgesaSaveS3Info(S3DataType,
//				    AmdS3SaveParamsPtr->S3DataBlock.NvStorageSize,
//				    AmdS3SaveParamsPtr->S3DataBlock.NvStorage);

	if (AmdS3SaveParamsPtr->S3DataBlock.VolatileStorageSize != 0) {
		S3DataType = S3DataTypeVolatile;

//		status = OemAgesaSaveS3Info(S3DataType,
//					    AmdS3SaveParamsPtr->S3DataBlock.VolatileStorageSize,
//					    AmdS3SaveParamsPtr->S3DataBlock.VolatileStorage);
	}

//	OemAgesaSaveMtrr();
	AmdReleaseStruct(&AmdInterfaceParams);

	return status;
}

/* We will reference AmdLateParams later to copy ACPI tables. */
static AMD_LATE_PARAMS *AmdLateParams = NULL;

AGESA_STATUS agesawrapper_amdinitlate(void)
{
	AGESA_STATUS status;
	AMD_INTERFACE_PARAMS AmdParamStruct;

	memset(&AmdParamStruct, 0, sizeof(AMD_INTERFACE_PARAMS));

	AmdParamStruct.AgesaFunctionName = AMD_INIT_LATE;
	AmdParamStruct.AllocationMethod = PostMemDram;
	AmdParamStruct.StdHeader.AltImageBasePtr = 0;
	AmdParamStruct.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	AmdParamStruct.StdHeader.Func = 0;
	AmdParamStruct.StdHeader.ImageBasePtr = 0;

#if IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15TN) || IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15RL) || \
	IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY16KB) || IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY15CZ)
	AmdParamStruct.StdHeader.HeapStatus = HEAP_SYSTEM_MEM;
#endif

	AmdCreateStruct(&AmdParamStruct);
	AmdLateParams = (AMD_LATE_PARAMS *) AmdParamStruct.NewStructPtr;
	status = AmdInitLate(AmdLateParams);
	AGESA_EVENTLOG(status, &AmdLateParams->StdHeader);
	ASSERT(status == AGESA_SUCCESS);

	/* No AmdReleaseStruct(&AmdParamStruct), we need AmdLateParams later. */
	return status;
}

void *agesawrapper_getlateinitptr(int pick)
{
	ASSERT(AmdLateParams != NULL);

	switch (pick) {
	case PICK_DMI:
		return AmdLateParams->DmiTable;
	case PICK_PSTATE:
		return AmdLateParams->AcpiPState;
//	case PICK_SRAT:
//		return AmdLateParams->AcpiSrat;
//	case PICK_SLIT:
//		return AmdLateParams->AcpiSlit;
	case PICK_WHEA_MCE:
		return AmdLateParams->AcpiWheaMce;
	case PICK_WHEA_CMC:
		return AmdLateParams->AcpiWheaCmc;
	case PICK_ALIB:
		return AmdLateParams->AcpiAlib;
	case PICK_IVRS:
#if IS_ENABLED(CONFIG_CPU_AMD_AGESA_FAMILY14)
		return NULL;
#else
		return AmdLateParams->AcpiIvrs;
#endif
	default:
		return NULL;
	}
	return NULL;
}

#endif /* __PRE_RAM__ */

AGESA_STATUS agesawrapper_amdlaterunaptask(UINT32 Func, UINT32 Data, VOID * ConfigPtr)
{
	AGESA_STATUS status;
	AP_EXE_PARAMS ApExeParams;

	memset(&ApExeParams, 0, sizeof(AP_EXE_PARAMS));

	ApExeParams.StdHeader.AltImageBasePtr = 0;
	ApExeParams.StdHeader.CalloutPtr = (CALLOUT_ENTRY) & GetBiosCallout;
	ApExeParams.StdHeader.Func = 0;
	ApExeParams.StdHeader.ImageBasePtr = 0;
	ApExeParams.FunctionNumber = Func;
	ApExeParams.RelatedDataBlock = ConfigPtr;

	status = AmdLateRunApTask(&ApExeParams);
	AGESA_EVENTLOG(status, &ApExeParams.StdHeader);
	ASSERT(status == AGESA_SUCCESS);

	return status;
}
