#ifndef __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_

#include <math.h>

// TODO remove global/local and vector/point 

double degreesToRadians(double degrees);

double radiansToDegrees(double radians);

struct Rotation {
    double radians = 0;

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
        return degreesToRadians(radians);
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

    Vector2d operator+ (Vector2d other);
    Vector2d operator- (Vector2d other);

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

    Rect2d(): p1(Vector2d()), p2(Vector2d()) {};

    Rect2d(Vector2d center, double height, double width) {
        p1.x = center.x - width / 2;
        p1.y = center.y - height / 2;
        p2.x = center.x + width / 2;
        p2.y = center.y + height / 2;
    }

    Rect2d(Vector2d p1, Vector2d p2): p1(p1), p2(p2) {}

    bool isInside(Vector2d point) {
        return p1.x <= point.x && point.x <= p2.x && p1.y <= point.y && point.y <= p2.y;
    }

    bool isInside(Rect2d rect) {
        return p1.x <= rect.p1.x && rect.p2.x <= p2.x && p1.y <= rect.p1.y && rect.p2.y <= p2.y;
    }

    bool isIntersecting(Rect2d rect) {
        bool horizontalIntersect = (p1.x <= rect.p1.x && rect.p1.x <= p2.x) || (p1.x <= rect.p2.x && rect.p2.x <= p2.x) || (rect.p1.x <= p1.x && p1.x <= rect.p2.x);
        bool verticalIntersect = (p1.y <= rect.p1.y && rect.p1.y <= p2.y) || (p1.y <= rect.p2.y && rect.p2.y <= p2.y) || (rect.p1.y <= p1.y && p1.y <= rect.p2.y);
        return verticalIntersect && horizontalIntersect;
    }

    Vector2d center() {
        return Vector2d(p1.x / 2 + p2.x / 2, p1.y/ 2 + p2.y / 2);
    }

    Vector2d dimensions() {
        return p2 - p1;
    }
};



#endif // __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_