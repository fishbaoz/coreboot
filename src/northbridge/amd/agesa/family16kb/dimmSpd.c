/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
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

/* warning: Porting.h includes an open #pragma pack(1) */
#include "Porting.h"
#include "AGESA.h"
#include "amdlib.h"
#include "dimmSpd.h"
#include "chip.h"


#define DIMENSION(array)(sizeof (array)/ sizeof (array [0]))

/*-----------------------------------------------------------------------------
 *
 * readSmbusByteData - read a single SPD byte from any offset
 */

static int readSmbusByteData (int iobase, int address, char *buffer, int offset)
{
	unsigned int status;
	UINT64 limit;

	address |= 1; // set read bit

	__outbyte (iobase + 0, 0xFF);                // clear error status
	__outbyte (iobase + 1, 0x1F);                // clear error status
	__outbyte (iobase + 3, offset);              // offset in eeprom
	__outbyte (iobase + 4, address);             // slave address and read bit
	__outbyte (iobase + 2, 0x48);                // read byte command

	// time limit to avoid hanging for unexpected error status (should never happen)
	limit = __rdtsc () + 2000000000 / 10;
	for (;;)
	{
		status = __inbyte (iobase);
		if (__rdtsc () > limit) break;
		if ((status & 2) == 0) continue;               // SMBusInterrupt not set, keep waiting
		if ((status & 1) == 1) continue;               // HostBusy set, keep waiting
		break;
	}

	buffer [0] = __inbyte (iobase + 5);
	if (status == 2) status = 0;                      // check for done with no errors
	return status;
}

/*-----------------------------------------------------------------------------
 *
 * readSmbusByte - read a single SPD byte from the default offset
 *                 this function is faster function readSmbusByteData
 */

static int readSmbusByte (int iobase, int address, char *buffer)
{
	unsigned int status;
	UINT64 limit;

	__outbyte (iobase + 0, 0xFF);                // clear error status
	__outbyte (iobase + 2, 0x44);                // read command

	// time limit to avoid hanging for unexpected error status
	limit = __rdtsc () + 2000000000 / 10;
	for (;;)
	{
		status = __inbyte (iobase);
		if (__rdtsc () > limit) break;
		if ((status & 2) == 0) continue;               // SMBusInterrupt not set, keep waiting
		if ((status & 1) == 1) continue;               // HostBusy set, keep waiting
		break;
	}

	buffer [0] = __inbyte (iobase + 5);
	if (status == 2) status = 0;                      // check for done with no errors
	return status;
}

/*---------------------------------------------------------------------------
 *
 * readspd - Read one or more SPD bytes from a DIMM.
 *           Start with offset zero and read sequentially.
 *           Optimization relies on autoincrement to avoid
 *           sending offset for every byte.
 *          Reads 128 bytes in 7-8 ms at 400 KHz.
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
	0x92, 0x11, 0x0B, 0x03,
	0x10,	/* 4:Bits 6 ~ 4: 000 = (8 banks), 001 = (16 banks), 010 = 5 (32 banks),011 = 6 (64 banks)*/
		/* Bits 3 ~ 0:0000 = 256 Mb, 0001 = 512 Mb, 0010 = 1 Gb, 0011 = 2 Gb, 0100 = 4 Gb, 0101 = 8 Gb, 0110 = 16 Gb */
	0x19,   /* 5: bit 5~3: 000 = 12, 001 = 13, 010 = 14, 011 = 15, 100 = 16
		      Bit 2~0: 000 = 9, 001 = 10, 010 = 11, 011 = 12 */
	0x02,   /*  */
	0x02 | 1 << 3, /*bit 5~3:000 = 1 Rank 001 = 2 Ranks 010 = 3 Ranks  011 = 4 Ranks,
			 bit 2~0: 000 = 4 bits 001 = 8 bits 010 = 16 bits 011 = 32 bits */
	0x03,
	0x11,	/* FTB: 0x51: 5 ps, 0x52: 2.5 ps, 0x11: 1ps */
	0x01, 0x08,		/* MTB: */
	0x0A,			/* tCK: A: 1.25ns: C:1.5ns */
	0x00,
	0xFE, 0x00,		/* CAS Latency */
	[16] = 0x69,		/* tAAmin: 13.125ns */
	0x78,			/* tWRmin: 15ns */
	0x69,			/* tRCDmin: 13.125ns */
	0x30,			/* tRRD: 6ns */
	0x69,			/* tRP: 13.125ns */
	0x11,			/* Upper of tRAS, tRC */
	0x18,			/* tRAS: 35ns*/
	0x81,			/* tRC: 48.125 */
	0x80, 0x07,		/* tRFC: ?? 240ns*/
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
#endif

static int readspd (int iobase, int SmbusSlaveAddress, char *buffer, int count)
{
	int index, error;

	/* read the first byte using offset zero */
	error = readSmbusByteData (iobase, SmbusSlaveAddress, buffer, 0);
	if (error) return error;

	/* read the remaining bytes using auto-increment for speed */
	for (index = 1; index < count; index++)
	{
		error = readSmbusByte (iobase, SmbusSlaveAddress, &buffer [index]);
		if (error) return error;
	}

	return 0;
}

static void writePmReg (int reg, int data)
{
	__outbyte (0xCD6, reg);
	__outbyte (0xCD7, data);
}

static void setupFch (int ioBase)
{
	writePmReg (0x2D, ioBase >> 8);
	writePmReg (0x2C, ioBase | 1);
	__outbyte (ioBase + 0x0E, 66000000 / 400000 / 4); // set SMBus clock to 400 KHz
}

AGESA_STATUS AmdMemoryReadSPD (UINT32 unused1, UINT32 unused2, AGESA_READ_SPD_PARAMS *info)
{
	int spdAddress, ioBase;
	ROMSTAGE_CONST struct device *dev = dev_find_slot(0, PCI_DEVFN(0x18, 2));
	ROMSTAGE_CONST struct northbridge_amd_agesa_family16kb_config *config = dev->chip_info;

	if ((dev == 0) || (config == 0))
		return AGESA_ERROR;

	if (info->SocketId     >= DIMENSION(config->spdAddrLookup      ))
		return AGESA_ERROR;
	if (info->MemChannelId >= DIMENSION(config->spdAddrLookup[0]   ))
		return AGESA_ERROR;
	if (info->DimmId       >= DIMENSION(config->spdAddrLookup[0][0]))
		return AGESA_ERROR;
#if 0
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
#endif

	spdAddress = config->spdAddrLookup
		[info->SocketId] [info->MemChannelId] [info->DimmId];

	if (spdAddress == 0) return AGESA_ERROR;
	ioBase = 0xB00;
	setupFch (ioBase);
	return readspd (ioBase, spdAddress, (void *) info->Buffer, 128);
}
