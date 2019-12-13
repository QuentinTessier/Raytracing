#pragma once

#include "vec3.h"

typedef struct ray {
    vec3 origin;
    vec3 direction;
} ray;

inline __attribute__((always_inline)) vec3 point_at_parameter(ray r, float t)
{
    return (vec3){
        r.origin.x + t * r.direction.x,
        r.origin.z + t * r.direction.z,
        r.origin.y + t * r.direction.y
    };
}