
#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"
uint32_t sensor_value;

#define GPIOAEN (1U<<2)
#define PIN5    (1U<<5)
#define LED_PIN  PIN5

int main(void)
{
	RCC->APB2ENR |= GPIOAEN;
		GPIOA->CRL &=~(1U<<23);
		GPIOA->CRL &=~(1U<<22);
		GPIOA->CRL |=(1U<<21);
		GPIOA->CRL &=~(1U<<20);
     uart2_tx_init();

     systick_1hz_interrupt();

    // Enable clock access to GPIOA

    while (1)
    {



        // Turn OFF LED
    }
}
void systick_callback(void)
{
	printf("a second passed");
    	GPIOA->ODR^=LED_PIN;
}
void SysTick_Handler(void)
{
systick_callback();
}
