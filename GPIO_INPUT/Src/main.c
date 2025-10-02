#include"stm32f1xx.h"

#define GPIOAEN (1U<<2)
#define GPIOCEN (1U<<4)
#define PIN5 (1U<<5)
#define LED_PIN PIN5
#define PIN13 (1U<<13)
#define LED_PIN PIN5
#define BTN_PIN PIN13
int main(void)
{
	RCC->APB2ENR |= GPIOAEN;
	RCC->APB2ENR |=GPIOCEN;
	//CNF5 23:22 = 00
	//MODE5 21:20 = 01
	GPIOA->CRL &=~(1U<<23);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL &=~(1U<<21);
	GPIOA->CRL |=(1U<<20);
    //CNF13 23:22=10
    //MODE13 21:20=00

	GPIOC->CRH |=(1U<<23);
	GPIOC->CRH &=~(1U<<22);
	GPIOC->CRH &=~(1U<<21);
	GPIOC->CRH &=~(1U<<20);


	 while (1)
	    {
	        if ((GPIOC->IDR & (1 << 13)) == 0)   // Button pressed (active low)
	        {
	            GPIOA->BSRR = (1 << 5);  // LED ON
	        }
	        else
	        {
	            GPIOA->BRR = (1 << 5);   // LED OFF
	        }
	    }
	}
