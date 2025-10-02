
#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "adc.h"
#include "uart.h"

static void adc_callback(void);
uint32_t sensor_value;
int main(void)
{
     uart2_tx_init();
     pa1_adc_interrupt_init();



    // Enable clock access to GPIOA

    while (1)
    {
    	start_conversion();// Turn OFF LED
    }
}

static void  adc_callback(void)
		{
	sensor_value=adc_read();
	printf("sensor value %d \n \r",sensor_value);

		}

void ADC_IRQHandler(void)
{
//check for eoc in sr
if ((ADC1->SR & SR_EOC)!=0)
		{
	//CLEAR EOC
	ADC1->SR &=~SR_EOC;
	adc_callback();
	//
		}
}
