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
 */

OperationRegion( SMIC, SystemMemory, 0xFED80000, 0x800000)
Field( SMIC, ByteAcc, NoLock, Preserve) {
    offset (0x0296),
    STMR, 15, //SMI Timer Count
#if 0
    TMRE, 1,  //SMI Timer Enable

    offset (0x0298),
    , 29, //
    TMRS, 1,  //SMI Timer select

    offset (0x036A),
    SMIB, 8,

    offset (0x03DA),
    , 4,
    RFCS, 2,  //ReFClockSel

    offset (0x03DC),
    , 30,
    SM30, 1,

    offset (0x03EE),
    U3PS, 2,  //Usb3PowerSel

    offset (0x0E28),
    ,29 ,
    SARP, 1,   //Sata Ref Clock Powerdown
    U2RP, 1,   //Usb2 Ref Clock Powerdown
    U3RP, 1,   //Usb3 Ref Clock Powerdown

//AGPIO6 for DevSlp on Gardenia board
    offset (0x1518),
    ,22 ,
    G06D, 1,   //Data
    G06E, 1,   //Enable

    offset (0x152C),
    ,22 ,
    G0BD, 1,   //Data
    G0BE, 1,   //Enable

//AGPI25 for SDCD
    offset (0x1564),
    ,16 ,
    G19S,1 ,
    ,5 ,
    G19D, 1,   //Data
    G19E, 1,   //Enable

    offset (0x1C00),
    XH0E, 1,
    ,6,
    U3PY, 1,
    , 7,
    UD3P, 1, //15
    U3PR, 1, //16
    , 11,
    FWLM, 1,  //FirmWare Load Mode
    FPLS, 1,  //Fw PreLoad Start
    FPLC, 1,  //Fw PreLoad Complete

    offset (0x1C04),
    UA04, 16,
    , 15,
    ROAM, 1,  //1= ROM 0=RAM

    offset (0x1C08),
    UA08, 32,

    offset (0x1E4A),
    I0TD, 2,
    , 1,
    I0PD, 1,
    offset (0x1E4B),
    I0DS, 3,

    offset (0x1E4C),
    I1TD, 2,
    , 1,
    I1PD, 1,
    offset (0x1E4D),
    I1DS, 3,

    offset (0x1E4E),
    I2TD, 2,
    , 1,
    I2PD, 1,
    offset (0x1E4F),
    I2DS, 3,

    offset (0x1E50),
    I3TD, 2,
    , 1,
    I3PD, 1,
    offset (0x1E51),
    I3DS, 3,

    offset (0x1E56),
    U0TD, 2,
    , 1,
    U0PD, 1,
    offset (0x1E57),
    U0DS, 3,

    offset (0x1E58),
    U1TD, 2,
    , 1,
    U1PD, 1,
    offset (0x1E59),
    U1DS, 3,

    offset (0x1E5E),
    SATD, 2,
    , 1,
    SAPD, 1,
    offset (0x1E5F),
    SADS, 3,

    offset (0x1E64),
    U2TD, 2,
    , 1,
    U2PD, 1,
    offset (0x1E65),
    U2DS, 3,

    offset (0x1E6E),
    U3TD, 2,
    , 1,
    U3PD, 1,
    offset (0x1E6F),
    U3DS, 3,

    offset (0x1E70),
    SDTD, 2,
    , 1,
    SDPD, 1,
    , 2,
    SDRT, 1,
    SDSC, 1,

    offset (0x1E71),
    SDDS, 3,

    offset (0x1E80),
    , 15,
    RQ15, 1,
    , 2,
    RQ18, 1,
    , 4,
    RQ23, 1,
    RQ24, 1,
    , 5,
    RQTY, 1,
    //SRRR, 32, //Shadow Reg Request Register
    offset (0x1E84),
    , 15,
    SASR, 1,  //SATA 15 Shadow Reg Request Status Register
    , 2,
    U2SR, 1,  //USB2 18 Shadow Reg Request Status Register
    , 4,
    U3SR, 1,  //USB3 23 Shadow Reg Request Status Register
    SDSR, 1,  //SD 24 Shadow Reg Request Status Register
    offset (0x1E88),
    SRSA, 32, //Shadow Reg SRAM Addr
    SRSD, 32, //Shadow Reg SRAM DATA

    offset (0x1E94), //S0i3
    SRDY, 1,         //S0i3 bios ready

    offset (0x1EA0), //Power Good Control
    PG1A, 1,
    PG2_, 1,
    ,1,
    U3PG, 1,  //Usb3 Power Good BIT3
    offset (0x1EA3), //Power Good Control
    PGA3, 8 ,
    //, 4,
    //SAPD, 1,  //SwSataLockPhyIF BIT28
    //U3SS, 1,  //Usb3SlpShutdown BIT29
    //U2SS, 1,  //Usb2SlpShutdown BIT30

    offset (400010), //AL2AHBx10 Control
    , 1,
    CCGE, 1,
    offset (400030), //AL2AHBx30 HCLK Control
    , 1,
    HCGE, 1,
#endif
}

Device(GPIO) {
	Name (_HID, "AMD0030")
	Name (_CID, "AMD0030")
	Name(_UID, 0)

	Method (_CRS, 0x0, NotSerialized) {
		Name (RBUF, ResourceTemplate () {
			//
			// Interrupt resource. In this example, banks 0 & 1 share the same
			// interrupt to the parent controller and similarly banks 2 & 3.
			//
			// N.B. The definition below is chosen for an arbitrary
			//      test platform. It needs to be changed to reflect the hardware
			//      configuration of the actual platform
			//
			Interrupt(ResourceConsumer, Level, ActiveLow, Shared, , , ) {7}

			//
			// Memory resource. The definition below is chosen for an arbitrary
			// test platform. It needs to be changed to reflect the hardware
			// configuration of the actual platform.
			//
			Memory32Fixed(ReadWrite, 0xFED81500, 0x300)
		})

		Return (RBUF)
	}

	Method (_STA, 0x0, NotSerialized) {
			Return (0x0F)
	}
}

Device(FUR0) {
//	Name(_HID,"AMD0020")
	Name(_HID,"PNP0501")
	Name(_UID,0x0)
	Name(_CRS, ResourceTemplate() {
		IRQ(Edge, ActiveHigh, Exclusive) {10}
		Memory32Fixed(ReadWrite, 0xFEDC6000, 0x2000)
	})
	Method (_STA, 0x0, NotSerialized) {
		Return (0x0F)
	}
}

Device(FUR1) {
//	Name(_HID,"AMD0020")
	Name(_HID,"PNP0501")
	Name(_UID,0x1)
	Name(_CRS, ResourceTemplate() {
			IRQ(Edge, ActiveHigh, Exclusive) {11}
			Memory32Fixed(ReadWrite, 0xFEDC8000, 0x2000)
	})
	Method (_STA, 0x0, NotSerialized) {
			Return (0x0F)
	}
}

Device(I2CA) {
	Name(_HID,"AMD0010")
	Name(_UID,0x0)
	Name(_CRS, ResourceTemplate() {
		IRQ(Edge, ActiveHigh, Exclusive) {3}
		Memory32Fixed(ReadWrite, 0xFEDC2000, 0x1000)
	})

	Method (_STA, 0x0, NotSerialized) {
		Return (0x0F)
	}
}

Device(I2CB)
{
	Name(_HID,"AMD0010")
	Name(_UID,0x1)
	Name(_CRS, ResourceTemplate() {
		IRQ(Edge, ActiveHigh, Exclusive) {15}
		Memory32Fixed(ReadWrite, 0xFEDC3000, 0x1000)
	})
	Method (_STA, 0x0, NotSerialized) {
		Return (0x0F)
	}
}
