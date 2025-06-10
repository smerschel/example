#include "protocol.h"
#include "system_defs.h"
#include <string.h>

#define MAX_PACKET_SIZE 1024  // Header size
#define MAX_PAYLOAD_SIZE MAX_PACKET_SIZE - sizeof(PacketHeader) - sizeof(u8) // minus the crc8 at the footer
#define ACK_VALUE 0x06
#define NAK_VALUE 0x15
#define SYNC_WORD 0xAA55  // Common sync word pattern

// Packet header structure
typedef struct {
    u16 sync;      // Sync word (0xAA55)
    u8 sys_id;     // System ID
    u8 cmd_id;     // Command ID
    u32 payload_length;     // Payload length
} PacketHeader;

// Protocol state machine states
typedef enum {
    STATE_SYNC1,    // Looking for first sync byte
    STATE_SYNC2,    // Looking for second sync byte
    STATE_HEADER,   // Reading header
    STATE_PAYLOAD,  // Reading payload
    STATE_CRC       // Reading CRC
} ProtocolState;

// Static buffer for responses
static u8 response_buffer[MAX_PACKET_SIZE];
static u8 command_buffer[MAX_PACKET_SIZE];
static ProtocolState current_state = STATE_SYNC1;
static u32 bytes_received = 0;
static u32 expected_length = 0;

u32 PRT_GetPacketSize(u32 payload_size) {
    return sizeof(PacketHeader) + payload_size + sizeof(u8);
}

i32 PRT_GetCommand(CircBuf* buf, u8** cmd_buffer_ptr) {
    if (buf == NULL || cmd_buffer_ptr == NULL) {
        return -1;
    }

    u8 byte;
    u32 dropped_bytes = 0;
    bool packet_complete = false;
    bool packet_valid = true;

    while (!packet_complete && CIR_Get(buf, &byte) == SUCCESS) {
        switch (current_state) {
            case STATE_SYNC1:
                if (byte == (SYNC_WORD & 0xFF)) {
                    current_state = STATE_SYNC2;
                    command_buffer[0] = byte;
                    bytes_received = 1;
                } else {
                    dropped_bytes++;
                }
                break;

            case STATE_SYNC2:
                if (byte == ((SYNC_WORD >> 8) & 0xFF)) {
                    current_state = STATE_HEADER;
                    command_buffer[1] = byte;
                    bytes_received = 2;
                } else {
                    // Sync word broken, go back to looking for first sync byte
                    current_state = STATE_SYNC1;
                    dropped_bytes+=2;
                    bytes_received=0;
                }
                break;

            case STATE_HEADER:
                command_buffer[bytes_received++] = byte;
                if (bytes_received == sizeof(PacketHeader)) {
                    // Header complete, check if it's valid
                    PacketHeader* header = (PacketHeader*)command_buffer;
                    if (header->payload_length > MAX_PAYLOAD_SIZE) {
                        // Invalid length, discard packet
                        current_state = STATE_SYNC1;
                        dropped_bytes += bytes_received;
                        bytes_received = 0;
                        packet_valid = false;
                    } else {
                        expected_length = sizeof(PacketHeader) + header->payload_length + sizeof(u8);
                        current_state = STATE_PAYLOAD;
                    }
                }
                break;

            case STATE_PAYLOAD:
                command_buffer[bytes_received++] = byte;
                if (bytes_received == expected_length - 1) {
                    current_state = STATE_CRC;
                }
                break;

            case STATE_CRC:
                command_buffer[bytes_received++] = byte;
                packet_complete = true;
                break;
        }
    }

    // Update dropped bytes count
    if (dropped_bytes > 0) {
        cvt.dropped_bytes.value += dropped_bytes;
        cvt.dropped_bytes.time = TIM_GetTicks();
    }

    // If we have a complete packet, validate and process it
    if (packet_complete && packet_valid) {
        PacketHeader* header = (PacketHeader*)command_buffer;
        
        // Check system ID
        if (header->sys_id != SYSTEM_ID) {
            // Wrong system ID, discard packet
            current_state = STATE_SYNC1;
            bytes_received = 0;
            return 0;
        }

        *cmd_buffer_ptr = command_buffer;
        
        // Reset state machine
        current_state = STATE_SYNC1;
        bytes_received = 0;
        
        return bytes_received;
    }

    // Packet not complete or invalid
    return 0;
}

u8 PRT_GetCommandId(const u8* cmd_buffer) {
    if (cmd_buffer == NULL) {
        return 0;
    }
    return ((PacketHeader*)cmd_buffer)->cmd_id;
}

u8* PRT_MakeAck(u8 cmd_id) {
    PacketHeader* header = (PacketHeader*)response_buffer;
    u8* payload = response_buffer + sizeof(PacketHeader);
    u8* crc = payload + 1;
    
    header->sync = SYNC_WORD;
    header->sys_id = SYSTEM_ID;
    header->cmd_id = cmd_id;
    payload[0] = ACK_VALUE;
    header->payload_length = 1;
    *crc = Packet_CalculateCRC8(response_buffer, sizeof(PacketHeader) + 1);
        
    return response_buffer;
}

u8* PRT_MakeNak(u8 cmd_id) {
    PacketHeader* header = (PacketHeader*)response_buffer;
    u8* payload = response_buffer + sizeof(PacketHeader);
    u8* crc = payload + 1;
    
    header->sync = SYNC_WORD;
    header->sys_id = SYSTEM_ID;
    header->cmd_id = cmd_id;
    payload[0] = NAK_VALUE;
    header->payload_length = 1;
    *crc = Packet_CalculateCRC8(response_buffer, sizeof(PacketHeader) + 1);
        
    return response_buffer;
}

u8* PRT_MakePacket(u8 cmd_id, const u8* payload, u32 payload_size) {
    if (payload == NULL || payload_size > MAX_PAYLOAD_SIZE) {
        return NULL;
    }

    PacketHeader* header = (PacketHeader*)response_buffer;
    u8* packet_payload = response_buffer + sizeof(PacketHeader);
    u8* crc = packet_payload + payload_size;
    
    // Fill in header
    header->sync = SYNC_WORD;
    header->sys_id = SYSTEM_ID;
    header->cmd_id = cmd_id;
    header->payload_length = payload_size;

    // Copy payload
    memcpy(packet_payload, payload, payload_size);

    // Calculate and append CRC
    *crc = Packet_CalculateCRC8(response_buffer, sizeof(PacketHeader) + payload_size);
        
    return response_buffer;
} 