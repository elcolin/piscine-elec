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

void ERROR()
{
    uart_printstr("ERROR");
}

void i2c_init()
{
    // TWBR = ((F_CPU / SCL_FREQUENCY) - 16) / 2;//si prescaler = 0;
    // TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
    
}
// (TWSR & 0xF8) == TW_START
void receive()
{
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    if (TWSR == 0x10)
    {
        uart_printstr("start received\n");
        TWDR =  (0x38 << 1) | 1;
    }

    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    uart_printstr("SLA+R transmitted\n");
    if (TWSR == 0x40){
        uart_printstr("entered\n");

        TWCR = (1<<TWINT) | (1<<TWEN);
    }

    while (!(TWCR & (1 << TWINT)));
    if (TWSR == 0x50)
        uart_printstr("command terminee!!\n");
}

void i2c_start()
{
    TWBR = ((F_CPU / SCL_FREQUENCY) - 16) / 2;//si prescaler = 0;
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA);
    while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
    if (TWSR == 0x08)
    {
        uart_printstr("start received\n");
        TWDR =  (0x38 << 1);//
    }
    TWCR = (1 << TWINT) | (1 << TWEN);
    _delay_ms(10);
    while (!(TWCR & (1 << TWINT)));
    if (TWSR == 0x18){
        uart_printstr("SLA+W transmitted entered\n");
        TWDR = 0xAC;
        TWCR = (1<<TWINT) | (1<<TWEN);
    }
    while (!(TWCR & (1 << TWINT)));
    if (TWSR == 0x28)
        uart_printstr("command terminee!!\n");
    receive();

    
}
// void {
//     while (!(TWCR & (1 << TWINT)));
//     if (!(TWSR == 0x40))
//         error()
//     value = TWDR;
//     TWCR = (1<<TWINT) | (1<<TWEN);

// }

void i2c_stop()
{
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

// TWCR |= (1 << TWINT);// on reinitialise TWINT

int main()
{
    uart_init((CLOCK_SPEED/ (16 * BAUD)));

    i2c_init();
    i2c_start();
    i2c_stop();
    while (1);
}


// sei();
// TWCR |= (1 << TWIE);

// // on reinitialise TWINT avant la condition START
//     //transmettre une condition START via TWCR
//     TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
//     while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
//     // on check dans TWSR si c'est le bon status code (start bien envoye)'
//     if ((TWSR & 0xF8) != TW_START)
//         ERROR();
//     //Load SLA+W dans TWDR
//     TWDR = 0x50;
//     //ecrire une valeur specifique dans TWCR, instructing the TWI hardware to transmit the SLA+W present in TWDR. 
//     TWCR = (1 << TWINT) | (1 << TWEN);
//     while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
//     // on check si SLA+W envoye et ACK recu
//     // on check la valeur de TWSR -> packet entier et ACK
//     // si expected, load data packet dans TWDR
//     TWCR |= (1 << TWINT);// on reinitialise TWINT
//     while (!(TWCR & (1 << TWINT)));//on attend le TWI interrupt flag
//     // on check dans TWSR si la data a ete envoye et ACK recu
//     // si expected on ecrit dans TWCR une condition stop
//     TWCR |= (1 << TWINT);// on reinitialise TWINT