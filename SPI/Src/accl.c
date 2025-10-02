#include "accl.h"
#include "stm32f1xx.h"
#include "i2c.h"   // your I2C driver header

// ---------------- MPU6050 REGISTER MAP ---------------- //
#define MPU6050_ADDR        0x68
#define PWR_MGMT_1          0x6B
#define ACCEL_XOUT_H        0x3B
#define GYRO_XOUT_H         0x43

// ---------------- MPU6050 DRIVER ---------------- //
void mpu6050_init(void)
{
    char data;

    // Wake up MPU6050 (clear sleep bit)
    data = 0x00;
    i2c1_burstwrite(MPU6050_ADDR, PWR_MGMT_1, 1, &data);
}

void mpu6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az)
{
    char rawData[6];

    // Burst read 6 bytes starting at ACCEL_XOUT_H
    i2c1_burstread(MPU6050_ADDR, ACCEL_XOUT_H, 6, rawData);

    *ax = (int16_t)( (rawData[0] << 8) | rawData[1] );
    *ay = (int16_t)( (rawData[2] << 8) | rawData[3] );
    *az = (int16_t)( (rawData[4] << 8) | rawData[5] );
}

void mpu6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz)
{
    char rawData[6];

    // Burst read 6 bytes starting at GYRO_XOUT_H
    i2c1_burstread(MPU6050_ADDR, GYRO_XOUT_H, 6, rawData);

    *gx = (int16_t)( (rawData[0] << 8) | rawData[1] );
    *gy = (int16_t)( (rawData[2] << 8) | rawData[3] );
    *gz = (int16_t)( (rawData[4] << 8) | rawData[5] );
}
