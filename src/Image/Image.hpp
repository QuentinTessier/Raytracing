#pragma once

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <iostream>
#include <string>
#include <stdint.h>
#include "glm/glm.hpp"

class Image {
    public:
        Image(uint32_t width, uint32_t height)
            : m_Width(width), m_Height(height), m_Pixels(nullptr)
        {
            m_Pixels = new uint8_t[m_Width * m_Height * nChannels];
        }

        ~Image() { delete m_Pixels; }

        // Copy constructor
        Image(Image const& o) = delete;
        Image& operator=(Image const& o) = delete;

        inline uint8_t *getRawPixel(size_t x, size_t y) { return &(m_Pixels[(y * m_Width + x) * nChannels]); }
        inline void setPixel(size_t x, size_t y, glm::vec3 color)
        {
            uint8_t *pixel = getRawPixel(x, y);

            pixel[0] = int(255.99f * color.x);
            pixel[1] = int(255.99f * color.y);
            pixel[2] = int(255.99f * color.z);
        }

        static void setRawPixel(uint8_t *pixel, glm::vec3 color)
        {
            pixel[0] = int(255.99f * color.x);
            pixel[1] = int(255.99f * color.y);
            pixel[2] = int(255.99f * color.z);
        }

        static const uint8_t nChannels = 3;

        void SaveAs(std::string const& filename)
        {
            stbi_flip_vertically_on_write(true);
            stbi_write_png(filename.c_str(), m_Width, m_Height, nChannels, m_Pixels, m_Width * nChannels);
        }

    private:
        uint32_t m_Width;
        uint32_t m_Height;
        uint8_t *m_Pixels;
};