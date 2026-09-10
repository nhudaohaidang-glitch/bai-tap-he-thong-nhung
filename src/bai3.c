#include "stm32f10x.h"

// Hàm delay cân chỉnh cho xung nhịp nội 8MHz
void delay_ms(uint16_t t) {
    volatile int i, j;
    for (i = 0; i < t; i++) {
        for (j = 0; j <= 1333; j++) {}
    }
}

int main(void) {
    uint16_t current_state = 0;
    uint16_t confirmed_state = 0;
    uint16_t output_res = 0;

    // 1. Cấp xung nhịp cho Port A (bit 2) và Port B (bit 3)
    RCC->APB2ENR |= (1 << 2) | (1 << 3); 

    // 2. Cấu hình PA0-PA7 làm Input Pull-up
    // CRL quản lý nửa dưới (chân 0-7). Mã 8 là Input Pull-up/down
    GPIOA->CRL &= ~0xFFFFFFFF; // Xóa sạch cấu hình cũ
    GPIOA->CRL |= 0x88888888;
    
    // Ghi mức 1 vào 8 bit thấp ODR để kích hoạt điện trở kéo lên
    GPIOA->ODR |= 0x00FF; 

    // 3. Cấu hình PB8-PB15 làm Output Push-pull 50MHz
    // CRH quản lý nửa trên (chân 8-15). Mã 3 là Output 50MHz Push-pull
    GPIOB->CRH &= ~0xFFFFFFFF; // Xóa sạch cấu hình cũ
    GPIOB->CRH |= 0x33333333;  

    while(1) {
        // BƯỚC 1: Đọc trạng thái thô của 8 nút nhấn
        current_state = GPIOA->IDR & 0x00FF;
        
        // BƯỚC 2: Trễ 20ms để chờ các tiếp điểm cơ khí hết rung (chống dội)
        delay_ms(20);
        
        // BƯỚC 3: Đọc lại và so sánh. Nếu sau 20ms mà tín hiệu vẫn không đổi -> phím đã ổn định
        if (current_state == (GPIOA->IDR & 0x00FF)) {
            confirmed_state = current_state;
        }

        // BƯỚC 4: Xử lý logic đảo bit theo đề bài (0 thành 1, 1 thành 0)
        output_res = (~confirmed_state) & 0x00FF;

        // BƯỚC 5: Xuất tín hiệu ra dàn LED ở PB8 - PB15
        // Dịch trái 8 bit để đẩy dữ liệu lên nửa trên của Port B
        // Dùng mặt nạ 0x00FF để bảo vệ an toàn, không làm hỏng dữ liệu đang có ở PB0-PB7
        GPIOB->ODR = (GPIOB->ODR & 0x00FF) | (output_res << 8);
    }
}
