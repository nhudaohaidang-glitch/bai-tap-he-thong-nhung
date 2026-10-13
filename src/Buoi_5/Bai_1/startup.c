#include <stdint.h>

// Khai báo hàm main
extern int main(void);

// Khai báo các biến địa chỉ vùng nhớ từ Linker Script
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// Khai báo các hàm ngắt của FreeRTOS
extern void vPortSVCHandler(void);
extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);

// Trình phục vụ ngắt mặc định (bắt lỗi)
void Default_Handler(void) {
    while (1);
}

// Trình phục vụ ngắt Reset (Chạy đầu tiên khi cấp nguồn)
void Reset_Handler(void) {
    // 1. Copy dữ liệu (.data) từ Flash sang RAM
    uint32_t *src = &_sidata;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) {
        *dst++ = *src++;
    }
    
    // 2. Xóa vùng nhớ (.bss) bằng 0
    dst = &_sbss;
    while (dst < &_ebss) {
        *dst++ = 0;
    }
    
    // 3. Nhảy vào chương trình chính
    main();
    while (1);
}

typedef void (*ISR_Handler)(void);

// Bảng Vector Ngắt (Vector Table) đã nhúng FreeRTOS
__attribute__((section(".isr_vector"), used))
const ISR_Handler vector_table[16] = {
    [0] = (ISR_Handler)&_estack,
    [1] = Reset_Handler,
    [2 ... 10] = Default_Handler,
    [11] = vPortSVCHandler,     // SVCall: Khởi động bộ lập lịch OS
    [12 ... 13] = Default_Handler,
    [14] = xPortPendSVHandler,  // PendSV: Chuyển đổi Task OS
    [15] = xPortSysTickHandler  // SysTick: Đếm thời gian OS
};
