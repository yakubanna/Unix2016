#pragma once

#include <stdbool.h>
#include <stddef.h>

bool RecvAll(int sockfd, char* data, size_t len);
bool SendAll(int sockfd, const char* data, size_t len);
