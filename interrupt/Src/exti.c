#include "exti.h"
#include "stm32f1xx.h"


#define GPIOCEN (1U<<4)
#define AFIOEN  (1U<<0)
#define EXTI_MR (1U<<13)
#define EXTI_FTSR    (1U<<13)
void pc13_exti_init(void)
{
	//disable global interrupts
	__disable_irq();

	//enable clock access for gpioc
	RCC->APB2ENR |= GPIOCEN;

	//set pc13 as input

	//enable clock access to AFIO

	RCC->APB2ENR |= AFIOEN;
	//select portc for exti13
	   GPIOC->CRH &= ~(0xF << ((13 - 8) * 4));
	    GPIOC->CRH |=  (0x4 << ((13 - 8) * 4));  // 0b0100 = input floating

	    // Map EXTI13 to Port C
	    AFIO->EXTICR[3] &= ~(0xF << 4);
	    AFIO->EXTICR[3] |=  (0x2 << 4);
	//unmask exti13
EXTI->IMR |= EXTI_MR;
	//select falling edge trigger
EXTI->FTSR |= EXTI_FTSR;
//enble exti line in nvic
NVIC_EnableIRQ(EXTI15_10_IRQn);
	//enable global interrupt
__enable_irq();
}

