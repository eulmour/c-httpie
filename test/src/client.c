#include <assert.h>
#include "client.h"

#if defined(_WIN32) || defined(_WIN64)
#   include <WinSock2.h>
#   include <ws2tcpip.h>
#   include <Windows.h> // put below winsock headers
    typedef SSIZE_T ssize_t;
#else 
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h> // inet_addr()
#   ifndef INVALID_SOCKET
#       define INVALID_SOCKET -1
#   endif
#   ifndef SOCKET_ERROR
#       define SOCKET_ERROR -1
#   endif
#endif

const char req[] = "GET / HTTP/1.1";

TEST_THREAD_RESULT client_start(void* data) {

    struct sockaddr_in local, client;
    int addr_len = sizeof(struct sockaddr_in);

#if defined(_WIN32) || defined(WIN32)
    SOCKET sock = INVALID_SOCKET, msg_sock;
    SSIZE_T sent;
#else
    int sock = INVALID_SOCKET, msg_sock = INVALID_SOCKET;
    ssize_t sent = SOCKET_ERROR;
#endif

    local = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
        .sin_port = htons(8080)
    };

    assert((sock = socket(AF_INET, SOCK_STREAM, 0)) != INVALID_SOCKET);
    assert(bind(sock, (const struct sockaddr *) &local, sizeof(local)) != SOCKET_ERROR);
    assert(listen(sock, 10) != SOCKET_ERROR);

    for (;;) {
        assert((msg_sock = accept(sock, (struct sockaddr*)&client, &addr_len)) != INVALID_SOCKET);
        assert((sent = send(msg_sock, req, (int)sizeof(req), 0)) != SOCKET_ERROR);
        // buffer.size += recv(msg_sock, &buffer.data[buffer.size], REQUEST_SIZE, 0);
    }
}