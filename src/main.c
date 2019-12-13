#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "hit_list.h"
#include "hit_type.h"

float hit_sphere(const vec3 center, float radius, const ray r)
{
    vec3 oc = (vec3){
        r.origin.x - center.x,
        r.origin.y - center.y,
        r.origin.z - center.z
    };
    float a = dot(r.direction, r.direction);
    float b = 2.0f * dot(oc, r.direction);
    float c = dot(oc, oc) - radius * radius;
    float discrim = b * b - 4 * a * c;
    if (discrim < 0) {
        return -1.f;
    } else {
        return (-b - sqrtf(discrim)) / (2.0 * a);
    }
}

vec3 color(const ray r)
{
    float t = hit_sphere((vec3){0.f, 0.f, -1.f}, 0.5f, r);
    if (t > 0.0f) {
        vec3 tmp = point_at_parameter(r, t);
        vec3 N = as_unit_vector((vec3){
            tmp.x,
            tmp.y,
            tmp.z - (-1.f)
        });
        return (vec3){
            0.5f * (N.x + 1),
            0.5f * (N.y + 1),
            0.5f * (N.z + 1),
        };
    }
    vec3 unit_direction = as_unit_vector(r.direction);
    t = 0.5 * (unit_direction.y + 1.0);
    return (vec3){
        (1.0 - t) * 1.0f + t * 0.5f,
        (1.0 - t) * 1.0f + t * 0.7f,
        (1.0 - t) * 1.0f + t * 1.0f
    };
}

int main()
{
    int nx = 200;
    int ny = 100;

    size_t count = 0;
    char *image_memory = malloc(sizeof(char) * nx * ny * 3);
    if (image_memory == 0)
        return 1;


    vec3 llc = (vec3){-2.0f, -1.0f, -1.0f};
    vec3 hori = (vec3){4.0f, 0.0f, 0.0f};
    vec3 vert = (vec3){0.0f, 2.0f, 0.0f};
    vec3 o = (vec3){0.0f, 0.0f, 0.0f};

    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            float u = (float)i / (float)nx;
            float v = (float)j / (float)ny;
            float b = 0.2f;
            ray r = (ray){o, (vec3){
                llc.x + u * hori.x + v * vert.x,
                llc.y + u * hori.y + v * vert.y,
                llc.z + u * hori.z + v * vert.z
            }};
            vec3 col = color(r);
            int ir = (int)(255.99 * col.x);
            int ig = (int)(255.99 * col.y);
            int ib = (int)(255.99 * col.z);

            image_memory[count + 0] = ir;
            image_memory[count + 1] = ig;
            image_memory[count + 2] = ib;
            count += 3;
        }
    }
    stbi_write_png("test.png", nx, ny, 3, image_memory, nx * 3);
    free(image_memory);
    return 0;
}