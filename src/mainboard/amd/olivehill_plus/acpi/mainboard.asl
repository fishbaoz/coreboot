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

/* Memory related values */
Name(LOMH, 0x0)	/* Start of unused memory in C0000-E0000 range */
Name(PBAD, 0x0)	/* Address of BIOS area (If TOM2 != 0, Addr >> 16) */
Name(PBLN, 0x0)	/* Length of BIOS area */

Name(PCBA, CONFIG_MMCONF_BASE_ADDRESS)	/* Base address of PCIe config space */
Name(PCLN, Multiply(0x100000, CONFIG_MMCONF_BUS_NUMBER)) /* Length of PCIe config space, 1MB each bus */
Name(HPBA, 0xFED00000)	/* Base address of HPET table */

Name(SSFG, 0x0D)		/* S1 support: bit 0, S2 Support: bit 1, etc. S0 & S5 assumed */

/* Some global data */
Name(OSVR, 3)	/* Assume nothing. WinXp = 1, Vista = 2, Linux = 3, WinCE = 4 */
Name(OSV, Ones)	/* Assume nothing */
Name(PMOD, One)	/* Assume APIC */

/* AcpiGpe0Blk */
OperationRegion(GP0B, SystemMemory, 0xfed80814, 0x04)
	Field(GP0B, ByteAcc, NoLock, Preserve) {
	, 11,
	USBS, 1,
}

/*  */
OperationRegion(PSPC, SystemMemory, 0xf0a00070, 0x04)
    Field(PSPC, ByteAcc, NoLock, Preserve) {
    PSPM, 32,
}

/*  */
OperationRegion(A0AC, SystemMemory, 0xFED81E94, 0x04)
    Field(PSPC, ByteAcc, NoLock, Preserve) {
//    , 14,
//    ACCL, 1,
//    ACCI, 1,
    ACCI, 16,
}
OperationRegion(APCR, SystemMemory, 0xfec00000, 0x00000020)
		Field(APCR, ByteAcc, NoLock, Preserve) {
		APCI, 0x00000008,
		, 8,
		, 8,
		, 8,
		, 16,
		APCM, 0x00000001,
}
//IndexField (APCI, APCD, ByteAcc, NoLock, Preserve) {
//		Offset(0x10),
//		, 16, AMK1, 1,
////		Offset(0x12),
//		Offset(0x14),
//		Offset(0x16),
//		Offset(0x18),
//		Offset(0x1a),
//		Offset(0x1c),
//		Offset(0x1e),
//
//		Offset(0x20),
//		Offset(0x22),
//		Offset(0x24),
//		Offset(0x26),
//		Offset(0x28),
//		Offset(0x2a),
//		Offset(0x2c),
//		Offset(0x2e),
//
//		Offset(0x30),
//		Offset(0x32),
//		Offset(0x34),
//		Offset(0x36),
//		Offset(0x38),
//		Offset(0x3a),
//		Offset(0x3c),
//		Offset(0x3e),
//}
