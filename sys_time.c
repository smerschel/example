#include "sys_time.h"
#include <stdio.h>
#include <time.h>

static bool is_initialized = false;
static u32 tick_count = 0;

ReturnCode TIM_Init(void) {
    if (is_initialized) {
        return SUCCESS;
    }
    is_initialized = true;
    tick_count = 0;
    return SUCCESS;
}

u32 TIM_GetTicks(void) {
    if (!is_initialized) {
        return 0;
    }
    tick_count++;
    return tick_count;
}

u32 TIM_GetMs(void) {
    if (!is_initialized) {
        return 0;
    }
    return tick_count * 10;  // Assuming 10ms per tick
}

#ifdef PLATFORM_BAREMETAL
// This function would be called by the timer interrupt handler
void TIME_Tick(void) {
    tick_count++;
}
#endif 