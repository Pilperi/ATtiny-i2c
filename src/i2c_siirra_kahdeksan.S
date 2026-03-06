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

; Siirrä dataa TWI pitkin, suuntaan tai toiseen
; Sisääntulo (i2c_kirjoita):
; R24 : kirjoitettava data
;
; Palauttaa (i2c_lue):
; R24 : luettu data
;
;; Rekisterikäyttö:
; R18
i2c_lue:
    CBI DDRB,I2C_PIN_SDA
    RJMP i2c_siirra_kahdeksan
i2c_kirjoita:
    OUT USIDR,R24
    SBI DDRB,I2C_PIN_SDA
i2c_siirra_kahdeksan:
    LDI R18,I2C_USISR_CLR
    OUT USISR,R18             ; Laskuri nollaan
i2c_siirra_loop:
    SBI USICR,USITC
    RCALL i2c_delay
i2c_siirra_odota_scl:         ; Vastaanottaja saattaa pitää SCL alhaalla
    SBIS PINB,I2C_PIN_SCL     ; (ks. clock strecthing)
    RJMP i2c_siirra_odota_scl
    SBI USICR,USITC
    SBIS USISR,USIOIF
    RJMP i2c_siirra_loop
    IN R24,USIDR
    LDI R18,I2C_USISR_CLR
    OUT USISR,R18             ; Nollaa laskuri ja flagit
    RET
