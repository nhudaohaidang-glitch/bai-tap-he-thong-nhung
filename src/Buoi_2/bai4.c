#include "stm32f10x.h"



int main(void) {

    // BƯỚC 1: CẤP XUNG NHỊP (CLOCK)

    // Bật Clock cho Timer 2 (Bit 0 thanh ghi APB1ENR)

    RCC->APB1ENR |= (1 << 0);

    // Bật Clock cho Port A (Bit 2) và khối Alternate Function (Bit 0) trên APB2ENR

    RCC->APB2ENR |= (1 << 2) | (1 << 0);



    // BƯỚC 2: CẤU HÌNH GPIOA (Chân PA0, PA1, PA2, PA3)

    // Để chân điều khiển được bởi ngoại vi (Timer), bắt buộc cấu hình ở chế độ

    // Alternate Function Push-Pull (AF-PP) với tốc độ 50MHz (Mã hex: 0xB).

    GPIOA->CRL &= ~0x0000FFFF;  // Xóa sạch cấu hình cũ của 4 chân đầu tiên

    GPIOA->CRL |=  0x0000BBBB;  // Ghi mã 0xB vào PA0, PA1, PA2, PA3



    // BƯỚC 3: CẤU HÌNH TIMER 2 (Chu kỳ 1KHz)

    TIM2->PSC = 7;      // Chia xung nhịp: 8MHz / (7 + 1) = 1MHz

    TIM2->ARR = 999;    // Chu kỳ đếm: 1MHz / (999 + 1) = 1000Hz (1KHz)



    // BƯỚC 4: THIẾT LẬP CHẾ ĐỘ PWM MODE 1 CHO 4 KÊNH

    // Kênh 1 & 2 (thanh ghi CCMR1): Set bit [6:4] và [14:12] bằng 110 (số 6 hệ thập phân)

    TIM2->CCMR1 |= (6 << 4) | (6 << 12);

    // Kênh 3 & 4 (thanh ghi CCMR2): Set bit [6:4] và [14:12] bằng 110 

    TIM2->CCMR2 |= (6 << 4) | (6 << 12);



    // BƯỚC 5: GHI GIÁ TRỊ ĐỘ RỘNG XUNG (DUTY CYCLE)

    // Hệ thống đếm từ 0 đến 999 (Tổng 1000 đơn vị thời gian)

    TIM2->CCR1 = 100;   // Kênh 1 (PA0): Mức cao chiếm 10%

    TIM2->CCR2 = 300;   // Kênh 2 (PA1): Mức cao chiếm 30%

    TIM2->CCR3 = 500;   // Kênh 3 (PA2): Mức cao chiếm 50%

    TIM2->CCR4 = 700;   // Kênh 4 (PA3): Mức cao chiếm 70%



    // BƯỚC 6: XUẤT TÍN HIỆU VÀ CHẠY TIMER

    // Cho phép xuất tín hiệu ra chân vật lý ở cả 4 kênh (Bit 0, 4, 8, 12)

    TIM2->CCER |= (1 << 0) | (1 << 4) | (1 << 8) | (1 << 12);

    

    // Kích hoạt bộ đếm Timer 2 (Bit 0 thanh ghi CR1)

    TIM2->CR1 |= (1 << 0);



    // Vòng lặp chính rỗng: CPU không cần làm gì, phần cứng tự động băm xung

    while (1) {

    }

} 
