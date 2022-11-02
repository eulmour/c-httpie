#ifndef HTTPIE_PERCEPTRON_H
#define HTTPIE_PERCEPTRON_H

#include "layer.h"

struct perceptron {
    struct layer* weights;
    float learn_rate;
    float threshold;
};

void perceptron_load(struct perceptron* perceptron, struct layer* weights);
void perceptron_unload(struct perceptron* perceptron);
int perceptron_perceive(struct perceptron* perceptron, struct layer* inputs);
int perceptron_train(struct perceptron* perceptron, struct layer* inputs, int expected);

#endif //HTTPIE_PERCEPTRON_H
