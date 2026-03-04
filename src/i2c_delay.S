#define __SFR_OFFSET 0
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

i2c_delay:
    NOP
    NOP
    NOP
    RET
