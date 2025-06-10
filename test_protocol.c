#include "protocol.h"
#include "system_defs.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_get_packet_size(void) {
    u32 size = PRT_GetPacketSize(10);
    assert(size == sizeof(PacketHeader) + 10 + sizeof(u8));
}

void test_get_command_id(void) {
    u8 cmd_buffer[1024];
    PacketHeader* header = (PacketHeader*)cmd_buffer;
    header->cmd_id = 42;
    u8 cmd_id = PRT_GetCommandId(cmd_buffer);
    assert(cmd_id == 42);
}

void test_make_ack(void) {
    u8* ack = PRT_MakeAck(42);
    assert(ack != NULL);
    PacketHeader* header = (PacketHeader*)ack;
    assert(header->sync == SYNC_WORD);
    assert(header->sys_id == SYSTEM_ID);
    assert(header->cmd_id == 42);
    assert(header->payload_length == 1);
    assert(ack[sizeof(PacketHeader)] == ACK_VALUE);
}

void test_make_nak(void) {
    u8* nak = PRT_MakeNak(42);
    assert(nak != NULL);
    PacketHeader* header = (PacketHeader*)nak;
    assert(header->sync == SYNC_WORD);
    assert(header->sys_id == SYSTEM_ID);
    assert(header->cmd_id == 42);
    assert(header->payload_length == 1);
    assert(nak[sizeof(PacketHeader)] == NAK_VALUE);
}

void test_make_packet(void) {
    u8 payload[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    u8* packet = PRT_MakePacket(42, payload, 10);
    assert(packet != NULL);
    PacketHeader* header = (PacketHeader*)packet;
    assert(header->sync == SYNC_WORD);
    assert(header->sys_id == SYSTEM_ID);
    assert(header->cmd_id == 42);
    assert(header->payload_length == 10);
    assert(memcmp(packet + sizeof(PacketHeader), payload, 10) == 0);
}

int main(void) {
    printf("Running protocol tests...\n");
    
    test_get_packet_size();
    test_get_command_id();
    test_make_ack();
    test_make_nak();
    test_make_packet();
    
    printf("All protocol tests passed!\n");
    return 0;
} 