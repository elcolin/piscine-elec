#include <avr/io.h>

#define LED_ON PORTB |= (1 << PB0)
#define LED_OFF PORTB &= ~(1 << PB0)
//LED is on port PB0


int main()
{
    //DDRB because we use brooch 0 on B port (PBO)
    DDRB |= (1<< PB0);
    // DDRB &= ~(1 << PD2);
    while (1)
    {
        // if ()
        LED_ON;
    }

   return 0; 
}