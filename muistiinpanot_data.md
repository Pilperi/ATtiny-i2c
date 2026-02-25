2026-02-25

# Nootteja datasegmenteistä bufferiargumenteista

Tässä viime aikoina yhdistellyt C:tä ja assemblyä AVR-kontekstissa, ja nyt törmäsin ongelmiin siinä miten passaan dataa C-puolelta assembly-puolelle.

## AVR-arkkitehtuurin muistirakenne

AVR käyttää Harvard-pohjaista suoritinarkkitehtuuria, eli suoritettava koodi ja data elää eri paikoissa. Peruskuvio on esitetty alla olevassa kuvassa.

<img src="img/attiny_muistirakenne.png" width="700"></img>

Ohjelmamuisti (FLASH) on yksi iso (8 kt) muistijatkumo, ja datamuisti (SRAM) on pienempi (512 tavua käyttökelpoista tilaa). Virtojen mennessä päälle suoritin alkaa lukea ohjeita ohjelmamuistin alusta järjestyksessä. Ekana ohjeena on yleensä hyppy alustusrutiineihin ja muut interruptitoimenpiteet.

SRAM-puolen alussa on ALU:n käyttämät työrekisterit `R0..R31` ja niiden jälkeen muistimäpätty IO, esim. porttisuunnat määrittävä `DDRB` SRAM-osoitteessa `0x0017`, kellon arvorekisteri `TCNT0`  osoitteessa `0x0032` ja samaiseen SRAM:iin osoittava stack pointer osoitteissa `0x003E:0x003D`. Muistimäpätyn IO jälkeen alkaa vapaasti käytettävä dataosio, jota stack pointer yleensä syö sen yläpäästä. Mitään hyvin määriteltyjä heap-mekaniikkoja jotka söisi muistia alhaalta päin ei (kai) ole.

Sekä ohjelmamuistin että datamuistin osoitteet on molemmat luku- ja kirjoituskelpoisia, ja ne on eroteltu eri ohjeiden alle.

Ohjelmamuisti:
| Ohje | Nimi                 | Toiminto                                                         |
|------|----------------------|------------------------------------------------------------------|
| LPM  | Load Program Memory  | Ohjelmamuistin arvo rekisteriin, osoite Z-rekisterissä `R31:R30` |
| SPM  | Store Program Memory | Rekisterin arvo ohjelmamuistiin, osoite Z-rekisterissä           |

Datamuisti:
| Ohje | Nimi                 | Toiminto                                                         |
|------|----------------------|------------------------------------------------------------------|
| LD   | Load Indirect        | Lue datamuistin arvo rekisteriin, osoite X-/Y-/Z-rekisterissä    |
| ST   | Store Indirect       | Rekisterin arvo datamuistiin, käy X,Y tai Z pointteriksi         |
| LDS  | Load Direct          | Lue kiinteän osoitteen data rekisteriin                          |
| STS  | Store Direct         | Kirjoita kiinteän osoitteen data rekisteriin                     |
| IN   | Load IO to register  | Lue muistimäpätyn IO:n arvo rekisteriin                          |
| OUT  | Store register to IO | Kirjoita rekisterin arvo muistimäpättyyn IO                      |

SRAM:in IO-muistialueelle `0x0020..0x005F` voi siis operoida sekä ohjeilla `IN/OUT` että `LD/ST` että `LDS/STS`, mutta `IN/OUT` on kellosyklin verran nopeampia kuin vastaavat ST-ohjeet.

Huomionarvoista on että ohjelmamuistiin voi operoida vain Z-pointteria (`R31:R30`) pitkin.
