#include "file.h"

#ifdef __linux__
#	define SLASH '/'

int file_exists(const char *path) {

    if (access( path, F_OK ) == 0) {
        return 1;
    } else {
        return 0;
    }
}

int cwd_get(size_t size, char* buffer) {

    (void)buffer;
	long path_max;
	char *buf;
	char *ptr;

	path_max = pathconf(".", _PC_PATH_MAX);
	if (path_max == -1) {
		size = 1024;
	} else if (path_max > 10240) {
		size = 10240;
	} else {
		size = path_max;
	}

	for (buf = ptr = NULL; ptr == NULL; size *= 2) {
		if ((buf = realloc(buf, size)) == NULL) {
            return -1;
		}

		ptr = getcwd(buf, size);
		if (ptr == NULL && errno != ERANGE) {
            return -1;
		}
	}

	free (buf);
}

#elif defined(_WIN32) || defined(_WIN64)
#	include "Windows.h"
#	define SLASH '\\'

int file_exists(const char* path) {
	DWORD dwAttrib = GetFileAttributes(path);
	return (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

int cwd_get(size_t size, char* buffer) {
	return GetCurrentDirectoryA((DWORD)size, buffer);
}

#endif

const char* path_make_absolute(const char* file) {

	char* path_buffer = (char*)malloc(FILE_MAX_PATH * sizeof(const char));
	int path_size = cwd_get(FILE_MAX_PATH, path_buffer);
	
	memcpy(&path_buffer[path_size], file, strlen(file) + 1);
	return path_buffer;
}

void path_get_absolute(struct path pth, char* buffer) {

	switch (pth.type) {
		case PATH_UNKNOWN: return;
		case PATH_ABSOLUTE:
			memcpy(buffer, (const void*)pth.path_str, pth.path_str_size);
			return;
		case PATH_RELATIVE: {
			char* buffer_ptr = buffer;
			const int cwd_length = cwd_get(FILE_MAX_PATH, buffer_ptr);
			buffer_ptr += cwd_length;
			*buffer_ptr++ = SLASH;
			memcpy(buffer_ptr, pth.path_str, pth.path_str_size);
			*(buffer_ptr + pth.path_str_size) = '\n';
			return;
		}
	}
}
