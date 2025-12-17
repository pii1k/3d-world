#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct CameraConfig
{
    float fov = 45.0f;
    float aspect_ratio = 16.0f / 9.0f;
    float near_plane = 0.1f;
    float far_plane = 1000.0f;
};

class Camera
{
public:
    explicit Camera(const glm::vec3 &position,
                    const glm::quat &orientation,
                    const CameraConfig &config);

    explicit Camera(const glm::vec3 &position,
                    const CameraConfig &config = {});

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec3 &getPosition() const { return position_; }
    void setPosition(const glm::vec3 &position);

    const glm::quat &getOrientation() const { return orientation_; }
    void setOrientation(const glm::quat &orientation);

    float getAspectRatio() const { return aspect_ratio_; }
    void setAspectRatio(float aspect_ratio);

    void rotate(float yaw, float pitch);
    void zoom(float fov_degrees);

private:
    glm::vec3 position_;
    glm::quat orientation_;

    float fov_;
    float aspect_ratio_;
    float near_plane_;
    float far_plane_;
};