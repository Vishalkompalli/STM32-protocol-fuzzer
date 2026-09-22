#ifndef UART_H
#define UART_H
#include <stdint.h>

void uart_init();
void uart_send_char(char c);
void uart_send_string(const char* s);

#endif
