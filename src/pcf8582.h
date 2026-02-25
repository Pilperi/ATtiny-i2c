/* PCF8582E-2 I2C EEPROM */
#ifndef PCF8582_H
#define PCF8582_H

#include <stdint.h>

#define PCF8582_ADDRESS_BASE 0b1010_0000
#define PCF8582_READ         1<<0
#define PCF8582_WRITE        0<<0

void pcf8582_write_buffer(uint8_t device_address, uint8_t* data_buffer, uint8_t buffer_size, uint8_t* error);
void pcf8582_read_buffer(uint8_t device_address, uint8_t* data_buffer, uint8_t buffer_size, uint8_t* error);

#endif