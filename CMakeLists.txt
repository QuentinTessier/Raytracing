cmake_minimum_required(VERSION 3.10)
project(Raytracer VERSION 1.0 LANGUAGES C)

add_executable(Raytracer
    src/main.c
    src/hit_list.c
)

target_link_libraries(Raytracer m)