The main goal of the exercise is to write an application using a UART communication interface that implements
the hierarchical menu on the serial console. The program should use the LPUART peripheral to exchange
information with the user. The program must be written in C using STM documentation for the microcontroller
registers.

The application fulfulls such requierements:
• Messages are read and written on the PC terminal (e.g. PuTTY application)
• Program should run on ARM processor connected to PC using EIA RS 232 serial interface (LPUART
module) with the following parameters: baud rate - 115200 bauds, 8 data bits, no parity, single stop bit
(configuration should be parameterised)
• Program source code should be clearly structured and divided into drivers for individual components
(LEDs, 7-LED display, joystick and LPUART serial interface) with suitable functions required for each
device (notice that not all functions are required for drivers)
• After reset, the program should welcome the user with the following message and should start from the
top level of menu (‘>’ should be displayed on each level):
“ Simple menu by Name Surname
Write ‘help’ to obtain more information
>”
• User can change level after executing the suitable command (e.g. LED, 7LED, Joy or LPUART)
• User can return to the higher level using the command UP
• User can turn on/off LEDs using commands on the ‘LED‘ level: Set or Clear
o Set/Clear <0-7> - for LEDs D1-D8
o Set/Clear <R, G, B> - for RGB LED D9
• User can display numbers from 0 to 9999 on the ‘7LED’ level (e.g. Display 1234) or read the
number Read
• User can read the state of joystick buttons using commands on the ‘Joy’ level (e.g. Read <L, R, U,
D, C>)
• User can read the configuration of the LPUART serial port using commands on the ‘LPUART’ level
(e.g. Status),
• All commands should be case insensitive (all commands are valid: Display, display,DISPlay)
• Command HELP executed on some level displays help only for this level


Additional information:
• Communication should be configured to 8N1 mode (8 data bits, no parity, one stop bit).
• The baud rate should be configured to 115200
• PuTTY is a console application that can be used on the PC to communicate over the UART
interface
