#ifndef HTTPIE_ROUTE_H
#define HTTPIE_ROUTE_H

#include "request.h"

typedef struct response (*route_callback)(const struct request request);

struct route {
    const char* name;
    route_callback proc;
};

struct route *route_match(struct route* routes, const char *query_str);

#endif //HTTPIE_ROUTE_H
