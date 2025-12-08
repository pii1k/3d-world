#pragma once

namespace ads::domain
{
class World;
}

namespace ads::application
{
class RenderPort
{
public:
    virtual ~RenderPort() = default;
    virtual void render(const ads::domain::World &world) = 0;
    virtual bool isActive() const = 0;
};
} // namespace ads::application
