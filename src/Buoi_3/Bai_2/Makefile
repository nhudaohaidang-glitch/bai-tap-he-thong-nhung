# Khai báo công cụ biên dịch
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

# Tên file đầu ra (bạn có thể đặt tên bất kỳ, ở đây đặt là main)
TARGET = main

# Danh sách các file mã nguồn CẦN DỊCH
SRC = main.c startup.c MAX7219.c

# File cấu hình Linker
LDSCRIPT = stm32f103c8.ld

# Cấu hình cờ biên dịch (Chip Cortex-M3, không dùng thư viện chuẩn C)
CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g3 -Wall -T $(LDSCRIPT) -nostdlib

# Luật biên dịch mặc định (gõ "make")
all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

# Luật nạp code vào vi điều khiển (gõ "make flash")
flash: $(TARGET).bin
	st-flash write $(TARGET).bin 0x08000000

# Luật dọn dẹp file rác (gõ "make clean")
clean:
	rm -f *.elf *.bin *.o
