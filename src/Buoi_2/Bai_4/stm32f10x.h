#ifndef STM32F10X_H
#define STM32F10X_H
#include <stdint.h>

// --- BẢN ĐỒ BỘ NHỚ GỐC ---
#define PERIPH_BASE       0x40000000UL
#define APB1PERIPH_BASE   PERIPH_BASE
#define APB2PERIPH_BASE   (PERIPH_BASE + 0x10000UL)
#define AHBPERIPH_BASE    (PERIPH_BASE + 0x20000UL)

// --- 1. CẤU TRÚC THANH GHI CÁC NGOẠI VI ---
typedef struct {
    volatile uint32_t CR;   volatile uint32_t CFGR; volatile uint32_t CIR;
    volatile uint32_t APB2RSTR; volatile uint32_t APB1RSTR; volatile uint32_t AHBENR;
    volatile uint32_t APB2ENR;  volatile uint32_t APB1ENR;  volatile uint32_t BDCR;
    volatile uint32_t CSR;
} RCC_TypeDef;

typedef struct {
    volatile uint32_t CRL;  volatile uint32_t CRH; volatile uint32_t IDR;
    volatile uint32_t ODR;  volatile uint32_t BSRR; volatile uint32_t BRR;
    volatile uint32_t LCKR;
} GPIO_TypeDef;

typedef struct {
    volatile uint32_t EVCR; volatile uint32_t MAPR; volatile uint32_t EXTICR[4];
    volatile uint32_t MAPR2;
} AFIO_TypeDef;

typedef struct {
    volatile uint32_t IMR;  volatile uint32_t EMR;  volatile uint32_t RTSR;
    volatile uint32_t FTSR; volatile uint32_t SWIER; volatile uint32_t PR;
} EXTI_TypeDef;

typedef struct {
    volatile uint32_t CTRL; volatile uint32_t LOAD; volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;

// Bổ sung cho các tuần sau: TIMERS (TIM2, TIM3, TIM4)
typedef struct {
    volatile uint32_t CR1;  volatile uint32_t CR2;  volatile uint32_t SMCR;
    volatile uint32_t DIER; volatile uint32_t SR;   volatile uint32_t EGR;
    volatile uint32_t CCMR1; volatile uint32_t CCMR2; volatile uint32_t CCER;
    volatile uint32_t CNT;  volatile uint32_t PSC;  volatile uint32_t ARR;
    volatile uint32_t RCR;  volatile uint32_t CCR1; volatile uint32_t CCR2;
    volatile uint32_t CCR3; volatile uint32_t CCR4; volatile uint32_t BDTR;
    volatile uint32_t DCR;  volatile uint32_t DMAR;
} TIM_TypeDef;

// Bổ sung UART (Truyền thông nối tiếp)
typedef struct {
    volatile uint32_t SR; volatile uint32_t DR; volatile uint32_t BRR;
    volatile uint32_t CR1; volatile uint32_t CR2; volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

// Bổ sung I2C và SPI (Giao tiếp cảm biến)
typedef struct {
    volatile uint32_t CR1; volatile uint32_t CR2; volatile uint32_t OAR1;
    volatile uint32_t OAR2; volatile uint32_t DR; volatile uint32_t SR1;
    volatile uint32_t SR2; volatile uint32_t CCR; volatile uint32_t TRISE;
} I2C_TypeDef;

typedef struct {
    volatile uint32_t CR1; volatile uint32_t CR2; volatile uint32_t SR;
    volatile uint32_t DR; volatile uint32_t CRCPR; volatile uint32_t RXCRCR;
    volatile uint32_t TXCRCR; volatile uint32_t I2SCFGR; volatile uint32_t I2SPR;
} SPI_TypeDef;

// --- 2. ÉP KIỂU ĐỊA CHỈ CON TRỎ TỚI PHẦN CỨNG ---
#define RCC       ((RCC_TypeDef *) (AHBPERIPH_BASE + 0x1000))
#define GPIOA     ((GPIO_TypeDef *) (APB2PERIPH_BASE + 0x0800))
#define GPIOB     ((GPIO_TypeDef *) (APB2PERIPH_BASE + 0x0C00))
#define GPIOC     ((GPIO_TypeDef *) (APB2PERIPH_BASE + 0x1000))
#define AFIO      ((AFIO_TypeDef *) (APB2PERIPH_BASE + 0x0000))
#define EXTI      ((EXTI_TypeDef *) (APB2PERIPH_BASE + 0x0400))
#define SysTick   ((SysTick_TypeDef *) 0xE000E010)

#define TIM2      ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x0000))
#define TIM3      ((TIM_TypeDef *) (APB1PERIPH_BASE + 0x0400))
#define USART1    ((USART_TypeDef *) (APB2PERIPH_BASE + 0x3800))
#define USART2    ((USART_TypeDef *) (APB1PERIPH_BASE + 0x4400))
#define SPI1      ((SPI_TypeDef *) (APB2PERIPH_BASE + 0x3000))
#define I2C1      ((I2C_TypeDef *) (APB1PERIPH_BASE + 0x5400))

#endif
