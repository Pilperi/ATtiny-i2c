/* I2C-funktioiden headerit */
#ifndef I2C_H
#define I2C_H

/* Käytetään sekä assemblyssä että C-koodissa: */
#include <avr/io.h>

#define I2C_PIN_SDA        PINB0
#define I2C_PIN_SCL        PINB2
#define I2C_MASK_SDA       (1<<I2C_PIN_SDA)
#define I2C_MASK_SCL       (1<<I2C_PIN_SCL)
#define I2C_MASK_SDA_N     (0xFF^I2C_MASK_SDA)
#define I2C_MASK_SCL_N     (0xFF^I2C_MASK_SCL)
#define I2C_MASK_SDA_SCL   (I2C_MASK_SDA | I2C_MASK_SCL)
#define I2C_MASK_SDA_SCL_N (0xFF^(I2C_MASK_SDA | I2C_MASK_SCL_N))

/* Käytetään vain C-kielessä */
#ifndef __ASSEMBLER__
#include <stdint.h>

/* Databufferioperaatiot, argumentit
1. Bufferin osoite
2. Bufferin koko tavuina
3. Laiteosoite
*/
void i2c_laheta_buffer(uint8_t*, uint8_t, uint8_t);
void i2c_lue_buffer(uint8_t*, uint8_t, uint8_t);

/* Alkeisfunktiot */
void i2c_setup(void);
void i2c_aloita(void);
void i2c_lopeta(void);
void i2c_siirra_kahdeksan(void);
void i2c_lue_ack(void);
void i2c_kirjoita_ack(void);

#endif // __ASSEMBLER__
#endif //I2C_H