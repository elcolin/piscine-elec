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

void	ft_putnbr(int n)
{
	long int	m;

	m = n;
	if (m >= 10)
		// ft_putnbr_fd(m / 10);
        uart_transmit(m/10);

	// ft_putchar_fd(m % 10 + '0', fd);
    uart_transmit(m % 10 + '0');

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

void ERROR(char *str)
{
    uart_printstr("ERROR: ");
    uart_printstr(str);
    uart_printstr("\n");
}

void print_hex_value(uint8_t data)
{
    char hex[5];
    hex[0] = "0123456789ABCDEF"[data >> 4];
    hex[1] = "0123456789ABCDEF"[data & 0x0F];
    hex[2] = '\r';
    hex[3] = '\n';
    hex[4] = '\0';
    uart_printstr(hex);
}

void i2c_init()
{
    TWBR = ((F_CPU / SCL_FREQUENCY) - 16) / 2;   
}


void start_slave(char c, char code)
{
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    if (TWSR != code)
        return ERROR("Start error");
    TWDR = c;
    TWCR = (1 << TWINT) | (1 << TWEN);
}

char receive_slave()
{
    TWCR = (1<<TWINT) | (1<<TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    // print_hex_value(TWSR);
    if (TWSR != 0x50)
        ERROR("receive error");
    uart_printstr("Receive over\n");
    print_hex_value(TWDR);
    // TWCR = (1<<TWINT) | (1<<TWEN);
    return TWDR;
}

void receive()
{
    start_slave(0x71, 0x10);
    _delay_ms(80);

    while (!(TWCR & (1 << TWINT)));
    if (TWSR != 0x40)
        ERROR("Master receiver error");
    while ((receive_slave()))
        _delay_ms(80);
}



void send_slave(char c)
{
    TWDR = c;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while (!(TWCR & (1 << TWINT)));
    if (TWSR != 0x28)
        return ERROR("Send error");
    uart_printstr("Send over\n");
}

void i2c_start()//Master transmitter
{
    start_slave((0x38 << 1) & ~(1), 0x08);
    _delay_ms(200);

    while (!(TWCR & (1 << TWINT)));
    if (TWSR != 0x18)
        ERROR("Master Transmitter error");
    send_slave(0xBE);
    send_slave(0x08);
    send_slave(0x00);
    
    _delay_ms(10);

    send_slave(0xAC);
    _delay_ms(10);
    send_slave(0x33);
    _delay_ms(10);
    send_slave(0x00);
    _delay_ms(80);

}

void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

int main()
{
    uart_init((CLOCK_SPEED/ (16 * BAUD)));

    i2c_init();
    i2c_start();
    receive();
    i2c_stop();
    while (1);
}