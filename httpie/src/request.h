#ifndef HTTPIE_REQUEST_H
#define HTTPIE_REQUEST_H

#include "framework.h"
#include "http.h"
#include "response.h"

#define QUERY_SIZE_MAX 248
#define QUERY_PARAMETERS_MAX 8

struct query_parameter {
    char key[60];
    size_t key_size;
    char value[60];
    size_t value_size;
};

struct query {
    char path[QUERY_SIZE_MAX];
    size_t path_size;
    size_t params_size;
    struct query_parameter params[QUERY_PARAMETERS_MAX];
};

struct request {
    enum http_method method;
    enum http_protocol protocol;
    enum http_content_type accept;
    struct query query;
    const char* body;
    size_t body_size;
    // struct http_content_encoding accept_encoding;
};

int request_get(struct request* request, const char* str_request);

#endif
