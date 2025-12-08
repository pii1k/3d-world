#include "world.hpp"

#include <algorithm>

namespace ads::domain
{
World::World(Vehicle ego_vehicle, std::vector<Pedestrian> pedestrians, float road_half_width)
    : ego_vehicle_(std::move(ego_vehicle)),
      pedestrians_(std::move(pedestrians)),
      road_half_width_(std::max(road_half_width, 1.0F))
{
}

void World::advance(float delta_seconds)
{
    ego_vehicle_.advance(delta_seconds);
    for (auto &ped : pedestrians_)
    {
        ped.advance(delta_seconds);
    }
    clampPedestrians();
}

void World::setEgoTargetSpeed(float target_speed_mps)
{
    ego_vehicle_.setTargetSpeed(target_speed_mps);
}

void World::clampPedestrians()
{
    for (auto &ped : pedestrians_)
    {
        ped = Pedestrian(
            {std::clamp(ped.position().x, -road_half_width_, road_half_width_), ped.position().y},
            ped.velocity());
    }
}
} // namespace ads::domain
