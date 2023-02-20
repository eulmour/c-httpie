#include "framework.h"
#include "server.h"
#include "request.h"
#include "file.h"

#if defined(_WIN32) || defined(_WIN64)
#   include <WinSock2.h>
#   include <ws2tcpip.h>
#   include <Windows.h> // put below winsock headers
    typedef SSIZE_T ssize_t;
#else 
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h> // inet_addr()
#   include <unistd.h>
#   ifndef INVALID_SOCKET
#       define INVALID_SOCKET -1
#   endif
#   ifndef SOCKET_ERROR
#       define SOCKET_ERROR -1
#   endif
#endif

#define REQUEST_SIZE 2048
#define DEFAULT_PORT 8080

struct httpie_internal {
#if defined(_WIN32) || defined(WIN32)
#   define HTTPIE_SOCK_T SOCKET
#else
#   define HTTPIE_SOCK_T int
#endif
    HTTPIE_SOCK_T sock, msg_sock;
    struct sockaddr_in local, client;
    int address_length;
    char address_str_buffer[16];
    ssize_t sent;
};

static void httpie_close(struct httpie* server) {
#if defined(_WIN32) || defined(_WIN64)
    closesocket(server->internal->msg_sock);
#else
    close(server->internal->msg_sock);
#endif
}

static struct response httpie_response_from_file(const char* wd, const char* path) {

    size_t wd_len = strlen(wd);
    size_t path_len = *path ? strlen(path) : 11;

    char* buffer = (char*)malloc(wd_len + path_len + 1);
    strcpy(buffer, wd);

    if (*path) {
        strcpy(buffer + wd_len, path);
    } else {
        strcpy(buffer + wd_len, "index.html");
    }

    struct response result = response_from_file(buffer);
    free(buffer);
    return result;
}

int httpie_job(struct httpie *server) {

    for (;;) {
        // wait for a connection
#if defined(_WIN32) || defined(_WIN64)
        server->internal->msg_sock = accept(
            server->internal->sock,
            (struct sockaddr*)&server->internal->client,
            &server->internal->address_length);
#else
        server->internal->msg_sock = accept(
            server->internal->sock,
            (struct sockaddr *) &server->internal->client,
            (socklen_t *) &server->internal->address_length);
#endif

        if (server->internal->msg_sock == INVALID_SOCKET) {
            fprintf(stderr, "Error: unable to accept socket.\n");
            continue;
        }

#ifndef NDEBUG
        printf("Connected to %s:%d\n",
            inet_ntop(
                AF_INET,
                &server->internal->client.sin_addr,
                server->internal->address_str_buffer,
                sizeof(server->internal->address_str_buffer)),
            htons(server->internal->client.sin_port));
#endif

        // char* buffer = (char*)malloc(REQUEST_SIZE);
        char* buffer;
        size_t buffer_size = 0;
        size_t body_size = 0;
        if (httpie_recv(server->internal->msg_sock, &buffer, &buffer_size, &body_size) == -1) {
            fprintf(stderr, "Failed to receive data.\n");
        }

        // check for errors
        if (buffer_size == 0) {
            goto cleanup;
        }

        // structure data
        struct request request; if (request_get(&request, buffer) == -1) {
            fprintf(stderr, "Failed to parse request\n");
            goto cleanup;
        }

        request.body = &buffer[buffer_size - body_size];
        request.body_size = body_size;

        // find route
        struct route *route = route_match(server->routes, request.query.path);

        // process and make response
        struct response response = route
            ? route->proc(request)
            : httpie_response_from_file(server->path_public, request.query.path + 1);

#ifndef NDEBUG
        printf("Client requested: %s\n", request.query.path);
#endif
        
        // send our response
        if ((server->internal->sent = httpie_send(server->internal->msg_sock, response.buf.data, (int)response.buf.size)) == SOCKET_ERROR) {
            fprintf(stderr, "Error sending response header.\n");
        }

        if (response.buf.dyn) {
            buf_free(&response.buf);
        }

cleanup:
        httpie_close(server);
        free(buffer);
    }

    return 0;
}

int httpie_listen(struct httpie *server) {

    (void)server;

    if (listen(server->internal->sock, 10) == SOCKET_ERROR) {
        return httpie_msg("Error when listening socket.") - 1;
    }

    if (httpie_job(server) == 1) {
        return - 1;
    } else {
        puts("Server is off.");
        return 0;
    }
}

int httpie_load(struct httpie *server) {

    if (server->internal) {
        return httpie_msg("Error: httpie_load called twice.\n") - 1;
    }

    server->internal = (struct httpie_internal*)malloc(sizeof(struct httpie_internal));

    memcpy(server->internal, &(struct httpie_internal) {
        .address_length = sizeof(struct sockaddr_in)
    }, sizeof(struct httpie_internal));

    server->internal->local = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_addr.s_addr = &server->address[0] ? inet_addr(server->address) : htonl(INADDR_ANY), //TODO inet_pton instead inet_addr
        .sin_port = htons(DEFAULT_PORT)
    };

    if (server->port) {
        if (server->port != 80 && server->port != 443) {
            server->internal->local.sin_port = htons(server->port);
        }
    }

#if defined(_WIN32) || defined(_WIN64)
    WSADATA	wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) == SOCKET_ERROR) {
        return httpie_msg("Error: WSA failure. Wsa: [%d]", WSAGetLastError()) - 1;
    }
#endif

    if ((server->internal->sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        return httpie_msg("Error: unable to create socket.\n") - 1;
    }

    if (bind(server->internal->sock, (const struct sockaddr *) &server->internal->local, sizeof(server->internal->local)) == SOCKET_ERROR) {
        return httpie_msg("Error: unable to bind socket.\n") - 1;
    }
    
    char cwd[FILE_MAX_PATH];
    cwd_get(FILE_MAX_PATH, cwd);
    printf("Server is working. Root directory is: %s\n", cwd);

    return 0;
}

int httpie_unload(struct httpie *server) {

    if (!server) {
        fprintf(stderr, "Failed to unload server: arg was null");
        return -1;
    }

    free(server->internal);

    return 0;
}

int httpie_recv(long long sock, char** buffer, size_t* size, size_t* body_size) {

    if (buffer == NULL) {
        return -1;
    }

    *buffer = (char*)malloc(REQUEST_SIZE);
    char* buffer_rw = *buffer;

    // receive request data
    if ((*size += recv((HTTPIE_SOCK_T)sock, &buffer_rw[*size], REQUEST_SIZE, 0)) == (size_t)(-1)) {
        return SOCKET_ERROR;
    }

    // collect contents if content-length is received
    char* content_size_str = NULL;
    buffer_rw[*size] = '\0';
    if ((content_size_str = strstr(buffer_rw, "Content-Length: ")) == NULL) {
        return 0;
    }

    size_t content_size = 0;
    if (!sscanf(content_size_str + 16, "%zul\r\n", &content_size) || content_size < 1) {
        return 0;
    }

    // needs larger buffer, require a reallocation
    if (*size + content_size > REQUEST_SIZE) {
        if ((*buffer = buffer_rw = (char*)realloc(buffer_rw, REQUEST_SIZE + content_size)) == NULL) {
            fprintf(stderr, "Unable to allocate memory.\n");
            return -1;
        }
    }

    const char* body_ptr = &buffer_rw[*size];

    do {
        *size += recv((HTTPIE_SOCK_T)sock, &buffer_rw[*size], (int)content_size, 0);
    } while((size_t)(&buffer_rw[*size] - *body_ptr) < content_size);

    *body_size = &buffer_rw[*size] - body_ptr;
    assert(*body_size == content_size);

    return 0;
}

long httpie_send(long long sock, const char* content, int size) {

    if (size < 1) {
        return 0;
    }

    long sent = 0;
    while ((sent = (long)send((HTTPIE_SOCK_T)sock, content, size, 0)) < size) {
        if (sent == SOCKET_ERROR) {
            return sent;
        }
        
        size -= sent;
    }
    return sent;
}
