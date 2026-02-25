#define __SFR_OFFSET 0
#include <avr/io.h>

.global testifunktio

.section .text

testifunktio:
    PUSH R24
    PUSH R25
    CP R24,R1
    CPC R25,R1
    BREQ testifunktio_loop_end
    LDI R18,1<<PINB1
testifunktio_loop:
    OUT PINB,R18
    OUT PINB,R18
    SBIW R24,1
    BREQ testifunktio_loop_end
    RJMP testifunktio_loop
testifunktio_loop_end:
    POP R25
    POP R24
    ADIW R24,1
    RET
