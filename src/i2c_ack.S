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
