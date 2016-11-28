#include "io.h"
#include "numbers.h"

#include <errno.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define MAX_CONNECTIONS 5
#define MAX_ATTEMPTS 32

void SigchldHandler(int s) {
    (void)(s);
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0) {
    }
    errno = saved_errno;
}

bool SetUpSignalHandlers() {
    struct sigaction sa;
    sa.sa_handler = SigchldHandler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        return false;
    }
    return true;
}

char RespondToGuess(uint32_t number, uint32_t guess) {
    if (number > guess) {
        return '>';
    }
    if (number < guess) {
        return '<';
    }
    return '=';
}

int ServeSingleClient(int sockfd, uint32_t number) {
    int attempt;
    for (attempt = 1; attempt <= MAX_ATTEMPTS; ++attempt) {
        uint32_t guess;
        if (!RecvAll(sockfd, (char*)&guess, sizeof(guess))) {
            break;
        }
        guess = ntohl(guess);

        char result = RespondToGuess(number, guess);
        fprintf(stderr, "* attempt #%d: x %c %"PRIu32"\n", attempt, result, guess);

        if (!SendAll(sockfd, &result, sizeof(result))) {
            break;
        }
    }
    if (attempt > MAX_ATTEMPTS) {
        fprintf(stderr, "limit of attempts reached, bye!\n");
    }

    if (close(sockfd) == -1) {
        perror("close socket");
        return 1;
    }
    return 0;
}

int RunMain(const char* path, struct TNumberSource* src) {
    if (!SetUpSignalHandlers()) {
        return 1;
    }

    int fd;

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_un local;
    local.sun_family = AF_UNIX;
    if (strlen(path) >= sizeof(local.sun_path)) {
        fprintf(stderr, "path '%s' is too long for UNIX domain socket\n", path);
        return 1;
    }
    strcpy(local.sun_path, path);
    socklen_t localLen = sizeof(local);
    unlink(local.sun_path);

    if (bind(fd, (struct sockaddr*)&local, localLen) == -1) {
        perror("bind");
        return 1;
    }

    if (listen(fd, MAX_CONNECTIONS) == -1) {
        perror("listen");
        return 1;
    }

    fprintf(stderr, "waiting for a connection...\n");

    for (;;) {
        int newFd;
        struct sockaddr_un remote;
        socklen_t remoteLen = sizeof(remote);

        if ((newFd = accept(fd, (struct sockaddr*)&remote, &remoteLen)) == -1) {
            perror("accept");
            return 1;
        }

        uint32_t number = NumberSourceGet(src);

        pid_t child = fork();
        if (child == -1) {
            perror("fork");
            return 1;
        }
        if (child != 0) {
            fprintf(stderr, "child process (PID %d) started, guessing number x = %"PRIu32"\n", (int)child, number);
        }
        if (child == 0) {
            // this is the child process
            if (close(fd) == -1) { // child doesn't need the listener
                perror("close listener socket");
            }
            return ServeSingleClient(newFd, number);
        }
        if (close(newFd) == -1) {
            perror("close new socket");
        }
    }
    return 0;
}

void PrintUsage(const char* prog) {
    fprintf(stderr, "=== number guessing server ===\n");
    fprintf(stderr, "Usage: %s UNIX_SOCKET_PATH [ NUMBERS_PATH ]\n\n", prog);
}

int main(int argc, char* argv[]) {
    if (!(2 <= argc && argc <= 3)) {
        PrintUsage(argv[0]);
        return 2;
    }

    const char* socketPath = argv[1];
    const char* numbersPath = (argc >= 3) ? argv[2] : NULL;

    struct TNumberSource src;
    NumberSourceInit(&src, numbersPath);
    const int ret = RunMain(socketPath, &src);
    NumberSourceDestroy(&src);
    return ret;
}
