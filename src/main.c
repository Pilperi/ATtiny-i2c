#ifndef F_CPU
#define F_CPU 1E6
#endif
#include <stdint.h>
#include <util/delay.h>
#include "pcf8582.h"

void main(void)
{
    uint8_t teksti[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t laiteosoite = 0xA0;
    uint8_t muistiosoite = 0x00;
    uint8_t maara = 6;
    uint8_t vastaanotto[6];
    // Kirjoita databufferi laitteelle
    pcf8582_kirjoita_bufferi(laiteosoite, muistiosoite, teksti, maara);
    // Laitteella menee ~63 ms kirjoittamiseen
    _delay_ms(64);
    // Lue datan muisti alkaen osoitteesta 1
    pcf8582_lue_bufferi(laiteosoite, muistiosoite+1, vastaanotto, maara);
    for(;;){
        __asm__("nop");
    }
}
