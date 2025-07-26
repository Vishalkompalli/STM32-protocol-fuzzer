#include "uart.h"
#include "delay.h"
#include "fuzzer.h"

int main()
{
    uart_init();
    fuzzer_run();

    /*while (1) {
        uart_send_string("Fuzzy ready...\r\n");
        delay_ms(2000);
    }*/
}

/*#include <stdint.h>

#define RCC_APB2ENR (*(volatile uint32_t*)(0x40021018))
#define GPIOA_CRH   (*(volatile uint32_t*)(0x40010804))
#define GPIOA_ODR   (*(volatile uint32_t*)(0x4001080C))

void delay(volatile uint32_t t) {
    while (t--);
}

int main() {
    RCC_APB2ENR |= (1 << 2);        // Enable GPIOA clock
    GPIOA_CRH &= ~(0xF << 4);       // Clear PA9 config
    GPIOA_CRH |=  (0x1 << 4);       // PA9 as push-pull output

    while (1) {
        GPIOA_ODR ^= (1 << 9);      // Toggle PA9
        delay(500000);
    }
}*/

