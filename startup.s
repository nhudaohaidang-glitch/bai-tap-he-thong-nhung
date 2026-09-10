.syntax unified
.cpu cortex-m3
.thumb

.global vtable
.global Reset_Handler

/* Đặt bảng Vector vào đúng phân vùng .isr_vector */
.section .isr_vector,"a",%progbits
vtable:
    .word 0x20005000      /* Vị trí 0: Đỉnh Stack (RAM) */
    .word Reset_Handler   /* Vị trí 1: Hàm khởi động */

/* Code thực thi */
.section .text
Reset_Handler:
    bl main               /* Nhảy vào hàm main() trong C */
    b .                   /* Vòng lặp vô tận nếu main() vô tình kết thúc */
