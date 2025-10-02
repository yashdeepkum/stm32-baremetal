#include"stm32f1xx.h"

#define GPIOAEN (1U<<2)
#define PIN5 (1U<<5)
#define LED_PIN PIN5

int main(void)
{
	RCC->APB2ENR |= GPIOAEN;
	GPIOA->CRL &=~(1U<<23);
	GPIOA->CRL &=~(1U<<22);
	GPIOA->CRL |=(1U<<21);
	GPIOA->CRL &=~(1U<<20);
	while(1)
	{
		GPIOA->ODR^=LED_PIN;
		for(int i=0;i<100000;i++)
		{

		}
	}
	}
