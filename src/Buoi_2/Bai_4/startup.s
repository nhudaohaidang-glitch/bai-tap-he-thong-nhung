.syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

.global g_pfnVectors
.global Default_Handler
.global Reset_Handler    /* SỬA LỖI 1: Công bố toàn cục Reset_Handler */

/* Khai báo các biến từ Linker Script */
.word _sidata
.word _sdata
.word _edata
.word _sbss
.word _ebss

  .section .text.Reset_Handler
  .type Reset_Handler, %function
Reset_Handler:
  ldr sp, =_estack      

  ldr r0, =_sdata
  ldr r1, =_edata
  ldr r2, =_sidata
  movs r3, #0
CopyData:
  ldr r4, [r2, r3]
  str r4, [r0, r3]
  adds r3, r3, #4
  adds r4, r0, r3
  cmp r4, r1
  bcc CopyData

  ldr r2, =_sbss
  ldr r4, =_ebss
  movs r3, #0
FillBss:
  str r3, [r2]
  adds r2, r2, #4
  cmp r2, r4
  bcc FillBss

  bl main
  bx lr
.size Reset_Handler, .-Reset_Handler

/* Hàm xử lý mặc định nếu quên viết ngắt */
  .section .text.Default_Handler,"ax",%progbits
  .type Default_Handler, %function    /* SỬA LỖI 2: Khai báo rõ đây là Function */
Default_Handler:
  b Default_Handler
.size Default_Handler, .-Default_Handler

/* Cấu hình weak để bạn có thể tự viết đè các hàm ngắt ở main.c */
  .weak SysTick_Handler
  .thumb_set SysTick_Handler, Default_Handler
  .weak TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler, Default_Handler
  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler, Default_Handler
  .weak EXTI15_10_IRQHandler
  .thumb_set EXTI15_10_IRQHandler, Default_Handler

  .section .isr_vector,"a",%progbits
  .type g_pfnVectors, %object
g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word Default_Handler /* NMI */
  .word Default_Handler /* HardFault */
  .word Default_Handler
  .word Default_Handler
  .word Default_Handler
  .word 0
  .word 0
  .word 0
  .word 0
  .word Default_Handler /* SVC */
  .word Default_Handler
  .word 0
  .word Default_Handler /* PendSV */
  .word SysTick_Handler /* SysTick */
  /* Các ngắt ngoại vi mở rộng */
  .word Default_Handler /* WWDG */
  .word Default_Handler /* PVD */
  .word Default_Handler /* TAMPER */
  .word Default_Handler /* RTC */
  .word Default_Handler /* FLASH */
  .word Default_Handler /* RCC */
  .word Default_Handler /* EXTI0 */
  .word Default_Handler /* EXTI1 */
  .word Default_Handler /* EXTI2 */
  .word Default_Handler /* EXTI3 */
  .word Default_Handler /* EXTI4 */
  .word Default_Handler /* DMA1_1 */
  .word Default_Handler /* DMA1_2 */
  .word Default_Handler /* DMA1_3 */
  .word Default_Handler /* DMA1_4 */
  .word Default_Handler /* DMA1_5 */
  .word Default_Handler /* DMA1_6 */
  .word Default_Handler /* DMA1_7 */
  .word Default_Handler /* ADC1_2 */
  .word Default_Handler /* USB_HP_CAN_TX */
  .word Default_Handler /* USB_LP_CAN_RX */
  .word Default_Handler /* EXTI9_5 */
  .word Default_Handler /* TIM1_BRK */
  .word Default_Handler /* TIM1_UP */
  .word Default_Handler /* TIM1_TRG_COM */
  .word Default_Handler /* TIM1_CC */
  .word TIM2_IRQHandler /* TIM2 */
  .word Default_Handler /* TIM3 */
  .word Default_Handler /* TIM4 */
  .word Default_Handler /* I2C1_EV */
  .word Default_Handler /* I2C1_ER */
  .word Default_Handler /* I2C2_EV */
  .word Default_Handler /* I2C2_ER */
  .word Default_Handler /* SPI1 */
  .word Default_Handler /* SPI2 */
  .word USART1_IRQHandler /* USART1 */
  .word Default_Handler /* USART2 */
  .word Default_Handler /* USART3 */
  .word EXTI15_10_IRQHandler /* EXTI15_10 */
