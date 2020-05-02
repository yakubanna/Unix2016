#pragma once

#include <stddef.h>

// To produce warnings about format mismatch
#ifdef __GNUC__
#define PRINTF_FORMAT(n, m) __attribute__((__format__(__printf__, n, m)))
#else
#define PRINTF_FORMAT(n, m)
#endif

struct TStringBuilder {
    char* Data;       // null-terminated
    size_t Length;    // does not include '\0'
    size_t Capacity_; // does not include '\0'
};

void TStringBuilder_Init(struct TStringBuilder* self);
void TStringBuilder_Destroy(struct TStringBuilder* self);

void TStringBuilder_AppendCStr(struct TStringBuilder* self, const char* data);
void TStringBuilder_AppendBuf(struct TStringBuilder* self, const char* data, size_t size);
void TStringBuilder_Sprintf(struct TStringBuilder* self, const char* format, ...) PRINTF_FORMAT(2, 3);
void TStringBuilder_Clear(struct TStringBuilder* self);
void TStringBuilder_ChopSuffix(struct TStringBuilder* self, const char* suffix);
