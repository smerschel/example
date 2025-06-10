#ifndef BARE_MUTEX_H
#define BARE_MUTEX_H

#include "types.h"

// Bare metal mutex structure
typedef struct {
    volatile bool locked;
} BareMutex;

// Initialize the mutex
// Returns SUCCESS or ERROR_INVALID_PARAMETER
ReturnCode BM_Init(BareMutex* mutex);

// Lock the mutex
// Returns SUCCESS or ERROR_INVALID_PARAMETER
ReturnCode BM_Lock(BareMutex* mutex);

// Unlock the mutex
// Returns SUCCESS or ERROR_INVALID_PARAMETER
ReturnCode BM_Unlock(BareMutex* mutex);

#endif // BARE_MUTEX_H 