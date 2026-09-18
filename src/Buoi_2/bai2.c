void HAL_SYSTICK_Callback(void);
#include "stm32f10x.h"

void HAL_Init(void) {
    SysTick->LOAD = 7999;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 1) | (1 << 0);
}

// Hàm này được startup.s gọi đến mỗi 1ms
void SysTick_Handler(void) {
    HAL_SYSTICK_Callback();
}

// Khai báo weak để giống hệt cách thư viện ST hoạt động
__attribute__((weak)) void HAL_SYSTICK_Callback(void) {
    static uint16_t count_10hz = 0;
    static uint16_t count_1hz = 0;
    static uint16_t count_0_1hz = 0;

    count_10hz++;
    if (count_10hz >= 50) {
        GPIOA->ODR ^= (1 << 0); // Đảo PA0
        count_10hz = 0;
    }

    count_1hz++;
    if (count_1hz >= 500) {
        GPIOA->ODR ^= (1 << 1); // Đảo PA1
        count_1hz = 0;
    }

    count_0_1hz++;
    if (count_0_1hz >= 5000) {
        GPIOA->ODR ^= (1 << 2); // Đảo PA2
        count_0_1hz = 0;
    }
}

int main(void) {
    HAL_Init();

    RCC->APB2ENR |= (1 << 2);
    
    GPIOA->CRL &= ~0x00000FFF;
    GPIOA->CRL |= 0x00000333;

    while (1) {
        // CPU rảnh rỗi
    }
}
