#include "command_manager.h"
#include "external_interface.h"
#include "sys_time.h"
#include "cvt.h"
#include "protocol.h"
#include <string.h>
#include <stddef.h>

#define CMD_BUFFER_SIZE 1024
#define RESP_BUFFER_SIZE 1024
#define MAX_HANDLERS 256

static bool is_initialized = false;

// Command handler table
static CommandHandlerInfo handlers[MAX_HANDLERS];

bool CMD_Init() {
    if (is_initialized) {
        return true;  // Already initialized
    }
    for (u32 i = 0; i < MAX_HANDLERS; i++) {
        handlers[i].handler = NULL;
        handlers[i].needs_ack = false;
    }
    
    is_initialized = true;
    return true;
}

ReturnCode CMD_RegisterHandler(u8 cmd_id, CmdHandler handler, bool needs_ack) {
    if (!is_initialized) {
        return ERROR_UNKNOWN;
    }
    
    handlers[cmd_id].handler = handler;
    handlers[cmd_id].needs_ack = needs_ack;
    
    return SUCCESS;
}

ReturnCode CMD_Task(void) {
    if (!is_initialized) {
        return ERROR_UNKNOWN;
    }

    ReturnCode result = ERROR_UNKNOWN;

    // Now try to get a command from the external interface buffer
    u8** cmd_buffer_ptr = NULL;
    i32 cmd_length = PRT_GetCommand(EXT_GetRecvBuffer(), cmd_buffer_ptr);
    
    if (cmd_length > 0) {
        // Command found, process it
        u8 cmd_id = PRT_GetCommandId(*cmd_buffer_ptr);
        
        // Look for registered handler
        if (handlers[cmd_id].handler != NULL) {
            // Call handler with payload
            result = handlers[cmd_id].handler(*cmd_buffer_ptr, 
                ((PacketHeader*)(*cmd_buffer_ptr))->length);
            
            // Send ACK/NAK if needed
            if (handlers[cmd_id].needs_ack) {
                i32 resp_length = 0;
                if(result == SUCCESS) {
                    resp_length = PRT_MakeAck(cmd_id, resp_buffer);
                } else {
                    resp_length = PRT_MakeNak(cmd_id, resp_buffer);
                }
                if (resp_length > 0) {
                    EXT_Send(resp_buffer, resp_length);
                }
            }
        }
        
        // Update CVT counters
        if (result == SUCCESS) {
            cvt.cmd_success_count.value++;
            cvt.cmd_success_count.last_update_time = TIM_GetTicks();
        } else {
            cvt.cmd_failure_count.value++;
            cvt.cmd_failure_count.last_update_time = TIM_GetTicks();
        }
    }
    return result;
}
