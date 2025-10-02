/*
 * uart.h
 *
 *  Created on: Sep 5, 2025
 *      Author: Asus
 */

#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include<stm32f1xx.h>
#define SR_RXNE      (1U<<5)
#define TCIE         (1U<<1)
#define CTCIF7       (1U<<25)
//void uart2_tx_init(void);
char uart2_read(void);
void uart2_tx_init(void);
void uart2_rxtx_init(void);
void uart2_rxtx_interrupt(void);

void dma1_channe7_init(uint32_t src, uint32_t dst,uint32_t len);
#endif /* UART_H_ */
