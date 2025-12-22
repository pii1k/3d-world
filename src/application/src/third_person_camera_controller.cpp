#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "third_person_camera_controller.hpp"

ThirdPersonCameraController::ThirdPersonCameraController(Camera &camera,
                                                         Registry &registry,
                                                         Entity target_entity,
                                                         const ThirdPersonControllerConfig &config)
    : camera_(camera),
      registry_(registry),
      target_entity_(target_entity),
      distance_(config.distance_to_target),
      pitch_(config.initial_pitch),
      yaw_(config.initial_yaw),
      sensitivity_(config.mouse_sensitivity),
      zoom_speed_(config.zoom_speed)
{
}

void ThirdPersonCameraController::processMouseMovement(float xoffset, float yoffset)
{
    yaw_ -= xoffset * sensitivity_;
    pitch_ += yoffset * sensitivity_;

    if (pitch_ > 89.0f)
        pitch_ = 89.0f;
    if (pitch_ < -5.0f)
        pitch_ = -5.0f;
}

void ThirdPersonCameraController::processMouseScroll(float yoffset)
{
    distance_ -= yoffset * zoom_speed_;
    if (distance_ < 2.0f)
        distance_ = 2.0f;
    if (distance_ > 20.0f)
        distance_ = 20.0f;
}

void ThirdPersonCameraController::update(float deltaTime)
{
    (void)deltaTime;
    const auto target_transform = registry_.getTransform(target_entity_);
    if (!target_transform)
        return;

    const glm::vec3 &target_position = target_transform->get().position;

    // 타겟을 중심으로 한 카메라의 상대 위치 계산
    float horizontal_distance = distance_ * cos(glm::radians(pitch_));
    float vertical_distance = distance_ * sin(glm::radians(pitch_));

    float theta = glm::radians(yaw_);
    float offset_x = horizontal_distance * sin(theta);
    float offset_z = horizontal_distance * cos(theta);

    glm::vec3 camera_position;
    camera_position.x = target_position.x - offset_x;
    camera_position.z = target_position.z - offset_z;
    camera_position.y = target_position.y + vertical_distance;

    camera_.setPosition(camera_position);

    // 카메라가 항상 타겟을 바라보도록 방향(쿼터니언) 설정
    // lookAt 행렬의 역행렬에서 회전 부분만 추출하여 쿼터니언을 만듭니다.
    glm::mat4 look_at_matrix = glm::lookAt(camera_position, target_position, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat camera_orientation = glm::conjugate(glm::quat_cast(look_at_matrix));

    camera_.setOrientation(camera_orientation);
}
