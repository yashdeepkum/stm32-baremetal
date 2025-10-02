#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "uart.h"

#define GPIOAEN     (1U<<2)   // Correct: GPIOA clock enable (bit 2)
#define GPIOA_5     (1U<<5)
#define LED_PIN     GPIOA_5

char key;
static void DMA_callback(void);
int main(void)
{

	char msg[31] = "hello from stm32 dma transfer\n\r";
	uart2_tx_init();
	dma1_channe7_init((uint32_t) msg, (uint32_t)&USART2->DR,31);
    // Enable clock access to GPIOA
    RCC->APB2ENR |= GPIOAEN;

    // Configure PA5 as output push-pull
    GPIOA->CRL &= ~(0xF << 20);   // Clear MODE5[1:0], CNF5[1:0]
    GPIOA->CRL |=  (0x2 << 20);   // MODE5 = 10 (Output 2 MHz), CNF5 = 00 (Push-pull)

    while (1)
    {


         // Turn OFF LED
    }
}
static void DMA_callback(void)
{
	GPIOA->ODR |= LED_PIN;
}
void DMA1_Channel7_IRQHandler (void){
	//CHECK FOR TRANSFER COMPLETE INTERRUP
	if (DMA1->ISR & (1U << 25))
	{
		//CLEAR FLAG
		DMA1->IFCR |= (1U << 25);

	//DO SOMETHING
		DMA_callback();
	}
}
