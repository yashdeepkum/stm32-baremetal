#include<stdio.h>
#include<stdint.h>
#include<stm32f1xx.h>
#include <uart.h>

//static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t periphclk, uint32_t baudrate );
//static uint16_t compute_uart_bd(uint32_t periphclk,uint32_t baudrate);
//void uar2_tx_init(void);
//void uart2_write(int ch);




int main(void)
{
	uart2_tx_init();
	while(1)
	{
		printf("Hello from stm32");
	}






	}
