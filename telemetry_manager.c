#include "telemetry_manager.h"
#include "sys_time.h"
#include "protocol.h"
#include "json.h"
#include <string.h>
#include <stdio.h>

#define MAX_PACKETS 32
#define MAX_PAYLOAD_SIZE 1024
#define MAX_CONFIG_LINE_SIZE 256

// Static buffer for building telemetry payloads
static u8 payload_buffer[MAX_PAYLOAD_SIZE];

// Static storage for packet configurations
static TlmPacketConfig tlm_packets[MAX_PACKETS];
static u32 num_packets = 0;
static bool is_initialized = false;

// Helper function to parse a single packet configuration from JSON
static bool parse_packet_config_json(const char** json, TlmPacketConfig* packet) {
    // Parse packet_id
    if (!JSON_ParseArrayElement(json, "\"packet_id\"", (int*)&packet->packet_id)) {
        return false;
    }
    
    // Parse num_items
    int num_items;
    if (!JSON_ParseArrayElement(json, "\"num_items\"", &num_items)) {
        return false;
    }
    
    if (num_items > 16) {  // Max items per packet
        return false;
    }
    packet->num_items = num_items;
    
    // Skip to items array
    *json = strstr(*json, "\"items\"");
    if (!*json) return false;
    *json = strchr(*json, '[');
    if (!*json) return false;
    *json = JSON_SkipWhitespace(*json + 1);
    
    // Parse each item
    for (u32 i = 0; i < packet->num_items; i++) {
        // Parse offset
        if (!JSON_ParseArrayElement(json, "\"offset\"", (int*)&packet->items[i].offset)) {
            return false;
        }
        
        // Parse size
        if (!JSON_ParseArrayElement(json, "\"size\"", (int*)&packet->items[i].size)) {
            return false;
        }
        
        // Skip to next item or end of array
        *json = strchr(*json, '}');
        if (!*json) return false;
        *json = JSON_SkipWhitespace(*json + 1);
        if (*json[0] == ',') {
            *json = JSON_SkipWhitespace(*json + 1);
        }
    }
    
    return true;
}

bool TLM_Init(void) {
    if (is_initialized) {
        return true;
    }

    // Open configuration file
    FILE* config_file = fopen("telemetry_config.json", "r");
    if (!config_file) {
        return false;
    }

    // Read entire file into buffer
    char config_buffer[4096];  // Adjust size as needed
    size_t bytes_read = fread(config_buffer, 1, sizeof(config_buffer) - 1, config_file);
    fclose(config_file);
    
    if (bytes_read == 0) {
        return false;
    }
    config_buffer[bytes_read] = '\0';

    // Skip to packets array
    const char* json = strstr(config_buffer, "\"packets\"");
    if (!json) {
        return false;
    }
    json = strchr(json, '[');
    if (!json) {
        return false;
    }
    json = JSON_SkipWhitespace(json + 1);

    // Parse each packet
    num_packets = 0;
    while (*json && num_packets < MAX_PACKETS) {
        if (*json == '{') {
            if (parse_packet_config_json(&json, &tlm_packets[num_packets])) {
                num_packets++;
            }
            json = JSON_SkipWhitespace(json);
            if (*json == ',') {
                json = JSON_SkipWhitespace(*json + 1);
            }
        } else if (*json == ']') {
            break;
        } else {
            json = JSON_SkipWhitespace(*json + 1);
        }
    }

    is_initialized = true;
    return true;
}

ReturnCode TLM_HandlePacketRequest(u8 packet_id) {
    if (!is_initialized) {
        return ERROR_UNKNOWN;
    }

    // Find the requested packet
    TlmPacketConfig* packet = NULL;
    for (u32 i = 0; i < num_packets; i++) {
        if (tlm_packets[i].packet_id == packet_id) {
            packet = &tlm_packets[i];
            break;
        }
    }

    if (packet == NULL) {
        return ERROR_INVALID_PARAMETER;
    }

    // Get CVT pointer
    CVT* cvt = CVT_Get();
    if (cvt == NULL) {
        return ERROR_UNKNOWN;
    }

    // Build packet payload
    u32 payload_size = 0;
    for (u32 i = 0; i < packet->num_items; i++) {
        TlmItemConfig* item = &packet->items[i];
        memcpy(payload_buffer + payload_size, 
               ((u8*)cvt) + item->offset, 
               item->size);
        payload_size += item->size;
    }

    // Create and send the packet
    u8* packet_data = PRT_MakePacket(packet_id, payload_buffer, payload_size);
    if (packet_data == NULL) {
        return ERROR_UNKNOWN;
    }

    // Send the packet
    if (EXT_Send(packet_data, PRT_GetPacketSize(payload_size)) != SUCCESS) {
        return ERROR_COMMUNICATION;
    }

    return SUCCESS;
}
