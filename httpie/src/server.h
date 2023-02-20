#ifndef HTTPIE_SERVER_H
#define HTTPIE_SERVER_H

#include "http.h"
#include "request.h"
#include "route.h"
#include <stdint.h>

struct httpie_internal;
struct httpie {
	const char* address;
	uint16_t port;
	struct route* routes;
	const char* path_public;
    void* user_data;
	struct httpie_internal* internal;
};

int httpie_load(struct httpie *server);
int httpie_unload(struct httpie *server);
int httpie_job(struct httpie *server);
int httpie_listen(struct httpie *server);
int httpie_recv(long long sock, char** buffer, size_t* size, size_t* body_size);
long httpie_send(long long sock, const char* content, int size);

#endif
