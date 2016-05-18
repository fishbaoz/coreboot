#
# This file is part of the coreboot project.
#
# Copyright (C) 2012 Advanced Micro Devices, Inc.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#

romstage-y += buildOpts.c
romstage-y += BiosCallOuts.c
romstage-y += PlatformGnbPcie.c

ramstage-y += buildOpts.c
ramstage-y += BiosCallOuts.c
ramstage-y += PlatformGnbPcie.c

cbfs-files-y += bootorder
bootorder-file := bootorder
bootorder-type := raw

cbfs-files-y += etc/boot-menu-message
etc/boot-menu-message-file := boot-menu-message
etc/boot-menu-message-type := raw

$(obj)/boot-menu-key:
	LC_ALL=C awk 'BEGIN{printf("%c", 0x86);}' > $@

cbfs-files-y += etc/boot-menu-key
etc/boot-menu-key-file := $(obj)/boot-menu-key
etc/boot-menu-key-type := raw
