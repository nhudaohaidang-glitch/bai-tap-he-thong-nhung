#include <stdint.h>

extern int main(void);

extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void Default_Handler(void) {
    while (1);
}

void Reset_Handler(void) {
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;

    while (dst < &_edata) {
        *dst++ = *src++;
    }

    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }

    main();
    while (1);
}

typedef void (*ISR_Handler)(void);

__attribute__((section(".isr_vector"), used))
const ISR_Handler vector_table[16] = {
    [0] = (ISR_Handler)&_estack,
    [1] = Reset_Handler,
    [2 ... 15] = Default_Handler
};
