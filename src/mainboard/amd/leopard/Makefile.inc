#
# This file is part of the coreboot project.
#
# Copyright (C) 2015 Advanced Micro Devices, Inc.
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

romstage-y += BiosCallOuts.c
romstage-y += OemCustomize.c
romstage-y += boardid.c
romstage-y += anx.c

ramstage-y += BiosCallOuts.c
ramstage-y += OemCustomize.c
ifeq ($(CONFIG_HUDSON_IMC_FWM), y)
ramstage-y += fchec.c
endif
ramstage-y += boardid.c

ramstage-y += oem_auth.c
# 1. Use coreboot-obmm-20151103-novalidation-readmac.rom to read MAC address.
# 2. Pick a new BOARDNUM.
# 3. Define WRITE_OTP as 0, build a coreboot.rom and run.
# 4. Define WRITE_OTP as 1, build a coreboot.rom which is the image for the board.
#SHA512SUM=$(if $(shell type sha512sum 2>/dev/null), sha512sum, sha512)
#ENCRIPTION=$(shell echo boardnum$(BOARDNUM) | $(SHA512SUM)  | grep -o "[0-9a-f]*" | sed -e 's/\([0-9a-f]\{2\}\)/\1 /g')

# Bettong 00B
BOARDNUM=$(shell printf %d 0xBB)
MACADDRESS=FF-FF-FF-FF-FF-FF
#ENCRIPTION="00 00 00 00 00 00 00 00 00 00 00 0A 00 04 00 00 20 00 20 20 02 18 2D 0A 6D 53 58 55 9E 9B C1 A9 38 98 9C 68 13 F0 92 5E CA B0 27 CA 65 E5 11 72 69 30 5C C2 16 E4 C8 37 9D 03 35 F5 62 43 35 68"
#ENCRIPTION="FE EF FB BF F7 7F DF FD FF FF FF FF 42 46 31 30 FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF"
#ENCRIPTION="FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF"
#Unique ID
#ENCRIPTION = $(call strip_quotes, "5f 5f 46 4d 41 50 5f 5f")

ENCRIPTION = $(call strip_quotes, "D8 63 38 06 43 54 29 38")  # vxworks only mac = 60
ENCRIPTION += $(call strip_quotes, "D8 65 50 34 DF 59 20 2A") # mac = 63
ENCRIPTION += $(call strip_quotes, "D8 65 50 34 DF 26 3B 22") # mac = 61
ENCRIPTION += $(call strip_quotes, "D8 65 50 34 DF 2E 15 2C") # mac = 62


# End of Encription
ENCRIPTION+= $(call strip_quotes, "FF FF FF FF FF FF FF FF")


#################################################
MACADDRESS_RECORD=$(shell echo $(MACADDRESS) | sed -e 's/-/ /g')

#Extract valiation.bin from final image
#./build/cbfstool build/coreboot.rom  extract -n olivehill/validation -f validation_read.bin
$(obj)/otp.bin : $(src)/mainboard/$(MAINBOARDDIR)/Makefile.inc
	for c in $(call strip_quotes,$(ENCRIPTION)) ; do \
		printf %d 0x$$c | LC_ALL=C awk '{printf ("%c", compl($$1));}'; \
	done >> $@.tmp
	ls -l $@.tmp | LC_ALL=C awk '{ if($$5<64){for (i=$$5; i<64; i++) {printf "%c", 255}}}' >> $@.tmp
	mv $@.tmp $@

$(obj)/otp.sh: $(src)/mainboard/$(MAINBOARDDIR)/Makefile.inc
	echo $(ENCRIPTION) | LC_ALL=C  awk ' \
		{i=1; printf("\n"); \
		 while (i <= NF) { \
			if ((i % 16)==1) printf("dpcmd --raw-instruction \"B1|06|02 00 00 %02d", i-1); \
			printf(" %s", $$i); \
			if ((i % 16)==0 || (i==NF)) printf("|04|C1\"\n"); \
			i++; \
		 } \
		}' >> $@.tmp
	mv $@.tmp $@

$(obj)/validation.bin :$(src)/mainboard/$(MAINBOARDDIR)/Makefile.inc $(obj)/otp.bin $(obj)/otp.sh
	echo "    VALIDATION $@"
	cp $(obj)/otp.bin $@.tmp
	for c in $(MACADDRESS_RECORD) ; do \
		printf %d 0x$$c | LC_ALL=C awk '{printf ("%c", $$1);}'; \
	done >> $@.tmp
	LC_ALL=C awk 'BEGIN {for (i=0;i<8;i++){printf ("%c", 255);}}' >> $@.tmp
	echo $(BOARDNUM) | LC_ALL=C awk '{printf ("%c%c", $$1 % 256, int($$1/256) % 256);}' >> $@.tmp
	mv $@.tmp $@
#	cp $(src)/mainboard/$(MAINBOARDDIR)/validation_set.bin $@

cbfs-files-y += leopard/validation
leopard/validation-file := $(obj)/validation.bin
#leopard/validation-position := 0xFFC0F200
leopard/validation-type := raw
