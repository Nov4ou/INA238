/*
 * iic.c
 *
 *  Created on: Apr 6, 2024
 *      Author: nov4ou
 */

#include "iic.h"

#define HIGH_BYTE(value) ((Uint8)((value) >> 8))
#define LOW_BYTE(value) ((Uint8)((value) & 0xFF))

extern Uint16 I2cIndex;

void I2CA_Init(void)
{
   // Initialize I2C
    I2caRegs.I2CSAR = 0x0041;       // Slave Address.
    I2caRegs.I2COAR = 0x002D;       //  address as Master.
    I2caRegs.I2CPSC.all = 6;        // Prescaler - need 7-12 Mhz on module clk
    I2caRegs.I2CCLKL = 10;          // NOTE: must be non zero
    I2caRegs.I2CCLKH = 5;           // NOTE: must be non zero
    I2caRegs.I2CIER.all = 0x2C;     // Enable SCD & ARDY interrupts

    I2caRegs.I2CMDR.bit.IRS = 1;    // Take I2C out of reset
                                    // Stop I2C when suspended

    I2caRegs.I2CFFTX.all = 0x6000;  // Enable FIFO mode and TXFIFO
//    I2caRegs.I2CFFRX.all = 0x2040;    // Enable RXFIFO, clear RXFFINT,
    return;
}

void I2CA_Write(Uint16 Addr, Uint16 data)
{
    I2caRegs.I2CCNT = 3;            // 3 Additional Bytes being tranferred.
    I2caRegs.I2CDXR = Addr;         // Send Register to be updated.
    I2caRegs.I2CDXR = HIGH_BYTE(data);
    I2caRegs.I2CDXR = LOW_BYTE(data);

    I2caRegs.I2CMDR.all = 0x6E20;   // Set up the control register
}

void I2CA_Read(Uint16 Addr)
{
    I2cIndex = 0;
    I2caRegs.I2CCNT = 1;            // 1 Additional Byte being tranferred.
    I2caRegs.I2CDXR = Addr;     // Send Register to be updated.
    I2caRegs.I2CMDR.all = 0x6620;   // Set up the control register:

    DELAY_US(50);                   // Delay 50 usec

    I2caRegs.I2CCNT = 2;            // Set up receive of 2 bytes.
    I2caRegs.I2CMDR.all = 0x6C20;   // Send "repeated" Start with Read (TRX off)
                                        // and Stop.
}

void I2CA_Wait(void)
{
   // Wait until the STP bit is cleared from any previous master communication.
   // Clearing of this bit by the module is delayed until after the SCD bit is
   // set. If this bit is not checked prior to initiating a new message, the
   // I2C could get confused.

    while (I2caRegs.I2CMDR.bit.STP == 1); // Wait for Stop condition bit to be zero.

    while (I2caRegs.I2CSTR.bit.BB == 1);  // Wait for Bus Busy to be zero.

}

