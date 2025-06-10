#include "external_interface.h"
#include "command_manager.h"
#include "telemetry_manager.h"
#include "cvt.h"
#include "sys_time.h"
#include <stddef.h>

extern CVT cvt;

int main(void) {
    ReturnCode result;

    // Initialize all components
    result = TIM_Init();
    if (result != SUCCESS) {
        return -1;
    }

    result = EXT_Init();
    if (result != SUCCESS) {
        return -1;
    }

    result = CMD_Init();  // No default handler for now
    if (result != SUCCESS) {
        return -1;
    }

    result = TLM_Init();
    if (result != SUCCESS) {
        return -1;
    }

    // Main loop
    while (1) {
        // Handle external interface communication
        EXT_Task();

        // Process commands
        result = CMD_Task();
        if (result != SUCCESS) {
            cvt.task_error_count.value++;
            cvt.task_error_count.timestamp = TIM_GetTicks();
        }
    }

    return 0;
} 