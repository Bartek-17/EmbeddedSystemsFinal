#include "joystick.h"
#include <stdint.h>

#define RCC_AHB2ENR (*(volatile uint32_t *)0x4002104C)
#define GPIOE_BASE 0x48001000
#define GPIO_MODER ((volatile uint32_t *)(GPIOE_BASE + 0x00))
#define GPIO_PUPDR ((volatile uint32_t *)(GPIOE_BASE + 0x0C))
#define GPIO_IDR ((volatile uint32_t *)(GPIOE_BASE + 0x10))

#define GPIOEEN (1 << 4)

#define JOY_UP 3
#define JOY_DOWN 2
#define JOY_RIGHT 0
#define JOY_LEFT 1
#define JOY_OK 15

void Joystick_Init(void)
{
    RCC_AHB2ENR |= GPIOEEN;

    *GPIO_MODER &= ~(3 << (JOY_UP * 2));
    *GPIO_MODER &= ~(3 << (JOY_DOWN * 2));
    *GPIO_MODER &= ~(3 << (JOY_RIGHT * 2));
    *GPIO_MODER &= ~(3 << (JOY_LEFT * 2));
    *GPIO_MODER &= ~(3 << (JOY_OK * 2));

    *GPIO_PUPDR &= ~(3 << (JOY_UP * 2));
    *GPIO_PUPDR |= (1 << (JOY_UP * 2));

    *GPIO_PUPDR &= ~(3 << (JOY_DOWN * 2));
    *GPIO_PUPDR |= (1 << (JOY_DOWN * 2));

    *GPIO_PUPDR &= ~(3 << (JOY_RIGHT * 2));
    *GPIO_PUPDR |= (1 << (JOY_RIGHT * 2));

    *GPIO_PUPDR &= ~(3 << (JOY_LEFT * 2));
    *GPIO_PUPDR |= (1 << (JOY_LEFT * 2));

    *GPIO_PUPDR &= ~(3 << (JOY_OK * 2));
    *GPIO_PUPDR |= (1 << (JOY_OK * 2));
}

int Joystick_Read(char dir)
{
    uint32_t state = *GPIO_IDR;
    switch (dir)
    {
    case 'U':
    case 'u': return !(state & (1 << JOY_UP));
    case 'D':
    case 'd': return !(state & (1 << JOY_DOWN));
    case 'L':
    case 'l': return !(state & (1 << JOY_LEFT));
    case 'R':
    case 'r': return !(state & (1 << JOY_RIGHT));
    case 'C':
    case 'c': return !(state & (1 << JOY_OK));
    default:
        return -1;
    }
}
