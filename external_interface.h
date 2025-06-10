#ifndef EXTERNAL_INTERFACE_H
#define EXTERNAL_INTERFACE_H

#include "types.h"
#include "circ_buf.h"

// Socket configuration
#define EXT_TX_PORT 12345
#define EXT_RX_PORT 12346
#define EXT_BUFFER_SIZE 1024

// Initialize the external interface
ReturnCode EXT_Init(void);

// Send data through the external interface
ReturnCode EXT_Send(const u8* data, u32 length);

// Receive data from the external interface
i32 EXT_Recv(u8* buffer, u32 max_length);

// Task function to handle socket communication
void EXT_Task(void);

#endif // EXTERNAL_INTERFACE_H 