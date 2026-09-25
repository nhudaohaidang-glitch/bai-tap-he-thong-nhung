#include "stm32f10x.h"

#define BUFFER_SIZE 100

#define MA_LOP  "D23DTVM02"
#define MA_NHOM "N08"

char rx_buffer[BUFFER_SIZE];
unsigned int rx_index = 0;


/* ================= UART1 INIT ================= */
void UART1_Init(void)
{
    /* Enable clock GPIOA */
    RCC->APB2ENR |= (1 << 2);

    /* Enable clock USART1 */
    RCC->APB2ENR |= (1 << 14);

    /* PA9 = USART1_TX
       Alternate Function Push-Pull, 50 MHz */
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);

    /* PA10 = USART1_RX
       Floating input */
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);

    /* Baudrate = 9600
       USART1 clock = 8 MHz */
    USART1->BRR = 0x0341;

    /* Enable USART */
    USART1->CR1 |= (1 << 13);

    /* Enable TX */
    USART1->CR1 |= (1 << 3);

    /* Enable RX */
    USART1->CR1 |= (1 << 2);
}


/* ================= SEND 1 BYTE ================= */
void UART1_SendChar(char c)
{
    while (!(USART1->SR & (1 << 7)));

    USART1->DR = c;
}


/* ================= SEND STRING ================= */
void UART1_SendString(char *str)
{
    while (*str)
    {
        UART1_SendChar(*str);
        str++;
    }
}


/* ================= RECEIVE 1 BYTE ================= */
char UART1_ReceiveChar(void)
{
    while (!(USART1->SR & (1 << 5)));

    return USART1->DR;
}


/* ================= MAIN ================= */
int main(void)
{
    UART1_Init();

    while (1)
    {
        /* Nhận từng ký tự từ PC */
        char c = UART1_ReceiveChar();

        /* Chưa gặp ! thì lưu vào buffer */
        if (c != '!')
        {
            if (rx_index < BUFFER_SIZE - 1)
            {
                rx_buffer[rx_index] = c;
                rx_index++;
            }
        }

        /* Gặp ! -> gửi lại bản tin */
        else
        {
            /* Kết thúc chuỗi */
            rx_buffer[rx_index] = '\0';

            /* <ma lop><ma nhom>: <ban tin> */
            UART1_SendString(MA_LOP);
            UART1_SendString(MA_NHOM);
            UART1_SendString(": ");
            UART1_SendString(rx_buffer);

            /* Xuống dòng */
            UART1_SendString("\n\r");

            /* Reset buffer để nhận bản tin mới */
            rx_index = 0;
        }
    }
}


/* startup.c yêu cầu hàm này.
   Bài này dùng polling, không dùng ngắt UART. */
void USART1_IRQHandler(void)
{
}
