#include "response.h"
#include "file.h"
#include "buf.h"

const struct response RESPONSE_GET[RESPONSE_STATUS_END] = {
    { 0 },
    {
        .header = {
            .data = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n",
            .size = 47
        },
        .body = { 0 }
    },
    {
        .header =
        {
            .data = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n",
            .size = 51
        },
        .body =
        {
            .data = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<title>404 Not Found</title>"
                "</head>"
                "<body>"
                "<h1>Not Found</h1>The requested URL was not found on this server."
                "</body>"
                "</html>",
            .size = 147
        }
    },
    {
        .header =
        {
            .data = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n",
            .size = 57
        },
        .body = { 0 }
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
    size_t content_type_len = strlen(HTTP_STR_CONTENT[type]);

    size_t total_size = 8
        + sizeof(HTTP_STR_200_OK)-1
        + sizeof("Content-Type: ")-1
        + content_type_len
        + sizeof("\r\n\r\n");

    struct buf header = buf_alloc(total_size);
    buf_append(&header, HTTP_STR_PROTOCOL[HTTP_PROTOCOL_1_1], 8); // TODO configure protocol ver
    buf_append(&header, HTTP_STR_200_OK, sizeof(HTTP_STR_200_OK)-1);
    buf_append(&header, "Content-Type: ", sizeof("Content-Type: ")-1);
    buf_append(&header, HTTP_STR_CONTENT[type], content_type_len);
    buf_append(&header, "\r\n\r\n", sizeof("\r\n\r\n"));

    return (struct response) {
        .header = header,
        .body = contents,
    };
}
