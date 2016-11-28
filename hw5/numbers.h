#pragma once

#include <stddef.h>
#include <stdint.h>

struct TNumberSource {
    size_t Count;
    size_t Pos;
    uint32_t* Data;
};

void NumberSourceInit(struct TNumberSource* self, const char* path);

uint32_t NumberSourceGet(struct TNumberSource* self);

void NumberSourceDestroy(struct TNumberSource* self);
