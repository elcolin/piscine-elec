#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//Calculer Baud rate register setting: p182

#define CLOCK_SPEED 16000000UL
#define BAUD 115200
#define LED_D2 PB1

//p185
void uart_transmit(unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0))); //si on utilise A register
    UDR0 = data;
}

char convert_case(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 32;
    else if (c >= 'A' && c <= 'Z')
        return c + 32;
    return c;
}

ISR(USART_RX_vect)
{
    uart_transmit(convert_case(UDR0));
}

void uart_init(unsigned int ubbr)
{
    UBRR0H = (unsigned char) ubbr >> 8;
    UBRR0L = (unsigned char) ubbr;

    //enable receiver and transmetter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // set frame format: 8data, 2 stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    UCSR0B |= (1 << RXCIE0);
}

void main()
{
    uart_init((CLOCK_SPEED/ (16 * BAUD)));//Asynchrone normal mode -1 retirer a cause des arrondis    
    sei();
    while(1);
}

