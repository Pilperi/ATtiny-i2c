#include <stdint.h>
#include "pcf8582.h"
#include "i2c.h"

/* Kirjoita bufferin data laitteelle */
void pcf8582_kirjoita_bufferi(uint8_t laiteosoite, uint8_t aloitusosoite,
                              uint8_t* bufferi_data, uint8_t bufferin_koko, uint8_t* error)
{
    ;
}

/* Lue bufferin verran dataa laitteelta, aloittaen aloitusosoitteesta. */
void pcf8582_read_buffer(uint8_t device_address, uint8_t* data_buffer, uint8_t buffer_size, uint8_t* error)
{
    //
}