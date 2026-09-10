#include "stm32f10x.h"

// Hàm delay (tương đối cho 8MHz)
void delay_ms(uint16_t t) {
    volatile int i, j;
    for (i = 0; i < t; i++) {
        for (j = 0; j <= 1333; j++) {}
    }
}

int main(void) {
    // 1. Cấp xung nhịp cho Port A
    RCC->APB2ENR |= (1 << 2);

    // 2. Cấu hình các chân PA0 và PA1
    // Xóa cấu hình cũ của 2 chân (8 bit thấp nhất của CRL)
    GPIOA->CRL &= ~0x000000FF; 
    
    // PA1 (Output 50MHz Push-pull): Mode = 11, CNF = 00 -> mã là 3
    // PA0 (Input Pull-up/Pull-down): Mode = 00, CNF = 10 -> mã là 8
    // Ghép lại ta có 0x38 cho 2 chân này
    GPIOA->CRL |= 0x00000038;

    // 3. Kích hoạt điện trở kéo lên (Pull-up) cho PA0
    // Ở chế độ Input, ghi mức 1 vào ODR tương đương với việc bật Pull-up.
    // LƯU Ý PHẦN CỨNG: Một đầu nút nhấn nối vào PA0, đầu còn lại nối xuống GND (đất).
    GPIOA->ODR |= (1 << 0);

    while (1) {
        // Đọc trạng thái chân PA0 từ thanh ghi IDR (Nhấn nút = 0, Nhả nút = 1)
        if ((GPIOA->IDR & (1 << 0)) == 0) {
            
            // Trễ khoảng 20ms để chống dội phím (Debounce)
            delay_ms(20);
            
            // Kiểm tra lại để chắc chắn không phải nhiễu
            if ((GPIOA->IDR & (1 << 0)) == 0) {
                
                // CỐT LÕI BÀI TOÁN: Vòng lặp chờ người dùng NHẢ NÚT ra
                // Chừng nào phím còn bị giữ (PA0 == 0), hệ thống sẽ kẹt ở đây
                while ((GPIOA->IDR & (1 << 0)) == 0) {}
                
                // Người dùng vừa nhả nút ra, tiến hành đảo trạng thái LED PA1
                GPIOA->ODR ^= (1 << 1);
            }
        }
    }
}
