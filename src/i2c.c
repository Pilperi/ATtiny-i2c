/* I2C-kommunikaatiofunktiot */
#include <stdint.h>
#include <avr/io.h>
#include "i2c.h"

/* prototyypit */
static void i2c_delay(void);

/* Alusta I2C-kommunikaatioväylä
Kutsun jälkeen:
DDRB  SDA + SCL
PORTB SDA + SCL
*/
void i2c_setup(void)
{
    // Oletuksena ylhäällä, DDRB ajaa alas tarvittaessa
    PORTB |= I2C_MASK_SDA_SCL;
    DDRB |= I2C_MASK_SDA_SCL;
    // Neutraali nolladata pohjalle
    USIDR = 0x00;
    // TWI, softakellotus
    USICR = (1<<USIWM1)|(1<<USICS1)|(1<<USICLK);
    USISR = 0xF0;
}

/* Aloita I2C-kommunikaatio
Ennen kutsua:
DDRB  SDA + SCL
PORTB SDA + SCL
*/
void i2c_aloita(void)
{
    // SDA ylhäältä alas kun SCL on ylhäällä
    PORTB &= I2C_MASK_SDA_N;
    i2c_delay();
    // Myös SCL alas
    PORTB &= I2C_MASK_SCL_N;
    i2c_delay();
    // PORTB takaisin oletus-ylös
    PORTB |= I2C_MASK_SDA;
    USISR = 0xF0;
}

/* Lopeta I2C-kommunikaatio (vapauta väylä)
Ennen kutsua:
DDRB  SDA + SCL
PORTB !SDA + !SCL

Kutsun jälkeen
DDRB  !SDA + !SCL
PORTB !SDA + !SCL
*/
void i2c_lopeta(void)
{
    // SDA alhaalta ylös kun SCL on ylhäällä
    // SCL ylös
    DDRB &= I2C_MASK_SCL_N;
    i2c_delay();
    // SDA ylös
    DDRB &= I2C_MASK_SDA_N;
    i2c_delay();
}

/* Lue ACK laitteelta
Ennen kutsua:
DDRB  SDA + SCL
PORTB SDA + SCL

Kutsun jälkeen
DDRB  SDA + SCL

lue : 0 jos kirjoitetaan ACK
      >0 jos luetaan ACK
*/ 
void i2c_ack(uint8_t lue)
{
    USISR = 0xFE;
    USIDR = 0x00;
    if (lue)
    {
        DDRB &= I2C_MASK_SDA_N;
    }
    else
    {
        DDRB |= I2C_MASK_SDA;
    }
    USICR |= (1<<USITC);
    i2c_delay();
    USICR |= (1<<USITC);
    if (lue)
    {
        DDRB |= I2C_MASK_SDA;
    }
    else
    {
        DDRB &= I2C_MASK_SDA_N;
    }
    USISR = 0xF0;
}

/* Siirrä yksittäinen tavu, suuntaan tai toiseen
Ennen kutsua:
DDRB  SCL, SDA riippuu suunnasta
PORTB SDA + SCL

Kutsun jälkeen
DDRB  !SDA
*/
void i2c_siirra_kahdeksan(void)
{
    USISR &= 0xF0;
    while(!(USISR & (1<<USIOIF)))
    {
        USICR |= (1<<USITC);
        i2c_delay();
        USICR |= (1<<USITC);
        i2c_delay();
    }
    DDRB &= I2C_MASK_SDA_N; // Vapauta SDA ACK varten
    USISR = 0x00;
}

static void i2c_delay(void){
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
}