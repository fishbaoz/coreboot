/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2013 Sage Electronic Engineering, LLC
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

/* No Super I/O device or functionality yet */
#undef SUPERIO_DEV
#undef SUPERIO_PNP_BASE
#undef W83627DHG_SHOW_UARTA
#undef W83627DHG_SHOW_UARTB
#undef W83627DHG_SHOW_KBC
#undef W83627DHG_SHOW_PS2M
#undef W83627DHG_SHOW_HWMON
#define SUPERIO_DEV		SIO0
#define SUPERIO_PNP_BASE	0x4e
#define W83627DHG_SHOW_UARTA	1
#define W83627DHG_SHOW_UARTB	1
#define W83627DHG_SHOW_KBC		1
#define W83627DHG_SHOW_PS2M		1

#include <superio/winbond/w83627dhg/acpi/superio.asl>
