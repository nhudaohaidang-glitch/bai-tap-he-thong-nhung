#include "stm32f10x.h"
#include "MAX7219.h"

// Hàm tạo trễ nội bộ cho thư viện
static void delay_us(uint32_t us) {
    for(volatile uint32_t i = 0; i < us * 8; i++);
}

void MAX7219_Write(uint8_t address, uint8_t data) {
    uint16_t frame = (address << 8) | data;
    
    GPIOA->BRR = (1 << 4); // CS Low
    
    for (int i = 15; i >= 0; i--) {
        if (frame & (1 << i)) GPIOA->BSRR = (1 << 7); // DIN High
        else                  GPIOA->BRR  = (1 << 7); // DIN Low
        
        GPIOA->BSRR = (1 << 5); // CLK High
        GPIOA->BRR  = (1 << 5); // CLK Low
    }
    
    GPIOA->BSRR = (1 << 4); // CS High
}

void MAX7219_Clear(void) {
    for(int i = 1; i <= 8; i++) {
        MAX7219_Write(i, 0x00);
    }
}

void MAX7219_Init(void) {
    RCC->APB2ENR |= (1 << 2); 
    
    // PA4(CS), PA5(CLK), PA7(DIN) cấu hình Output Push-Pull 50MHz
    GPIOA->CRL &= ~((0xF << 16) | (0xF << 20) | (0xF << 28));
    GPIOA->CRL |=  ((0x3 << 16) | (0x3 << 20) | (0x3 << 28));
    
    GPIOA->BSRR = (1 << 4); 
    GPIOA->BRR  = (1 << 5) | (1 << 7);
    
    delay_us(1000); // Ổn định nguồn
    
    MAX7219_Write(0x09, 0x00); // Tắt Decode
    MAX7219_Write(0x0A, 0x01); // Sáng mức 1
    MAX7219_Write(0x0B, 0x07); // Quét 8 hàng
    MAX7219_Write(0x0F, 0x00); // Tắt Test
    MAX7219_Write(0x0C, 0x01); // Bật IC
    
    MAX7219_Clear();
}
