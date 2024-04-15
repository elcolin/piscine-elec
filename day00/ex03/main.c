#include <avr/io.h>
#include <util/delay.h>

#define LED_ON PORTB |= (1 << PB0)
#define LED_OFF PORTB &= ~(1 << PB0)
#define BUTTON_PRESSED !(PIND & (1 << PD2))
//LED is on port PB0


int main()
{
    //DDRB because we use brooch 0 on B port (PBO)
    int power = 0;
     DDRB |= (1<< PB0);
    //PB0 is set as output
    DDRB &= ~(1 << PD2);
    //PD2 is set as input
    while (1)
    {
        if (BUTTON_PRESSED)
        {
            _delay_ms(100);
            //delay to avoid debouncing effect
            if ((PIND & (1 << PD2)))
            //checks if button came back to original position
                power = !power;
            if (power)
                LED_ON;
            else
                LED_OFF;
            
        }
    }

   return 0; 
}