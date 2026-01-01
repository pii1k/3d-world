#pragma once

#include <glm/glm.hpp>

namespace colors
{
inline const glm::vec3 kGrassGreen{
    123.0f / 255.0f, // R: 0x7B
    161.0f / 255.0f, // G: 0xA1
    143.0f / 255.0f  // B: 0x8F
}; // #7BA18F

inline const glm::vec3 kGrassGreenAlt{
    144.0f / 255.0f, // R: 0x90
    161.0f / 255.0f, // G: 0xA1
    92.0f / 255.0f   // B: 0x5C
}; // #90A15C

inline const glm::vec3 kWaterBlue{0.10f, 0.34f, 0.82f};
inline const glm::vec3 kSoilOchre{0.74f, 0.56f, 0.26f};
} // namespace colors