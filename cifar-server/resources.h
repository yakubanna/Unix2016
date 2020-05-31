#pragma once

#include "http_response.h"

void CreateErrorPage(struct THttpResponse* response, enum EHttpCode code);
void CreateIndexPage(struct THttpResponse* response, int page);
void SendCifarBitmap(struct THttpResponse* response, int number);
void SendStaticFile(struct THttpResponse* response, const char* path);
void SendStaticDirectory(struct THttpResponse* response);
