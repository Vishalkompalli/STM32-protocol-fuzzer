#include "uart.h"
#include <stdint.h>
#define USART1_BASE 0x40013800
#define RCC_BASE    0x40021000
#define GPIOA_BASE  0x40010800

#define USART1_SR   (*(volatile uint32_t*)(USART1_BASE + 0x00))
#define USART1_DR   (*(volatile uint32_t*)(USART1_BASE + 0x04))
#define USART1_BRR  (*(volatile uint32_t*)(USART1_BASE + 0x08))
#define USART1_CR1  (*(volatile uint32_t*)(USART1_BASE + 0x0C))

#define RCC_APB2ENR (*(volatile uint32_t*)(RCC_BASE + 0x18))
#define GPIOA_CRH   (*(volatile uint32_t*)(GPIOA_BASE + 0x04))

void uart_init()
{
    RCC_APB2ENR |= (1 << 2) | (1 << 14); // Enable GPIOA and USART1 clocks
    GPIOA_CRH &= ~(0xF << 4);            // Clear PA9 config
    GPIOA_CRH |=  (0xB << 4);            // PA9: AF push-pull, 50 MHz

    //USART1_BRR = 0x1D4C;                 // 72MHz / 9600 baud
    USART1_BRR = 0x341;  // 8 MHz / 9600 = 834 decimal = 0x341
    //USART1_BRR = 0x112; // For 115200 baud
    //USART1_BRR = 0x24E8;  // For 115200 baud with a 72 MHz system clock



    USART1_CR1 = (1 << 13) | (1 << 3);   // UE, TE
}

void uart_send_char(char c)
{
    while (!(USART1_SR & (1 << 7)));     // Wait until TXE
    USART1_DR = c;
}

void uart_send_string(const char* s)
{
    while (*s)
    {
        uart_send_char(*s++);
    }
}
