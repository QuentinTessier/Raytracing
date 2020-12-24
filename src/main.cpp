/*
** EPITECH PROJECT, 2019
** RogueLike
** File description:
** main
*/

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <limits>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <getopt.h>
#include "Raytracer/Shape/Sphere.hpp"
#include "Raytracer/Shape/Box.hpp"
#include "Raytracer/HitList.hpp"
#include "Raytracer/Camera.hpp"
#include "Raytracer/Material.hpp"
#include "Raytracer/Random.hpp"

static const char *soptions = "o:w:h:s:";

static const struct option loptions[] = {
    {"object", required_argument, 0, 'o'},
    {"width", required_argument, 0, 'w'},
    {"height", required_argument, 0, 'h'},
    {"samples", required_argument, 0, 's'},
    {0, 0, 0, 0}
};

void print_usage(char const *name)
{
    printf(
        "\nUsage %s --object name --width nx --height ny --samples ns\n"
        "\t--object\t|\t-o\tName of the image generated by the program\n"
        "\t--width\t|\t-w\tWidth of the image generated by the program\n"
        "\t--height\t|\t-h\tHeight of the image generated by the program\n"
        "\t--samples\t|\t-s\tNumber of samples use by the program to generate the image\n",
        name
    );
}

glm::vec3 color(const ray& r, hitable *world, int depth)
{
    hit_record rec;
    if (world->hit(r, 0.001, std::numeric_limits<float>::max(), rec)) {
        ray scattered;
        glm::vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return (attenuation * color(scattered, world, depth + 1));
        } else {
            return (glm::vec3(0, 0, 0));
        }
    } else {
        glm::vec3 unit_dir = glm::normalize(r.direction());
        float t = 0.5 * (unit_dir.y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
    }
}

bool process_params(int ac, char **av, std::string& name, int& width, int& height, int& samples)
{
    int long_index = 0;
    int opt = 0;

    while ((opt = getopt_long(ac, av, soptions, loptions, &long_index)) != -1) {
        switch (opt) {
            case 'o':
                name = optarg;
                break;
            case 'w':
                width = atoi(optarg);
                break;
            case 'h':
                height = atoi(optarg);
                break;
            case 's':
                samples = atoi(optarg);
                break;
            default:
                print_usage(av[0]);
                return false;
        }
    }
    return true;
}

void vec3_to_Color(glm::vec3 v, int ns, uint8_t *r, uint8_t *g, uint8_t *b)
{
    v /= float(ns);
    v = glm::sqrt(v);

    *r = int(255.99f * v.x);
    *g = int(255.99f * v.y);
    *b = int(255.99f * v.z);
}

#define CHANNELS 3

int main(int ac, char **av)
{
    int nx = 200;
    int ny = 112;
    int ns = 100;
    std::string name("../raytraced.png");
    if (!process_params(ac, av, name, nx, ny, ns))
        return 1;
    uint8_t *pixels = new uint8_t[nx * ny * CHANNELS];
    hitable *list[3];
    list[0] = new sphere(glm::vec3(0,-100.5,-1), 100, new lambertian(glm::vec3(0.8, 0.8, 0.0)));
    list[1] = new Box(glm::vec3(1, 0, 0), 0.5, new metal(glm::vec3(0.8, 0.6, 0.2), 0.0));
    list[2] = new Box(glm::vec3(-1, 0, 0), 0.5, new lambertian(glm::vec3(0.1, 0.2, 0.5)));
    hitable *world = new hitable_list(list, 3);
    camera cam(glm::vec3(3, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 90, float(nx) / float(ny));

    size_t offset = 0;
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            glm::vec3 col(0.f, 0.f, 0.f);
            for (int s=0; s < ns; s++) {
                float u = float(i + randf()) / float(nx);
                float v = float(j + randf()) / float(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            vec3_to_Color(col, ns, pixels + offset, pixels + offset + 1, pixels + offset + 2);
            offset += 3;
        }
    }
    stbi_write_png(name.c_str(), nx, ny, CHANNELS, pixels, nx * CHANNELS);
    delete pixels;
    return (0);
}