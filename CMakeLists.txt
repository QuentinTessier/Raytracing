cmake_minimum_required(VERSION 3.10)
project(Raytracer LANGUAGES CXX)

add_executable(Raytracer
    src/main.cpp
)

target_include_directories(Raytracer PRIVATE ./include)

target_include_directories(Raytracer PRIVATE extern/glm)
target_include_directories(Raytracer PRIVATE extern/stb)