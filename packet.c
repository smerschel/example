#include "packet.h"

// CRC8 polynomial: x^8 + x^2 + x + 1
#define CRC8_POLY 0x07

u8 Packet_CalculateCRC8(const u8* buffer, u32 length) {
    u8 crc = 0;
    
    for (u32 i = 0; i < length; i++) {
        crc ^= buffer[i];
        for (u8 j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ CRC8_POLY;
            } else {
                crc = (crc << 1);
            }
        }
    }
    
    return crc;
}

bool Packet_VerifyCRC8(const u8* buffer, u32 length, u8 crc) {
    return Packet_CalculateCRC8(buffer, length) == crc;
} 