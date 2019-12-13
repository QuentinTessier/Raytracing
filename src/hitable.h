#pragma once

#include "ray.h"

typedef short int hitable;

enum hitable_tags {
    HIT_ERROR = -1,
    HIT_SPHERE,
    HIT_COUNT
};

typedef struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
} hit_record;

typedef char (*hitable_func)(const ray r, float tmin, float tmax, hit_record *rec);

extern hitable_func hit[HIT_COUNT];