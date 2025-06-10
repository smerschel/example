#include "cvt.h"
#include "sys_time.h"
#include <string.h>

static CVT cvt;

bool CVT_Init(void) {
    cvt.dropped_bytes.value = 0;
    cvt.dropped_bytes.timestamp = TIM_GetTicks();
    cvt.task_error_count.value = 0;
    cvt.task_error_count.timestamp = TIM_GetTicks();
    cvt.cmd_success_count.value = 0;
    cvt.cmd_success_count.timestamp = TIM_GetTicks();
    cvt.cmd_failure_count.value = 0;
    cvt.cmd_failure_count.timestamp = TIM_GetTicks();    
    return true;
}
