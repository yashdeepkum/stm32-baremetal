#include "stm32f1xx.h"
#include "adc.h"

/* Custom defines */
#define ADC1EN     (1U<<9)     /* ADC1 clock enable */
#define GPIOAEN    (1U<<2)     /* GPIOA clock enable */

#define ADC_CH1    (1U)        /* Channel 1 number */
#define ADC_SEQ_LEN (0U)       /* Sequence length = 1 conversion */

#define ADC_MOD_EN (1U<<0)     /* ADON: ADC enable */
#define ADC_CAL    (1U<<2)     /* CAL: Calibration */
#define SWSTART    (1U<<22)    /* Start conversion */
#define ADCEOC     (1U<<1)     /* End of conversion flag */
#define CR2_CONT    (1U<<1)
#define CR1_EOCIE    (1U<<5)
/* PA1 → ADC Channel 1 */

void pa1_adc_interrupt_init(void)
{
    /* 1) Enable clock access to GPIOA */
    RCC->APB2ENR |= GPIOAEN;

    /* 2) Set PA1 to analog mode (MODE1 = 00, CNF1 = 00) */
    /* Clear MODE1[1:0] (bits 5 and 4) */
    GPIOA->CRL &= ~(1U << 4);
    GPIOA->CRL &= ~(1U << 5);

    /* Clear CNF1[1:0] (bits 7 and 6) */
    GPIOA->CRL &= ~(1U << 6);
    GPIOA->CRL &= ~(1U << 7);
  /* Clear bits for PA1 */

    /* 3) Enable clock access to ADC1 */
    RCC->APB2ENR |= ADC1EN;
    ADC1->CR1 |=CR1_EOCIE;
    NVIC_EnableIRQ(ADC1_IRQn);

    /* 4) Configure sequence */
    ADC1->SQR1 = ADC_SEQ_LEN;   /* Sequence length = 1 */
    ADC1->SQR3 = ADC_CH1;       /* First conversion: channel 1 */

    /* 5) Enable ADC module (wake-up) */
    ADC1->CR2 |= ADC_MOD_EN;

    /* Small delay */
    for (volatile int i=0; i<1000; i++) {}

    /* 6) Calibrate ADC */
    ADC1->CR2 |= ADC_CAL;
    while (ADC1->CR2 & ADC_CAL) {}
}



void pa1_adc_init(void)
{
    /* 1) Enable clock access to GPIOA */
    RCC->APB2ENR |= GPIOAEN;

    /* 2) Set PA1 to analog mode (MODE1 = 00, CNF1 = 00) */
    /* Clear MODE1[1:0] (bits 5 and 4) */
    GPIOA->CRL &= ~(1U << 4);
    GPIOA->CRL &= ~(1U << 5);

    /* Clear CNF1[1:0] (bits 7 and 6) */
    GPIOA->CRL &= ~(1U << 6);
    GPIOA->CRL &= ~(1U << 7);
  /* Clear bits for PA1 */

    /* 3) Enable clock access to ADC1 */
    RCC->APB2ENR |= ADC1EN;

    //ENABLE ADC EOC INTERRUPT


    /* 4) Configure sequence */
    ADC1->SQR1 = ADC_SEQ_LEN;   /* Sequence length = 1 */
    ADC1->SQR3 = ADC_CH1;       /* First conversion: channel 1 */

    /* 5) Enable ADC module (wake-up) */
    ADC1->CR2 |= ADC_MOD_EN;

    /* Small delay */
    for (volatile int i=0; i<1000; i++) {}

    /* 6) Calibrate ADC */
    ADC1->CR2 |= ADC_CAL;
    while (ADC1->CR2 & ADC_CAL) {}
}

void start_conversion(void)
{   ADC1->CR2 |=CR2_CONT;
    /* Start conversion by software */
    ADC1->CR2 |= ADC_MOD_EN;  /* make sure ADC is ON */
    ADC1->CR2 |= SWSTART;
}

uint32_t adc_read(void)
{
    /* Wait for conversion complete */
    while (!(ADC1->SR & ADCEOC)) {}
    return ADC1->DR;
}
