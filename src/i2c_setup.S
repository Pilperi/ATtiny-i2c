#define __SFR_OFFSET 0
#include <avr/io.h>
#include <attiny_i2c.h>

.global i2c_setup
.global i2c_aloita
.global i2c_aloita_rep
.global i2c_lopeta
.global i2c_lue
.global i2c_kirjoita
.global i2c_ack
.global i2c_delay

.section .text

; Alusta I2C-väylä
; Sisääntulo:
; R24 : TWI määritelmä
;
; Rekisterikäyttö:
; R18
i2c_setup:
    IN R18,PORTB
    ORI R18,I2C_MASK_SDA_SCL
    OUT PORTB,R18
    IN R18,DDRB
    ORI R18,I2C_MASK_SDA_SCL
    OUT DDRB,R18
    OUT USICR,R24
    RET
