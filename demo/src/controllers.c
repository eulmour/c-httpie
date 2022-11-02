#include "controllers.h"
#include "perceptron/layer.h"
#include "perceptron/perceptron.h"

#include <sys/stat.h>

#include <time.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#if defined (_WIN32) || defined (_WIN64)
#   include <direct.h>
#   define MAKE_DIR(dir) _mkdir(dir)
#elif defined (__unix__) || defined (__unix)
#   define MAKE_DIR(dir) mkdir(dir, 0755)
#endif

STBIWDEF unsigned char *stbi_write_png_to_mem(const unsigned char*, int, int, int, int, int*);

#define LOCAL_LAYER_WIDTH 256
#define LOCAL_LAYER_HEIGHT 256

struct response route_hello(const struct request request) {

    (void)request;

    return response_static(
        HTTP_STR_200_OK,
        HTTP_STR_CONTENT_TEXT_HTML,
        "<!DOCTYPE html>"
        "<html>"
        "<head>"
            "<title>OK</title>"
        "</head>"
        "<body>"
            "<h1>Success</h1>Hello world"
        "</body>"
        "</html>"
    );
}

struct response route_json_test(struct request request) {

    (void)request;

    return response_static(
        HTTP_STR_200_OK,
        HTTP_STR_CONTENT_APP_JSON,
        "{\"label\":\"Test\",\"entries\": []}"
    );
}

struct response route_ml_generate(struct request request) {

    int layer_width = 256;
    int layer_height = 256;

    if (strcmp(request.query.params[0].key, "width") == 0)
        layer_width = atoi(request.query.params[0].value);
    if (strcmp(request.query.params[1].key, "height") == 0)
        layer_height = atoi(request.query.params[1].value);

    struct layer layer;
    layer_load(&layer, layer_width, layer_height);

    srand((unsigned int)time(NULL));

    if (rand() % 2 == 0) {
        layer_random_rect(&layer);
    } else {
        layer_random_circle(&layer);
    }

    unsigned char* image = (unsigned char*)malloc(layer.size);

    if (!image) {
        fprintf(stderr, "Unable to allocate memory.\n");
        return response_500_static_text("Unable to allocate memory.\n");
    }

    for (unsigned y = 0; y < (unsigned)layer_height; y++) {

        for (unsigned x = 0; x < (unsigned)layer_width; x++) {

            image[4 * layer_width * y + 4 * x + 0] = (unsigned char)*layer_at(&layer, y, x) * 255;
            image[4 * layer_width * y + 4 * x + 1] = (unsigned char)*layer_at(&layer, y, x) * 255;
            image[4 * layer_width * y + 4 * x + 2] = (unsigned char)*layer_at(&layer, y, x) * 255;
            image[4 * layer_width * y + 4 * x + 3] = 255;
        }
    }

	int png_size = 0;
	unsigned char* png_data = stbi_write_png_to_mem(
        (const unsigned char*)image, 0, layer_width, layer_height, 4, &png_size);

    if (png_data == NULL)
        return response_500_static_text("Unable to encode png file.");

    free(image);
    layer_unload(&layer);

    return (struct response) {
        .body = (struct buf) { .data = (char*)png_data, .size = png_size, .capacity = png_size, .dyn = 1 },
        .header = string_from_static(HTTP_STR_200_OK HTTP_STR_CONTENT_IMG_PNG),
    };
}

struct response route_ml_guess(struct request request) {

    const char* file_path = "models/model.dat";

    struct layer inputs = {0};
    struct layer weights = {0};
    struct perceptron perceptron = {0};
    struct response response;

    int layer_width = 256;
    int layer_height = 256;

    if (strcmp(request.query.params[0].key, "width") == 0)
        layer_width = atoi(request.query.params[0].value);
    if (strcmp(request.query.params[1].key, "height") == 0)
        layer_height = atoi(request.query.params[1].value);

    if (layer_load_from_png(&inputs, request.body, request.body_size, layer_width, layer_height) == -1) {
        response = response_500_static_text("Failed to process png image");
        goto finish;
    }

    if (layer_load_from_bin_file(&weights, file_path, layer_width, layer_height) == -1) {
        response = response_500_static_text("Model file is not found");
        goto finish;
    }

    perceptron_load(&perceptron, &weights);

    if (!request.body || request.body_size < 1) {
        response = RESPONSE_GET[RESPONSE_STATUS_BAD_REQUEST];
        goto finish;
    }
    if (inputs.size < 1) {
        response = response_400_static_text("No input data.");
        goto finish;
    }
    if (weights.size < 1) {
        response = response_404_static_text("The neuron didn't trained yet.");
        goto finish;
    }

    srand((unsigned int)501);

    int wish_result = perceptron_perceive(&perceptron, &inputs);

    struct buf result_buf = buf_alloc(64 * sizeof(char));
    result_buf.size = sprintf(result_buf.data, "This is probably %s", wish_result == 1 ? "circle" : "square") + 1;

    response = (struct response) {
        .body = result_buf,
        .header = string_from_static(HTTP_STR_200_OK HTTP_STR_CONTENT_TEXT_PLAIN)
    };

finish:
    perceptron_unload(&perceptron);
    layer_unload(&weights);
    layer_unload(&inputs);

    return response;
}

struct response route_ml_train(struct request request) {

    int layer_width = 256;
    int layer_height = 256;
    int train_passes = 5;
    int train_iterations = 200;
    int train_seed = 500;

    if (strcmp(request.query.params[0].key, "width") == 0)
        layer_width = atoi(request.query.params[0].value);
    if (strcmp(request.query.params[1].key, "height") == 0)
        layer_height = atoi(request.query.params[1].value);
    if (strcmp(request.query.params[2].key, "passes") == 0)
        train_passes = atoi(request.query.params[2].value);
    if (strcmp(request.query.params[3].key, "iter") == 0)
        train_iterations = atoi(request.query.params[3].value);
    if (strcmp(request.query.params[4].key, "seed") == 0)
        train_seed = atoi(request.query.params[4].value);

    const char* data_folder = "models";

    struct stat dir_info;
    if (stat(data_folder, &dir_info) != 0) {
        fprintf(stderr, "Cannot access %s. Creating directory.\n", data_folder);

        if (MAKE_DIR(data_folder) && errno != EEXIST) {
            fprintf(stderr, "Cannot create %s directory\n", data_folder);
            return response_500_static_text("Cannot create %s directory\n");
        }
    }

    struct layer inputs;
    struct layer weights;
    struct perceptron perceptron;

    layer_load(&inputs, layer_width, layer_height);
    layer_load(&weights, layer_width, layer_height);
    perceptron_load(&perceptron, &weights);

    struct buf out_buffer = buf_alloc(64 * train_passes * sizeof(char));
    char* current_buffer_pos = out_buffer.data;

    srand((unsigned int)train_seed);
    for (int i = 0; i < train_passes; ++i) {

        int adj = 0;
        for (int j = 0; j < train_iterations; ++j) {
            layer_random_rect(&inputs);
            adj += perceptron_train(&perceptron, &inputs, 0) ? 0 : 1;
            layer_random_circle(&inputs);
            adj += perceptron_train(&perceptron, &inputs, 1) ? 0 : 1;
        }

        current_buffer_pos += sprintf(current_buffer_pos, "[INFO] Pass %d: adjusted %d times\n", i, adj);

        if (adj < 1) break;
    }

    // save model
    layer_save_as_bin_file(perceptron.weights, "models/model.dat");

    current_buffer_pos += sprintf(
            current_buffer_pos, "[INFO] %zu bytes model file saved to models/model.dat\n", perceptron.weights->size);

    layer_save_as_png_file(perceptron.weights, "models/model.png");
    current_buffer_pos += sprintf(
            current_buffer_pos, "[INFO] Image file saved to models/model.png\n");

    out_buffer.size = current_buffer_pos - out_buffer.data;

    perceptron_unload(&perceptron);
    layer_unload(&weights);
    layer_unload(&inputs);

    return (struct response) {
        .body = out_buffer,
        .header = string_from_static(HTTP_STR_200_OK HTTP_STR_CONTENT_TEXT_PLAIN)
    };
}

struct response route_ml_model_image(struct request request) {

    struct buf image_file = buf_from_file("models/model.png");
    if (image_file.size < 1) {
        return response_404_static_text("Image is not available yet");
    }

    return (struct response) {
        .body = image_file,
        .header = string_from_static(HTTP_STR_200_OK HTTP_STR_CONTENT_IMG_PNG),
    };
}
