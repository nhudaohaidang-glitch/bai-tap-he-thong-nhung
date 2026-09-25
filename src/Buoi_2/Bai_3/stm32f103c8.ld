ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 20K
}

SECTIONS
{
    /* Interrupt vector table */
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector))
        . = ALIGN(4);
    } > FLASH


    /* Program code */
    .text :
    {
        . = ALIGN(4);

        *(.text)
        *(.text.*)

        . = ALIGN(4);

        _etext = .;
    } > FLASH


    /* Read-only data */
    .rodata :
    {
        . = ALIGN(4);

        *(.rodata)
        *(.rodata.*)

        . = ALIGN(4);
    } > FLASH


    /* Initialized data */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);

        _sdata = .;

        *(.data)
        *(.data.*)

        . = ALIGN(4);

        _edata = .;
    } > RAM AT > FLASH


    /* Uninitialized data */
    .bss :
    {
        . = ALIGN(4);

        _sbss = .;

        *(.bss)
        *(.bss.*)
        *(COMMON)

        . = ALIGN(4);

        _ebss = .;
    } > RAM


    /* Remove unnecessary sections */
    /DISCARD/ :
    {
        *(.note*)
        *(.comment*)
    }
}
