#pragma once

class CameraController
{
public:
    virtual ~CameraController() = default;

    virtual void processMouseMovement(float xoffset, float yoffset) = 0;
    virtual void processMouseScroll(float yoffset) = 0;
    virtual void update(float delta_time) = 0;
};