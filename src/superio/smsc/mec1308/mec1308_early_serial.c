/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2017 GiPU Corp.  All rights reserved.
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


#include "mec1308.h"

static inline void mec1308_early_init(unsigned port)
{
	/* Enable SMSC UART 0 */
	device_t dev;

	dev = PNP_DEV (port, MEC1308_UART);
	pnp_set_logical_device(dev);
	pnp_set_enable(dev, 0);
	pnp_set_iobase(dev, PNP_IDX_IO0, CONFIG_TTYS0_BASE);
	pnp_set_enable(dev, 1);
}
