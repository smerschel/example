#ifndef TELEMETRY_MANAGER_H
#define TELEMETRY_MANAGER_H

#include "types.h"
#include "cvt.h"
#include "external_interface.h"

// Telemetry item configuration
typedef struct {
    u32 offset;     // Offset into CVT in bytes
    u32 size;       // Size of item in bytes
} TlmItemConfig;

// Telemetry packet configuration
typedef struct {
    u8 packet_id;           // Unique packet identifier
    u32 num_items;          // Number of items in this packet
    TlmItemConfig items[16]; // Array of item configurations (fixed size)
} TlmPacketConfig;

// Initialize the telemetry manager
// Returns true if successful, false otherwise
bool TLM_Init(void);

// Handle a request for a specific telemetry packet
// packet_id: ID of the requested packet
// Returns SUCCESS if packet was sent, error code otherwise
ReturnCode TLM_HandlePacketRequest(u8 packet_id);

#endif // TELEMETRY_MANAGER_H 