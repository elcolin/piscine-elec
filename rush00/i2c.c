#include "i2c.h"
#include "uart.h"

/* Formula to get I2C frequency to 100hz
*  (F_CPU / (2 * SCL)) - 8
*  Where:
*  F_CPU = 16Mhz and SCL = 100Hz
*  (16000000 / (2 * 100000)) - 8 = 72
*/
void i2c_init() {
	/* TWBR – TWI Bit Rate Register
	*  TWBR selects the division factor
	*  for the bit rate generator. 
	*/
	TWBR = 72;
	/* TWSR – TWI Status Register
	*  Set this Prescaler bits to 0
	*  Means prescaler value is 1
	*  (no prescaling)
	*/
	TWSR = 0;
}

void i2c_start_slave()
{
	TWCR = 0;
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
    TWAR = (1 << TWGCE);
    // while (!(TWCR & (1 << TWINT)));
}

void i2c_start_master() {
	/* TWCR – TWI Control Register
	*  - The TWEN bit enables TWI operation
	*  and activates the TWI interface
	*  - Enable TWSTA bit to one when to
	*  become a Master on the 2-wire Serial Bus.
	*  Before any operation by the software,
	* the TWINT Flag must be cleared by software
	* by writing a logic one to it
	*/
	/* [STEP] 1 & 2 of datasheet (p.225)*/
	/*Send START condition*/
	TWCR = 0;
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	/* Wait for TWINT Flag set.
	*  This indicates that the START
	*  condition has been transmitted
	*/
    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop() {
	/* STEP 7.5 of datasheet (p.225) */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(unsigned char data) {
	/* TWDR – TWI Data Register
	*  We write the next byte to be transmitted
	*/
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_read(void) {
	//TWDR = SLA_W | 1;
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
}

void print_hex_value(unsigned char c) {
	char hex_digits[] = "0123456789ABCDEF";

    /* Computing MSB then LSB */
	uart_tx(hex_digits[c >> 4]);
	uart_tx(hex_digits[c & 0x0F]);
}

void describe_status_slave_transmitter() {
	uart_printstr("[ST]: ");
	switch (TWSR & 0xF8) {
		case 0xA8:
			uart_printstr("Own SLA+R has been received; ACK has been returned.\n");
			break;
		case 0xB0:
			uart_printstr("Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned.\n");
			break;
		case 0xB8:
			uart_printstr("Data byte in TWDR has been transmitted; ACK has been received.\n");
			break;
		case 0xC0:
			uart_printstr("Data byte in TWDR has been transmitted; NOT ACK has been received.\n");
			break;
		case 0xC8:
			uart_printstr("Last data byte in TWDR has been transmitted (TWEA = “0”); ACK has been received.\n");
			break;
		default:
		uart_printstr("Cannot identify the status code.\n");
	}
}

void describe_status_slave_receiver() {
	uart_printstr("[SR]: ");
	switch (TWSR & 0xF8) {
		case 0x60:
			uart_printstr("Own SLA+W has been received; ACK has been returned.\n");
			break;
		case 0x68:
			uart_printstr("Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned.\n");
			break;
		case 0x70:
			uart_printstr("General call address has been received; ACK has been returned.\n");
			break;
		case 0x78:
			uart_printstr("Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned.\n");
			break;
		case 0x80:
			uart_printstr("Previously addressed with own SLA+W; data has been received; ACK has been returned.\n");
			break;
		case 0x88:
			uart_printstr("Previously addressed with own SLA+W; data has been received; NOT ACK has been returned.\n");
			break;
		case 0x90:
			uart_printstr("Previously addressed with general call; data has been received; ACK has been returned.\n");
			break;
		case 0x98:
			uart_printstr("Previously addressed with general call; data has been received; NOT ACK has been returned.\n");
			break;
		case 0xA0:
		uart_printstr("A STOP condition or repeated START condition has been received while still addressed as Slave.\n");
			break;
		default:
		uart_printstr("Cannot identify the status code.\n");
	}
}

void describe_status_master_transmitter(uint8_t status) {
	uart_printstr("[MT]: ");
	switch (status) {
	case 0x08:
		uart_printstr("A START condition has been transmitted.\n");
		break;

	case 0x10:
		uart_printstr("A repeated START condition has been transmitted.\n");
		break;

	case 0x18:
		uart_printstr("SLA+W has been transmitted; ACK has been received.\n");
		break;

	case 0x20:
		uart_printstr("SLA+W has been transmitted; NOT ACK has been received.\n");
		break;

	case 0x28:
		uart_printstr("Data byte has been transmitted; ACK has been received.\n");
		break;

	case 0x30:
		uart_printstr("Data byte has been transmitted; NOT ACK has been received.\n");
		break;

	case 0x38:
		uart_printstr("Arbitration lost in SLA+W or data bytes.\n");
		break;

	default:
		uart_printstr("Cannot identify the status code.\n");
	}
}

void describe_status_master_receiver(uint8_t status) {
	uart_printstr("[MR]: ");
	switch (status) {
	case 0x08:
		uart_printstr("A START condition has been transmitted.\n");
		break;

	case 0x10:
		uart_printstr("A repeated START condition has been transmitted.\n");
		break;


	case 0x38:
		uart_printstr("Arbitration lost in SLA+R or NOT ACK bit.\n");
		break;

	case 0x40:
		uart_printstr("SLA+R has been transmitted; ACK has been received.\n");
		break;

	case 0x48:
		uart_printstr("SLA+R has been transmitted; NOT ACK has been received.\n");
		break;

	case 0x50:
		uart_printstr("Data byte has been received; ACK has been returned.\n");
		break;

	case 0x58:
		uart_printstr("Data byte has been received; NOT ACK has been returned.\n");
		break;

	default:
		uart_printstr("Cannot identify the status code.\n");
	}
}
