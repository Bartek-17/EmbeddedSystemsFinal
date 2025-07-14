#include "lpuart.h"
#include <stdint.h>

// Peripheral base addresses
#define RCC_BASE        0x40021000
#define GPIOC_BASE      0x48000800
#define LPUART1_BASE    0x40008000

// RCC registers
#define RCC_AHB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x4C))
#define RCC_APB1ENR2    (*(volatile uint32_t *)(RCC_BASE + 0x5C))

// GPIOC registers
#define GPIOC_MODER     (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
#define GPIOC_AFRL      (*(volatile uint32_t *)(GPIOC_BASE + 0x20))

// LPUART1 register structure
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    uint32_t RESERVED1;
    uint32_t RESERVED2;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint16_t RDR;
    uint16_t RESERVED3;
    volatile uint16_t TDR;
    uint16_t RESERVED4;
} LPUART_TypeDef;

#define LPUART1 ((volatile LPUART_TypeDef *)LPUART1_BASE)

// Bit masks
#define CR1_UE   (1 << 0)
#define CR1_RE   (1 << 2)
#define CR1_TE   (1 << 3)
#define CR1_M0   (1 << 12)
#define CR1_M1   (1 << 28)
#define CR2_STOP (0x3 << 12)

#define ISR_TXE  (1 << 7)
#define ISR_TC   (1 << 6)
#define ISR_RXNE (1 << 5)
#define ISR_ORE  (1 << 3)
#define ISR_PE   (1 << 0)
#define ISR_FE   (1 << 1)
#define ISR_NE   (1 << 2)

#define ICR_ORECF (1 << 3)
#define ICR_PECF  (1 << 0)
#define ICR_FECF  (1 << 1)
#define ICR_NCF   (1 << 2)

#define GPIOC_CLOCK_ENABLE (1 << 2)
#define LPUART1_CLOCK_ENABLE (1 << 0)

#define SYSTEM_CLOCK_HZ    4000000
#define BAUD_RATE          115200
#define BRR_VALUE          ((256 * SYSTEM_CLOCK_HZ) / BAUD_RATE)

int LPUART_init(void)
{
    RCC_AHB2ENR |= GPIOC_CLOCK_ENABLE;
    RCC_APB1ENR2 |= LPUART1_CLOCK_ENABLE;

    GPIOC_AFRL &= ~(0xF << (4 * 0));
    GPIOC_AFRL |=  (0x8 << (4 * 0));
    GPIOC_AFRL &= ~(0xF << (4 * 1));
    GPIOC_AFRL |=  (0x8 << (4 * 1));

    GPIOC_MODER &= ~(0x3 << (2 * 0));
    GPIOC_MODER |=  (0x2 << (2 * 0));
    GPIOC_MODER &= ~(0x3 << (2 * 1));
    GPIOC_MODER |=  (0x2 << (2 * 1));

    LPUART1->CR1 &= ~CR1_M0;
    LPUART1->CR1 &= ~CR1_M1;
    LPUART1->CR2 &= ~CR2_STOP;
    LPUART1->BRR = BRR_VALUE;
    LPUART1->CR3 &= ~(1 << 7);
    LPUART1->CR3 &= ~(1 << 6);
    LPUART1->CR1 |= CR1_UE | CR1_TE | CR1_RE;

    return 0;
}

int LPUART_SendChar(char data)
{
    while (!(LPUART1->ISR & ISR_TXE));
    LPUART1->TDR = data;
    return 0;
}

int LPUART_ReceiveChar(char* data)
{
    if (!(LPUART1->ISR & ISR_RXNE)){
    	return -1;
    }
    *data = (unsigned char)(LPUART1->RDR & 0xFF);

    if (LPUART1->ISR & ISR_ORE) { LPUART1->ICR |= ICR_ORECF; return 1; }
    if (LPUART1->ISR & ISR_PE)  { LPUART1->ICR |= ICR_PECF; return 2; }
    if (LPUART1->ISR & ISR_FE)  { LPUART1->ICR |= ICR_FECF; return 3; }
    if (LPUART1->ISR & ISR_NE)  { LPUART1->ICR |= ICR_NCF;  return 4; }

    return 0;
}

int LPUART_SendString(char* str)
{
    while (*str != '\0') {
        LPUART_SendChar(*str++);
    }
    return 0;
}

int get_baud_rate(void) {
    return (256 * SYSTEM_CLOCK_HZ) / LPUART1->BRR;
}

char* get_stop_bits(void) {
    uint32_t val = (LPUART1->CR2 >> 12) & 0x3;
    if (val == 0) return "1 stop bit";
    if (val == 2) return "2 stop bits";
    return "Reserved";
}

char* get_m_bits(void) {
    uint32_t m1 = (LPUART1->CR1 >> 28) & 1;
    uint32_t m0 = (LPUART1->CR1 >> 12) & 1;
    if (m1 == 0 && m0 == 0) return "8 bits";
    if (m1 == 0 && m0 == 1) return "9 bits";
    if (m1 == 1 && m0 == 0) return "7 bits";
    return "Reserved";
}

char* get_parity(void) {
    return (LPUART1->CR1 & (1 << 10)) ? "Enabled" : "Disabled";
}
