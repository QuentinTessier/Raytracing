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
    double closest_so_fars;
    double closest_so_farb;

    for (size_t i = 0; i < BoxList.size(); ++i) {
        if (DODBox_hit(r, BoxList[i], tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            closest_so_farb= closest_so_far;
            rec = temp_rec;
        }
    }
    for (size_t i = 0; i < SphereList.size(); ++i) {
        if (DODSphere_hit(r, SphereList[i], tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            closest_so_fars = closest_so_far;
            rec = temp_rec;
        }
    }
    return hit_anything;
}