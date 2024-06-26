
BIN = main.bin
HEX = main.hex

CC = avr-gcc

CPU_SPEED = 16000000UL
MICROCONTROLLER = atmega328p

#CFLAGS = -Wall -Wextra -Werror

BAUDRATE = 115200
PROGRAMMER = arduino

DEVICE = /dev/tty.usbserial-110 #/dev/ttyUSB0 

SRCS = main.c \
		port.c \

OBJS = $(addprefix objs/,$(SRCS:.c=.o))
OBJ_DIRS = $(sort $(dir $(OBJS)))

all: flash

flash: $(HEX)
	avrdude -v -c $(PROGRAMMER) -p $(MICROCONTROLLER) -P $(DEVICE) -b $(BAUDRATE) -D -U flash:w:$(HEX)

hex: $(HEX)

objs/%.o : %.c | $(OBJ_DIRS)
	$(CC) -Os -DF_CPU=$(CPU_SPEED) -mmcu=$(MICROCONTROLLER) $(CFLAGS) -I. -c $< -o $@

$(BIN) : $(OBJS)
	$(CC) -Os -DF_CPU=$(CPU_SPEED) -mmcu=$(MICROCONTROLLER) $(CFLAGS) $(OBJS) -o main.bin

$(HEX): $(BIN)
	avr-objcopy -R .eeprom -O ihex $(BIN) $(HEX)

$(OBJ_DIRS):
	mkdir -p $@

clean:
	rm -rf objs $(HEX) $(BIN)

re: clean all
.PHONY: all hex flash clean re
