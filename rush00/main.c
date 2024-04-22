#include "uart.h"
#include "i2c.h"
#include "whoiswho.h"

int main() {
    DDRB &= ~(1 << PD2);
	leds_init();
    uart_init();

    i2c_init();
    i2c_start_slave();
    while (1) {
		// MASTER
        if (SW1_PRESSED)
        {
            _delay_ms(100);
            if (SW1_UNPRESSED)
                become_master_receiver();
        }
		// SLAVE
        if (TWCR & (1 << TWINT))
        {
            describe_status_slave_transmitter(TWSR);
			become_slave();
        }
    }

    return 0;
}