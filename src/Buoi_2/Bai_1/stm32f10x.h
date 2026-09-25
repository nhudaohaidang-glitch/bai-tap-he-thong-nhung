#ifndef STM32F10X_H
#define STM32F10X_H

#include <stdint.h>


/* =========================================================
 * RCC
 * ========================================================= */

typedef struct
{
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


/* =========================================================
 * GPIO
 * ========================================================= */

typedef struct
{
    volatile uint32_t CRL;
    volatile uint32_t CRH;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;


/* =========================================================
 * USART
 * ========================================================= */

typedef struct
{
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;


/* =========================================================
 * TIMER
 * ========================================================= */

typedef struct
{
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM_TypeDef;


/* =========================================================
 * NVIC
 * ========================================================= */

typedef struct
{
    volatile uint32_t ISER[8];

    uint32_t RESERVED0[24];

    volatile uint32_t ICER[8];

    uint32_t RESERVED1[24];

    volatile uint32_t ISPR[8];

    uint32_t RESERVED2[24];

    volatile uint32_t ICPR[8];

    uint32_t RESERVED3[24];

    volatile uint32_t IABR[8];

    uint32_t RESERVED4[56];

    volatile uint8_t IP[240];

} NVIC_TypeDef;


/* =========================================================
 * PERIPHERAL BASE ADDRESSES
 * ========================================================= */

#define RCC_BASE        0x40021000UL
#define GPIOA_BASE      0x40010800UL
#define USART1_BASE     0x40013800UL
#define TIM2_BASE       0x40000000UL
#define NVIC_BASE       0xE000E100UL


/* =========================================================
 * PERIPHERAL POINTERS
 * ========================================================= */

#define RCC     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)

#define USART1  ((USART_TypeDef *)USART1_BASE)

#define TIM2    ((TIM_TypeDef *)TIM2_BASE)

#define NVIC    ((NVIC_TypeDef *)NVIC_BASE)


/* =========================================================
 * INTERRUPT NUMBER
 * ========================================================= */

#define USART1_IRQn 37


/* =========================================================
 * ENABLE NVIC INTERRUPT
 * ========================================================= */

static inline void NVIC_EnableIRQ(int IRQn)
{
    NVIC->ISER[IRQn / 32] |=
        (1UL << (IRQn % 32));
}


/* =========================================================
 * GLOBAL INTERRUPT CONTROL
 * ========================================================= */

static inline void __disable_irq(void)
{
    __asm volatile ("cpsid i");
}


static inline void __enable_irq(void)
{
    __asm volatile ("cpsie i");
}


/* =========================================================
 * USART1 INTERRUPT HANDLER
 * ========================================================= */

void USART1_IRQHandler(void);


#endif
