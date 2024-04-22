#ifndef WHOISWHO_H
#define WHOISWHO_H

#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"
#include "uart.h"
#include "game.h"

#define SW1_PRESSED !(PIND & (1 << PD2))
#define SW1_UNPRESSED (PIND & (1 << PD2))

void    become_master_receiver();
void	become_slave();

#endif