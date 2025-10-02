#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "uart.h"

#define GPIOAEN     (1U<<2)   // Correct: GPIOA clock enable (bit 2)
#define GPIOA_5     (1U<<5)
#define LED_PIN     GPIOA_5

char key;

int main(void)
{
	void uart2_rxtx_interrupt(void);

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
static void uart_callback(void)
{
	key= USART2->DR;
		if (key == '1')
		            GPIOA->ODR |= LED_PIN;   // Turn ON LED
		        else
		            GPIOA->ODR &= ~LED_PIN;
}
void USART2_IRQHandler(void)
{//CHECK IF RXNE IS SET
if(USART2->SR & SR_RXNE)
{
	 uart_callback();
	}
}
