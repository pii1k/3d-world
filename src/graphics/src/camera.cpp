#include "camera.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(glm::vec3 position)
    : position_(position),
      world_up_(glm::vec3(0.0f, 1.0f, 0.0f)),
      front_(glm::vec3(0.0f, 0.0f, -1.0f)),
      yaw_(-90.0f), // yaw는 0일때 +x 방향을 보므로, -z 방향을 보기위해 -90으로 시작
      pitch_(0.0f),
      movement_speed_(2.5f),
      mouse_sensitivity_(0.1f),
      fov_(45.0f),
      aspect_ratio_(1280.0f / 720.0f) // 이 값은 나중에 창 크기 바꾸기 할때 수정
{
    this->updateCameraVectors();
}

// 카메라의 위치(position)와 방향(front), 위(up) 벡터를 사용하여 view 행렬 계산 -> glm::lookAt 함수가 이 역할을 해줌
glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position_, position_ + front_, up_);
}

// 원근 투영(perspective projection) 행렬 계산 -> 시야각, 종횡비, 0.1f와 100.0f는 near/far 평면
glm::mat4 Camera::getProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov_), aspect_ratio_, 0.1f, 100.0f);
}

// delta_time을 곱해줘 프레임 속도에 관계없이 일정한 속도로 움직이게함
void Camera::processKeyborad(char direction, float delta_time)
{
    float velocity = movement_speed_ * delta_time;

    if (direction == 'W')
        position_ += front_ * velocity;

    if (direction == 'S')
        position_ -= front_ * velocity;

    if (direction == 'A')
        position_ -= right_ * velocity;

    if (direction == 'D')
        position_ += right_ * velocity;
}

// 마우스의 움직임(offset x, y)에 따라 카메라의 방향 (yaw, pitch) 변경
void Camera::processMouseMovement(float offset_x, float offset_y)
{
    offset_x *= mouse_sensitivity_;
    offset_y *= mouse_sensitivity_;

    yaw_ += offset_x;
    pitch_ += offset_y;

    // pitch(상하 각도)가 90도 이상으로 넘어가지 않도록 제한
    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -89.0f)
        pitch_ = -89.0f;

    // 변경된 yaw, pitch 값을 바탕으로 실제 방향 벡터 다시 계산
    updateCameraVectors();
}

// 마우스 휠 스크롤에 따라 시야각을 조절하여 줌 인/아웃 효과
void Camera::processMouseScroll(float offset_y)
{
    fov_ -= offset_y;
    if (fov_ < 1.0f)
        fov_ = 1.0f;
    if (fov_ > 45.0f)
        fov_ = 45.0f;
}

// yaw와 pitch 각도로부터 실제 front, right, up 벡터를 계산하는 핵심 함수
void Camera::updateCameraVectors()
{
    glm::vec3 new_front;
    new_front.x = cos(glm::radians(yaw_) * cos(glm::radians(pitch_)));
    new_front.y = sin(glm::radians(pitch_));
    new_front.z = cos(glm::radians(yaw_) * cos(glm::radians(pitch_)));
    front_ = glm::normalize(new_front);

    // right 벡터와 up 벡터도 다시 계산
    // right 벡터는 월드 좌표계의 up 벡터와 front 벡터를 외적하여 구함 (corss product)
    right_ = glm::normalize(glm::cross(front_, world_up_));
    // up 벡터는 right 벡터와 front 벡터를 외적하여 구함
    up_ = glm::normalize(glm::cross(right_, front_));
}