#include "uart.h"

#define UBRR 8

void uart_init() {

    UBRR0L = (uint8_t)(UBRR & 0xFF);
    UBRR0H = (uint8_t)(UBRR >> 8);
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx( char data)
{
    while (!(UCSR0A & (1 << UDRE0))); //si on utilise A register
    UDR0 = data;
}

void uart_printstr(const char *str)
{
    int i = 0;
    while (str[i])
    {
        uart_tx(str[i]);
        if (str[i++] == '\n')
            uart_tx('\r');
    }
}