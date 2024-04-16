/*
 * ina238.c
 *
 *  Created on: Apr 6, 2024
 *      Author: nov4ou
 */

#include "ina238.h"

Uint16 rawRecValue = 0;
float shuntVoltage = 0.0;
float busVoltage = 0.0;
float temperature = 0.0;
float current = 0.0;
//long long fm = 2 ^ 15;
double temp;
unsigned char data[3];
Uint8 recData[2];
unsigned char tempid[2] = {0, 0};
Uint8 id[2];

Uint16 I2cIndex;
Uint16 InData[3];

struct MSG I2cMsgIn, I2cMsgOut;

void INA238_Write(struct MSG *msg)
{
    Uint16 Addr = msg->addr;
    Uint16 Data = msg->data;
    I2CA_Write(Addr, Data);
    I2CA_Wait();
}

void INA238_Init()
{
    I2CA_Init();
    // CONFIG
    I2cMsgIn.addr = CONFIG_REG;
    I2cMsgIn.data = 0x0010;
    INA238_Write(&I2cMsgIn);

    // ADC CONFIG
    I2cMsgIn.addr = ADC_CONFIG;
    I2cMsgIn.data = 0xF002;
    INA238_Write(&I2cMsgIn);

    // ShUNT_CAL
//    I2cMsgIn.addr = SHUNT_CALIBRATION_REG;
//    I2cMsgIn.data = 500;
//    INA238_Write(&I2cMsgIn);
}

void INA238_Read(struct MSG *msg)
{
    Uint16 Addr = msg->addr;
    I2CA_Read(Addr);
    I2CA_Wait();
}

void INA238_Read_Manufacturer_ID()
{
    I2cMsgOut.addr = MANUFACTURER_ID_REG;
    INA238_Read(&I2cMsgOut);
    id[0] = InData[0];
    id[1] = InData[1];
}

void INA238_Read_Shunt_Voltage()
{
    I2cMsgOut.addr = SHUNT_VOLTAGE_REG;
    INA238_Read(&I2cMsgOut);
    rawRecValue = (InData[0] << 8) | InData[1];
    shuntVoltage = rawRecValue * 1.25 / 1000;
}

void INA238_Read_Bus_Voltage()
{
    I2cMsgOut.addr = BUS_VOLTAGE_REG;
    INA238_Read(&I2cMsgOut);
    rawRecValue = (InData[0] << 8) | InData[1];
    busVoltage = rawRecValue * 3.125 / 1000;
}

void INA238_Read_Current()
{

    I2cMsgOut.addr = CURRENT_REG;
    INA238_Read(&I2cMsgOut);
    rawRecValue = (InData[0] << 8) | InData[1];
//    current = rawRecValue * 0.0000610351563;
    current = rawRecValue * 0.0000305175781;
//    temp = 2.0 / fm;
//    current = rawRecValue * temp;
}

void INA238_Read_Temperature()
{
    I2cMsgOut.addr = TEMP_REG;
    INA238_Read(&I2cMsgOut);
    rawRecValue = (InData[0] << 8) | InData[1];
    rawRecValue = rawRecValue >> 4;
    temperature = rawRecValue * 125.0 / 1000;
}

interrupt void i2c_int1a_isr(void)     // I2C-A
{
    Uint16 IntSource;

   // Read interrupt source
    IntSource = I2caRegs.I2CISRC.bit.INTCODE & 0x7;

    switch(IntSource)
    {
        case I2C_NO_ISRC:   // =0
            break;

        case I2C_ARB_ISRC:  // =1
            break;

        case I2C_NACK_ISRC: // =2
            break;

        case I2C_ARDY_ISRC: // =3
            break;

        case I2C_RX_ISRC:   // =4
            InData[I2cIndex++] = I2caRegs.I2CDRR;
            break;

        case I2C_TX_ISRC:   // =5
            break;

        case I2C_SCD_ISRC:  // =6
            break;

        case I2C_AAS_ISRC:  // =7
            break;

        default:
            asm("   ESTOP0"); // Halt on invalid number.
    }

   // Enable future I2C (PIE Group 8) interrupts
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP8;
}
