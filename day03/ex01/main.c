#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//Calculer Baud rate register setting: p182

#define CLOCK_SPEED 16000000UL
#define BAUD 115200

//p185
void uart_transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0))); //si on utilise A register
    UDR0 = data;
}

void uart_printstr(const char *str)
{
    int i = 0;
    while (str[i])
    {
        uart_transmit(str[i]);
        if (str[i++] == '\n')
            uart_transmit('\r');
    }
}

void init_timer1()
{
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = (CLOCK_SPEED / (1024)) * 2;

}

ISR(TIMER1_COMPA_vect)
{
    uart_printstr("Hello World!\n");

}

void uart_init(unsigned int ubbr)
{
    UBRR0H = (unsigned char) ubbr >> 8;
    UBRR0L = (unsigned char) ubbr;

    //enable receiver and transmetter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // set frame format: 8data, 2 stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}



void main()
{
    uart_init((CLOCK_SPEED/ (16 * BAUD)));//Asynchrone normal mode -1 retirer a cause des arrondis
    init_timer1();
    sei();
    // uart_transmit('Z');
    while(1);
}