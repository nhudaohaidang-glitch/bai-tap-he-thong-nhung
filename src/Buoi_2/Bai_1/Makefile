TARGET = stm32_pwm_uart

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = cortex-m3

CFLAGS = -mcpu=$(CPU) \
         -mthumb \
         -std=c11 \
         -O0 \
         -g3 \
         -Wall \
         -Wextra \
         -ffreestanding \
         -fno-builtin \
         -ffunction-sections \
         -fdata-sections

LDFLAGS = -T stm32f103c8.ld \
          -nostdlib \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map \
          -lgcc


SRC = main.c startup.c

OBJ = $(SRC:.c=.o)


all: $(TARGET).bin
	@echo "Build complete!"
	$(SIZE) $(TARGET).elf


%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@


$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@


flash: $(TARGET).bin
	st-flash --reset write $(TARGET).bin 0x08000000


clean:
	rm -f *.o *.elf *.bin *.map
