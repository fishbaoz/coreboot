##
## This file is part of the coreboot project.
##
## Copyright (C) 2007-2008 coresystems GmbH
## Copyright (C) 2010 Siemens AG, Inc.
## (Written by Josef Kellermann <joseph.kellermann@heitec.de> for Siemens AG, Inc.)
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc.
##

ramstage-$(CONFIG_PCI_OPTION_ROM_RUN_REALMODE) += int15_func.c
