#pragma once

namespace sim::domain
{
class World;
}

namespace sim::application
{
class RenderPort
{
public:
    virtual ~RenderPort() = default;
    virtual void render(const sim::domain::World &world) = 0;
};
} // namespace sim::application
