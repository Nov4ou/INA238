/*
 * ina238.h
 *
 *  Created on: Apr 6, 2024
 *      Author: nov4ou
 */

#ifndef APP_INA238_INA238_H_
#define APP_INA238_INA238_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "iic.h"


#define I2C_SLAVE_ADDR        0x41
#define I2C_NUMBYTES          2
//#define I2C_EEPROM_HIGH_ADDR  0x00
//#define I2C_EEPROM_LOW_ADDR   0x30
#define CONFIG_REG            0x00
#define ADC_CONFIG            0x01
#define SHUNT_VOLTAGE_REG     0x04
#define BUS_VOLTAGE_REG       0x05
#define SHUNT_CALIBRATION_REG 0x02
#define CURRENT_REG           0x07
#define MANUFACTURER_ID_REG   0x3E
#define TEMP_REG              0x06

#define HIGH_BYTE_MASK        0xFF00
#define LOW_BYTE_MASK         0x00FF


#define CONFIG_RST            0x01
#define CONFIG_CONVDLY_0      0x00
#define CONFIG_CONVDLY_2      0x01
#define CONFIG_CONVDLY_510    0xFF
#define ADCRANGE_163_84       0x00
#define ADCRANGE_40_96        0x01


struct MSG {
    Uint16 addr;
    Uint16 data;
};


void INA238_Write(struct MSG *msg);
void INA238_Init();
void INA238_Read(struct MSG *msg);
void INA238_Read_Manufacturer_ID();
void INA238_Read_Shunt_Voltage();
void INA238_Read_Bus_Voltage();
void INA238_Read_Current();
void INA238_Read_Temperature();

interrupt void i2c_int1a_isr(void);


//void INA238_Read_Manufacturer_ID();
//void INA238_Reset();
//void INA238_Read_Shunt_Voltage();
//void INA238_Read_Bus_Voltage();
//void INA238_Read_Temperature();


#endif /* APP_INA238_INA238_H_ */
