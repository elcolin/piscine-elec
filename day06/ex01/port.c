#include "port.h"


void init_port(char port)
{
    DDRX |= (1 << port);
}

void toggle_port(char port)
{
    PORTX ^= (1 << port);
}

void port_on(char port)
{
	PORTX |= (1 << port);
}

void port_off(char port)
{
	PORTX &= ~(1 << port);
	// PORTB &= ~(1 << PB0);
}

void set_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
	char	led[] = {LED_R, LED_G, LED_B};
	uint8_t	set[] = {red, green, blue};
	for (int i = 0; i < 3; i++)
	{
		if (set[i])
			port_on(led[i]);
		else
			port_off(led[i]);

	}
}