#pragma once

#include <vector>

#include "v2x_message.hpp"

namespace ads::domain
{
class World;
}

namespace ads::application
{
class V2XCommPort
{
public:
    virtual ~V2XCommPort() = default;
    virtual std::vector<V2XMessage> receiveAll(const ads::domain::World &world) = 0;
};
} // namespace ads::application
