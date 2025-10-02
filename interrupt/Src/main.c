
#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"

#define GPIOAEN (1U<<2)
#define PIN5    (1U<<5)
#define LED_PIN  PIN5

//connect juper wire from pa0 to pa6
int main(void)
{
	RCC->APB2ENR |= GPIOAEN;
	GPIOA->CRL &= ~(0xF << (5 * 4));
	// Set MODE5 = 0b10 (output 2MHz), CNF5 = 0b00 (GP push-pull)
	GPIOA->CRL |=  (0x2 << (5 * 4));
	     uart2_tx_init();

	pc13_exti_init();


    while (1)
    {//wait until edge is captured


        // Turn OFF LED
    }
}

static void exti_callback(void)
{
printf("btn is pressed")	;
GPIOA->ODR^=LED_PIN;
}
void EXTI15_10_IRQHANDLER(void)
{
	if((EXTI->PR & LINE13)!=0)
		//clear the pr flag
		EXTI->PR |=LINE13;
        //do something
	exti_callback();
}
