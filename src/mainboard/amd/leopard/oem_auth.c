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

#include <spi-generic.h>
#include <spi_flash.h>
#include <string.h>
#include <device/pci.h>
#include <cbfs.h>
//#include <agesawrapper.h>
#include <AGESA.h>

#define OTP_LEN 0x40
#define UNIQUE_ID_LEN 8

//#define VAL_DATA_LEN UNIQUE_ID_LEN
#define VAL_DATA_LEN OTP_LEN

u8 otpdata[VAL_DATA_LEN]={[0 ... VAL_DATA_LEN-1]=0xFF};

#define SHOW_MAC_MESSAGE 0
/*
  Write OTP number.
  1. Set BOARD_NUM as a new number which is not marked as used.
  2. Set WRITE_OTP as 1.
  3. build coreboot.rom and run. "write OTP ok" means successful.
  4. Set WRITE_OTP as 0.
  5. build coreboot.rom, which is the final image.
 */

#define WRITE_OTP 0

//echo  1 |  sha512sum | grep -o "[0-9a-f]*" | sed -e 's/\([0-9a-f]\{2\}\)/0x\1,/g' | sed -e 's/,$/};/g' | sed -e 's/^0x/sha512_auth[VAL_DATA_LEN]={0x/g'

// echo  1 |  sha512sum  | grep -o "[0-9a-f]*" |  sed -e 's/\([0-9a-f]\{2\}\)/\1 /g'
// dpcmd --raw-instruction "B1|02 00 00 00      |C1"
#define BOARD_NUM 0xFFFFFFFF

//#define BOARD_NUM 99 //No tag

#if (BOARD_NUM==0xFFFFFFFF)	/* otp data is in CBFS */
//u8 sha512_auth[VAL_DATA_LEN+0x10];
u8 *sha512_auth=NULL;
#endif


/* 2a bb 44 55 ae 34 ac 57 */
/* dpcmd --raw-instruction "b1|06|02 00 00 00 2a bb 44 55 ae 34 ac 57" */
#if (BOARD_NUM==99) || (BOARD_NUM==4) || (BOARD_NUM==2)
u8 sha512_auth[VAL_DATA_LEN]={0x2a,0xbb,0x44,0x55,0xae,0x34,0xac,0x57,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#endif

/* dpcmd --raw-instruction "b1|06|02 00 00 00 f9 48 85 f1 02 80 14 34" */
#if (BOARD_NUM==134)
u8 sha512_auth[VAL_DATA_LEN]={0xf9,0x48,0x85,0xf1,0x02,0x80,0x14,0x34,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
#endif

#if (BOARD_NUM==36)
/* Kabini */
//u8 sha512_auth[VAL_DATA_LEN]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0A,0x00,0x04,0x00,0x00,0x20,0x00,0x20,0x20,0x02,0x18,0x2D,0x0A,0x6D,0x53,0x58,0x55,0x9E,0x9B,0xC1,0xA9,0x38,0x98,0x9C,0x68,0x13,0xF0,0x92,0x5E,0xCA,0xB0,0x27,0xCA,0x65,0xE5,0x11,0x72,0x69,0x30,0x5C,0xC2,0x16,0xE4,0xC8,0x37,0x9D,0x03,0x35,0xF5,0x62,0x43,0x35,0x68};
u8 sha512_auth[VAL_DATA_LEN]={0xEC,0xAC,0x2A,0x9A,0x34,0x05,0x7D,0xD4,0x47,0xF8,0xA5,0x4B,0x0E,0xE6,0xB6,0xAF,0x63,0x4D,0xA4,0x61,0x22,0x19,0x2D,0x0A,0x6D,0x53,0x58,0x55,0x9E,0x9B,0xC1,0xA9,0x38,0x98,0x9C,0x68,0x13,0xF0,0x92,0x5E,0xCA,0xB0,0x27,0xCA,0x65,0xE5,0x11,0x72,0x69,0x30,0x5C,0xC2,0x16,0xE4,0xC8,0x37,0x9D,0x03,0x35,0xF5,0x62,0x43,0x35,0x68};
#endif

/* read unique id by sf100 */
/* dpcmd --raw-instruction "4B 00 00 00 00" -raw-require-return 8 */

#define SHOW_OTP 0
static AGESA_STATUS oem_validate_otp(void)
{
	struct spi_flash *flash;
	u32 i;

	#if WRITE_OTP || SHOW_OTP
	printk(BIOS_DEBUG, "data in BIOS:\n ");
	for (i=0; i<VAL_DATA_LEN; i++) {
		printk(BIOS_DEBUG, " %02X", sha512_auth[i]);
	}
	#endif

	spi_init();
	flash = spi_flash_probe(0, 0);
	if (!flash)
		return -1;

	spi_claim_bus(flash->spi);

	#if VAL_DATA_LEN == OTP_LEN
	spi_flash_cmd_read_otp(flash, 0, VAL_DATA_LEN, otpdata);
	#else
	spi_flash_cmd_read_uniqueid(flash, 0, VAL_DATA_LEN, otpdata);
	#endif

	#if WRITE_OTP || SHOW_OTP
	printk(BIOS_DEBUG, "data in OTP:\n");
	#endif
	#if VAL_DATA_LEN == OTP_LEN
	for (i=0; i<VAL_DATA_LEN; i++) {
		#if WRITE_OTP || SHOW_OTP
		printk(BIOS_DEBUG, " %02X", otpdata[i]);
		#endif
		outb(i, 0x80);
		outb(i, 0x80);	/* add some delay to exit from otp mode */
		if (otpdata[i] != sha512_auth[i]) {
			#if WRITE_OTP
			printk(BIOS_DEBUG, "([%x should be %02X])\n", i, sha512_auth[i]);
			#endif
			#if !SHOW_OTP
			for (;;); /* Hang */
			#endif
		}
	}
	#else
	u64 *key;
	for (key = (u64 *) sha512_auth; ; key ++) {
		#if SHOW_OTP
		printk(BIOS_DEBUG, "data in OTP:\n %02X %02X %02X %02X %02X %02X %02X %02X\n",
		       ((u8 *)key)[0], ((u8 *)key)[1], ((u8 *)key)[2], ((u8 *)key)[3],
		       ((u8 *)key)[4], ((u8 *)key)[5], ((u8 *)key)[6], ((u8 *)key)[7]);
		#endif
		if (*key == -1)
			for (;;);
		if (*key == ~(*(u64 *)otpdata))
			break;
	}
	#endif
	#if WRITE_OTP || SHOW_OTP
	printk(BIOS_DEBUG, "\n");
	#endif

	spi_release_bus(flash->spi);
	return 0;
}

#if WRITE_OTP
static AGESA_STATUS oem_write_otp(void)
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
	spi_flash_cmd_read_otp(flash, 0, VAL_DATA_LEN, otpdata);
	for (i=0; i<VAL_DATA_LEN; i++) {
		printk(BIOS_DEBUG, " %02X", otpdata[i]);
		if (otpdata[i] != 0xFF) {
			printk(BIOS_DEBUG, "\nOTP is not empty\n");
			/* print data left */
			i ++;
			while (i < VAL_DATA_LEN) printk(BIOS_DEBUG, " %02X", otpdata[i++]);
			for (;;); /* Hang */
			break;
		}
	}
	printk(BIOS_DEBUG, "\n");

	spi_flash_cmd_write_otp(flash, 0, VAL_DATA_LEN, sha512_auth);
	printk(BIOS_DEBUG, "wrote\n");

//	spi_flash_cmd(flash->spi, 0x2F, NULL, 0); /* locked */
	printk(BIOS_DEBUG, "locked\n");

	flash->spi->rw = SPI_WRITE_FLAG;

	spi_release_bus(flash->spi);
	return 0;
}
#endif

static void read_mac(void)
{
	device_t dev;
	u32 ioaddr;
	#if (BOARD_NUM==0xFFFFFFFF)
	u8 *mac_valid=sha512_auth + 0x40;
	#else
	u8 mac_valid[8] = {0x00,0x90,0x27,0xe0,0x01,0x3d,0xff,0xff}; /* 134 7 */
	#endif
	u32 mac_hi=0x63a9, mac_lo=0xb3211b01, *mac_ptr = (u32*)mac_valid;
	dev = dev_find_device(0x8086, 0x10D3, 0);

	ioaddr = pci_read_config32(dev, 0x18) & ~0xF;

	outl(0x5400, ioaddr);
	mac_lo = inl(ioaddr + 4);
	outl(0x5404, ioaddr);
	mac_hi = (inl(ioaddr + 4) & 0xFFFF);
	#if SHOW_MAC_MESSAGE
	printk(BIOS_DEBUG, "mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
	       mac_valid[0], mac_valid[1], mac_valid[2], mac_valid[3], mac_valid[4], mac_valid[5]);
	printk(BIOS_DEBUG, "mac_lo=%x, mac_hi=%x\n", mac_lo, mac_hi);
	#endif
	if ((mac_lo != mac_ptr[0]) || (mac_hi != (mac_ptr[1] & 0xFFFF))) {
		outb (mac_valid[5], 0x80);
		for (;;);
	}
	#if SHOW_MAC_MESSAGE
	printk(BIOS_DEBUG, "mac validation ok\n");
	#endif

	/* TODO:nvram */
}

AGESA_STATUS oem_auth(void)
{
	/* Get data from CBFS */
	#if (BOARD_NUM==0xFFFFFFFF)
	sha512_auth = cbfs_boot_map_with_leak("leopard/validation",
				   CBFS_TYPE_RAW, NULL);
	#endif

#if WRITE_OTP
	oem_write_otp();
	oem_validate_otp();
	printk(BIOS_DEBUG, "write OTP ok\n");
	for (;;);
#else
	oem_validate_otp();
#endif
#if (BOARD_NUM != 36)
//	read_mac();
#endif


	return 0;
}
