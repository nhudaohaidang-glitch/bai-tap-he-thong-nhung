#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

// =========================================================================
// 1. CÁC HÀM HOOK VÀ THƯ VIỆN BỔ SUNG CỦA HỆ ĐIỀU HÀNH
// =========================================================================
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    (void)xTask;
    (void)pcTaskName;
    while (1); 
}
void vApplicationMallocFailedHook(void) { while(1); }
void vApplicationIdleHook(void) {}
void vApplicationTickHook(void) {}

void *memset(void *dest, int c, unsigned int count) {
    char *bytes = (char *)dest;
    while (count--) *bytes++ = (char)c;
    return dest;
}
void *memcpy(void *dest, const void *src, unsigned int count) {
    char *dest8 = (char *)dest;
    const char *src8 = (const char *)src;
    while (count--) *dest8++ = *src8++;
    return dest;
}

// =========================================================================
// 2. CÁC TASK NHÁY LED VỚI TẦN SỐ ĐƯỢC YÊU CẦU
// =========================================================================

// Task 1: Nháy 10 Hz (Chu kỳ 100ms -> Chờ 50ms)
void vTaskLED1(void *pvParameters) {
    while (1) {
        GPIOA->ODR ^= (1 << 1);        
        vTaskDelay(pdMS_TO_TICKS(50)); 
    }
}

// Task 2: Nháy 1 Hz (Chu kỳ 1000ms -> Chờ 500ms)
void vTaskLED2(void *pvParameters) {
    while (1) {
        GPIOA->ODR ^= (1 << 2);        
        vTaskDelay(pdMS_TO_TICKS(500)); 
    }
}

// Task 3: Nháy 0.1 Hz (Chu kỳ 10000ms -> Chờ 5000ms)
void vTaskLED3(void *pvParameters) {
    while (1) {
        GPIOA->ODR ^= (1 << 3);        
        vTaskDelay(pdMS_TO_TICKS(5000)); 
    }
}

// =========================================================================
// 3. CHƯƠNG TRÌNH CHÍNH
// =========================================================================
int main(void) {
    // Cấu hình phần cứng: Bật clock GPIOA
    RCC->APB2ENR |= (1 << 2);
    
    // Cấu hình PA1, PA2, PA3 làm Output Push-Pull 2MHz
    GPIOA->CRL &= ~((0xF << 4) | (0xF << 8) | (0xF << 12)); 
    GPIOA->CRL |=  ((0x2 << 4) | (0x2 << 8) | (0x2 << 12)); 
    
    // Tắt cả 3 LED lúc khởi động
    GPIOA->BRR = (1 << 1) | (1 << 2) | (1 << 3); 

    // Tạo các Task giao cho hệ điều hành quản lý
    xTaskCreate(vTaskLED1, "LED_10Hz", 128, NULL, 1, NULL);
    xTaskCreate(vTaskLED2, "LED_1Hz",  128, NULL, 1, NULL);
    xTaskCreate(vTaskLED3, "LED_0_1Hz", 128, NULL, 1, NULL);

    // Khởi động bộ lập lịch (Scheduler)
    vTaskStartScheduler();

    while (1) {
    }
}
