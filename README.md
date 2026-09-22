# STM32 UART Protocol Fuzzer

A host-controlled fuzzing setup for testing a custom binary UART protocol on an STM32F103C8T6(Bluepill).

The STM32 acts as the **System Under Test (SUT)**, while a Python script running on the host generates test cases, sends them over UART, validates responses, and records unexpected behavior.

## Architecture

```text
                    STM32 Protocol Fuzzer
                             │
             ┌───────────────┴───────────────┐
             │                               │
         STM32 SUT                     Python Fuzzer
             │                               │
      Protocol FSM                     Test generation
             │                               │
        UART TX/RX  ◄──── PL2303 ────►  Classification
             │                               │
        Responses                         Logging
                                             │
                                          failures/
````

### Hardware

```text
Windows PC
    │
  USB
    │
  PL2303
    │
    ├──── TXD ─────► PA10 (USART1 RX)
    ├──── RXD ◄───── PA9  (USART1 TX)
    └──── GND ────── GND
                       │
                 STM32F103C8T6
```

## Protocol

```text
┌─────────┬────────┬─────────┬──────────────┐
│  START  │ LENGTH │ COMMAND │     DATA     │
│  0xAA   │ 1 byte │ 1 byte  │   0-32 bytes │
└─────────┴────────┴─────────┴──────────────┘
```

Commands:

| Command | Description     |
| ------- | --------------- |
| `0x01`  | ECHO            |
| `0x02`  | STATUS          |
| Other   | Unknown command |

Example:

```text
AA 03 01 10 20 30
```

Response:

```text
ECHO: 10 20 30
```

## Fuzzing

The Python controller tests:

* Valid frames
* Unknown commands
* Invalid lengths
* Invalid start bytes
* Mutated data
* Boundary conditions
* Truncated frames

Unexpected responses and timeouts are saved in `failures/` for reproduction.

## Results

Final fuzz campaign:

```text
Total tests:        1000
Passes:              408
Expected errors:     404
Expected timeouts:   188
Unexpected timeouts:   0
Unexpected output:     0
Failures saved:        0
```

All 1,000 test cases produced the expected classification for the defined test campaign.

## Project Structure

```text
STM32-protocol-fuzzer/
│
├── firmware/       # STM32 protocol implementation
├── fuzzer/         # Python fuzzing controller
├── tests/          # Test cases and vectors
├── docs/           # Project documentation
├── screenshots/    # Test/setup screenshots
│
├── README.md
├── .gitignore
└── LICENSE
```

## Technologies

* STM32F103C8T6
* Embedded C
* UART / USART
* Finite State Machine
* Python
* PySerial
* STM32CubeIDE
* PL2303 USB-to-TTL

## Status

**Implementation Phase: finished**

```