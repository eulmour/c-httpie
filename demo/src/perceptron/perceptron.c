#include "perceptron.h"
#include <memory.h>

#define PERCEPTRON_ERROR -1
#define PERCEPTRON_FAIL 0
#define PERCEPTRON_PASS 1

void perceptron_load(struct perceptron* perceptron, struct layer* weights) {
    memset(perceptron, 0, sizeof(struct perceptron));
    perceptron->learn_rate = 0.03f;
    perceptron->threshold = 1.f;
    perceptron->weights = weights;
}

void perceptron_unload(struct perceptron* perceptron) {
    memset(perceptron, 0, sizeof(struct perceptron));
}

int perceptron_perceive(struct perceptron* perceptron, struct layer* inputs) {

    if (inputs->width != perceptron->weights->width || inputs->height != perceptron->weights->height) {
        return PERCEPTRON_ERROR;
    }

    float result = perceptron->threshold;

    for (size_t y = 0; y < perceptron->weights->height; ++y) {
        for (size_t x = 0; x < perceptron->weights->width; ++x) {
            result += (*layer_at(inputs, y, x)) * (*layer_at(perceptron->weights, y, x));
        }
    }

    return result < 0.f ? PERCEPTRON_FAIL : PERCEPTRON_PASS;
}

int perceptron_train(struct perceptron* perceptron, struct layer* inputs, int expected) {

    int result;
    if ((result = perceptron_perceive(perceptron, inputs)) == PERCEPTRON_ERROR) {
        return result;
    } else if (result == expected) {
        return PERCEPTRON_PASS;
    }

    perceptron->threshold += perceptron->learn_rate * (expected - result);

    for (size_t y = 0; y < perceptron->weights->height; ++y) {
        for (size_t x = 0; x < perceptron->weights->width; ++x) {
            *layer_at(perceptron->weights, y, x)
                += perceptron->learn_rate
                * ((float)expected - (float)result)
                * (*layer_at(inputs, y, x));
        }
    }

    return PERCEPTRON_FAIL;
}