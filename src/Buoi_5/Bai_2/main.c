#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// =========================================================================
// BIẾN TOÀN CỤC
// =========================================================================
QueueHandle_t qUART;
volatile uint32_t press_count = 0; // Biến đếm số lần nhấn nút

// =========================================================================
// HÀM HỖ TRỢ: GỬI VÀ ĐỊNH DẠNG CHUỖI UART
// (Thay thế sprintf để tránh lỗi thư viện hệ thống trên bare-metal)
// =========================================================================
void UART1_SendString(char* str) {
    while (*str) {
        while (!(USART1->SR & (1 << 7))); // Chờ cờ TXE trống
        USART1->DR = (*str & 0xFF);
        str++;
    }
}

// Đạt yêu cầu: "Định dạng chuỗi ký tự hiển thị"
void UART1_SendNumber(uint32_t num) {
    char buffer[12];
    int i = 10;
    buffer[11] = '\0';
    if (num == 0) { UART1_SendString("0"); return; }
    while (num > 0 && i >= 0) { buffer[i] = (num % 10) + '0'; num /= 10; i--; }
    UART1_SendString(&buffer[i + 1]);
}

// =========================================================================
// 1. CẤU HÌNH PHẦN CỨNG 
// =========================================================================
void Hardware_Init(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 3) | (1 << 0) | (1 << 14);

    // Đạt yêu cầu: "Cấu hình giao tiếp UART1 truyền thông nối tiếp với máy tính"
    GPIOA->CRH &= ~0x00000FF0; 
    GPIOA->CRH |=  0x000004A0; // PA9 (TX), PA10 (RX)
    USART1->BRR = 0x0341;      // Baudrate 9600
    USART1->CR1 |= (1 << 13) | (1 << 3) | (1 << 2); 

    // Cấu hình chân PB0 làm Input Pull-up
    GPIOB->CRL &= ~0x0000000F;
    GPIOB->CRL |=  0x00000008;
    GPIOB->ODR |=  (1 << 0); 

    // Đạt yêu cầu: "Cấu hình 1 nút nhấn tạo ngắt ngoài (EXTI - cạnh xuống/Falling edge)"[cite: 7]
    AFIO->EXTICR[0] &= ~0x000F; 
    AFIO->EXTICR[0] |=  0x0001; // Map EXTI0 vào PB0
    EXTI->IMR |= (1 << 0);      // Bật ngắt dòng 0
    EXTI->FTSR |= (1 << 0);     // Kích hoạt ngắt ở sườn xuống (Falling edge)[cite: 7]

    // Cấu hình ưu tiên ngắt cho FreeRTOS (Bắt buộc)
    NVIC->IP[6] = (15 << 4);     
    NVIC->ISER[0] |= (1 << 6);  
    *((volatile uint32_t *)0xE000ED0C) = 0x05FA0300; // Nhóm ưu tiên ngắt = 4
}

// =========================================================================
// 2. NGẮT NGOÀI EXTI0[cite: 7]
// =========================================================================
void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {
        EXTI->PR = (1 << 0); // Xóa cờ ngắt

        // Yêu cầu 1: Tăng biến đếm số lần nhấn nút[cite: 7]
        press_count++; 
        
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        // Yêu cầu 2: Gửi giá trị số đếm vào Queue từ ISR[cite: 7]
        xQueueSendFromISR(qUART, (void *)&press_count, &xHigherPriorityTaskWoken);
        
        // Yêu cầu 3: Chuyển ngữ cảnh ngay lập tức sang Task nhận nếu có yêu cầu[cite: 7]
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

// =========================================================================
// 3. TASK TRUYỀN DỮ LIỆU UART[cite: 7]
// =========================================================================
void vTaskUART(void *pvParameters) {
    uint32_t received_count = 0;

    while (1) {
        // Yêu cầu 1: Đưa Task về trạng thái Blocked để chờ dữ liệu từ Queue (portMAX_DELAY)[cite: 7]
        if (xQueueReceive(qUART, &received_count, portMAX_DELAY) == pdPASS) {
            
            // Yêu cầu 2: Nhận giá trị đếm và định dạng chuỗi ký tự hiển thị[cite: 7]
            // Yêu cầu 3: Truyền chuỗi ký tự qua UART lên Serial Terminal của máy tính[cite: 7]
            UART1_SendString("So lan nhan nut: ");
            UART1_SendNumber(received_count);
            UART1_SendString("\r\n");
        }
    }
}

// =========================================================================
// CHƯƠNG TRÌNH CHÍNH
// =========================================================================
int main(void) {
    Hardware_Init();

    // Khởi tạo Queue
    qUART = xQueueCreate(10, sizeof(uint32_t));

    if (qUART != NULL) {
        // Tạo Task UART
        xTaskCreate(vTaskUART, "UART_Task", 256, NULL, 1, NULL);
        
        // Khởi động hệ điều hành
        vTaskStartScheduler();
    }

    while (1) {}
}
