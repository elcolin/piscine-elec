#include <avr/io.h>
#include <util/delay.h>

#define LED_D2 PB1
#define CLOCK_FREQUENCY 16000000


void init_led(char x)
{
    DDRB |= (1 << x);
}

void timer1_init() {
    TCCR1A |= (1 << COM1A0) ;// Toggle state of LED
    // CTC mode with prescaler 1024 -> 16000000 / 1024
    TCCR1B |= (1 << CS10) | (1 << CS12) | (1 << WGM12);


    // Nombre de cycle d'horloge = Frequence de l'horloge divise / Frequence du signal or Frequence du signal = 1
    OCR1A = (CLOCK_FREQUENCY / 1024 )/2; 

}

void main()
{
    init_led(LED_D2);
    timer1_init();
    while (1)
    {
        
    }
}


// #include <avr/interrupt.h>

// #define LED_PIN PB1

// void timer1_init() {
//     // Set CTC mode with prescaler 1024
//     TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

//     // Set compare value for 1 Hz frequency (depends on clock frequency)
//     OCR1A = 15624;  // Assuming F_CPU = 16 MHz and prescaler 1024

//     // Enable Timer1 compare match interrupt
    // TIMSK1 |= (1 << OCIE1A);
// }

// void led_init() {
//     // Set LED pin as output
//     DDRB |= (1 << LED_PIN);
// }

// ISR(TIMER1_COMPA_vect) {
//     // Toggle LED pin on compare match
//     PORTB ^= (1 << LED_PIN);
// }

// int main() {
//     // Initialize LED pin
//     led_init();
    
//     // Initialize Timer1
//     timer1_init();
    
//     // Enable global interrupts
//     sei();
    
//     while (1) {
//         // Empty main loop
//     }
    
//     return 0;
// }