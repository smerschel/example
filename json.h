#ifndef JSON_H
#define JSON_H

#include "types.h"

// Helper function to skip whitespace in JSON string
const char* JSON_SkipWhitespace(const char* str);

// Helper function to parse a JSON number
bool JSON_ParseNumber(const char** str, int* value);

// Helper function to find a JSON field
const char* JSON_FindField(const char* json, const char* field_name);

// Helper function to parse a JSON array element
bool JSON_ParseArrayElement(const char** json, const char* field_name, int* value);

#endif // JSON_H 