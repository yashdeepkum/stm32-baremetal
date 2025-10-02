/*
 * tim.h
 *
 *  Created on: Sep 9, 2025
 *      Author: Asus
 */

#ifndef TIM_H_
#define TIM_H_

void tim2_1hz_init(void);
void tim2_pa0_output_cmp(void);
void tim3_input_capture(void);


#define SR_UIF    (1U<<0)
#define SR_CC1IF   (1U<<1)
#endif /* TIM_H_ */
