#include "display.h"
#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)
#define RCC_IOPB_EN (1 << 1)
#define RCC_IOPE_EN (1 << 4)
#define RCC_IOPG_EN (1 << 6)

#define GPIOB_BASE 0x48000400
#define GPIOE_BASE 0x48001000
#define GPIOG_BASE 0x48001800
#define PWR_BASE 0x40007000
#define IOSV_BIT 9

#define MODER_OFFSET 0x00
#define ODR_OFFSET 0x14
#define IDR_OFFSET 0x10
#define PWR_OFFSET 0x04

#define GPIO_MODER(port) ((volatile uint32_t *)((port) + MODER_OFFSET))
#define GPIO_ODR(port) ((volatile uint32_t *)((port) + ODR_OFFSET))
#define GPIO_IDR(port) ((volatile uint32_t *)((port) + IDR_OFFSET))
#define PWR_CR2 ((volatile uint32_t *)(PWR_BASE + PWR_OFFSET))

#define SEG_A (1 << 0)
#define SEG_B (1 << 1)
#define SEG_C (1 << 2)
#define SEG_D (1 << 3)
#define SEG_E (1 << 4)
#define SEG_F (1 << 5)
#define SEG_G (1 << 6)
#define SEG_MASK (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G)

#define DIG1 (1 << 2)
#define DIG2 (1 << 3)
#define DIG3 (1 << 4)
#define DIG4 (1 << 5)
#define DIG_MASK (DIG1 | DIG2 | DIG3 | DIG4)

static const uint8_t SEGMENT_MAP[10] = {
    0b00111111, 0b00000110, 0b01011011, 0b01001111,
    0b01100110, 0b01101101, 0b01111101, 0b00000111,
    0b01111111, 0b01101111
};

uint16_t current_display_value = 10000;

void Display_SetValue(uint16_t val) {
    if (val <= 9999) {
        current_display_value = val;
    }
}

uint16_t Display_GetValue(void) {
	return current_display_value;
}

static void Delay_ms(volatile uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 200; i++) {}
}

static void Enable_Power(void) {
    *PWR_CR2 |= (1 << IOSV_BIT);
}

static void Enable_GPIO_Clocks(void) {
    RCC_AHB2ENR |= RCC_IOPB_EN | RCC_IOPE_EN | RCC_IOPG_EN;
}

static void Configure_Segment_Pins(void) {
    for (int i = 0; i <= 6; i++) {
        *GPIO_MODER(GPIOG_BASE) &= ~(3 << (i * 2));
        *GPIO_MODER(GPIOG_BASE) |= (1 << (i * 2));
    }
}

static void Configure_Digit_Pins(void) {
    for (int i = 2; i <= 5; i++) {
        *GPIO_MODER(GPIOB_BASE) &= ~(3 << (i * 2));
        *GPIO_MODER(GPIOB_BASE) |= (1 << (i * 2));
    }
}

void GPIO_Init(void) {
    Enable_GPIO_Clocks();
    Configure_Segment_Pins();
    Configure_Digit_Pins();
    Enable_Power();
}

static void SetSegments(uint8_t val) {
    uint8_t pattern = SEGMENT_MAP[val];
    uint32_t seg_output = 0;

    seg_output |= (pattern & (1 << 0)) ? SEG_A : 0;
    seg_output |= (pattern & (1 << 1)) ? SEG_B : 0;
    seg_output |= (pattern & (1 << 2)) ? SEG_C : 0;
    seg_output |= (pattern & (1 << 3)) ? SEG_D : 0;
    seg_output |= (pattern & (1 << 4)) ? SEG_E : 0;
    seg_output |= (pattern & (1 << 5)) ? SEG_F : 0;
    seg_output |= (pattern & (1 << 6)) ? SEG_G : 0;

    *GPIO_ODR(GPIOG_BASE) = (*GPIO_ODR(GPIOG_BASE) & ~SEG_MASK) | seg_output;
}

static void DisableAllDigits(void) {
    *GPIO_ODR(GPIOB_BASE) &= ~DIG_MASK;
}

static void EnableDigit(uint8_t digitIndex) {
    DisableAllDigits();
    if (digitIndex == 0) *GPIO_ODR(GPIOB_BASE) |= DIG1;
    if (digitIndex == 1) *GPIO_ODR(GPIOB_BASE) |= DIG2;
    if (digitIndex == 2) *GPIO_ODR(GPIOB_BASE) |= DIG3;
    if (digitIndex == 3) *GPIO_ODR(GPIOB_BASE) |= DIG4;
}

void RefreshDisplay(uint16_t number) {
	if (current_display_value != 10000){
		uint8_t digits[4] = {
			(number / 1000) % 10,
			(number / 100) % 10,
			(number / 10) % 10,
			number % 10
		};

		uint8_t leading_zero = 1;

		for (uint8_t i = 0; i < 4; i++) {
			DisableAllDigits();

			if (digits[i] != 0 || i == 3 || !leading_zero) {
				leading_zero = 0;
				SetSegments(digits[i]);
				EnableDigit(i);
			} else {
				DisableAllDigits();
			}

			Delay_ms(1);
		}
	}
}


