#ifndef __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_

#include <math.h>

struct RelativeRotation {
    double radians;

    RelativeRotation(): radians(0) {}

    RelativeRotation(double radians): radians(radians) {}

    RelativeRotation operator+ (RelativeRotation other) {
        RelativeRotation result;
        result.radians = (radians + other.radians);
        return result;
    }
};

struct GlobalRotation {
    double radians = 0;

    GlobalRotation operator+ (RelativeRotation other) {
        GlobalRotation result;
        result.radians = std::fmod(radians + other.radians, M_PI * 2);
        return result;
    }

    RelativeRotation operator- (GlobalRotation other) {
        RelativeRotation result;
        result.radians = std::fmod(radians - other.radians, M_PI * 2);
        return result;
    }
};

struct Vector2d {
    double x;
    double y;

    Vector2d(GlobalRotation rotation, double length) {
        x = cos(rotation.radians) * length;
        y = sin(rotation.radians) * length;
    }
};

struct Point2d {
    double x;
    double y;

    Point2d(double x, double y): x(x), y(y) {}
    Point2d(): x(0), y(0) {}

    Point2d operator+ (Vector2d vector) {
        return Point2d(x + vector.x, y + vector.y);
    }
};



#endif // __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_