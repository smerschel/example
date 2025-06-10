#include "external_interface.h"
#include <stdio.h>
#include <assert.h>

#define BUFFER_SIZE 32

void test_null_parameters(void) {
    i32 result;
    
    // Test NULL buffer for EXT_Recv
    result = EXT_Recv(NULL, BUFFER_SIZE);
    assert(result == ERROR_INVALID_PARAMETER);
    
    // Test NULL buffer for EXT_Send
    result = EXT_Send(NULL, BUFFER_SIZE);
    assert(result == ERROR_INVALID_PARAMETER);
    
    // Test zero size for EXT_Recv
    u8 buffer[BUFFER_SIZE];
    result = EXT_Recv(buffer, 0);
    assert(result == ERROR_INVALID_PARAMETER);
    
    // Test zero size for EXT_Send
    result = EXT_Send(buffer, 0);
    assert(result == ERROR_INVALID_PARAMETER);
}

void test_data_transfer(void) {
    u8 buffer[BUFFER_SIZE];
    i32 result;
    
    // Test receiving data
    result = EXT_Recv(buffer, BUFFER_SIZE);
    assert(result > 0);
    assert(result <= BUFFER_SIZE);
    
    // Verify received data pattern
    for (i32 i = 0; i < result; i++) {
        assert(buffer[i] == (u8)(0xAA + i));
    }
    
    // Test sending data
    result = EXT_Send(buffer, result);
    assert(result > 0);
}

int main(void) {
    printf("Running tests...\n");
    
    test_null_parameters();
    test_data_transfer();
    
    printf("All tests passed!\n");
    return 0;
} 