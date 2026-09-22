#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

void protocol_init(void);
void protocol_process_byte(uint8_t byte);

#endif
