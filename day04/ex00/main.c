#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>
#include "uart.h"
#include "i2c.h"

#define SCL_FREQUENCY 100000
// #define F_CPU 16000000

#define CLOCK_SPEED 16000000UL
#define BAUD 115200
#define SLA 0x38

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