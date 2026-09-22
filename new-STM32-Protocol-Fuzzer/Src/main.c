#include "uart.h"
#include "delay.h"
#include "fuzzer.h"
#include "protocol.h"

int main()
{
    uart_init();
    protocol_init();
//    fuzzer_run();
//    uart_send_string("UART ECHO READY\r\n");
    uart_send_string("PROTOCOL PARSER READY\r\n");
//    uint8_t byte = (uint8_t)uart_receive_char();
//    protocol_process_byte(byte);
//        		uint8_t test_frame[] = {
//        		    0xAA,
//        		    0x03,
//        		    0x01,
//        		    0x10,
//        		    0x20,
//        		    0x30
//        		};
//
//        		for (int i = 0; i < 6; i++)
//        		{
//        		    protocol_process_byte(test_frame[i]);
//        		}
//        		uart_send_string("TEST COMPLETE\r\n");
        		while (1)
        		{
        		    uint8_t byte = (uint8_t)uart_receive_char();
        		    protocol_process_byte(byte);
        		}
}
