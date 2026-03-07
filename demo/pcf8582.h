/* PCF8582E-2 I2C EEPROM */
#ifndef PCF8582_H
#define PCF8582_H

#include <stdint.h>

#define PCF8582_ADDRESS_BASE 0b1010_0000
#define PCF8582_MASK_READ         1<<0
#define PCF8582_MASK_WRITE      ~(1<<0)

void pcf8582_kirjoita_bufferi(uint8_t, uint8_t, uint8_t*, uint8_t);
void pcf8582_lue_bufferi(uint8_t, uint8_t, uint8_t*, uint8_t);

#endif // PCF8582_H
