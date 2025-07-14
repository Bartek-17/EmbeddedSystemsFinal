#include "led.h"
#include "display.h"

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)
#define GPIOC_BASE  0x48000800
#define GPIOD_BASE  0x48000C00
#define GPIOE_BASE  0x48001000
#define GPIOB_BASE  0x48000400

#define GPIO_MODER(port) ((volatile uint32_t *)((port) + 0x00))
#define GPIO_ODR(port)   ((volatile uint32_t *)((port) + 0x14))

#define GPIOCEN  (1 << 2)
#define GPIODEN  (1 << 3)
#define GPIOEEN  (1 << 4)
#define GPIOBEN  (1 << 1)

static struct {
    uint32_t port;
    uint8_t pin;
    char id;
} leds[] = {
    { GPIOC_BASE, 6, '0' }, { GPIOC_BASE, 7, '1' }, { GPIOC_BASE, 8, '2' }, { GPIOC_BASE, 9, '3' },
    { GPIOE_BASE, 4, '4' }, { GPIOD_BASE, 3, '5' }, { GPIOE_BASE, 5, '6' }, { GPIOE_BASE, 6, '7' },
    { GPIOD_BASE, 13, 'R' }, { GPIOB_BASE, 8, 'G' }, { GPIOD_BASE, 12, 'B' }
};

void LED_Init(void)
{
    RCC_AHB2ENR |= GPIOCEN | GPIODEN | GPIOEEN | GPIOBEN;
    for (int i = 0; i < sizeof(leds) / sizeof(leds[0]); i++) {
        volatile uint32_t *moder = GPIO_MODER(leds[i].port);
        *moder &= ~(3 << (leds[i].pin * 2));
        *moder |=  (1 << (leds[i].pin * 2)); // Set as output
        *GPIO_ODR(leds[i].port) &= ~(1 << leds[i].pin); // Turn off
    }
}

int LED_Set(char id, int state)
{
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        if (leds[i].id == id || leds[i].id == (id ^ 0x20)) { // Case-insensitive match
            if (state == ON)
                *GPIO_ODR(leds[i].port) |= (1 << leds[i].pin);
            else
                *GPIO_ODR(leds[i].port) &= ~(1 << leds[i].pin);
            return 0;
        }
    }
    return -1;
}

int LED_Get(char id)
{
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        if (leds[i].id == id || leds[i].id == (id ^ 0x20)) {
            return ((*GPIO_ODR(leds[i].port)) & (1 << leds[i].pin)) ? ON : OFF;
        }
    }
    return -1;
}

void LED_Toggle(char id)
{
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        if (leds[i].id == id || leds[i].id == (id ^ 0x20)) {
            *GPIO_ODR(leds[i].port) ^= (1 << leds[i].pin);
            return;
        }
    }
}

static void Delay_ms(volatile uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 200; i++) {}
}

static void Delay_ms_non_blocking(int total_ms)
{
	if(Display_GetValue() == 10000){
		Delay_ms(500);
	}
	else{
	    for (int ms = 0; ms < total_ms; ms++) {
	    	RefreshDisplay(Display_GetValue());
	    }
	}
}


int LED_Blink(char id, int times)
{
    for (int i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        if (leds[i].id == id || leds[i].id == (id ^ 0x20)) {
            for (int j = 0; j < times; j++) {
                *GPIO_ODR(leds[i].port) ^= (1 << leds[i].pin);
                Delay_ms_non_blocking(100);
                *GPIO_ODR(leds[i].port) ^= (1 << leds[i].pin);
                Delay_ms_non_blocking(100);
            }
            return 0;
        }
    }
    return -1;
}

