#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdlib.h>

#define SLA_W (0x38 << 1)
#define SLA_R ((0x38 << 1) | 1)

#define ACK 1
#define NACK 0

void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(unsigned char data);
void i2c_read();
void print_hex_value(unsigned char c);
void print_status();

#endif
