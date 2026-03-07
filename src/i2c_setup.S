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
    IN R18,I2C_PORTREG
    ORI R18,I2C_MASK_SDA_SCL
    OUT I2C_PORTREG,R18
    IN R18,I2C_DDR
    ORI R18,I2C_MASK_SDA_SCL
    OUT I2C_DDR,R18
    OUT USICR,R24
    LDI R18,0xFF
    OUT USIBR,R18
    RET
