#include "uart.h"

/* Formula to get the UBRR value:
 * (F_CPU / (16 * UART_BAUDRATE)) - 1
 * Where
 * F_CPU         = 16000000
 * UART_BAUDRATE = 115200
 * (16000000 / (16 * 115200)) - 1 = 7.68
 * We round it: 8
*/
#define UBRR	8

void uart_init() {
	/* UBRRnL and UBRRnH – USART Baud Rate Registers
	*  It defines the number of bps.
	*/
    UBRR0L = (uint8_t)(UBRR & 0xFF);
    UBRR0H = (uint8_t)(UBRR >> 8);

	/* UCSR0B – USART Control and Status Register 0 B
	*  RXEN0: Receiver Enable 0
	*  Writing this bit to one enables the USART Receiver. 
	*  TXEN0: Transmitter Enable 0
	*  Writing this bit to one enables the USART Transmitter.
	*/
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	/* UCSR0C – USART Control and Status Register 0 C
	*  UCSZ01 and UCSZ00 bits activated means
	*  we want 8-bit characters
 	*/
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c) {
	/* UCSR0A – USART Control and Status Register 0 A
	*  UDREn: USART Data Register Empty
	*  The UDREn Flag indicates if the transmit buffer (UDRn)
	*  is ready to receive new data. If UDREn is one, the
	*  buffer is empty, and therefore ready to be written.
	*  The UDREn Flag can generate a Data Register Empty
	*  interrupt (see description of the UDRIEn bit).
	*  UDREn is set after a reset to indicate that the
	*  Transmitter is ready.
	*/
    while(!(UCSR0A & (1 << UDRE0)));

    /* UDR0 – USART I/O Data Register n
	*  The USART Transmit Data Buffer Register and
	*  USART Receive Data Buffer Registers share the same I/O
	*  address referred to as USART Data Register or UDR0.
	*  The Transmit Data Buffer Register (TXB) will be the
	*  destination for data written to the UDR0 Register location.
	*  Reading the UDR0 Register location will return the
	*  contents of the Receive Data Buffer Register (RXB).
	*/
    UDR0 = c;
}

void uart_printstr(const char *str) {
	while (*str) {
		uart_tx(*str);
		if (*str == '\n')
			uart_tx('\r');
		str++;
	}
}
