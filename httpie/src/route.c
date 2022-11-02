#include "route.h"
#include <stddef.h>

static int path_match(const char *str1, const char *str2) {
    for (; *str1 != '\0'; str1++, str2++) {
        if (*str1 != *str2) {
            if ((*str1 == '\\') || (*str1 == '/')) {
                continue;
            }
            return 0;
        }
    }

    return *str2 == '\0' ? 1 : 0;
}

struct route *route_match(struct route* routes, const char *query_str) {

    for (struct route *r = routes; r->name != NULL; r++) {
        if (path_match(r->name, query_str)) {
            return r;
        }
    }

    return (struct route *) NULL;
}
