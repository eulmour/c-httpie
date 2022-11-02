#include "routes.h"
#include "controllers.h"

struct route routes[] = {
    {
        .name = "\\json",
        .proc = &route_json_test
    },
    {
        .name = "/generate",
        .proc = &route_ml_generate,
    },
    {
        .name = "/guess",
        .proc = &route_ml_guess,
    },
    {
        .name = "/train",
        .proc = &route_ml_train,
    },
    {
        .name = "/model_image",
        .proc = &route_ml_model_image,
    },

    { NULL }
};
