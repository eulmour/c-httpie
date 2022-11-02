#include "httpie.h"
#include "routes.h"

static const char help_message[] =
    "c-httpie 0.1.0\n\n"
    "USAGE:\n"
        "\tmain [OPTIONS]\n\n"
    "FLAGS:\n"
        "\t-h, --help       Prints help information\n"
        "\t-V, --version    Prints version information\n\n"
    "OPTIONS:\n"
        "\t-a, --address <ADDRESS>          Sets address\n"
        "\t-p, --port <PORT>                Sets port\n"
        "\t-d, --directory <DIRECTORY>      Sets public directory. Example: -d www/\n\n"
    "c-httpie reads HTTPIE_ADDRESS environment variable";

int main(int argc, char* argv[]) {

    struct args {
        int help;
        int version;
        const char* address;
        const char* port;
        const char* directory;
    } args = { 0 };

    for (int i = 1; i < argc; i++) {

        if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "--address") == 0) {
            args.address = argv[1 + i++]; continue;
        }
        if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--port") == 0) {
            args.port = argv[1 + i++]; continue;
        }
        if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--directory") == 0) {
            args.directory = argv[1 + i++]; continue;
        }
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            args.help = 1; continue;
        }
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            args.version = 1; continue;
        }
    }

    if (args.help) {
        printf("%s\n", help_message);
        exit(EXIT_SUCCESS);
    }

    if (args.version) {
        puts("0.1.0");
        exit(EXIT_SUCCESS);
    }

    struct httpie server = {
        .address = args.address ? args.address : "127.0.0.1",
        .port = args.port ? atoi(args.port) : 8080,
        .routes = routes,
        .path_public = args.directory ? args.directory : "www/",
    };

	int res = httpie_load(&server);

	if (res != 0) return res;

	while (httpie_listen(&server));

	return httpie_unload(&server);
}