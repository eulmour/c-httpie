#ifndef HTTPIE_HTTP_H
#define HTTPIE_HTTP_H

#include "framework.h"

#if defined(HTTPIE_PROTOCOL_1_0)
#   define HTTPIE_PROTOCOL "HTTP/1.0"
#elif defined(HTTPIE_PROTOCOL_1_1)
#   define HTTPIE_PROTOCOL "HTTP/1.1"
#elif defined(HTTPIE_PROTOCOL_2_0)
#   define HTTPIE_PROTOCOL "HTTP/2.0"
#elif defined(HTTPIE_PROTOCOL_3_0)
#   define HTTPIE_PROTOCOL "HTTP/3.0"
#else
#   define HTTPIE_PROTOCOL "HTTP/1.1"
#endif

#define HTTP_STR_200_OK HTTPIE_PROTOCOL " 200 OK\r\n"
#define HTTP_STR_400_BAD_REQUEST HTTPIE_PROTOCOL " 400 Bad Request\r\n"
#define HTTP_STR_404_NOT_FOUND HTTPIE_PROTOCOL " 404 Not Found\r\n"
#define HTTP_STR_500_INTERNAL_SERVER_ERROR HTTPIE_PROTOCOL " 500 Internal Server Error\r\n"

#define HTTP_STR_CONTENT_UNKNOWN "Content-Type: */*\r\n\r\n"
#define HTTP_STR_CONTENT_TEXT_PLAIN "Content-Type: text/plain\r\n\r\n"
#define HTTP_STR_CONTENT_TEXT_HTML "Content-Type: text/html\r\n\r\n"
#define HTTP_STR_CONTENT_TEXT_CSS "Content-Type: text/css\r\n\r\n"
#define HTTP_STR_CONTENT_APP_JS "Content-Type: application/javascript\r\n\r\n"
#define HTTP_STR_CONTENT_APP_JSON "Content-Type: application/json\r\n\r\n"
#define HTTP_STR_CONTENT_APP_WASM "Content-Type: application/wasm\r\n\r\n"
#define HTTP_STR_CONTENT_APP_XML "Content-Type: application/xml\r\n\r\n"
#define HTTP_STR_CONTENT_IMG_PNG "Content-Type: image/png\r\n\r\n"
#define HTTP_STR_CONTENT_IMG_JPEG "Content-Type: image/jpeg\r\n\r\n"
#define HTTP_STR_CONTENT_IMG_ICO "Content-Type: image/vnd.microsoft.icon\r\n\r\n"
#define HTTP_STR_CONTENT_IMG_WEBP "Content-Type: image/webp\r\n\r\n"
#define HTTP_STR_CONTENT_AUDIO_AAC "Content-Type: audio/aac\r\n\r\n"
#define HTTP_STR_CONTENT_AUDIO_MPEG "Content-Type: audio/mpeg\r\n\r\n"
#define HTTP_STR_CONTENT_AUDIO_OGG "Content-Type: audio/ogg\r\n\r\n"
#define HTTP_STR_CONTENT_AUDIO_WEBM "Content-Type: audio/webm\r\n\r\n"
#define HTTP_STR_CONTENT_VIDEO_MPEG "Content-Type: video/mpeg\r\n\r\n"
#define HTTP_STR_CONTENT_VIDEO_MP4 "Content-Type: video/mp4\r\n\r\n"
#define HTTP_STR_CONTENT_VIDEO_WEBM "Content-Type: video/webm\r\n\r\n"

enum http_method {
    HTTP_METHOD_UNKNOWN = 0,
    HTTP_METHOD_GET,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_CONNECT,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_TRACE,
    HTTP_METHOD_PATCH,
    HTTP_METHOD_ENUM_END
};

enum http_protocol {
    HTTP_PROTOCOL_UNKNOWN = HTTP_METHOD_ENUM_END,
    HTTP_PROTOCOL_1_0,
    HTTP_PROTOCOL_1_1,
    HTTP_PROTOCOL_2_0,
    HTTP_PROTOCOL_3_0,
    HTTP_PROTOCOL_ENUM_END
};

enum http_content_type {
    HTTP_CONTENT_UNKNOWN = HTTP_PROTOCOL_ENUM_END,
    HTTP_CONTENT_TEXT_PLAIN,
    HTTP_CONTENT_TEXT_HTML,
    HTTP_CONTENT_TEXT_CSS,
    HTTP_CONTENT_APP_JAVASCRIPT,
    HTTP_CONTENT_APP_JSON,
    HTTP_CONTENT_APP_WASM,
    HTTP_CONTENT_APP_XML,
    HTTP_CONTENT_IMG_PNG,
    HTTP_CONTENT_IMG_JPEG,
    HTTP_CONTENT_IMG_WEBP,
    HTTP_CONTENT_IMG_ICON,
    HTTP_CONTENT_AUDIO_MPEG,
    HTTP_CONTENT_AUDIO_WEBM,
    HTTP_CONTENT_AUDIO_MP4,
    HTTP_CONTENT_AUDIO_OGG,
    HTTP_CONTENT_AUDIO_AAC,
    HTTP_CONTENT_VIDEO_MPEG,
    HTTP_CONTENT_VIDEO_MP4,
    HTTP_CONTENT_VIDEO_WEBM,
    HTTP_CONTENT_ENUM_END
};

enum http_status_code {
    HTTP_STATUS_CODE_UNKNOWN = HTTP_CONTENT_ENUM_END,
    HTTP_100_CONTINUE, // info
    HTTP_101_SWITCHING_PROTOCOLS,
    HTTP_102_PROCESSING,
    HTTP_103_EARLY_HINTS,
    HTTP_200_OK, // SUCCESS
    HTTP_201_CREATED,
    HTTP_202_ACCEPTED,
    HTTP_203_NON_AUTHORITATIVE_INFORMATION,
    HTTP_204_NO_CONTENT,
    HTTP_205_RESET_CONTENT,
    HTTP_206_PARTIAL_CONTENT,
    HTTP_207_MULTI_STATUS,
    HTTP_208_ALREADY_REPORTED,
    HTTP_226_IM_USED,
    HTTP_300_MULTIPLE_CHOICES, // REDIRECT
    HTTP_301_MOVED_PERMANENTLY,
    HTTP_302_MOVED_TEMPORARILY,
    HTTP_303_SEE_OTHER,
    HTTP_304_NOT_MODIFIED,
    HTTP_305_USE_PROXY,
    HTTP_306_RESERVED,
    HTTP_307_TEMPORARY_REDIRECT,
    HTTP_308_PERMANENT_REDIRECT,
    HTTP_400_BAD_REQUEST, // CLIENT ERROR
    HTTP_401_UNAUTHORIZED,
    HTTP_402_PAYMENT_REQUIRED,
    HTTP_403_FORBIDDEN,
    HTTP_404_NOT_FOUND,
    HTTP_405_METHOD_NOT_ALLOWED,
    HTTP_406_NOT_ACCEPTABLE,
    HTTP_407_PROXY_AUTHENTICATION_REQUIRED,
    HTTP_408_REQUEST_TIMEOUT,
    HTTP_409_CONFLICT,
    HTTP_410_GONE,
    HTTP_411_LENGTH_REQUIRED,
    HTTP_412_PRECONDITION_FAILED,
    HTTP_413_PAYLOAD_TOO_LARGE,
    HTTP_414_URI_TOO_LONG,
    HTTP_415_UNSUPPORTED_MEDIA_TYPE,
    HTTP_416_RANGE_NOT_SATISFIABLE,
    HTTP_417_EXPECTATION_FAILED,
    HTTP_418_I_AM_A_TEAPOT,
    HTTP_419_AUTHENTICATION_TIMEOUT,
    HTTP_421_MISDIRECTED_REQUEST,
    HTTP_422_UNPROCESSABLE_ENTITY,
    HTTP_423_LOCKED,
    HTTP_424_FAILED_DEPENDENCY,
    HTTP_425_TOO_EARLY,
    HTTP_426_UPGRADE_REQUIRED,
    HTTP_428_PRECONDITION_REQUIRED,
    HTTP_429_TOO_MANY_REQUESTS,
    HTTP_431_REQUEST_HEADER_FIELDS_TOO_LARGE,
    HTTP_449_RETRY_WITH,
    HTTP_451_UNAVAILABLE_FOR_LEGAL_REASONS,
    HTTP_499_CLIENT_CLOSED_REQUEST,
    HTTP_500_INTERNAL_SERVER_ERROR, // SERVER ERROR
    HTTP_501_NOT_IMPLEMENTED,
    HTTP_502_BAD_GATEWAY,
    HTTP_503_SERVICE_UNAVAILABLE,
    HTTP_504_GATEWAY_TIMEOUT,
    HTTP_505_HTTP_VERSION_NOT_SUPPORTED,
    HTTP_506_VARIANT_ALSO_NEGOTIATES,
    HTTP_507_INSUFFICIENT_STORAGE,
    HTTP_508_LOOP_DETECTED,
    HTTP_509_BANDWIDTH_LIMIT_EXCEEDED,
    HTTP_510_NOT_EXTENDED,
    HTTP_511_NETWORK_AUTHENTICATION_REQUIRED,
    HTTP_520_UNKNOWN_ERROR,
    HTTP_521_WEB_SERVER_IS_DOWN,
    HTTP_522_CONNECTION_TIMED_OUT,
    HTTP_523_ORIGIN_IS_UNREACHABLE,
    HTTP_524_A_TIMEOUT_OCCURRED,
    HTTP_525_SSL_HANDSHAKE_FAILED,
    HTTP_526_INVALID_SSL_CERTIFICATE,
    HTTP_STATUS_CODE_ENUM_END
};


enum http_content_encoding {
    HTTP_ENCODING_UNKNOWN
};

//enum response_status {
//    RESPONSE_RESULT_UNKNOWN,
//    RESPONSE_RESULT_OK,
//    RESPONSE_RESULT_NOT_FOUND,
//    RESPONSE_RESULT_ENUM_END
//};

extern const char* const HTTP_STR_METHOD[];
extern const char* const HTTP_STR_PROTOCOL[];
extern const char* const HTTP_STR_CONTENT[];
extern const char* const HTTP_STR_CONTENT_EXT[];

//extern const char* const HTTP_STR[];

extern struct http_def {
    const char* const str;
    size_t size;
} HTTP_DEF[];

enum http_method http_method_from_str(const char* str);
enum http_protocol http_protocol_from_str(const char* str);
enum http_content_type http_content_from_str(const char* str);
enum http_content_type http_content_from_filename(const char* name);

#endif
