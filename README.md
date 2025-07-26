# STM32_Projects
This is my personal repo where I build projects using STM32F103C8T6 (Bluepill) 

**1. Embedded UART Protocol Fuzzer for Robustness Testing**
I built a bare-metal embedded fuzzing tool on the STM32F103 (Blue Pill) that stress-tests UART-based communication protocols by injecting malformed, random, and edge-case payloads into a target system. The goal is to simulate real-world communication faults and evaluate the resilience of embedded systems under unpredictable input conditions.

      (generate weird payloads)         (sends bytes at 9600 baud)         (converts UART to USB)   (Python reads /dev/ttyUSB0)
[Fuzzer: STM32] ---> [UART Peripheral: PA9/PA10] ---> [USB-to-Serial (PL2303)] ---> [Linux Host] ---> [Logger: timestamps + logs output]
