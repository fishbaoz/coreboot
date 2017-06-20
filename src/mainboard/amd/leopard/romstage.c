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

#include <console/console.h>
#include <arch/acpi.h>
#include <arch/io.h>
#include <arch/stages.h>
#include <cpu/x86/lapic.h>
#include <cpu/x86/bist.h>
#include <cpu/amd/car.h>
#include <cpu/amd/pi/s3_resume.h>
#include <northbridge/amd/pi/agesawrapper.h>
#include <northbridge/amd/pi/agesawrapper_call.h>
#include <southbridge/amd/pi/hudson/hudson.h>

#define SIO1036_SERIAL_MINIBOARD 0

#if SIO1036_SERIAL_MINIBOARD
#include "superio/smsc/sio1036/sio1036_early_init.c"
#define SERIAL_DEV PNP_DEV(0x4E, SIO1036_SP1)
#else
#include "superio/fintek/f81216h/f81216h.h"
#define SERIAL_DEV PNP_DEV(0x4e, F81216H_SP1)
#endif

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx)
{
	u32 val;
#if CONFIG_HAVE_ACPI_RESUME
	void *resume_backup_memory;
#endif

	amd_initmmio();

	/* TODO: */
#ifdef SERIAL_DEV
	pci_devfn_t dev = PCI_DEV(0, 0x14, 3);
	pci_write_config32(dev, 0x44, 0xff03ffd5);
	pci_write_config32(dev, 0x48, 0x20ff1b);
//	*(volatile u32 *) (0xFED80000 + 0xE00 + 0x28) &= ~(7 << 16); /* 48Mhz */
	*(volatile u32 *) (0xFED80000 + 0xE00 + 0x28) |= (1 << 16); /* 24Mhz */
	*(volatile u32 *) (0xFED80000 + 0xE00 + 0x40) &= ~(1 << 2); /* 24Mhz */

#endif
	hudson_lpc_port80();

	if (!cpu_init_detectedx && boot_cpu()) {
		post_code(0x30);

#if IS_ENABLED(CONFIG_HUDSON_UART)
		configure_hudson_uart();
#endif
#if SIO1036_SERIAL_MINIBOARD
		sio1036_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE);
#else
		f81216h_enable_serial(SERIAL_DEV, CONFIG_TTYS0_BASE, MODE_7777);
#endif
		post_code(0x31);
		console_init();
	}

	/* Halt if there was a built in self test failure */
	post_code(0x34);
	report_bist_failure(bist & 0x7FFFFFFF); /* Mask bit 31. One result of Silicon Observation */

	/* Load MPB */
	val = cpuid_eax(1);
	printk(BIOS_DEBUG, "BSP Family_Model: %08x \n", val);
	printk(BIOS_DEBUG, "cpu_init_detectedx = %08lx \n", cpu_init_detectedx);

	post_code(0x37);
	AGESAWRAPPER(amdinitreset);
	post_code(0x38);
	printk(BIOS_DEBUG, "Got past agesawrapper_amdinitreset\n");

	post_code(0x39);
	AGESAWRAPPER(amdinitearly);
	int s3resume = acpi_is_wakeup_s3();
	if (!s3resume) {
		post_code(0x40);
		AGESAWRAPPER(amdinitpost);
		post_code(0x41);
		AGESAWRAPPER(amdinitenv);
		/* TODO: Disable cache is not ok. */
		disable_cache_as_ram();
	} else { /* S3 detect */
		printk(BIOS_INFO, "S3 detected\n");

		post_code(0x60);
		AGESAWRAPPER(amdinitresume);

		AGESAWRAPPER(amds3laterestore);

		post_code(0x61);
		prepare_for_resume();
	}

	val = 0x100;
	while(!(DP_RX_Chip_Located()) ) { // chip location
		val --;
		if (!val) break;
	}

	ANX1122_Initialization ();


	if (s3resume || acpi_is_wakeup_s4()) {
		outb(0xEE, PM_INDEX);
		outb(0x8, PM_DATA);
	}

	post_code(0x50);
	copy_and_run();

	post_code(0x54);  /* Should never see this post code. */
}
