#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"


// =========================================================================
// 2. KHAI BÁO CẤU TRÚC THAM SỐ CHO LED
// =========================================================================
typedef struct {
    uint16_t pin_mask;  // Chân điều khiển 
    uint32_t delay_ms;  // Thời gian chờ để nháy (ms)
} LED_Param_t;

// =========================================================================
// 3. HÀM TASK DUY NHẤT
// =========================================================================
void vTaskBlinkLED(void *pvParameters) {
    LED_Param_t *pxParam = (LED_Param_t *)pvParameters;
    
    uint16_t pin = pxParam->pin_mask;
    TickType_t delay_ticks = pdMS_TO_TICKS(pxParam->delay_ms);

    while (1) {
        GPIOA->ODR ^= pin;          
        vTaskDelay(delay_ticks);    
    }
}

// =========================================================================
// 4. CHƯƠNG TRÌNH CHÍNH
// =========================================================================
int main(void) {
    // Cấu hình phần cứng
    RCC->APB2ENR |= (1 << 2);
    GPIOA->CRL &= ~((0xF << 4) | (0xF << 8) | (0xF << 12)); 
    GPIOA->CRL |=  ((0x2 << 4) | (0x2 << 8) | (0x2 << 12)); 
    GPIOA->BRR = (1 << 1) | (1 << 2) | (1 << 3); 

    // Cấu hình thông số 3 LED (Bắt buộc có static)
    static LED_Param_t led1 = { (1 << 1), 50 };   
    static LED_Param_t led2 = { (1 << 2), 500 };  
    static LED_Param_t led3 = { (1 << 3), 5000 }; 

    // Gọi 3 Task sử dụng chung 1 hàm
    xTaskCreate(vTaskBlinkLED, "Task_LED1", 128, (void *)&led1, 1, NULL);
    xTaskCreate(vTaskBlinkLED, "Task_LED2", 128, (void *)&led2, 1, NULL);
    xTaskCreate(vTaskBlinkLED, "Task_LED3", 128, (void *)&led3, 1, NULL);

    // Khởi động hệ điều hành
    vTaskStartScheduler();

    while (1) {
    }
}
