#include "common.hpp"

Point2d Point2d::operator+(Point2d other)
{
    return Point2d(x + other.x, y + other.y);
}

Point2d Point2d::operator+(Vector2d vector)
{
    return Point2d(x + vector.x, y + vector.y);
}

Point2d Point2d::operator-(Vector2d vector)
{
    return Point2d(x - vector.x, y - vector.y);
}

double degreesToRadians(int degrees) {
    return degrees * (M_PI / 180);
}