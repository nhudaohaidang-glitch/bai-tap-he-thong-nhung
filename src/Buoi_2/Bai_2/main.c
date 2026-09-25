#include "stm32f10x.h"

volatile uint32_t count_01Hz = 0;
volatile uint32_t count_1Hz  = 0;
volatile uint32_t count_10Hz = 0;


/* =========================
   Cấu hình GPIO
   PA0 -> LED 0.1 Hz
   PA1 -> LED 1 Hz
   PA2 -> LED 10 Hz

   PA9  -> UART TX
   PA10 -> UART RX
   ========================= */
void GPIO_Config(void)
{
    // Bật clock GPIOA
    RCC->APB2ENR |= (1 << 2);

    // PA0, PA1, PA2: Output Push-Pull, 2 MHz
    GPIOA->CRL &= ~(0xFFF);
    GPIOA->CRL |=  (0x222);
}


/* =========================
   SysTick = 1 ms
   HCLK = 8 MHz
   ========================= */
void SysTick_Config_1ms(void)
{
    // 8 MHz -> 1 ms
    SysTick->LOAD = 8000 - 1;

    // Xóa bộ đếm
    SysTick->VAL = 0;

    /*
       Bit 2 = HCLK
       Bit 1 = Enable interrupt
       Bit 0 = Enable SysTick
    */
    SysTick->CTRL = (1 << 2) |
                    (1 << 1) |
                    (1 << 0);
}


/* =========================
   Ngắt SysTick
   ========================= */
void SysTick_Handler(void)
{
    count_01Hz++;
    count_1Hz++;
    count_10Hz++;


    /* PA0: 0.1 Hz
       Đảo trạng thái mỗi 5 giây */
    if (count_01Hz >= 5000)
    {
        GPIOA->ODR ^= (1 << 0);
        count_01Hz = 0;
    }


    /* PA1: 1 Hz
       Đảo trạng thái mỗi 500 ms */
    if (count_1Hz >= 500)
    {
        GPIOA->ODR ^= (1 << 1);
        count_1Hz = 0;
    }


    /* PA2: 10 Hz
       Đảo trạng thái mỗi 50 ms */
    if (count_10Hz >= 50)
    {
        GPIOA->ODR ^= (1 << 2);
        count_10Hz = 0;
    }
}


/* =========================
   USART1 IRQ
   Không dùng trong BAI_2
   ========================= */
void USART1_IRQHandler(void)
{
}


/* =========================
   MAIN
   ========================= */
int main(void)
{
    GPIO_Config();

    SysTick_Config_1ms();

    while (1)
    {
    }
}
