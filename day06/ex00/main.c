#include "port.h"
#include <util/delay.h>

void main()
{
	char RGB[3] = {LED_R, LED_G, LED_B};
	for (int i = 0; i < 3; i++)
		init_port(RGB[i]);
	while (1)
	{
		for (int i = 0; i < 3; i++)
		{
			toggle_port(RGB[i]);
			_delay_ms(1000);
			toggle_port(RGB[i]);
		}
	}
	
}