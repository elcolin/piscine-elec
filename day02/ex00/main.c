#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_D1 PB0
// Interrupt Vectors: page 66
// External Interrupts: p 79

void init_LED()
{
    DDRB |= (1 << LED_D1); // Configuration de la broche du LED comme sortie
}

void init_interrupt()
{
    DDRB &= ~(1 << PD2);

    EICRA |= (1 << ISC00) | (1 << ISC01);
    EIMSK |= (1 << INT0);
}

ISR(INT0_vect)
{
    cli();
    _delay_ms(10);
    sei();
    PORTB ^= (1 << LED_D1);

}

void main()
{
    init_LED();
    init_interrupt();
    sei(); //Set Enable Interrups
    while (1)
    {
        /* code */
    }
    
    return;
}