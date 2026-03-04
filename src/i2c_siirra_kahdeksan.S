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

; Siirrä dataa TWI pitkin, suuntaan tai toiseen
; Rekisterikäyttö:
; R18
i2c_lue_kahdeksan:
    CBI DDRB,I2C_PIN_SDA
    RJMP i2c_siirra_kahdeksan
i2c_kirjoita_kahdeksan:
    SBI DDRB,I2C_PIN_SDA
i2c_siirra_kahdeksan:
    LDI R18,I2C_USISR_CLR
    OUT USISR,R18         ; Laskuri nollaan
i2c_siirra_loop:
    SBI USICR,USITC
    RCALL i2c_delay
    SBI USICR,USITC
    SBIS USISR,USIOIF
    RJMP i2c_siirra_loop
    LDI R18,I2C_USISR_CLR
    OUT USISR,R18         ; Nollaa laskuri ja flagit
    RET
