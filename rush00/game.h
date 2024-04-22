#ifndef GAME_H
#define GAME_H

#include "leds.h"

#define SW1_PRESSED !(PIND & (1 << PD2))
#define SW1_UNPRESSED (PIND & (1 << PD2))

void start_game(uint8_t im_master);

#endif