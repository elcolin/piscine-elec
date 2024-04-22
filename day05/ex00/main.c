#include "uart.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>

#define SW1_PRESSED !(PIND & (1 << PD2))
#define SW1_UNPRESSED (PIND & (1 << PD2))
#define	ADDR 0

void binary_to_led(int value)
{
    char PBs[4] = {PB0, PB1, PB2, PB4};
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

void main()
{	
	DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB4);
    //LED D1, D2, D3 and D4 set as output

    DDRB &= ~(1 << PD2);
	char value = eeprom_read_byte(ADDR);

	if (value >= 15)
	{
		value = 0;
		eeprom_write_byte(ADDR, value);
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
				eeprom_write_byte(ADDR, value);
			}
    	}
	}
}