#include "io.h"

#include <sys/socket.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>

bool SendAll(int sockfd, const void* data, size_t len) {
    while (len != 0) {
        ssize_t ret = send(sockfd, data, len, 0);
        if (ret == -1) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            perror("send");
            return false;
        }
        data += ret;
        len -= ret;
    }
    return true;
}
