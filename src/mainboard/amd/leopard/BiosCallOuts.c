/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2015 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <device/pci_def.h>
#include <device/device.h>
#include "AGESA.h"
#include "amdlib.h"
#include <northbridge/amd/pi/BiosCallOuts.h>
#include <northbridge/amd/pi/00660F01/chip.h>
#include "Ids.h"
#include "OptionsIds.h"
#include "heapManager.h"
#include "FchPlatform.h"
#include "cbfs.h"
#if IS_ENABLED(CONFIG_HUDSON_IMC_FWM)
#include "imc.h"
#endif
#include "hudson.h"
#include <stdlib.h>
#include "BiosCallOuts.h"
#include "northbridge/amd/pi/dimmSpd.h"
#include "northbridge/amd/pi/agesawrapper.h"
#include <PlatformMemoryConfiguration.h>
#include <boardid.h>

static AGESA_STATUS Fch_Oem_config(UINT32 Func, UINT32 FchData, VOID *ConfigPtr);
static AGESA_STATUS board_ReadSpd(UINT32 Func, UINTN Data, VOID *ConfigPtr);

const BIOS_CALLOUT_STRUCT BiosCallouts[] =
{
	{AGESA_ALLOCATE_BUFFER,          agesa_AllocateBuffer },
	{AGESA_DEALLOCATE_BUFFER,        agesa_DeallocateBuffer },
	{AGESA_LOCATE_BUFFER,            agesa_LocateBuffer },
	{AGESA_READ_SPD,                 board_ReadSpd },
	{AGESA_DO_RESET,                 agesa_Reset },
	{AGESA_READ_SPD_RECOVERY,        agesa_NoopUnsupported },
	{AGESA_RUNFUNC_ONAP,             agesa_RunFuncOnAp },
	{AGESA_GET_IDS_INIT_DATA,        agesa_EmptyIdsInitData },
	{AGESA_HOOKBEFORE_DQS_TRAINING,  agesa_NoopSuccess },
	{AGESA_HOOKBEFORE_EXIT_SELF_REF, agesa_NoopSuccess },
	{AGESA_FCH_OEM_CALLOUT,          Fch_Oem_config },
	{AGESA_GNB_GFX_GET_VBIOS_IMAGE,  agesa_GfxGetVbiosImage }
};
const int BiosCalloutsLen = ARRAY_SIZE(BiosCallouts);

static const GPIO_CONTROL oem_leopard_gpio[] = {
	{86, Function1, FCH_GPIO_PULL_UP_ENABLE | FCH_GPIO_OUTPUT_VALUE | FCH_GPIO_OUTPUT_ENABLE | DrvStrengthSel_12mA},
	{64, Function1, FCH_GPIO_PULL_UP_ENABLE | FCH_GPIO_OUTPUT_VALUE | FCH_GPIO_OUTPUT_ENABLE | DrvStrengthSel_12mA},
	{-1}
};
/**
 * Fch Oem setting callback
 *
 *  Configure platform specific Hudson device,
 *   such as Azalia, SATA, IMC etc.
 */
AGESA_STATUS Fch_Oem_config(UINT32 Func, UINT32 FchData, VOID *ConfigPtr)
{
	AMD_CONFIG_PARAMS *StdHeader = ConfigPtr;

	if (StdHeader->Func == AMD_INIT_RESET) {
		FCH_RESET_DATA_BLOCK *FchParams_reset = (FCH_RESET_DATA_BLOCK *)FchData;
		printk(BIOS_DEBUG, "Fch OEM config in INIT RESET ");
		//FchParams_reset->EcChannel0 = TRUE; /* logical devicd 3 */
		FchParams_reset->FchReset.Xhci0Enable = IS_ENABLED(CONFIG_HUDSON_XHCI_ENABLE);
		FchParams_reset->FchReset.Xhci1Enable = FALSE;
		FchParams_reset->EarlyOemGpioTable = oem_leopard_gpio;
	} else if (StdHeader->Func == AMD_INIT_ENV) {
		FCH_DATA_BLOCK *FchParams_env = (FCH_DATA_BLOCK *)FchData;
		printk(BIOS_DEBUG, "Fch OEM config in INIT ENV ");
#if IS_ENABLED(CONFIG_HUDSON_IMC_FWM)
		oem_fan_control(FchParams_env);
#endif

		/* XHCI configuration */
#if CONFIG_HUDSON_XHCI_ENABLE
		FchParams_env->Usb.Xhci0Enable = TRUE;
#else
		FchParams_env->Usb.Xhci0Enable = FALSE;
#endif
		FchParams_env->Usb.Xhci1Enable = FALSE;
		FchParams_env->Usb.USB30PortInit = 8; /* 8: If USB3 port is unremoveable. */

		/* sata configuration */
		/* SD configuration */
		/* Rev F has an on-board eMMC, which only supports SD 2.0 */
		if (board_id() == 'F') {
			FchParams_env->Sd.SdConfig = SdVer2;
		}
	}
	printk(BIOS_DEBUG, "Done\n");

	return AGESA_SUCCESS;
}

const UINT8 spd_buffer[0x100] = {
	0x92, 0x11, 0x0B, 0x03,
	0x04,	/* 4:Bits 6 ~ 4: 000 = (8 banks), 001 = (16 banks), 010 = 5 (32 banks),011 = 6 (64 banks)*/
		/* Bits 3 ~ 0:0000 = 256 Mb, 0001 = 512 Mb, 0010 = 1 Gb, 0011 = 2 Gb, 0100 = 4 Gb, 0101 = 8 Gb, 0110 = 16 Gb */
	0x19,   /* 5: bit 5~3: 000 = 12, 001 = 13, 010 = 14, 011 = 15, 100 = 16
		      Bit 2~0: 000 = 9, 001 = 10, 010 = 11, 011 = 12 */
	0x00,   /*  */
	0x02 | 1 << 3, /*bit 5~3:000 = 1 Rank 001 = 2 Ranks 010 = 3 Ranks  011 = 4 Ranks,
			 bit 2~0: 000 = 4 bits 001 = 8 bits 010 = 16 bits 011 = 32 bits */
	0x03,
	0x52,	/* FTB: 0x51: 5 ps, 0x52: 2.5 ps, 0x11: 1ps */
	0x01, 0x08,		/* MTB: */
	0x0A,			/* tCK: A: 1.25ns: C:1.5ns */
	0x00,
	0xFE, 0x00,		/* CAS Latency */
	[16] = 0x6E,		/* tAAmin: 0x69:13.125ns, 0x6E: 13.75 */
	0x78,			/* tWRmin: 15ns. All DDR3 */
	0x6E,			/* tRCDmin: see tAAmin */
	0x30,			/* tRRD: 6ns */
	0x6E,			/* tRP: see tAAmin */
	0x11,			/* Upper of tRAS, tRC */
	0x18,			/* tRAS: 35ns*/
	0x86,			/* tRC: 0x81:48.125 0x86:48.75 */
	0x80, 0x07,		/* tRFC: ?? 90ns   (240ns: 0x80, 0x07) */
	0x3C,			/* tWTR: 7.5ns. */
	0x3C,			/* tRTP: 7.5ns */
	0x01, 0x00,		/* tFAW: ?? 32ns*/
	0x83,
	0x01,
	[32] = 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	[48] = 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	[60] = 0x0F, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	[77] = 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	[117] = 0x80,
	0x2C,
	0x00,
	0x00,
	0x00,
	0x00, 0x00, 0x00, 0x00,
	[126] = 0x3F, 0xDC,
	/*4KTF12864HZ-1G4K1*/
	[146] = 0x4B, 0x31, 0x80, 0x2C,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF
};


static AGESA_STATUS board_ReadSpd(UINT32 Func, UINTN Data, VOID *ConfigPtr)
{
#ifdef __PRE_RAM__
	int spdAddress;
	AGESA_READ_SPD_PARAMS *info = ConfigPtr;

	ROMSTAGE_CONST struct device *dev = dev_find_slot(0, PCI_DEVFN(0x18, 2));
	ROMSTAGE_CONST struct northbridge_amd_pi_00660F01_config *config = dev->chip_info;
	UINT8 spdAddrLookup_rev_F [2][2][4]= {
		{ {0xA0, 0xA2}, {0xA4, 0xAC}, }, /* socket 0 - Channel 0 & 1 - 8-bit SPD addresses */
		{ {0x00, 0x00}, {0x00, 0x00}, }, /* socket 1 - Channel 0 & 1 - 8-bit SPD addresses */
	};

	if ((dev == 0) || (config == 0))
		return AGESA_ERROR;
	if (info->SocketId >= ARRAY_SIZE(config->spdAddrLookup))
		return AGESA_ERROR;
	if (info->MemChannelId >= ARRAY_SIZE(config->spdAddrLookup[0]))
		return AGESA_ERROR;
	if (info->DimmId >= ARRAY_SIZE(config->spdAddrLookup[0][0]))
		return AGESA_ERROR;
	#if 1
	if (board_id() == 'F')
		spdAddress = spdAddrLookup_rev_F
			[info->SocketId] [info->MemChannelId] [info->DimmId];
	else
		spdAddress = config->spdAddrLookup
			[info->SocketId] [info->MemChannelId] [info->DimmId];

	if (spdAddress == 0)
		return AGESA_ERROR;
	int err = hudson_readSpd(spdAddress, (void *) info->Buffer, 128);
	if (err)
		return AGESA_ERROR;
	#else
	for (spdAddress=0; spdAddress<256; spdAddress++)
		((UINT8*)info->Buffer)[spdAddress] = spd_buffer[spdAddress];
	for (spdAddress=0; spdAddress<256; spdAddress++) {
		printk(BIOS_DEBUG, "%02X ", ((UINT8*)info->Buffer)[spdAddress]);
		if ((spdAddress%16) == 15)
			printk(BIOS_DEBUG, "\n");
	}

	return AGESA_SUCCESS;

	#endif

#endif
	return AGESA_SUCCESS;
}

#ifdef __PRE_RAM__

const PSO_ENTRY DDR4PlatformMemoryConfiguration[] = {
	DRAM_TECHNOLOGY(ANY_SOCKET, DDR4_TECHNOLOGY),
	NUMBER_OF_DIMMS_SUPPORTED (ANY_SOCKET, ANY_CHANNEL, 2),
	NUMBER_OF_CHANNELS_SUPPORTED (ANY_SOCKET, 2),
	MOTHER_BOARD_LAYERS (LAYERS_6),
	MEMCLK_DIS_MAP (ANY_SOCKET, ANY_CHANNEL, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00),
	CKE_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0xff, 0xff, 0xff, 0xff),
	ODT_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0xff, 0xff, 0xff, 0xff),
	CS_TRI_MAP (ANY_SOCKET, ANY_CHANNEL, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00),
	PSO_END
};

void OemPostParams(AMD_POST_PARAMS *PostParams)
{
	if (board_id() == 'F') {
		PostParams->MemConfig.PlatformMemoryConfiguration = (PSO_ENTRY *)DDR4PlatformMemoryConfiguration;
	}
	PostParams->MemConfig.CfgIgnoreSpdChecksum = FALSE;
}
#endif
