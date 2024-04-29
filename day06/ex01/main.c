#include "port.h"
#include <util/delay.h>

void main()
{
	char RGB[3] = {LED_R, LED_G, LED_B};
	char schedule[][3] = {{1, 0, 0}, {0, 1, 0}, {0,0,1}, {1, 1, 0}, {0, 1, 1}, {1, 0, 1}, {1, 1, 1}};
	for (int i = 0; i < 3; i++)
		init_port(RGB[i]);
	while (1)
	{
		for (int i = 0; i < 7; i++)
		{
			set_rgb(schedule[i][0], schedule[i][1], schedule[i][2]);
			_delay_ms(1000);
		}
	}
	
}