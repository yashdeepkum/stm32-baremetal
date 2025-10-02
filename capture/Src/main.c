
#include <stdio.h>
#include <stdint.h>
#include "stm32f1xx.h"
#include "adc.h"
#include "uart.h"
#include "systick.h"
#include "tim.h"


volatile int timestamp=0;
//connect jumper wire from pa0 to pa6
int main(void)
{
	tim2_pa0_output_cmp();
	tim3_input_capture();


    // Enable clock access to GPIOA

    while (1)
    {//wait until edge is captured
    	while(!(TIM3->SR & SR_CC1IF)){}
    	//READ VALUE
    	timestamp=TIM3->CCR1;



        // Turn OFF LED
    }
}
