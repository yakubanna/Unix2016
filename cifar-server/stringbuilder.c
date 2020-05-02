#include "stringbuilder.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 15

static void TStringBuilder_AllocateFreeSpace(struct TStringBuilder* self, size_t capacityNeeded) {
    if (capacityNeeded <= self->Capacity_) {
        return;
    }

    size_t newCapacityZ = self->Capacity_ + 1;
    do {
        newCapacityZ = newCapacityZ * 2;
    } while (newCapacityZ < capacityNeeded + 1);

    char* newData = realloc(self->Data, newCapacityZ);
    if (newData == NULL) { // almost impossible with current allocators
        abort();
    }

    self->Data = newData;
    self->Capacity_ = newCapacityZ - 1;
}

static void TStringBuilder_EnsureNullTerminated(struct TStringBuilder* self) {
    self->Data[self->Length] = '\0';
}

void TStringBuilder_Init(struct TStringBuilder* self) {
    self->Length = 0;
    self->Capacity_ = INITIAL_CAPACITY;
    self->Data = malloc(INITIAL_CAPACITY + 1);
    if (self->Data == NULL) { // almost impossible with current allocators
        abort();
    }
    TStringBuilder_EnsureNullTerminated(self);
}

void TStringBuilder_Destroy(struct TStringBuilder* self) {
    free(self->Data);
}

void TStringBuilder_AppendCStr(struct TStringBuilder* self, const char* data) {
    TStringBuilder_AppendBuf(self, data, strlen(data));
}

void TStringBuilder_AppendBuf(struct TStringBuilder* self, const char* data, size_t size) {
    TStringBuilder_AllocateFreeSpace(self, self->Length + size);
    memcpy(self->Data + self->Length, data, size);
    self->Length += size;
    TStringBuilder_EnsureNullTerminated(self);
}

static bool VsprintfImpl(struct TStringBuilder* self, const char* format, va_list params, size_t* addLength) {
    const size_t freeSpace = self->Capacity_ - self->Length; // does not include '\0'

    va_list lst;
    va_copy(lst, params);
    const int ret = vsnprintf(self->Data + self->Length, freeSpace + 1, format, lst);
    va_end(lst);

    if (ret < 0) {
        abort();
    }
    // ret does not include '\0'
    *addLength = (size_t)ret;
    if (*addLength <= freeSpace) {
        self->Length += *addLength;
        return true;
    }
    return false;
}

static void TStringBuilder_Vsprintf(struct TStringBuilder* self, const char* format, va_list params) {
    size_t addLength;
    if (!VsprintfImpl(self, format, params, &addLength)) {
        // need more space
        TStringBuilder_AllocateFreeSpace(self, self->Length + addLength);
        VsprintfImpl(self, format, params, &addLength);
    }
    // sprintf adds '\0'
}

void TStringBuilder_Sprintf(struct TStringBuilder* self, const char* format, ...) {
    va_list lst;
    va_start(lst, format);
    TStringBuilder_Vsprintf(self, format, lst);
    va_end(lst);
}

void TStringBuilder_Clear(struct TStringBuilder* self) {
    self->Length = 0;
    TStringBuilder_EnsureNullTerminated(self);
}

void TStringBuilder_ChopSuffix(struct TStringBuilder* self, const char* suffix) {
    const size_t suffLen = strlen(suffix);
    if (suffLen <= self->Length &&
        memcmp(self->Data + self->Length - suffLen, suffix, suffLen) == 0) {
        self->Length -= suffLen;
        TStringBuilder_EnsureNullTerminated(self);
    }
}
