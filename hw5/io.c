#include "io.h"

#include <sys/socket.h>
#include <sys/types.h>

#include <stdio.h>

bool RecvAll(int sockfd, char* data, size_t len) {
    while (len != 0) {
        ssize_t ret = recv(sockfd, data, len, 0);
        if (ret == -1) {
            perror("recv");
            return false;
        }
        if (ret == 0) {
            fprintf(stderr, "recv: connection closed by client\n");
            return false;
        }
        data += ret;
        len -= ret;
    }
    return true;
}

bool SendAll(int sockfd, const char* data, size_t len) {
    while (len != 0) {
        ssize_t ret = send(sockfd, data, len, 0);
        if (ret == -1) {
            perror("send");
            return false;
        }
        if (ret == 0) {
            fprintf(stderr, "send: connection closed by client\n");
            return false;
        }
        data += ret;
        len -= ret;
    }
    return true;
}
