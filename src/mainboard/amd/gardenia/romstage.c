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

#include <stdint.h>
#include <string.h>
#include <device/pci_def.h>
#include <device/pci_ids.h>
#include <arch/acpi.h>
#include <arch/io.h>
#include <arch/stages.h>
#include <device/pnp_def.h>
#include <arch/cpu.h>
#include <cpu/x86/lapic.h>
#include <console/console.h>
#include <console/loglevel.h>
#include "cpu/amd/car.h"
#include "agesawrapper.h"
#include <northbridge/amd/agesa/agesawrapper_call.h>
#include "cpu/x86/bist.h"
#include "cpu/x86/lapic.h"
#include "southbridge/amd/agesa/hudson/hudson.h"
#include "cpu/amd/agesa/s3_resume.h"
#include "cbmem.h"

#define _SIMNOW_ 0

#if _SIMNOW_
#include "superio/smsc/mec1308/mec1308_early_serial.c"
#endif

#include "superio/smsc/sio1036/sio1036_early_init.c"

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx);
void disable_cache_as_ram(void);

/* TODO: Larne has some setting which is not clear about the root cause */
#define _LARNE_ 0

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx)
{
	u32 val;
#if CONFIG_HAVE_ACPI_RESUME
	void *resume_backup_memory;
#endif
	/* This is not defined in Kabini BKDG. In Hudson RRG,
	   PMIOxD2[5:4] is "Drive strength control for LpcClk[1:0]".
	   It needs to be set as 4mA. Otherwise the serial output is
	   bad code.
	*/
	#if _LARNE_
	outb(0xD2, 0xcd6);
	outb(0x00, 0xcd7);
	#endif

	AGESAWRAPPER(amdinitmmio);

	hudson_lpc_port80();

	if (!cpu_init_detectedx && boot_cpu()) {
		post_code(0x30);

		post_code(0x31);
		#if _SIMNOW_
		mec1308_early_init(0x2e);
		#endif
		sio1036_early_init(0x4E);

		console_init();
	}

	/* enable lan */
	#if 1
	outb(0x81, 0x666); inb(0x80); inb(0x80); inb(0x80);
	outb(0xAC, 0x662); inb(0x80); inb(0x80); inb(0x80);
	outb(0x80, 0x662); inb(0x80); inb(0x80); inb(0x80);
	#endif

	/* Halt if there was a built in self test failure */
	post_code(0x34);
	report_bist_failure(bist);

	/* Load MPB */
	val = cpuid_eax(1);
	printk(BIOS_DEBUG, "BSP Family_Model: %08x \n", val);
	printk(BIOS_DEBUG, "cpu_init_detectedx = %08lx \n", cpu_init_detectedx);
	#if _LARNE_ && ! _SIMNOW_
	/* TODO: Just Guess.
	   On Larne, after LpcClkDrvSth is set, it needs some time to be stable, because of the buffer ICS551M */
	int i;
	for(i = 0; i < 200000; i++)
		val = inb(0xcd6);
	#endif

	post_code(0x37);
	AGESAWRAPPER(amdinitreset);
	post_code(0x38);
	printk(BIOS_DEBUG, "Got past agesawrapper_amdinitreset\n");

	post_code(0x39);

	AGESAWRAPPER(amdinitearly);
	int s3resume = acpi_is_wakeup_early() && acpi_s3_resume_allowed();
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

	outb(0xEA, 0xCD6);
	outb(0x1, 0xcd7);

	//for (;;); // 8254 ok.
	post_code(0x50);
	copy_and_run();

	post_code(0x54);  /* Should never see this post code. */
}
