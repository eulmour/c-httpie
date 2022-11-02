#include "utils.h"
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

int clampi(int x, int low, int high) {
    if (x < low)  x = low;
    if (x > high) x = high;
    return x;
}

struct buf png_decode(const unsigned char* data, int size) {

    int width = 0, height = 0, bpp = 0;
    stbi_uc* out = stbi_load_from_memory((const stbi_uc*)data, size, &width, &height, &bpp, 4);

    size_t new_size = width * height * bpp;

    if (out) {
        return (struct buf) {
            .data = (char*)out,
			.size = new_size,
			.capacity = new_size,
            .dyn = 1
        };
    } else {
		fprintf(stderr, "PNG decode error occurred\n");
		return (struct buf) {0};
    }
}

STBIWDEF unsigned char *stbi_write_png_to_mem(const unsigned char*, int, int, int, int, int*);
struct buf png_encode(const unsigned char* data, int width, int height) {

    int png_size = 0;
    int stride = 4;

	void* png = stbi_write_png_to_mem(data, width*stride, width, height, stride, &png_size);

    if (png) {
        return (struct buf) {
            .data = png,
            .size = (size_t)png_size,
            .capacity = png_size,
            .dyn = 1
        };
    } else {
		fprintf(stderr, "PNG encode occurred\n");
		return (struct buf) {0};
    }
}
