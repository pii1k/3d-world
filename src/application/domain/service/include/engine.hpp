#pragma once

#include "render_port.hpp"

namespace ads::domain::service
{
class Engine
{
public:
    Engine();

    bool setRenderPort(port::out::RenderPort *render_port);

private:
    void *render_port_ = nullptr;
};
} // namespace ads::domain::service