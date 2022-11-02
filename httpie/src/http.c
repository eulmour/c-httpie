#include "http.h"
#include "framework.h"

// #define HTTP_CONTENT_STR_SIZE 20

const char* const HTTP_STR_METHOD[HTTP_METHOD_ENUM_END] = {
	NULL,
	"GET",
    "HEAD",
	"POST",
	"PUT",
    "DELETE",
    "CONNECT",
    "OPTIONS",
    "TRACE",
    "PATCH"
};

const char* const HTTP_STR_PROTOCOL[] = {
	NULL,
	"HTTP/1.0",
	"HTTP/1.1",
	"HTTP/2.0",
    "HTTP/3.0"
};

const char* const HTTP_STR_CONTENT[] = {
	"*/*",
	"text/plain",
	"text/html",
	"text/css",
	"application/javascript",
	"application/json",
	"application/wasm",
	"application/xml",
	"image/png",
	"image/jpeg",
	"image/webp",
	"image/vnd.microsoft.icon",
	"audio/mpeg",
	"audio/webm",
	"audio/mp4",
	"audio/ogg",
	"audio/aac",
	"video/mpeg",
	"video/mp4",
	"video/webm"
};

const char* const HTTP_STR_CONTENT_EXT[HTTP_CONTENT_ENUM_END] = {
    NULL,
    ".txt",
    ".html",
    ".css",
    ".js",
    ".json",
    ".wasm",
    ".xml",
    ".png",
    ".jpg",
    ".webp",
    ".ico",
    ".mp3",
    ".webm",
    ".mp4",
    ".ogg",
    ".aac",
    ".mpeg",
    ".mp4",
    ".webm",
};

// const char* const HTTP_STATUS_STR[HTTP_STATUS_CODE_ENUM_END] = {
// 	NULL,
// 	"HTTP/1.1 100 Continue"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 101 Switching Protocols"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 102 Processing"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 103 Early Hints"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 200 OK"								HTTP_STATUS_TAIL,
// 	"HTTP/1.1 201 Created"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 202 Accepted"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 203 Non-Authoritative Information"	HTTP_STATUS_TAIL,
// 	"HTTP/1.1 204 No Content"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 205 Reset Content"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 206 Partial Content"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 207 Multi-Status"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 208 Already Reported"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 226 Im Used"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 301 Moved Permanently"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 302 Moved Temporarily"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 303 See Other"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 304 Not Modified"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 305 Use Proxy"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 306 Reserved"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 307 Temporary Redirect"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 308 Permanent Redirect"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 400 Bad Request"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 401 Unauthorized"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 402 Payment Required"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 403 Forbidden"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 404 Not Found"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 405 Method Not Allowed"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 406 Not Acceptable"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 407 Proxy Authentication Required"	HTTP_STATUS_TAIL,
// 	"HTTP/1.1 408 Request Timeout"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 409 Conflict"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 410 Gone"								HTTP_STATUS_TAIL,
// 	"HTTP/1.1 411 Length Required"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 412 Precondition Failed"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 413 Payload Too Large"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 414 URL Too Long"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 415 Unsupported Media Type"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 416 Range Not Satisfiable"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 417 Expectation Failed"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 418 I'm a teapot"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 419 Authentication Timeout"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 421 Misdirected Request"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 422 Uprocessable Entity"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 423 Locked"							HTTP_STATUS_TAIL,
// 	"HTTP/1.1 424 Failed Dependency"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 425 Too Early"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 426 Upgrade Required"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 428 Precondition Required"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 429 Too Many Requests"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 431 Request Header Fields Too Large"	HTTP_STATUS_TAIL,
// 	"HTTP/1.1 449 Retry With"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 451 Unavailable For Legal Reasons"	HTTP_STATUS_TAIL,
// 	"HTTP/1.1 499 Client Closed Request"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 500 Internal Server Error"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 501 Not Implemented"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 502 Bad Gateway"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 503 Service Unavailable"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 504 Gateway Timeout"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 505 HTTP Version Not Supported"		HTTP_STATUS_TAIL,
// 	"HTTP/1.1 506 Variant Also Negotiated"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 507 Insufficient Storage"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 508 Loop Detected"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 509 Bandwidth Limit Exceeded"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 510 Not Extended"						HTTP_STATUS_TAIL,
// 	"HTTP/1.1 511 Network Authentication Required"	HTTP_STATUS_TAIL,
// 	"HTTP/1.1 520 Unknown Error"					HTTP_STATUS_TAIL,
// 	"HTTP/1.1 521 Web Server Is Down"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 522 Connection Timed Out"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 523 Origin Is Unreachable"			HTTP_STATUS_TAIL,
// 	"HTTP/1.1 524 A Timeout Occurred"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 525 SSL Handshake Failed"				HTTP_STATUS_TAIL,
// 	"HTTP/1.1 526 Invalid SSL Certificate"			HTTP_STATUS_TAIL,
// };

// static enum http_content_type http_content_type_get(const char* const name) {

    // (void)name;
	// const char str_hit[] = "Content-Type:";
    // const char* begin = strstr(name, str_hit);
	
    // for (int i = 1; i < HTTP_CONTENT_ENUM_END; i++) {
	// 	if (!strcmp(name, HTTP_STR_CONTENT[i])) {
	// 		return (enum http_content_type)i;
	// 	}
    // }

    // return HTTP_CONTENT_UNKNOWN;

// 	if (begin != NULL)
// 	{
// 		char buffer[HTTP_CONTENT_STR_SIZE];

// 		{
// 			begin += sizeof(str_hit) * sizeof(const char);
// 			unsigned int distance = strstr(begin, ";") - begin;
// //			memcpy_s(buffer, 20, begin, distance);
//             memcpy(buffer, begin, distance);
// 			buffer[distance] = '\0';
// 		}

// 		if (!strncmp(
// 			buffer,
// 			HTTP_CONTENT_STR[HTTP_CONTENT_TEXT_PLAIN],
// 			HTTP_CONTENT_STR_SIZE)) {
// 			return HTTP_CONTENT_TEXT_PLAIN;
// 		}

// 		if (!strncmp(
// 			buffer,
// 			HTTP_CONTENT_STR[HTTP_CONTENT_TEXT_HTML],
// 			HTTP_CONTENT_STR_SIZE)) {
// 			return HTTP_CONTENT_TEXT_HTML;
// 		}

// 		if (!strncmp(
// 			buffer,
// 			HTTP_CONTENT_STR[HTTP_CONTENT_TEXT_CSS],
// 			HTTP_CONTENT_STR_SIZE)) {
// 			return HTTP_CONTENT_TEXT_CSS;
// 		}

// 		if (!strncmp(
// 			buffer,
// 			HTTP_CONTENT_STR[HTTP_CONTENT_APP_JAVASCRIPT],
// 			HTTP_CONTENT_STR_SIZE)) {
// 			return HTTP_CONTENT_APP_JAVASCRIPT;
// 		}

// 		if (!strncmp(
// 			buffer,
// 			HTTP_CONTENT_STR[HTTP_CONTENT_IMG_JPEG],
// 			HTTP_CONTENT_STR_SIZE)) {
// 			return HTTP_CONTENT_IMG_JPEG;
// 		}

// 		if (!strncmp(
// 			buffer,
// 			HTTP_CONTENT_STR[HTTP_CONTENT_IMG_WEBP],
// 			HTTP_CONTENT_STR_SIZE)) {
// 			return HTTP_CONTENT_IMG_WEBP;
// 		}
// 	}
// 	else
// 	{
// 		return HTTP_CONTENT_UNKNOWN;
// 	}
//}

enum http_content_type http_content_from_filename(const char* const name) {

	const char* extension = strchr(name, '.');

    for (int i = 1; i < HTTP_CONTENT_ENUM_END; i++) {
        if (!strcmp(extension, HTTP_STR_CONTENT_EXT[i])) {
            return (enum http_content_type)i;
        }
    }

	return HTTP_CONTENT_UNKNOWN;
}

enum http_method http_method_from_str(const char* str) {

    for (int i = 1; i < HTTP_METHOD_ENUM_END; i++) {
        if (!strcmp(str, HTTP_STR_METHOD[i])) {
            return (enum http_method)i;
        }
    }

    return HTTP_METHOD_UNKNOWN;
}

enum http_protocol http_protocol_from_str(const char* str) {

    for (int i = 1; i < HTTP_PROTOCOL_ENUM_END; i++) {
        if (strcmp(str, HTTP_STR_PROTOCOL[i]) == 0) {
            return (enum http_protocol)i;
        }
    }

    return HTTP_PROTOCOL_UNKNOWN;
}
