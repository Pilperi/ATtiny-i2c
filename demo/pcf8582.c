#include <stdint.h>
#include <avr/io.h>
#include "pcf8582.h"
#include "i2c.h"

/* Kirjoita bufferin data laitteelle */
void pcf8582_kirjoita_bufferi(uint8_t laiteosoite, uint8_t aloitusosoite,
                              uint8_t* bufferi_data, uint8_t bufferin_koko)
{
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    USIDR = laiteosoite & ~(1<<0); // Kirjoitusmoodi
    i2c_kirjoita_kahdeksan();
    i2c_ack(I2C_ACK_LUE);
    USIDR = aloitusosoite;
    i2c_kirjoita_kahdeksan();
    i2c_ack(I2C_ACK_LUE);
    for (uint8_t tavunro=0; tavunro < bufferin_koko; tavunro++)
    {
        USIDR = bufferi_data[tavunro];
        i2c_kirjoita_kahdeksan();
        i2c_ack(I2C_ACK_LUE);
    }
    i2c_lopeta();
}

/* Lue bufferin verran dataa laitteelta, aloittaen aloitusosoitteesta. */
void pcf8582_lue_bufferi(uint8_t laiteosoite, uint8_t aloitusosoite,
                         uint8_t* bufferi_data, uint8_t bufferin_koko)
{
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    USIDR = laiteosoite & ~(1<<0); // Kirjoitusmoodi
    i2c_kirjoita_kahdeksan();
    i2c_ack(I2C_ACK_LUE);
    USIDR = aloitusosoite;
    i2c_kirjoita_kahdeksan();
    i2c_ack(I2C_ACK_LUE);
    i2c_aloita_rep();
    USIDR = laiteosoite | (1<<0); // Lukumoodi
    i2c_kirjoita_kahdeksan();
    i2c_ack(I2C_ACK_LUE);
    DDRB &= I2C_MASK_SDA_N;
    for (uint8_t tavunro=0; tavunro < bufferin_koko; tavunro++)
    {
        i2c_lue_kahdeksan();
        bufferi_data[tavunro] = USIDR;
        i2c_ack(I2C_ACK_KIRJOITA);
    }
    i2c_lopeta();
}
