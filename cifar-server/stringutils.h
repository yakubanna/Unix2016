#pragma once

#include <stdbool.h>

int GetIntParam(const char* queryString, const char* name);

bool StartsWith(const char* s, const char* prefix);

// case-insensitive
bool EndsWithCI(const char* s, const char* suffix);
