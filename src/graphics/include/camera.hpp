#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    // 생성자
    Camera(glm::vec3 position);
    // Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    void processKeyborad(char direction, float delta_time);
    void processMouseMovement(float offset_x, float offset_y);
    void processMouseScroll(float offset_y);

    void setAspectRatio(float aspect_ratio) { aspect_ratio_ = aspect_ratio; };

private:
    // 카메라 속성
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 world_up_;

    // 오일러 각(카메라 방향)
    float yaw_;
    float pitch_;

    // 카메라 옵션
    float movement_speed_;
    float mouse_sensitivity_;
    float fov_;
    float aspect_ratio_ = 1.0f;

    // 카메라 벡터들을 다시 계산하는 내부 함수
    void updateCameraVectors();
};