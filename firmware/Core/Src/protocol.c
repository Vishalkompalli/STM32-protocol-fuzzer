#include "protocol.h"
#include "uart.h"

#define FRAME_START       0xAA
#define MAX_DATA_LENGTH   32

typedef enum
{
    WAIT_START,
    READ_LENGTH,
    READ_COMMAND,
    READ_DATA
} parser_state_t;

static parser_state_t state;

static uint8_t length;
static uint8_t command;
static uint8_t data[MAX_DATA_LENGTH];
static uint8_t data_index;


void protocol_init(void)
{
    state = WAIT_START;
    length = 0;
    command = 0;
    data_index = 0;
}


static void reset_parser(void)
{
    state = WAIT_START;
    length = 0;
    command = 0;
    data_index = 0;
}


static void send_error(const char *message)
{
    uart_send_string("ERROR: ");
    uart_send_string(message);
    uart_send_string("\r\n");
}

static void send_hex(uint8_t byte)
	{
	    const char hex[] = "0123456789ABCDEF";

	    uart_send_char(hex[(byte >> 4) & 0x0F]);
	    uart_send_char(hex[byte & 0x0F]);
	}

static void process_frame(void)
{

    if (command == 0x01)
    {
        // ECHO command
        uart_send_string("ECHO: ");

        for (uint8_t i = 0; i < length; i++)
        {
//            uart_send_char(data[i]);
        	send_hex(data[i]);
        	uart_send_char(' ');
        }

        uart_send_string("\r\n");
    }
    else if (command == 0x02)
    {
        // STATUS command
        uart_send_string("STATUS: OK\r\n");
    }
    else
    {
        send_error("UNKNOWN COMMAND");
    }
}


void protocol_process_byte(uint8_t byte)
{
    switch (state)
    {
        case WAIT_START:

            if (byte == FRAME_START)
            {
                state = READ_LENGTH;
            }

            break;


        case READ_LENGTH:

            length = byte;

            if (length > MAX_DATA_LENGTH)
            {
                send_error("LENGTH TOO LARGE");
                reset_parser();
            }
            else
            {
                state = READ_COMMAND;
            }

            break;


        case READ_COMMAND:

            command = byte;
            data_index = 0;

            if (length == 0)
            {
                process_frame();
                reset_parser();
            }
            else
            {
                state = READ_DATA;
            }

            break;


        case READ_DATA:

            data[data_index] = byte;
            data_index++;

            if (data_index >= length)
            {
                process_frame();
                reset_parser();
            }

            break;


        default:

            reset_parser();

            break;
    }
}
