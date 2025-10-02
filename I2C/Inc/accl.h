#ifndef ACCL_H
#define ACCL_H

#include <stdint.h>

// MPU6050 public API
void mpu6050_init(void);
void mpu6050_read_accel(int16_t* ax, int16_t* ay, int16_t* az);
void mpu6050_read_gyro(int16_t* gx, int16_t* gy, int16_t* gz);

#endif
