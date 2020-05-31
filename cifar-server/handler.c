#include "handler.h"

#include "http_request.h"
#include "http_response.h"
#include "resources.h"
#include "stringutils.h"

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef DEBUG
static const char* SafeStr(const char* value) {
    return (value != NULL) ? value : "<null>";
}
#endif

static void Handle(const struct THttpRequest* request, struct THttpResponse* response) {
    #ifdef DEBUG
    fprintf(
        stderr, "method: '%s'; path: '%s'; qs: '%s'\n",
        SafeStr(request->Method), SafeStr(request->Path), SafeStr(request->QueryString)
    );
    #endif

    if (strcasecmp(request->Method, "GET") != 0) { // case-insensitive compare
        CreateErrorPage(response, HTTP_METHOD_NOT_ALLOWED);
        return;
    }

    if (strcmp(request->Path, "/") == 0) {
        int page = request->QueryString ? GetIntParam(request->QueryString, "page") : 0;
        CreateIndexPage(response, page);
        return;
    }
    if (StartsWith(request->Path, "/images/")) {
        int n;
        if (sscanf(request->Path, "/images/%d.bmp", &n) == 1) {
            SendCifarBitmap(response, n);
            return;
        }
    }
    if (StartsWith(request->Path, "/static/")) {
        if (strstr(request->Path, "../") != NULL) {
            CreateErrorPage(response, HTTP_METHOD_NOT_ALLOWED);
            return;
        }
        SendStaticFile(response, request->Path + 1);
        return;
    }

    CreateErrorPage(response, HTTP_NOT_FOUND);
}

void ServeClient(int sockfd) {
    struct THttpRequest req;
    THttpRequest_Init(&req);
    struct THttpResponse resp;
    THttpResponse_Init(&resp);

    if (THttpRequest_Receive(&req, sockfd)) {
        Handle(&req, &resp);
    } else {
        CreateErrorPage(&resp, HTTP_BAD_REQUEST);
    }

    THttpResponse_Send(&resp, sockfd);
    THttpResponse_Destroy(&resp);
    THttpRequest_Destroy(&req);

    close(sockfd);
}
