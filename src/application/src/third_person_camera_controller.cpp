#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "third_person_camera_controller.hpp"

ThirdPersonCameraController::ThirdPersonCameraController(
    Camera &camera,
    World &world,
    entity_id target_entity,
    const ThirdPersonControllerConfig &config)
    : camera_(camera),
      world_(world) {}

void ThirdPersonCameraController::processMouseMovement(float dx, float dy)
{
    controller_state_.yaw_deg += dx * controller_config_.mouse_sensitivity;
    controller_state_.pitch_deg += dy * controller_config_.mouse_sensitivity;
    controller_state_.pitch_deg = std::clamp(controller_state_.pitch_deg, controller_config_.min_pitch_deg, controller_config_.max_pitch_deg);
}

void ThirdPersonCameraController::processMouseScroll(float yoffset)
{
    controller_state_.distance -= yoffset * controller_config_.zoom_speed;
    controller_state_.distance = std::clamp(controller_state_.distance, controller_config_.min_distance, controller_config_.max_distance);
}

void ThirdPersonCameraController::update(float /*dt*/)
{
    const auto tr = world_.getComponent<TransformComponent>(controller_state_.target);
    if (!tr)
        return;

    const glm::vec3 pivot = tr->get().position + controller_config_.pivot_offset;

    const float yaw = glm::radians(controller_state_.yaw_deg);
    const float pitch = glm::radians(controller_state_.pitch_deg);

    const glm::quat q = glm::normalize(
        glm::angleAxis(yaw, glm::vec3(0, 1, 0)) *
        glm::angleAxis(pitch, glm::vec3(1, 0, 0)));

    const glm::vec3 forward = q * glm::vec3(0, 0, -1);
    camera_.setOrientation(q);
    camera_.setPosition(pivot - forward * controller_state_.distance);
}