#ifndef LED_H
#define LED_H

#include <stdint.h>

#define ON 1
#define OFF 0

void LED_Init(void);
int LED_Set(char id, int state);
int LED_Get(char id);
int LED_Blink(char id, int times);
void LED_Toggle(char id);

#endif
