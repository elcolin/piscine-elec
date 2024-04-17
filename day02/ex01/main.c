#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_D2 PB1
#define CLOCK_FREQUENCY 16000000
// Timer1 modes: p 140
// Interrupt Vectors: page 66
// External Interrupts: p 79
// Timer0 modes: p115

void init_LED()
{
    DDRB |= (1 << LED_D2); // Configuration de la broche du LED comme sortie
}

void init_timer1() {
    // Set CTC mode with prescaler 1024
    // TCCR1A |= (1 << COM1A0);
    // TCCR1B |= (1 << CS10) | (1 << CS12) | (1 << WGM12);

    
    // OCR1A = (CLOCK_FREQUENCY / 1024);
    // OCR1A = ICR1 * 0.1;
}

void init_timer0(){

    TCCR0A |= (1 << WGM01);
    TIMSK0 |= (1 << OCIE0A);
    TCCR0B |= (1 << CS00);

    OCR0A = 254;

}



ISR(TIMER0_COMPA_vect)
{
    //fonctionne

}
// cli();

//     PORTB ^= (1 << LED_D2);
//     _delay_ms(2000);

//     sei();

void main()
{
    init_LED();
    init_timer0();
    sei(); //Set Enable Interrups
    while (1)
    {
        /* code */
    }
    
    return;
}

// void init_interrupt()
// {
//     // DDRB &= ~(1 << PD2);

//     // EICRA |= (1 << ISC00) | (1 << ISC01);
//     // EIMSK |= (1 << INT0);
// }

// ISR(INT0_vect)
// {
//     cli();
//     _delay_ms(10);
//     sei();
//     PORTB ^= (1 << LED_D2);

// }