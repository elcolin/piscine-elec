#include "i2c.h"
#include "uart.h"

/* Formula to get I2C frequency to 100hz
*  (F_CPU / (2 * SCL)) - 8
*  Where:
*  F_CPU = 16Mhz and SCL = 100Hz
*  (16000000 / (2 * 100000)) - 8 = 72
*/
void i2c_init() {
	TWBR = 72;
	TWSR = 0;
}


void i2c_start() {
	TWCR = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
	print_status();
}

void i2c_stop() {
	/* STEP 7.5 of datasheet (p.225) */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(unsigned char data) {

	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	print_status();

}

void i2c_read(void) {
	//TWDR = SLA_W | 1;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	print_status();

}

void print_hex_value(unsigned char c) {
	char hex_digits[] = "0123456789ABCDEF";

    /* Computing MSB then LSB */
	uart_tx(hex_digits[c >> 4]);
	uart_tx(hex_digits[c & 0x0F]);
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
