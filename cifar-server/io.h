#pragma once

#include <stdbool.h>
#include <stddef.h>

bool SendAll(int sockfd, const void* data, size_t len);
