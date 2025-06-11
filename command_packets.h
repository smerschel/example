#ifndef COMMAND_PACKETS_H
#define COMMAND_PACKETS_H

#include "types.h"

// Structure for telemetry request command
typedef struct {
    u8 packet_id;    // ID of the telemetry packet to request
} tlm_request_telemetry_cmd;

#endif // COMMAND_PACKETS_H 