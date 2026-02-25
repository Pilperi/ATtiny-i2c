#define __SFR_OFFSET 0
#include <avr/io.h>

.global siirra_data
.global testiteksti

.section .data
testiteksti: .asciz "kissa"

.section .text
siirra_data:
    LDI ZL,lo8(testiteksti)
    LDI ZH,hi8(testiteksti)
    LDI XL,lo8(RAMSTART)
    LDI XH,hi8(RAMSTART)
siirra_data_loop:
    LPM R18,Z+
    ST X+,R18
    CPI R18,0x00
    BRNE siirra_data_loop
    RET
