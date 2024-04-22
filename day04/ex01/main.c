#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include "i2c.h"
#include "uart.h"

int main()
{
    uart_init();

    i2c_init();
    while (1)
    {
        i2c_start();
        i2c_write(SLA_W);
        _delay_ms(40);
        i2c_write(0xAC);
        i2c_write(0x33);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(80);
        i2c_start();
        i2c_write(SLA_R);
        for (int i = 0; i < 7; i++)
        {
            i2c_read();
            print_hex_value(TWDR);
            uart_tx(' ');
        }
        _delay_ms(2000);
        uart_printstr("\n");
        i2c_stop();
    }

}