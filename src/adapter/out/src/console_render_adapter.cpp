#include "console_render_adapter.hpp"

#include <iomanip>
#include <iostream>

#include "world.hpp"

namespace sim::adapter
{
ConsoleRenderAdapter::ConsoleRenderAdapter(std::ostream &stream) : stream_(&stream) {}

void ConsoleRenderAdapter::render(const domain::World &world)
{
    *stream_ << "[frame " << frame_++ << "] ";
    *stream_ << "ego pos=(" << std::fixed << std::setprecision(2) << world.egoVehicle().position().x << ", "
             << world.egoVehicle().position().y << ") ";
    *stream_ << "speed=" << world.egoVehicle().speedMps() << " m/s";

    const auto pedestrians = world.pedestrians();
    if (!pedestrians.empty())
    {
        *stream_ << " | pedestrians: ";
        for (std::size_t idx = 0; idx < pedestrians.size(); ++idx)
        {
            const auto &ped = pedestrians[idx];
            *stream_ << "#" << idx << " (" << ped.position().x << ", " << ped.position().y << ")";
            if (idx + 1 < pedestrians.size())
            {
                *stream_ << ", ";
            }
        }
    }

    *stream_ << '\n';
    stream_->flush();
}
} // namespace sim::adapter
