#include "uart.h"

#define UART_BAUDRATE 9600
#define FREQ_APB2     8000000U   // 8 MHz APB2 clock

static uint16_t compute_uart_bd(uint32_t periphClk, uint32_t baudrate)
{
    return (periphClk + (baudrate/2U)) / baudrate;
}

void uart1_init(uint32_t baud)
{
    // Enable clock for GPIOA and USART1
    RCC->APB2ENR |= (1U<<2);   // IOPAEN
    RCC->APB2ENR |= (1U<<14);  // USART1EN

    // Configure PA9 as AF push-pull (TX), PA10 as input floating (RX)
    GPIOA->CRH &= ~((0xF << (4*1)) | (0xF << (4*2))); // clear CNF/MODE for PA9/PA10
    GPIOA->CRH |= (0x0B << 4);  // PA9: MODE=11 (50MHz), CNF=10 (AF PP)
    GPIOA->CRH |= (0x04 << 8);  // PA10: MODE=00, CNF=01 (input floating)

    // Baudrate
    USART1->BRR = compute_uart_bd(FREQ_APB2, baud);

    // Enable TX, RX and USART
    USART1->CR1 = (1U<<3) | (1U<<2) | (1U<<13);
}

void uart1_write(char c)
{
    while(!(USART1->SR & (1U<<7))); // wait until TXE=1
    USART1->DR = (c & 0xFF);
}

void uart1_print(const char *str)
{
    while(*str)
    {
        uart1_write(*str++);
    }
}
