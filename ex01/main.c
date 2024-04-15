#include <avr/io.h>

#define LED_ON PORTB |= (1 << PB0)
#define LED_OFF PORTB &= ~(1 << PB0)
//LED is on port PB0


int main()
{
    //DDRB because we use brooch 0 on B port (PBO)
    DDRB |= (1 << PB0);
    //PB0 is set as output
    while (1)
    {
        //Setting the bit 0 on port B to 1 to turn on LED_D1
        LED_ON;
    }

   return 0; 
}