Name(MWTT, Zero)
Scope(\_SB){
//**************** BTDC, "MWTT", I2C device validate tool, ASL update Variable Start ****************
//BTDC Comment Out    Name(MWTT, 1)  //template setting
    Scope(\_SB.FUR0)
	{
		Device(UART)
		{
			Name(_HID, "UTK0001")
			Name(_CID, "UARTTest")
			Name(_UID, Zero)
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x22)
				{
					0x8E, 0x1D, 0x00, 0x01, 0x00, 0x03, 0x02, 0x35,
					0x00, 0x01, 0x0A, 0x00, 0x00, 0xC2, 0x01, 0x00,
					0x20, 0x00, 0x20, 0x00, 0x00, 0xC0, 0x5C, 0x5F,
					0x53, 0x42, 0x2E, 0x46, 0x55, 0x52, 0x30, 0x00,
					0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
	}
	Scope(\_SB.FUR1)
	{
		Device(UART)
		{
			Name(_HID, "UTK0001")
			Name(_CID, "UARTTest")
			Name(_UID, One)
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x22)
				{
					0x8E, 0x1D, 0x00, 0x01, 0x00, 0x03, 0x02, 0x35,
					0x00, 0x01, 0x0A, 0x00, 0x00, 0xC2, 0x01, 0x00,
					0x20, 0x00, 0x20, 0x00, 0x00, 0xC0, 0x5C, 0x5F,
					0x53, 0x42, 0x2E, 0x46, 0x55, 0x52, 0x31, 0x00,
					0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
	}
	Scope(\_SB.I2CA)
	{
		Device(WTP1)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK0001")
			Name(_CID, "SPBTestKMDF")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xA0, 0x86, 0x01, 0x00,
					0x7F, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MTP1)
		{
			Name(_HID, "STK0001")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xA0, 0x86, 0x01, 0x00,
					0x11, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(WTP2)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK0002")
			Name(_CID, "SPBTestKMDF")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xA0, 0x86, 0x01, 0x00,
					0x11, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MTP2)
		{
			Name(_HID, "STK0002")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x80, 0x1A, 0x06, 0x00,
					0x12, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(WTP3)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK0003")
			Name(_CID, "SPBTestKMDF")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x80, 0x1A, 0x06, 0x00,
					0x12, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MTP3)
		{
			Name(_HID, "STK0003")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x40, 0x42, 0x0F, 0x00,
					0x13, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(WTP4)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK0004")
			Name(_CID, "SPBTestKMDF")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x40, 0x42, 0x0F, 0x00,
					0x13, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MTP4)
		{
			Name(_HID, "STK0004")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xE0, 0x22, 0x02, 0x00,
					0x14, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MTP5)
		{
			Name(_HID, "STK0005")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x20, 0x30, 0x05, 0x00,
					0x15, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x41, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
	}
	Scope(\_SB.I2CB)
	{
		Device(WT21)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK00012")
			Name(_CID, "SPBTestKMDF2")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xA0, 0x86, 0x01, 0x00,
					0x7F, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MT21)
		{
			Name(_HID, "STK00012")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xA0, 0x86, 0x01, 0x00,
					0x11, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(WT22)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK00022")
			Name(_CID, "SPBTestKMDF2")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xA0, 0x86, 0x01, 0x00,
					0x11, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MT22)
		{
			Name(_HID, "STK00022")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x80, 0x1A, 0x06, 0x00,
					0x12, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(WT23)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK00032")
			Name(_CID, "SPBTestKMDF2")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x80, 0x1A, 0x06, 0x00,
					0x12, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MT23)
		{
			Name(_HID, "STK00032")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x40, 0x42, 0x0F, 0x00,
					0x13, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(WT24)
		{
			Name(_ADR, Zero)
			Name(_HID, "STK00042")
			Name(_CID, "SPBTestKMDF2")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1E)
				{
					0x8E, 0x19, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x40, 0x42, 0x0F, 0x00,
					0x13, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x2E, 0x49,
					0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, One))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MT24)
		{
			Name(_HID, "STK00042")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0xE0, 0x22, 0x02, 0x00,
					0x14, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
		Device(MT25)
		{
			Name(_HID, "STK00052")
			Name(_CID, "WITTTest")
			Method(_CRS, 0, NotSerialized)
			{
				Name(RBUF, Buffer(0x1F)
				{
					0x8E, 0x1A, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00,
					0x00, 0x01, 0x06, 0x00, 0x20, 0x30, 0x05, 0x00,
					0x15, 0x00, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x2E,
					0x49, 0x32, 0x43, 0x42, 0x00, 0x79, 0x00
				})
				Return(RBUF)
			}
			Method(_STA, 0, NotSerialized)
			{
				If(LEqual(MWTT, Zero))
				{
					Return(0x0F)
				}
				Else
				{
					Return(Zero)
				}
			}
		}
	}
//**************** BTDC, "MWTT", I2C device validate tool, ASL update Variable End ****************
    /* //BTDC Comment out
	Scope(_SB.PCI0.SDIO)
	{
		Device(WLAN)
		{
			Name(_ADR, One)
			Name(_S4W, 0x02)
			Name(_S0W, 0x02)
			Method(_STA, 0, NotSerialized)
			{
				Return(0x0F)
			}
			Method(_RMV, 0, NotSerialized)
			{
				If(M2WL)
				{
					Return(Zero)
				}
				Else
				{
					Return(One)
				}
			}
			Method(_INI, 0, NotSerialized)
			{
				If(LGreaterEqual(BDID, One))
				{
					Store(One, GE17)
					Store(Zero, GV17)
					Sleep(0x14)
					Store(One, GV17)
				}
			}
			Method(_PS0, 0, Serialized)
			{
				If(LGreaterEqual(BDID, One))
				{
					Store(One, GV17)
				}
			}
			Method(_PS3, 0, Serialized)
			{
				If(LGreaterEqual(BDID, One))
				{
					Store(Zero, GV17)
				}
			}
		}
	}

	Scope(\_SB.FUR0)
	{
//BTDC MerlinFalcon board doesn't have BlueToolth device
		Device(BTH0)
		{
			Name(_HID, "BCM2E62")
            Method(_CRS, 0, NotSerialized)
         {
                Name (UBUF, ResourceTemplate ()
                {
                    UartSerialBus (0x0001C200, DataBitsEight, StopBitsOne,
                        0x00, LittleEndian, ParityTypeNone, FlowControlNone,
                        0x0010, 0x0010, "\\_SB.FUR0",
                        0x00, ResourceConsumer, ,
                        )
                    GpioInt (Edge, ActiveLow, Shared, PullDefault, 0x0000,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0012
                        }
                    GpioIo (Exclusive, PullDown, 0x0000, 0x0000, IoRestrictionNone,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x000E
                        }
                    GpioIo (Exclusive, PullDown, 0x0000, 0x0000, IoRestrictionOutputOnly,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x000B
                        }
                })
             Return(UBUF)
         }
         Method(_STA, 0, NotSerialized)
         {
          if (LEqual(UTDB, one)) {
            Return (0x00) //Disable this uart device in OS
          } else {
                 If(LGreaterEqual(OSVR, 15))
                 {
                     Return(0x0F)
                 }
                 Else
                 {
                     Return(Zero)
                 }
             }
         }
     }
//BTDC end for BlueToolth device
    }
    */
    Scope(\_SB.FUR1)
    {
/* //BTDC MerlinFalcon board doesn't have GPS device
     Device(GPS0)
     {
         Name(_HID, "BCM4752")
         Name(_HRV, Zero)
         Name(_STR, Buffer(0x26)
         {
             0x42, 0x00, 0x72, 0x00, 0x6F, 0x00, 0x61, 0x00,
             0x64, 0x00, 0x63, 0x00, 0x6F, 0x00, 0x6D, 0x00,
             0x20, 0x00, 0x47, 0x00, 0x4E, 0x00, 0x53, 0x00,
             0x53, 0x00, 0x20, 0x00, 0x34, 0x00, 0x37, 0x00,
             0x35, 0x00, 0x32, 0x00, 0x00, 0x00
         })
         Method(_CRS, 0, NotSerialized)
         {
                Name (UBUF, ResourceTemplate ()
                {
                    UartSerialBus (0x0001C200, DataBitsEight, StopBitsOne,
                        0xC0, LittleEndian, ParityTypeNone, FlowControlHardware,
                        0x0020, 0x0020, "\\_SB.FUR1",
                        0x00, ResourceConsumer, ,
                        )
                    GpioIo (Exclusive, PullDown, 0x0000, 0x0000, IoRestrictionNoneAndPreserve,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0046
                        }
                })
             Return(UBUF)
         }
         Method(_STA, 0, NotSerialized)
         {
              if (LEqual(UTDB, 2)) {
                Return (0x00) //Disable this uart device in OS
              } else {
                     If(LGreaterEqual(OSVR, 15))
                     {
                         Return(0x0F)
                     }
                     Else
                     {
                         Return(Zero)
                     }
              }
            }
        }
*/ //BTDC end
    }
    Scope(\_SB.I2CA)
    {
/* //BTDC MerlinFalcon board doesn't have NFC device
         Device(NFC1)
         {
         Name(_HID, EISAID("BCM2F25"))
         Method(_CRS, 0, NotSerialized)
         {
                Name (RBUF, ResourceTemplate ()
                {
                    I2cSerialBus (0x0077, ControllerInitiated, 0x000186A0,
                        AddressingMode7Bit, "\\_SB.I2CA",
                        0x00, ResourceConsumer, ,
                        )
                    GpioInt (Edge, ActiveHigh, Exclusive, PullNone, 0x0000,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x008B
                        }
                    GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0041
                        }
                    GpioIo (Exclusive, PullDefault, 0x0000, 0x0000, IoRestrictionOutputOnly,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0040
                        }
                })
             Return(RBUF)
         }
         Method(_STA, 0, NotSerialized)
         {
             If(LGreaterEqual(OSVR, 15))
             {
                 Return(0x0F)
             }
             Else
                {
                    Return(Zero)
                }
            }
        }
*/ //BTDC end
    }


    Scope (\_SB.I2CB)
    {
        Device (TPNL)
        {
            Name (_HID, EisaId ("PNP0C50"))  // _HID: Hardware ID
            Name (_CID, "PNP0C50")  // _CID: Compatible ID
            Method (_CRS, 0, NotSerialized)  // _CRS: Current Resource Settings
            {
                Name (RBUF, ResourceTemplate ()
                {
                    I2cSerialBus (0x004A, ControllerInitiated, 0x000F4240,
                        AddressingMode7Bit, "\\_SB.I2CB",
                        0x00, ResourceConsumer, ,
                        )
                    GpioInt (Level, ActiveLow, Shared, PullUp, 0x0000,
                        "\\_SB.GPIO", 0x00, ResourceConsumer, ,
                        )
                        {   // Pin list
                            0x0008
                        }
                })
                Return (RBUF)
            }

            Method (_STA, 0, NotSerialized)  // _STA: Status
            {
                        Return (0x0F)
            }

            Method (_DSW, 3, NotSerialized)  // _DSW: Device Sleep Wake
            {
                If (Arg0) {}
                Else
                {
                }
            }

            Method (_PS0, 0, NotSerialized)  // _PS0: Power State 0
            {
            }

            Method (_PS3, 0, NotSerialized)  // _PS3: Power State 3
            {
            }

            Method (_DSM, 4, Serialized)  // _DSM: Device-Specific Method
            {
                Name (_T_1, Zero)  // _T_x: Emitted by ASL Compiler
                Name (_T_0, Zero)  // _T_x: Emitted by ASL Compiler
                If (LEqual (Arg0, Buffer (0x10)
                        {
                            /* 0000 */   0xF7, 0xF6, 0xDF, 0x3C, 0x67, 0x42, 0x55, 0x45,
                            /* 0008 */   0xAD, 0x05, 0xB3, 0x0A, 0x3D, 0x89, 0x38, 0xDE
                        }))
                {
                    While (One)
                    {
                        Store (ToInteger (Arg2), _T_0)
                        If (LEqual (_T_0, Zero))
                        {
                            While (One)
                            {
                                Store (ToInteger (Arg1), _T_1)
                                If (LEqual (_T_1, One))
                                {
                                    Return (Buffer (One)
                                    {
                                         0x03
                                    })
                                }
                                Else
                                {
                                    Return (Buffer (One)
                                    {
                                         0x00
                                    })
                                }

                                Break
                            }
                        }
                        Else
                        {
                            If (LEqual (_T_0, One))
                            {
                                Return (Zero)
                            }
                            Else
                            {
                                Return (Zero)
                            }
                        }

                        Break
                    }
                }
                Else
                {
                    Return (Buffer (One)
                    {
                         0x00
                    })
                }
            }
        }
    }
}

