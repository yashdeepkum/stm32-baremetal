
#include "stm32f1Xx.h"

#define GPIOAEN     (1U<<2)   // Enable clock for GPIOA
#define AFIOEN      (1U<<0)   // Enable clock for AFIO
#define UART2EN     (1U<<17)  // Enable clock for USART2

#define SYS_FREQ    8000000U
#define APB1_CLK    SYS_FREQ
#define UART_BAUDRATE 9600U

#define CR1_TE      (1U<<3)
#define CR1_UE      (1U<<13)
#define SR_TXE      (1U<<7)

#define CR1_RE       (1U<<2)
#define SR_RXNE      (1U<<5)

#define CR1_RXNEIE    (1U<<5)
#define DMA1EN        (1U<<0)
#define DMACEN         (1U<<0)
#define MINC            (1U<<7)
#define DTD              (1U<<4)
#define TCIE              (1U<<1)
#define DMAT              (1U<<7)

static uint16_t compute_uart_bd(uint32_t periphclk, uint32_t baudrate)
{
    return (periphclk + (baudrate/2U)) / baudrate;
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate)
{
    USARTx->BRR = 833;
}

void uart2_write(int ch)
{
    // wait until transmit data register is empty
    while (!(USART2->SR & SR_TXE)) {}
    // write to data register
    USART2->DR = (ch & 0xFF);
}

char uart2_read(void)
{ while (!(USART2->SR & SR_RXNE)) {}
	//READ DATA
return USART2-> DR;

	}


void dma1_channe7_init(uint32_t src, uint32_t dst,uint32_t len)

{
//enable clk access to dma
	RCC->AHBENR |= DMA1EN;
	//disable dma1  channel7
	DMA1_Channel7->CCR &=~DMACEN;
	//wait until dma is disabled
	while(DMA1_Channel7->CCR & DMACEN){}

	//clear all interrupt flags of channel7
	DMA1->IFCR |=(1U<<24);
	DMA1->IFCR |=(1U<<25);
		DMA1->IFCR |=(1U<<26);
		DMA1->IFCR |=(1U<<27);
	//set the dest buffer
		DMA1_Channel7->CPAR =dst;
	//set the source buffer
		DMA1_Channel7->CMAR =src;
	//set the length
		DMA1_Channel7->CNDTR =len;


	//enable memory increment
		DMA1_Channel7->CCR |= MINC;
	//configure direction i.e.memory array to peripheral
		DMA1_Channel7->CCR |= DTD ;
		//ENABLE TRANSFER COMPLETE INTERRUPT
		DMA1_Channel7->CCR |=TCIE;



	//enable uart2 tx dma
		USART2->CR3 |= DMAT;
	//dma interrupt enable in nvic
		NVIC_EnableIRQ(DMA1_Channel7_IRQn);


}


void uart2_tx_init(void)
{
    /************* Configure UART GPIO pin ************/

    // Enable clock access to GPIOA & AFIO
    RCC->APB2ENR |= GPIOAEN;
    RCC->APB2ENR |= AFIOEN;

    // Set PA2 to alternate function push-pull
    // PA2 is configured in CRL (bits [11:8])
    GPIOA->CRL |=  (1U << 8);   // MODE2_0 = 1
    GPIOA->CRL |=  (1U << 9);   // MODE2_1 = 1
    // CNF2_0 = 0 → already cleared
    GPIOA->CRL |=  (1U << 11);
    GPIOA->CRL &=  ~(1U << 10);
    // MODE2 = 11 (Output 50 MHz), CNF2 = 10 (AF push-pull)

    /************* Configure UART module ************/

    // Enable clock access to USART2
    RCC->APB1ENR |= UART2EN;

    // Set baud rate
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    // Configure transfer direction (transmitter only)
    USART2->CR1 = CR1_TE;

    // Enable UART module
    USART2->CR1 |= CR1_UE;
}



void uart2_rxtx_init(void)
{
    /************* Configure UART GPIO pin ************/

    // Enable clock access to GPIOA & AFIO
    RCC->APB2ENR |= GPIOAEN;
    RCC->APB2ENR |= AFIOEN;

    // Set PA2 to alternate function push-pull
    // PA2 is configured in CRL (bits [11:8])
    GPIOA->CRL |=  (1U << 8);   // MODE2_0 = 1
        GPIOA->CRL |=  (1U << 9);   // MODE2_1 = 1
        // CNF2_0 = 0 → already cleared
        GPIOA->CRL |=  (1U << 11);
        GPIOA->CRL &=  ~(1U << 10); // MODE2 = 11 (Output 50 MHz), CNF2 = 10 (AF push-pull)

        //PA3 is configured as uart mode
        GPIOA->CRL &= ~(1U << 12);   // MODE3_0 = 0
        GPIOA->CRL &= ~(1U << 13);   // MODE3_1 = 0
        GPIOA->CRL |=  (1U << 14);   // CNF3_0 = 1
        GPIOA->CRL &= ~(1U << 15);
           /************* Configure UART module ************/

    // Enable clock access to USART2
    RCC->APB1ENR |= UART2EN;

    // Set baud rate
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    // Configure transfer direction (transmitter only)
    USART2->CR1 = CR1_TE;
    USART2->CR1 |=  CR1_RE;
    // Enable UART module
    USART2->CR1 |= CR1_UE;
}

int __io_putchar(int ch)
{
    uart2_write(ch);
    return ch;
}

void uart2_rxtx_interrupt(void)
{
    /************* Configure UART GPIO pin ************/

    // Enable clock access to GPIOA & AFIO
    RCC->APB2ENR |= GPIOAEN;
    RCC->APB2ENR |= AFIOEN;

    // Set PA2 to alternate function push-pull
    // PA2 is configured in CRL (bits [11:8])
    GPIOA->CRL |=  (1U << 8);   // MODE2_0 = 1
        GPIOA->CRL |=  (1U << 9);   // MODE2_1 = 1
        // CNF2_0 = 0 → already cleared
        GPIOA->CRL |=  (1U << 11);
        GPIOA->CRL &=  ~(1U << 10); // MODE2 = 11 (Output 50 MHz), CNF2 = 10 (AF push-pull)

        //PA3 is configured as uart mode
        GPIOA->CRL &= ~(1U << 12);   // MODE3_0 = 0
        GPIOA->CRL &= ~(1U << 13);   // MODE3_1 = 0
        GPIOA->CRL |=  (1U << 14);   // CNF3_0 = 1
        GPIOA->CRL &= ~(1U << 15);
           /************* Configure UART module ************/

    // Enable clock access to USART2
    RCC->APB1ENR |= UART2EN;

    // Set baud rate
    uart_set_baudrate(USART2, APB1_CLK, UART_BAUDRATE);

    // Configure transfer direction (transmitter only)
    USART2->CR1 = CR1_TE;
    USART2->CR1 |=  CR1_RE;
    //ENABLE RXNE INTERRUPT
    USART2->CR1 |= CR1_RXNEIE ;
//ENABLE UART2 IN NVIC
    NVIC_EnableIRQ(USART2_IRQn);
    // Enable UART module
    USART2->CR1 |= CR1_UE;
}
