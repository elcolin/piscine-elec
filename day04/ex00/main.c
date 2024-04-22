#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "i2c.h"

// p223 schema explicatif


void ERROR(char *str)
{
    uart_printstr("ERROR: ");
    uart_printstr(str);
    uart_printstr("\n");
}

int main()
{
    uart_init();

    i2c_init();
    i2c_start();
    i2c_stop();
    while (1);
}