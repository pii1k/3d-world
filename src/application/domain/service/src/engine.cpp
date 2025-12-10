#include "engine.hpp"
#include "render_port.hpp"

namespace ads::domain::service
{
Engine::Engine() {};

bool Engine::setRenderPort(port::out::RenderPort *render_port)
{
    if (!render_port)
        return false;

    render_port_ = render_port;
    return true;
}
} // namespace ads::domain::service