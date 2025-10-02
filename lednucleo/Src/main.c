#define PERIPH_BASE (0x40000000UL)

#define APB2_OFFSET (0x0x00010000UL)
#define APB2_BASE (PERIPH_BASE+APB2_OFFSET)

#define GPIOA_OFFSET (0x00010800UL)
#define GPIOA_BASE   (PERIPH_BASE+GPIOA_OFFSET)

#define AHB_OFFSET (0x00018000 UL)
#define AHB_BASE   (PERIPH_BASE+AHB_OFFSET)

#define RCC_OFFSET (0x00021000UL)
#define RCC_BASE   (PERIPH_BASE+RCC_OFFSET)

#define RCC_APB2ENR_OFFSET (0x18UL)
#define RCC_APB2ENR (*(volatile unsigned long *)(RCC_BASE+RCC_APB2ENR_OFFSET))


#define GPIOA_CRL_OFFSET (0x00UL)
#define GPIOA_CRL_BASE   (*(volatile unsigned long *)(GPIOA_BASE+GPIOA_CRL_OFFSET))

#define GPIOA_ODR_OFFSET (0x0CUL)
#define GPIOA_ODR_BASE   (*(volatile unsigned long *)(GPIOA_BASE+GPIOA_ODR_OFFSET))

#define GPIOAEN (1U<<2)
#define LED_PIN (1U<<5)

int main(void)
{
	RCC_APB2ENR |= GPIOAEN;
	GPIOA_CRL_BASE &= ~(1U<<23);
	GPIOA_CRL_BASE &=~(1U<<22);
	GPIOA_CRL_BASE |= (1U<<21);
	GPIOA_CRL_BASE &=~(1U<<20);

	while (1)
	{
		GPIOA_ODR_BASE ^=LED_PIN;
		for (int i=0;i<=100000;i++)
		{}
	}
	}
