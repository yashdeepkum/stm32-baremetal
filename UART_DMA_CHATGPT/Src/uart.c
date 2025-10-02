#include "stm32f1xx.h"
#include "uart.h"

/* ---------------- Defines ---------------- */
#define GPIOAEN     (1U<<2)    // GPIOA clock enable
#define AFIOEN      (1U<<0)    // AFIO clock enable
#define UART2EN     (1U<<17)   // USART2 clock enable
#define DMA1EN      (1U<<0)    // DMA1 clock enable

#define CR1_TE      (1U<<3)
#define CR1_RE      (1U<<2)
#define CR1_UE      (1U<<13)

#define SR_TXE      (1U<<7)
#define SR_RXNE     (1U<<5)

#define DMA_CCR_MINC   (1U<<7)
#define DMA_CCR_DIR    (1U<<4)
#define DMA_CCR_TCIE   (1U<<1)
#define DMA_CCR_EN     (1U<<0)

#define DMA_ISR_TCIF7  (1U<<25)   // Transfer complete flag for Channel 7
#define DMA_IFCR_CTCIF7 (1U<<25)  // Clear transfer complete flag

/* ---------------- Clock Setup ---------------- */
void clock_config(void)
{
    /* Enable HSE */
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)) {}

    /* PLL = HSE × 9 → 72 MHz */
    RCC->CFGR |= RCC_CFGR_PLLSRC;
    RCC->CFGR |= RCC_CFGR_PLLMULL9;
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {}

    /* Flash latency */
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    /* Prescalers */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  // APB1 = 36 MHz

    /* Switch SYSCLK to PLL */
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {}
}

/* ---------------- Baudrate Helpers ---------------- */
static uint16_t compute_uart_bd(uint32_t periphclk, uint32_t baudrate)
{
    return (periphclk + (baudrate/2U)) / baudrate;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate)
{
    USARTx->BRR = compute_uart_bd(periphclk, baudrate);
}

/* ---------------- UART Init ---------------- */
void uart2_tx_init(void)
{
    /* Enable GPIOA + AFIO */
    RCC->APB2ENR |= GPIOAEN | AFIOEN;

    /* Enable USART2 */
    RCC->APB1ENR |= UART2EN;

    /* PA2 = TX (AF PP, 50 MHz) */
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL |=  (0xB << 8);

    /* PA3 = RX (input floating) */
    GPIOA->CRL &= ~(0xF << 12);
    GPIOA->CRL |=  (0x4 << 12);

    /* Baudrate = 115200 @ 36 MHz */
    uart_set_baudrate(USART2, 36000000U, 115200U);

    /* Enable TX, RX, USART */
    USART2->CR1 = CR1_TE | CR1_RE;
    USART2->CR1 |= CR1_UE;
}

/* ---------------- Blocking TX/RX ---------------- */
void uart2_write(int ch)
{
    while (!(USART2->SR & SR_TXE)) {}
    USART2->DR = (ch & 0xFF);
}

char uart2_read(void)
{
    while (!(USART2->SR & SR_RXNE)) {}
    return USART2->DR;
}

/* printf redirection */
int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

/* ---------------- DMA TX Support ---------------- */
void uart2_tx_dma_init(uint32_t srcAddr, uint32_t length)
{
    /* Enable DMA1 clock */
    RCC->AHBENR |= DMA1EN;

    /* Enable USART2 TX DMA */
    USART2->CR3 |= USART_CR3_DMAT;

    /* Disable channel before config */
    DMA1_Channel7->CCR &= ~DMA_CCR_EN;

    /* Source = memory */
    DMA1_Channel7->CMAR = srcAddr;

    /* Destination = USART2->DR */
    DMA1_Channel7->CPAR = (uint32_t)&USART2->DR;

    /* Number of data */
    DMA1_Channel7->CNDTR = length;

    /* Config: mem->periph, increment mem, 8-bit */
    DMA1_Channel7->CCR = DMA_CCR_MINC | DMA_CCR_DIR | DMA_CCR_TCIE;

    /* Enable DMA IRQ in NVIC */
    NVIC_EnableIRQ(DMA1_Channel7_IRQn);

    /* Start DMA */
    DMA1_Channel7->CCR |= DMA_CCR_EN;
}

/* Weak DMA callback (user can override) */
__attribute__((weak)) void dma_callback(void)
{
    // default: do nothing
}

/* DMA1 Channel7 IRQ Handler */
void DMA1_Channel7_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF7)
    {
        /* Clear flag */
        DMA1->IFCR |= DMA_IFCR_CTCIF7;

        /* Disable DMA */
        DMA1_Channel7->CCR &= ~DMA_CCR_EN;

        /* Call user callback */
        dma_callback();
    }
}
