Device(FUR0) {
	Name(_HID,"AMD0020")
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
	Name(_HID,"AMD0020")
	Name(_UID,0x1)
	Name(_CRS, ResourceTemplate() {
			IRQ(Edge, ActiveHigh, Exclusive) {11}
			Memory32Fixed(ReadWrite, 0xFEDC8000, 0x2000)
	})
	Method (_STA, 0x0, NotSerialized) {
			Return (0x0F)
	}
}
