#include <avr/io.h>
#include <util/delay.h>

// #define LED_ON PORTB |= (1 << PB0)
// #define LED_OFF PORTB &= ~(1 << PB0)
#define SW1_PRESSED !(PIND & (1 << PD2))
#define SW1_UNPRESSED (PIND & (1 << PD2))
#define SW2_PRESSED !(PIND & (1 << PD4))
#define SW2_UNPRESSED (PIND & (1 << PD4))
//LED is on port PB0

void binary_to_led(int value)
{
    char PBs[4] = {PB0, PB1, PB2, PB4};
    int i = 3;
    while (i >= 0)
    {
        if (1 & (value >> i))
        // shifts the bits on a 4 bit range: 1000 >> 3 gives 0001 and we do the & operation
        // if it gives 1, then we enter the condition to turn on the LED
            PORTB |= (1 << PBs[i]);
        else 
            PORTB &= ~(1 << PBs[i]);
        i--;
    }
}


int main()
{
    int value = 0;
    //DDRB because we use brooch 0 on B port (PBO)
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB4);
    //LED D1, D2, D3 and D4 set as output

    DDRB &= ~(1 << PD2);
    DDRB &= ~(1 << PD4);
    //PD2 and PD4 is set as input
    while (1)
    {
        if (SW1_PRESSED)
        {
            _delay_ms(100);
            //delay to avoid debouncing effect
            if (SW1_UNPRESSED && value < 15)
                value++;
            binary_to_led(value);
            //checks if button came back to original position
        }
        if (SW2_PRESSED)
        {
            _delay_ms(100);
            //delay to avoid debouncing effect
            if (SW2_UNPRESSED && value)
                value--;
            binary_to_led(value);
            //checks if button came back to original position
        }

        
    }
   return 0; 
}
