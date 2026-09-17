#include "stm32f10x.h"

// Hàm delay đã được cân chỉnh cho xung nhịp nội 8MHz 
void delay_ms(uint16_t t) {
    volatile int i, j;
    for (i = 0; i < t; i++) {
        for (j = 0; j <= 1333; j++) {
        }
    }
}

int main(void) {
    // 1. Cấp xung nhịp cho Port A (Bit số 2 trong thanh ghi APB2ENR)
    RCC->APB2ENR |= (1 << 2);
    
    // 2. Cấu hình PA0 đến PA7 ở chế độ Output Push-Pull, tốc độ 50MHz
    // Thanh ghi CRL quản lý 8 chân thấp (Pin 0 - Pin 7). Mỗi chân chiếm 4 bit.
    // Mã cấu hình Output 50MHz Push-Pull là 0x3.
    // Cấu hình đồng loạt 8 chân: 0x33333333
    GPIOA->CRL = 0x33333333;
    // Mảng chứa giá trị Hex để lần lượt bật từng LED (dịch bit)
    uint16_t led_arr[8] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080};

    while(1) {
        // Chiều 1: LED chạy từ trái sang phải (Từ PA0 đến PA7)
        for(int i = 0; i <= 7; i++) {
            // Dùng mặt nạ 0xFF00 để giữ nguyên trạng thái nửa trên (PA8-PA15)
            // Nhằm bảo vệ an toàn cho 2 chân nạp code (PA13, PA14)
            GPIOA->ODR = (GPIOA->ODR & 0xFF00) | led_arr[i];
            delay_ms(200); // Trễ 200ms cho hiệu ứng lướt nhanh
        }
    
        // Chiều 2: LED chạy ngược từ phải sang trái (Từ PA6 về PA1)
        // Bỏ qua i=7 và i=0 ở vòng lặp này để tránh việc 2 chân ngoài cùng bị chớp 2 lần liên tiếp
        for(int i = 6; i >= 1; i--) {
            GPIOA->ODR = (GPIOA->ODR & 0xFF00) | led_arr[i];
            delay_ms(200);
        }
    }
}
