#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// =========================================================================
// 1. BIẾN TOÀN CỤC CỦA HỆ ĐIỀU HÀNH
// =========================================================================
QueueHandle_t qLED[3];

// =========================================================================
// 2. CẤU HÌNH PHẦN CỨNG 
// =========================================================================
void Hardware_Init(void) {
    RCC->APB2ENR |= (1 << 2) | (1 << 3) | (1 << 0); 
    RCC->APB1ENR |= (1 << 0);                       

    // Cấu hình PA0, PA1, PA2 làm PWM Output, PA3 input
    GPIOA->CRL &= ~0x0000FFFF; 
    GPIOA->CRL |=  0x00008AAA; 
    GPIOA->ODR |=  (1 << 3);
    
    // Cấu hình PB0, PB1 làm Input Pull-up
    GPIOB->CRL &= ~0x000000FF;
    GPIOB->CRL |=  0x00000088;
    GPIOB->ODR |=  (1 << 0) | (1  << 1); 

    // Cấu hình Timer 2 băm xung 1kHz
    TIM2->PSC = 8 - 1;       
    TIM2->ARR = 1000 - 1;    
    
    TIM2->CCMR1 |= (6 << 4) | (1 << 3) | (6 << 12) | (1 << 11); 
    TIM2->CCMR2 |= (6 << 4) | (1 << 3);                         
    TIM2->CCER  |= (1 << 0) | (1 << 4) | (1 << 8); 
    
    TIM2->CCR1 = 0; TIM2->CCR2 = 0; TIM2->CCR3 = 0; 
    TIM2->CR1 |= (1 << 7) | (1 << 0); 
}

// =========================================================================
// 3. TASK ĐIỀU KHIỂN LED 
// =========================================================================
void vTaskLED(void *pvParameters) {
    int led_id = (int)pvParameters; 
    uint16_t brightness = 0;
    volatile uint32_t *ccr_reg;     

    if (led_id == 0) ccr_reg = &(TIM2->CCR1);
    else if (led_id == 1) ccr_reg = &(TIM2->CCR2);
    else ccr_reg = &(TIM2->CCR3);

    while (1) {
        // Ngủ chờ tín hiệu từ Queue
        if (xQueueReceive(qLED[led_id], &brightness, portMAX_DELAY) == pdPASS) {
            *ccr_reg = brightness; 
        }
    }
}

// =========================================================================
// 4. TASK ĐỌC NÚT NHẤN 
// =========================================================================
void vTaskButton(void *pvParameters) {
    int current_led = 0; // Bắt đầu ở LED 1 (PA0)
    uint16_t brightness_state[3] = {0, 0, 0}; 
    
    // Lưu trạng thái trước đó của các nút (1 = Đang nhả phím)
    uint8_t last_btn1 = 1, last_btn2 = 1, last_btn3 = 1;

    while (1) {
        // Đọc trạng thái vật lý thực tế của 3 chân
        uint8_t btn1 = (GPIOB->IDR & (1 << 0)) ? 1 : 0;
        uint8_t btn2 = (GPIOB->IDR & (1 << 1)) ? 1 : 0;
        // ĐÃ SỬA: Đổi GPIOB thành GPIOA cho nút 3 ở chân PA3
        uint8_t btn3 = (GPIOA->IDR & (1 << 3)) ? 1 : 0;

        // NÚT 1: Đổi LED
        if (btn1 == 0 && last_btn1 == 1) {
            current_led = (current_led + 1) % 3; 
        }
        last_btn1 = btn1; 

        // NÚT 2: Tăng độ sáng
        if (btn2 == 0 && last_btn2 == 1) {
            if (brightness_state[current_led] <= 900) {
                brightness_state[current_led] += 100;
                xQueueSend(qLED[current_led], &brightness_state[current_led], 0);
            }
        }
        last_btn2 = btn2;

        // NÚT 3: Giảm độ sáng
        if (btn3 == 0 && last_btn3 == 1) {
            if (brightness_state[current_led] >= 100) {
                brightness_state[current_led] -= 100;
                xQueueSend(qLED[current_led], &brightness_state[current_led], 0);
            }
        }
        last_btn3 = btn3;

        // Nghỉ 50ms để lọc nhiễu dội phím (Debounce)
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

// =========================================================================
// 5. CHƯƠNG TRÌNH CHÍNH
// =========================================================================
int main(void) {
    Hardware_Init();

    // Khởi tạo 3 Queue độc lập
    qLED[0] = xQueueCreate(1, sizeof(uint16_t));
    qLED[1] = xQueueCreate(1, sizeof(uint16_t));
    qLED[2] = xQueueCreate(1, sizeof(uint16_t));

    // Tạo các Task
    xTaskCreate(vTaskButton, "Task_Btn", 128, NULL, 2, NULL); 
    xTaskCreate(vTaskLED, "Task_LED1", 128, (void *)0, 1, NULL);
    xTaskCreate(vTaskLED, "Task_LED2", 128, (void *)1, 1, NULL);
    xTaskCreate(vTaskLED, "Task_LED3", 128, (void *)2, 1, NULL);

    vTaskStartScheduler();

    while (1) {
    }
}
