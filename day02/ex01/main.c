#include <avr/interrupt.h>
#include <util/delay.h>

#define TRUE 0
#define FALSE 1
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

void init_timer1()
{
    // Configuration du Timer1 en mode PWM, Phase Correct, 8-bit
    TCCR1A |= (1 << WGM10); // Mode PWM, Phase Correct, 8-bit
    TCCR1B |= (1 << CS11);  // Prescaler de 8, démarre le timer

    // Configuration de la broche de sortie OC1A (PB1) pour le mode de comparaison PWM
    // DDRB |= (1 << PB1);    // Définit PB1 comme sortie
    TCCR1A |= (1 << COM1A1); // Mettre OC1A (PB1) à 1 lorsqu'il y a une correspondance entre TCNT1 et OCR1A
}

// p132

void init_timer0(){

    TCCR0A |= (1 << WGM01);
    TIMSK0 |= (1 << OCIE0A);
    // TCCR0B |= (1 << CS00);

    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 128;

}

ISR(TIMER0_COMPA_vect)
{
    static uint8_t duty_cycle = 0; // Initialisation du rapport cyclique à 0
    static int increasing = TRUE; // Flag pour indiquer si le rapport cyclique est en train d'augmenter

    // Incrémente ou décrémente le rapport cyclique
    if (increasing)
    {
        duty_cycle++;
        if (duty_cycle == 100)
            increasing = FALSE; // Inverser la direction lorsque le rapport cyclique atteint 100%
    }
    else
    {
        duty_cycle--;
        if (duty_cycle == 0)
            increasing = TRUE; // Inverser la direction lorsque le rapport cyclique atteint 0%
    }

    // Mettre à jour la valeur de comparaison OCR1A
    OCR1A = (duty_cycle * 255) / 100; // Convertir le rapport cyclique en valeur pour OCR1A (0 à 255)

}

// ISR(TIMER0_COMPA_vect)
// {
//     //fonctionne

// }
// cli();

//     PORTB ^= (1 << LED_D2);
//     _delay_ms(2000);

//     sei();

void main()
{
    init_LED();
    init_timer0();
    init_timer1();
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