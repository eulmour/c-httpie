#ifndef HTTPIE_UTILS_H
#define HTTPIE_UTILS_H

#include "httpie.h"

#define RAND_RANGE(low, high) (rand() % ((high) - (low)) + (low))

int clampi(int x, int low, int high);

struct buf png_decode(const unsigned char* data, int size);
struct buf png_encode(const unsigned char* data, int width, int height);

#endif //HTTPIE_UTILS_H
