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

