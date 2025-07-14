#include "lpuart.h"
#include "display.h"
#include "interface.h"
#include "led.h"
#include "joystick.h"

int main(void)
{
    char input;

    LPUART_init();
    GPIO_Init();
    LED_Init();
    Joystick_Init();

    Interface_Init();


    while (1)
    {
        RefreshDisplay(Display_GetValue());

        if (LPUART_ReceiveChar(&input) == 0) {
            Interface_ProcessInput(input);
        }
    }
    return 0;
}
