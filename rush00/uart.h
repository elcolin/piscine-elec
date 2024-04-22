#ifndef UART_H
#define UART_H

#include <avr/io.h>

void uart_init();
void uart_tx(char c);
void uart_printstr(const char *str);

#endif
