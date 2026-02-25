#define __SFR_OFFSET 0
#include <avr/io.h>

.global main
.global i2c_aloita
.global i2c_laheta
.global i2c_lue_ack
.global i2c_lopeta

.section .data
testiteksti: .asciz "kissa"

.section .text
siirra_data:
    LDI ZL,lo8(.testiteksti)
    LDI ZH,hi8(.testiteksti)
    LDI XL,lo8(RAMSTART)
    LDI XH,hi8(RAMSTART)
siirra_data_loop:
    LPM R18,Z+
    ST X+,R18
    CPI R18,0x00
    BRNE siirra_data_loop
    RET

main:
    RCALL siirra_data
    RCALL i2c_setup
    RCALL i2c_aloita
    LDI R18,0xA0
    OUT USIDR,R18
    RCALL i2c_laheta
    RCALL i2c_lue_ack
    RCALL i2c_lopeta
    CBI USISR,USIPF
main_loop:
    RJMP main_loop
    RET
