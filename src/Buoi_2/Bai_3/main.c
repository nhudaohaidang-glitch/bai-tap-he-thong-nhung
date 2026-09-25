#include "stm32f10x.h"
#include <stdint.h>

/* =========================
   SysTick
   HCLK = 8 MHz
   ========================= */

void SysTick_Init(void)
{
    SysTick->LOAD = 8000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0);
}

void delay_ms(uint32_t ms)
{
    for (uint32_t i = 0; i < ms; i++)
    {
        SysTick->VAL = 0;

        while ((SysTick->CTRL & (1 << 16)) == 0);
    }
}


/* =========================
   UART1
   PA9  -> TX
   PA10 -> RX
   9600 baud
   PCLK2 = 8 MHz
   ========================= */

void UART1_Init(void)
{
    RCC->APB2ENR |= (1 << 2);
    RCC->APB2ENR |= (1 << 14);

    /* PA9 = USART1_TX */
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);

    /* PA10 = USART1_RX */
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);

    /* 8 MHz / 9600 baud */
    USART1->BRR = 0x0341;

    /* USART enable + TX enable */
    USART1->CR1 = (1 << 13) | (1 << 3);
}

void UART1_SendChar(char c)
{
    while ((USART1->SR & (1 << 7)) == 0);

    USART1->DR = c;
}

void UART1_SendString(const char *str)
{
    while (*str)
    {
        UART1_SendChar(*str);
        str++;
    }
}

void UART1_SendNumber(uint32_t number)
{
    char buffer[10];
    int i = 0;

    if (number == 0)
    {
        UART1_SendChar('0');
        return;
    }

    while (number > 0)
    {
        buffer[i++] = (number % 10) + '0';
        number /= 10;
    }

    while (i > 0)
    {
        UART1_SendChar(buffer[--i]);
    }
}


/* =========================
   ADC1
   PA0 = ADC1_IN0
   ========================= */

void ADC1_Init(void)
{
    /* GPIOA clock */
    RCC->APB2ENR |= (1 << 2);

    /* ADC1 clock */
    RCC->APB2ENR |= (1 << 9);

    /* PA0 = Analog input */
    GPIOA->CRL &= ~(0xF << 0);

    /*
       ADC clock = PCLK2 / 6
       8 MHz / 6 = 1.333 MHz
    */
    RCC->CFGR &= ~(3 << 14);
    RCC->CFGR |=  (2 << 14);

    /* Channel 0 sample time = 55.5 cycles */
    ADC1->SMPR2 &= ~(7 << 0);
    ADC1->SMPR2 |=  (5 << 0);

    /* 1 regular conversion */
    ADC1->SQR1 &= ~(0xF << 20);

    /* First conversion = channel 0 */
    ADC1->SQR3 &= ~(0x1F << 0);

    /*
       CR2:
       EXTSEL bits 19:17 = 111
       -> SWSTART
    */
    ADC1->CR2 &= ~(7 << 17);
    ADC1->CR2 |=  (7 << 17);

    /*
       EXTTRIG = bit 20
    */
    ADC1->CR2 |= (1 << 20);

    /* Enable ADC */
    ADC1->CR2 |= (1 << 0);

    delay_ms(1);

    /* Reset calibration */
    ADC1->CR2 |= (1 << 3);

    while (ADC1->CR2 & (1 << 3));

    /* Start calibration */
    ADC1->CR2 |= (1 << 2);

    while (ADC1->CR2 & (1 << 2));
}


/* =========================
   ADC READ
   ========================= */

uint16_t ADC1_Read(void)
{
    /* Clear old EOC */
    ADC1->SR &= ~(1 << 1);

    /* Start conversion */
    ADC1->CR2 |= (1 << 22);

    /* Wait for EOC */
    while ((ADC1->SR & (1 << 1)) == 0);

    /* Return ADC value */
    return (uint16_t)ADC1->DR;
}


/* =========================
   MAIN
   ========================= */

int main(void)
{
    uint16_t adc_value;
    uint32_t voltage_mV;

    SysTick_Init();

    UART1_Init();

    UART1_SendString("\r\n");
    UART1_SendString("===========================\r\n");
    UART1_SendString("   STM32 ADC TEST\r\n");
    UART1_SendString("===========================\r\n");
    UART1_SendString("PA0 -> ADC1_IN0\r\n");
    UART1_SendString("USART1: 9600 baud\r\n");

    UART1_SendString("ADC initializing...\r\n");

    ADC1_Init();

    UART1_SendString("ADC OK!\r\n");
    UART1_SendString("Starting conversion...\r\n");

    while (1)
    {
        adc_value = ADC1_Read();

        /*
           Voltage(mV)
           = ADC * 3300 / 4095
        */
        voltage_mV =
            ((uint32_t)adc_value * 3300) / 4095;

        UART1_SendString("ADC = ");
        UART1_SendNumber(adc_value);

        UART1_SendString("    Voltage = ");

        UART1_SendNumber(voltage_mV / 1000);

        UART1_SendChar('.');

        if ((voltage_mV % 1000) < 100)
            UART1_SendChar('0');

        if ((voltage_mV % 1000) < 10)
            UART1_SendChar('0');

        UART1_SendNumber(voltage_mV % 1000);

        UART1_SendString(" V\r\n");

        delay_ms(1000);
    }
}


/* =========================
   UART IRQ
   ========================= */

void USART1_IRQHandler(void)
{
}
