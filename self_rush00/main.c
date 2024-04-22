#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define SCL_FREQUENCY 100000
#define SW1_PRESSED !(PIND & (1 << PD2))
#define SW1_UNPRESSED (PIND & (1 << PD2))
#define CLOCK_SPEED 16000000UL
#define BAUD 115200


void uart_init(unsigned int ubbr)
{
    UBRR0H = (unsigned char) ubbr >> 8;
    UBRR0L = (unsigned char) ubbr;

    //enable receiver and transmetter
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // set frame format: 8data, 2 stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void uart_tx( char data)
{
    while (!(UCSR0A & (1 << UDRE0))); //si on utilise A register
    UDR0 = data;
}

void uart_printstr(const char *str)
{
    int i = 0;
    while (str[i])
    {
        uart_tx(str[i]);
        if (str[i++] == '\n')
            uart_tx('\r');
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
        case 0xA8:
            uart_printstr("Own SLA+R has been received and ACK has been returned\n");
            break;
        case 0xB0:
            uart_printstr("Arbitration lost in SLA+R/W as Master, own SLA+R has been received and ACK has been returned\n");
            break;
        case 0xB8:
            uart_printstr("Data byte in TWDR has been transmitted and ACK has been received\n");
            break;
        case 0xC0:
            uart_printstr("Data byte in TWDR has been transmitted and NOT ACK has been received\n");
            break;
        case 0xC8:
            uart_printstr("Last data byte in TWDR has been transmitted (TWEA = “0”)and ACK has been received\n");
            break;
        case 0x70:
            uart_printstr("General call address has been received and ACK has been returned\n");
            break;
        default:
            uart_printstr("Unknown status\n");
            break;
    }
}

void print_hex_value(uint8_t c)
{
    char hex[5];
    hex[0] = "0123456789ABCDEF"[c >> 4];
    hex[1] = "0123456789ABCDEF"[c & 0x0F];
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
}

char i2c_read()
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA) ;
    while (!(TWCR & (1 << TWINT)));
    // print_hex_value(TWDR);
    return TWDR;
}

void i2c_write(char c)
{
    TWDR = c;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_start(char addr)
{
    TWCR = 0;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    // print_status();
    i2c_write(addr);
}

void i2c_start_slave()
{
    TWCR = 0;
    TWAR = (1 << TWGCE);
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    // print_hex_value(TWSR);
    print_status();
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    print_hex_value(TWSR);
    print_status();
    uart_printstr(TWDR);

}

void become_master()
{
    while(1)
    {
        
    }
}

void become_slave()
{
    print_status();
    while(1)
    {
        if (SW1_PRESSED)
        {
            _delay_ms(100);
            if (SW1_UNPRESSED)
            {
                TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
                while (!(TWCR & (1 << TWINT)));
                print_hex_value(TWSR);

            }
        }
    }
}


void main()
{
    // uart_init(CLOCK_SPEED/ (16 * BAUD));
    // i2c_init();
    // i2c_start_slave();
    // while (1)
    // {
    //     if (SW1_PRESSED)
    //     {
    //         _delay_ms(100);
    //         //delay to avoid debouncing effect
    //         if (SW1_UNPRESSED)
    //             become_master();
    //     }
    //     if (TWCR & (1 << TWINT))
    //         become_slave();
    // }
    
}

// char i2c_read()
// {
//     TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA) ;
//     while (!(TWCR & (1 << TWINT)));
//     // print_hex_value(TWDR);
//     return TWDR;
// }

// void i2c_write(char c)
// {
//     TWDR = c;
//     TWCR = (1<<TWINT) | (1<<TWEN);
//     while (!(TWCR & (1 << TWINT)));
//     print_status();
// }

// void i2c_stop()
// {
//     TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
// }
