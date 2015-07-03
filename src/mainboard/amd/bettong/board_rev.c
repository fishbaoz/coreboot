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

#include <stdint.h>
#include <arch/io.h>
#include <arch/acpi.h>
#include <console/console.h>
#include <reset.h>
#include "board_rev.h"

char get_board_id(void)
{
	u32 gpiommioaddr;
	u8  value = 0;
	u8  boardrev = 0;
	char boardid = 'u';

	gpiommioaddr = 0xfed80000ul + 0x1500;
	value = *(volatile u8 *) (gpiommioaddr + (7 << 2) + 2); //agpio7 //board_id2
	boardrev = value & 1;
	value = *(volatile u8 *) (gpiommioaddr + (6 << 2) + 2); //agpio6 //board_id1
	boardrev |= (value & 1) << 1;
	value = *(volatile u8 *) (gpiommioaddr + (5 << 2) + 2); //agpio5 //board_id0
	boardrev |= (value & 1) << 2;

	boardid = 'A' + boardrev;

	return boardid;
}
