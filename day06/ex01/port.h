#ifndef LED_H
#define LED_H
#include <avr/io.h>

#define DDRX DDRD
#define PORTX PORTD
#define LED_B PD3
#define LED_R PD5
#define LED_G PD6

void init_port(char port);
void toggle_port(char port);
void set_rgb(uint8_t red, uint8_t green, uint8_t blue);
void port_off(char port);
void port_on(char port);

#endif