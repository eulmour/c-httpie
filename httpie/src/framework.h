#ifndef CORE_MAIN_H
#define CORE_MAIN_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "buf.h"

int httpie_msg(const char* fmt, ...);

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#ifdef __linux__

#include <unistd.h>

#define httpie_error(M, ...)\
    fprintf(stderr, "[ERROR] : [%s] : " M "\n", clean_errno(), ##__VA_ARGS__)

#elif defined(_WIN32) || defined(_WIN64)

#define httpie_error(msg, ...)\
    httpie_msg("[ERROR] : [%s]\nFile %s, line %d\n\n" msg, clean_errno(),  __FILE__, __LINE__, ##__VA_ARGS__);

#ifdef HTTPIE_INTERNAL
#define measure(function) \
{\
    LARGE_INTEGER frequency;\
    LARGE_INTEGER start;\
    LARGE_INTEGER end;\
    double interval;\
\
    QueryPerformanceFrequency(&frequency);\
    QueryPerformanceCounter(&start);\
\
    function;\
\
    QueryPerformanceCounter(&end);\
    interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;\
\
	TCHAR buffer[16];\
	const TCHAR* pStr = num_to_str(interval, buffer);\
	copy_chars("s.\r\n", pStr + 11);\
	ConsoleWrite(pStr, 16);\
}
#endif // HTTPIE_INTERNAL

#endif // _WIN32 || _WIN64

#define assertf(cond, format, ...) if(!(cond)) {httpie_error(format, ##__VA_ARGS__); assert(cond); }

char* base64_encode(const unsigned char* src, size_t len);
char* base64_decode(const void* data, size_t len);

#endif //CORE_MAIN_H
