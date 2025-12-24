#pragma once

class CameraController
{
public:
    virtual ~CameraController() = default;

    virtual void processMouseMovement(float offset_x, float offset_y) = 0;
    virtual void processMouseScroll(float offset_y) = 0;
    virtual void update(float delta_time) = 0;
};