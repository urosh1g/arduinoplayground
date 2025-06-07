CC = avr-gcc
OBJCOPY = avr-objcopy

# NOTE TO READER: find your device
# 1. Unplug device
# 2. $ ls /dev/tty*
# 3. Plug device
# 4. $ ls /dev/tty* -> the one not present in step 2.
DEVICE = /dev/ttyUSB0

MCU = atmega328p
F_CPU = 16000000UL

INCLUDE_DIR = include
SRC_DIR = src
CFLAGS = -I$(INCLUDE_DIR) -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -Wall -Wextra -Werror -std=gnu99 

TARGET = main
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/dht11.c

all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

clean:
	rm -f *.o *.elf *.hex

flash:
	avrdude -v -patmega328p -carduino -P $(DEVICE) -b115200 -D -Uflash:w:$(TARGET).hex:i

