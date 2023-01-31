#include "buf.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

struct buf buf_alloc(size_t size) {

    if (size < 1) {
        return (struct buf) {0};
    }

    void* data = malloc(size);

    if (!data) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return (struct buf) { 0 };
    }

    memset(data, 0, size);

    return (struct buf) {
        .data = (char*)data,
        .size = 0,
        .capacity = size,
        .dyn = 1
    };
}

struct buf buf_new(const void* data, size_t size) {

    void* mem = malloc(size);
    
    if (!mem) {
        fprintf(stderr, "Cannot allocate memory\n");
        abort();
    }

    memcpy(mem, data, size);

    return (struct buf) {
        .data = mem,
        .size = size,
        .dyn = 1
    };
}

struct buf buf_from_file(const char* path) {

#if defined(_WIN32) || defined(_WIN64)
    FILE* file;
	fopen_s(&file, path, "rb");
#else
    FILE* file = fopen(path, "rb");
#endif

    if (!file) {
        return (struct buf){0};
    }

    fseek(file, 0L, SEEK_END);
    const long file_size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* file_buffer = (char*)malloc((size_t)file_size);

    if (!file_buffer) {
        fprintf(stderr, "Cannot allocate memory");
        return (struct buf){0};
    }

    fread((void*)file_buffer, file_size, sizeof(const char), file);
    fclose(file);

    return (struct buf) {
        .data = file_buffer,
        .size = file_size,
        .capacity = file_size,
        .dyn = 1
    };
}

int buf_to_file(struct buf* buffer, const char* path) {

    FILE* f = fopen(path, "wb"); if (!f) { return -1; }
    fwrite(buffer->data, buffer->size, 1, f);
    fclose(f);

    return 0;
}

int buf_append(struct buf* dest, const char* source, size_t source_size) {

    if (
        !dest
        || !dest->data
        || !source
        || source_size < 1
        || (dest->size + source_size) > dest->capacity
        )
    {
        return -1;
    }

    memcpy(dest->data + dest->size, source, source_size);
    dest->size += source_size;
    return 0;
}

void buf_append_realloc(struct buf* dest, struct buf* source) {

    if (!dest || !dest->data || !source || !source->data || source->size < 1) {
        return;
    }

    size_t dest_size = dest->size * sizeof(*dest->data);
    size_t source_size = source->size * sizeof(*source->data);
    dest->data = (char*)realloc(dest->data, dest_size + source_size + sizeof(*dest->data));

    memcpy(dest->data + dest_size, source->data, source_size + sizeof(*source->data));
}

void buf_free(struct buf *string) {

    if (string->data != NULL) {
		free(string->data);
    }

    memset(string, 0, sizeof(struct buf));
}
