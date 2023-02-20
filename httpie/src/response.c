#include "response.h"
#include "file.h"
#include "buf.h"
#include <stdarg.h>
#include <assert.h>

const struct response RESPONSE_GET[RESPONSE_STATUS_END] = {
    { 0 },
    {
        .buf = {
            .data = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n",
            .size = 47
        },
    },
    {
        .buf =
        {
            .data = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>404 Not Found</title>"
                "</head>"
                "<body>"
                "<h1>Not Found</h1>The requested URL was not found on this server."
                "</body>"
                "</html>",
            .size = 198
        },
    },
    {
        .buf =
        {
            .data = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n",
            .size = 57
        },
    }
};

struct response response_from_file(const char* path) {

    if (!file_exists(path)) {
        return RESPONSE_GET[RESPONSE_STATUS_NOT_FOUND];
    }

    struct buf contents = buf_from_file(path);

 	if (!contents.data) {
        return RESPONSE_GET[RESPONSE_STATUS_NOT_FOUND];
    }

    enum http_content_type type = http_content_from_filename(path);
    // size_t content_type_len = strlen(HTTP_STR_CONTENT[type]);

    return response_mk(contents.data, contents.size,
        HTTPIE_PROTOCOL" 200 OK\r\n"
        "Content-Type: %s\r\n"
        "Content-Length: %s\r\n\r\n",
        HTTP_STR_CONTENT[type],
        contents.size
    );

    // size_t total_size = 8
    //     + sizeof(HTTP_STR_200_OK)-1
    //     + sizeof("Content-Type: ")-1
    //     + content_type_len
    //     + sizeof("\r\n\r\n");

    // struct buf header = buf_alloc(total_size);
    // buf_append(&header, HTTP_STR_PROTOCOL[HTTP_PROTOCOL_1_1], 8); // TODO configure protocol ver
    // buf_append(&header, HTTP_STR_200_OK, sizeof(HTTP_STR_200_OK)-1);
    // buf_append(&header, "Content-Type: ", sizeof("Content-Type: ")-1);
    // buf_append(&header, HTTP_STR_CONTENT[type], content_type_len);
    // buf_append(&header, "\r\n\r\n", sizeof("\r\n\r\n"));

    // return (struct response) {
    //     .header = header,
    //     .body = contents,
    // };
}

struct response response_mk(void* body, size_t body_size, const char* fmt, ...) {

    size_t size = 0;
    va_list args, tmp_args;
    struct buf buffer;
    char* data = 0;

    assert(fmt != NULL || body != NULL);

    if (fmt != NULL) {
        va_start(args, fmt);
        va_copy(tmp_args, args);

#if defined (WIN32) || defined (_WIN32)
        size = _vscprintf(fmt, tmp_args) + 1;
#else
        size = vsnprintf(NULL, 0, fmt, tmp_args) + 1;
#endif
        va_end(tmp_args);
         
        buffer = buf_alloc(size + body_size);

        if (size < 1) {
            va_end(args);
            return (struct response) {0};
        }
        if ((data = (char*)malloc(size)) == NULL) {
            fprintf(stderr, "Unable to allocate memory");
            return (struct response) {0};
        }

        if (vsnprintf(data, size, fmt, args) < 0) {
            data[size - 1] = 0;
        }

        buf_append(&buffer, data, size);
        free(data);
    } else {
        buffer = buf_alloc(size + body_size);
    }

    if (body != NULL && body_size > 0) {
        buf_append(&buffer, body, body_size);
    }

    va_end(args);

    buffer.size -= 1;
    return (struct response) { .buf = buffer };
}
