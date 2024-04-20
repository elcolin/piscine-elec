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

void	ft_putnbr(long int n)
{
	long int	m;

	m = n;
	if (m < 0)
		m *= -1;
	if (n < 0)
		uart_transmit('-');
	if (m >= 10)
		ft_putnbr(m / 10);
	uart_transmit(m % 10 + '0');

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
    // TWCR = (1 << TWINT) | (1 << TWEN);
}


void i2c_start(char addr)
{
    TWCR = 0;
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    // print_status();
    TWDR = addr;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
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
    // print_status();
}

void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void print_binary(long int n) {
    // Determine the number of bits in an integer
    long int num_bits = sizeof(n) * 8;
    
    // Iterate through each bit starting from the most significant bit
    for (long int i = num_bits - 1; i >= 0; i--) {
        // Use bitwise AND to check the value of each bit
        long int bit = (n >> i) & 1;
        // Print the bit (either 0 or 1)
        ft_putnbr(bit);
    }
    uart_printstr("\n");
}
// void    print_data(char *hex_value)
// {
//     char poids_fort = (hex_value[3] & 0x0F);
//     char poids_moyen = hex_value[4];
//     int poids_faible = hex_value[5];
//     uart_printstr("poids fort: ");
//     print_binary(poids_fort);
//     long int ST = ((long int)poids_fort << 16);
//     print_binary(ST);
//     uart_printstr("poids moyen: ");
//     // print_hex_value(hex_value[4])
//     print_binary(poids_moyen);
//     ST |= (((long int)poids_moyen & 0xFF) << 8);
//     print_binary(ST);
//     uart_printstr("poids faible: ");
//     print_binary(poids_faible & 0xFF);
//     ST |= (poids_faible & 0xFF);
//     print_binary(ST);
//     ft_putnbr(ST);
//     uart_printstr("\n");
//     float temp = (float) ST / 1048576.0f * 200 - 50 ;
//     ft_putnbr((int) temp);
// }

void    print_data(char *hex_value)
{
    long int ST = ((long int)(hex_value[3] & 0x0F) << 16);
    ST |= (((long int)hex_value[4] & 0xFF) << 8);
    ST |= (hex_value[5] & 0xFF);
    float temp = (float) ST / 1048576.0f * 200 - 50 ;
    ft_putnbr((int) temp);
}


int main()
{
    char hex_value[7] = {0};
    uart_init((CLOCK_SPEED/ (16 * BAUD)));

    i2c_init();
    while(1)
    {
        i2c_start(0x70);
        _delay_ms(40);
        i2c_write(0xAC);
        i2c_write(0x33);
        i2c_write(0x00);
        i2c_stop();
        _delay_ms(80);
        i2c_start(0x71);
        for (int i = 0; i < 7; i++)
            hex_value[i] = i2c_read();
        print_data(hex_value);
        uart_printstr("\n");
        i2c_stop();
        _delay_ms(3000);
    }

}