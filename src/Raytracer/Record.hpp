#pragma once

#include "Ray.hpp"

struct Record {
    glm::vec3 normal;
    glm::vec3 p;
    glm::vec3 color;
    float t;
};