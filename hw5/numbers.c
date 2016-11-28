#include "numbers.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX 1000000000

void NumberSourceInit(struct TNumberSource* self, const char* path) {
    self->Count = 0;
    self->Pos = 0;
    self->Data = NULL;

    srand(42);

    if (path) {
        FILE* f;
        if ((f = fopen(path, "r"))) {
            unsigned int n;
            if (fscanf(f, "%u", &n) == 1) {
                self->Data = (uint32_t*)calloc(n, sizeof(uint32_t));
                if (self->Data) {
                    for (uint32_t i = 0; i < n; ++i) {
                        unsigned int x;
                        if ((fscanf(f, "%u", &x) == 1) && (x <= MAX)) {
                            self->Data[self->Count++] = x;
                        }
                    }
                }
            }
            fclose(f);
        }
    }
}

uint32_t NumberSourceGet(struct TNumberSource* self) {
    if (self->Count) {
        const uint32_t x = self->Data[self->Pos];
        self->Pos++;
        if (self->Pos == self->Count) {
            self->Pos = 0;
        }
        return x;
    } else {
        return rand() % (MAX + 1);
    }
}

void NumberSourceDestroy(struct TNumberSource* self) {
    free(self->Data);
}
