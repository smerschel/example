#ifndef COMMAND_MANAGER_H
#define COMMAND_MANAGER_H

// Removed include for common_types.h
#include "circ_buf.h"
#include "protocol.h"

// Command handler function type
typedef ReturnCode (*CmdHandler)(const u8* payload, u32 length);

// Structure to hold command handler information
typedef struct {
    CmdHandler handler;
    bool needs_ack;
} CommandHandlerInfo;

// Initialize the command manager
// Returns true if successful, false otherwise
bool CMD_Init();

// Register a command handler
// cmd_id: Command identifier
// handler: Function to handle the command
// needs_ack: Whether to send ack/nak for this command
// Returns SUCCESS or error code
ReturnCode CMD_RegisterHandler(u8 cmd_id, CmdHandler handler, bool needs_ack);

// Main task function that processes incoming commands
// This function should be called in a loop
ReturnCode CMD_Task(void);

#endif // COMMAND_MANAGER_H 