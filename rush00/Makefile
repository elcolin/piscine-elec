# **************************************************************************** #
#       TITLE                                                                  #
# **************************************************************************** #
BIN = main.bin
HEX = main.hex

# **************************************************************************** #
#       COMMANDS                                                               #
# **************************************************************************** #
CC = avr-gcc

# **************************************************************************** #
#       FLAGS                                                                  #
# **************************************************************************** #

CPU_SPEED = 16000000UL
MICROCONTROLLER = atmega328p

#CFLAGS = -Wall -Wextra -Werror

BAUDRATE = 115200
PROGRAMMER = arduino

DEVICE = /dev/ttyUSB0

# **************************************************************************** #
#       SOURCES                                                                #
# **************************************************************************** #
SRCS = main.c \
		uart.c \
		i2c.c \
		leds.c \
		whoiswho.c \
		game.c

# **************************************************************************** #
#       RULES                                                                  #
# **************************************************************************** #
OBJS = $(addprefix objs/,$(SRCS:.c=.o))
OBJ_DIRS = $(sort $(dir $(OBJS)))

flash: $(HEX)
	avrdude -v -c $(PROGRAMMER) -p $(MICROCONTROLLER) -P $(DEVICE) -b $(BAUDRATE) -D -U flash:w:$(HEX)

all: flash

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

# **************************************************************************** #
#       PHONY                                                                  #
# **************************************************************************** #
.PHONY: all hex flash clean re
