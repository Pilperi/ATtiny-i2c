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

; Lähetä I2C aloitussignaali viemällä data alas kellon ollessa ylhäällä
; Olettaa että DDRB on asetettu ennen kutsua.
; Rekisterikäyttö:
; R18
i2c_aloita_rep:
    SBI USIDR,7
    SBI PORTB,I2C_PIN_SDA
    SBI PORTB,I2C_PIN_SCL
    RCALL i2c_delay
i2c_aloita:
    LDI R18,I2C_MASK_SDA_SCL
    OUT DDRB,R18
    CBI PORTB,I2C_PIN_SDA
    RCALL i2c_delay
    CBI PORTB,I2C_PIN_SCL
    SBI PORTB,I2C_PIN_SDA ; SDA oletustila takaisin ylös
    LDI R18,I2C_USISR_CLR
    OUT USISR, R18        ; Kuittaa aloitus
    RET
