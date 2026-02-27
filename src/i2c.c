/* I2C-kommunikaatiofunktiot */
#include <stdint.h>
#include <avr/io.h>
#include "i2c.h"

/* prototyypit */
static void i2c_setup(void);
static void i2c_aloita(void);
static void i2c_laheta(void);
static void i2c_lue_ack(void);
static void i2c_lopeta(void);
static void i2c_delay(void);

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


// Alusta I2C-kommunikaatioväylä
static void i2c_setup(void)
{
    // Oletuksena ylhäällä, ajetaan alas
    PORTB |= (1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL);
    DDRB |= (1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL);
    // Neutraali nolladata pohjalle
    USIDR = 0x00;
    // Softakellotus
    USICR = (1<<USIWM1)|(1<<USICS1)|(1<<USICLK);
}

// Aloita I2C-kommunikaatio
static void i2c_aloita(void)
{
    // SDA ylhäältä alas kun SCL on ylhäällä
    PORTB &= ~(1<<I2C_PIN_SDA);
    while(PINB & (1<<I2C_PIN_SDA)){;} // Odotetaan tilan vaihtumista
    PORTB &= ~(1<<I2C_PIN_SCL);
    while(PINB & (1<<I2C_PIN_SCL)){;} // Odotetaan tilan vaihtumista
}

// Lopeta I2C-kommunikaatio (vapauta väylä)
static void i2c_lopeta(void)
{
    // SDA alhaalta ylös kun SCL on ylhäällä
    // Molemmat nollaan jos jossain muualla
    PORTB &= ~(1<<I2C_PIN_SCL);
    PORTB &= ~(1<<I2C_PIN_SDA);
    PORTB |= (1<<I2C_PIN_SCL);
    while(PINB & (1<<I2C_PIN_SCL)){;} // Odotetaan tilan vaihtumista
    PORTB |= (1<<I2C_PIN_SDA);
    while(PINB & (1<<I2C_PIN_SDA)){;} // Odotetaan tilan vaihtumista
    DDRB &= ~(1<<I2C_PIN_SCL);
    DDRB &= ~(1<<I2C_PIN_SDA);
}

// Lue ACK laitteelta
static void i2c_lue_ack(void)
{
    uint8_t ddrb_val = DDRB; // Otetaan alkuperäinen arvo talteen
    DDRB &= ~(1<<I2C_PIN_SDA);
    USIDR = 0x00;
    USICR |= (1<<USITC);
    i2c_delay();
    USICR |= (1<<USITC);
    USISR = 0x00;
    DDRB = ddrb_val; // Palautetaan DDRB lähtötilaan
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

// Lähetä tavun verran tavaraa
static void i2c_laheta(void)
{
    PORTB |= (1<<I2C_PIN_SDA);
    DDRB |= (1<<I2C_PIN_SCL)|(1<<I2C_PIN_SDA);
    USISR &= 0xF0;
    i2c_siirra_kahdeksan();
}

// Lähetä tavun verran tavaraa
static void i2c_lue(void)
{
    PORTB |= (1<<I2C_PIN_SDA);
    DDRB |= (1<<I2C_PIN_SCL);
    DDRB &= ~(1<<I2C_PIN_SDA);
    USISR &= 0xF0;
    i2c_siirra_kahdeksan();
}

static void i2c_delay(void){
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
}