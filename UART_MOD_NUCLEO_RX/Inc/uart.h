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

//void uart2_tx_init(void);
char uart2_read(void);
void uart2_rxtx_init(void);


#endif /* UART_H_ */
