#pragma once

#include <math.h>

typedef struct vec3 {
    float x, y, z;
} vec3;

inline __attribute__((always_inline)) float length(vec3 v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

inline __attribute__((always_inline)) float length2(vec3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline __attribute__((always_inline)) void make_unit_vector(vec3 *v)
{
    float k = 1.0 / sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);

    v->x *= k;
    v->y *= k;
    v->z *= k;
}

inline __attribute__((always_inline)) vec3 as_unit_vector(vec3 v)
{
    float k = 1.0 / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    return (vec3){v.x * k, v.y * k, v.z * k};
}

inline __attribute__((always_inline)) float dot(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}