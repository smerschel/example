#ifndef CIRC_BUF_H
#define CIRC_BUF_H

#include "types.h"
#include "bare_mutex.h"

// Circular buffer structure
typedef struct {
    u8* buffer;           // Pointer to buffer
    u32 size;            // Size of buffer
    u32 head;            // Head index
    u32 tail;            // Tail index
    u32 count;           // Number of bytes in buffer
    u32 dropped_bytes;   // Number of bytes dropped due to buffer full
    BareMutex mutex;     // Mutex for thread safety
} CircBuf;

// Initialize circular buffer with pre-allocated buffer
ReturnCode CIR_Init(CircBuf* buf, u8* buffer, u32 size);

// Put a byte into the buffer
ReturnCode CIR_Put(CircBuf* buf, u8 data);

// Get a byte from the buffer
ReturnCode CIR_Get(CircBuf* buf, u8* data);

// Get number of bytes in buffer
u32 CIR_GetCount(CircBuf* buf);

// Get number of dropped bytes
u32 CIR_GetDroppedBytes(CircBuf* buf);

// Check if buffer is empty
bool CIR_IsEmpty(CircBuf* buf);

// Check if buffer is full
bool CIR_IsFull(CircBuf* buf);

#endif // CIRC_BUF_H 