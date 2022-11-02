#include "httpie.h"
#include <assert.h>
#include "main.h"
#include "client.h"

typedef char* (*test_func)();

struct response route_json(struct request request) {
    return response_200_static_text("Ok");
}

static char* main_test() {

    static struct route routes[] = {
        {
            .name = "\\json",
            .proc = &route_json
        },
        { NULL }
    };

    struct httpie server = {
        .address = "127.0.0.1",
        .port = 8080,
        .routes = routes,
        .path_public = "www/",
    };

    TEST_THREAD_CREATE(client_start, NULL);

	mu_assert(httpie_load(&server) == 0);

	while (httpie_listen(&server));

	mu_assert(httpie_unload(&server) == 0);
    return 0;
}

int main(int argc, char* argv[]) {

    fprintf(stderr, "Test is not implementerd yet");
    return EXIT_SUCCESS;
    
    test_func funcs[]  = {
        main_test,
        NULL
    };

    for (test_func* i = &funcs[0]; i != NULL; i++) {
        const char* result = (*i)();
        if (result) {
            fprintf(stderr, "%s", result);
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
