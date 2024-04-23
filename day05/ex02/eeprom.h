#ifndef EEPROM_H
#define EEPROM_H

#include "uart.h"
#include <avr/eeprom.h>
#include <avr/io.h>
#include <stdbool.h>

#define MAGIC_NUMBER 0xDEADBEEF

bool safe_eeprom_read(void *dst, size_t offset, size_t length);
bool safe_eeprom_write(void * buffer, size_t offset, size_t length);

#endif