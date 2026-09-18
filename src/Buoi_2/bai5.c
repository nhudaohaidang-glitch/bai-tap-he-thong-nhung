#include "stm32f10x.h"
#include <stdint.h>

/* ================= VARIABLES ================= */

volatile char rx_buffer[32];
volatile uint8_t rx_index = 0;
volatile uint8_t command_ready = 0;

volatile uint8_t led_on = 0;
volatile uint8_t pwm_percent = 50;


/* ================= UART ================= */

void USART1_SendChar(char c)
{
    while (!(USART1->SR & (1 << 7)))
    {
    }

    USART1->DR = c;
}


void USART1_SendString(const char *s)
{
    while (*s)
    {
        USART1_SendChar(*s);
        s++;
    }
}


/* ================= PWM ================= */

uint16_t PWM_CCR(uint8_t percent)
{
    if (percent == 0)
        return 0;

    if (percent >= 100)
        return 1000;

    return percent * 10;
}


void PWM_Init(void)
{
    /* GPIOA clock */
    RCC->APB2ENR |= (1 << 2);

    /* TIM2 clock */
    RCC->APB1ENR |= (1 << 0);

    /* PA0 = TIM2_CH1
       Alternate Function Push-Pull 50 MHz */

    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |=  (0xB << 0);

    /*
     * HSI = 8 MHz
     *
     * 8 MHz / 8 = 1 MHz
     */

    TIM2->PSC = 7;

    /*
     * 1 MHz / 1000 = 1 kHz
     */

    TIM2->ARR = 999;

    /* PWM Mode 1 */

    TIM2->CCMR1 &= ~(7 << 4);
    TIM2->CCMR1 |=  (6 << 4);

    /* Preload */

    TIM2->CCMR1 |= (1 << 3);

    /* Enable CH1 */

    TIM2->CCER |= (1 << 0);

    /* LED OFF initially */

    TIM2->CCR1 = 0;

    /* Update registers */

    TIM2->EGR |= (1 << 0);

    /* Start timer */

    TIM2->CR1 |= (1 << 0);
}


void LED_ON(void)
{
    led_on = 1;

    TIM2->CCR1 = PWM_CCR(pwm_percent);
}


void LED_OFF(void)
{
    led_on = 0;

    TIM2->CCR1 = 0;
}


void PWM_Set(uint8_t percent)
{
    if (percent > 100)
        percent = 100;

    pwm_percent = percent;

    if (led_on)
    {
        TIM2->CCR1 = PWM_CCR(percent);
    }
}


/* ================= USART1 ================= */

void USART1_Init(void)
{
    /* GPIOA clock */

    RCC->APB2ENR |= (1 << 2);

    /* USART1 clock */

    RCC->APB2ENR |= (1 << 14);


    /*
     * PA9 = TX
     * Alternate Function Push-Pull
     */

    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);


    /*
     * PA10 = RX
     * Input Floating
     */

    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);


    /*
     * HSI = 8 MHz
     * Baudrate = 115200
     */

    USART1->BRR = 0x45;


    /* Enable USART */

    USART1->CR1 |= (1 << 13);

    /* Transmitter */

    USART1->CR1 |= (1 << 3);

    /* Receiver */

    USART1->CR1 |= (1 << 2);

    /* RX interrupt */

    USART1->CR1 |= (1 << 5);


    /* Enable USART1 interrupt */

    NVIC_EnableIRQ(USART1_IRQn);
}


/* ================= USART INTERRUPT ================= */

void USART1_IRQHandler(void)
{
    if (USART1->SR & (1 << 5))
    {
        char c = USART1->DR;


        /* Ignore Enter characters */

        if (c == '\r' || c == '\n')
        {
            return;
        }


        /* ! = end of command */

        if (c == '!')
        {
            rx_buffer[rx_index] = '\0';

            command_ready = 1;

            rx_index = 0;

            return;
        }


        /* Store character */

        if (rx_index < 31)
        {
            rx_buffer[rx_index] = c;

            rx_index++;
        }
        else
        {
            rx_index = 0;
        }
    }
}


/* ================= STRING COMPARE ================= */

uint8_t StringEqual(const char *a, const char *b)
{
    while (*a && *b)
    {
        if (*a != *b)
            return 0;

        a++;
        b++;
    }

    return (*a == '\0' && *b == '\0');
}


/* ================= NUMBER SEND ================= */

void SendNumber(uint8_t n)
{
    if (n >= 100)
    {
        USART1_SendString("100");
    }
    else if (n >= 10)
    {
        USART1_SendChar((n / 10) + '0');
        USART1_SendChar((n % 10) + '0');
    }
    else
    {
        USART1_SendChar(n + '0');
    }
}


/* ================= PWM COMMAND ================= */

void ProcessPWM(const char *cmd)
{
    uint8_t value = 0;
    uint8_t i = 4;


    while (cmd[i] >= '0' &&
           cmd[i] <= '9')
    {
        value = value * 10 +
                (cmd[i] - '0');

        i++;
    }


    if (value <= 100)
    {
        PWM_Set(value);

        USART1_SendString("PWM = ");

        SendNumber(value);

        USART1_SendString("%\r\n");
    }
    else
    {
        USART1_SendString("PWM ERROR\r\n");
    }
}


/* ================= COMMAND PROCESS ================= */

void ProcessCommand(void)
{
    char command[32];
    uint8_t i = 0;


    __disable_irq();


    while (rx_buffer[i] != '\0' &&
           i < 31)
    {
        command[i] = rx_buffer[i];
        i++;
    }

    command[i] = '\0';

    command_ready = 0;


    __enable_irq();


    /* ON */

    if (StringEqual(command, "ON"))
    {
        LED_ON();

        USART1_SendString("LED ON\r\n");
    }


    /* OFF */

    else if (StringEqual(command, "OFF"))
    {
        LED_OFF();

        USART1_SendString("LED OFF\r\n");
    }


    /* STATUS */

    else if (StringEqual(command, "Status"))
    {
        USART1_SendString("LED: ");

        if (led_on)
            USART1_SendString("ON\r\n");
        else
            USART1_SendString("OFF\r\n");

        USART1_SendString("PWM: ");

        SendNumber(pwm_percent);

        USART1_SendString("%\r\n");
    }


    /* PWM */

    else if (command[0] == 'P' &&
             command[1] == 'W' &&
             command[2] == 'M' &&
             command[3] == ':')
    {
        ProcessPWM(command);
    }


    /* UNKNOWN */

    else
    {
        USART1_SendString("UNKNOWN COMMAND\r\n");
    }
}


/* ================= MAIN ================= */

int main(void)
{
    PWM_Init();

    USART1_Init();

    LED_OFF();


    USART1_SendString(
        "\r\nSTM32 PWM UART READY\r\n"
    );


    while (1)
    {
        if (command_ready)
        {
            ProcessCommand();
        }
    }
}
