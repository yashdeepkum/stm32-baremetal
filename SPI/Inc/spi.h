#ifndef SPI_H_
#define SPI_H_


#include "stm32f1xx.h"
#include <stdint.h>
void cs_disable(void);
void cs_enable(void);
void spi_rx(uint8_t *data,uint32_t size);
void spi_tx(uint8_t *data,uint32_t size);
void spi_gpio_init(void);
void spi1_config(void);
#endif
