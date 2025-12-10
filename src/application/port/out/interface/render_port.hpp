#pragma once

namespace ads::port::out
{
class RenderPort
{
public:
    virtual void render() = 0;
};
} // namespace ads::port::out