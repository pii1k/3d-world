#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct TransformComponent
{
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 getTransform() const
    {
        glm::mat4 rotation_matrix = glm::mat4_cast(rotation);
        return glm::translate(glm::mat4(1.0f), position) * rotation_matrix * glm::scale(glm::mat4(1.0f), scale);
    }
};

struct RenderableComponent
{
    int mesh_id = 0;
    glm::vec3 color{1.0f, 0.5f, 0.2f};
    bool use_grid = false;
};

enum class LightType
{
    Directional,
    Point,
    Spot,
};

struct LightComponent
{
    LightType type{LightType::Directional};
    glm::vec3 color{1.0f};
    float intensity{1.0f};
    glm::vec3 position{0.0f};
    glm::vec3 direction{0, -1, 0};
    float range{15.0f};
    float inner_cone{0.85f};
    float outer_cone{0.9f};
    bool enabled{true};
};

struct PhysicsComponent
{
    glm::vec3 velocity = glm::vec3(0.0f);
    glm::vec3 acceleration = glm::vec3(0.0f);
    float friction = 0.95f;
    bool grounded = false;
};

struct PlayerControllerComponent
{
};