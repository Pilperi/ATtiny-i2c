#ifndef F_CPU
#define F_CPU 1E6
#endif
#include <stdint.h>
#include <util/delay.h>
#include "pcf8582.h"

void main(void)
{
    uint8_t data_buff_kirjoita[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t laite_osoite = 0xA0;
    uint8_t laite_muistiosoite = 0x00;
    uint8_t data_buff_lue[6];
    // Kirjoita databufferi laitteelle
    pcf8582_kirjoita_bufferi(laite_osoite, laite_muistiosoite, data_buff_kirjoita, 6);
    // Laitteella menee ~63 ms kirjoittamiseen
    _delay_ms(64);
    // Lue datan muisti, ekalla lukukerralla ulos pitäisi tulla
    // aiemmin kirjoitettu data, ja jälkimmäisten kutsujen pitäisi
    // kirjoittaa samat arvot vanhojen päälle ts.
    // data_buff_lue = data_buff_kirjoita
    // data_buff_lue[1:] = data_buff_kirjoita[1:]
    // data_buff_lue[2:] = data_buff_kirjoita[2:]
    // data_buff_lue[3:] = data_buff_kirjoita[3:]
    pcf8582_lue_bufferi(laite_osoite,   laite_muistiosoite,   data_buff_lue, 6);
    pcf8582_lue_bufferi(laite_osoite+1, laite_muistiosoite+1, data_buff_lue, 5);
    pcf8582_lue_bufferi(laite_osoite+2, laite_muistiosoite+2, data_buff_lue, 4);
    pcf8582_lue_bufferi(laite_osoite+3, laite_muistiosoite+3, data_buff_lue, 3);
    // Kirjoita luettu data (tarkistetaan että onhan data_buff_lue == data_buff_kirjoita)
    pcf8582_kirjoita_bufferi(laite_osoite, laite_muistiosoite, data_buff_lue, 6);
    for(;;){
        __asm__("nop");
    }
}
