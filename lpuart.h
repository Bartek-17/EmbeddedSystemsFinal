#ifndef LPUART_H
#define LPUART_H


int LPUART_init(void);
int LPUART_SendChar(char data);
int LPUART_ReceiveChar(char* data);
int LPUART_SendString(char* str);

int get_baud_rate(void);
char* get_stop_bits(void);
char* get_m_bits(void);
char* get_parity(void);

#endif
