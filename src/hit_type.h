#include "hitable.h"

/* HIT_SPHERE */
typedef struct sphere {
    vec3 center;
    float radius;
} sphere;

inline __attribute__((always_inline)) sphere new_sphere(float radius, vec3 center)
{
    return (sphere){center, radius};
}

extern unsigned int hit_type_sizes[HIT_COUNT];