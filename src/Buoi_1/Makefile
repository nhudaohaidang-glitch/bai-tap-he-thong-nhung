CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m3 -mthumb -O2 -Wall -g
LDFLAGS = -T linker.ld -nostdlib

# Tự động gộp TẤT CẢ file .c trong thư mục và file startup.s
SOURCES = $(wildcard *.c) startup.s

all: main.bin

main.elf: $(SOURCES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

main.bin: main.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f *.elf *.bin

flash: main.bin
	st-flash write main.bin 0x08000000
