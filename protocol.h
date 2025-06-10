#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "types.h"
#include "circ_buf.h"

// Get a command from the buffer
i32 PRT_GetCommand(CircBuf* buf, u8** cmd_buffer_ptr);

// Get the command ID from a command buffer
u8 PRT_GetCommandId(const u8* cmd_buffer);

// Make an ACK response for a command that succeeded
u8* PRT_MakeAck(u8 cmd_id);

// Make a NAK response for a command that failed
u8* PRT_MakeNak(u8 cmd_id);

// Create a packet with the given payload
// cmd_id: Command/telemetry ID
// payload: Pointer to payload data
// payload_size: Size of payload in bytes
// Returns pointer to complete packet (header + payload + CRC)
u8* PRT_MakePacket(u8 cmd_id, const u8* payload, u32 payload_size);

// Get the size of a packet with the given payload size
u32 PRT_GetPacketSize(u32 payload_size);

#endif // PROTOCOL_H 