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

    void setCameraParameters(CameraParameters parameters) {
        camera = parameters;
    }

    CameraParameters getCameraParameters() {
        return camera;
    }

    void draw_point(Point2d originalPoint, double z, ALLEGRO_COLOR color) {
        Point2d displayPoint = transformPoint(originalPoint, z, camera);
        al_draw_filled_circle(displayPoint.x, displayPoint.y, 3, color);
    }

    static GraphicsEngine* instance() {
        static GraphicsEngine instance;
        return &instance;
    }
};


#endif // __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_