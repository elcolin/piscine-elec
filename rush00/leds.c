#include "leds.h"

void leds_init() {
	DDRB |= (1 << PB0) | (1 << PB1) |(1 << PB2) |(1 << PB4);
}

void leds_countdown() {
	PORTB |= (1 << PB0);
	_delay_ms(500);
	PORTB |= (1 << PB1);
	_delay_ms(500);
	PORTB |= (1 << PB2);
	_delay_ms(500);
	PORTB |= (1 << PB4);
	_delay_ms(250);
	PORTB ^= ~(1 << PB0) | ~(1 << PB1) | ~(1 << PB2) | ~(1 << PB4);
}

void leds_winner() {
	for (uint8_t i = 0; i < 6; i++) {
		PORTB ^= ~(1 << PB0) | ~(1 << PB1) | ~(1 << PB2) | ~(1 << PB4);
		_delay_ms(500);
	}
}
