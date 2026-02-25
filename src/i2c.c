/* I2C-kommunikaatiofunktiot */
#include <stdint.h>
#include <avr/io.h>
#include "i2c.h"

// Lähetä yksittäinen tavu
void i2c_laheta(void)
{
    i2c_setup();
    i2c_aloita();
    i2c_siirra();
}

// Lue yksittäinen tavu
void i2c_lue(void)
{
    //
}


// Alusta I2C-kommunikaatioväylä
static void i2c_setup(void)
{
    // Oletuksena ylhäällä, ajetaan alas
    PORTB |= (1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL);
    DDRB |= (1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL);
    // Neutraali nolladata pohjalle
    USIDR = 0x00;
    // Counter interrupt, TWI, kellona Timer0 compare match
    //USICR = (1<<USIOIE)|(1<<USIWM1)|(0<<USIWM0)|(0<<USICS1)|(1<<USICS0);
    USICR = (1<<USIOIE)|(1<<USIWM1)|(0<<USIWM0)|(1<<USICS1)|(0<<USICS0)|(1<<USICLK);
}

// Laita TIMER0 ajastimeksi
static void i2c_kello_kayntiin(void)
{
    // Timer0 100 kHz
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    // Compare match 10 µs välein
    OCR0A = I2C_CLOCK_STANDARD;
    OCR0B = 5;
    // Ei kosketa pinnien toiminnallisuuksiin, CTC-moodi
    TCCR0A = (0 << COM0A1)|(0 << COM0A0) | (0 << COM0B1)|(0 << COM0B0) | (1 << WGM01)|(0 << WGM00);
    TCCR0B = (0 << WGM02) | (0 << CS02)|(0 << CS01)|(1 << CS00);
    // Aseta kellon arvo nollaan
    TCNT0 = 0x00;
}

// Aloita I2C-kommunikaatio
static void i2c_aloita(void)
{
    // SDA ylhäältä alas kun SCL on ylhäällä
    PORTB &= ~(1<<I2C_PIN_SDA);
    __asm__("nop");
    __asm__("nop");
    PORTB &= ~(1<<I2C_PIN_SCL);
    __asm__("nop");
    __asm__("nop");
    while(USISIF==1){; /* Odota että USI huomaa aloituksen */}
}

// Lopeta I2C-kommunikaatio (vapauta väylä)
static void i2c_lopeta(void)
{
    // SDA alhaalta ylös kun SCL on ylhäällä
    PORTB |= (1<<I2C_PIN_SDA);
    __asm__("nop");
    __asm__("nop");
    PORTB |= (1<<I2C_PIN_SCL);
}

// Lue ACK laitteelta
static void i2c_lue_ack(void)
{
    DDRB &= ~(1<<I2C_PIN_SDA);
    USIDR = 0x00;
    i2c_siirra();
}

// Siirrä yksittäinen tavu, suuntaan tai toiseen
static void i2c_siirra(void)
{
    // Kellotetaan overflow asti (8 kelloa ylös ja alas)
    while (!(USISR && (1<<USIOIF)))
    {
        USICR |= (1<<USITC);
        i2c_delay();
    }
    // Vapauta SDA
    DDRB &= ~(1<<I2C_PIN_SDA);
}

static void i2c_delay(void){
    __asm__("nop");
    __asm__("nop");
    __asm__("nop");
}