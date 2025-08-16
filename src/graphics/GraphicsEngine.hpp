#ifndef __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_

#include <../common/common.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <vector>
#include <iostream>


struct CameraParameters {
    Point2d position;
    double z;
    double fov; // horizontal
    Point2d displaySize;
    Rect2d renderingArea;
};

class GraphicsEngine {
    CameraParameters camera;

    struct Layer {
        double z;
        ALLEGRO_BITMAP* bitmap = nullptr;
    };

    std::vector<Layer> layers{};

    void setLayerAsTargetBitmap(double z) {
        for (int i = 0; i < layers.size(); i++) {
            if (layers.at(i).z == z) {
                al_set_target_bitmap(layers.at(i).bitmap);
                return;
            } else if (layers.at(i).z >= z) {
                layers.insert(layers.begin() + i, Layer{z, nullptr});
                layers.at(i).bitmap = al_create_bitmap(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()));
                al_set_target_bitmap(layers.at(i).bitmap);
                al_clear_to_color(al_map_rgba(0, 0, 0, 0));
                return;
            }
        }

        layers.push_back(Layer{z, nullptr});
        layers.back().bitmap = al_create_bitmap(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display()));
        al_set_target_bitmap(layers.back().bitmap);
        al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    }

public:
    ALLEGRO_BITMAP* image = nullptr;

    void loadImages() {
        image = al_load_bitmap("resources/assets/heater.png");
    }

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

    void drawPoint(Point2d aPoint, double z, ALLEGRO_COLOR color) {
        setLayerAsTargetBitmap(z);
        Point2d displayPoint = transformPoint(aPoint, z, camera);
        al_draw_filled_circle(displayPoint.x, displayPoint.y, 3, color);
    }

    void drawRectangle(Rect2d aRect, double z, ALLEGRO_COLOR color, int thickness = 0) {
        setLayerAsTargetBitmap(z);
        Rect2d displayRect;
        displayRect.p1 = transformPoint(aRect.p1, z, camera);
        displayRect.p2 = transformPoint(aRect.p2, z, camera);
        if (thickness <= 0) al_draw_filled_rectangle(displayRect.p1.x, displayRect.p1.y, displayRect.p2.x, displayRect.p2.y, color);
        else al_draw_rectangle(displayRect.p1.x, displayRect.p1.y, displayRect.p2.x, displayRect.p2.y, color, thickness);
    }

    void drawLine(Point2d aPoint0, Point2d aPoint1, double z, ALLEGRO_COLOR color, int thickness = 0) {
        setLayerAsTargetBitmap(z);
        Point2d displayPoint0 = transformPoint(aPoint0, z, camera);
        Point2d displayPoint1 = transformPoint(aPoint1, z, camera);
        al_draw_line(displayPoint0.x, displayPoint0.y, displayPoint1.x, displayPoint1.y, color, thickness);
    }

    void drawCircle(Point2d aPoint, double r, double z, ALLEGRO_COLOR color, int thickness = 0) {
        setLayerAsTargetBitmap(z);
        Point2d displayPoint = transformPoint(aPoint, z, camera);
        double displayR = transformScalar(r, z, camera);
        if (thickness <= 0) al_draw_filled_circle(displayPoint.x, displayPoint.y, r, color);
        else al_draw_circle(displayPoint.x, displayPoint.y, r, color, thickness);
    }

    void drawBitmap(Point2d aPoint, ALLEGRO_BITMAP* bitmap, double z) {
        setLayerAsTargetBitmap(z);
        Point2d displayPoint = transformPoint(aPoint, z, camera);
        double displayW = transformScalar(al_get_bitmap_width(bitmap), z, camera);
        double displayH = transformScalar(al_get_bitmap_height(bitmap), z, camera);
        al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), displayPoint.x, displayPoint.y, displayW, displayH, 0);
    }

    static GraphicsEngine* instance() {
        static GraphicsEngine instance;
        return &instance;
    }

    void draw() {
        for (int i = layers.size() - 1; i >= 0; i--) {
            al_draw_bitmap(layers.at(i).bitmap, 0, 0, 0); 
        }
    }

    void clearBitmaps() {
        for (int i = layers.size() - 1; i >= 0; i--) {
            al_destroy_bitmap(layers.at(i).bitmap);
        }
        layers.clear();
    }
};


#endif // __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_