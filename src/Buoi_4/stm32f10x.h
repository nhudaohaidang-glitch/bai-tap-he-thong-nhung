#ifndef STM32F10X_H
#define STM32F10X_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB1RSTR;
    volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t APB1ENR;
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

// Thanh ghi cấu hình bộ đếm SysTick của lõi ARM Cortex-M
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;

#define RCC_BASE        0x40021000UL
#define GPIOA_BASE      0x40010800UL
#define SysTick_BASE    0xE000E010UL

#define RCC             ((RCC_TypeDef *)RCC_BASE)
#define GPIOA           ((GPIO_TypeDef *)GPIOA_BASE)
#define SysTick         ((SysTick_TypeDef *)SysTick_BASE)

#endif
