#include "bare_mutex.h"
#include <stddef.h>

ReturnCode BM_Init(BareMutex* mutex) {
    if (mutex == NULL) {
        return ERROR_INVALID_PARAMETER;
    }
    mutex->locked = false;
    return SUCCESS;
}

ReturnCode BM_Lock(BareMutex* mutex) {
    if (mutex == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    // Simple spinlock implementation
    while (mutex->locked) {
        // Wait for mutex to be unlocked
        // In a real implementation, you might want to yield or sleep here
    }
    mutex->locked = true;
    return SUCCESS;
}

ReturnCode BM_Unlock(BareMutex* mutex) {
    if (mutex == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    mutex->locked = false;
    return SUCCESS;
} 