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

; Lue tai kirjoita ACK
; Sisääntulo:
; R24 : 0: kirjoita ACK, >0: lue ack
;
; Palauttaa:
; R24 : 0 jos ACK, >0 jos NACK
; Rekisterikäyttö:
; R18
i2c_ack:
    OUT USIDR,R1
    IN R18,DDRB
    PUSH R18
    CPSE R24,R1
    RJMP i2c_ack_lue        ; R24 != 0, luetaan
    ORI R18,I2C_MASK_SDA    ; R24 == 0, kirjoitetaan
    RJMP i2c_ack_kellota
i2c_ack_lue:
    ANDI R18,I2C_MASK_SDA_N ; Lue ACK
    SBI USIDR,7             ; MSB 1, vastaus kirjoittaa sen alas
i2c_ack_kellota:
    OUT DDRB,R18
    LDI R18,0xFF
    OUT USISR,R18
    SBI USICR,USITC
i2c_ack_odota_scl:
    SBIS PINB,I2C_PIN_SCL
    RJMP i2c_ack_odota_scl  ; Odota vaihtumista (ks. clock streching)
    RCALL i2c_delay
    SBI USICR,USITC
    IN R24,USIBR
    POP R18                 ; Palauta DDRB lähtötilaan
    OUT DDRB,R18
    LDI R18,I2C_USISR_CLR   ; Nollaa overflow
    OUT USISR,R18
    RET
