#pragma once

#include "v2x_comm_port.hpp"

namespace sim::adapter
{
class InMemoryV2XAdapter : public application::V2XCommPort
{
public:
    explicit InMemoryV2XAdapter(float caution_distance_m);

    std::vector<application::V2XMessage> receiveAll(const domain::World &world) override;

private:
    float caution_distance_m_{12.0F};
};
} // namespace sim::adapter
