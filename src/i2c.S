#define __SFR_OFFSET 0
#include <avr/io.h>
#include "i2c.h"

.global i2c_setup
.global i2c_aloita
.global i2c_aloita_rep
.global i2c_lopeta
.global i2c_siirra_kahdeksan
.global i2c_ack

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
    CBI PORTB,I2C_PIN_SDA
    RCALL i2c_delay
    CBI PORTB,I2C_PIN_SCL
    SBI PORTB,I2C_PIN_SDA ; SDA oletustila takaisin ylös
    LDI R18,I2C_USISR_CLR
    OUT USISR, R18        ; Kuittaa aloitus
    RET

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

; Lue tai kirjoita ACK
; Sisääntulo:
; R24 : 0: kirjoita ACK, >0: lue ack
;
; Rekisterikäyttö:
; R18
i2c_ack:
    IN R18,DDRB
    PUSH R18
    CPSE R24,R1
    BREQ i2c_ack_lue
    ORI R18,I2C_MASK_SDA    ; Kirjoita ACK
    RJMP i2c_ack_kellota
i2c_ack_lue:
    ANDI R18,I2C_MASK_SDA_N ; Lue ACK
i2c_ack_kellota:
    OUT DDRB,R18
    OUT USIDR,R1
    SBI USICR,USITC
    RCALL i2c_delay
    SBI USICR,USITC
    POP R18                 ; Palauta DDRB lähtötilaan
    OUT DDRB,R18
    RET

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

i2c_delay:
    NOP
    NOP
    NOP
    RET
