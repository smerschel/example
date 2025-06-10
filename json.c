#include "json.h"
#include <string.h>
#include <stdlib.h>

const char* JSON_SkipWhitespace(const char* str) {
    while (*str && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r')) {
        str++;
    }
    return str;
}

bool JSON_ParseNumber(const char** str, int* value) {
    char* end;
    *value = strtol(*str, &end, 10);
    if (end == *str) {
        return false;
    }
    *str = end;
    return true;
}

const char* JSON_FindField(const char* json, const char* field_name) {
    const char* field = strstr(json, field_name);
    if (!field) return NULL;
    
    // Skip to the value
    field = strchr(field, ':');
    if (!field) return NULL;
    
    return JSON_SkipWhitespace(field + 1);
}

bool JSON_ParseArrayElement(const char** json, const char* field_name, int* value) {
    // Find the field
    *json = JSON_FindField(*json, field_name);
    if (!*json) return false;
    
    // Parse the number
    return JSON_ParseNumber(json, value);
} 