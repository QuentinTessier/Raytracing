#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vec_swizzle.hpp"
#include "Raytracer/Hit.hpp"

struct DODBox {
    glm::vec3 center;
    float size;
    DODMaterial mat;
};