# 2026-02-21
#
# Makefile i2c-testikoodin kääntämiseen kirjastoksi (lib/libattiny_i2c.a)
#
# Katso Makefile_demo (make -f Makefile_demo) esimerkiksi siitä miten kutsua tulosta.


#########################################################################
#
# Perusmääritykset: kohdekansiot, käännösflagit ymv
#
#########################################################################
SHELL=/bin/bash

# Kohteen suoritinarkkitehtuuri
MMCU=attiny85
TARGET_ARCH=avr2

# Lähdekoodikansio
KOODIKANSIO=src

# Käännöksen tulokset
KOHDEKANSIO=lib
KOHDE_LIB=$(KOHDEKANSIO)/libattiny_i2c.a

# Assemblyn kääntäjä ja sen vaatimat argumentit (suorittimen tyyppi ymv)
COMP_AS=avr-gcc
COMPFLAGS_AS=-mmcu=$(MMCU) -Os -c -I lib -shared
# C-kääntäjä ja sen vaatimat argumentit
COMP_CC=avr-gcc
COMPFLAGS_C=-mmcu=$(MMCU) -Os -c -I lib -shared
# Arkistoija ja sen flagit
ARCHIVER=avr-ar
ARCHFLAGS=rcs

# Lista kaikesta C-lähdekoodista
C_SOURCES := $(shell find $(KOODIKANSIO) -name '*.c')
C_FILENAMES := $(notdir $(C_SOURCES))
# Lista assembly-lähdekoodista
S_SOURCES := $(shell find $(KOODIKANSIO) -name '*.S')
S_FILENAMES := $(notdir $(S_SOURCES))
# Käännetyt versiot
# C:stä käännetyt .c.o ja assemblystä .S.o ja nämä uudelleennimetään myöhemmin sitten .o
C_OBJECTS := $(addprefix $(KOHDEKANSIO)/,$(C_FILENAMES:%.c=%.c.o))
S_OBJECTS := $(addprefix $(KOHDEKANSIO)/,$(S_FILENAMES:%.S=%.S.o))

# Mitkä tulokset otetaan C-koodista ja mitkä assemblystä
O_OBJECTS_FROM_C =
O_OBJECTS_FROM_S = $(KOHDEKANSIO)/i2c_setup.o $(KOHDEKANSIO)/i2c_aloita.o $(KOHDEKANSIO)/i2c_lopeta.o $(KOHDEKANSIO)/i2c_ack.o $(KOHDEKANSIO)/i2c_siirra_kahdeksan.o $(KOHDEKANSIO)/i2c_delay.o $(KOHDEKANSIO)/i2c_ping.o
LIB_HEADER = $(KOHDEKANSIO)/attiny_i2c.h

#########################################################################
.PHONY: lib_out

lib_out: $(KOHDEKANSIO) clean $(KOHDE_LIB)

#########################################################################
#
# Kohdetiedostojen hallinta: putsaus- ja printtioperaatiot ymv
#
#########################################################################

# Build-kansion luonti jos uupuu
$(KOHDEKANSIO):
	mkdir $(KOHDEKANSIO)

# Poista väliaikatiedostot .o ja .elf
clean:
ifneq ("$(wildcard $(KOHDEKANSIO)/*.o)","")
	rm $(KOHDEKANSIO)/*.o
endif

# Paketoi .o-tiedostot libra-arkistoksi.
$(KOHDE_LIB): $(O_OBJECTS_FROM_C) $(O_OBJECTS_FROM_S)
	@echo Archive
	@echo O_OBJECTS_FROM_C $(O_OBJECTS_FROM_C)
	@echo O_OBJECTS_FROM_S $(O_OBJECTS_FROM_S)
	$(ARCHIVER) $(ARCHFLAGS) $(KOHDE_LIB) $(O_OBJECTS_FROM_C) $(O_OBJECTS_FROM_S)

$(O_OBJECTS_FROM_C): $(C_OBJECTS)
	@echo 
	@echo C_OBJECTS $(C_OBJECTS)
	test "$@" != "" && cp $(patsubst %.o,%.c.o,$@) $@
$(O_OBJECTS_FROM_S): $(S_OBJECTS)
	@echo 
	@echo S_OBJECTS $(S_OBJECTS)
	test "$@" != "" && cp $(patsubst %.o,%.S.o,$@) $@

# Käännä kaikki .c-tiedostot .c.o-tiedostoiksi
$(C_OBJECTS): $(C_SOURCES) $(LIB_HEADER)
	$(COMP_CC) $(COMPFLAGS_C) $(C_COMPILER_TARGET_FLAGS) -o $@ $(addprefix $(KOODIKANSIO)/,$(notdir $(patsubst %.c.o,%.c,$@)))
# Käännä kaikki .S-tiedostot .S.o-tiedostoiksi
$(S_OBJECTS): $(S_SOURCES) $(LIB_HEADER)
	$(COMP_AS) $(COMPFLAGS_AS) -o $@ $(addprefix $(KOODIKANSIO)/,$(notdir $(patsubst %.S.o,%.S,$@)))
