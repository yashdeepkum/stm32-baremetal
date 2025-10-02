#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "uart.h"

#define GPIOAEN   (1U<<2)
#define GPIOA_5   (1U<<5)
#define LED_PIN   GPIOA_5

volatile char key;   // make it volatile so debugger updates live

int main(void)
{
    uart2_rxtx_init();

    // Enable GPIOA clock
    RCC->APB2ENR |= GPIOAEN;

    // Configure PA5 as output push-pull
    GPIOA->CRL &= ~(0xF << 20);
    GPIOA->CRL |=  (0x2 << 20);   // Output 2 MHz, push-pull

    while (1)
    {
        key = uart2_read();   // blocking read

        if (key == '1')
            GPIOA->ODR |= LED_PIN;    // Turn ON LED
        else if (key == '0')
            GPIOA->ODR &= ~LED_PIN;   // Turn OFF LED

        // Optional: send feedback back to RealTerm
        uart2_write(key);
        uart2_write('\r');
        uart2_write('\n');
    }
}
