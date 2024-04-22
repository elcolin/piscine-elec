#include "uart.h"

if (SW1_PRESSED)
        {
            _delay_ms(100);
            if (SW1_UNPRESSED)
                become_master_receiver();
        }

void EEPROM_write(/*ajouter ici*/)
{
    
}

void main()
{

}