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

#undef SUPERIO_DEV
#undef SUPERIO_PNP_BASE
#undef F81216H_SHOW_UARTA
#undef F81216H_SHOW_UARTB
#undef F81216H_SHOW_KBC
#undef F81216H_SHOW_PS2M
#undef F81216H_SHOW_HWMON
#define SUPERIO_DEV		SIO0
#define SUPERIO_PNP_BASE	0x4e
#define F81216H_SHOW_UARTA	1
#include <superio/fintek/f81216h/acpi/superio.asl>
