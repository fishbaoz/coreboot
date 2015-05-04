OperationRegion( SMIC, SystemMemory, 0xFED80000, 0x800000)
Field( SMIC, ByteAcc, NoLock, Preserve) {
    offset (0x0296),
    STMR, 15, //SMI Timer Count  
    TMRE, 1,  //SMI Timer Enable 

    offset (0x0298),
    , 29, // 
    TMRS, 1,  //SMI Timer select 

    offset (0x0360),
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

//AGPI25 for SDCD
    offset (0x1564),
    ,16 ,
    G19S,1 ,
    ,5 ,
    G19D, 1,   //Data
    G19E, 1,   //Enable

    offset (0x1C00),
    ,7,
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

}

Mutex (FDAS, 0) //FCH Device Aoac Semophore
  
Method(FDDC, 2, Serialized) //FCH Device D3Cold Control Arg0:device Arg1:D state
{ 
  Acquire (FDAS, 0xFFFF)
  if (LEqual(Arg1, 0)) {
    //Turn on Power
    //if (LEqual (CCGE, zero)) {Store (one, CCGE)}
    //if (LEqual (HCGE, zero)) {Store (one, HCGE)}
    Switch (ToInteger(Arg0))
    {
      Case (Package () {5, 15, 24}) { Store ( one, PG1A) }
      Case (Package () {6, 7, 8, 11, 12, 18}) { Store ( one, PG2_) }
      Case (Package () {5, 15, 24,     6, 7, 8, 11, 12, 18,    23   }) { Store (zero, SRDY) } //clear S0i3 flag
    }
    //D0
    Switch (ToInteger(Arg0))
    {
      Case (5)  { store(0x00, I0TD) store(one, I0PD) store(I0DS, Local0) while (LNotEqual(Local0,0x7)) {store(I0DS, Local0)}}
      Case (6)  { store(0x00, I1TD) store(one, I1PD) store(I1DS, Local0) while (LNotEqual(Local0,0x7)) {store(I1DS, Local0)}}
      Case (7)  { store(0x00, I2TD) store(one, I2PD) store(I2DS, Local0) while (LNotEqual(Local0,0x7)) {store(I2DS, Local0)}}
      Case (8)  { store(0x00, I3TD) store(one, I3PD) store(I3DS, Local0) while (LNotEqual(Local0,0x7)) {store(I3DS, Local0)}}
      Case (11) { store(0x00, U0TD) store(one, U0PD) store(U0DS, Local0) while (LNotEqual(Local0,0x7)) {store(U0DS, Local0)}}
      Case (12) { store(0x00, U1TD) store(one, U1PD) store(U1DS, Local0) while (LNotEqual(Local0,0x7)) {store(U1DS, Local0)}}
      Case (15) { STD0 ()}  //SATA
      Case (18) { U2D0 ()}  //EHCI
      Case (23) { U3D0 ()}  //XHCI
      Case (24) { SDD0 ()}  //SD
    }
  } else {
    //D3 
    Switch (ToInteger(Arg0))
    {
      Case (5)  { store(zero, I0PD) store(I0DS, Local0) while (LNotEqual(Local0,0x0)) {store(I0DS, Local0)}  store(0x03, I0TD)}
      Case (6)  { store(zero, I1PD) store(I1DS, Local0) while (LNotEqual(Local0,0x0)) {store(I1DS, Local0)}  store(0x03, I1TD)}
      Case (7)  { store(zero, I2PD) store(I2DS, Local0) while (LNotEqual(Local0,0x0)) {store(I2DS, Local0)}  store(0x03, I2TD)}
      Case (8)  { store(zero, I3PD) store(I3DS, Local0) while (LNotEqual(Local0,0x0)) {store(I3DS, Local0)}  store(0x03, I3TD)}
      Case (11) { store(zero, U0PD) store(U0DS, Local0) while (LNotEqual(Local0,0x0)) {store(U0DS, Local0)}  store(0x03, U0TD)}
      Case (12) { store(zero, U1PD) store(U1DS, Local0) while (LNotEqual(Local0,0x0)) {store(U1DS, Local0)}  store(0x03, U1TD)}
      Case (15) { STD3 ()}  //SATA
      Case (18) { U2D3 ()}  //EHCI
      Case (23) { U3D3 ()}  //XHCI
      Case (24) { SDD3 ()}  //SD
    }
    //Turn off Power
    if (LEqual (I0TD, 3)) {
      if (LEqual (SATD, 3)) {
        if (LEqual (SDTD, 3)) { Store (zero, PG1A) }
      }
    }
    if (LEqual (I1TD, 3)) {
      if (LEqual (I2TD, 3)) {
        if (LEqual (I3TD, 3)) { 
          if (LEqual (U0TD, 3)) {
            if (LEqual (U1TD, 3)) {
             if (LEqual (U2TD, 3)) { Store (zero, PG2_) }
            }
          }
        }
      }
    }
    if (LEqual (PG1A, 0)) {
      if (LEqual (PG2_, 0)) {
        //if (LEqual (U3PG, 0)) { Store (one, SRDY) }  //Set S0i3 Flag
        if (LEqual (U3PG, 0)) { 
          if (LEqual(S03D, 1)) {  //Do for S0I3.
            Store (one, SRDY) 
          }  //Set S0i3 Flag
        }
      }
    }
  }
  Release (FDAS)
}

Method(FPTS,0, Serialized)  // FCH PTS
{
  if (LNotEqual(U_PS, 0x03)) {
    Store(0x8C, IO80)
    breakpoint
  }
  if (LNotEqual(E_PS, 0x03)) {
    Store(0x8D, IO80)
    breakpoint
  }
  if (LEqual(\XHCD, zero)) {
    if (LNotEqual(U3TD, 0x03)) {
      Store(0x82, IO80)
      FDDC (23, 3)
    }
  }
  if (LEqual(\EHCD, zero)) {
    if (LNotEqual(U2TD, 0x03)) {FDDC (18, 3)}
  }

  if (LEqual(\ST_D, one)) {
    if (LNotEqual(SATD, 0x00)) {FDDC (15, 0)}
  }

}

Method(FWAK,0, Serialized)  // FCH WAK
{
  if (LEqual(\XHCD, zero)) {
    if (LEqual(U3TD, 0x03)) {
      Store(0x80, IO80)
      FDDC (23, 0)
    }
  }
  if (LEqual(\EHCD, zero)) {
    if (LEqual(U2TD, 0x03)) {FDDC (18, 0)}
  }
  //FINI ()
//Put UART/I2C to D3 Cold if it is disabled
  if (LEqual(\UT0E, zero)) {
    if (LNotEqual(U0TD, 0x03)) {FDDC (11, 3)}
  }
  if (LEqual(\UT1E, zero)) {
    if (LNotEqual(U1TD, 0x03)) {FDDC (12, 3)}
  }
  if (LEqual(\IC0E, zero)) {
    if (LNotEqual(I0TD, 0x03)) {FDDC (5, 3)}
  }
  if (LEqual(\IC1E, zero)) {
    if (LNotEqual(I1TD, 0x03)) {FDDC (6, 3)}
  }
  if (LEqual(\IC2E, zero)) {
    if (LNotEqual(I2TD, 0x03)) {FDDC (7, 3)}
  }
  if (LEqual(\IC3E, zero)) {
    if (LNotEqual(I3TD, 0x03)) {FDDC (8, 3)}
  }
}

Method(PWGC,2, Serialized)  // Arg0 bit location;Arg1 clear or set if Arg0 5, setting exceptions
{
  and (PGA3, 0xDF, Local0)  //clear BIT5
  if (Arg1) { or (Arg0, Local0, Local0)}
  else { 
    Not (Arg0, Local1) 
    And (Local1, Local0, Local0)
  }
  store (Local0, PGA3)
  if (LEqual (Arg0, 0x20)) {
    store (PGA3, Local0)
    and (Arg0, Local0, Local0)
    while (LNot(Local0)) {
      store(PGA3, Local0)
      and (Arg0, Local0, Local0)
    }
  }
}

Method(SDD3,0, Serialized)
{
//  store (\SD2C, SDID)
  SDSP ()
  store (zero, SDSC)
  store (zero, RQTY)
  store (one, RQ24)
  store (SDSR, Local0)
  while (Local0) {store (SDSR, Local0)}

  store (zero, SDPD)
  store (SDDS, Local0)
  while (LNotEqual(Local0,zero)) {store (SDDS, Local0)}

  store (0x03,SDTD)
  if (LEqual(ESPI, zero)) { store (one, LDQ0) }
}

Method(SDD0,0, Serialized)
{
  // Set PwrOnDev = 1
  store (one, SDPD)
  // Wait for HW reset completed.
  store (SDDS, Local0)
  while (LNotEqual(Local0,0x7)) {store (SDDS, Local0)}

  // ENH449309:Workaround for OBS448495 SD Host Controller SUBSYS ID change issu
  SRAD (24, 200)

  // Set TargetDeviceState to D0-uninitialized
  store (0x00,SDTD)

  // Set RequestType to restore
  store (one, RQTY)
  store (one, RQ24)

  // Wait for restore complete
  store (SDSR, Local0)
  while (LNot(Local0)) {store (SDSR, Local0)}

  store (SDID, \SD2C)

  SDRP ()
  store (SNHG, LDQ0)
  store (SF18, FC18)
}

Method(STD3,0, Serialized)
{
  ST_S ()  //save SATA registers
  ShiftLeft( SDS0, 2, Local0)
  Add (0xFED81500, Local0, Local0)
  ShiftLeft( SDS1, 2, Local1)
  Add (0xFED81500, Local1, Local1)
  OperationRegion( SDSA, SystemMemory, Local0, 0x04)
  Field( SDSA, ByteAcc, NoLock, Preserve) {
    ,22 ,
    GP0D, 1,   //Data
    GP0E, 1,   //Enable
  }
  OperationRegion( SDSB, SystemMemory, Local1, 0x04)
  Field( SDSB, ByteAcc, NoLock, Preserve) {
    ,22 ,
    GP1D, 1,   //Data
    GP1E, 1,   //Enable
  }
  store (zero, RQTY)
  store (one, RQ15)
  store (SASR, Local0)
  while (Local0) {store (SASR, Local0)}

  PWGC (0x10, 1)
  store (one, SM30)
  if (SDS0) {
    store (one, GP0E)  //AGIO06 for DevSlp on Gardenia
    store (zero, GP0D)
  }

  store (zero, SAPD)
  store (SADS, Local0)
  while (LNotEqual(Local0,zero)) {store (SADS, Local0)}

  store (0x03,SATD)

  store (one, SARP)
}

Method(STD0,0, Serialized)
{
  ShiftLeft( SDS0, 2, Local0)
  Add (0xFED81500, Local0, Local0)
  ShiftLeft( SDS1, 2, Local1)
  Add (0xFED81500, Local1, Local1)
  OperationRegion( SDSA, SystemMemory, Local0, 0x04)
  Field( SDSA, ByteAcc, NoLock, Preserve) {
    ,22 ,
    GP0D, 1,   //Data
    GP0E, 1,   //Enable
  }
  OperationRegion( SDSB, SystemMemory, Local1, 0x04)
  Field( SDSB, ByteAcc, NoLock, Preserve) {
    ,22 ,
    GP1D, 1,   //Data
    GP1E, 1,   //Enable
  }
  store (zero, SARP)
  store (0x00,SATD)
  store (one, SAPD)
  store (SADS, Local0)
  while (LNotEqual(Local0,0x7)) {store (SADS, Local0)}
  stall (120)
  PWGC (0x10, 0)
  store (zero, SM30)
  store (zero, DSDN) //

  ST_R ()
  if (LEqual(RFCS,0x1)) {
    store (zero, ST84)
    stall (120)
    store (one, ST84)
  }

  if (SDS0) {
    store (one, GP0E)  //AGIO06 for DevSlp on Gardenia
    store (one, GP0D)  //ungate
  }
  store (one, RQTY)
  store (one, RQ15)

  store (SASR, Local0)
  while (LNot(Local0)) {store (SASR, Local0)}
  //Eanble watchdog SATA_PCI_0x44[0] = 1
  store (1, ST44)

  store (1, DDIC)
  stall (100)
  store (0, DDIC)

}
Method (SU2P, 0, Serialized)  //Shutdow Usb2 PLL
{
    PWGC (0x40, 0)  
    store (one, U2RP)
}

Method(U2D3,0, Serialized)
{
  store (zero, RQTY)
  store (one, RQ18)

  store (U2SR, Local0)
  while (Local0) {store (U2SR, Local0)}

  store (zero, U2PD)

  store (U2DS, Local0)
  while (LNotEqual(Local0,zero)) {store (U2DS, Local0)}

  store (0x03,U2TD)

  if (LEqual(U3TD, 0x03)) {SU2P ()}
}

Method(U2D0,0, Serialized)
{
  PWGC (0x40, 1)
  store (zero, U2RP)
  store (0x00,U2TD)

  store (zero, U2TD)
  store (one, U2PD)

  store (U2DS, Local0)
  while (LNotEqual(Local0,0x7)) {store (U2DS, Local0)}

  store (one, RQTY)
  store (one, RQ18)
  store (U2SR, Local0)
  while (LNot(Local0)) {store (U2SR, Local0)}
  store (EHID, \EH2C)
}

Method(LXFW,3, Serialized)  //Load Xhci FirmWare
{
//      FWLM, 1,  //FirmWare Load Mode
//      FPLS, 1,  //Fw PreLoad Start
//      FPLC, 1,  //Fw PreLoad Complete
//      ROAM, 1,  //1= ROM 0=RAM

    store (one, FWLM)
    store (Arg0, ROAM)
    store (Arg1, UA04)
    store (Arg2, UA08)
    store (one, FPLS)
    store (FPLC, Local0)
    while (LNot(Local0)) {store (FPLC, Local0)}
    store (zero, FPLS)
  }

Method(X1_S,0, Serialized)
{
  store (zero, RQTY)
  store (one, RQ23)
  store (U3SR, Local0)
  while (Local0) {store (U3SR, Local0)}
}
Method(X1_R,0, Serialized)
{
  store (one, RQTY)
  store (one, RQ23)
  store (U3SR, Local0)
  while (LNot(Local0)) {store (U3SR, Local0)}
}
Method(U3D3,0, Serialized)
{
  //force D3 hot if it is not in D3 hot.
  //if (LEqual(U_PS, 0)) {store(3, U_PS)}
  if (LEqual(U_PS, 3)) {

    X0_S () //Software
    //X1_S ()  //Hardware
  
    PWGC (0x20, 1)
  
    store (one, UD3P)
    store (zero, U3PD)
  
    store (U3DS, Local0)
    while (LNotEqual(Local0,zero)) {store (U3DS, Local0)}
  
    store (0x03,U3TD)
    store (one, U3RP)
    if (LEqual(U2TD, 0x03)) {SU2P ()}
    store (zero, U3PG)
  
    store (one, U3PS)
  }
}

Method(U3D0,0, Serialized)
{
  store (zero, U3PS)

  store (one, U3PG)
  store (zero, U2RP)
  store (zero, U3RP)
  PWGC (0x40, 1)

  store (zero, U3TD)
  store (one, U3PD)

  store (U3DS, Local0)
  while (LNotEqual(Local0,0x7)) {store (U3DS, Local0)}

  store (U3PY, Local0)
  while (LNotEqual(Local0,1)) {store (U3PY, Local0)}

  store (zero, U3PR)
  //Reload RAM/ROM
  store (AUSS, Local0)
  if (LNotEqual(Local0,1)) {store(one, AUSS)}
  store (AUSS, Local0)
  while (LNotEqual(Local0,1)) {store (AUSS, Local0)}

  LXFW (1, FW00, FW01)
  LXFW (0, FW02, FW03)

  //X1_R ()
  X0_R ()

  store (one, U3PR)
  store (zero, UD3P)
  store (0x01,U3TD)
  store (XHID, \XH2C)
}

OperationRegion(ECMC, SystemIo, 0x72, 0x02)
Field(ECMC, AnyAcc, NoLock, Preserve)
{
  ECMI, 8,
  ECMD, 8,      
}

IndexField(ECMI, ECMD, ByteAcc, NoLock, Preserve) {
  Offset (0x08),
  FRTB, 32,
}

OperationRegion(FRTP, SystemMemory, FRTB, 0x100)
Field(FRTP, AnyAcc, NoLock, Preserve)
{
  PEBA, 32,
  Offset (0x04),
  , 5,
  IC0E, 1,   //I2C0, 5
  IC1E, 1,
  IC2E, 1,
  IC3E, 1,   //I2C3, 8
  , 2,
  UT0E, 1,   //UART0, 11
  UT1E, 1,   //UART1, 12
  , 14,
  ESPI, 1,

  Offset (0x08),
  , 5,
  IC0D, 1,   //I2C0, 5
  IC1D, 1,
  IC2D, 1,
  IC3D, 1,   //I2C3, 8
  , 2,
  UT0D, 1,   //UART0, 11
  UT1D, 1,   //UART1, 12
  , 2,
  ST_D, 1,   //SATA, 15
  , 2,
  EHCD, 1,   //EHCI, 18
  , 4,
  XHCD, 1,   //XCHI, 23
  SD_D, 1,   //SD,   24
  , 6,
  S03D, 1,   //S0I3 flag, 31   

  Offset (0x0C),
  FW00, 16, 
  FW01, 32, 
  FW02, 16, 
  FW03, 32, 
  SDS0, 8, //SataDevSlpPort0S5Pin
  SDS1, 8, //SataDevSlpPort1S5Pin
}

Method(FINI) { 
//  breakpoint
//  SRAD (5, 200)
  store (\SD2C, SDID)
  store (\EH2C, EHID)
  store (\XH2C, XHID)
  store (\ST2C, STID)

  store (LDQ0, SNHG)
  store (FC18, SF18)

  If (LGreaterEqual(TPOS, 0x60)) {
  } Else {
    store (zero, S03D)
    store (zero, XHCD)
    store (zero, EHCD)
    store (zero, ST_D)
    store (zero, SD_D)
  }

//Put UART/I2C to D3 Cold if it is disabled
  if (LEqual(\UT0E, zero)) {
    if (LNotEqual(U0TD, 0x03)) {FDDC (11, 3)}
  }
  if (LEqual(\UT1E, zero)) {
    if (LNotEqual(U1TD, 0x03)) {FDDC (12, 3)}
  }
  if (LEqual(\IC0E, zero)) {
    if (LNotEqual(I0TD, 0x03)) {FDDC (5, 3)}
  }
  if (LEqual(\IC1E, zero)) {
    if (LNotEqual(I1TD, 0x03)) {FDDC (6, 3)}
  }
  if (LEqual(\IC2E, zero)) {
    if (LNotEqual(I2TD, 0x03)) {FDDC (7, 3)}
  }
  if (LEqual(\IC3E, zero)) {
    if (LNotEqual(I3TD, 0x03)) {FDDC (8, 3)}
  }

}

OperationRegion(FCFG, SystemMemory, PEBA, 0x01000000)
Field(FCFG, DwordAcc, NoLock, Preserve)
{
  //XHCI
  Offset(0x0008002C),
    XH2C, 32,

  Offset(0x00080048),       
    IDEX, 32,
    DATA, 32,
  Offset(0x00080054),       
    U_PS, 2,

  //SATA
  Offset(0x00088010),
    ST10, 32,
    ST14, 32,
    ST18, 32,
    ST1C, 32,
    ST20, 32,
    ST24, 32,

  Offset(0x0008802C),
    ST2C, 32,

  Offset(0x00088040),
    ST40, 1,

  Offset(0x00088044),
    ST44, 1,

  Offset(0x0008804C),
    , 2,
    DDIC, 1,  //DisableDynamicInterfaceClockPowerSaving

  Offset(0x00088064),       
    S_PS, 2,

  Offset(0x00088084),
    , 1,
    ST84, 1,
    , 28,
    DSDN, 1, // DShutDowN

  Offset(0x0008808C),
    ST8C, 8,

//EHCI
  Offset(0x0009002C),
    EH2C, 32,

  Offset(0x000900C4),       
    E_PS, 2,

  Offset(0x000A3078),  
    , 2,  
    LDQ0,  1, //

  Offset(0x000A30CB),    
    ,  7,
    AUSS,  1, //AutoSizeStart

//SD
  Offset(0x000A7004),    
    ,  1,
    SDME,  1,

  Offset(0x000A7010),    
    SDBA,  32,
  Offset(0x000A702C),    
    SD2C,  32,
  Offset(0x000A7094),    
    D_PS,  2,
    ,  6,
    SDPE, 1,   
    ,  6,
    PMES, 1,   

  Offset(0x000A70B4),    //Set Enable
    ,  8,
    SETE, 1,   

  Offset(0x000A70D0),    //
    ,  17,
    FC18, 1,   //Force1.8v
}

OperationRegion(SDMC, SystemMemory, SDBA, 0x100)
Field(SDMC, AnyAcc, NoLock, Preserve)
{
  Offset(0x24),       //0x28
    , 6,
    CDTC, 2,   //control of CdTestLevel CdTestEn

  Offset(0x27),       //0x2B
    IRWE, 1,
    ISWE, 1,
    RMWE, 1,
}

//SD falling Edge
  Method(SDFE,0, Serialized) { Store(0x2, CDTC) Store(0x3, CDTC) Store(0x00, CDTC)                  Store(one, SETE) Store(one, PMES) Store(zero, SETE) }
//SD Rising Edge
  Method(SDRE,0, Serialized) { Store(0x3, CDTC) Store(0x2, CDTC) Store(0x00, CDTC)                  Store(one, SETE) Store(one, PMES) Store(zero, SETE) }

Method(SDSP,0, Serialized)  //SD Save Pin
{
  Store(one, SDME)
  Store(SDPE, B600)   
  Store(IRWE, B601)   
  Store(ISWE, B602)   
  Store(RMWE, B603)   
  Store(zero, SDME)
}

Method(SDRP,0, Serialized)  //SD Restore Pin
{
  if (SDFG) {
    if (G19S) {SDFE ()}
    else { SDRE ()}
    store (zero, SDFG)
  }
}
Method(SRAD,2, Serialized)  //SoftResetAoacDevice, Arg0:Device ID, Arg1:reset period in micro seconds
{
  ShiftLeft(Arg0, 1, Local0)
  Add (Local0, 0xfed81e40, Local0)
  OperationRegion( ADCR, SystemMemory, Local0, 0x02)
  Field( ADCR, ByteAcc, NoLock, Preserve) { //AoacD3ControlRegister
    ADTD, 2,
    ADPS, 1,
    ADPD, 1, 
    ADSO, 1, 
    ADSC, 1, 
    ADSR, 1, 
    ADIS, 1, 
    ADDS, 3, 
  }
  store (one, ADIS)       // IsSwControl = 1
  store (zero, ADSR)      // SwRstB = 0
  stall (Arg1)
  store (one, ADSR)       // SwRstB = 1
  store (zero, ADIS)      // IsSwControl = 0
  stall (Arg1)
}

Name (SVBF, Buffer (0x1000) {0})
CreateDWordField(SVBF, 0x000, S000)
CreateDWordField(SVBF, 0x004, S004)
CreateDWordField(SVBF, 0x008, S008)
CreateDWordField(SVBF, 0x00C, S00C)
CreateDWordField(SVBF, 0x018, S018)
CreateDWordField(SVBF, 0x01C, S01C)
CreateDWordField(SVBF, 0x020, S020)
CreateDWordField(SVBF, 0x030, S030)
CreateDWordField(SVBF, 0x118, S118)
CreateDWordField(SVBF, 0x158, S158)
CreateDWordField(SVBF, 0x198, S198)
CreateDWordField(SVBF, 0x1D8, S1D8)
CreateDWordField(SVBF, 0x300, S300)
CreateDWordField(SVBF, 0x304, S304)
CreateDWordField(SVBF, 0x308, S308)
CreateDWordField(SVBF, 0x30C, S30C)
CreateDWordField(SVBF, 0x310, S310)
CreateDWordField(SVBF, 0x428, S428)
CreateDWordField(SVBF, 0x438, S438)
CreateDWordField(SVBF, 0x43C, S43C)
CreateDWordField(SVBF, 0x458, S458)
CreateDWordField(SVBF, 0x468, S468)
CreateDWordField(SVBF, 0x46C, S46C)
CreateDWordField(SVBF, 0x470, S470)
CreateDWordField(SVBF, 0x480, S480)
CreateDWordField(SVBF, 0x484, S484)
CreateDWordField(SVBF, 0x488, S488)
CreateDWordField(SVBF, 0x48C, S48C)

CreateByteField(SVBF, 0x600, B600)
CreateByteField(SVBF, 0x601, B601)
CreateByteField(SVBF, 0x602, B602)
CreateByteField(SVBF, 0x603, B603)

//SD flag
CreateByteField(SVBF, 0x700, SDFG)  //SD EVENT FLAG
CreateByteField(SVBF, 0x701, SNHG)  //NOHOG
CreateByteField(SVBF, 0x702, SF18)  //Force18
CreateDWordField(SVBF, 0x704, SDID) // SD SSID

//EHCI,XHCI,SATA SSIDs
CreateDWordField(SVBF, 0x730, EHID) // EHCI SSID
CreateDWordField(SVBF, 0x734, XHID) // XHCI SSID
CreateDWordField(SVBF, 0x738, STID) // SATA SSID

//SATA
CreateDWordField(SVBF, 0x810, S810)
CreateDWordField(SVBF, 0x814, S814)
CreateDWordField(SVBF, 0x818, S818)
CreateDWordField(SVBF, 0x81C, S81C)
CreateDWordField(SVBF, 0x820, S820)
CreateDWordField(SVBF, 0x824, S824)
CreateDWordField(SVBF, 0x82C, S82C)
CreateByteField(SVBF, 0x88C, S88C)


Method(ST_S,0)
{ 
  Store (ST10, S810)
  Store (ST14, S814)
  Store (ST18, S818)
  Store (ST1C, S81C)
  Store (ST20, S820)
  Store (ST24, S824)
//  Store (ST2C, S82C)
  Store (ST8C, S88C)
}                                  

Method(ST_R,0)
{ 
  Store (S810, ST10)
  Store (S814, ST14)
  Store (S818, ST18)
  Store (S81C, ST1C)
  Store (S820, ST20)
  Store (S824, ST24)
//  Store (STID, ST2C)
  store(one, ST40)
  store(STID, \ST2C)
  store(zero, ST40)
  Store (S88C, ST8C)
}                                  

Method(X0_S,0)
{
  Store(0x00000000, IDEX) Store(DATA, S000)
  Store(0x00000004, IDEX) Store(DATA, S004)
  Store(0x00000008, IDEX) Store(DATA, S008)
  Store(0x0000000C, IDEX) Store(DATA, S00C)
  Store(0x00000018, IDEX) Store(DATA, S018)
  Store(0x0000001C, IDEX) Store(DATA, S01C)
  Store(0x00000020, IDEX) Store(DATA, S020)
  Store(0x00000030, IDEX) Store(DATA, S030)
  Store(0x00000118, IDEX) Store(DATA, S118)
  Store(0x00000158, IDEX) Store(DATA, S158)
  Store(0x00000198, IDEX) Store(DATA, S198)
  Store(0x000001D8, IDEX) Store(DATA, S1D8)
  Store(0x00000300, IDEX) Store(DATA, S300)
  Store(0x00000304, IDEX) Store(DATA, S304)
  Store(0x00000308, IDEX) Store(DATA, S308)
  Store(0x0000030C, IDEX) Store(DATA, S30C)
  Store(0x00000310, IDEX) Store(DATA, S310)
  Store(0x40000028, IDEX) Store(DATA, S428)
  Store(0x40000038, IDEX) Store(DATA, S438)
  Store(0x4000003C, IDEX) Store(DATA, S43C)
  Store(0x40000058, IDEX) Store(DATA, S458)
  Store(0x40000068, IDEX) Store(DATA, S468)
  Store(0x4000006C, IDEX) Store(DATA, S46C)
  Store(0x40000070, IDEX) Store(DATA, S470)
  Store(0x40000080, IDEX) Store(DATA, S480)
  Store(0x40000084, IDEX) Store(DATA, S484)
  Store(0x40000088, IDEX) Store(DATA, S488)
  Store(0x4000008C, IDEX) Store(DATA, S48C)
}                                         

Method(X0_R,0)
{
  Store(0x00000000, IDEX) Store(S000, DATA)
  Store(0x00000004, IDEX) Store(S004, DATA)
  Store(0x00000008, IDEX) Store(S008, DATA)
  Store(0x0000000C, IDEX) Store(S00C, DATA)
  Store(0x00000018, IDEX) Store(S018, DATA)
  Store(0x0000001C, IDEX) Store(S01C, DATA)
  Store(0x00000020, IDEX) Store(S020, DATA)
  Store(0x00000030, IDEX) Store(S030, DATA)
  Store(0x00000118, IDEX) Store(S118, DATA)
  Store(0x00000158, IDEX) Store(S158, DATA)
  Store(0x00000198, IDEX) Store(S198, DATA)
  Store(0x000001D8, IDEX) Store(S1D8, DATA)
  Store(0x00000300, IDEX) Store(S300, DATA)
  Store(0x00000304, IDEX) Store(S304, DATA)
  Store(0x00000308, IDEX) Store(S308, DATA)
  Store(0x0000030C, IDEX) Store(S30C, DATA)
  Store(0x00000310, IDEX) Store(S310, DATA)
  Store(0x40000028, IDEX) Store(S428, DATA)
  Store(0x40000038, IDEX) Store(S438, DATA)
  Store(0x4000003C, IDEX) Store(S43C, DATA)
  Store(0x40000058, IDEX) Store(S458, DATA)
  Store(0x40000068, IDEX) Store(S468, DATA)
  Store(0x4000006C, IDEX) Store(S46C, DATA)
  Store(0x40000070, IDEX) Store(S470, DATA)
  Store(0x40000080, IDEX) Store(S480, DATA)
  Store(0x40000084, IDEX) Store(S484, DATA)
  Store(0x40000088, IDEX) Store(S488, DATA)
  Store(0x4000008C, IDEX) Store(S48C, DATA)
} 

Scope(\_SB)
{
    Name(XHD0, 0)
    Name(XHD3, 0)
    PowerResource(P0U3, 0, 0) {
      Method(_STA) {
        Return(XHD0)
      }
      Method(_ON) {
	Store(0x01, XHD0)
      }
      Method(_OFF) {
      	Store(0x00, XHD0)
      }
    }
    PowerResource(P3U3, 0, 0) {
      Method(_STA) {
        Return(XHD3)
      }
      Method(_ON) {
	Store(0x01, XHD3)
      }
      Method(_OFF) {
	Store(0x00, XHD3)
      }
    }

    Name(EHD0, 0)
    Name(EHD3, 0)
    PowerResource(P0U2, 0, 0) {
      Method(_STA) {
        Return(EHD0)
      }
      Method(_ON) {
	Store(0x01, EHD0)
      }
      Method(_OFF) {
      	Store(0x00, EHD0)
      }
    }
    PowerResource(P3U2, 0, 0) {
      Method(_STA) {
        Return(EHD3)
      }
      Method(_ON) {
	Store(0x01, EHD3)
      }
      Method(_OFF) {
	Store(0x00, EHD3)
      }
    }

    Name(D0SD, 0)
    Name(D3SD, 0)
    PowerResource(P0SD, 0, 0) {
      Method(_STA) {
        Return(D0SD)
      }
      Method(_ON) {
	Store(0x01, D0SD)
      }
      Method(_OFF) {
      	Store(0x00, D0SD)
      }
    }
    PowerResource(P3SD, 0, 0) {
      Method(_STA) {
        Return(D3SD)
      }
      Method(_ON) {
	Store(0x01, D3SD)
      }
      Method(_OFF) {
	Store(0x00, D3SD)
      }
    }
    Name(D0ST, 1)
    Name(D3ST, 1)
    PowerResource(P0ST, 0, 0) {
      Method(_STA) {
        Return(D0ST)
      }
      Method(_ON) {
	Store(0x01, D0ST)
      }
      Method(_OFF) {
      	Store(0x00, D0ST)
      }
    }
    PowerResource(P3ST, 0, 0) {
      Method(_STA) {
        Return(D3ST)
      }
      Method(_ON) {
	Store(0x01, D3ST)
      }
      Method(_OFF) {
	Store(0x00, D3ST)
      }
    }
}

Scope(\_SB.PCI0.XHC0)
{
// Indicate support for D0.
     Name(_PR0, Package() {P0U3}) // Power resources required for D0

// Indicate support for D3Cold.
     Name(_PR3, Package() {P3U3}) // Power resource for D3; These will
                                        //  be turned off ONLY if drivers
    Method(_S0W,0)
    {
      if (LEqual(\XHCD, one)) {Return(4) }
      else { Return(0)}
    }

    Method(_S4W,0)
    {
      Return(4)
    }

    Method(_PS0,0)
    {
      if (LEqual(\XHCD, one)) {
        if (LEqual(U3TD, 0x03)) {
          Store(0x80, IO80)
          FDDC (23, 0)
        }
      }
    }
    Method(_PS3,0)
    {
      if (LEqual(\XHCD, one)) {
        if (LNotEqual(U3TD, 0x03)) {
          if (LEqual(U_PS, 0x03)) {
            Store(0x82, IO80)
            FDDC (23, 3)
          }
        }
      }
    }
}
Scope(\_SB.PCI0.EHC1)
{
// Indicate support for D0.
     Name(_PR0, Package() {P0U2}) // Power resources required for D0

// Indicate support for D3Cold.
     Name(_PR3, Package() {P3U2}) // Power resource for D3; These will
                                        //  be turned off ONLY if drivers
    Method(_S0W,0)
    {
      if (LEqual(\EHCD, one)) {Return(4) }
      else { Return(0)}
    }

    Method(_S4W,0)
    {
      Return(4)
    }

    Method(_PS0,0)
    {
      if (LEqual(\EHCD, one)) {
        Store(0x50, IO80)
        if (LNotEqual(U2TD, 0x00)) {FDDC (18, 0)}
      }
    }
    Method(_PS3,0)
    {
      if (LEqual(\EHCD, one)) {
        if (LEqual(E_PS, 0x03)) {
          Store(0x52, IO80)
          if (LNotEqual(U2TD, 0x03)) {FDDC (18, 3)}
        }
      }
    }

    // BIOS will patch RHRS to _CRS if HW_REDUCED_ACPI is enabled
    Method (RHRS, 0, NotSerialized)
    {
        Name (RBUF, ResourceTemplate ()
        {
            GpioInt (Level, ActiveHigh, ExclusiveAndWake, PullNone, 0x0000, "\\_SB.GPIO", 0x00, ResourceConsumer, , )
                {54}    // Int_iusb_wakeup0
        })
        Return(RBUF)        // Yes, return GPIO interrupt info
    }
}
Scope(\_SB.PCI0.SDIO)
{
    OperationRegion(PCFG, PCI_Config, 0x2C, 0x04)
    Field(PCFG, DWordAcc, NoLock, Preserve)
    {
      SSID, 32
    }

// Indicate support for D0.
     Name(_PR0, Package() {P0SD}) // Power resources required for D0

// Indicate support for D3Cold.
     Name(_PR3, Package() {P3SD}) // Power resource for D3; These will
                                        //  be turned off ONLY if drivers
    Method(_S0W,0)
    {
      if (LEqual(\SD_D, one)) {Return(4) }
      else { Return(4)}
    }
    Method(_PS0,0)
    {
      if (LEqual(\SD_D, one)) {
        Store(0xF5D0, P80H)
        if (LNotEqual(SDTD, 0x00)) {
          FDDC (24, 0) 
          sleep (100)
          if (LEqual(S03D, 0)) {  //Do for Non S0I3.
            \_SB.SDRT () // platform should provide reset devices behind SD controllers
          }
        }
      }
    }
    Method(_PS3,0)
    {
      if (LEqual(\SD_D, one)) {
        if (LEqual(D_PS, 0x03)) {
          Store(0xF5D3, P80H)
          if (LNotEqual(SDTD, 0x03)) {FDDC (24, 3)}
        }
      }
    }
}

Scope(\_SB.PCI0.SATA)
{
// Indicate support for D0.
     Name(_PR0, Package() {P0ST}) // Power resources required for D0

// Indicate support for D3Cold.
     Name(_PR3, Package() {P3ST}) // Power resource for D3; These will
                                        //  be turned off ONLY if drivers
    Method(_S0W,0)
    {
      if (LEqual(\ST_D, one)) {Return(4) }
      else { Return(0)}
    }
    Method(_PS0,0)
    {
      if (LEqual(\ST_D, one)) {
        Store(0xA0, IO80)
        if (LNotEqual(SATD, 0x00)) {FDDC (15, 0)}
      }
    }
    Method(_PS3,0)
    {
      if (LEqual(\ST_D, one)) {
        if (LNotEqual(SATD, 0x03)) {
          if (LEqual(S_PS, 0x03)) {
            Store(0xA2, IO80)
            FDDC (15, 3)
          }
        }
      }
    }
}

Scope(\_SB)
{

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
      If (LGreaterEqual(TPOS, 0x60)) {
        Return (0x0F)
      } Else {
        Return (0x00)
      }
    }

    Name(_AEI, ResourceTemplate ()
    {
      //Timer Event
      GpioInt(Edge, ActiveHigh, Exclusive, PullDown, , "\\_SB.GPIO") {180}
      GpioInt(Edge, ActiveLow, Exclusive, PullUp, , "\\_SB.GPIO") {25}
    })
    
    Method (_EVT, 0x1) { // Handle all ACPI Events signaled by GPIO Controller GPI2
      Switch (ToInteger(Arg0))
      {
        Case (180) {
          Store(0x88, IO80)
          breakpoint
        }
        Case (25) {
          //breakpoint
          if (LEqual(\SD_D, one)) { 
            Notify (\_SB.PCI0.SDIO,0x0)
          }
        }
      }
    }//End of Method  
    
  } // End Device(GPIO)

  Device(AAHB) {
    Name(_HID,"AAHB0000")            // Hardware Device ID
    Name(_UID,0x0)
    Name(_CRS, ResourceTemplate()
    {
      IRQ(Edge, ActiveHigh, Exclusive) {7}
      Memory32Fixed(ReadWrite, 0xFEDC0000, 0x2000)
    })

    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        Return (0x00) //disable it for now
      } Else {
        Return (0x00)
      }
    }
  } // End Device(AAHB)


  Device(FUR0) {
    Name(_HID,"AMD0020")            // Hardware Device ID
    Name(_UID,0x0)
    Name(_CRS, ResourceTemplate() {
      IRQ(Edge, ActiveHigh, Exclusive) {10}
      Memory32Fixed(ReadWrite, 0xFEDC6000, 0x2000)
    })
    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        if (LEqual(\UT0E, one)) { Return (0x0F)}
        Return (0x00)
      } Else {
        Return (0x00)
      }
    }
    Method(_S0W,0) {
      if (LAnd(\UT0D, \UT0E)) { Return(4) }
      else { Return(0)}
    }
    Method(_PS0,0) {
      if (LAnd(\UT0D, \UT0E)) {
        Store(0xD0, IO80)
        if (LEqual(U0TD, 0x03)) {FDDC (11, 0)}
      }
    }
    Method(_PS3,0) {
      if (LAnd(\UT0D, \UT0E)) {
        Store(0xD2, IO80)
        if (LNotEqual(U0TD, 0x03)) {FDDC (11, 3)}
      }
    }
  } // End Device(FUR0)

  Device(FUR1) {
    Name(_HID,"AMD0020")            // Hardware Device ID
    Name(_UID,0x1)
    Name(_CRS, ResourceTemplate() {
      IRQ(Edge, ActiveHigh, Exclusive) {11}
      Memory32Fixed(ReadWrite, 0xFEDC8000, 0x2000)
    })
    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        if (LEqual(\UT1E, one)) { Return (0x0F)}
        Return (0x00)
      } Else {
        Return (0x00)
      }
    }
    Method(_S0W,0) {
      if (LAnd(\UT1D, \UT1E)) { Return(4) }
      else { Return(0)}
    }
    Method(_PS0,0) {
      if (LAnd(\UT1D, \UT1E)) {
        Store(0xD8, IO80)
        if (LEqual(U1TD, 0x03)) {FDDC (12, 0)}
      }
    }
    Method(_PS3,0) {
      if (LAnd(\UT1D, \UT1E)) {
        Store(0xDA, IO80)
        if (LNotEqual(U1TD, 0x03)) {FDDC (12, 3)}
      }
    }
  } // End Device(FUR1)

  Device(I2CA) {
    Name(_HID,"AMD0010")            // Hardware Device ID
    Name(_UID,0x0)
    Name(_CRS, ResourceTemplate() {
      IRQ(Edge, ActiveHigh, Exclusive) {3}
      Memory32Fixed(ReadWrite, 0xFEDC2000, 0x1000)
    })

    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        if (LEqual(\IC0E, one)) { Return (0x0F)}
        Return (0x00)
      } Else {
        Return (0x00)
      }
    }


    Method(_S0W,0) {
      if (LAnd(\IC0D, \IC0E)) { Return(4) }
      else { Return(0)}
    }

    Method(_PS0,0) {
      if (LAnd(\IC0D, \IC0E)) {
        Store(0xE0, IO80)
        if (LEqual(I0TD, 0x03)) {FDDC (5, 0)}
      }
    }

    Method(_PS3,0) {
      if (LAnd(\IC0D, \IC0E)) {
        Store(0xE2, IO80)
        if (LNotEqual(I0TD, 0x03)) {FDDC (5, 3)}
      }
    }

    Method(RSET,0) { SRAD (5, 200)}

  } // End Device(I2CA)
  Device(I2CB)
  {
    Name(_HID,"AMD0010")            // Hardware Device ID
    Name(_UID,0x1)
    Name(_CRS, ResourceTemplate() {
      IRQ(Edge, ActiveHigh, Exclusive) {15}
      Memory32Fixed(ReadWrite, 0xFEDC3000, 0x1000)
    })
    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        if (LEqual(\IC1E, one)) { Return (0x0F)}
        Return (0x00)
      } Else {
        Return (0x00)
      }
    }
    Method(_S0W,0) {
      if (LAnd(\IC1D, \IC1E)) { Return(4) }
      else { Return(0)}
    }
    Method(_PS0,0) {
      if (LAnd(\IC1D, \IC1E)) {
        Store(0xE4, IO80)
        if (LEqual(I1TD, 0x03)) {FDDC (6, 0)}
      }
    }
    Method(_PS3,0) {
      if (LAnd(\IC1D, \IC1E)) {
        Store(0xE6, IO80)
        if (LNotEqual(I1TD, 0x03)) {FDDC (6, 3)}
      }
    }

    Method(RSET,0) { SRAD (6, 200)}

  } // End Device(I2CB)

  Device(I2CC) {
    Name(_HID,"AMD0010")            // Hardware Device ID
    Name(_UID,0x2)
    Name(_CRS, ResourceTemplate() {
      IRQ(Edge, ActiveHigh, Exclusive) {6}
      Memory32Fixed(ReadWrite, 0xFEDC4000, 0x1000)
    })
    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        if (LEqual(\IC2E, one)) { Return (0x0F)}
        Return (0x00)
      } Else {
        Return (0x00)
      }
    }
    Method(_S0W,0) {
      if (LAnd(\IC2D, \IC2E)) { Return(4) }
      else { Return(0)}
    }
    Method(_PS0,0) {
      if (LAnd(\IC2D, \IC2E)) {
        Store(0xE8, IO80)
        if (LEqual(I2TD, 0x03)) {FDDC (7, 0)}
      }
    }
    Method(_PS3,0) {
      if (LAnd(\IC2D, \IC2E)) {
        Store(0xEA, IO80)
        if (LNotEqual(I2TD, 0x03)) {FDDC (7, 3)}
      }
    }

    Method(RSET,0) { SRAD (7, 200)}

  } // End Device(I2CC)

  Device(I2CD) {
    Name(_HID,"AMD0010")            // Hardware Device ID
    Name(_UID,0x3)
    Name(_CRS, ResourceTemplate() {
      IRQ(Edge, ActiveHigh, Exclusive) {14}
      Memory32Fixed(ReadWrite, 0xFEDC5000, 0x1000)
    })
    Method (_STA, 0x0, NotSerialized) {
      If (LGreaterEqual(TPOS, 0x60)) {
        if (LEqual(\IC3E, one)) { Return (0x0F)}
        Return (0x00)
      } Else {
        Return (0x00)
      }
    }
    Method(_S0W,0) {
      if (LAnd(\IC3D, \IC3E)) { Return(4) }
      else { Return(0)}
    }
    Method(_PS0,0) {
      if (LAnd(\IC3D, \IC3E)) {
        Store(0xEA, IO80)
        if (LEqual(I3TD, 0x03)) {FDDC (8, 0)}
      }
    }
    Method(_PS3,0) {
      if (LAnd(\IC3D, \IC3E)) {
        Store(0xEC, IO80)
        if (LNotEqual(I3TD, 0x03)) {FDDC (8, 3)}
      }
    }

    Method(RSET,0) { SRAD (8, 200)}

  } // End Device(I2CD)
}

