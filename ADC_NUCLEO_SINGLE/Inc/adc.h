/*
 * uart.h
 *
 *  Created on: Sep 5, 2025
 *      Author: Asus
 */

#ifndef ADC_H_
#define ADC_H_
#include <stdint.h>

#include<stm32f1xx.h>

//void uart2_tx_init(void);
void pa1_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
