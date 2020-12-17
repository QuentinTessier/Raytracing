#pragma once

#include <random>

static inline float randf()
{
    static std::uniform_real_distribution<float> dist(0.0, 1.0);
    static std::mt19937 gen;
    return dist(gen);
}