/*
** EPITECH PROJECT, 2019
** RogueLike
** File description:
** main
*/

#include <limits>
#include <iostream>
#include <fstream>
#include "Sphere.hpp"
#include "Box.hpp"
#include "HitList.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Random.hpp"

vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return (attenuation * color(scattered, world, depth + 1));
        } else {
            return (vec3(0, 0, 0));
        }
    } else {
        vec3 unit_dir = unit_vector(r.direction());
        float t = 0.5 * (unit_dir.y() + 1.0f);
        return (1.0f - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main()
{
    std::ofstream file("../test1.ppm", std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open())
        return 1;
    int nx = 200;
    int ny = 112;
    int ns = 100;
    std::cout << "Writing a image( " << nx << ", " << ny << " ) with " << ns << " samples per pixels\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    hitable *list[3];
    list[0] = new sphere(vec3(0,-100.5,-1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[1] = new Box(vec3(1,0,-1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.0));
    //list[2] = new sphere(vec3(-1,0,-1), 0.5, new dielectric(1.5));
    //list[3] = new Box(vec3(-1,0,-1), -0.45, new dielectric(1.5));
    list[2] = new Box(vec3(0,0,-1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
    hitable *world = new hitable_list(list, 3);
    camera cam(vec3(-2,2,1), vec3(0, 0, -1), vec3(0, 1, 0), 90, float(nx) / float(ny));

    file << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0.f, 0.f, 0.f);
            for (int s=0; s < ns; s++) {
                float u = float(i + randf()) / float(nx);
                float v = float(j + randf()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            file << ir << " " << ig << " " << ib << "\n";
        }
    }
    file.close();
    return (0);
}