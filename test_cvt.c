#include "cvt.h"
#include "sys_time.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_init(void) {
    assert(CVT_Init() == true);
    assert(cvt.dropped_bytes.value == 0);
    assert(cvt.task_error_count.value == 0);
    assert(cvt.cmd_success_count.value == 0);
    assert(cvt.cmd_failure_count.value == 0);
}

void test_update_dropped_bytes(void) {
    CVT_UpdateDroppedBytes(100);
    assert(cvt.dropped_bytes.value == 100);
}

void test_update_task_error_count(void) {
    CVT_UpdateTaskErrorCount(5);
    assert(cvt.task_error_count.value == 5);
}

void test_update_cmd_success_count(void) {
    CVT_UpdateCmdSuccessCount(10);
    assert(cvt.cmd_success_count.value == 10);
}

void test_update_cmd_failure_count(void) {
    CVT_UpdateCmdFailureCount(2);
    assert(cvt.cmd_failure_count.value == 2);
}

int main(void) {
    printf("Running CVT tests...\n");
    
    test_init();
    test_update_dropped_bytes();
    test_update_task_error_count();
    test_update_cmd_success_count();
    test_update_cmd_failure_count();
    
    printf("All CVT tests passed!\n");
    return 0;
} 