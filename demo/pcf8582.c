#include <stdint.h>
#include <avr/io.h>
#include "pcf8582.h"
#include <attiny_i2c.h>


/* Kirjoita bufferin data laitteelle */
void pcf8582_kirjoita_bufferi(uint8_t laiteosoite, uint8_t aloitusosoite,
                              uint8_t* bufferi_data, uint8_t bufferin_koko)
{
    uint8_t ack_vastaus;
    uint8_t tavunro = 0;
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    while (tavunro < bufferin_koko)
    {
        if (tavunro)
        {
            i2c_aloita_rep();
        }
        else
        {
            i2c_aloita();
        }
        i2c_kirjoita(laiteosoite & PCF8582_MASK_WRITE);
        ack_vastaus = i2c_ack(I2C_ACK_LUE);
        i2c_kirjoita(aloitusosoite + tavunro);
        ack_vastaus = i2c_ack(I2C_ACK_LUE);
        uint8_t sananumero = 0;
        while ((sananumero < 8) & (tavunro < bufferin_koko))
        {
            i2c_kirjoita(bufferi_data[tavunro]);
            ack_vastaus = i2c_ack(I2C_ACK_LUE);
            sananumero++;
            tavunro++;
        }
        i2c_lopeta();
        ack_vastaus = i2c_ping(laiteosoite);
        while(ack_vastaus){
            /* Laiska delay ~5 ms */
            for(uint16_t cnt=1000; cnt > 0; cnt--){
                __asm__("nop");
            }
            ack_vastaus = i2c_ping(laiteosoite);
        }
    }
}

/* Lue bufferin verran dataa laitteelta, aloittaen aloitusosoitteesta. */
void pcf8582_lue_bufferi(uint8_t laiteosoite, uint8_t aloitusosoite,
                         uint8_t* bufferi_data, uint8_t bufferin_koko)
{
    uint8_t ack_vastaus;
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(laiteosoite & ~(1<<0));
    ack_vastaus = i2c_ack(I2C_ACK_LUE);
    i2c_kirjoita(aloitusosoite);
    ack_vastaus = i2c_ack(I2C_ACK_LUE);
    i2c_aloita_rep();
    i2c_kirjoita(laiteosoite | (1<<0));
    ack_vastaus = i2c_ack(I2C_ACK_LUE);
    DDRB &= I2C_MASK_SDA_N; // Vaihdetaan lukutilaan
    for (uint8_t tavunro=0; tavunro < bufferin_koko; tavunro++)
    {
        bufferi_data[tavunro] = i2c_lue();
        ack_vastaus = i2c_ack(I2C_ACK_KIRJOITA);
    }
    i2c_lopeta();
}
