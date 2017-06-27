/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2017 GiPU, Inc.
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

// ---------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>> Note <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ---------------------------------------------------------------------------
// Copyright 2012 Analogix
//
// Version 0.02
// Programming guide explain the detailed protocol of the I2C slave interface
// Here is the reference pseudocode for EC or other I2C master.
//
//  version control: V0.02
//  Add 5.4G VCO Gain increase patch
//
//  version control: V0.03
//  1.Add LVDS SSCG initial code for 1080P resolution example
//  2.Remove delay 3 ms and replaced with other intial code to reduce the initial time lenght

//  version control: V0.04
//  1.disable LVDS SSCG
//  2.Disable 5.4G enable for HSW GOP
//  3.Force 0x00600 to 0x01 for HSW GOP
//  4.Add LVDS sequence patch


// ---------------------------------------------------------------------------
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>End<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
// ---------------------------------------------------------------------------
#include <device/pci_def.h>
#include <device/device.h>

/* warning: Porting.h includes an open #pragma pack(1) */
#include "Porting.h"
#include "AGESA.h"
#include "amdlib.h"

//Device address//
#define ADDR_Access     0x50  //DEV_ADDRESS_FOR_ADDRESS
#define DATA_Access     0x8c  //DEV_ADDRESS_FOR_DATA

char DP_RX_Read_Data(unsigned char *d);
char DP_RX_Write_Data(unsigned char d);

//*--------------------------------------------------------------------------------------
//* Function Name       : DP_RX_Send_Address
//* Object              : send related address to device address 0x50
//* Input Parameters    : slaveID--register OSB addr, addrm--offset addr bit8~bit15, addrl--offset addr bit0~bit7.
//* Output Parameters   : I2C transfer status result
//*--------------------------------------------------------------------------------------
char DP_RX_Send_Address( unsigned char slaveID,  unsigned char addrm,  unsigned char addrl)
{
#if 0
        i2c_send_start_bit(); //I2C start

        i2c_send_byte(ADDR_Access); // followed with 0x8c address

        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 1;       // if no device acknowledge,  terminate transfer
        }
        i2c_send_byte(0x00); // followed with 0x00

        if (i2c_read_ack())
        {
                i2c_send_stop_bit();
                return 2;       // device no accept the data, terminate transfer
        }
        i2c_send_byte(slaveID); // send slave ID
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 3;       // device no accept the data, terminate transfer
        }

        i2c_send_byte(0x00); // most of the high byte of the address is 0x00
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 4;       // device no accept the data, terminate transfer
	}

        i2c_send_byte(addrm); // send the middle byte of the address
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 5;       // device no accept the data, terminate transfer
        }
        i2c_send_byte(addrl); //send the low byte of the address
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 6;       // device no accept the data, terminate transfer
        }

        //i2c_read_ack();
        i2c_send_stop_bit();
#else
	int ioBase = 0xB00;

	writeSmbusByteData (ioBase, ADDR_Access, slaveID, 0);
	writeSmbusByteData (ioBase, ADDR_Access, 0, 1);
	writeSmbusByteData (ioBase, ADDR_Access, addrm, 2);
	writeSmbusByteData (ioBase, ADDR_Access, addrl, 3);
//	writeSmbusByte (ioBase, ADDR_Access, 0);
//	writeSmbusByte (ioBase, ADDR_Access, addrm);
//	writeSmbusByte (ioBase, ADDR_Access, addrl);
#endif
        return 0;
}


//*--------------------------------------------------------------------------------------
//* Function Name       : DP_RX_Read
//* Object              : read one byte from ANX1122/3 registers
//* Input Parameters    : slaveID--register OSB addr, addrm--offset addr bit8~bit15, addrl--offset addr bit0~bit7, rxdata--value of register.
//* Output Parameters   : rc-result description
//*--------------------------------------------------------------------------------------
unsigned char DP_RX_Read(unsigned char slaveID, unsigned char addrm, unsigned char addrl, unsigned char *rxdata)
{
	unsigned char rc;
        DP_RX_Send_Address(slaveID, addrm, addrl);
        rc = DP_RX_Read_Data(rxdata);
	return rc;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : DP_RX_Write
//* Object              : wirte one byte to ANX1122/3 registers
//* Input Parameters    : slaveID--register OSB addr, addrm--offset addr bit8~bit15, addrl--offset addr bit0~bit7, txdata-- write in value.
//* Output Parameters   : rc-result description
//*--------------------------------------------------------------------------------------
unsigned char DP_RX_Write( unsigned char slaveID,  unsigned char addrm,  unsigned char addrl,  unsigned char txdata)
{
	unsigned char rc;
        DP_RX_Send_Address(slaveID, addrm, addrl);
        rc = DP_RX_Write_Data(txdata);
	return rc;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : DP_RX_Read_Data
//* Object              : read back data from device address 0x8c
//* Input Parameters    : d--register value read from device address 0x8c.
//* Output Parameters   : I2C transfer status result
//*--------------------------------------------------------------------------------------
char DP_RX_Read_Data(unsigned char *d)
{
#if 0
        i2c_send_start_bit(); //I2C start bit
        i2c_send_byte(DATA_Access); // followed with 0x50 for data access address
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 1;       // no device acknowledge
        }
        i2c_send_byte(0x00); // followed with 0x00
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 2;       // device no accept the data, terminate transfer
        }
        i2c_send_restart_bit(); // I2C restart
        i2c_send_byte(DATA_Access | 0x01); // 0x50 + 1 for I2C reading action
        if (i2c_read_ack())
                return 1;       // no device acknowledge
        *d = i2c_read_byte(); // get read back data
        i2c_send_ack(1);
        i2c_send_stop_bit();
#else
	int ioBase = 0xB00;
	readSmbusByteData (ioBase, DATA_Access, d, 0);

#endif
        return 0;
}
//*--------------------------------------------------------------------------------------
//* Function Name       : DP_RX_Write_Data
//* Object              : write data to device address 0x8c
//* Input Parameters    : d--write in value
//* Output Parameters   : I2C transfer status result
//*--------------------------------------------------------------------------------------
char DP_RX_Write_Data(unsigned char d)
{
#if 0
        i2c_send_start_bit();//I2C start bit
        i2c_send_byte(DATA_Access);// followed with 0x50 for data access address
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 1;       // no device acknowledge
        }
        i2c_send_byte(0x00); // followed with 0x00
        if (i2c_read_ack()) {
                i2c_send_stop_bit();
                return 2;       // device no accept the data, terminate transfer
        }
        i2c_send_byte(d);// send write-in data
        i2c_read_ack();
        i2c_send_stop_bit();
#else
	int ioBase = 0xB00;
	writeSmbusByteData (ioBase, DATA_Access, d, 0);
#endif
        return 0;
}

//*--------------------------------------------------------------------------------------
//* Function Name       : ANX1122_Initialization
//* Object              : initial ANX1122 with necessary register setting. All of these setting must be
//* Input Parameters    : None
//* Output Parameters   : None
//*--------------------------------------------------------------------------------------
void ANX1122_Initialization(void)
{
	DP_RX_Write(0x00, 0x00, 0x17, 0x80);//Force HPD low

	DP_RX_Write(0x00, 0x00, 0x0a, 0x00);//power up registers
	DP_RX_Write(0x00, 0x00, 0x07, 0x00);//Power up registers
	DP_RX_Write(0x05, 0x00, 0x15, 0xc8);// internal OSC clock adjustment
	DP_RX_Write(0x0c, 0x00, 0x02, 0x89);// loop bit to 10uA to make VPLL lock quickly
	DP_RX_Write(0x03, 0x00, 0xae, 0xfc);// delay M ready with 31 lines



	DP_RX_Write(0x0a, 0x00, 0x0e, 0x90);//make power sequencer can not force video pll power down reset and M ready signal
	DP_RX_Write(0x04, 0x00, 0x05, 0x25);//Manual Reset video FIFO
	DP_RX_Write(0x04, 0x00, 0x08, 0x11);//video power is controlled by SW
	DP_RX_Write(0x0c, 0x00, 0x00, 0x10);//LVDS POWER down is controlled by reigster

	mdelay(3);

	DP_RX_Write(0x01, 0x07, 0x02, 0xf6);//
	DP_RX_Write(0x01, 0x07, 0x21, 0x04);//
       	// LVDS PWM Bypass function enable

	DP_RX_Write(0x05, 0x00, 0x66, 0xea);//
	DP_RX_Write(0x05, 0x00, 0x68, 0xea);//
	// Increase 5.4G VCO gain level

	DP_RX_Write(0x0a, 0x00, 0x0e, 0xd0);//make power sequence can force video pll power down reset and M ready signal
	DP_RX_Write(0x04, 0x00, 0x05, 0x2d);//enalbe auto reset V FIFO
	DP_RX_Write(0x04, 0x00, 0x08, 0x01);//Video power is controlled by FSM
	DP_RX_Write(0x0c, 0x00, 0x00, 0x00);// LVDS power down is controlled by FSM

	//DP_RX_Write(0x01, 0x00, 0x01, 0x14);//enable 5.4G capability: use 2.7G for HSW GOP mode
	DP_RX_Write(0x01, 0x00, 0x02, 0xc2);//enable Training pattern 3 capability
	DP_RX_Write(0x05, 0x00, 0x93, 0x2d);//5.4G BOOST up to level 5th


	//DP_RX_Write(0x0c, 0x00, 0x0a, 0x20);//Power up SSCG model and Select SSC mode
	DP_RX_Write(0x0c, 0x00, 0x0a, 0x00);//SSCG disable
	DP_RX_Write(0x0c, 0x00, 0x0b, 0xbc);//refer to the calculation/program guide to setting correct value according to Mod AMP and Mod Freq
	DP_RX_Write(0x0c, 0x00, 0x0c, 0x19);//refer to the calculation/program guide to setting correct value according to Mod AMP and Mod Freq
	DP_RX_Write(0x0c, 0x00, 0x0d, 0x01);//Release SSCG Model reset

  	DP_RX_Write(0x01, 0x06, 0x00, 0x01);//force local Power state to 0x01 for GOP HSW

	// LVDS power up/down sequence patch
	DP_RX_Write(0x0a, 0x00, 0x13, 0x05);//
	DP_RX_Write(0x0a, 0x00, 0x16, 0x05);//
	DP_RX_Write(0x0a, 0x00, 0x17, 0x80);//

	//Default output is dual port mode
	//DP_RX_Write(0x0a, 0x00, 0x01, 0x42);//Single LVDS port output

        DP_RX_Write(0x00, 0x00, 0x17, 0x00);//Force HPD high

}

unsigned char DP_RX_Chip_Located(void)
{
	unsigned char c=0,c0=0;

	DP_RX_Read(0x00, 0x00, 0x00, &c);
	DP_RX_Read(0x00, 0x00, 0x01, &c0);

	printk(BIOS_DEBUG, "c=%x, c0=%x-------------------------------\n", c, c0);
	if((c == 0x85) && (c0 == 0x14))
        {
		return 1;
        }
        else
        {
		return 0;
        }
}

#if 0
void main ()
{
	while(!(DP_RX_Chip_Located()) ) // chip location
        {
		;
        }

        ANX1122_Initialization ();

}
#endif
