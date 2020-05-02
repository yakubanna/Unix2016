#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// n = width*height
// data = [n REDs, n GREENs, n BLUEs], total size = 3*n
//
// On success, returns true and fills `outdata` and `outsize`,
// the caller should free() the memory
bool BuildBmpFileData(int width, int height, const uint8_t* source, char** outdata, size_t* outsize);
