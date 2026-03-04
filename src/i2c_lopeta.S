#define __SFR_OFFSET 0
#include <avr/io.h>
#include "i2c.h"

.global i2c_setup
.global i2c_aloita
.global i2c_aloita_rep
.global i2c_lopeta
.global i2c_lue_kahdeksan
.global i2c_kirjoita_kahdeksan
.global i2c_ack
.global i2c_delay

.section .text

; Lähetä I2C lopetussignaali viemällä data ylös kellon ollessa ylhäällä
; Olettaa että DDRB on asetettu ennen kutsua.
; Rekisterikäyttö:
; R18
i2c_lopeta:
    CBI USIDR,7
    LDI R18,I2C_MASK_SDA_SCL
    OUT DDRB,R18
    CBI PORTB,I2C_PIN_SDA
    CBI DDRB,I2C_PIN_SCL
    RCALL i2c_delay
    CBI DDRB,I2C_PIN_SDA
    LDI R18,I2C_USISR_CLR
    OUT USISR,R18          ; Kuittaa lopetus
    RET
