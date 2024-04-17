#include <avr/io.h>
#include <util/delay.h>

#define LED_D2 PB1
#define CLOCK_CYCLE 4
#define DELAY (F_CPU * 0.5) / CLOCK_CYCLE / 25

void led_toggle(char x)
{
    // PORTB = ~(PORTB << x);
    PORTB ^= (1 << x);
}

void init_led(char x)
{
    DDRB |= (1 << x);
}



void main()
{
    init_led(LED_D2);
    unsigned long int target = 0;
    while (1)
    {
        if (target == DELAY)
        {
            led_toggle(LED_D2);
            target = 0;
        }
        target++;
    }
}