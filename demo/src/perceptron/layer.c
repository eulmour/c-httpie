#include "layer.h"
#include "utils.h"
#include <assert.h>
#include <limits.h>

void layer_load(struct layer* layer, size_t width, size_t height) {

    memset(layer, 0, sizeof(struct layer));

    layer->width = width;
    layer->height = height;
    layer->size = width*height*sizeof(float);
    layer->data = (float*)malloc(layer->size);

    if (!layer->data) {
        abort();
    }

    memset(layer->data, 0, layer->size);
}

void layer_unload(struct layer* layer) {
    if (layer->size > 0) {
        free(layer->data);
    }
}

int layer_load_from_bin(struct layer* layer, const void* data, size_t width, size_t height) {

    layer->data = (float*)data;
    layer->size = width*height*sizeof(float);
    layer->width = width;
    layer->height = height;
    return 0;
}

int layer_load_from_bin_file(struct layer* layer, const char* path, size_t width, size_t height) {

    struct buf file_buffer = buf_from_file(path);
    if (file_buffer.size < 1) {
        return -1;
    }

    layer->data = (float*)file_buffer.data;
    layer->size = file_buffer.size;
    layer->width = width;
    layer->height = height;
    return 0;
}

int layer_load_from_png(struct layer* layer, const void* data, size_t size, size_t width, size_t height) {

    struct buf png = png_decode(data, (int)size);
    if (png.size < 1) {
        return -1;
    }

    // convert to 1 bit color
    for (float* current = (float*)png.data; current != (float*)&png.data[png.size]; current++) {
        *current = *current < 0.5f
            ? 0.f
            : 1.f;
    }

    layer->data = (float*)png.data;
    layer->size = png.size;
    layer->width = width;
    layer->height = height;

    return 0;
}

int layer_load_from_png_file(struct layer* layer, const char* path, size_t width, size_t height) {

    struct buf file_buffer = buf_from_file(path);
    if (file_buffer.size < 1) {
        return -1;
    }

    struct buf png = png_decode((unsigned char*)file_buffer.data, (int)file_buffer.size);
    if (png.size < 1) {
        return -1;
    }

    layer->data = (float*)png.data;
    layer->size = png.size;
    layer->width = width;
    layer->height = height;
    return 0;
}

int layer_save_as_bin_file(const struct layer* layer, const char* path) {

    return buf_to_file(&(struct buf) {
        .data = (char *) layer->data,
        .size = layer->size,
        .capacity = layer->size
    }, path);
}

int layer_save_as_png_file(const struct layer* layer, const char* path) {

    struct buf png = png_encode(
            (const unsigned char*)layer->data, (int)layer->width, (int)layer->height);

    if (png.size < 1) {
        return -1;
    }

    return buf_to_file(&png, path);
}

void layer_random_rect(struct layer* layer) {

    layer_fill_rect(layer, 0, 0, (int)layer->width, (int)layer->height, 0.0f);

    int x = RAND_RANGE(0, layer->width);
    int y = RAND_RANGE(0, layer->height);

    int w = (int)layer->width - x;
    if (w < 2) w = 2;
    w = RAND_RANGE(1, w);

    int h = (int)layer->height - x;
    if (h < 2) h = 2;
    h = RAND_RANGE(1, h);

    layer_fill_rect(layer, x, y, w, h, 1.0f);
}

void layer_random_circle(struct layer* layer) {

    layer_fill_rect(layer, 0, 0, (int)layer->width, (int)layer->height, 0.0f);
    int cx = RAND_RANGE(0, layer->width);
    int cy = RAND_RANGE(0, layer->height);
    int r = INT_MAX;
    if (r > cx) r = cx;
    if (r > cy) r = cy;
    if (r > (int)layer->width - cx) r = (int)layer->width - cx;
    if (r > (int)layer->height - cy) r = (int)layer->height - cy;
    if (r < 2) r = 2;
    r = RAND_RANGE(1, r);
    layer_fill_circle(layer, cx, cy, r, 1.0f);
}

void layer_fill_rect(struct layer* layer, int x, int y, int w, int h, float value) {

    assert(w > 0);
    assert(h > 0);

    int x0 = clampi(x, 0, (int)layer->width - 1);
    int y0 = clampi(y, 0, (int)layer->height - 1);
    int x1 = clampi(x0 + w - 1, 0, (int)layer->width - 1);
    int y1 = clampi(y0 + h - 1, 0, (int)layer->height - 1);

    for (int i = y0; i <= y1; ++i) {

        for (int j = x0; j <= x1; ++j) {
            *layer_at(layer, i, j) = value;
        }
    }
}

void layer_fill_circle(struct layer* layer, int cx, int cy, int r, float value) {

    assert(r > 0);
    int x0 = clampi(cx - r, 0, (int)layer->width-1);
    int y0 = clampi(cy - r, 0, (int)layer->height-1);
    int x1 = clampi(cx + r, 0, (int)layer->width-1);
    int y1 = clampi(cy + r, 0, (int)layer->height-1);

    for (int y = y0; y <= y1; ++y) {
        for (int x = x0; x <= x1; ++x) {
            int dx = x - cx;
            int dy = y - cy;
            if (dx*dx + dy*dy <= r*r) {
                *layer_at(layer, y, x) = value;
            }
        }
    }
}

float* layer_at(const struct layer* layer, size_t y, size_t x) {
    return &layer->data[y * layer->width + x];
}
