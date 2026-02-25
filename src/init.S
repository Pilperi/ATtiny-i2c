#define __SFR_OFFSET 0
#include <avr/io.h>

.global init_data
.section .text.init

init_data:
init_data_sec_rodata:                 ; Read only data (const etc)
    LDI ZL,lo8(pmem_rodata_start_la)  ; Lukuosoite (LPM vaatii Z)
    LDI ZH,hi8(pmem_rodata_start_la)
    LDI YL,lo8(sram_rodata_start)     ; Kirjoitusosoite
    LDI YH,hi8(sram_rodata_start)
    LDI XL,lo8(sram_rodata_end)       ; Kirjoituksen lopetusosoite
    LDI XH,hi8(sram_rodata_end)
    RCALL init_siirra_data
init_data_sec_data:                   ; RW data (arvoilla alustettu)
    LDI ZL,lo8(pmem_data_start_la)    ; Lukuosoite
    LDI ZH,hi8(pmem_data_start_la)
    LDI YL,lo8(sram_data_start)       ; Kirjoitusosoite
    LDI YH,hi8(sram_data_start)
    LDI XL,lo8(sram_data_end)         ; Kirjoituksen lopetusosoite
    LDI XH,hi8(sram_data_end)
    RCALL init_siirra_data
init_data_sec_bss:                    ; Alustamaton (nollalla alustettu) data
    LDI YL,lo8(sram_bss_start)        ; Kirjoitusosoite
    LDI YH,hi8(sram_bss_start)
    LDI XL,lo8(sram_bss_end)          ; Kirjoituksen lopetusosoite
    LDI XH,hi8(sram_bss_end)
    RJMP init_bss_fill_loop_check
init_bss_fill_loop:
    ST Y+,R1                          ; Kirjoita SRAM 0x00
init_bss_fill_loop_check:
    CP YL,XL
    CPC YH,XH
    BRNE init_bss_fill_loop
    RET                               ; init_data END

; Yleispätevä datan siirtofunktio ohjelmamuistista datamuistiin
init_siirra_data:
    RJMP init_siirra_data_loop_check
init_siirra_data_loop:
    LPM R18,Z+                        ; Lue FLASH Z:sta
    ST Y+,R18                         ; Kirjoita SRAM Y:hyn
init_siirra_data_loop_check:          ; Jatka kunnes Y==X
    CP YL,XL
    CPC YH,XH
    BRNE init_siirra_data_loop
    RET
