#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdio.h>

#define SCL_FREQUENCY 100000
// #define F_CPU 16000000

#define CLOCK_SPEED 16000000UL
#define BAUD 115200
#define SLA 0x38

// p223 shcema explicatif

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

void print_status()
{
    switch (TWSR)
    {
        case 0x08:
            uart_printstr("A START condition has been transmitted\n");
            break;
        case 0x10:
            uart_printstr("A repeated START condition has been transmitted\n");
            break;
        case 0x18:
            uart_printstr("SLA+W has been transmitted and ACK has been received\n");
            break;
        case 0x20:
            uart_printstr("SLA+W has been transmitted and NOT ACK has been received\n");
            break;
        case 0x28:
            uart_printstr("Data byte has been transmitted and ACK has been received\n");
            break;
        case 0x30:
            uart_printstr("Data byte has been transmitted and NOT ACK has been received\n");
            break;
        case 0x38:
            uart_printstr("Arbitration lost in SLA or data bytes\n");
            break;
        case 0x40:
            uart_printstr("SLA+R has been transmitted and ACK has been received\n");
            break;
        case 0x48:
            uart_printstr("SLA+R has been transmitted and NOT ACK has been received\n");
            break;
        case 0x50:
            uart_printstr("Data byte has been received and ACK has been returned\n");
            break;
        case 0x58:
            uart_printstr("Data byte has been received and NOT ACK has been returned\n");
            break;
        default:
            uart_printstr("Unknown status\n");
    }
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

void print_hex_value(uint8_t data)
{
    char hex[5];
    hex[0] = "0123456789ABCDEF"[data >> 4];
    hex[1] = "0123456789ABCDEF"[data & 0x0F];
    // hex[2] = '\r';
    // hex[3] = '\n';
    hex[2] = ' ';
    hex[3] = ' ';
    hex[4] = '\0';
    uart_printstr(hex);
}

void i2c_init()
{
    TWBR = ((F_CPU / SCL_FREQUENCY) - 16) / 2; 
    // TWCR = (1 << TWINT) | (1 << TWEN);
}


void start_slave(char c)
{
    TWCR = 0;
    
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    // print_status();
    TWDR = c;
    TWCR = (1 << TWINT) | (1 << TWEN);
    // TWDR = c;

}


char i2c_read()
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA) ;
    while (!(TWCR & (1 << TWINT)));
    // print_hex_value(TWDR);
    return TWDR;
}

void receive()
{
    start_slave(0x71);
    _delay_ms(80);
    while (!(TWCR & (1 << TWINT)));
    int i = 0;
    while (i < 7)
    {
        print_hex_value(i2c_read());
        i++;
    }

}


void i2c_write(char c)
{
    TWDR = c;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1 << TWINT)));
    // print_status();
}

void i2c_start()//Master transmitter
{
    start_slave((0x38 << 1) & ~(1));
    _delay_ms(200);

    while (!(TWCR & (1 << TWINT)));
    _delay_ms(10);
    i2c_write(0xAC);
    _delay_ms(10);
    i2c_write(0x33);
    _delay_ms(10);
    i2c_write(0x00);
    _delay_ms(80);

}

void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
    // TWCR = 0;

}

int main()
{
    char hex_value[7] = {0};
    uart_init((CLOCK_SPEED/ (16 * BAUD)));

    i2c_init();
    while (1)
    {
        i2c_start();
        i2c_stop();
        _delay_ms(80);

        receive();
        uart_printstr("\n");
        i2c_stop();
    }
        // i2c_stop();

}