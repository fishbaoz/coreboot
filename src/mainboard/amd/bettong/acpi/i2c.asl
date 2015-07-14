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
