#include "common.hpp"

Vector2d Vector2d::operator+(Vector2d other)
{
    return Vector2d(x + other.x, y + other.y);
}

Vector2d Vector2d::operator-(Vector2d other)
{
    return Vector2d(x - other.x, y - other.y);
}

Vector2d Vector2d::operator*(double other)
{
    return Vector2d(x * other, y * other);
}

Vector2d Vector2d::operator/ (double other)
{
    return Vector2d(x / other, y / other);
}

double degreesToRadians(double degrees) {
    return degrees * (M_PI / 180);
}

double radiansToDegrees(double radians) {
    return radians * (180 / M_PI);
}
