#include "server.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_PORT 8080

static void PrintUsage(const char* argv0) {
    fprintf(stderr, "Usage: %s [-p PORT]\n\n", argv0);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "  -p PORT      TCP port to listen (default: %d)\n", DEFAULT_PORT);
}

static bool ParseOptions(int argc, char* argv[], uint16_t* port) {
    int c;
    while ((c = getopt(argc, argv, "p:")) != -1) {
        switch (c) {
        case 'p':
            if (sscanf(optarg, "%hu", port) != 1) {
                fprintf(stderr, "Invalid port option: %s\n", optarg);
                PrintUsage(argv[0]);
                return false;
            }
            break;
        default: /* '?' */
            PrintUsage(argv[0]);
            return false;
        }
    }
    return true;
}

int main (int argc, char* argv[]) {
    uint16_t port = DEFAULT_PORT;
    if (!ParseOptions(argc, argv, &port)) {
        return EXIT_FAILURE;
    }
    if (!RunServer(port)) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
