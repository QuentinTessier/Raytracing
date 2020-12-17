#pragma once

#include "Hit.hpp"

class Box : public hitable {
public:
    Box() {}
    Box(vec3 center, float r, material *m)
        : center(center), size(r), mat_ptr(m)
    { }

    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;

    vec3 center;
    float size;
    material *mat_ptr;
};

bool Box::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
    vec3 m = rcp(r.direction());
    vec3 n = m * r.origin();
    vec3 k = abs(m) * size;
    vec3 t1 = -n - k;
    vec3 t2 = -n + k;
    float tN = std::max(std::max(t1.x(), t1.y()), t1.z());
    float tF = std::min(std::min(t2.x(), t2.y()), t2.z());
    if (tN > tF || tF < 0.0)
        return false;
    rec.t = tN;
    rec.p = r.point_at_parameter(rec.t);
    rec.normal = -sign(r.direction()) * step(t1.yzx(), t1) * step(t1.zxy(), t1);
    rec.mat_ptr = mat_ptr;
    return true;
}