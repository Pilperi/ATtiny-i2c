#define __SFR_OFFSET 0
#include <avr/io.h>

#define I2C_PIN_SDA PINB0
#define I2C_PIN_SCL PINB2

.global i2c_laheta_buffer
.global i2c_laheta
.global i2c_lue

.global i2c_setup
.global i2c_kello_kayntiin
.global i2c_aloita
.global i2c_lopeta
.global i2c_lue_ack
.global i2c_siirra

.section .text

; Lähetä SRAM-bufferista dataa
; Muistiosoite R25:R24
; Tavujen määrä R22
; Laiteosoite R20
; Laitteen muistiosoite R18
i2c_laheta_buffer:
    CPI R22,0
    BREQ i2c_laheta_buffer_valmis
    ANDI R20,~(1<<0)
    PUSH R18
    RCALL i2c_setup
    RCALL i2c_aloita
    OUT USIDR,R20
    RCALL i2c_laheta
    RCALL i2c_lue_ack
    POP R18
    OUT USIDR,R18
    RCALL i2c_laheta
    RCALL i2c_lue_ack
    MOV XL,R24
    MOV XH,R25
i2c_laheta_buffer_loop:
    LD R19, X+
    OUT USIDR,R19
    RCALL i2c_laheta
    RCALL i2c_lue_ack
    DEC R22
    BREQ i2c_laheta_buffer_loop_valmis
    RJMP i2c_laheta_buffer_loop
i2c_laheta_buffer_loop_valmis:
    RCALL i2c_lopeta
i2c_laheta_buffer_valmis:
    RET

; Alusta I2C-väylä
; Rekisterikäyttö:
; R18, R19
i2c_setup:
    IN R18,PORTB
    LDI R19,(1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL)
    OR R18,R19
    OUT PORTB,R18
    OUT USIDR,R1
    OUT DDRB,R19
    LDI R18,(1<<USIWM1)|(1<<USICS1)|(1<<USICLK)
    OUT USICR,R18
    RET

; Lähetä I2C aloitussignaali viemällä data alas kellon ollessa ylhäällä
; Rekisterikäyttö:
; -
i2c_aloita:
    CBI PORTB,I2C_PIN_SDA
i2c_aloita_odota_sda:
    SBIC PINB,I2C_PIN_SDA
    RJMP i2c_aloita_odota_sda
    CBI PORTB,I2C_PIN_SCL
i2c_aloita_odota_scl:
    SBIC PINB,I2C_PIN_SCL
    RJMP i2c_aloita_odota_scl
    RET

; Lähetä I2C lopetussignaali viemällä data ylös kellon ollessa ylhäällä
; Rekisterikäyttö:
; R18
i2c_lopeta:
    CBI PORTB,I2C_PIN_SCL
    CBI PORTB,I2C_PIN_SDA
    LDI R18,(1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL)
    OUT DDRB,R18
    SBI PORTB,I2C_PIN_SCL
i2c_lopeta_odota_scl:
    SBIS PINB,I2C_PIN_SCL
    RJMP i2c_lopeta_odota_scl
    SBI PORTB,I2C_PIN_SDA
i2c_lopeta_odota_sda:
    SBIS PINB,I2C_PIN_SDA
    RJMP i2c_lopeta_odota_sda
    CBI DDRB,I2C_PIN_SCL
    CBI DDRB,I2C_PIN_SDA
    RET

; Lue ACK
; Rekisterikäyttö:
; R18
i2c_lue_ack:
    IN R18,DDRB
    CBI DDRB,I2C_PIN_SDA
    OUT USISR,R1
    SBI USICR,USITC
    RCALL i2c_delay
    SBI USICR,USITC
    OUT DDRB,R18
    OUT USISR,R1
    RET

; Siirrä kahdeksan bittiä, suuntaan tai toiseen
; Rekisterikäyttö:
; -
i2c_siirra_kahdeksan:
    SBI USICR,USITC
    RCALL i2c_delay
    SBI USICR,USITC
    SBIS USISR,USIOIF
    RJMP i2c_siirra_kahdeksan
    CBI DDRB,I2C_PIN_SDA ; Vapauta SDA heti
    OUT USISR,R1
    RET

; Lähetä USIDR arvot väylään
; Rekisterikäyttö:
; R18
i2c_laheta:
    SBI PORTB,I2C_PIN_SDA
    IN R18,DDRB
    ORI R18,(1<<I2C_PIN_SCL)|(1<<I2C_PIN_SDA)
    OUT DDRB,R18
    IN R18,USISR        ; Putsaa flagit, laskuri nollaan
    ANDI R18,0xF0
    OUT USISR,R18
    RCALL i2c_siirra_kahdeksan
    RET

; Lue data väylästä
; Rekisterikäyttö:
; R18
i2c_lue:
    IN R18,PORTB
    ORI R18,(1<<I2C_PIN_SDA)
    OUT PORTB,R18
    IN R18,DDRB
    ORI R18,(1<<I2C_PIN_SCL)
    ANDI R18,~(1<<I2C_PIN_SDA)
    OUT DDRB,R18
    IN R18,USISR        ; Putsaa flagit, laskuri ykköseen
    ANDI R18,0xF0
    OUT USISR,R18
    IN R18,USIDR
    RCALL i2c_siirra_kahdeksan
    RET

i2c_delay:
    NOP
    NOP
    NOP
    RET