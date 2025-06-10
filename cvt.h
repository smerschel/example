#ifndef CVT_H
#define CVT_H

#include "types.h"

// Current Value Table structure
typedef struct {
    TimedU32 dropped_bytes;
    TimedU32 task_error_count;
    TimedU32 cmd_success_count;
    TimedU32 cmd_failure_count;
} CVT;

// Initialize the CVT
// Returns true if successful, false otherwise
bool CVT_Init(void);

#endif // CVT_H 