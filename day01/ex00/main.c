#include <avr/io.h>
#include <util/delay.h>

#define LED_D2 PB1

void led_on(char x)
{
    PORTB |= (1 << x);
}

void led_off(char x)
{
    PORTB &= ~(1 << x);
}

void init_led(char x)
{
    DDRB |= (1 << x);
}

void main()
{
    init_led(LED_D2);
    while (1)
    {
        _delay_ms(5000);
        led_on(LED_D2);
        _delay_ms(5000);
        led_off(LED_D2);
    }
}