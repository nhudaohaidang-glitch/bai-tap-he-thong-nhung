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

/* =========================================================================
   BỔ SUNG ĐỊNH NGHĨA CHO GPIOB VÀ TIM2
   ========================================================================= */

// 1. Định nghĩa địa chỉ cho GPIOB (Tái sử dụng GPIO_TypeDef đã có sẵn)
#define GPIOB_BASE            0x40010C00
#define GPIOB                 ((GPIO_TypeDef *) GPIOB_BASE)

// 2. Khai báo cấu trúc thanh ghi phần cứng cho các bộ Timer (TIM)
typedef struct {
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

// 3. Định nghĩa địa chỉ cơ sở cho TIM2
#define TIM2_BASE             0x40000000
#define TIM2                  ((TIM_TypeDef *) TIM2_BASE)

/* =========================================================================
   BỔ SUNG ĐỊNH NGHĨA CHO BÀI TẬP UART VÀ NGẮT NGOÀI
   ========================================================================= */

// 1. Định nghĩa cho khối USART
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;
#define USART1_BASE           0x40013800
#define USART1                ((USART_TypeDef *) USART1_BASE)

// 2. Định nghĩa cho khối AFIO (Alternate Function I/O - Dùng để map ngắt)
typedef struct {
    volatile uint32_t EVCR;
    volatile uint32_t MAPR;
    volatile uint32_t EXTICR[4];
    volatile uint32_t MAPR2;
} AFIO_TypeDef;
#define AFIO_BASE             0x40010000
#define AFIO                  ((AFIO_TypeDef *) AFIO_BASE)

// 3. Định nghĩa cho khối EXTI (External Interrupt - Ngắt ngoài)
typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;
#define EXTI_BASE             0x40010400
#define EXTI                  ((EXTI_TypeDef *) EXTI_BASE)

// 4. Định nghĩa cho khối NVIC (Trình điều khiển ngắt trung tâm của lõi ARM)
typedef struct {
    volatile uint32_t ISER[8];
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];
    uint32_t RSERVED1[24];
    volatile uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];
    uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];
} NVIC_Type;
#define NVIC_BASE             0xE000E100
#define NVIC                  ((NVIC_Type *) NVIC_BASE)

#endif
