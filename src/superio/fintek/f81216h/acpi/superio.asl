/*
 * This file is part of the coreboot project.
 *
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

/*
 * Include this file into a mainboard's DSDT _SB device tree and it will
 * expose the W83627DHG SuperIO and some of its functionality.
 *
 * It allows the change of IO ports, IRQs and DMA settings on logical
 * devices, disabling and reenabling logical devices and controlling power
 * saving mode on logical devices or the whole chip.
 *
 *   LDN		State
 *
 * Controllable through preprocessor defines:
 */

#undef SUPERIO_CHIP_NAME
#define SUPERIO_CHIP_NAME F81216H
#include <superio/acpi/pnp.asl>

Device(SUPERIO_DEV) {
	Name (_HID, EisaId("PNP0A05"))
	Name (_STR, Unicode("Fintek F81216H Super I/O"))
	Name (_UID, SUPERIO_UID(SUPERIO_DEV,))

	/* Mutex for accesses to the configuration ports */
//	Mutex(CMMX, 1)

	/* SuperIO configuration ports */
	OperationRegion (CREG, SystemIO, SUPERIO_PNP_BASE, 0x02)
	Field (CREG, ByteAcc, NoLock, Preserve)
	{
		PNP_ADDR_REG,	8,
		PNP_DATA_REG,   8
	}
	IndexField (ADDR, DATA, ByteAcc, NoLock, Preserve)
	{
		Offset (0x07),
		PNP_LOGICAL_DEVICE,	8, /* Logical device selector */

//		Offset (0x22),
//		FDPW,			1, /* FDC Power Down */
//		,			2,
//		PRPW,			1, /* PRT Power Down */
//		UAPW,			1, /* UART A Power Down */
//		UBPW,			1, /* UART B Power Down */
//		HWPW,			1, /* HWM Power Down */
//		Offset (0x23),
//		IPD,			1, /* Immediate Chip Power Down */
//
		Offset (0x30),
		PNP_DEVICE_ACTIVE,	1, /* Logical device activation */

		Offset (0x60),
		PNP_IO0_HIGH_BYTE,	8, /* First I/O port base - high byte */
		PNP_IO0_LOW_BYTE,	8, /* First I/O port base - low byte */
		Offset (0x62),
		PNP_IO1_HIGH_BYTE,	8, /* Second I/O port base - high byte */
		PNP_IO1_LOW_BYTE,	8, /* Second I/O port base - low byte */

		Offset (0x70),
		PNP_IRQ0,		4, /* First IRQ */
		Offset (0x72),
		PNP_IRQ1,		4, /* Second IRQ */

//		Offset (0x74),
//		PNP_DMA0,		8, /* DMA */
	}

	Method (_CRS)
	{
		/* Announce the used i/o ports to the OS */
		Return (ResourceTemplate () {
			IO (Decode16, SUPERIO_PNP_BASE, SUPERIO_PNP_BASE, 0x01, 0x02)
		})
	}
	Method (_PSC) {
//		ENTER_CONFIG_MODE (PNP_NO_LDN_CHANGE)
//		  Store (IPD, Local0)
//		EXIT_CONFIG_MODE ()
//		If (Local0) { Return (2) }
//		Else { Return (0) }
		Return (0)
	}

	#undef PNP_ENTER_MAGIC_1ST
	#undef PNP_ENTER_MAGIC_2ND
	#undef PNP_ENTER_MAGIC_3RD
	#undef PNP_EXIT_MAGIC_1ST
	#define PNP_ENTER_MAGIC_1ST	0x77
	#define PNP_ENTER_MAGIC_2ND	0x77
	#define PNP_EXIT_MAGIC_1ST	0xaa
	#include <superio/acpi/pnp_config.asl>

#ifdef F81216H_SHOW_UARTA
	#undef SUPERIO_UART_LDN
	#undef SUPERIO_UART_DDN
	#undef SUPERIO_UART_PM_REG
	#undef SUPERIO_UART_PM_VAL
	#undef SUPERIO_UART_PM_LDN
	#define SUPERIO_UART_LDN 0
	#include <superio/acpi/pnp_uart.asl>
#endif

#ifdef F81216H_SHOW_UARTB
	#undef SUPERIO_UART_LDN
	#undef SUPERIO_UART_DDN
	#undef SUPERIO_UART_PM_REG
	#undef SUPERIO_UART_PM_VAL
	#undef SUPERIO_UART_PM_LDN
	#define SUPERIO_UART_LDN 1
	#include <superio/acpi/pnp_uart.asl>
#endif
}
