#include <stdint.h>
#include <avr/io.h>
#include "i2c.h"

void main(void)
{
    uint8_t teksti[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t laiteosoite = 0xA0;
    uint8_t maara = 6;
    uint8_t muistiosoite = 0x00;
    i2c_laheta_buffer(teksti, maara, laiteosoite, muistiosoite);
    for(;;){
        __asm__("nop");
    }
}
