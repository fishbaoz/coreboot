
	.section ".imc", "a", @progbits
#if CONFIG_BOARD_AMD_GARDENIA
	.globl __imc_start
__imc_start:
        .long 0xf7000
        .long 0xfff08fff
#endif
	.section ".id", "a", @progbits
	 .globl __id_start
__id_start:
ver:
	.asciz COREBOOT_VERSION
vendor:
	.asciz CONFIG_MAINBOARD_VENDOR
part:
	.asciz CONFIG_MAINBOARD_PART_NUMBER
.long __id_end + CONFIG_ID_SECTION_OFFSET - ver  /* Reverse offset to the vendor id */
.long __id_end + CONFIG_ID_SECTION_OFFSET - vendor  /* Reverse offset to the vendor id */
.long __id_end + CONFIG_ID_SECTION_OFFSET - part    /* Reverse offset to the part number */
.long CONFIG_ROM_SIZE                               /* Size of this romimage */
	.globl __id_end
__id_end:


.previous
