romstage-y += smbus.c smbus_spd.c
ramstage-y += hudson.c
ramstage-y += usb.c
ramstage-y += lpc.c
ramstage-y += sm.c
ramstage-y += ide.c
ramstage-y += sata.c
ramstage-y += hda.c
ramstage-y += pci.c
ramstage-y += pcie.c
ramstage-y += sd.c

ramstage-y += agesawrapper.c

ramstage-$(CONFIG_HAVE_ACPI_TABLES) += fadt.c
ramstage-y += reset.c
romstage-$(CONFIG_USBDEBUG_IN_ROMSTAGE) += enable_usbdebug.c
ramstage-$(CONFIG_USBDEBUG) += enable_usbdebug.c
romstage-y += early_setup.c

ramstage-$(CONFIG_SPI_FLASH) += spi.c
ramstage-y += resume.c ramtop.c

romstage-y += ramtop.c

romstage-y += imc.c
ramstage-y += imc.c

smm-$(CONFIG_HAVE_SMI_HANDLER) += smihandler.c smi_util.c
ramstage-$(CONFIG_HAVE_SMI_HANDLER) += smi.c smi_util.c

#ifeq ($(CONFIG_HUDSON_FWM), y)

# ROMSIG At ROMBASE + 0x20000:
# +-----------+---------------+----------------+------------+
# |0x55AA55AA |EC ROM Address |GEC ROM Address |USB3 ROM    |
# +-----------+---------------+----------------+------------+
# |PSPDIR ADDR|
# +-----------+
#
# EC ROM should be 64K aligned.

# HVB or Bypass or Static
CONFIG_HVB=HVB

HUDSON_FWM_POSITION=$(shell printf %u $(CONFIG_HUDSON_FWM_POSITION))

#assume the cbfs header is less than 128 bytes.
ROMSIG_SIZE=16
ifeq ($(CONFIG_HUDSON_XHCI_FWM), y)
HUDSON_XHCI_POSITION=$(call int-add,$(HUDSON_FWM_POSITION) $(ROMSIG_SIZE) 256)
XHCI_FWM_SIZE=$(call file-size,$(CONFIG_HUDSON_XHCI_FWM_FILE))
else
HUDSON_XHCI_POSITION=0
XHCI_FWM_SIZE=0
endif

ifeq ($(CONFIG_HUDSON_GEC_FWM), y)
HUDSON_GEC_POSITION=$(call int-add,$(HUDSON_FWM_POSITION) $(ROMSIG_SIZE) 128 \
	$(XHCI_FWM_SIZE) 128)
GEC_FWM_SIZE=$(call file-size,$(CONFIG_HUDSON_GEC_FWM_FILE))
else
HUDSON_GEC_POSITION=0
GEC_FWM_SIZE=0
endif

ifeq ($(CONFIG_HUDSON_IMC_FWM), y)
HUDSON_IMC_POSITION=$(call int-align,\
	$(call int-add,\
		$(HUDSON_FWM_POSITION) $(ROMSIG_SIZE) 128 $(XHCI_FWM_SIZE)\
		128 $(GEC_FWM_SIZE) 128),\
	65536)
else
HUDSON_IMC_POSITION=0
endif

HUDSON_PSP_DIRECTORY_POSITION=0
ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_ML), y)
#ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_COMBO), y)
#HUDSON_PSP_DIRECTORY_POSITION=$(shell printf %u 0xFFf80000)  #TODO: remove the hardcode.
HUDSON_PSP_DIRECTORY_POSITION=$(shell echo $(HUDSON_FWM_POSITION) 262144 |  awk '{printf("%.0f", $$1 + $$2)}')
#else
#HUDSON_PSP_DIRECTORY_POSITION=$(shell printf %u 0xFFb80000)  #TODO: remove the hardcode.
#endif
endif
#HUDSON_PSP_DIRECTORY_POSITION=$(shell printf %u 0xFFb00000)  #TODO: remove the hardcode.

ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_MLFP4), y)
HUDSON_PSP_DIRECTORY_POSITION=$(shell echo $(HUDSON_FWM_POSITION) 262144 |  awk '{printf("%.0f", $$1 + $$2)}')
endif

ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ), y)
HUDSON_PSP_DIRECTORY_POSITION=$(shell echo $(HUDSON_FWM_POSITION) 262144 |  awk '{printf("%.0f", $$1 + $$2)}')
endif


$(obj)/coreboot_hudson_romsig.bin: $(call strip_quotes, $(CONFIG_HUDSON_XHCI_FWM_FILE)) \
			$(call strip_quotes, $(CONFIG_HUDSON_IMC_FWM_FILE)) \
			$(call strip_quotes, $(CONFIG_HUDSON_GEC_FWM_FILE)) \
			$(obj)/config.h
	echo "    Hudson FW  $@"
	for fwm in 1437226410 \
		$(HUDSON_IMC_POSITION) \
		$(HUDSON_GEC_POSITION) \
		$(HUDSON_XHCI_POSITION) \
		$(HUDSON_PSP_DIRECTORY_POSITION); do \
		echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done > $@
	LC_ALL=C awk 'BEGIN {for (i=0; i<48; i++) {printf "%c", 255}}' >> $@

FIRMWARE=y
ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_KB), y)
ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_COMBO), y)
FIRMWARE=n
endif
endif

ifeq ($(FIRMWARE), y)
ifeq ($(CONFIG_HUDSON_FWM), y)
cbfs-files-y += hudson/fwm
hudson/fwm-file := $(obj)/coreboot_hudson_romsig.bin
hudson/fwm-position := $(HUDSON_FWM_POSITION)
hudson/fwm-type := raw
endif

ifeq ($(CONFIG_HUDSON_XHCI_FWM), y)
cbfs-files-y += hudson/xhci
hudson/xhci-file := $(call strip_quotes, $(CONFIG_HUDSON_XHCI_FWM_FILE))
hudson/xhci-position := $(HUDSON_XHCI_POSITION)
hudson/xhci-type := raw
hudson/xhci-required := Hudson XHCI firmware (available in coreboot/3rdparty if enabled)
endif

ifeq ($(CONFIG_HUDSON_IMC_FWM), y)
cbfs-files-y += hudson/imc
hudson/imc-file := $(call strip_quotes, $(CONFIG_HUDSON_IMC_FWM_FILE))
hudson/imc-position := $(HUDSON_IMC_POSITION)
hudson/imc-type := raw
hudson/imc-required := Hudson IMC Firmware (available in coreboot/3rdparty if enabled)
endif

ifeq ($(CONFIG_HUDSON_GEC_FWM), y)
cbfs-files-y += hudson/gec
hudson/gec-file := $(call strip_quotes, $(CONFIG_HUDSON_GEC_FWM_FILE))
hudson/gec-position := $(HUDSON_GEC_POSITION)
hudson/gec-type := raw
hudson/gec-required := Hudson Gigabit Ethernet Controller Firmware (Contact your AMD representative)
endif

ifdef CONFIG_HUDSON_AHCI_ROM
stripped_ahci_rom_id = $(call strip_quotes,$(CONFIG_AHCI_ROM_ID))
cbfs-files-y += pci$(stripped_ahci_rom_id).rom
pci$(stripped_ahci_rom_id).rom-file := $(call strip_quotes,$(CONFIG_AHCI_ROM_FILE))
pci$(stripped_ahci_rom_id).rom-type := optionrom
pci$(stripped_ahci_rom_id).rom-required := Hudson AHCI Option ROM (Contact your AMD representative)
endif
#endif #ifeq ($(CONFIG_HUDSON_FWM), y)
endif

ifeq ($(CONFIG_HUDSON_PSP), y)

# 0
# catenate the pubkey and pspdir together to save some space.
AMDPUBKEY_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x100)}')   #$(shell printf %u 0xFFb00100)
AMDPUBKEY_SIZE=$(word 5,$(shell ls -l $(CONFIG_AMD_PUBKEY_FILE)))
#cbfs-files-y += hudson/amdpubkey
#hudson/amdpubkey-file := $(call strip_quotes, $(CONFIG_AMD_PUBKEY_FILE))
#hudson/amdpubkey-position := $(AMDPUBKEY_POS)
#hudson/amdpubkey-type := raw

ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_ML), y)
FIRMWARE_LOCATE=f16ml/Firmwares/ML
FIRMWARE_TYPE=
else ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_MLFP4), y)
FIRMWARE_LOCATE=f16mlfp4/Firmwares/ML
FIRMWARE_TYPE=
else
FIRMWARE_LOCATE=f15cz/Firmwares/CZ
FIRMWARE_TYPE=CZ
endif
# 1
ifeq ($(CONFIG_HVB), Bypass)
CONFIG_PSPBTLDR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspBootLoader$(FIRMWARE_TYPE).Bypass.sbin
else ifeq ($(CONFIG_HVB), HVB)
CONFIG_PSPBTLDR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspBootLoader$(FIRMWARE_TYPE).HVB.sbin
else ifeq ($(CONFIG_HVB), Static)
CONFIG_PSPBTLDR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspBootLoader$(FIRMWARE_TYPE).HVB.sbin
endif
ifeq ($(FIRMWARE_TYPE), CZ)
CONFIG_PSPBTLDR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspBootLoader_prod_$(FIRMWARE_TYPE).sbin
endif
#CONFIG_PSPBTLDR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspBootLoader$(FIRMWARE_TYPE).Bypass.sbin
PSPBTLDR_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x1000)}') #$(shell printf %u 0xFFb10000)
PSPBTLDR_SIZE=$(word 5,$(shell ls -l $(CONFIG_PSPBTLDR_FILE)))
cbfs-files-y += hudson/pspbtldr
hudson/pspbtldr-file := $(CONFIG_PSPBTLDR_FILE)
hudson/pspbtldr-position := $(PSPBTLDR_POS)
hudson/pspbtldr-type := raw

#8
CONFIG_SMUFWM_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/SmuFirmware$(FIRMWARE_TYPE).sbin
SMUFWM_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0xb000)}') #$(shell printf %u 0xFFb20000)
SMUFWM_SIZE=$(word 5,$(shell ls -l $(CONFIG_SMUFWM_FILE)))
cbfs-files-y += hudson/smufwm
hudson/smufwm-file := $(CONFIG_SMUFWM_FILE)
hudson/smufwm-position := $(SMUFWM_POS)
hudson/smufwm-type := raw

#3
ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ), y)
CONFIG_PSPRCVR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspRecoveryBootLoader_prod_$(FIRMWARE_TYPE).sbin
else
CONFIG_PSPRCVR_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspRecovery$(FIRMWARE_TYPE).sbin
endif
PSPRCVR_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x30000)}') #$(shell printf %u 0xFFBb0000)
PSPRCVR_SIZE=$(word 5,$(shell ls -l $(CONFIG_PSPRCVR_FILE)))
cbfs-files-y += hudson/psprcvr
hudson/psprcvr-file := $(CONFIG_PSPRCVR_FILE)
hudson/psprcvr-position := $(PSPRCVR_POS)
hudson/psprcvr-type := raw

# 5
CONFIG_PUBSIGNEDKEY_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/RtmPubSigned$(FIRMWARE_TYPE).key
PUBSIGNEDKEY_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x400)}') #$(shell printf %u 0xFFb00400)
PUBSIGNEDKEY_SIZE=$(word 5,$(shell ls -l $(CONFIG_PUBSIGNEDKEY_FILE)))
cbfs-files-y += hudson/pubsignedkey
hudson/pubsignedkey-file := $(CONFIG_PUBSIGNEDKEY_FILE)
hudson/pubsignedkey-position := $(PUBSIGNEDKEY_POS)
hudson/pubsignedkey-type := raw

# 2
ifeq ($(FIRMWARE_TYPE), CZ)
CONFIG_PSPSCUREOS_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspSecureOs_prod_$(FIRMWARE_TYPE).csbin
else
CONFIG_PSPSCUREOS_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspSecureOs$(FIRMWARE_TYPE).sbin
endif
PSPSECUREOS_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x3A000)}') #$(shell printf %u 0xFFbc0000)
PSPSECUREOS_SIZE=$(word 5,$(shell ls -l $(CONFIG_PSPSCUREOS_FILE)))
cbfs-files-y += hudson/pspsecureos
hudson/pspsecureos-file := $(CONFIG_PSPSCUREOS_FILE)
hudson/pspsecureos-position := $(PSPSECUREOS_POS)
hudson/pspsecureos-type := raw

# 4
CONFIG_PSPNVRAM_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspNvram$(FIRMWARE_TYPE).bin
PSPNVRAM_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x6E000)}') #$(shell printf %u 0xFFbf0000)
PSPNVRAM_SIZE=$(word 5,$(shell ls -l $(CONFIG_PSPNVRAM_FILE)))
cbfs-files-y += hudson/pspnvram
hudson/pspnvram-file := $(CONFIG_PSPNVRAM_FILE)
hudson/pspnvram-position := $(PSPNVRAM_POS)
hudson/pspnvram-type := raw

ifeq ($(CONFIG_HVB), HVB)
# 6
RTM_FILE=$(objcbfs)/bootblock.bin     #The file size need to be 256 bytes aligned.
RTM_SIZE=$(word 5,$(shell ls -l $(RTM_FILE)))
RTM_POS=$(shell echo 4294967296 $(RTM_SIZE) | awk '{print $$1 - $$2}')

# 7
RTMSIGN_FILE=$(obj)/bootblock_sig.bin
RTMSIGN_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x800)}') #$(shell printf %u 0xFFb00800)
RTMSIGN_SIZE=256 #it should be hardcoded to 256, otherwise circular dependency comes up.$(word 5,$(shell ls -l $(RTMSIGN_FILE)))
cbfs-files-y += hudson/rtmsign
hudson/rtmsign-file := $(RTMSIGN_FILE)
hudson/rtmsign-position := $(RTMSIGN_POS)
hudson/rtmsign-type := raw

else ifeq ($(CONFIG_HVB), Static)
RTM_FILE=$(top)/bootblock.bin     #The file size need to be 256 bytes aligned.
RTM_SIZE=$(word 5,$(shell ls -l $(RTM_FILE)))
RTM_POS=$(shell echo 4294967296 $(RTM_SIZE) | awk '{print $$1 - $$2}')

# 7
RTMSIGN_FILE=$(top)/bootblock_sig.bin
RTMSIGN_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x800)}') #$(shell printf %u 0xFFb00800)
RTMSIGN_SIZE=256 #it should be hardcoded to 256, otherwise circular dependency comes up.$(word 5,$(shell ls -l $(RTMSIGN_FILE)))
cbfs-files-y += hudson/rtmsign
hudson/rtmsign-file := $(RTMSIGN_FILE)
hudson/rtmsign-position := $(RTMSIGN_POS)
hudson/rtmsign-type := raw

$(objcbfs)/bootblock.bin: $(top)/bootblock.bin
	cp $(top)/bootblock.bin $@

endif

CONFIG_SMUSCS_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/SmuScs$(FIRMWARE_TYPE).bin
SMUSCS_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x7F000)}') #$(shell printf %u 0xFFC00000)
SMUSCS_SIZE=$(word 5,$(shell ls -l $(CONFIG_SMUSCS_FILE)))
cbfs-files-y += hudson/smuscs
hudson/smuscs-file := $(CONFIG_SMUSCS_FILE)
hudson/smuscs-position := $(SMUSCS_POS)
hudson/smuscs-type := raw

ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ), y)
CONFIG_SECUREDEBUGKEY_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspSecureDebug$(FIRMWARE_TYPE).Key
SECUREDEBUGKEY_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x7FD00)}') #$(shell printf %u 0xFFC00000)
SECUREDEBUGKEY_SIZE=$(word 5,$(shell ls -l $(CONFIG_SECUREDEBUGKEY_FILE)))
cbfs-files-y += hudson/securedebugkey
hudson/securedebugkey-file := $(CONFIG_SECUREDEBUGKEY_FILE)
hudson/securedebugkey-position := $(SECUREDEBUGKEY_POS)
hudson/securedebugkey-type := raw

CONFIG_SMUFWM2_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/SmuFirmware2_prod_$(FIRMWARE_TYPE).sbin
SMUFWM2_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x80200)}') #$(shell printf %u 0xFFC00000)
SMUFWM2_SIZE=$(word 5,$(shell ls -l $(CONFIG_SMUFWM2_FILE)))
cbfs-files-y += hudson/smufwm2
hudson/smufwm2-file := $(CONFIG_SMUFWM2_FILE)
hudson/smufwm2-position := $(SMUFWM2_POS)
hudson/smufwm2-type := raw

endif

## 19
#CONFIG_PSP_EARLY_UNLOCK_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspEarlyUnlock.sbin
#PSP_EARLY_UNLOCK_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x76000)}') #$(shell printf %u 0xFFC00000)
#PSP_EARLY_UNLOCK_SIZE=$(word 5,$(shell ls -l $(CONFIG_PSP_EARLY_UNLOCK_FILE)))
#cbfs-files-y += hudson/psp_early_unlock
#hudson/psp_early_unlock-file := $(CONFIG_PSP_EARLY_UNLOCK_FILE)
#hudson/psp_early_unlock-position := $(PSP_EARLY_UNLOCK_POS)
#hudson/psp_early_unlock-type := raw
#
# 12

ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ), y)
CONFIG_TRUSTLET_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspTrustlets_prod_$(FIRMWARE_TYPE).cbin
TRUSTLET_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x86000)}') #$(shell printf %u 0xFFC00000)
TRUSTLET_SIZE=$(word 5,$(shell ls -l $(CONFIG_TRUSTLET_FILE)))
cbfs-files-y += hudson/trustlet
hudson/trustlet-file := $(CONFIG_TRUSTLET_FILE)
hudson/trustlet-position := $(TRUSTLET_POS)
hudson/trustlet-type := raw

# 13
CONFIG_TRUSTLET_KEY_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/TrustletKey_prod_$(FIRMWARE_TYPE).sbin
TRUSTLET_KEY_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0x9D000)}') #$(shell printf %u 0xFFC00000)
TRUSTLET_KEY_SIZE=$(word 5,$(shell ls -l $(CONFIG_TRUSTLET_KEY_FILE)))
cbfs-files-y += hudson/trustlet_key
hudson/trustlet_key-file := $(CONFIG_TRUSTLET_KEY_FILE)
hudson/trustlet_key-position := $(TRUSTLET_KEY_POS)
hudson/trustlet_key-type := raw
endif

#CONFIG_PSPAGESAS3_FILE=$(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/PspAgesaS3Resume_prod_$(FIRMWARE_TYPE).csbin
#PSPAGESAS3_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", $$1 + 0xA2000)}') #$(shell printf %u 0xFFC00000)
#PSPAGESAS3_SIZE=$(word 5,$(shell ls -l $(CONFIG_PSPAGESAS3_FILE)))
#cbfs-files-y += hudson/pspagesas3
#hudson/pspagesas3-file := $(CONFIG_PSPAGESAS3_FILE)
#hudson/pspagesas3-position := $(PSPAGESAS3_POS)
#hudson/pspagesas3-type := raw


#ifeq ($(CONFIG_BLOB_SIGN), y)
#FINAL_BIN_SIZE=$(BLOB_SIZE)
#FINAL_BIN_POS=$(BLOB_POS)
#FINAL_SIGN_SIZE=$(BLOBSIGN_SIZE)
#FINAL_SIGN_POS=$(BLOBSIGN_POS)
#else
#FINAL_BIN_SIZE=$(RTM_SIZE)
#FINAL_BIN_POS=$(RTM_POS)
#FINAL_SIGN_SIZE=$(RTMSIGN_SIZE)
#FINAL_SIGN_POS=$(RTMSIGN_POS)
#endif

define output_hex
echo $(1) | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'
endef

ifeq ($(CONFIG_HVB), HVB)
$(RTMSIGN_FILE): $(RTM_FILE) $(obj)/coreboot_psp_directory.bin #$(REVERSESTR)
	echo "    Sign RTM   $@"
	echo $(RTM_SIZE) | LC_ALL=C awk '{d=4294967295-$$1+1-32; printf("%c%c%c%c", d % 256, int(d/256) % 256, int(d/65536) % 256, int(d/16777216))};' > $(obj)/bootblock_tail.bin
	dd if=$(RTM_FILE) of=$(obj)/bootblock_cut.bin bs=1 count=`echo $(RTM_SIZE) | awk '{printf("%d", $$1 - 4);}'` >/dev/null 2>/dev/null
	cat $(obj)/bootblock_cut.bin $(obj)/bootblock_tail.bin $(obj)/coreboot_psp_directory.bin > $(obj)/bootblock_pspdir_combine.bin
ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_ML), y)
#	touch authorization ; echo "-sp_username xxxx -sp_password xxxxxx" > authorization
	cd $(top)/util/SPClient; sh ./SPClient.sh -in ../../$(obj)/bootblock_pspdir_combine.bin -out ../../$@ `cat authorization 2>/dev/null`
else ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY16_MLFP4), y)
	cd $(top)/util/SPClient; sh ./SPClient.sh -in ../../$(obj)/bootblock_pspdir_combine.bin -out ../../$@ `cat authorization 2>/dev/null`
else ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ), y)
	cd $(top)/util/SPClient; sh ./SPClient_ATL.sh -in ../../$(obj)/bootblock_pspdir_combine.bin -out ../../$@ `cat authorization 2>/dev/null`
endif
#	openssl dgst -binary -sign $(top)/SPClient/testPrivateKey.key -sha256 -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-2 -out $(obj)/bootblock_sig_unrev.bin $(obj)/bootblock_pspdir_combine.bin
#	$(REVERSESTR) $(obj)/bootblock_sig_unrev.bin $(obj)/bootblock_sig.bin
	rm $(obj)/bootblock_tail.bin $(obj)/bootblock_cut.bin $(obj)/bootblock_pspdir_combine.bin
#	rm $(obj)/bootblock_sig_unrev.bin
endif

$(obj)/coreboot_psp_directory.bin: $(obj)/config.h $(FLETCHER) $(RTM_FILE)
	echo "    PSPDir     $@"
	for fwm in 0 $(AMDPUBKEY_SIZE) $(AMDPUBKEY_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done > $@_tail.tmp
	for fwm in 1 $(PSPBTLDR_SIZE) $(PSPBTLDR_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 8 $(SMUFWM_SIZE) $(SMUFWM_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 3 $(PSPRCVR_SIZE) $(PSPRCVR_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 5 $(PUBSIGNEDKEY_SIZE) $(PUBSIGNEDKEY_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
ifneq ($(CONFIG_HVB), Bypass)
	for fwm in 6 $(RTM_SIZE) $(RTM_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 7 $(RTMSIGN_SIZE) $(RTMSIGN_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
endif
	for fwm in 2 $(PSPSECUREOS_SIZE) $(PSPSECUREOS_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 4 $(PSPNVRAM_SIZE) $(PSPNVRAM_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
#	for fwm in 19 $(PSP_EARLY_UNLOCK_SIZE) $(PSP_EARLY_UNLOCK_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
ifeq ($(CONFIG_CPU_AMD_AGESA_FAMILY15_CZ), y)
	for fwm in 12 $(TRUSTLET_SIZE) $(TRUSTLET_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 13 $(TRUSTLET_KEY_SIZE) $(TRUSTLET_KEY_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
endif
	for fwm in 9 $(SECUREDEBUGKEY_SIZE) $(SECUREDEBUGKEY_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 95 $(SMUSCS_SIZE) $(SMUSCS_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 11 4294967295 1 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
#	for fwm in 16 $(PSPAGESAS3_SIZE) $(PSPAGESAS3_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in 18 $(SMUFWM2_SIZE) $(SMUFWM2_POS) 0; do \
	echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done >> $@_tail.tmp
	for fwm in  1347637284 0 `ls -l $@_tail.tmp | awk '{printf("%d", $$5/16);}'` 0; do \
		echo  $$fwm | LC_ALL=C awk '{printf ("%c%c%c%c", $$1 % 256, int($$1/256) % 256, int($$1/65536) % 256, int($$1/16777216));}'; \
	done > $@_head.tmp
	cat $@_head.tmp $@_tail.tmp > $@.tmp
	$(FLETCHER) < $@.tmp > $@
	rm $@_head.tmp $@_tail.tmp $@.tmp

$(obj)/coreboot_psp_directory_combine_pubkey.bin: $(obj)/coreboot_psp_directory.bin
	cat  $(obj)/coreboot_psp_directory.bin > $@
	ls -l $(obj)/coreboot_psp_directory.bin | LC_ALL=C awk '{for (i=0; i<256-$$5; i++) {printf "%c", 255}}' >> $@
	cat  $(top)/src/vendorcode/amd/agesa/$(FIRMWARE_LOCATE)/AmdPubKey$(FIRMWARE_TYPE).bin >> $@

cbfs-files-y += hudson/pspdir
hudson/pspdir-file := $(obj)/coreboot_psp_directory_combine_pubkey.bin
hudson/pspdir-position := $(HUDSON_PSP_DIRECTORY_POSITION)
hudson/pspdir-type := raw

endif

ifeq ($(CONFIG_BOARD_AMD_GARDENIA), y)
ECFW_POS=$(shell echo $(HUDSON_PSP_DIRECTORY_POSITION) | awk '{printf("%.0f", 0xFF800000 + 0xf7000)}')
cbfs-files-y += hudson/ECFW
hudson/ECFW-file := $(top)/GardenEC.bin
hudson/ECFW-position := $(ECFW_POS)
hudson/ECFW-type := raw
endif
