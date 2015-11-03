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
 * Foundation, Inc.
 */

#include <spi-generic.h>
#include <spi_flash.h>
#include <string.h>
#include <device/pci.h>
#include <agesawrapper.h>
#include <AGESA.h>

#define OTP_LEN 0x40
u8 otpdata[OTP_LEN];

/*
  Skip OTP validation.
  1. Set VALIDATE_OTP = 0.

  Skip MAC validation.
  1. Set VALIDATE_MAC_ADDRESS = 0.
 */
#define SHOW_MESSAGE 0
/*
  Write OTP number.
  1. Set BOARD_NUM as a new number which is not marked as used.
  2. Set WRITE_OTP as 1.
  3. build coreboot.rom and run. "write OTP ok" means successful.
  4. Set WRITE_OTP as 0.
  5. build coreboot.rom, which is the final image.
 */

#define VALIDATE_OTP 1
#define WRITE_OTP 0

//echo  1 |  sha512sum | grep -o "[0-9a-f]*" | sed -e 's/\([0-9a-f]\{2\}\)/0x\1,/g' | sed -e 's/,$/};/g' | sed -e 's/^0x/sha512_auth[OTP_LEN]={0x/g'

// echo  1 |  sha512sum  | grep -o "[0-9a-f]*" |  sed -e 's/\([0-9a-f]\{2\}\)/\1 /g'
// dpcmd --raw-instruction "B1|02 00 00 00      |C1"
#define BOARD_NUM 1

#if (BOARD_NUM==1)
/* used */
u8 sha512_auth[OTP_LEN]={0xec,0xac,0x2a,0x9a,0x34,0x05,0x7d,0xd4,0x47,0xf8,0xa5,0x4b,0x0e,0xe6,0xb6,0xaf,0x63,0x4d,0xa4,0x61,0x22,0x19,0x2d,0x0a,0x6d,0x53,0x58,0x55,0x9e,0x9b,0xc1,0xa9,0x38,0x98,0x9c,0x68,0x13,0xf0,0x92,0x5e,0xca,0xb0,0x27,0xca,0x65,0xe5,0x11,0x72,0x69,0x30,0x5c,0xc2,0x16,0xe4,0xc8,0x37,0x9d,0x03,0x35,0xf5,0x62,0x43,0x35,0x68};
#endif

#if (BOARD_NUM==2)
u8 sha512_auth[OTP_LEN]={0x4d,0xba,0xfc,0xf9,0xe4,0x63,0x6d,0xa7,0x98,0x66,0xe3,0x7a,0x47,0x99,0xc5,0x8e,0xab,0xce,0xb5,0x7d,0xb7,0x23,0x7b,0x34,0x2d,0x81,0x74,0xed,0x68,0xa8,0x66,0x44,0xc7,0x96,0x64,0x11,0x67,0xde,0x44,0xec,0x26,0x44,0x24,0x3c,0x24,0x58,0x05,0xec,0x16,0xc4,0x7d,0x5e,0x4f,0x12,0xc2,0xb8,0x28,0x90,0xd4,0x0d,0x62,0xcc,0x39,0x45};
#endif

#if (BOARD_NUM==3)
u8 sha512_auth[OTP_LEN]={0x4e,0x3f,0x72,0xb5,0x79,0x01,0x67,0xba,0xf0,0xdc,0xcd,0xda,0xc5,0xed,0xdc,0x4f,0x56,0x08,0x07,0xbe,0xa2,0x61,0xcd,0xd7,0x3e,0x0c,0x93,0xe4,0xa7,0x49,0xce,0xbe,0x6a,0xb9,0xc6,0xb0,0xac,0xfe,0x83,0xa0,0x5e,0x8c,0xca,0xdc,0x65,0x38,0x63,0x72,0x31,0x7b,0x48,0x7b,0x0a,0x8c,0xb9,0x85,0x57,0x02,0x72,0xdd,0x5e,0xb5,0xa0,0xc3};
#endif

#if (BOARD_NUM==4)
u8 sha512_auth[OTP_LEN]={0x01,0x4b,0x7e,0x9c,0x36,0x02,0xf9,0xd1,0xac,0xe9,0xbb,0x4f,0x22,0x5a,0x2d,0x7c,0x0c,0xc6,0x47,0x9b,0x88,0xd5,0x8d,0x1e,0x75,0xf5,0x4c,0x52,0xc7,0x58,0xea,0xe7,0x0c,0x9d,0x8b,0x3b,0x9f,0x3f,0x84,0x59,0xb5,0xf6,0x27,0xb9,0x65,0x00,0x42,0xd2,0x50,0x00,0xec,0xd2,0x05,0xad,0x4b,0x22,0xb0,0x9b,0xf0,0xb9,0x06,0x02,0x9d,0x93};
#endif

#if (BOARD_NUM==5)
u8 sha512_auth[OTP_LEN]={0x5b,0xe1,0x2b,0x8b,0x69,0x79,0xe2,0x6b,0x98,0x2e,0xad,0xd6,0x3d,0xc1,0x46,0x79,0xde,0x74,0xc2,0x6d,0xcd,0x3b,0x15,0x7a,0x9d,0xb7,0xc3,0x8f,0xf1,0x5c,0x4a,0xc5,0x86,0x16,0xfa,0x08,0x83,0xe4,0xd9,0xd1,0x38,0x5d,0x6a,0x1c,0x80,0x7a,0x97,0xd7,0x88,0x02,0x42,0xa9,0x61,0x8d,0xf0,0xa4,0x1e,0x93,0x24,0xe7,0x3a,0x62,0x0e,0x8e};
#endif

#if (BOARD_NUM==6)
u8 sha512_auth[OTP_LEN]={0xff,0xe5,0x3d,0xa9,0xb4,0xf9,0xf0,0xc3,0x5b,0x0c,0x89,0xbb,0x72,0xb5,0x52,0x09,0xc0,0xda,0x67,0x2a,0x73,0xfb,0xd2,0x1f,0xce,0x19,0x59,0x4c,0x8e,0x37,0xdd,0xb9,0xef,0xf1,0x79,0x36,0x30,0x32,0x06,0xb3,0xe0,0x7e,0x3c,0xd0,0x8e,0x5a,0x59,0x1f,0x84,0x24,0x8a,0xbc,0xc3,0xb0,0xf9,0x8b,0x5a,0x12,0xe4,0x63,0xb6,0xe7,0xd4,0x18};
#endif

#if (BOARD_NUM==7)
u8 sha512_auth[OTP_LEN]={0xe8,0xad,0xd7,0x2e,0xd6,0x51,0x21,0x34,0xda,0x95,0x0e,0xb0,0x3f,0xd6,0xdb,0x1b,0x24,0xd9,0xc1,0x86,0x63,0xf5,0x54,0x01,0x76,0xff,0x46,0xb1,0xd2,0x58,0x60,0x59,0x82,0xf0,0x76,0x19,0x8b,0x97,0x73,0xdc,0xd9,0xd8,0x4f,0x04,0x8a,0xa1,0x6e,0x04,0x04,0xf7,0xca,0x47,0x8c,0xe6,0x62,0x38,0x1c,0xbb,0x03,0xe6,0xee,0x7e,0x4f,0xa3};
#endif

#if (BOARD_NUM==8)
u8 sha512_auth[OTP_LEN]={0xac,0x5b,0x79,0x88,0x09,0xfd,0xb9,0x00,0xe2,0x60,0xdb,0x4b,0x42,0xf0,0xb1,0x68,0xbe,0x13,0xd0,0xee,0xfc,0x44,0x65,0xd1,0x78,0xe4,0x3a,0x3b,0xbd,0xf1,0x2d,0x36,0x75,0xf5,0x24,0x51,0x6f,0xd0,0xcb,0x55,0x78,0x54,0x9c,0x27,0xca,0x75,0x77,0x4d,0x16,0xb8,0x57,0x3a,0xb2,0x26,0x30,0x91,0xad,0xe1,0xa5,0xed,0x31,0x53,0xa3,0xfb};
#endif

#if (BOARD_NUM==9)
u8 sha512_auth[OTP_LEN]={0x42,0xe6,0xeb,0x8f,0x97,0xf7,0xdd,0x22,0xe7,0xb2,0x92,0x5f,0xc4,0xcd,0xfe,0x96,0xfb,0x17,0x8c,0xf7,0xc9,0x1a,0x60,0x3c,0xa5,0xfb,0xaf,0x8a,0xcd,0x07,0x52,0x7b,0x3f,0x76,0x83,0x8b,0x53,0xa3,0x7c,0x0f,0x5b,0x78,0x96,0x75,0xb4,0x9e,0x60,0x7a,0xe1,0xfb,0xa1,0xc4,0x38,0xd9,0xec,0x19,0x03,0xf3,0x4d,0x1d,0x74,0x83,0xd1,0x3b};
#endif

#if (BOARD_NUM==10)
u8 sha512_auth[OTP_LEN]={0x3f,0xfb,0x4a,0x46,0xbc,0xe8,0x87,0x7a,0xfc,0xcd,0x6c,0x7e,0x1e,0x7c,0xcc,0x09,0xa4,0xd0,0x25,0x46,0xbf,0xae,0xd1,0xe7,0xc9,0xf4,0xa0,0x60,0x0d,0x4c,0xbd,0xaa,0x57,0x97,0x8b,0x6a,0x39,0x8b,0xe3,0x77,0x34,0xc5,0x09,0x97,0xce,0x6f,0xfa,0x53,0xf1,0xe2,0x42,0x67,0x9f,0x9a,0x35,0x3c,0x08,0x25,0x64,0x7b,0x21,0x5f,0x53,0xd0};
#endif



static AGESA_STATUS oem_validate_auth(void)
{
	struct spi_flash *flash;
	u32 i;

	spi_init();
	flash = spi_flash_probe(0, 0, 0, 0);
	if (!flash)
		return -1;

	spi_claim_bus(flash->spi);

	spi_flash_cmd_read_otp(flash, 0, OTP_LEN, otpdata);
	for (i=0; i<OTP_LEN; i++) {
		#if WRITE_OTP
		printk(BIOS_DEBUG, " %02X", otpdata[i]);
		#endif
		if (otpdata[i] != sha512_auth[i]) {
			#if WRITE_OTP
			printk(BIOS_DEBUG, "([%x should be %02X])\n", i, sha512_auth[i]);
			#endif
			for (;;); /* Hang */
		}
	}
	#if WRITE_OTP
	printk(BIOS_DEBUG, "\n");
	#endif

	spi_release_bus(flash->spi);
	return 0;
}

#if WRITE_OTP
static AGESA_STATUS oem_write_auth(void)
{
	struct spi_flash *flash;
	u32 i;

	spi_init();
	flash = spi_flash_probe(0, 0, 0, 0);
	if (!flash)
		return -1;

	flash->spi->rw = SPI_WRITE_FLAG;
	spi_claim_bus(flash->spi);

	/* read first */
	spi_flash_cmd_read_otp(flash, 0, OTP_LEN, otpdata);
	for (i=0; i<OTP_LEN; i++) {
		printk(BIOS_DEBUG, " %02X", otpdata[i]);
		if (otpdata[i] != 0xFF) {
			printk(BIOS_DEBUG, "\nOTP is not empty\n");
			/* print data left */
			i ++;
			while (i < OTP_LEN) printk(BIOS_DEBUG, " %02X", otpdata[i]);
			for (;;); /* Hang */
		}
	}
	printk(BIOS_DEBUG, "\n");

	spi_flash_cmd_write_otp(flash, 0, OTP_LEN, sha512_auth);
	printk(BIOS_DEBUG, "wrote\n");

	spi_flash_cmd(flash->spi, 0x2F, NULL, 0); /* locked */
	printk(BIOS_DEBUG, "locked\n");

	flash->spi->rw = SPI_WRITE_FLAG;

	spi_release_bus(flash->spi);
	return 0;
}
#endif

#define VALIDATE_MAC_ADDRESS 1

#if VALIDATE_MAC_ADDRESS
void read_mac()
{
	device_t dev;
	u32 ioaddr;
	u8 mac_valid[8] = {0x00,0x1b,0x21,0xb3,0xa9,0x63,0,0};
	u32 mac_hi, mac_lo, *mac_ptr = mac_valid;
	dev = dev_find_device(0x8086, 0x10D3, 0);

	ioaddr = pci_read_config32(dev, 0x18) & ~0xF;

	outl(0x5400, ioaddr);
	mac_lo = inl(ioaddr + 4);
	outl(0x5404, ioaddr);
	mac_hi = inl(ioaddr + 4) & 0xFFFF;
	#if SHOW_MESSAGE
	printk(BIOS_DEBUG, "mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
	       mac_valid[0], mac_valid[1], mac_valid[2], mac_valid[3], mac_valid[4], mac_valid[5]);
	#endif
	if ((mac_lo != mac_ptr[0]) || (mac_hi != mac_ptr[1]))
		for (;;);
	#if SHOW_MESSAGE
	printk(BIOS_DEBUG, "mac validation ok\n");
	#endif

	/* TODO:nvram */
}
#endif

AGESA_STATUS oem_auth(void)
{
#if VALIDATE_OTP
#if WRITE_OTP
	oem_write_auth();
	oem_validate_auth();
	printk(BIOS_DEBUG, "write OTP ok\n");
#else
	oem_validate_auth();
#endif

#if VALIDATE_MAC_ADDRESS
	read_mac();
#endif

#if WRITE_OTP
	for (;;);
#endif

#endif	/* VALIDATE_OTP */
	return 0;
}

