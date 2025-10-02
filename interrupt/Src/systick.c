#include "stm32f1xx.h"
#define SYSTICK_LOAD_VAL 16000
#define CTRL_ENABLE      (1U<<0)
#define CTRL_CLKSRC       (1U<<2)
#define CTRL_CNTFLAG      (1U<<16)




void systickdelayms(int delay)
{//configure systick
    //reload with number of clock per ms
	SysTick->LOAD = SYSTICK_LOAD_VAL;
    	//clear systick current value reg
	SysTick->VAL = 0;
    	//enable systick and select internal clk src
    	SysTick->CTRL = CTRL_ENABLE | CTRL_CLKSRC;
 for (int i=0;i<delay;i++){
	 //wait until the COUNTFLAG is set
	 while ((SysTick->CTRL& CTRL_CNTFLAG)==0){}

 }
 SysTick->CTRL=0;
}
