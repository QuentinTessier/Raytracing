#pragma once

#include "Ray.hpp"

enum DODMaterialTypes {
    DM_LAMBERTIAN,
    DM_METAL,
    DM_DIELETRIC
};

struct DODMaterial {
    DODMaterial() = default;
    DODMaterial(DODMaterialTypes type, glm::vec3 albedo, float fuzz = 0, float ref_idx = 0)
        : type(type), albedo(albedo), fuzz(fuzz), refraction_index(ref_idx)
    { }

    DODMaterialTypes type;
    glm::vec3 albedo;
    float fuzz;
    float refraction_index;
};

struct DODHitRecord {
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    DODMaterial mat;
};

class hitable {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, DODHitRecord& rec) const = 0;
};