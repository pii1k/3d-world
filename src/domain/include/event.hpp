#pragma once

#include <variant>

namespace domain
{
enum class EventType
{
    OnScroll,
    OnMouseButton,
    OnKey
};

struct ScrollEvent
{
    double offset_x = 0;
    double offset_y = 0;
};

struct MouseButtonEvent
{
    int button = 0;
    int action = 0;
    int mods = 0;
    double x = 0;
    double y = 0;
};

struct KeyEvent
{
    int key = 0;
    int scancode = 0;
    int action = 0;
    int mods = 0;
};

struct ResizeEvent
{
    int width = 0;
    int height = 0;
};

using Event = std::variant<ScrollEvent,
                           MouseButtonEvent,
                           KeyEvent,
                           ResizeEvent>;

} // namespace domain