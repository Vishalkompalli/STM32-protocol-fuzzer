# Embedded UART Protocol Fuzzer for Robustness Testing

## Overview
This repository contains a bare-metal embedded fuzzing tool built on the **STM32F103 (Blue Pill)** microcontroller. The primary goal of this project is to perform robustness and resilience testing on embedded systems that communicate via UART protocols. By injecting a variety of malformed, random, and edge-case payloads, the fuzzer simulates real-world communication faults, helping to identify potential vulnerabilities and weaknesses in a target system's protocol handler.

## How It Works
The system operates by having the STM32 microcontroller generate a stream of unpredictable data payloads. This data is transmitted via its UART peripheral at a specified baud rate (e.g., 9600 baud) and is then logged on a host machine for analysis. The typical setup involves a simple hardware chain:

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

## Getting Started

### Hardware Requirements
* **STM32F103C8T6** (Blue Pill) development board
* **USB-to-Serial** converter (e.g., PL2303)
* **Breadboard and jumper wires** for connections

### Software Requirements
* **Linux Host Machine** (Ubuntu recommended)
* **GNU ARM Embedded Toolchain**: for compiling the C code for the STM32.
* **st-link**: for flashing the compiled firmware onto the STM32.
* **Python 3**: for the logging script on the host machine.
* **pyserial**: Python library for serial communication. Install with `pip install pyserial`.

### Building and Flashing the Firmware
1.  Navigate to the `stm32-protocol-fuzzer` directory.
2.  Run `make` to compile the C source code.
3.  Flash the generated `.hex` or `.bin` file to the STM32 using `st-link`.

### Running the Fuzzer
1.  Connect the STM32 to the Linux host via the USB-to-Serial converter.
2.  Ensure the converter is recognized as `/dev/ttyUSB0`. If not, adjust the Python script accordingly.
3.  Run the Python logger script to begin capturing the fuzzer's output.

## Repository Structure
* `stm32-protocol-fuzzer/`: C source files for the embedded fuzzer.
* `logger/`: Python script for capturing and logging data on the host.
* `Makefile`: Build script for compiling the embedded firmware.

## License
This project is licensed under the MIT License - see the `LICENSE` file for details.
