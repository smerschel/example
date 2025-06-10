#include "circ_buf.h"
#include <string.h>

ReturnCode CIR_Init(CircBuf* buf, u8* buffer, u32 size) {
    if (buf == NULL || buffer == NULL || size == 0) {
        return ERROR_INVALID_PARAMETER;
    }
    
    buf->buffer = buffer;
    buf->size = size;
    buf->head = 0;
    buf->tail = 0;
    buf->count = 0;
    buf->dropped_bytes = 0;
    
    // Initialize mutex
    BareMutex_Init(&buf->mutex);
    
    return SUCCESS;
}

ReturnCode CIR_Put(CircBuf* buf, u8 data) {
    if (buf == NULL) {
        return ERROR_INVALID_PARAMETER;
    }
    
    BareMutex_Lock(&buf->mutex);
    
    if (CIR_IsFull(buf)) {
        // Buffer is full, drop oldest byte
        buf->tail = (buf->tail + 1) % buf->size;
        buf->dropped_bytes++;
    } else {
        buf->count++;
    }
    
    buf->buffer[buf->head] = data;
    buf->head = (buf->head + 1) % buf->size;
    
    BareMutex_Unlock(&buf->mutex);
    return SUCCESS;
}

ReturnCode CIR_Get(CircBuf* buf, u8* data) {
    if (buf == NULL || data == NULL) {
        return ERROR_INVALID_PARAMETER;
    }
    
    BareMutex_Lock(&buf->mutex);
    
    if (CIR_IsEmpty(buf)) {
        BareMutex_Unlock(&buf->mutex);
        return ERROR_BUFFER_UNDERFLOW;
    }
    
    *data = buf->buffer[buf->tail];
    buf->tail = (buf->tail + 1) % buf->size;
    buf->count--;
    
    BareMutex_Unlock(&buf->mutex);
    return SUCCESS;
}

u32 CIR_GetCount(CircBuf* buf) {
    if (buf == NULL) {
        return 0;
    }
    
    BareMutex_Lock(&buf->mutex);
    u32 count = buf->count;
    BareMutex_Unlock(&buf->mutex);
    
    return count;
}

u32 CIR_GetDroppedBytes(CircBuf* buf) {
    if (buf == NULL) {
        return 0;
    }
    
    BareMutex_Lock(&buf->mutex);
    u32 dropped = buf->dropped_bytes;
    BareMutex_Unlock(&buf->mutex);
    
    return dropped;
}

bool CIR_IsEmpty(CircBuf* buf) {
    if (buf == NULL) {
        return true;
    }
    
    BareMutex_Lock(&buf->mutex);
    bool empty = (buf->count == 0);
    BareMutex_Unlock(&buf->mutex);
    
    return empty;
}

bool CIR_IsFull(CircBuf* buf) {
    if (buf == NULL) {
        return true;
    }
    
    BareMutex_Lock(&buf->mutex);
    bool full = (buf->count == buf->size);
    BareMutex_Unlock(&buf->mutex);
    
    return full;
} 