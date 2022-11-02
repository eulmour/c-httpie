#ifndef HTTPIE_CONTROLLERS_H
#define HTTPIE_CONTROLLERS_H

#include "httpie.h"

struct response route_json_test(struct request request);

struct response route_ml_generate(struct request request);
struct response route_ml_guess(struct request request);
struct response route_ml_train(struct request request);
struct response route_ml_model_image(struct request request);

void route_ml_generate_free(void*);
void route_ml_guess_free(void*);
void route_ml_train_free(void*);

#endif