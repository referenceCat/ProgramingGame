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

    RelativeRotation operator- (RelativeRotation other) {
        RelativeRotation result;
        result.radians = (radians - other.radians);
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

struct Vector2d;

struct Point2d {
    double x;
    double y;

    Point2d(double x, double y): x(x), y(y) {}
    Point2d(): x(0), y(0) {}

    Point2d operator+ (Vector2d vector);
};

struct Vector2d {
    double x;
    double y;

    Vector2d(): x(0), y(0) {}

    Vector2d(Point2d p1, Point2d p2) {
        x = p1.x - p2.x;
        y = p1.y - p2.y;
    }

    Vector2d(GlobalRotation rotation, double length) {
        x = cos(rotation.radians) * length;
        y = sin(rotation.radians) * length;
    }
};

struct Rect2d {
    Point2d p1;
    Point2d p2;

    Rect2d(Point2d center, double height, double width) {
        p1.x = center.x - width / 2;
        p1.y = center.y - height / 2;
        p2.x = center.x + width / 2;
        p2.y = center.y + height / 2;
    }

    bool isInside(Point2d point) {
        return p1.x <= point.x && point.x <= p2.x && p1.y <= point.y && point.y <= p2.y;
    }

    Point2d center() {
        return Point2d(p1.x / 2 + p2.x / 2, p1.y/ 2 + p2.y / 2);
    }
};

double degreesToRadians(int degrees);



#endif // __PROJECTS_PROGRAMINGGAME_SRC_COMMON_HPP_