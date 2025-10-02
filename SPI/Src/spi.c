#include "spi.h"
#include "stm32f1xx.h"
#define SPI_EN (1U<<12)
#define AFIOEN   (1U<<0)
#define GPIOAEN   (1U<<2)
#define CPOL      (1U<<0)
#define CPHA      (1U<<1)
#define RXONLY     (1U<<10)
#define MSBF       (1U<<7)
#define MSTR       (1U<<2)
#define DFF        (1U<<11)
#define SSM        (1U<<9)
#define SSI        (1U<<8)
#define SPE         (1U<<6)
#define TXE (1U<<1)
#define BSY (1U<<7)
#define RXNE (1U<<0)
//SPI NSS PA4
//SPI SCK PA5
//SPI MISO PA6
//SPI MOSI PA7

void spi_gpio_init(void)
{ //ENABLE clk access to gpioa
	RCC->APB2ENR |= GPIOAEN;
	//ENABLE CLK FOR ALTERNATE FUNCTION
	RCC->APB2ENR |= AFIOEN;
	 //PA4
	GPIOA->CRL |=  (1U<<16);
	    GPIOA->CRL |=  (1U<<17);
	    GPIOA->CRL &= ~(1U<<18);
	    GPIOA->CRL &= ~(1U<<19);

	    // --- PA5 (SCK) = AF push-pull, 50 MHz ---
	    GPIOA->CRL |=  (1U<<20);
	    GPIOA->CRL |=  (1U<<21);
	    GPIOA->CRL |=  (1U<<22);
	    GPIOA->CRL &= ~(1U<<23);

	    // --- PA6 (MISO) = Input floating ---
	    GPIOA->CRL &= ~(1U<<24);
	    GPIOA->CRL &= ~(1U<<25);
	    GPIOA->CRL |=  (1U<<26);
	    GPIOA->CRL &= ~(1U<<27);

	    // --- PA7 (MOSI) = AF push-pull, 50 MHz ---
	    GPIOA->CRL |=  (1U<<28);
	    GPIOA->CRL |=  (1U<<29);
	    GPIOA->CRL |=  (1U<<30);
	    GPIOA->CRL &= ~(1U<<31);


}
void spi1_config(void)
{ //ENABLE CLK FOR SPI1
	RCC->APB2ENR |= SPI_EN;
	//FCLK/4
	     SPI1->CR1 &= ~(1U<<5);
	     SPI1->CR1  &= ~ (1U<<4);
	     SPI1->CR1 |= (1U<<3);
	     //CLK POLARITY TO 1 AND CLK PHASE TO 1
	     SPI1->CR1 |= CPOL;
	     SPI1->CR1 |=CPHA;
	     //FULL DUPLEX MODE
	     SPI1->CR1 &=~ RXONLY;
	    //SET MSB FIRST
	     SPI1->CR1 &=~ MSBF;
	     //SPI MASTER MODE
	     SPI1->CR1 |= MSTR;
	     //DATA FORMAT TO BE 8 BIT
	     SPI1->CR1 &=~ DFF;
	     //SELECT SOFTWARE SLAVE MANAGEMENT BY SETTING SSM1 AND SSI1
	     SPI1->CR1 |= SSM;
	     SPI1->CR1 |= SSI;
	     //ENABLE SPI MODULE
	     SPI1->CR1 |= SPE;
}
void spi_tx(uint8_t *data,uint32_t size)
{
	uint32_t i=0;
	uint8_t tmp;
	while (i<size)
	{//wait till txe is set
	 while (!(SPI1->SR & TXE)){}

		 //write the data to the data register
		 SPI1->DR = data[i];
		 i++;//data is pointer to buffer
	 }
	 //wait until txe is set
	 while (!(SPI1->SR & TXE)){}
	//wait for busy flag is reset
		 while ((SPI1->SR & BSY)){}
//CLEAR OVERRUN FLAG
		tmp = SPI1->DR;
		tmp = SPI1->SR;



}
void spi_rx(uint8_t *data,uint32_t size){
	while (size)
	{
		//send dummy data
		SPI1->DR =0;
		//WAIT UNTIL RXNE IS SET
		while (!(SPI1->SR & RXNE)){}
		//READ DATA FROM DATA REGISTER

		*data++ = (SPI1->DR);
		size--;
	}
}
void cs_enable(void)
{
	GPIOA->ODR &=~(1U<<9);

}
//pull high to disable

void cs_disable(void)
{
GPIOA->ODR |=(1U<<9);
}


