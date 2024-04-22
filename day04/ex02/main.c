#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>
#include "i2c.h"
#include "uart.h"

// void    print_data(char *hex_value)
// {
//     long int ST = ((long int)(hex_value[3] & 0x0F) << 16);
//     ST |= (((long int)hex_value[4] & 0xFF) << 8);
//     ST |= (hex_value[5] & 0xFF);
//     float temp = (float) ST / 1048576.0f * 200 - 50 ;

//     long int RH = ((long int)(hex_value[1] & 0xFF) << 12) | ((long int)(hex_value[2] & 0xFF) << 8) | (hex_value[3] & 0x0F);
//     float hum = (float) RH / 1048576.0f * 100;
//     char buffer[7];
//     dtostrf(temp, 2, 0, buffer);
//     uart_printstr("Temperature: ");
//     uart_printstr(buffer);
//     uart_printstr("ºC, Humidity: ");
//     dtostrf(hum, 3, 1, buffer);
//     uart_printstr(buffer);
//     uart_printstr("%\n");
// }

float   get_temperature(char *hex_value)
{
    long int ST = ((long int)(hex_value[3] & 0x0F) << 16) | (((long int)hex_value[4] & 0xFF) << 8) | (hex_value[5] & 0xFF);
    return (float) ST / 1048576.0f * 200 - 50 ;
}

float   get_humidity(char *hex_value)
{
    long int RH = ((long int)(hex_value[1] & 0xFF) << 12) | ((long int)(hex_value[2] & 0xFF) << 8) | (hex_value[3] & 0x0F);
    return (float) RH / 1048576.0f * 100 ;
}

void    set_captor()
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
}

float get_average(float *data)
{
    return (data[0] + data[1] + data[2]) / 3.0f;
}

void print_data(float hum_avr, float temp_avr)
{
    char buffer[7];
    dtostrf(temp_avr, 2, 0, buffer);
    uart_printstr("Temperature: ");
    uart_printstr(buffer);
    uart_printstr("ºC, Humidity: ");
    dtostrf(hum_avr, 3, 1, buffer);
    uart_printstr(buffer);
    uart_printstr("%\n");
}

int main()
{
    char    hex_value[3][7] = {0};
    float   hum[3] = {0};
    float   temp[3] = {0};
    float   temp_avr = 0;
    float   hum_avr = 0;
    uart_init();

    i2c_init();
    while(1)
    {
        for (int j = 0; j < 3; j++)
        {
            set_captor();
            for (int i = 0; i < 7; i++)
            {
                i2c_read();
                hex_value[j][i] = TWDR;
            }
            i2c_stop();
            hum[j] = get_humidity(hex_value[j]);
            temp[j] = get_temperature(hex_value[j]);
            _delay_ms(1000);

        }
        print_data(get_average(hum), get_average(temp));
    }

}