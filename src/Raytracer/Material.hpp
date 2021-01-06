#pragma once

#include <cassert>
#include "Random.hpp"
#include "Ray.hpp"
#include "Hit.hpp"

class material {
	public:
		virtual bool scatter(const ray& r_in, const DODHitRecord& rec, glm::vec3& attenuation, ray& scattered) const = 0;
};

float distance2(glm::vec3 const& v1)
{
    return (v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
}

glm::vec3 random_in_unit_sphere()
{
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(randf(), randf(), randf()) - glm::vec3(1.f, 1.f, 1.f);
    } while (distance2(p) >= 1.0);
    return (p);
}

glm::vec3 reflect(const glm::vec3& v, const glm::vec3& n)
{
    return v - 2 * glm::dot(v, n) * n;
}

float schlick(float cosine, float ref_idx)
{
    float r0 = (1.f - ref_idx) / (1.f + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.f - r0) * pow((1.f - cosine), 5);
}

bool refract(const glm::vec3& v, const glm::vec3& n, float ni_over_nt, glm::vec3& refracted) {
    glm::vec3 uv = glm::normalize(v);
    float dt = glm::dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * (1.f - dt * dt);
    if (discriminant > 0.f)  {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);
        return true;
    } else
        return false;
}

static bool DODMaterial_Metal_scatter(const ray& r_in, const DODHitRecord& rec, glm::vec3& attenuation, ray& scattered)
{
    glm::vec3 reflected = reflect(glm::normalize(r_in.direction()), rec.normal);
    scattered = ray(rec.p, reflected + rec.mat.fuzz * random_in_unit_sphere());
    attenuation = rec.mat.albedo;
    return (glm::dot(scattered.direction(), rec.normal) > 0);
}

static bool DODMaterial_Dieletric_scatter(const ray& r_in, const DODHitRecord& rec, glm::vec3& attenuation, ray& scattered)
{
    float ref_idx = rec.mat.refraction_index;
    glm::vec3 outward_normal;
    glm::vec3 reflected = reflect(r_in.direction(), rec.normal);
    float ni_over_nt;
    attenuation = glm::vec3(1.0, 1.0, 1.0);
    glm::vec3 refracted;
    float reflect_prob, cosine;
    if (dot(r_in.direction(), rec.normal) > 0) {
        outward_normal = -rec.normal;
        ni_over_nt = ref_idx;
        cosine = glm::dot(r_in.direction(), rec.normal) / r_in.direction().length();
        cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
    } else {
        outward_normal = rec.normal;
        ni_over_nt = 1.0 / ref_idx;
        cosine = -glm::dot(r_in.direction(), rec.normal) / r_in.direction().length();
    }
    if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
        reflect_prob = schlick(cosine, ref_idx);
    else
        reflect_prob = 1.0;
    if (randf() < reflect_prob)
        scattered = ray(rec.p, reflected);
    else
        scattered = ray(rec.p, refracted);
    return (true);
}

static bool DODMaterial_Lambertian_scatter(const ray& r_in, const DODHitRecord& rec, glm::vec3& attenuation, ray& scattered)
{
    glm::vec3 target = rec.p + rec.normal + random_in_unit_sphere();
    scattered = ray(rec.p, target - rec.p);
    attenuation = rec.normal;
    return (true);
}

bool DODMaterial_scatter(const ray& r_in, const DODHitRecord& rec, glm::vec3& attenuation, ray& scattered)
{
    if (rec.mat.type == DM_LAMBERTIAN)
        return DODMaterial_Lambertian_scatter(r_in, rec, attenuation, scattered);
    else if (rec.mat.type == DM_METAL) {
        return DODMaterial_Metal_scatter(r_in, rec, attenuation, scattered);
    } else if (rec.mat.type == DM_DIELETRIC) {
        return DODMaterial_Dieletric_scatter(r_in, rec, attenuation, scattered);
    } else {
        printf("Material %d is unknown\n", rec.mat.type);
        assert(0 && "Unknown material");
    }
    return false;
}