#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void GPIO_Init(void);
void RefreshDisplay(uint16_t number);
void Display_SetValue(uint16_t val);
uint16_t Display_GetValue(void);


#endif
