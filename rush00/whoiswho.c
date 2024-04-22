#include "whoiswho.h"

void    become_master_receiver()
{
    i2c_start_master();
    describe_status_master_receiver(TWSR);

    i2c_write((SLA_W << 1) | 1);
    describe_status_master_receiver(TWSR);
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while(1)
    {
        if (TWCR & (1 << TWINT))
        {
			start_game(1);
            // describe_status_master_receiver(TWSR);
            uart_tx(TWDR);
            TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);

        }
        if (SW1_PRESSED)
        {
            _delay_ms(100);
            if (SW1_UNPRESSED)
            {

            }
        }
    }
}

void become_master_transmitter()
{

}


void become_slave()
{
    while(1)
    {
        if (SW1_PRESSED)
        {
			_delay_ms(100);
			i2c_write('A'); // ready to play
			start_game(0);
			describe_status_slave_transmitter();
        }
    }
}