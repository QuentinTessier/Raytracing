/*
** EPITECH PROJECT, 2019
** RogueLike
** File description:
** HitList
*/

#pragma once

#include <vector>
#include "Hit.hpp"
#include "Shape/Box.hpp"
#include "Shape/Sphere.hpp"

static inline bool DODBox_hit(const ray& r, DODBox const& b, float tmin, float tmax, DODHitRecord& rec)
{
    glm::mat4 trans = glm::translate(glm::mat4(1.0f), b.center);
    glm::mat4 txi = glm::inverse(trans);
    glm::vec3 rdd = glm::xyz(trans * glm::vec4(r.direction(), 0.0f));
    glm::vec3 roo = glm::xyz(trans * glm::vec4(r.origin(), 1.0f));

    glm::vec3 m = 1.0f / rdd;
    glm::vec3 n = m * roo;
    glm::vec3 k = glm::abs(m) * b.size;
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
    rec.mat = b.mat;
    return true;
}

static inline bool DODBox_hit2(const ray& r, DODBox const& b, float tmin, float tmax, DODHitRecord& rec)
{
    glm::vec3 oc = r.origin() - b.center;

//    float winding = glm::max(glm::abs(r.origin()) * (1 / (b.size / 2))) < 1.0 ? -1 : 1;
    float winding = 1;

    glm::vec3 sgn = -glm::sign(r.direction());
    glm::vec3 radius = glm::vec3(b.size, b.size, b.size);
    glm::vec3 d = radius * winding * sgn - r.origin();
    d *= glm::vec3(1 / r.direction().x, 1 / r.direction().y, 1 / r.direction().z);

    glm::bvec3 test = glm::bvec3(
        glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().y, r.origin().z) + glm::vec2(r.direction().y, r.direction().z) * d.x), glm::vec2(radius.y, radius.z))),
        glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().z, r.origin().x) + glm::vec2(r.direction().z, r.direction().x) * d.y), glm::vec2(radius.z, radius.x))),
        glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().x, r.origin().y) + glm::vec2(r.direction().x, r.direction().y) * d.z), glm::vec2(radius.x, radius.y)))
    );
    sgn = test.x ? glm::vec3(sgn.x, 0, 0) : (test.y ? glm::vec3(0, sgn.y, 0) : glm::vec3(0, 0, test.z ? sgn.z : 0));

    rec.t = (sgn.x != 0) ? d.x : ((sgn.y != 0) ? d.y : d.z);
    rec.p = r.point_at_parameter(rec.t);
    rec.normal = sgn;
    rec.mat = b.mat;
    return (sgn.x != 0.f) || (sgn.y != 0.f) || (sgn.z != 0.f);
}

static inline bool DODSphere_hit(const ray& r, DODSphere const& s, float tmin, float tmax, DODHitRecord& rec)
{
    glm::vec3 oc = r.origin() - s.center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - s.radius * s.radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(b * b - a * c)) / a;
        if (temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - s.center) / s.radius;
            rec.mat = s.mat;
            return (true);
        }
        temp = (-b + sqrt(b * b - a * c)) / a;
        if (temp > tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - s.center) / s.radius;
            rec.mat = s.mat;
            return (true);
        }
    }
    return (false);
}

class DODHitList {
    public:
        bool traverse(const ray& r, float tmin, float tmax, DODHitRecord& rec) const;

        std::vector<DODBox> BoxList;
        std::vector<DODSphere> SphereList;
};

bool DODHitList::traverse(const ray& r, float tmin, float tmax, DODHitRecord& rec) const
{
    DODHitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = tmax;

    for (size_t i = 0; i < SphereList.size(); ++i) {
        if (DODSphere_hit(r, SphereList[i], tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    for (size_t i = 0; i < BoxList.size(); ++i) {
        if (DODBox_hit(r, BoxList[i], tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}