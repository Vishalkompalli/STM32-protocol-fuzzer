#include "fuzzer.h"
#include "uart.h"
#include "delay.h"
#include <stdint.h>

static uint32_t seed = 0xDEADBEEF;

static uint8_t random_byte() {
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    return (uint8_t)(seed & 0xFF);
}

static void send_payload(const char* label, uint8_t* data, int len) {
    uart_send_string(label);
    for (int i = 0; i < len; i++) {
        uart_send_char(data[i]);
    }
    uart_send_string("\r\n");
}

void fuzzer_run() {
    uart_send_string("FUZZING MODES ACTIVE\r\n");

    while (1) {
        uint8_t buffer[64];

        // Mode 1: Random Bytes
        for (int i = 0; i < 16; i++) buffer[i] = random_byte();
        send_payload("[RAND]: ", buffer, 16);
        delay_ms(500);

        // Mode 2: Bit-flip Fuzz
        for (int i = 0; i < 16; i++) buffer[i] = 0xFF ^ (1 << (i % 8));
        send_payload("[FLIP]: ", buffer, 16);
        delay_ms(500);

        // Mode 3: Overflow Payload
        for (int i = 0; i < 64; i++) buffer[i] = random_byte();
        send_payload("[OVER]: ", buffer, 64);
        delay_ms(500);

        // Mode 4: Null Spam
        for (int i = 0; i < 32; i++) buffer[i] = 0x00;
        send_payload("[NULL]: ", buffer, 32);
        delay_ms(500);
    }
}

