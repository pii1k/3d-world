#pragma once

#include <cmath>

namespace sim::domain
{
struct Vector2
{
    float x{0.0F};
    float y{0.0F};
};

inline Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y};
}

inline Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y};
}

inline Vector2 operator*(const Vector2 &lhs, float scalar)
{
    return {lhs.x * scalar, lhs.y * scalar};
}

inline Vector2 operator*(float scalar, const Vector2 &rhs)
{
    return rhs * scalar;
}

inline float dot(const Vector2 &lhs, const Vector2 &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y;
}

inline float length(const Vector2 &vec)
{
    return std::sqrt(dot(vec, vec));
}

inline float distance(const Vector2 &lhs, const Vector2 &rhs)
{
    return length(lhs - rhs);
}

inline Vector2 normalize(const Vector2 &vec)
{
    const float len = length(vec);
    if (len == 0.0F)
    {
        return {};
    }
    return {vec.x / len, vec.y / len};
}
} // namespace sim::domain
