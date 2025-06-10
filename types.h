#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>

// Basic type definitions (Rust-like)
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef float    f32;
typedef double   f64;

// Return codes
typedef enum {
    SUCCESS = 0,
    ERROR_INVALID_PARAMETER = -1,
    ERROR_TIMEOUT = -2,
    ERROR_BUFFER_OVERFLOW = -3,
    ERROR_BUFFER_UNDERFLOW = -4,
    ERROR_NOT_INITIALIZED = -5,
    ERROR_COMMUNICATION = -6,
    ERROR_HARDWARE = -7,
    ERROR_UNKNOWN = -8
} ReturnCode;

// Types with timestamps
typedef struct {
    u8 value;
    u64 timestamp;  // Clock ticks for baremetal systems
} TimedU8;

typedef struct {
    u16 value;
    u64 timestamp;
} TimedU16;

typedef struct {
    u32 value;
    u64 timestamp;
} TimedU32;

typedef struct {
    u64 value;
    u64 timestamp;
} TimedU64;

typedef struct {
    i8 value;
    u64 timestamp;
} TimedI8;

typedef struct {
    i16 value;
    u64 timestamp;
} TimedI16;

typedef struct {
    i32 value;
    u64 timestamp;
} TimedI32;

typedef struct {
    i64 value;
    u64 timestamp;
} TimedI64;

typedef struct {
    f32 value;
    u64 timestamp;
} TimedF32;

typedef struct {
    f64 value;
    u64 timestamp;
} TimedF64;

typedef struct {
    bool value;
    u64 timestamp;
} TimedBool;

#endif // TYPES_H 