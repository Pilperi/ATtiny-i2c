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

; Lähetä I2C lopetussignaali viemällä data ylös kellon ollessa ylhäällä
; Kutsun jälkeen DDR on asetettu SDA:lle ja SCL:lle.
;
; Rekisterikäyttö:
; R18
i2c_lopeta:
    SBI USIDR,7
    IN R18,I2C_DDR
    ORI R18,I2C_MASK_SDA_SCL
    OUT I2C_DDR,R18
    CBI I2C_PORTREG,I2C_PIN_SDA
    SBI I2C_PORTREG,I2C_PIN_SCL
    RCALL i2c_delay
    SBI I2C_PORTREG,I2C_PIN_SDA
    LDI R18,I2C_USISR_CLR
    OUT USISR,R18          ; Kuittaa lopetus
    RET
