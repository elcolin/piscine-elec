#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h> // Include stdint.h for data types

#define TRUE 0
#define FALSE 1
#define LED_D2 PB1
#define CLOCK_FREQUENCY 16000000UL // Use uint32_t for clock frequency
#define PREDIVIDER 64
#define PRESCALER0 1024
#define PRESCALER1 1024

// Timer1 modes: p 140
// Interrupt Vectors: page 66
// External Interrupts: p 79
// Timer0 modes: p115


void init_LED(int x)
{
    DDRB |= (1 << x); // Configure LED pin as output
}


void    init_timer0()
{

    TCCR0B |= (1 << CS02) | (1 << CS00);//1024
    TCCR0A |= (1 << WGM01);//Timer0 en mode CTC
    // Activation de l'interruption de comparaison pour OCR0A
    TIMSK0 |= (1 << OCIE0A);
    OCR0A = (CLOCK_FREQUENCY / (PRESCALER1 * 2)) / 200 - 1;
    // OCR0A = 255;
}
void init_timer1() {
    //Mode 14 WGM
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << CS12) | (1 << CS10) | (1 << WGM12) | (1 << WGM13); //1024

    
    ICR1 = (CLOCK_FREQUENCY / (PRESCALER1 * 2)) / 60 - 1;
    OCR1A = ICR1;
}

float duty_cycle = 0.1;
int increasing = TRUE;// Flag to indicate duty cycle increase

ISR(TIMER0_COMPA_vect)
{
    // PORTB ^= (1 << PB2);

    //attention au overflow
    

    // Increment or decrement duty cycle
    if (increasing == TRUE)
    {
        duty_cycle += 0.01;
        if (duty_cycle >= 1)
            increasing = FALSE; // when duty cycle reaches 100%
    }
    else
    {
        duty_cycle -= 0.01;
        if (duty_cycle <= 0.02)
            increasing = TRUE; //when duty cycle reaches 0%
    }
    OCR1A = ICR1 * duty_cycle;
}

void main()
{
    init_LED(LED_D2);
    init_LED(PB0);
    // init_LED(PB2);

    init_timer0();
    init_timer1();
    sei(); // Enable interrupts
    while (1)
    {
        // Main loop
    }
}
