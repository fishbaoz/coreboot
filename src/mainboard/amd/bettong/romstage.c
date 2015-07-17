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
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
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
#include <delay.h>

#define ACPI_MMIO_BASE ((void *)0xFED80000)
#define AOAC_BASE 0x1E00

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx)
{
	u32 val;
#if CONFIG_HAVE_ACPI_RESUME
	void *resume_backup_memory;
#endif
#if CONFIG_DRIVERS_UART_8250MEM
	u8 byte;
	msr_t msr;
#endif

	amd_initmmio();

	hudson_lpc_port80();

	if (!cpu_init_detectedx && boot_cpu()) {
		post_code(0x30);

		post_code(0x31);
#if CONFIG_DRIVERS_UART_8250MEM
		msr = rdmsr(0x1B);
		msr.lo |= 1 << 11;
		wrmsr(0x1B, msr);
		byte = read8(ACPI_MMIO_BASE + AOAC_BASE + 0x56 + CONFIG_UART_FOR_CONSOLE * 2);
		byte |= 1 << 3;
		write8(ACPI_MMIO_BASE + AOAC_BASE + 0x56 + CONFIG_UART_FOR_CONSOLE * 2, byte);
		byte = read8(ACPI_MMIO_BASE + AOAC_BASE + 0x62);
		byte |= 1 << 3;
		write8(ACPI_MMIO_BASE + AOAC_BASE + 0x62, byte);
		write8(ACPI_MMIO_BASE + 0xD00 + 0x89, 0);
		write8(ACPI_MMIO_BASE + 0xD00 + 0x8A, 0);
		write8(ACPI_MMIO_BASE + 0xD00 + 0x8E, 0);
		write8(ACPI_MMIO_BASE + 0xD00 + 0x8F, 0);

		udelay(2000);
		write8((void *)0xFEDC6000 + 0x2000 * CONFIG_UART_FOR_CONSOLE + 0x88, 0x01); //reset UART
#endif
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
	if (s3resume || acpi_is_wakeup_s4()) {
		outb(0xEE, 0xCD6);
		outb(0x8, 0xCD7);
	}

	post_code(0x50);
	copy_and_run();

	post_code(0x54);  /* Should never see this post code. */
}
