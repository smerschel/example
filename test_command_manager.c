#include "command_manager.h"
#include "external_interface.h"
#include "sys_time.h"
#include "cvt.h"
#include "protocol.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

// Mock command handler for testing
static ReturnCode mock_cmd_handler(u8* payload, u32 length) {
    printf("Mock command handler called with length %d\n", length);
    return SUCCESS;
}

void test_init(void) {
    assert(CMD_Init() == true);
    assert(CMD_Init() == true);  // Should return true if already initialized
}

void test_register_handler(void) {
    ReturnCode result = CMD_RegisterHandler(1, mock_cmd_handler, true);
    assert(result == SUCCESS);
}

void test_process_command(void) {
    // Simulate a command in the external interface buffer
    u8 cmd_buffer[1024];
    PacketHeader* header = (PacketHeader*)cmd_buffer;
    header->sys_id = SYSTEM_ID;
    header->cmd_id = 1;
    header->length = 0;
    cmd_buffer[PACKET_HEADER_SIZE] = Packet_CalculateCRC8(&cmd_buffer[PACKET_HEADER_SIZE], 0);

    // Set the external interface buffer
    EXT_SetRecvBuffer(cmd_buffer);

    // Process the command
    CMD_Task();

    // Verify CVT counters
    assert(cvt.cmd_success_count.value > 0);
    assert(cvt.cmd_failure_count.value == 0);
}

int main(void) {
    printf("Running command manager tests...\n");
    
    test_init();
    test_register_handler();
    test_process_command();
    
    printf("All command manager tests passed!\n");
    return 0;
} 