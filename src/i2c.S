#define __SFR_OFFSET 0
#include <avr/io.h>
#include "i2c.h"

.global i2c_laheta_buffer

.section .text

; Lähetä SRAM-bufferista dataa
; Muistiosoite R25:R24
; Tavujen määrä R22
; Laiteosoite R20
i2c_laheta_buffer:
    CPI R22,0
    BREQ i2c_laheta_buffer_valmis
    ANDI R20,~(1<<0)
    RCALL i2c_setup
    RCALL i2c_aloita
    SBI PORTB,I2C_PIN_SDA ; Oletuksena ylhäällä
    ; Kirjoitusmoodi:
    IN R18,DDRB
    ORI R18,(1<<I2C_PIN_SCL)|(1<<I2C_PIN_SDA)
    OUT DDRB,R18
    ; Lähetä laiteosoite
    OUT USIDR,R20
    RCALL i2c_siirra_kahdeksan
    RCALL i2c_lue_ack
    SBI DDRB,I2C_PIN_SDA ; Datalinja takaisin haltuun ACK jälkeen
    ; Aloita bufferin lukeminen ja lähettäminen
    MOV XL,R24
    MOV XH,R25
i2c_laheta_buffer_loop:
    LD R18, X+
    OUT USIDR,R18
    RCALL i2c_siirra_kahdeksan
    RCALL i2c_lue_ack
    SBI DDRB,I2C_PIN_SDA
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
    IN R19,DDRB
    ORI R19,(1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL)
    ORI R18,(1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL)
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
    OUT USISR, R1
    RET

; Lähetä I2C lopetussignaali viemällä data ylös kellon ollessa ylhäällä
; Rekisterikäyttö:
; R18
i2c_lopeta:
    CBI PORTB,I2C_PIN_SCL
    CBI PORTB,I2C_PIN_SDA
    IN R18,DDRB
    ORI R18,(1<<I2C_PIN_SDA)|(1<<I2C_PIN_SCL)
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
; R19 paluuarvo (ACK signaali)
i2c_lue_ack:
    IN R18,DDRB
    CBI DDRB,I2C_PIN_SDA
    OUT USISR,R1
    SBI USICR,USITC
    NOP
    IN R19,PINB
    ANDI R19,(1<<I2C_PIN_SDA)
    SBI USICR,USITC
    OUT DDRB,R18
    OUT USISR,R1
    RET

i2c_kirjoita_ack:
    IN R18,DDRB
    SBI DDRB,I2C_PIN_SDA
    IN R19,PORTB
    CBI PORTB,I2C_PIN_SDA
    OUT USISR,R1
    SBI USICR,USITC
    NOP
    NOP
    SBI USICR,USITC
    OUT DDRB,R18
    OUT PORTB,R19
    OUT USISR,R1
    RET


; Siirrä dataa TWI pitkin, suuntaan tai toiseen
; Rekisterikäyttö:
; R18 : jos nolla, kirjoitetaan, muutoin luetaan
; R19
i2c_siirra_kahdeksan:
    IN R19,USISR
    ANDI R19,0xF0
    OUT USISR,R19 ; Laskuri nollaan
i2c_siirra_loop:
    SBI USICR,USITC
    NOP
    NOP
    NOP
    NOP
    SBI USICR,USITC
    SBIS USISR,USIOIF
    RJMP i2c_siirra_loop
    CBI DDRB,I2C_PIN_SDA       ; Vapauta SDA heti
    OUT USISR,R1               ; Nollaa laskuri ja flagit
    RET
