#include "stm32f10x.h"
#include "MAX7219.h"

int main(void) {
    // Khởi tạo ngoại vi GPIO và cấu hình IC MAX7219
    MAX7219_Init();

    // Hiển thị số 1 (Mỗi lệnh tương ứng với 1 hàng từ trên xuống dưới)
    MAX7219_Write(1, 0x18); // Hàng 1: 0001 1000 
    MAX7219_Write(2, 0x38); // Hàng 2: 0011 1000 (Có thêm nét gạch chéo ở đầu chữ số 1)
    MAX7219_Write(3, 0x18); // Hàng 3: 0001 1000
    MAX7219_Write(4, 0x18); // Hàng 4: 0001 1000
    MAX7219_Write(5, 0x18); // Hàng 5: 0001 1000
    MAX7219_Write(6, 0x18); // Hàng 6: 0001 1000
    MAX7219_Write(7, 0x18); // Hàng 7: 0001 1000
    MAX7219_Write(8, 0x7E); // Hàng 8: 0111 1110 (Nét gạch ngang ở chân chữ số 1)

    while (1) {
        // Vi điều khiển nhàn rỗi, MAX7219 tự động chốt và duy trì hiển thị
    }
}
