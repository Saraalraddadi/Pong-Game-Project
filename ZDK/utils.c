
#include "utils.h"

float min(float x, float y) {
    return x > y ? y : x;
}

float max(float x, float y) {
    return x < y ? y : x;
}

float clamp(float bot, float x, float top) {
    return min(top, max(x, bot));
}

float sqr(float x) {
    return x * x;
}

float cube(float x) {
    return x * x * x;
}
