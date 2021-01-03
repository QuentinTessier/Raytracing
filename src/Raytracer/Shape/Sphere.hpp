#pragma once

#include "Raytracer/Hit.hpp"

struct DODSphere {
    glm::vec3 center;
    float radius;
    DODMaterial mat;
};