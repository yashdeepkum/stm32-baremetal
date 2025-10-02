
#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"

uint32_t sensor_value;


int main(void)
{
	tim2_pa0_output_cmp();



    // Enable clock access to GPIOA

    while (1)
    {//WAIT FOR UIF


        // Turn OFF LED
    }
}
