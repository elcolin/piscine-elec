#include <avr/io.h>
#include <util/delay.h>

#define LED_ON PORTB |= (1 << PB0)
#define LED_OFF PORTB &= ~(1 << PB0)
//LED is on port PB0


int main()
{
    //DDRB because we use brooch 0 on B port (PBO)
    DDRB |= (1<< PB0);
    //PB0 is set as output
    DDRB &= ~(1 << PD2);
    //PD2 is set as input
    while (1)
    {
        // if ()
        if (!(PIND & (1 << PD2)))
        //while PD2 is low than turn the LED on
            LED_ON;
        else
            LED_OFF;
    }

   return 0; 
}