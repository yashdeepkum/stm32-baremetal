#include "stm32f1xx.h"
#include "uart.h"
#include <stdio.h>

char msg[] = "Hello via DMA on USART2!\r\n";

void dma_callback(void)
{
    /* Toggle PA5 LED on DMA complete */
    RCC->APB2ENR |= (1U<<2);        // enable GPIOA
    GPIOA->CRL &= ~(0xF << 20);     // PA5 output
    GPIOA->CRL |=  (0x2 << 20);
    GPIOA->ODR ^= (1U<<5);          // toggle LED
}

int main(void)
{
    clock_config();
    uart2_tx_init();

    /* Blocking printf */
    printf("UART ready...\r\n");

    /* Send via DMA */
    uart2_tx_dma_init((uint32_t)msg, sizeof(msg)-1);

    while (1)
    {
        // main loop can do other tasks
    }
}
