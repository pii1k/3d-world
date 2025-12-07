#pragma once

#include <vector>

#include "v2x_message.hpp"

namespace sim::domain
{
class World;
}

namespace sim::application
{
class V2XCommPort
{
public:
    virtual ~V2XCommPort() = default;
    virtual std::vector<V2XMessage> receiveAll(const sim::domain::World &world) = 0;
};
} // namespace sim::application
