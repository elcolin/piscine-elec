#Files
SRC = main.c

#Variable
F_CPU = 1600000UL

#Compiler
CC = avr-gcc
CFLAGS +=  -DF_CPU=$(F_CPU) -mmcu=atmega328p -Os
BIN = $(SRC:%.c=%.bin)


#Bin to hex
OBJCOPY = avr-objcopy
OFLAGS= -O ihex -R .eeprom
HEX= $(BIN:%.bin=%.hex)

#Hex to controller
DUDE = avrdude
# DFLAGS= -c arduino -p atmega328p -b 115200 -P /dev/tty.usbserial-10 -Uflash:w:
DFLAGS= -c arduino -p atmega328p -b 115200 -P /dev/ttyUSB0 -Uflash:w:

all: hex

hex: $(HEX)

%.bin: %.c
	$(CC) $(CFLAGS) $< -o $@

%.hex: %.bin
	$(OBJCOPY) $(OFLAGS) $< $@

flash: $(HEX)
	$(DUDE) $(DFLAGS)$<:i

clean:
	rm -rf $(BIN)
	rm -rf $(HEX)

re: clean flash

.PRECIOUS: %.bin

.PHONY: flash hex