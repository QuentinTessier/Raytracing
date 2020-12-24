#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/vec_swizzle.hpp"
#include "Hit.hpp"

class Box : public hitable {
public:
    Box() {}
    Box(glm::vec3 center, float r, material *m)
        : center(center), size(r), mat_ptr(m)
    {
        trans = glm::translate(glm::mat4(1.0f), center);
    }

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

    glm::vec3 center;
    glm::mat4 trans;
    float size;
    material *mat_ptr;
};

bool Box::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
    glm::mat4 txi = glm::inverse(trans);
    glm::vec3 rdd = glm::xyz(trans * glm::vec4(r.direction(), 0.0f));
    glm::vec3 roo = glm::xyz(trans * glm::vec4(r.origin(), 1.0f));

    glm::vec3 m = 1.0f / rdd;
    glm::vec3 n = m * roo;
    glm::vec3 k = glm::abs(m) * size;
    glm::vec3 t1 = -n - k;
    glm::vec3 t2 = -n + k;
    float tN = std::max(std::max(t1.x, t1.y), t1.z);
    float tF = std::min(std::min(t2.x, t2.y), t2.z);
    if (tN > tF || tF < 0.0)
        return false;
    rec.t = tN;
    rec.p = r.point_at_parameter(rec.t);
    rec.normal = -sign(rdd) * glm::step(glm::yzx(t1), t1) * glm::step(glm::zxy(t1), t1);
    glm::vec4 tmp = (txi * glm::vec4(rec.normal, 0.0));
    rec.normal = glm::vec3(tmp.x, tmp.y, tmp.z);
    rec.mat_ptr = mat_ptr;
    return true;
}