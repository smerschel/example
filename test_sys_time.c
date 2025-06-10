#include "sys_time.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_init(void) {
    ReturnCode result = TIM_Init();
    assert(result == SUCCESS);
    assert(TIM_Init() == SUCCESS);  // Should return SUCCESS if already initialized
}

void test_get_ticks(void) {
    u32 initial_ticks = TIM_GetTicks();
    assert(initial_ticks >= 0);
    u32 later_ticks = TIM_GetTicks();
    assert(later_ticks > initial_ticks);
}

void test_get_ms(void) {
    u32 ms = TIM_GetMs();
    assert(ms >= 0);
    assert(ms % 10 == 0);  // Assuming 10ms per tick
}

int main(void) {
    printf("Running system time tests...\n");
    
    test_init();
    test_get_ticks();
    test_get_ms();
    
    printf("All system time tests passed!\n");
    return 0;
} 