#pragma once

#ifndef FILE_MAX_PATH
#define FILE_MAX_PATH 256
#endif

#include "framework.h"

enum path_type {
	PATH_UNKNOWN, PATH_ABSOLUTE, PATH_RELATIVE
};

struct path {
	enum path_type type;
	char* path_str;
	size_t path_str_size;
};

int file_exists(const char* path);
int cwd_get(size_t size, char* buffer);
const char* path_make_absolute(const char* file);
void path_get_absolute(struct path pth, char* buffer);
