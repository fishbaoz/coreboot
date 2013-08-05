/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
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

#include <device/pci_def.h>
#include <device/device.h>
#include <stdlib.h>
#include "OEM.h"		/* SMBUS0_BASE_ADDRESS */

/* warning: Porting.h includes an open #pragma pack(1) */
#include "Porting.h"
#include "AGESA.h"
#include "amdlib.h"
#include "dimmSpd.h"
#include "chip.h"

/* uncomment for source level debug - GDB gets really confused otherwise. */
//#pragma optimize ("", off)

/**
 *	Read a single SPD byte.  If the first byte is being read, set up the
 *	address and offset. Following bytes auto increment.
 */
static UINT8 readSmbusByte(UINT16 iobase, UINT8 address, char *buffer,
						   int offset, int initial_offset)
{
	unsigned int status = -1;
	UINT64 time_limit;

	/* clear status register */
	__outbyte(iobase + SMBUS_STATUS_REG, 0x1E);

	if (offset == initial_offset) {
		/* Clear slave status, set offset, set slave address and start reading */
		__outbyte(iobase + SMBUS_SLAVE_STATUS_REG, 0x3E);
		__outbyte(iobase + SMBUS_CONTROL_REG, offset);
		__outbyte(iobase + SMBUS_HOST_CMD_REG, address | READ_BIT);
		__outbyte(iobase + SMBUS_COMMAND_REG, SMBUS_READ_BYTE_COMMAND);
	} else {
		/* Issue read command - auto increments to next byte */
		__outbyte(iobase + SMBUS_COMMAND_REG, SMBUS_READ_COMMAND);
	}
	/* time limit to avoid hanging for unexpected error status */
	time_limit = __rdtsc() + MAX_READ_TSC_COUNT;
	while (__rdtsc() <= time_limit) {
		status = __inbyte(iobase + SMBUS_STATUS_REG);
		if ((status & SMBUS_INTERRUPT_MASK) == 0)
			continue;	/* SMBusInterrupt not set, keep waiting */
		if ((status & HOSTBUSY_MASK) != 0)
			continue;	/* HostBusy set, keep waiting */
		break;
	}

	if (status != STATUS__COMPLETED_SUCCESSFULLY)
		return AGESA_ERROR;

	buffer[0] = __inbyte(iobase + SMBUS_DATA0_REG);
	return AGESA_SUCCESS;
}

static void writePmReg(UINT8 reg, UINT8 data)
{
	__outbyte(PMIO_INDEX_REG, reg);
	__outbyte(PMIO_DATA_REG, data);
}

static void setupFch(UINT16 ioBase)
{
	/* set up SMBUS - Set to SMBUS 0 & set base address */
	/* For SB800 & Hudson1 to SB900 & Hudson 2/3 */
	writePmReg(SMBUS_BAR_HIGH_BYTE, ioBase >> 8);
	writePmReg(SMBUS_BAR_LOW_BYTE, (ioBase & 0xe0) | 1);

	/* set SMBus clock to 400 KHz */
	__outbyte(ioBase + SMBUS_CLOCK_REG, SMBUS_FREQUENCY_CONST / 400000);
}

/**
 *	Read one or more SPD bytes from a DIMM.
 *	Start with offset zero and read sequentially.
 *	Reads 128 bytes in 7-8 ms at 400 KHz.
 */
#if 0
UINT8 spd_buffer[0x100] = {
	[0] = 92,
	[1] = 0x05,
	[2] = 0x0B,
	[3] = 0x01,
	[4] = 0x02,
	[5] = 0x12,
	[6] = 0x00,
	[7] = 0x00,
	[8] = 0x0B,
	[9] = 0x52,
	[10] = 0x01,
	[11] = 0x08,
	[12] = 0x0C,
	[13] = 0x00,
	[14] = 0x7C,
	[15] = 0x00,
	[16] = 0x6C,
	[17] = 0x78,
	[18] = 0x6C,
	[19] = 0x30,
	[20] = 0x6C,
	[21] = 0x11,
	[22] = 0x20,
	[23] = 0x8C,
	[24] = 0x70,
	[25] = 0x03,
	[26] = 0x3C,
	[27] = 0x3C,
	[28] = 0x00,
	[29] = 0xF0,
	[30] = 0x82,
	[31] = 0x05,
	[32] = 0x80,
	[33] = 0x00,
	[34 ... 59] = 0x00,
	[60] = 0x03,
	[61] = 0x11,
	[62] = 0x0B,
	[63] = 0x00,
	[64] = 0x00,
	[65] = 0x04,
	[66] = 0xB3,
	[67] = 0x03,
	[68] = 0x00,
	[69] = 0x00,
	[70] = 0x50,
	[71] = 0x55,
	[72] = 0x00,
	[73] = 0x00,
	[74] = 0x00,
	[75] = 0x00,
	[76] = 0x00,
	[77 ... 116] = 0x00,
	[117] = 0x80,
	[118] = 0x2C,
	//[119] =  Variable
	//120 Variable
	//121 Variable
	//122125 Variable
	//126127 Variable
	//128145 18JBF25672PY-1G4DZ
	[146] = 0x44,
	[147] = 0x5A,
	[148] = 0x80,
	[149] = 0x2C,
	[150 ... 175] = 0x00,
	[176 ... 255] = 0xFF
};
#else
UINT8 spd_buffer[0x100] = {
	0x92, 0x11, 0x0B, 0x03, 0x03, 0x11, 0x02, 0x02,	0x03, 0x11, 0x01, 0x08, 0x0C, 0x00, 0x7E, 0x00,
	0x69, 0x78, 0x69, 0x3C, 0x69, 0x11, 0x20, 0x89, 0x00, 0x05, 0x3C, 0x3C, 0x01, 0x68, 0x83, 0x05,
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
#endif
static UINT8 readspd(UINT16 iobase, UINT8 SmbusSlaveAddress, char *buffer,
					 UINT16 count)
{
	UINT16 index;
	UINT8 status;
	UINT8 initial_offset = 0;

	setupFch(iobase);

	for (index = initial_offset; index < count; index++) {
		status = readSmbusByte(iobase, SmbusSlaveAddress, &buffer[index], index,
				initial_offset);
		if (status != AGESA_SUCCESS)
			return status;
	}

	return status;
}

/**
 * Gets the SMBUS address for an SPD from the array in devicetree.cb
 * then read the SPD into the supplied buffer.
 */
AGESA_STATUS agesa_ReadSPD(UINT32 unused1, UINT32 unused2, void *infoptr)
{
	UINT8 spdAddress;
	AGESA_READ_SPD_PARAMS *info = infoptr;
	ROMSTAGE_CONST struct device *dev = dev_find_slot(0, PCI_DEVFN(0x18, 2));
	ROMSTAGE_CONST struct northbridge_amd_agesa_family14_config *config = NULL;

	if ((dev == 0) || (dev->chip_info == 0))
		return AGESA_ERROR;

	if ((info->SocketId == 0) && (info->MemChannelId == 0) && (info->DimmId == 0)) {
		int i;
		for (i=0; i<256; i++)
			((UINT8*)info->Buffer)[i] = spd_buffer[i];
		for (i=0; i<256; i++) {
			printk(BIOS_DEBUG, "%02X ", ((UINT8*)info->Buffer)[i]);
			if ((i%16) == 15)
				printk(BIOS_DEBUG, "\n");
		}

		return AGESA_SUCCESS;
	} else {
		return AGESA_ERROR;
	}

	config = dev->chip_info;
	if (info->SocketId >= ARRAY_SIZE(config->spdAddrLookup))
		return AGESA_ERROR;
	if (info->MemChannelId >= ARRAY_SIZE(config->spdAddrLookup[0]))
		return AGESA_ERROR;
	if (info->DimmId >= ARRAY_SIZE(config->spdAddrLookup[0][0]))
		return AGESA_ERROR;

	spdAddress = config->spdAddrLookup
		[info->SocketId][info->MemChannelId][info->DimmId];

	if (spdAddress == 0)
		return AGESA_ERROR;
	return readspd(SMBUS0_BASE_ADDRESS, spdAddress, (void *)info->Buffer, 128);
}
