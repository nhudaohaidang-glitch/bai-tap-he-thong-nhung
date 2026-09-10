CC = arm-none-eabi-gcc
# Khai báo CPU và chỉ điểm dùng linker.ld
CFLAGS = -mcpu=cortex-m3 -mthumb -nostartfiles -T linker.ld -I.

all: main.bin

# Đóng gói ra file bin
main.bin: main.elf
	arm-none-eabi-objcopy -O binary main.elf main.bin

# Biên dịch cả main.c và startup.s
main.elf: main.c startup.s
	$(CC) $(CFLAGS) main.c startup.s -o main.elf

# Lệnh nạp code
flash: main.bin
	st-flash write main.bin 0x8000000

clean:
	rm -f *.elf *.bin
