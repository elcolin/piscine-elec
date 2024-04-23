#include "uart.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>

#define SW1_PRESSED !(PIND & (1 << PD2))
#define SW1_UNPRESSED (PIND & (1 << PD2))
#define SW2_PRESSED !(PIND & (1 << PD4))
#define SW2_UNPRESSED (PIND & (1 << PD4))

#define MAGIC 124
#define MADIC_ADDR 5
#define COUNT_ADDR 4

void binary_to_led(char value)
{
    unsigned char PBs[4] = {PB0, PB1, PB2, PB4};
    int i = 3;
    while (i >= 0)
    {
        if (1 & (value >> i))
            PORTB |= (1 << PBs[i]);
        else 
            PORTB &= ~(1 << PBs[i]);
        i--;
    }
}

void init_eeprom()
{
	for (uint16_t i = 0; i < 4; i++)
		eeprom_update_byte((uint8_t *) i, 0);
	eeprom_update_byte((uint8_t *) MADIC_ADDR, MAGIC);
	eeprom_update_byte((uint8_t *) COUNT_ADDR, 0);

}

void main()
{	
	uint16_t	current_cnt = 0;
	unsigned char	value = 0;
	DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB4);
    //LED D1, D2, D3 and D4 set as output

    DDRB &= ~(1 << PD2);
    DDRB &= ~(1 << PD4);

	if (eeprom_read_byte((uint8_t *) MADIC_ADDR) != MAGIC)
		init_eeprom();
	else
	{
		current_cnt = eeprom_read_byte((uint8_t *) COUNT_ADDR);
		value = eeprom_read_byte((uint8_t *)current_cnt);
	}
	binary_to_led(value);
	while(1)
	{
		if (SW1_PRESSED)
    	{
			_delay_ms(100);
			if (SW1_UNPRESSED)
			{
				binary_to_led(++value);
				eeprom_update_byte((uint8_t *)current_cnt, value);
			}
    	}
		if (SW2_PRESSED)
    	{
			_delay_ms(100);
			if (SW2_UNPRESSED)
			{
				if (++current_cnt >= COUNT_ADDR)
					current_cnt = 0;
				value = eeprom_read_byte((uint8_t *)current_cnt);
				eeprom_update_byte((uint8_t *) COUNT_ADDR, current_cnt);
				binary_to_led(value);
			}
    	}
	}
}