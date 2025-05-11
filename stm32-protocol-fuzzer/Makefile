TARGET = main
BUILD_DIR = build
SRC = src
INC = inc

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -nostdlib -Wall -Werror -O0 -g
CFLAGS += -I$(INC)
LDFLAGS = -Tlinker.ld

SOURCES = $(SRC)/main.c $(SRC)/uart.c $(SRC)/delay.c $(SRC)/fuzzer.c
OBJECTS = $(SOURCES:.c=.o)

all: $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/$(TARGET).elf: startup.o $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

startup.o: startup.s
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) *.o $(SRC)/*.o

flash: all
	st-flash write $(BUILD_DIR)/$(TARGET).bin 0x8000000
