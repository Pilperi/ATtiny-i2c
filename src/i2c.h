/* I2C-funktioiden headerit */
#ifndef I2C_H
#define I2C_H

/* Käytetään sekä assemblyssä että C-koodissa: */
#include <avr/io.h>

#define I2C_PIN_SDA PINB0
#define I2C_PIN_SCL PINB2

/* Käytetään vain C-kielessä */
#ifndef __ASSEMBLER__
#include <stdint.h>

/* Lähetä databufferi laitteelle

1. Bufferin osoite
2. Bufferin koko tavuina
3. Laiteosoite
*/
void i2c_laheta_buffer(uint8_t*, uint8_t, uint8_t);

#endif // __ASSEMBLER__
#endif //I2C_H