#include "stm32f10x.h"

void delay_ms(uint16_t t) {
    // Thêm volatile để chống trình biên dịch tối ưu hóa (xóa) vòng lặp rỗng
    volatile int i, j; 
    for(i = 0; i < t; i++){
        // Giảm từ 0x2AFF xuống 1333 để tương thích với xung nhịp 8MHz
        for(j = 0; j <= 1333; j++){ 
        }
    }
}

int main(void){
    // Cap clk cho port C
    RCC->APB2ENR |= 0x00000010; 
    
    // Cau hinh chan PC13 o che do Output push pull
    GPIOC->CRH &= ~0x00F00000; 
    GPIOC->CRH |= 0x00300000; 
    
    while (1){
        GPIOC->ODR ^= (1<<13); 
        delay_ms(1000); 
    }
}
