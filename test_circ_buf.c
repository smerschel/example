#include "circ_buf.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_put_get(void) {
    CircBuf buf;
    u8 data;
    ReturnCode result;
    u8 buffer[3];  // Pre-allocated buffer for CIR_Init

    // Initialize buffer
    result = CIR_Init(&buf, buffer, 3);
    assert(result == SUCCESS);

    // Test NULL parameters
    result = CIR_Put(NULL, 1);
    assert(result == ERROR_INVALID_PARAMETER);
    result = CIR_Get(NULL, &data);
    assert(result == ERROR_INVALID_PARAMETER);
    result = CIR_Get(&buf, NULL);
    assert(result == ERROR_INVALID_PARAMETER);

    // Test empty buffer get
    result = CIR_Get(&buf, &data);
    assert(result == ERROR_BUFFER_UNDERFLOW);

    // Test normal put/get
    result = CIR_Put(&buf, 1);
    assert(result == SUCCESS);
    assert(buf.count == 1);
    result = CIR_Put(&buf, 2);
    assert(result == SUCCESS);
    assert(buf.count == 2);
    result = CIR_Put(&buf, 3);
    assert(result == SUCCESS);
    assert(buf.count == 3);

    // Test overflow
    result = CIR_Put(&buf, 4);
    assert(result == SUCCESS);
    assert(buf.count == 3);
    assert(buf.dropped_bytes == 1);

    // Test get
    result = CIR_Get(&buf, &data);
    assert(result == SUCCESS);
    assert(data == 2);  // 1 was dropped
    assert(buf.count == 2);

}

void test_count_and_status(void) {
    CircBuf buf;
    ReturnCode result;
    u8 buffer[3];  // Pre-allocated buffer for CIR_Init

    result = CIR_Init(&buf, buffer, 3);
    assert(result == SUCCESS);

    // Test empty buffer
    assert(CIR_IsEmpty(&buf));
    assert(!CIR_IsFull(&buf));
    assert(CIR_GetCount(&buf) == 0);
    assert(CIR_GetDroppedBytes(&buf) == 0);

    // Fill buffer
    CIR_Put(&buf, 1);
    assert(!CIR_IsEmpty(&buf));
    assert(!CIR_IsFull(&buf));
    assert(CIR_GetCount(&buf) == 1);

    CIR_Put(&buf, 2);
    CIR_Put(&buf, 3);
    assert(CIR_IsFull(&buf));
    assert(CIR_GetCount(&buf) == 3);

    // Test overflow
    CIR_Put(&buf, 4);
    assert(CIR_IsFull(&buf));
    assert(CIR_GetCount(&buf) == 3);
    assert(CIR_GetDroppedBytes(&buf) == 1);

}

int main(void) {
    printf("Running circular buffer tests...\n");
    
    test_init_deinit();
    test_put_get();
    test_count_and_status();
    
    printf("All circular buffer tests passed!\n");
    return 0;
} 