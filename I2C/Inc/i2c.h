/*
 * i2c.h
 *
 *  Created on: Sep 15, 2025
 *      Author: Asus
 */

#ifndef I2C_H_
#define I2C_H_
void i2c1_init(void);
void i2c1_byteread(char saddr,char maddr,char* data);
void i2c1_burstread(char saddr,char maddr,int n,char* data);
void i2c1_burstwrite(char saddr,char maddr,int n,char* data);

#endif /* I2C_H_ */
