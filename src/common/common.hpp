#ifndef __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_

#include <math.h>
#include <algorithm>

struct Rotation {
    double radians = 0;

    static double degreesToRadians(double degrees) {
        return degrees * (M_PI / 180);
    }

    static double radiansToDegrees(double radians) {
        return radians * (180 / M_PI);
    }


    Rotation(): radians(0) {}

    Rotation(double radians): radians(radians) {}

    Rotation operator+ (Rotation other) {
        Rotation result;
        result.radians = std::fmod(radians + other.radians, M_PI * 2);
        return result;
    }

    Rotation operator- (Rotation other) {
        Rotation result;
        result.radians = std::fmod(radians - other.radians, M_PI * 2);
        return result;
    }

    double degress() {
        return radiansToDegrees(radians);
    }

    static Rotation fromDegrees(double degrees) {
        return Rotation(degreesToRadians(degrees));
    }
};

struct Vector2d {
    double x;
    double y;

    Vector2d(double x, double y): x(x), y(y) {}
    Vector2d(): x(0), y(0) {}
    Vector2d(Rotation rotation, double length) {
        x = cos(rotation.radians) * length;
        y = sin(rotation.radians) * length;
    }

    Vector2d operator+(Vector2d other)
    {
        return Vector2d(x + other.x, y + other.y);
    }

    Vector2d operator-(Vector2d other)
    {
        return Vector2d(x - other.x, y - other.y);
    }

    Vector2d operator*(double other)
    {
        return Vector2d(x * other, y * other);
    }

    bool operator==(Vector2d other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(Vector2d other) {
        return x != other.x || y != other.y;
    }

    Vector2d operator/ (double other)
    {
        return Vector2d(x / other, y / other);
    }

    Rotation getDirection() {
        return Rotation(std::atan2(y, x));
    }

    Vector2d rotate(Rotation rotation) {
        Vector2d result;
        result.x = x * std::cos(rotation.radians) - y * std::sin(rotation.radians);
        result.y = x * std::sin(rotation.radians) + y * std::cos(rotation.radians);
        return result;
    }

    double lenght() {
        return sqrt(x * x + y * y);
    }
};

struct Rect2d {
    Vector2d p1;
    Vector2d p2;

    // do not use
    Rect2d(): p1(Vector2d()), p2(Vector2d()) {};

    // do not use
    Rect2d(Vector2d center, double height, double width) {
        p1.x = center.x - width / 2;
        p1.y = center.y - height / 2;
        p2.x = center.x + width / 2;
        p2.y = center.y + height / 2;
    }

    // do not use
    Rect2d(Vector2d p1, Vector2d p2): p1(p1), p2(p2) {}

    bool isInside(Vector2d point) {
        return p1.x < point.x && point.x < p2.x && p1.y <=point.y && point.y < p2.y;
    }

    bool isInside(Rect2d rect) {
        return p1.x < rect.p1.x && rect.p2.x < p2.x && p1.y < rect.p1.y && rect.p2.y < p2.y;
    }

    Vector2d center() {
        return Vector2d(p1.x / 2 + p2.x / 2, p1.y/ 2 + p2.y / 2);
    }

    bool isIntersecting(Rect2d other) {
        bool horizontalIntersect = (p1.x < other.p1.x && other.p1.x < p2.x) || (p1.x < other.p2.x && other.p2.x < p2.x) || (other.p1.x < p1.x && p1.x < other.p2.x) || (other.p1.x < center().x && center().x< other.p2.x);
        bool verticalIntersect = (p1.y < other.p1.y && other.p1.y < p2.y) || (p1.y < other.p2.y && other.p2.y < p2.y) || (other.p1.y < p1.y && p1.y < other.p2.y)|| (other.p1.y < center().y && center().y < other.p2.y);
        return verticalIntersect && horizontalIntersect;
    }

    Vector2d dimensions() {
        return p2 - p1;
    }

    static Rect2d fromCenterAndDimensions(Vector2d center, Vector2d dimensions) {
        return Rect2d(center, dimensions.y, dimensions.x);
    }

    static Rect2d fromTwoCorners(Vector2d ap1, Vector2d ap2) {
        return Rect2d(Vector2d(std::min(ap1.x, ap2.x), std::min(ap1.y, ap2.y)),Vector2d(std::max(ap1.x, ap2.x), std::max(ap1.y, ap2.y)));
    }

    bool operator==(Rect2d other) {
        return p1 == other.p1 && p2 == other.p2;
    }

    bool operator!=(Rect2d other) {
        return p1 != other.p1 || p2 != other.p2;
    }
};



#endif // __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_