/* I2C-funktioiden headerit */
#ifndef I2C_H
#define I2C_H

/* Käytetään sekä assemblyssä että C-koodissa: */
#include <avr/io.h>

/* USI sijainti vaihtelee tinymallin mukaan */
/* ATtiny85 */
#ifdef (__AVR_ATtiny85__)
#define I2C_PIN_SDA          PINB0
#define I2C_PIN_SCL          PINB2
#define I2C_DDR              DDRB
#define I2C_PINREG           PINB
#define I2C_PORTREG          PORTB

/* ATtiny2313 */
#elif defined (__AVR_ATtiny2313__)
#define I2C_PIN_SDA          PINB5
#define I2C_PIN_SCL          PINB7
#define I2C_DDR              DDRB
#define I2C_PINREG           PINB
#define I2C_PORTREG          PORTB

/* Tuntematon (ei mul ole kuin näitä kahta) */
#else
#   error "Arkkitehtuuria ei määritelty tai ei tueta!"
#endif

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


/* Alusta I2C-väylä */
void i2c_setup(uint8_t toimintamoodi);

/* Lähetä I2C-aloitussignaali: SDA alas SCL ollessa ylhäällä */
void i2c_aloita(void);

/* Lähetä toistettu aloitus */
void i2c_aloita_rep(void);

/* Lähetä I2C-lopetussignaali: SDA ylös SCL ollessa ylhäällä */
void i2c_lopeta(void);

/* Lue tavu (palauttaa luetun datan) */
uint8_t i2c_lue(void);

/* Kirjoita tavu (palauttaa kirjoitetun datan)*/
uint8_t i2c_kirjoita(uint8_t data);

/* Toimita ACK: jos lue 0, kirjoita ACK. Muutoin lue ACK ja palauta. */
uint8_t i2c_ack(uint8_t lue);

/* Pingaa laiteosoitetta. Palauttaa laitteen ACK (0 jos paikalla) */
uint8_t i2c_ping(uint8_t laiteosoite);

#endif // __ASSEMBLER__
#endif //I2C_H