#ifndef STM32F10X_H
#define STM32F10X_H

#include <stdint.h>

// 1. Nhóm thanh ghi RCC
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR; // Bit 2: Port A, Bit 4: Port C
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

// 2. Nhóm thanh ghi GPIO
typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

// 3. Nhóm thanh ghi SysTick Timer
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;

// 4. Ép kiểu địa chỉ vật lý thành Con trỏ Cấu trúc
#define RCC       ((RCC_TypeDef *) 0x40021000)
#define GPIOA     ((GPIO_TypeDef *) 0x40010800)
#define GPIOC     ((GPIO_TypeDef *) 0x40011000)
#define SysTick   ((SysTick_TypeDef *) 0xE000E010)

#endif

