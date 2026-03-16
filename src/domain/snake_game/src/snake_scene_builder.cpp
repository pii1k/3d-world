#include "snake_scene_builder.hpp"
#include "colors.hpp"
#include "snake_logic.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace domain
{
namespace
{
constexpr float kGroundThickness = 1.0f;
constexpr float kSnakeY = 0.5f;
constexpr float kFoodScale = 0.7f;
constexpr float kSnakeScale = 0.9f;
} // namespace

void SnakeSceneBuilder::buildScene(const SnakeLogic &logic, SceneData &out_scene) const
{
    const SnakeGrid &grid = logic.grid();

    // Ground
    {
        const float ground_width = static_cast<float>(grid.width) * grid.cell_size;
        const float ground_length = static_cast<float>(grid.height) * grid.cell_size;

        const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -kGroundThickness * 0.5f, 0.0f)) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(ground_width, kGroundThickness, ground_length));

        out_scene.objects.push_back({domain::MeshType::Cube,
                                     model,
                                     graphics::colors::kDirtBrown});
    }

    // Food
    {
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), gridToWorld(logic, logic.food())) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(kFoodScale));
        out_scene.objects.push_back({domain::MeshType::Cube,
                                     model,
                                     graphics::colors::kAppleRed});
    }

    // Snake
    const auto &snake = logic.snake();
    for (size_t i = 0; i < snake.size(); ++i)
    {
        const glm::vec3 color = (i == 0) ? graphics::colors::kGreen : graphics::colors::kSoilOchre;
        const glm::mat4 model = glm::translate(glm::mat4(1.0f), gridToWorld(logic, snake[i])) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(kSnakeScale));
        out_scene.objects.push_back({domain::MeshType::Cube,
                                     model,
                                     color});
    }
}

glm::vec3 SnakeSceneBuilder::gridToWorld(const SnakeLogic &logic, const glm::ivec2 &cell) const
{
    const SnakeGrid &grid = logic.grid();
    const float origin_x = -0.5f * (static_cast<float>(grid.width - 1)) * grid.cell_size;
    const float origin_z = -0.5f * (static_cast<float>(grid.height - 1)) * grid.cell_size;
    return glm::vec3(origin_x + static_cast<float>(cell.x) * grid.cell_size,
                     kSnakeY,
                     origin_z + static_cast<float>(cell.y) * grid.cell_size);
}
} // namespace domain