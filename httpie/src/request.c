#include "request.h"
#include "http.h"

/**
 * This noise parses request string to create a request structure
 */
int request_get(struct request* request, const char* str_request) {

    memset(request, 0, sizeof(struct request));

    // 1. extract method
	char retval[8] = { 0 };
    sscanf(str_request, "%7s ", retval);
    request->method = http_method_from_str(retval);

    if (request->method == HTTP_METHOD_UNKNOWN) {
        return -1;
    }

    // 2. extract path
    sscanf(str_request, "%*s %247s ", request->query.path);
    request->query.path_size = strlen(request->query.path);

    // 3. extract protocol
	char str_buf[16] = { 0 };
    sscanf(str_request, "%*s %*s %15s\r\n", str_buf);
    request->protocol = http_protocol_from_str(str_buf);

    // 4. extract parameters if provided
    char* request_parameter_str_pos = NULL;
    if ((request_parameter_str_pos = strstr(request->query.path, "?"))) {

        *request_parameter_str_pos = '\0';
        request->query.path_size = request_parameter_str_pos - request->query.path;

        for (int i = 0; ; i++) {

            struct query_parameter parameter = {0};

            request_parameter_str_pos++;
            char* delim = strstr(request_parameter_str_pos, "=");

            parameter.key_size = delim - request_parameter_str_pos; // TODO sub can overflow
            memcpy(parameter.key, request_parameter_str_pos, parameter.key_size);

            request_parameter_str_pos = strstr(request_parameter_str_pos, "&");

            if (!delim) {
                continue;
            }

            parameter.value_size = request_parameter_str_pos
                ? (size_t)(request_parameter_str_pos - (delim + 1))
                : strlen(delim + 1);

            memcpy(parameter.value, delim + 1, parameter.value_size);

            request->query.params[i] = parameter;
            request->query.params_size++;

            if (!request_parameter_str_pos) {
                break;
            }
        }
    }

    return 0;
}
