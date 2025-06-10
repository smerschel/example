#ifndef SYS_TIME_H
#define SYS_TIME_H

#include "types.h"

// Initialize the system time
ReturnCode TIM_Init(void);

// Get the current tick count
u32 TIM_GetTicks(void);

// Get the current time in milliseconds
u32 TIM_GetMs(void);

#endif // SYS_TIME_H 