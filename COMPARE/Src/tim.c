#include "stm32f1xx.h"
#define TIM2EN   (1U<<0)
#define CR1_CEN (1U<<0)
#define OC_TOGGLE (1U<<4) | (1U<<5)
#define CCER_CC1E (1U<<0)
#define AFIOEN     (1U<<0)
#define GPIOAEN     (1U<<2)
void tim2_1hz_init(void)
{ //in my case led is not on pa5 like video so connect external led on pa0
	//enable clk access to tim2
	RCC->APB1ENR |= TIM2EN;

	//set prescaler value
	TIM2->PSC = 1600 - 1; //16 000 000/1600=10000
	//set auto reload value
	TIM2->ARR = 10000 - 1;//10000/10000=1
	//clear counter
	TIM2->CNT =0;

	//enable timer
	TIM2->CR1 = CR1_CEN;
}

void tim2_pa0_output_cmp(void)
{
	//enable clk access to tim2
	RCC->APB1ENR |= TIM2EN;
	//enable clock acess to gpioa
	RCC->APB2ENR |= GPIOAEN;
	//enable alternate function clock
	RCC->APB2ENR |= AFIOEN;
	GPIOA->CRL |=(1U<<3);
		GPIOA->CRL &=~(1U<<2);
		GPIOA->CRL |=(1U<<1);
		GPIOA->CRL |=(1U<<0);
	//set prescaler value
	TIM2->PSC = 1600 - 1; //16 000 000/1600=10000
	//set auto reload value
	TIM2->ARR = 10000 - 1;//10000/10000=1

	 //set output compare toggle mode
	 TIM2->CCMR1 =OC_TOGGLE;

	//enable tim2 ch1 in compare mode
	TIM2->CCER =CCER_CC1E;//BIT 0 FOR USING CHANNEL 1
	 //clear counter
	TIM2->CNT =0;

	//enable timer
	TIM2->CR1 = CR1_CEN;
}
