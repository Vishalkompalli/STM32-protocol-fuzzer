# Embedded UART Protocol Fuzzer for Robustness Testing

I built a bare-metal embedded fuzzing tool on the STM32F103 (Blue Pill) that stress-tests UART-based communication protocols by injecting malformed, random, and edge-case payloads into a target system. The goal is to simulate real-world communication faults and evaluate the resilience of embedded systems under unpredictable input conditions.

<pre>
                      
      [Fuzzer: STM32]   
            |
            |      (generate weird payloads)   
            |   
[UART Peripheral: PA9/PA10]   
            |   
            |      (sends bytes at 9600 baud)   
            |   
[USB-to-Serial (PL2303)]   
            |   
            |      (converts UART to USB)   
            |   
      [Linux Host]   
            |   
            |      (Python reads /dev/ttyUSB0)   
            |   
[Logger: timestamps + logs output]   
</pre>
