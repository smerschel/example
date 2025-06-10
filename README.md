# Embedded Systems Project

This project implements a command and telemetry system for embedded devices. It includes a circular buffer implementation, command manager, protocol handling, and system time management.

## Features

- Circular buffer with thread-safe operations
- Command manager for handling incoming commands
- Protocol implementation for packet handling
- System time management
- Telemetry management
- Unit tests with coverage reporting

## Building

To build the project:

```bash
# Build main program
make

# Build and run tests
make test

# Generate coverage report
make coverage
```

## Project Structure

- `circ_buf.*` - Circular buffer implementation
- `command_manager.*` - Command handling system
- `protocol.*` - Protocol implementation
- `sys_time.*` - System time management
- `telemetry_manager.*` - Telemetry system
- `test_*.c` - Unit tests
- `Makefile` - Build system

## Testing

The project includes unit tests for all major components. Run the tests with:

```bash
make test
```

To generate a coverage report:

```bash
make coverage
``` 