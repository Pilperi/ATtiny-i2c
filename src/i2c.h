/**/
#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <avr/io.h>

#define I2C_ERROR_NO_ACK 1<<0
#define I2C_PIN_SDA PINB0
#define I2C_PIN_SCL PINB2
#define I2C_CLOCK_STANDARD 5

/* Lähetä databufferi laitteelle

1. Bufferin osoite
2. Bufferin koko tavuina
3. Laiteosoite
4. Kirjoitusosoite laiteella
*/
void i2c_laheta_buffer(uint8_t*, uint8_t, uint8_t, uint8_t);
void i2c_laheta(void);
void i2c_lue(void);

static void i2c_setup(void);
static void i2c_kello_kayntiin(void);
static void i2c_aloita(void);
static void i2c_lopeta(void);
static void i2c_lue_ack(void);
static void i2c_siirra(void);
static void i2c_delay(void);

#endif //I2C_H