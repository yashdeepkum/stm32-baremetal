
#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "adc.h"
#include "uart.h"

uint32_t sensor_value;
int main(void)
{
     uart2_tx_init();
     pa1_adc_init();


    // Enable clock access to GPIOA

    while (1)
    {start_conversion();
    	sensor_value=adc_read();
    	printf("sensor value %d \n \r",sensor_value);

        // Turn OFF LED
    }
}
