#ifndef HTTPIE_BUF_H
#define HTTPIE_BUF_H

#include <stddef.h>

struct buf {
    char* data;
    size_t size; // -1 for string size
    size_t capacity;
    int dyn;
};

#define string_from_local(text, sz)\
    (struct buf) { .data = (text), .size = (sz), .capacity = (sz) }

#define string_from_static(text)\
    (struct buf) {\
        .data = (text),\
        .size = sizeof(text),\
    }

#define buf_from_mem(ptr, sz)\
    (struct buf) { .data = (ptr), .size = (sz), .capacity = (sz), .dyn = 1 }

struct buf buf_alloc(size_t size);
struct buf buf_new(const void* data, size_t size);

struct buf buf_from_file(const char* path);
int buf_to_file(struct buf* buffer, const char* path);

int buf_append(struct buf* dest, const char* source, size_t source_size);
void buf_append_realloc(struct buf* dest, struct buf* source);
void buf_free(struct buf *buffer);

#endif
