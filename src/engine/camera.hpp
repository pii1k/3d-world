#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct CameraConfig
{
    glm::vec3 position{};
    glm::vec3 target{};
    glm::vec3 up{};
    float fov_deg = 45.0f;
    float near_plane = 0.1f;
    float far_plane = 200.0f;
    bool use_ortho = false;
    float ortho_size = 10.0f;
};

class Camera
{
public:
    explicit Camera(const CameraConfig &cfg);

    glm::mat4 view() const;
    glm::mat4 proj(int w, int h) const;

    void setPosition(const glm::vec3 &pos);
    void setTarget(const glm::vec3 &target);
    void setUp(const glm::vec3 &up);
    void setPerspective(float fov_deg, float near_plane, float far_plane);
    void setOrtho(float size, float near_plane, float far_plane);

private:
    glm::vec3 position_{};
    glm::vec3 target_{};
    glm::vec3 up_{};
    float fov_deg_ = 0.f;
    float near_plane_, far_plane_ = 0.f;
    bool use_ortho_ = false;
    float ortho_size_ = 0.f;
};