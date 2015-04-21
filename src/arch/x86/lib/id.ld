SECTIONS {
	/*imc start*/
	. = 0xFFFFFF00;
	.MEC . : {
	     *(.imc)
	}
	_imc_end = .;
	/*imc end*/
	. = (0xffffffff - CONFIG_ID_SECTION_OFFSET) - (__id_end - __id_start) + 1;
	.id (.): {
		*(.id)
	} = 0xFF
}
