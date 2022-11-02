#ifndef HTTPIE_RESPONSE_H
#define HTTPIE_RESPONSE_H

#include "framework.h"
#include "http.h"

struct response {
    struct buf header;
    struct buf body;
//     content-type,
//     content-encoding,
//     date,
//     server
};

struct response response_from_file(const char* path);

enum RESPONSE_STATUS {
    RESPONSE_STATUS_UNKNOWN,
    RESPONSE_STATUS_BAD_REQUEST,
    RESPONSE_STATUS_NOT_FOUND,
    RESPONSE_STATUS_INTERNAL_SERVER_ERROR,
    RESPONSE_STATUS_END
};

extern const struct response RESPONSE_GET[];

#define response_static(status, type, str_body)\
    (struct response) {\
        .header = string_from_static(status type),\
        .body = string_from_static(str_body),  \
    }

#define response_200_static_text(str)\
    response_static(HTTP_STR_200_OK, HTTP_STR_CONTENT_TEXT_PLAIN, str)

#define response_400_static_text(str)\
    response_static(HTTP_STR_400_BAD_REQUEST, HTTP_STR_CONTENT_TEXT_PLAIN, str)

#define response_404_static_text(str)\
    response_static(HTTP_STR_404_NOT_FOUND, HTTP_STR_CONTENT_TEXT_PLAIN, str)

#define response_500_static_text(str)\
    response_static(HTTP_STR_500_INTERNAL_SERVER_ERROR, HTTP_STR_CONTENT_TEXT_PLAIN, str)

#endif
