#include "uart.h"
#include "eeprom.h"
#include <util/delay.h>


void main()
{	
	uart_init();
	char *str = "eeprom";
	uart_tx('A');
	while (1)
	{
		uart_tx('A');
		// if(safe_eeprom_read(str, 0, 7) == true)
		// 	uart_printstr(str);
		// else
		// {
		// 	uart_printstr("false");
		// 	return;
		// }
	}
	
}