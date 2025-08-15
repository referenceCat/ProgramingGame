#ifndef __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_

#include <../common/common.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

struct CameraParameters {
    Point2d position;
    double z;
    double fov; // horizontal
    Point2d displaySize;
    Rect2d renderingArea;
};

class GraphicsEngine {
    CameraParameters camera;
public:
    static Point2d transformPoint(Point2d originalPoint, double z, const CameraParameters& cameraParameters) {
        Point2d relativePoint = originalPoint - cameraParameters.position;

        Point2d proportionalPoint = relativePoint;
        proportionalPoint.x /= cameraParameters.fov / 2;
        proportionalPoint.y /= cameraParameters.fov / 2;
        proportionalPoint.x /= (z - cameraParameters.z) / -cameraParameters.z;
        proportionalPoint.y /= (z - cameraParameters.z) / -cameraParameters.z;

        Point2d resultPoint;
        double pixelPerUnit = cameraParameters.displaySize.x / 2;
        resultPoint.x = cameraParameters.displaySize.x / 2 + proportionalPoint.x * pixelPerUnit;
        resultPoint.y = cameraParameters.displaySize.y / 2 + proportionalPoint.y * pixelPerUnit;
        return resultPoint;
    }

    static Point2d transformPointInverse(Point2d originalPoint, const CameraParameters& cameraParameter, double z) {
        // TODO
    }

    static double transformScalar(double value, double z, const CameraParameters& cameraParameters) {
        value *= (z - cameraParameters.z) / -cameraParameters.z;
        value /= cameraParameters.fov;
        value *= cameraParameters.displaySize.x;
        return value;
    }

    void setCameraParameters(CameraParameters parameters) {
        camera = parameters;
    }

    CameraParameters getCameraParameters() {
        return camera;
    }

    void draw_point(Point2d aPoint, double z, ALLEGRO_COLOR color) {
        Point2d displayPoint = transformPoint(aPoint, z, camera);
        al_draw_filled_circle(displayPoint.x, displayPoint.y, 3, color);
    }

    void draw_rectangle(Rect2d aRect, double z, ALLEGRO_COLOR color, int thickness = 0) {
        Rect2d displayRect;
        displayRect.p1 = transformPoint(aRect.p1, z, camera);
        displayRect.p2 = transformPoint(aRect.p2, z, camera);
        if (thickness <= 0) al_draw_filled_rectangle(displayRect.p1.x, displayRect.p1.y, displayRect.p2.x, displayRect.p2.y, color);
        else al_draw_rectangle(displayRect.p1.x, displayRect.p1.y, displayRect.p2.x, displayRect.p2.y, color, thickness);
    }

    void draw_line(Point2d aPoint0, Point2d aPoint1, double z, ALLEGRO_COLOR color, int thickness = 0) {
        Point2d displayPoint0 = transformPoint(aPoint0, z, camera);
        Point2d displayPoint1 = transformPoint(aPoint1, z, camera);
        al_draw_line(displayPoint0.x, displayPoint0.y, displayPoint1.x, displayPoint1.y, color, thickness);
    }

    void draw_circle(Point2d aPoint, double r, double z, ALLEGRO_COLOR color, int thickness = 0) {
        Point2d displayPoint = transformPoint(aPoint, z, camera);
        double displayR = transformScalar(r, z, camera);
        if (thickness <= 0) al_draw_filled_circle(displayPoint.x, displayPoint.y, r, color);
        else al_draw_circle(displayPoint.x, displayPoint.y, r, color, thickness);
    }

    static GraphicsEngine* instance() {
        static GraphicsEngine instance;
        return &instance;
    }
};


#endif // __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_