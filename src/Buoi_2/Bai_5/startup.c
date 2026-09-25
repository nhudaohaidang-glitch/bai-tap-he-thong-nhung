#include <stdint.h>

extern int main(void);
extern void USART1_IRQHandler(void);

/* Symbols from linker script */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;


/* Default interrupt handler */
void Default_Handler(void)
{
    while (1)
    {
    }
}


/* Reset Handler */
void Reset_Handler(void)
{
    uint32_t *src;
    uint32_t *dst;

    /* Copy .data from Flash to RAM */
    src = &_sidata;
    dst = &_sdata;

    while (dst < &_edata)
    {
        *dst++ = *src++;
    }


    /* Clear .bss */
    dst = &_sbss;

    while (dst < &_ebss)
    {
        *dst++ = 0;
    }


    /* Run main program */
    main();


    while (1)
    {
    }
}


/*
 * Vector table
 *
 * USART1 IRQ number = 37
 * Vector index = 16 + 37 = 53
 */

typedef void (*ISR_Handler)(void);

__attribute__((section(".isr_vector"), used))
const ISR_Handler vector_table[54] =
{
    [0] = (ISR_Handler)&_estack,
    [1] = Reset_Handler,

    [2 ... 52] = Default_Handler,

    [53] = USART1_IRQHandler
};
