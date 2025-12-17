#pragma once

#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/glm.hpp>
#include <vector>

#include "registry.hpp"

namespace
{
constexpr size_t kMaxLights = 16;
}

struct GpuLight
{
    glm::vec4 position;  // w에 type 인코딩
    glm::vec4 direction; // w에 range
    glm::vec4 color;     // rgb + intensity
    glm::vec4 params;    // inner,outer cone 등
};

class LightingSystem
{
public:
    void update(const Registry &registry)
    {
        gpu_lights_.clear();
        for (auto &[entity, light] : registry.lights_)
        {
            if (!light.enabled)
                continue;
            GpuLight gpu_light{};
            gpu_light.position = glm::vec4(light.position, static_cast<float>(light.type));
            gpu_light.direction = glm::vec4(glm::normalize(light.direction), light.range);
            gpu_light.color = glm::vec4(light.color, light.intensity);
            gpu_light.params = glm::vec4(light.inner_cone, light.outer_cone, 0.0f, 0.0f);
            gpu_lights_.push_back(gpu_light);

            if (gpu_lights_.size() >= kMaxLights)
                break;
        }
    }

    const std::vector<GpuLight> &getGpuLights() { return gpu_lights_; }

private:
    std::vector<GpuLight> gpu_lights_;
};