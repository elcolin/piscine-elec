#include <avr/io.h>
#include <util/delay.h>

#define LED_ON PORTB |= (1 << PB0)
#define LED_OFF PORTB &= ~(1 << PB0)
//LED is on port PB0


int main()
{
    //DDRB because we use brooch 0 on B port (PBO)
    DDRB |= (1<< PB0);
    DDRB &= ~(1 << PD2);
    while (1)
    {
        // if ()
        if (!(PIND & (1 << PD2)))
            LED_ON;
        else
            LED_OFF;
    }

   return 0; 
}