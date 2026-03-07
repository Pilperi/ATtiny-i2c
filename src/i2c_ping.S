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
.global i2c_ping

.section .text

; Kutsu laitetta ja katso vastaako se.
; Sisääntulo:
; R24 : laitteen osoite
;
; Palauttaa:
; R24 : Onko laite vastaamatta:
;       0 jos laite vastaa
;      >0 jos laite ei vastaa
;
i2c_ping:
    RCALL i2c_aloita
    RCALL i2c_kirjoita
    RCALL i2c_ack
    RCALL i2c_lopeta
    RET
