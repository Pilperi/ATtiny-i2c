#ifndef F_CPU
#define F_CPU 1E6
#endif
#include <stdint.h>
#include <util/delay.h>
#include <attiny_i2c.h>
#include "pcf8582.h"

void main(void)
{
    /* Testidata joka kirjoitetaan laitteelle */
    uint8_t data_buff_kirjoita[] = {
        0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
        0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00,
        1<<0, 1<<1, 1<<2, 1<<3
    };
    uint8_t laite_osoite = 0xA0;
    uint8_t laite_muistiosoite = 0x00;
    uint8_t data_buff_lue[20];
    // Katsotaan että laite vastaa kun pingataan
    uint8_t ping_vastaus;
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    ping_vastaus = i2c_ping(laite_osoite);
    i2c_ping(ping_vastaus);
    // Katsotaan että laite ei vastaa muihin pingeihin
    ping_vastaus = i2c_ping(laite_osoite + 0x0E);
    i2c_ping(ping_vastaus);
    // Kirjoita databufferi laitteelle
    pcf8582_kirjoita_bufferi(laite_osoite, laite_muistiosoite, data_buff_kirjoita, 20);
    // Lue laitteen muistista, pitäisi olla samat mitä just kirjoitettiin
    pcf8582_lue_bufferi(laite_osoite,   laite_muistiosoite,   data_buff_lue, 20);
    // Kirjoita osa luetusta datasta (katso että menihän data bufferiin)
    pcf8582_kirjoita_bufferi(laite_osoite, laite_muistiosoite, data_buff_lue, 3);
    for(;;){
        __asm__("nop");
    }
}
