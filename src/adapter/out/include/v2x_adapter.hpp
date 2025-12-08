#pragma once

#include "v2x_comm_port.hpp"

namespace ads::adapter
{
class V2XAdapter : public application::V2XCommPort
{
public:
    explicit V2XAdapter(float caution_distance_m);

    std::vector<application::V2XMessage> receiveAll(const domain::World &world) override;

private:
    float caution_distance_m_{12.0F};
};
} // namespace ads::adapter
