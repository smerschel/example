#ifndef PACKET_H
#define PACKET_H

#include "types.h"

#define MAX_PAYLOAD_SIZE 1024
#define PACKET_HEADER_SIZE 6  // 1 byte sys_id + 1 byte cmd_id + 4 bytes length
#define PACKET_FOOTER_SIZE 1  // 1 byte CRC8
#define MIN_PACKET_SIZE (PACKET_HEADER_SIZE + PACKET_FOOTER_SIZE)

// Packet header structure
typedef struct {
    u16 sync;      // Sync word (0xAA55)
    u8 sys_id;      // System identifier
    u8 cmd_id;      // Command/Response identifier
    u32 length;     // Payload length (not including header or footer)
} PacketHeader;

// Calculate CRC8 for a buffer
// buffer: Buffer to calculate CRC for
// length: Length of buffer
// Returns CRC8 value
u8 Packet_CalculateCRC8(const u8* buffer, u32 length);

// Verify CRC8 for a buffer
// buffer: Buffer to verify
// length: Length of buffer
// crc: Expected CRC8 value
// Returns true if CRC matches, false otherwise
bool Packet_VerifyCRC8(const u8* buffer, u32 length, u8 crc);

#endif // PACKET_H 