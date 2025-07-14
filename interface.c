#include "interface.h"
#include "lpuart.h"
#include "display.h"
#include "led.h"
#include "joystick.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_CMD_LEN 64

static char cmd_buffer[MAX_CMD_LEN];
static uint8_t cmd_index = 0;

static enum {
    MENU_TOP,
    MENU_LED,
    MENU_7LED,
    MENU_JOY,
    MENU_LPUART
} current_menu = MENU_TOP;

static void to_lowercase(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

static void print_prompt(void) {
    switch (current_menu) {
        case MENU_TOP: LPUART_SendString("> "); break;
        case MENU_LED: LPUART_SendString("LED> "); break;
        case MENU_7LED: LPUART_SendString("7LED> "); break;
        case MENU_JOY: LPUART_SendString("JOY> "); break;
        case MENU_LPUART: LPUART_SendString("LPUART> "); break;
    }
}

static void show_help(void) {
    switch (current_menu) {
        case MENU_TOP:
            LPUART_SendString("Commands: LED, 7LED, Joy, LPUART, Help\r\n");
            break;
        case MENU_LED:
            LPUART_SendString("Set <id>, Clear <id>, Blink <id>, Toggle <id>, Status <id>\r\n");
            break;
        case MENU_7LED:
            LPUART_SendString("Display <val>, Read\r\n");
            break;
        case MENU_JOY:
            LPUART_SendString("Read <L/R/U/D/C>\r\n");
            break;
        case MENU_LPUART:
            LPUART_SendString("Status\r\n");
            break;
    }
}

void Interface_Init(void) {
    cmd_index = 0;
    memset(cmd_buffer, 0, sizeof(cmd_buffer));
    current_menu = MENU_TOP;
    LPUART_SendString("Simple menu by Bartek\r\nWrite 'help' to obtain more information\r\n");
    print_prompt();
}

void Interface_ProcessInput(char input) {
    if ((input == '\b' || input == 127) && cmd_index > 0) {
        cmd_index--;
        LPUART_SendString("\b \b");
        return;
    }

    if (input == '\r' || input == '\n') {
        cmd_buffer[cmd_index] = '\0';
        LPUART_SendString("\r\n");

        char *cmd = strtok(cmd_buffer, " \r\n");
        char *arg = strtok(NULL, " \r\n");

        if (!cmd) {
            print_prompt();
            return;
        }

        to_lowercase(cmd);
        if (arg) to_lowercase(arg);

        if (strcmp(cmd, "help") == 0) {
            show_help();
        } else if (strcmp(cmd, "up") == 0) {
            current_menu = MENU_TOP;
        } else if (current_menu == MENU_TOP) {
            if (strcmp(cmd, "led") == 0) current_menu = MENU_LED;
            else if (strcmp(cmd, "7led") == 0) current_menu = MENU_7LED;
            else if (strcmp(cmd, "joy") == 0) current_menu = MENU_JOY;
            else if (strcmp(cmd, "lpuart") == 0) current_menu = MENU_LPUART;
            else LPUART_SendString("Unknown command.\r\n");
        } else if (current_menu == MENU_7LED) {
            if (strcmp(cmd, "display") == 0 && arg) {
                int val = atoi(arg);
                if (val >= 0 && val <= 9999) Display_SetValue(val);
                else LPUART_SendString("Invalid value\r\n");
            } else if (strcmp(cmd, "read") == 0) {
                char msg[16];
                if (Display_GetValue() == 10000) {
                    LPUART_SendString("7LED not set yet.\r\n");
                } else {
                    sprintf(msg, "%d\r\n", Display_GetValue());
                    LPUART_SendString(msg);
                }
            }
        } else if (current_menu == MENU_LED) {
            if (!arg) LPUART_SendString("Missing LED ID\r\n");
            else if (strcmp(cmd, "set") == 0) LED_Set(arg[0], ON);
            else if (strcmp(cmd, "clear") == 0) LED_Set(arg[0], OFF);
            else if (strcmp(cmd, "toggle") == 0) LED_Toggle(arg[0]);
            else if (strcmp(cmd, "blink") == 0) LED_Blink(arg[0], 5);
            else if (strcmp(cmd, "status") == 0) {
                int s = LED_Get(arg[0]);
                LPUART_SendString(s ? "ON\r\n" : "OFF\r\n");
            }
        } else if (current_menu == MENU_JOY) {
            if (strcmp(cmd, "read") == 0 && arg) {
                int state = Joystick_Read(arg[0]);
                if (state >= 0) {
                    LPUART_SendString(state ? "Pressed\r\n" : "Released\r\n");
                } else {
                    LPUART_SendString("Invalid direction\r\n");
                }
            }
        } else if (current_menu == MENU_LPUART) {
            if (strcmp(cmd, "status") == 0) {
                char msg[128];
                sprintf(msg, "Baud: %d\r\nBits: %s\r\nStop: %s\r\nParity: %s\r\n",
                    get_baud_rate(), get_m_bits(), get_stop_bits(), get_parity());
                LPUART_SendString(msg);
            }
        }

        cmd_index = 0;
        memset(cmd_buffer, 0, sizeof(cmd_buffer));
        print_prompt();
        return;
    }

    if (cmd_index < MAX_CMD_LEN - 1) {
        cmd_buffer[cmd_index++] = input;
        LPUART_SendChar(input);
    }
}
