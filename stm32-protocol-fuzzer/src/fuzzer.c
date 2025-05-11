#include "fuzzer.h"
#include "uart.h"
#include "delay.h"
#include <stdint.h>

static uint8_t random_byte() {
    // Dirty pseudo-random generator using a counter + shift (good enough for now)
    static uint32_t seed = 0x12345678;
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return (uint8_t)(seed & 0xFF);
}

void fuzzer_run() {
    uart_send_string("FUZZING STARTED\r\n");

    while (1) {
        uart_send_string("Payload: ");
        for (int i = 0; i < 16; i++) {
            uint8_t byte = random_byte();
            uart_send_char(byte);   // Send actual garbage byte
        }
        uart_send_string("\r\n");

        delay_ms(1000);
    }
}
