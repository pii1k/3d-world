#include "bat_scene_builder.hpp"
#include "colors.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace domain
{
namespace
{
constexpr float kGroundThickness = 1.0f;
constexpr float kBatScale = 0.6f;
constexpr float kWorldSize = 60.0f;
} // namespace

void BatSceneBuilder::buildScene(const BatLogic &logic, SceneData &out_scene) const
{
    // Ground
    {
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -kGroundThickness * 0.5f, 0.0f)) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(kWorldSize, kGroundThickness, kWorldSize));

        out_scene.objects.push_back({domain::MeshType::Cube,
                                     model,
                                     graphics::colors::kStoneGray});
    }

    // Bat (placeholder)
    {
        const BatState &bat = logic.state();
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), bat.position) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(kBatScale));

        out_scene.objects.push_back({domain::MeshType::Cube,
                                     model,
                                     graphics::colors::kSkyBlue});
    }
}
} // namespace domain
