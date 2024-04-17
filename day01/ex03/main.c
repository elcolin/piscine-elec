#include <avr/io.h>
#include <util/delay.h>

#define CLOCK_FREQUENCY 16000000


#define TRUE 0
#define FALSE 1

#define SW1 PD2
#define SW2 PD4

#define LED_D2 PB1

#define DUTY ICR1 * 0.1

int button_pressed(char x)
{
    if (!(PIND & (1 << x)))
        return TRUE;
    return FALSE;
}




void init_led(char x)
{
    DDRB |= (1 << x);
}

void timer1_init() {
    // Set CTC mode with prescaler 1024
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << CS10) | (1 << CS12) | (1 << WGM12) | (1 << WGM13);

    
    ICR1 = (CLOCK_FREQUENCY / 1024 - 1);
    OCR1A = ICR1 * 0.1;
}

void increment_duty()
{
    if (OCR1A + DUTY < ICR1)
    OCR1A += DUTY;

}

void decrement_duty()
{

    if (OCR1A - DUTY > 0)
    OCR1A -= DUTY;
}



void main()
{
    init_led(LED_D2);
    DDRB &= ~(1 << PD2);
    DDRB &= ~(1 << PD4);
    timer1_init();
    while (1)
    {
        if (button_pressed(SW1) == TRUE)
        {
            _delay_ms(100);
            if (button_pressed(SW1) == FALSE)
                increment_duty();
        }
        if (button_pressed(SW2) == TRUE)
        {
            _delay_ms(100);
            if (button_pressed(SW2) == FALSE)
                decrement_duty();
        }
    }
}
