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
 */

#include <device/pci_def.h>
#include <device/device.h>

/* warning: Porting.h includes an open #pragma pack(1) */
#include "Porting.h"
#include "AGESA.h"
#include "amdlib.h"

#include <northbridge/amd/pi/dimmSpd.h>

/*-----------------------------------------------------------------------------
 *
 * readSmbusByteData - read a single SPD byte from any offset
 */

int readSmbusByteData (int iobase, int address, char *buffer, int offset)
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

UINT8 writeSmbusByteData(UINT16 iobase, UINT8 address, UINT8 buffer,
					 int offset)
{
	//unsigned int status = -1;
	//UINT64 time_limit;
	unsigned int status;
	UINT64 limit;


	/* clear status register */
	__outbyte(iobase + 0, 0xFF);
	__outbyte(iobase + 1, 0x1F);

	/* set offset, set slave address, set data and start writing */
	__outbyte(iobase + 3, offset);
	__outbyte(iobase + 4, address & (~1));
	__outbyte(iobase + 5, buffer);
	__outbyte(iobase + 2, 0x48);

	/* time limit to avoid hanging for unexpected error status */
	limit = __rdtsc() + 2000000000 / 10;//MAX_READ_TSC_COUNT;
//	while (__rdtsc() <= time_limit) {
//		status = __inbyte(iobase + SMBUS_STATUS_REG);
//		if ((status & SMBUS_INTERRUPT_MASK) == 0)
//			continue;	/* SMBusInterrupt not set, keep waiting */
//		if ((status & HOSTBUSY_MASK) != 0)
//			continue;	/* HostBusy set, keep waiting */
//		break;
//	}
	for (;;)
	{
		status = __inbyte (iobase);
		if (__rdtsc () > limit) break;
		if ((status & 2) == 0) continue;               // SMBusInterrupt not set, keep waiting
		if ((status & 1) == 1) continue;               // HostBusy set, keep waiting
		break;
	}

//	if (status != STATUS__COMPLETED_SUCCESSFULLY)
//		return AGESA_ERROR;

//	return AGESA_SUCCESS;
	if (status == 2) status = 0;                      // check for done with no errors
	return status;

}


/*-----------------------------------------------------------------------------
 *
 * readSmbusByte - read a single SPD byte from the default offset
 *                 this function is faster function readSmbusByteData
 */

int readSmbusByte (int iobase, int address, char *buffer)
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

int writeSmbusByte (int iobase, int address, char buffer)
{
	unsigned int status;
	UINT64 limit;

	__outbyte (iobase + 0, 0xFF);                // clear error status
	__outbyte (iobase + 5, buffer);
	__outbyte (iobase + 2, 0x44);                // read command

	// time limit to avoid hanging for unexpected error status
	limit = __rdtsc () + 2000000000 / 10;
	for (;;) {
		status = __inbyte (iobase);
		if (__rdtsc () > limit) break;
		if ((status & 2) == 0) continue;               // SMBusInterrupt not set, keep waiting
		if ((status & 1) == 1) continue;               // HostBusy set, keep waiting
		break;
	}

	//buffer [0] = __inbyte (iobase + 5);
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

static int readspd (int iobase, int SmbusSlaveAddress, char *buffer, int count)
{
	int index, error;

	printk(BIOS_SPEW, "-------------READING SPD-----------\n");
	printk(BIOS_SPEW, "iobase: 0x%08X, SmbusSlave: 0x%08X, count: %d\n",
						iobase, SmbusSlaveAddress, count);

	/* read the first byte using offset zero */
	error = readSmbusByteData (iobase, SmbusSlaveAddress, buffer, 0);

	if (error) {
		printk(BIOS_ERR, "-------------SPD READ ERROR-----------\n");
		return error;
	}

	/* read the remaining bytes using auto-increment for speed */
	for (index = 1; index < count; index++)
	{
		error = readSmbusByte (iobase, SmbusSlaveAddress, &buffer [index]);
		if (error) {
			printk(BIOS_ERR, "-------------SPD READ ERROR-----------\n");
			return error;
		}
	}
	printk(BIOS_SPEW, "\n");
	printk(BIOS_SPEW, "-------------FINISHED READING SPD-----------\n");

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

#define ADDR_Access     0x50  //DEV_ADDRESS_FOR_ADDRESS
#define DATA_Access     0x8c  //DEV_ADDRESS_FOR_DATA


int hudson_readSpd(int spdAddress, char *buf, size_t len)
{
	int ioBase = 0xB00;
	setupFch (ioBase);
	return readspd (ioBase, spdAddress, buf, len);
}
