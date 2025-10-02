#ifndef UART_H
#define UART_H

#include "stm32f1xx.h"

void uart1_init(uint32_t baud);
void uart1_write(char c);
void uart1_print(const char *str);

#endif
