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



ramstage-y += oem_auth.c

# 1. Use coreboot-obmm-20151103-novalidation-readmac.rom to read MAC address.
# 2. Pick a new BOARDNUM.
# 3. Define WRITE_OTP as 0, build a coreboot.rom and run.
# 4. Define WRITE_OTP as 1, build a coreboot.rom which is the image for the board.

#BOARDNUM=1  #Used 36
#MACADDRESS=00-1b-21-b3-a9-63

#BOARDNUM=2   #Used 104
#MACADDRESS=be-9b-1d-38-22-f3

BOARDNUM=$(shell printf %d 0x88)
MACADDRESS=00-90-27-e0-00-d2
#OTP=
#00000000  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
#*
#00000020  ff 01 ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|
#00000030  ff ff ff ff ff ff ff ff  ff ff ff ff ff ff ff ff  |................|

BOARDNUM=$(shell printf %d 0x96)
MACADDRESS=00-90-27-e0-00-d3



SHA512SUM=$(if $(shell type sha512sum 2>/dev/null), sha512sum, sha512)
ENCRIPTION=$(shell echo boardnum$(BOARDNUM) | $(SHA512SUM)  | grep -o "[0-9a-f]*" | sed -e 's/\([0-9a-f]\{2\}\)/\1 /g')
ENCRIPTION+=$(shell echo $(MACADDRESS) | sed -e 's/-/ /g')

#Extract valiation.bin from final image
#./build/cbfstool build/coreboot.rom  extract -n olivehill/validation -f validation_read.bin
$(obj)/validation.bin :$(src)/mainboard/$(MAINBOARDDIR)/Makefile.inc
	echo "    VALIDATION $@"
	for c in $(ENCRIPTION) ; do \
		printf %d 0x$$c | LC_ALL=C awk '{printf ("%c", $$1);}'; \
	done >> $@.tmp
	LC_ALL=C awk 'BEGIN {for (i=0;i<8;i++){printf ("%c", 255);}}' >> $@.tmp
	echo $(BOARDNUM) | LC_ALL=C awk '{printf ("%c%c", $$1 % 256, int($$1/256) % 256);}' >> $@.tmp
	mv $@.tmp $@
#	cp $(src)/mainboard/$(MAINBOARDDIR)/validation_set.bin $@

cbfs-files-y += olivehill/validation
olivehill/validation-file := $(obj)/validation.bin
olivehill/validation-position := 0xFFC0F200
olivehill/validation-type := raw
