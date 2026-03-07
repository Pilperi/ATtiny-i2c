/* I2C-funktioiden headerit */
#ifndef I2C_H
#define I2C_H

/* Käytetään sekä assemblyssä että C-koodissa: */
#include <avr/io.h>

#define I2C_PIN_SDA          PINB0
#define I2C_PIN_SCL          PINB2
#define I2C_MASK_SDA         (1<<I2C_PIN_SDA)
#define I2C_MASK_SCL         (1<<I2C_PIN_SCL)
#define I2C_MASK_SDA_N      ~(I2C_MASK_SDA)
#define I2C_MASK_SCL_N      ~(I2C_MASK_SCL)
#define I2C_MASK_SDA_SCL     (I2C_MASK_SDA | I2C_MASK_SCL)
#define I2C_MASK_SDA_SCL_N  ~((I2C_MASK_SDA | I2C_MASK_SCL_N))

#define I2C_MOODI_SOFTAKELLO ((1<<USIWM1)|(1<<USICS1)|(0<<USICS0)|(1<<USICLK))
#define I2C_MOODI_TIMER0     ((1<<USIWM1)|(1<<USICS0))
#define I2C_USISR_CLR        0xF0

#define I2C_ACK_KIRJOITA     0
#define I2C_ACK_LUE          1

/* Käytetään vain C-kielessä */
#ifndef __ASSEMBLER__
#include <stdint.h>

/* Alkeisfunktiot */
void i2c_setup(uint8_t);
void i2c_aloita(void);
void i2c_aloita_rep(void);
void i2c_lopeta(void);
uint8_t i2c_lue(void);
uint8_t i2c_kirjoita(uint8_t);
uint8_t i2c_ack(uint8_t lue);

#endif // __ASSEMBLER__
#endif //I2C_H