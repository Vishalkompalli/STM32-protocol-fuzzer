#include "delay.h"
#define SYST_CSR (*(volatile uint32_t*)0xE000E010)
#define SYST_RVR (*(volatile uint32_t*)0xE000E014)
#define SYST_CVR (*(volatile uint32_t*)0xE000E018)

void delay_ms(uint32_t ms) {
    SYST_RVR = 72000 - 1; // 1ms tick at 72MHz
    SYST_CVR = 0;
    SYST_CSR = 5;

    for (uint32_t i = 0; i < ms; i++) {
        while (!(SYST_CSR & (1 << 16)));
    }

    SYST_CSR = 0;
}
