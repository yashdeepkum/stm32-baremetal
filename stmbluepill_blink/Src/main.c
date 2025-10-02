// Bare-metal LED Blink for STM32F103C8 (Blue Pill)
// LED is on PC13 (active low)

#define PERIPH_BASE        0x40000000UL
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE     (PERIPH_BASE + 0x20000)

#define RCC_BASE           (AHBPERIPH_BASE + 0x1000)
#define GPIOC_BASE         (APB2PERIPH_BASE + 0x1000)

#define RCC_APB2ENR        (*(volatile unsigned long *)(RCC_BASE + 0x18))
#define GPIOC_CRH          (*(volatile unsigned long *)(GPIOC_BASE + 0x04))
#define GPIOC_ODR          (*(volatile unsigned long *)(GPIOC_BASE + 0x0C))

// Simple delay loop
void delay(volatile unsigned int count) {
    while(count--) __asm__("nop");
}

int main(void) {
    // 1. Enable GPIOC clock (bit 4 in RCC_APB2ENR)
    RCC_APB2ENR |= (1 << 4);

    // 2. Configure PC13 as General purpose output push-pull
    //    CRH controls pins 8–15, PC13 is in bits 20–23
    GPIOC_CRH &= ~(0xF << 20);   // clear config
    GPIOC_CRH |=  (0x1 << 20);   // MODE13 = 01 (output, max 10 MHz)
    // CNF13 = 00 (general purpose push-pull)

    while(1) {
        // 3. Toggle LED
        GPIOC_ODR &= ~(1 << 13);   // LED ON (active low)
        delay(500000);

        GPIOC_ODR |= (1 << 13);    // LED OFF
        delay(500000);
    }
}
