#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void clock_config(void);
void uart2_tx_init(void);
void uart2_write(int ch);
char uart2_read(void);

/* DMA-based transmit */
void uart2_tx_dma_init(uint32_t srcAddr, uint32_t length);
void dma_callback(void);   // user can override

#endif /* UART_H_ */
