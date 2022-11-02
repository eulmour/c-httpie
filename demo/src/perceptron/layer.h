#ifndef HTTPIE_LAYER_H
#define HTTPIE_LAYER_H

#define LAYER_MAX_CAPACITY 1024 * 32
#define LAYER_SCALE 25.f
#define LAYER_COLOR_INTENSITY 255U
#define LAYER_RANGE 10.f
#define LAYER_DEFAULT_WIDTH 20
#define LAYER_DEFAULT_HEIGHT 20

#include <stdlib.h>
#include <stdbool.h>

struct layer {
    float* data;
    size_t width;
    size_t height;
    size_t size;
};

void layer_load(struct layer* layer, size_t width, size_t height);
void layer_unload(struct layer* layer);

int layer_load_from_bin(struct layer* layer, const void* data, size_t width, size_t height);
int layer_load_from_bin_file(struct layer* layer, const char* path, size_t width, size_t height);
int layer_load_from_png(struct layer* layer, const void* data, size_t size, size_t width, size_t height);
int layer_load_from_png_file(struct layer* layer, const char* path, size_t width, size_t height);

int layer_save_as_bin_file(const struct layer* layer, const char* path);
int layer_save_as_png_file(const struct layer* layer, const char* path);

void layer_random_rect(struct layer* layer);
void layer_random_circle(struct layer* layer);
void layer_fill_rect(struct layer* layer, int x, int y, int w, int h, float value);
void layer_fill_circle(struct layer* layer, int cx, int cy, int r, float value);
float* layer_at(const struct layer* layer, size_t y, size_t x);

#endif //HTTPIE_LAYER_H
