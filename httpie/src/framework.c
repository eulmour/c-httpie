#include "framework.h"

#ifdef __linux__

#include <stdarg.h>

int httpie_msg(const char* fmt, ...) {

	size_t size;
	va_list args, tmp_args;
	char* str = 0;

	va_start(args, fmt);
	va_copy(tmp_args, args);
    size = vsnprintf(NULL, 0, fmt, tmp_args) + 1;
	va_end(tmp_args);

	if (size > 0) {
		str = (char*)malloc(size);
		if (vsnprintf(str, size, fmt, args) < 0) {
			str[size-1] = 0;
		}

        write(STDOUT_FILENO, str, size);
        free(str);
	}

	va_end(args);
	return 0;
}

#elif defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

int httpie_msg(const char* fmt, ...) {

	size_t size;
	va_list args, tmp_args;

	va_start(args, fmt);
	va_copy(tmp_args, args);
	size = _vscprintf(fmt, tmp_args) + 1;
	va_end(tmp_args);

	if (size > 0) {

        char* data = 0;
        if ((data = (char*)malloc(size)) != NULL) {
            va_end(args);
            return -1;
        }

		if (vsnprintf_s(data, size, _TRUNCATE, fmt, args) < 0) {
			data[size-1] = 0;
		}

        MessageBoxA(NULL, data, "Message", MB_OK);
        free(data);
	}

	va_end(args);
	return 0;
}

#endif

char* base64_encode(const unsigned char* src, size_t len) {
    static const unsigned char base64_table[65] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    unsigned char *out, *pos;
    const unsigned char *end, *in;

    size_t olen;

    olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len) {
        return NULL; /* integer overflow */
    }

    char* outStr = (char*)malloc(olen * sizeof(char));
    if (!outStr) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return NULL;
    }

    out = (unsigned char*)&outStr[0];

    end = src + len;
    in = src;
    pos = out;
    while (end - in >= 3) {
        *pos++ = base64_table[in[0] >> 2];
        *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = base64_table[in[2] & 0x3f];
        in += 3;
    }

    if (end - in) {
        *pos++ = base64_table[in[0] >> 2];
        if (end - in == 1) {
            *pos++ = base64_table[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else {
            *pos++ = base64_table[((in[0] & 0x03) << 4) |
                                  (in[1] >> 4)];
            *pos++ = base64_table[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return outStr;
}

char* base64_decode(const void* data, size_t len) {

    static const int B64index[256] = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
        7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
        0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
    };

    unsigned char* p = (unsigned char*)data;

    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;

    size_t str_size = (L / 4 * 3 + pad) * sizeof(char);
    char* str = (char*)malloc(str_size);

    if (!str) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return NULL;
    }

    size_t j = 0;
    for (size_t i = 0; i < L; i += 4) {

        int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
        str[j++] = (char)(n >> 16);
        str[j++] = (char)(n >> 8 & 0xFF);
        str[j++] = (char)(n & 0xFF);
    }

    if (pad) {

        int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
        str[str_size - 1] = (char)(n >> 16);

        if (len > L + 2 && p[L + 2] != '=') {
            n |= B64index[p[L + 2]] << 6;

            str[j++] = n >> 8 & 0xFF;
        }
    }

    return str;
}
