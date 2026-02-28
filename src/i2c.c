/* I2C-kommunikaatiofunktiot */
#include <stdint.h>
#include <avr/io.h>
#include "i2c.h"

/* prototyypit */
static void i2c_delay(void);
static void i2c_siirra_kahdeksan(void);

// Lähetä databufferi laitteelle
void i2c_laheta_buffer(uint8_t* bufferi, uint8_t bufferin_koko, uint8_t laiteosoite)
{
    i2c_setup();
    i2c_aloita();
    laiteosoite &= ~(1<<0);
    USIDR = laiteosoite;
    i2c_laheta();
    i2c_lue_ack();
    for (uint8_t tavunro=0; tavunro < bufferin_koko; tavunro++)
    {
        USIDR = bufferi[tavunro];
        i2c_laheta();
        i2c_lue_ack();
    }
    i2c_lopeta();
}

// Lue dataa laitteelta bufferiin
void i2c_lue_buffer(uint8_t* bufferi, uint8_t bufferin_koko, uint8_t laiteosoite)
{
    i2c_setup();
    i2c_aloita();
    laiteosoite |= (1<<0);
    USIDR = laiteosoite;
    i2c_laheta();
    i2c_lue_ack();
    for (uint8_t tavunro=0; tavunro < bufferin_koko; tavunro++)
    {
        i2c_lue();
        bufferi[tavunro] = USIDR;
        i2c_kirjoita_ack();
    }
    i2c_lopeta();
}


// Alusta I2C-kommunikaatioväylä
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
DDRB oltava päällä SDA ja SCL ennen kutsua
PORTB oltava molemmat ylhäällä
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
DDRB  päällä SDA ja SCL
PORTB molemmat alhaalla
*/
void i2c_lopeta(void)
{
    // SDA alhaalta ylös kun SCL on ylhäällä
    // Molemmat nollaan jos jossain muualla
    // SCL ylös
    DDRB &= I2C_MASK_SCL_N;
    i2c_delay();
    // SDA ylös
    DDRB &= I2C_MASK_SDA_N;
    i2c_delay();
}

/* Lue ACK laitteelta
DDRB  päällä sekä SDA että SCL kutsun jälkeen.
*/ 
void i2c_lue_ack(void)
{
    USISR = 0xFE;
    USIDR = 0x00;
    DDRB &= I2C_MASK_SDA_N;
    USICR |= (1<<USITC);
    i2c_delay();
    USICR |= (1<<USITC);
    DDRB |= I2C_MASK_SDA;
    USISR = 0xF0;
}

// Kirjoita ACK laitteelle
void i2c_kirjoita_ack(void)
{
    USISR = 0xFE;
    USIDR = 0x00;
    DDRB |= I2C_MASK_SDA;
    USICR |= (1<<USITC);
    i2c_delay();
    USICR |= (1<<USITC);
    DDRB &= I2C_MASK_SDA_N;
    USISR = 0xF0;
}

// Lähetä tavun verran tavaraa
void i2c_laheta(void)
{
    USISR &= 0xF0;
    i2c_siirra_kahdeksan();
}

// Lähetä tavun verran tavaraa
void i2c_lue(void)
{
    USISR &= 0xF0;
    i2c_siirra_kahdeksan();
}

// Siirrä yksittäinen tavu, suuntaan tai toiseen
static void i2c_siirra_kahdeksan(void)
{
    while(!(USISR & (1<<USIOIF)))
    {
        USICR |= (1<<USITC);
        i2c_delay();
        USICR |= (1<<USITC);
        i2c_delay();
    }
    DDRB &= ~(1<<I2C_PIN_SDA);
    USISR = 0x00;
}

static void i2c_delay(void){
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
}