#pragma once

#include <cstddef>
#include <iosfwd>

#include "render_port.hpp"

namespace sim::adapter
{
class ConsoleRenderAdapter : public application::RenderPort
{
public:
    explicit ConsoleRenderAdapter(std::ostream &stream);

    void render(const domain::World &world) override;

private:
    std::ostream *stream_;
    std::size_t frame_{0};
};
} // namespace sim::adapter
