# STM32 UART Hierarchical Menu Application

This project is a UART-based application designed for STM32 microcontrollers, implementing a hierarchical menu system accessible via a serial terminal (e.g., PuTTY). The communication is handled using the LPUART peripheral with direct register access based on STM documentation.

## Features

- UART communication over RS-232 using LPUART.
- Baud rate: 115200, 8 data bits, no parity, 1 stop bit (8N1 configuration).
- Hierarchical menu system navigable through typed commands.
- Modular code with separate drivers for:
  - LEDs (including RGB LED)
  - 7-segment display
  - Joystick
  - LPUART serial interface
- Case-insensitive command parsing.
- Parameterized UART configuration.

## How It Works

Upon reset, the application initializes the UART and displays the welcome message:


From this top-level menu, users can navigate to sub-menus using the following commands:

- `LED` — Control onboard LEDs.
- `7LED` — Display or read 4-digit numbers on the 7-segment display.
- `JOY` — Read joystick input state.
- `LPUART` — Check the status of the UART interface.
- `UP` — Return to the previous (higher) menu level.

## Command Reference

### Top-Level

- `HELP` — Display top-level help.
- `LED`, `7LED`, `JOY`, `LPUART` — Enter respective submenu.
- `UP` — (Unavailable here, as this is the top-level menu.)

### LED Level

- `SET <0-7>` — Turn on LED D1–D8.
- `CLEAR <0-7>` — Turn off LED D1–D8.
- `SET <R|G|B>` — Turn on RGB LED (D9).
- `CLEAR <R|G|B>` — Turn off RGB LED (D9).
- `HELP` — Display LED-related commands.

### 7LED Level

- `DISPLAY <0–9999>` — Show a number on the 7-segment display.
- `READ` — Read the current displayed value.
- `HELP` — Display 7-segment-related commands.

### JOY Level

- `READ <L|R|U|D|C>` — Read the state of joystick buttons (Left, Right, Up, Down, Center).
- `HELP` — Display joystick-related commands.

### LPUART Level

- `STATUS` — Show current UART configuration.
- `HELP` — Display LPUART-related commands.

## Notes

- All commands are case-insensitive: `display`, `Display`, and `DISPLAY` are all valid.
- The project uses direct register-level programming (no HAL/LL drivers).
- PuTTY or similar terminal emulator is required for interacting with the system.

## Dependencies

- STM32 microcontroller with LPUART support.
- Serial interface configured for RS-232 (115200 8N1).
- PC terminal software (e.g., PuTTY).

## License

This project is provided under an open-source license (add specific license if applicable).
