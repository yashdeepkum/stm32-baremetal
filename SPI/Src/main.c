#include "stm32f1xx.h"
#include "i2c.h"
#include "accl.h"
#include "uart.h"
#include <stdio.h>

char buffer[80];

int main(void)
{
    int16_t ax, ay, az;
    int16_t gx, gy, gz;

    // Init peripherals
    uart1_init(9600);     // UART for debugging
    i2c1_init();          // I2C1 on PB8/PB9
    mpu6050_init();       // Wake up MPU6050

    uart1_print("MPU6050 Ready...\r\n");

    while (1)
    {
        // Read sensors
        mpu6050_read_accel(&ax, &ay, &az);
        mpu6050_read_gyro(&gx, &gy, &gz);

        // Format data into buffer
        snprintf(buffer, sizeof(buffer),
                 "AX=%d AY=%d AZ=%d | GX=%d GY=%d GZ=%d\r\n",
                 ax, ay, az, gx, gy, gz);

        // Send via UART
        uart1_print(buffer);

        // crude delay ~100ms
        for (volatile int i = 0; i < 800000; i++);
    }
}
