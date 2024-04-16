/*
 * iic.h
 *
 *  Created on: Apr 6, 2024
 *      Author: nov4ou
 */

#ifndef APP_IIC_IIC_H_
#define APP_IIC_IIC_H_

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File


void I2CA_Init(void);
void I2CA_Write(Uint16 Addr, Uint16 data);
void I2CA_Read(Uint16 Addr);
void I2CA_Wait(void);



#endif /* APP_IIC_IIC_H_ */
