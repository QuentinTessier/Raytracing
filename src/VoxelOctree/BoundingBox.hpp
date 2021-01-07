#pragma once

#include "Raytracer/Record.hpp"

struct BoundingBox {
    glm::vec3 center;
    float size;

    BoundingBox() = default;

    BoundingBox(glm::vec3 center, float size)
        : center(center), size(size)
    { }

    BoundingBox(BoundingBox const& o)
        : center(o.center), size(o.size)
    { }
    BoundingBox& operator=(BoundingBox const& o)
    { center = o.center; size = o.size; return *this; }

    glm::vec3 max() const { return center + size; }
    glm::vec3 min() const { return center - size; }

    inline bool Intersect(ray const& r, float tmin, float tmax, Record& rec) const
    {
        glm::vec3 oc = r.origin() - center;
        // float winding = glm::max(glm::abs(r.origin()) * (1.f / (size / 2))) < 1.0 ? -1 : 1;
        float winding = 1;
        glm::vec3 sgn = -glm::sign(r.direction());
        glm::vec3 radius = glm::vec3(size, size, size);
        glm::vec3 d = radius * winding * sgn - r.origin();
        d *= r.idirection();

        glm::bvec3 test = glm::bvec3(
            glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().y, r.origin().z) + glm::vec2(r.direction().y, r.direction().z) * d.x), glm::vec2(radius.y, radius.z))),
            glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().z, r.origin().x) + glm::vec2(r.direction().z, r.direction().x) * d.y), glm::vec2(radius.z, radius.x))),
            glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().x, r.origin().y) + glm::vec2(r.direction().x, r.direction().y) * d.z), glm::vec2(radius.x, radius.y)))
        );
        sgn = test.x ? glm::vec3(sgn.x, 0, 0) : (test.y ? glm::vec3(0, sgn.y, 0) : glm::vec3(0, 0, test.z ? sgn.z : 0));

        rec.t = (sgn.x != 0) ? d.x : ((sgn.y != 0) ? d.y : d.z);
        rec.p = r.point_at_parameter(rec.t);
        rec.normal = sgn;
        rec.color = glm::vec3(1, 1, 1);

        return (sgn.x != 0.f) || (sgn.y != 0.f) || (sgn.z != 0.f);
    }

    bool Intersect(ray const& r, float tmin, float tmax) const
    {
        glm::vec3 oc = r.origin() - center;
        // float winding = glm::max(glm::abs(r.origin()) * (1.f / (size / 2))) < 1.0 ? -1 : 1;
        float winding = 1;
        glm::vec3 sgn = -glm::sign(r.direction());
        glm::vec3 radius = glm::vec3(size, size, size);
        glm::vec3 d = radius * winding * sgn - r.origin();
        d *= r.idirection();

        glm::bvec3 test = glm::bvec3(
            glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().y, r.origin().z) + glm::vec2(r.direction().y, r.direction().z) * d.x), glm::vec2(radius.y, radius.z))),
            glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().z, r.origin().x) + glm::vec2(r.direction().z, r.direction().x) * d.y), glm::vec2(radius.z, radius.x))),
            glm::all(glm::lessThan(glm::abs(glm::vec2(r.origin().x, r.origin().y) + glm::vec2(r.direction().x, r.direction().y) * d.z), glm::vec2(radius.x, radius.y)))
        );
        sgn = test.x ? glm::vec3(sgn.x, 0, 0) : (test.y ? glm::vec3(0, sgn.y, 0) : glm::vec3(0, 0, test.z ? sgn.z : 0));

        return (sgn.x != 0.f) || (sgn.y != 0.f) || (sgn.z != 0.f);
    }

    bool Contains(glm::vec3 p)
    {
        glm::vec3 vmin = this->min();
        glm::vec3 vmax = this->max();

        return (   (p.x >= vmin.x && p.x <= vmax.x)
            && (p.y >= vmin.y && p.y <= vmax.y)
            && (p.z >= vmin.z && p.z <= vmax.z)
        );
    }
};