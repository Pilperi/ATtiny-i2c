2026-02-26

# Yksinkertainen I2C-funktiokokoelma

Testimielessä kasaan simppelin työkalupakin I2C-kommunikaatioon ATtiny-arkkitehtuurille. Käyttää USI-väylää, joka on geneerinen sarjaliikennetoiminnallisuus ATtiny-sarjassa.

Samalla kattelen vähän, miten C ja assemblyn yhdistely toimii ihan oikeita juttuja tehdessä, eikä vain demomielessä kuten [toisessa hommassa](https://github.com/Pilperi/attiny85-c-asm) joka oli lähinnä aloitteleva testijuttu.

## Assemblyfunktioiden kutsuminen C-koodista

Kutsujen rekisterirakenne on selitetty aika nätisti auki [GCC wikissä](https://gcc.gnu.org/wiki/avr-gcc#Calling_Convention).

Lähinnä täytyy
1. Olla C-headeri jossa määritelty että jossain on sen ja sen mallinen funktio olemassa
2. Assemblyssä olla samanniminen funktio merkattuna `.global`
3. Kunnioittaa kutsu- ja paluuargumenttispeksiä assembly-toteutuksessa

Toisin sanottuna minimaalitoteutuksena C-koodissa
```C
/* i2c.h */
#include <stdint.h>
#include <avr/io.h>

#define I2C_PIN_SDA PINB0
#define I2C_PIN_SCL PINB2

/* Lähetä databufferi laitteelle

1. uint8_t* Bufferin osoite
2. uint8_t  Bufferin koko tavuina
3. uint8_t  Laiteosoite
4. uint8_t  Kirjoitusosoite laiteella
*/
void i2c_laheta_buffer(uint8_t*, uint8_t, uint8_t, uint8_t);
```
```C
/* main.c */
#include <stdint.h>
#include "i2c.h"

void main(void)
{
    uint8_t teksti[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
    uint8_t laiteosoite = 0xA0;
    uint8_t maara = 6;
    uint8_t muistiosoite = 0x00;
    i2c_laheta_buffer(teksti, maara, laiteosoite, muistiosoite);
}
```
ja assembly-koodissa
```Assembly
; i2c.S

#define __SFR_OFFSET 0
#include <avr/io.h>

#define I2C_PIN_SDA PINB0
#define I2C_PIN_SCL PINB2

.global i2c_laheta_buffer

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
```

Jekkuna tässä on se, että assembly-toteutuksessa voi kyllä tehdä `#include <avr/io.h>`, mutta jos yrittää `#include "i2c.h"` saadakseen sieltä `I2C_PIN_SDA`-määritelmät sun muut, tulee
```
src/i2c.h:20: Virhe: tuntematon käskykoodi ”void”
```
ja kasa muita virheitä siitä että C:n perussanastoa ei tunneta. En tiedä mikä homma, kääntäjä menee johonkin omaan assembly-tilaan ja sekoaa C-koodista, selvittelen ehkä myöhemmin.

Assemblyfunktion kutsuargumenttien rekisteripaikkojen lasku menee GCC wikin reseptillä
```
1. argumentti uint8_t*
Aloitetaan R = R26
uint8_t* on 2 tavua (muistiosoitteet 16 bit)
2 tavua parillinen, ei tehdä mitään
Vähennetään R26 - 2 (argumentin koko) = R24
R24 > R8 joten argumentti passataan rekistereissä, alkaen R24 eli R25:R24

2. argumentti uint8_t
R = R24 edellisestä
uint8_t on 1 tavu, pyöristetään ylös parilliseen ja viedään 2 tavua
R24 - 2 (pyöristetty koko) = R22
R22 > R8 joten passataan rekistereissä, R23:R22

jne
```

## Muuttujien muistiosoiteet

Muuttujien muistisijainneista ja data-avaruuksista paasaukset @ [muistiinpanot_data.md](muistiinpanot_data.md).

## I2C USI-väylällä

...
