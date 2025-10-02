#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ===== Peripheral base addresses ===== */
#define PERIPH_BASE       0x40000000UL
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x20000UL)

/* ===== RCC ===== */
#define RCC_BASE          (AHBPERIPH_BASE + 0x1000UL)
#define RCC_APB2ENR       (*(volatile uint32_t *)(RCC_BASE + 0x18))
#define RCC_APB1ENR       (*(volatile uint32_t *)(RCC_BASE + 0x1C))

#define RCC_APB2ENR_IOPAEN   (1U<<2)
#define RCC_APB2ENR_IOPBEN   (1U<<3)
#define RCC_APB2ENR_IOPCEN   (1U<<4)
#define RCC_APB2ENR_AFIOEN   (1U<<0)
#define RCC_APB2ENR_SPI1EN   (1U<<12)
#define RCC_APB2ENR_USART1EN (1U<<14)

/* ===== GPIO ===== */
#define GPIOA_BASE        (APB2PERIPH_BASE + 0x0800UL)
#define GPIOB_BASE        (APB2PERIPH_BASE + 0x0C00UL)
#define GPIOC_BASE        (APB2PERIPH_BASE + 0x1000UL)

#define GPIOA_CRL         (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_CRH         (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
#define GPIOA_IDR         (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
#define GPIOA_ODR         (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))
#define GPIOA_BSRR        (*(volatile uint32_t *)(GPIOA_BASE + 0x10))
#define GPIOA_BRR         (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

#define GPIOB_CRL         (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
#define GPIOB_CRH         (*(volatile uint32_t *)(GPIOB_BASE + 0x04))
#define GPIOB_ODR         (*(volatile uint32_t *)(GPIOB_BASE + 0x0C))
#define GPIOB_BSRR        (*(volatile uint32_t *)(GPIOB_BASE + 0x10))
#define GPIOB_BRR         (*(volatile uint32_t *)(GPIOB_BASE + 0x14))

#define GPIOC_CRH         (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
#define GPIOC_BSRR        (*(volatile uint32_t *)(GPIOC_BASE + 0x10))
#define GPIOC_BRR         (*(volatile uint32_t *)(GPIOC_BASE + 0x14))

/* ===== SPI1 ===== */
#define SPI1_BASE         (APB2PERIPH_BASE + 0x3000UL)
#define SPI1_CR1          (*(volatile uint32_t *)(SPI1_BASE + 0x00))
#define SPI1_SR           (*(volatile uint32_t *)(SPI1_BASE + 0x08))
#define SPI1_DR           (*(volatile uint32_t *)(SPI1_BASE + 0x0C))

#define SPI_CR1_SPE       (1U<<6)
#define SPI_CR1_MSTR      (1U<<2)
#define SPI_CR1_BR_DIV8   (0x2U<<3)
#define SPI_SR_TXE        (1U<<1)
#define SPI_SR_RXNE       (1U<<0)
#define SPI_SR_BSY        (1U<<7)

/* ===== USART1 (GPS) ===== */
#define USART1_BASE       (APB2PERIPH_BASE + 0x3800UL)
#define USART1_SR         (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR         (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR        (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1        (*(volatile uint32_t *)(USART1_BASE + 0x0C))

#define USART_SR_TXE      (1U<<7)
#define USART_SR_RXNE     (1U<<5)
#define USART_CR1_UE      (1U<<13)
#define USART_CR1_TE      (1U<<3)
#define USART_CR1_RE      (1U<<2)

/* ===== SX1278 registers ===== */
#define REG_FIFO                0x00
#define REG_OP_MODE             0x01
#define REG_FRF_MSB             0x06
#define REG_FRF_MID             0x07
#define REG_FRF_LSB             0x08
#define REG_PA_CONFIG           0x09
#define REG_LNA                 0x0C
#define REG_FIFO_ADDR_PTR       0x0D
#define REG_FIFO_TX_BASE_ADDR   0x0E
#define REG_FIFO_RX_BASE_ADDR   0x0F
#define REG_PAYLOAD_LENGTH      0x22
#define REG_MODEM_CONFIG_1      0x1D
#define REG_MODEM_CONFIG_2      0x1E
#define REG_IRQ_FLAGS           0x12
#define REG_DIO_MAPPING_1       0x40

/* ===== Pins ===== */
#define PIN_NSS   (1U<<4)   // PA4
#define PIN_SCK   (1U<<5)   // PA5
#define PIN_MISO  (1U<<6)   // PA6
#define PIN_MOSI  (1U<<7)   // PA7
#define PIN_DIO0  (1U<<1)   // PA1
#define PIN_RST   (1U<<0)   // PB0
#define PIN_LED   (1U<<13)  // PC13

/* ===== helpers ===== */
static inline void nss_low(void)  { GPIOA_BRR = PIN_NSS; }
static inline void nss_high(void) { GPIOA_BSRR = PIN_NSS; }
static inline void sx_reset_low(void)  { GPIOB_BRR = PIN_RST; }
static inline void sx_reset_high(void) { GPIOB_BSRR = PIN_RST; }
static inline int dio0_read(void) { return (GPIOA_IDR & PIN_DIO0) != 0; }
static inline void led_on(void)  { GPIOC_BRR = PIN_LED; }
static inline void led_off(void) { GPIOC_BSRR = PIN_LED; }

static void delay(volatile uint32_t d) { while(d--) { __asm__("nop"); } }

/* ===== SPI1 ===== */
void spi1_init_master(void) {
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_SPI1EN;

    /* PA4 NSS */
    GPIOA_CRL &= ~(0xF << (4*4));
    GPIOA_CRL |=  (0x03 << (4*4));

    /* PA5 SCK */
    GPIOA_CRL &= ~(0xF << (5*4));
    GPIOA_CRL |=  (0x0B << (5*4));

    /* PA6 MISO */
    GPIOA_CRL &= ~(0xF << (6*4));
    GPIOA_CRL |=  (0x04 << (6*4));

    /* PA7 MOSI */
    GPIOA_CRL &= ~(0xF << (7*4));
    GPIOA_CRL |=  (0x0B << (7*4));

    /* PA1 DIO0 input floating */
    GPIOA_CRL &= ~(0xF << (1*4));
    GPIOA_CRL |=  (0x04 << (1*4));

    /* PB0 RESET output */
    GPIOB_CRL &= ~(0xF << (0*4));
    GPIOB_CRL |=  (0x03 << (0*4));

    /* PC13 LED output */
    GPIOC_CRH &= ~(0xF << ((13-8)*4));
    GPIOC_CRH |=  (0x02 << ((13-8)*4));

    nss_high();
    sx_reset_high();

    /* SPI1 Master, BR = PCLK/8 */
    SPI1_CR1 = SPI_CR1_MSTR | SPI_CR1_BR_DIV8;
    SPI1_CR1 |= SPI_CR1_SPE;
}

static uint8_t spi1_transfer(uint8_t tx) {
    while (!(SPI1_SR & SPI_SR_TXE));
    SPI1_DR = tx;
    while (!(SPI1_SR & SPI_SR_RXNE));
    return (uint8_t)SPI1_DR;
}

/* SX1278 R/W */
void sx_write_reg(uint8_t addr, uint8_t val) {
    nss_low();
    spi1_transfer(addr | 0x80);
    spi1_transfer(val);
    nss_high();
}

uint8_t sx_read_reg(uint8_t addr) {
    uint8_t v;
    nss_low();
    spi1_transfer(addr & 0x7F);
    v = spi1_transfer(0x00);
    nss_high();
    return v;
}

/* SX1278 init */
void sx1278_init_433mhz(void) {
    sx_reset_low(); delay(20000); sx_reset_high(); delay(20000);

    sx_write_reg(REG_OP_MODE, 0x80);  /* sleep, LoRa mode */
    delay(10000);
    sx_write_reg(REG_OP_MODE, 0x81);  /* standby */

    sx_write_reg(REG_FRF_MSB, 0x6C);
    sx_write_reg(REG_FRF_MID, 0x80);
    sx_write_reg(REG_FRF_LSB, 0x00);

    sx_write_reg(REG_PA_CONFIG, 0x8F);
    sx_write_reg(REG_LNA, 0x23);
    sx_write_reg(REG_FIFO_TX_BASE_ADDR, 0x80);
    sx_write_reg(REG_FIFO_RX_BASE_ADDR, 0x00);

    sx_write_reg(REG_MODEM_CONFIG_1, 0x72);
    sx_write_reg(REG_MODEM_CONFIG_2, 0x74);

    sx_write_reg(REG_DIO_MAPPING_1, 0x40);
    sx_write_reg(REG_IRQ_FLAGS, 0xFF);
}

void sx1278_send_packet(uint8_t *buf, uint8_t len) {
    if (len == 0 || len > 255) return;

    sx_write_reg(REG_FIFO_ADDR_PTR, sx_read_reg(REG_FIFO_TX_BASE_ADDR));

    nss_low();
    spi1_transfer(REG_FIFO | 0x80);
    for (uint8_t i=0;i<len;i++) spi1_transfer(buf[i]);
    nss_high();

    sx_write_reg(REG_PAYLOAD_LENGTH, len);
    sx_write_reg(REG_IRQ_FLAGS, 0xFF);
    sx_write_reg(REG_OP_MODE, 0x83);

    for (volatile uint32_t t=0; t<0x7FFFFFF; t++) {
        if (dio0_read()) break;
        uint8_t irq = sx_read_reg(REG_IRQ_FLAGS);
        if (irq & 0x08) break;
    }

    sx_write_reg(REG_IRQ_FLAGS, 0x08);
    sx_write_reg(REG_OP_MODE, 0x81);  /* standby */
}

/* ===== USART1 for GPS ===== */
void usart1_init_9600(void) {
    RCC_APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    /* PA9 TX */
    GPIOA_CRH &= ~(0xF << (1*4));
    GPIOA_CRH |=  (0x0B << (1*4));
    /* PA10 RX */
    GPIOA_CRH &= ~(0xF << (2*4));
    GPIOA_CRH |=  (0x04 << (2*4));

    USART1_BRR = 0x341; /* 8MHz PCLK2, 9600 baud */
    USART1_CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

int usart1_read_nonblock(void) {
    if (USART1_SR & USART_SR_RXNE) return (int)(USART1_DR & 0xFF);
    return -1;
}

/* ===== NMEA parser ===== */
#define NMEA_BUF_SZ  256
static char nmea_buf[NMEA_BUF_SZ];
static uint16_t nmea_idx = 0;

int32_t nmea_coord_to_int(const char *str, char dir) {
    if (strlen(str)<4) return 0;
    int deg=0, min=0;
    float frac=0.0f;

    if (dir=='N' || dir=='S') {
        char degs[3]={str[0],str[1],0};
        deg = atoi(degs);
        min = atoi(str+2);
        frac = atof(str+2) - min;
    } else {
        char degs[4]={str[0],str[1],str[2],0};
        deg = atoi(degs);
        min = atoi(str+3);
        frac = atof(str+3) - min;
    }
    float val = deg + (min+frac)/60.0f;
    if (dir=='S'||dir=='W') val=-val;
    return (int32_t)(val*1e7);
}

/* ===== Main ===== */
int main(void) {
    spi1_init_master();
    usart1_init_9600();
    sx_reset_high();
    sx1278_init_433mhz();

    led_on(); delay(300000); led_off();

    while (1) {
        int ch = usart1_read_nonblock();
        if (ch >= 0) {
            char c = (char)ch;
            if (nmea_idx < (NMEA_BUF_SZ-1)) nmea_buf[nmea_idx++] = c;

            if (c == '\n') {
                nmea_buf[nmea_idx] = 0; // terminate
                nmea_idx = 0;

                if (strncmp(nmea_buf,"$GPGGA,",7)==0 || strncmp(nmea_buf,"$GPRMC,",7)==0) {
                    char *tokens[20]={0}; int ti=0;
                    char *p=nmea_buf;
                    while(*p && ti<20) {
                        tokens[ti++] = p;
                        char *comma=strchr(p,',');
                        if(!comma) break;
                        *comma=0; p=comma+1;
                    }

                    char *lat_str=0; char lat_dir='N';
                    char *lon_str=0; char lon_dir='E';
                    if(strncmp(nmea_buf,"$GPGGA,",7)==0) { lat_str=tokens[2]; lat_dir=tokens[3][0]; lon_str=tokens[4]; lon_dir=tokens[5][0]; }
                    else if(strncmp(nmea_buf,"$GPRMC,",7)==0){ lat_str=tokens[3]; lat_dir=tokens[4][0]; lon_str=tokens[5]; lon_dir=tokens[6][0]; }

                    if(lat_str && lon_str && strlen(lat_str)>0 && strlen(lon_str)>0) {
                        int32_t lat=nmea_coord_to_int(lat_str,lat_dir);
                        int32_t lon=nmea_coord_to_int(lon_str,lon_dir);

                        uint8_t payload[8];
                        payload[0]=(lat>>24)&0xFF; payload[1]=(lat>>16)&0xFF;
                        payload[2]=(lat>>8)&0xFF; payload[3]=lat&0xFF;
                        payload[4]=(lon>>24)&0xFF; payload[5]=(lon>>16)&0xFF;
                        payload[6]=(lon>>8)&0xFF; payload[7]=lon&0xFF;

                        led_on();
                        sx1278_send_packet(payload,8);
                        led_off();
                    }
                }
            }
        }
        delay(1000);
    }
    return 0;
}
