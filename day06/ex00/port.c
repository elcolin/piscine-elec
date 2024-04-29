#include "port.h"


void init_port(char port)
{
    DDRX |= (1 << port);
}

void toggle_port(char port)
{
    PORTX ^= (1 << port);
}