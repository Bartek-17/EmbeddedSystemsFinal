The main goal of the exercise is to write an application using a UART communication interface that implements
the hierarchical menu on the serial console. The program should use the LPUART peripheral to exchange
information with the user. The program must be written in C using STM documentation for the microcontroller
registers.

The application should fulfil the following requirements:
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

Menu structure:
Menu level:
LED>
Set <id>
Clear <id>
Blink <id>
Status <id>
Toggle <id>
Command function
Top level
Turn on LED (id: 0-7 or R,G,B)
Turn off LED (id: 0-7 or R,G,B)
Blink LED five times (id: 0-7 or R,G,B)
Display on/off status of LED (id: 0-7 or R,G,B)
Invert LED state (id: 0-7 or R,G,B)
7LED>
Display <val>
Read
Joy>
Read <id>
LPUART>
Status
Display number from 0 to 9999 on the 7-LED display
Display on the terminal the number from 7-LED display
Display on the terminal the current state of joystick buttons
(id: L – left, R – right, U – up, D – down, C - center)
Display baudrate, number of databits and parity bits,
informations read from registers and then calculated baudrate


Additional information:
• Communication should be configured to 8N1 mode (8 data bits, no parity, one stop bit).
• The baud rate should be configured to 115200
• PuTTY is a console application that can be used on the PC to communicate over the UART
interface
