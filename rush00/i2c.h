#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdlib.h>

/* Humidity and Temperature Sensor :
*  the AHT20, its address is 0x38
*  we do one bit shift to left
*  in order to make room for RW (read/write) bit
*/
#define SLA_W (0x00 << 1)

#define ACK 1
#define NACK 0

void i2c_init();
void i2c_start_slave();
void i2c_start_master();
void i2c_stop();
void i2c_write(unsigned char data);
void i2c_read();
void print_hex_value(unsigned char c);
void describe_status_master_transmitter(uint8_t status);
void describe_status_master_receiver(uint8_t status);
void describe_status_slave_transmitter();
void describe_status_slave_receiver();

#endif
