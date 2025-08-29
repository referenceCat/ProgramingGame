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
    ALLEGRO_BITMAP* furnaceSprite = nullptr;
    ALLEGRO_BITMAP* baseSpite = nullptr;
    ALLEGRO_BITMAP* segment0Sprite = nullptr;
    ALLEGRO_BITMAP* segment1Sprite = nullptr;
    ALLEGRO_BITMAP* assemblerBaseSprite = nullptr;
    ALLEGRO_BITMAP* assemblerCyllindersSprite = nullptr;
    ALLEGRO_BITMAP* assemblerPressSprite = nullptr;
    ALLEGRO_BITMAP* assemblerPlateSprite = nullptr;
    ALLEGRO_BITMAP* backhroundTile100x100 = nullptr;
    ALLEGRO_BITMAP* controllerSprite = nullptr;
    ALLEGRO_BITMAP* boxCreatorDestroyerBaseSprite = nullptr;

    ALLEGRO_BITMAP* worldLayer0 = nullptr;
    ALLEGRO_BITMAP* worldLayer1 = nullptr;
    ALLEGRO_BITMAP* worldLayer2 = nullptr;
    

    void loadImages() {
        backhroundTile100x100 = al_create_bitmap(100, 100);
        al_set_target_bitmap(backhroundTile100x100);
        al_draw_filled_rectangle(0, 0, 100, 100, al_map_rgb(50, 50, 50));
        for (int i = 0; i < 100; i++) {
            int x = i % 10;
            int y = i / 10;
            if ((x + y) % 2) al_draw_filled_rectangle(x * 10, y * 10, x * 10 + 10, y * 10 + 10, al_map_rgb(20, 20, 20));

        }

        furnaceSprite = al_load_bitmap("resources/assets/heater.png");

        baseSpite = al_load_bitmap("resources/assets/base.png");
        segment0Sprite = al_load_bitmap("resources/assets/segment0.png");
        segment1Sprite = al_load_bitmap("resources/assets/segment1.png");

        assemblerBaseSprite = al_load_bitmap("resources/assets/assembler0.png");
        assemblerCyllindersSprite = al_load_bitmap("resources/assets/assembler1.png");
        assemblerPressSprite = al_load_bitmap("resources/assets/assembler3.png");
        assemblerPlateSprite = al_load_bitmap("resources/assets/assembler2.png");

        controllerSprite = al_load_bitmap("resources/assets/controller.png");

        boxCreatorDestroyerBaseSprite = al_load_bitmap("resources/assets/box_creator_destroyer_base.png");

        worldLayer0 = al_load_bitmap("resources/assets/background0.png");
        worldLayer1 = al_load_bitmap("resources/assets/background1.png");
        worldLayer2 = al_load_bitmap("resources/assets/background2.png");
    }

    void drawBackground() { // TODO very slow way to draw tiles
        double tileSize = transformScalar(al_get_bitmap_width(backhroundTile100x100), 0, camera);
        Point2d position = transformPoint(Point2d(static_cast<int> (camera.position.x) / 100 * 100, static_cast<int> (camera.position.y) / 100 * 100) , 0, camera);
        for (int x = -10; x < 10; x++) for (int y = -10; y < 10; y++) {
            al_draw_scaled_bitmap(backhroundTile100x100, 0, 0, 100, 100, position.x + x * tileSize, position.y + y * tileSize, tileSize, tileSize, 0);
        }
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
        value /= (z - cameraParameters.z) / -cameraParameters.z;
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

    void drawBitmap(Point2d aPoint, ALLEGRO_BITMAP* bitmap, double z, Point2d bitmapPivot = Point2d(), GlobalRotation bitmapRotation = GlobalRotation()) {
        setLayerAsTargetBitmap(z);
        Point2d displayPoint = transformPoint(aPoint, z, camera);
        // double displayW = transformScalar(al_get_bitmap_width(bitmap), z, camera);
        // double displayH = transformScalar(al_get_bitmap_height(bitmap), z, camera);
        // al_draw_scaled_bitmap(bitmap, 0, 0, al_get_bitmap_width(bitmap), al_get_bitmap_height(bitmap), displayPoint.x, displayPoint.y, displayW, displayH, 0);

        double sizeMultiplayer = transformScalar(1, z, camera);
        al_draw_scaled_rotated_bitmap(bitmap, bitmapPivot.x, bitmapPivot.y, displayPoint.x, displayPoint.y, sizeMultiplayer, sizeMultiplayer, bitmapRotation.radians, 0);
    }

    static GraphicsEngine* instance() {
        static GraphicsEngine instance;
        return &instance;
    }

    void draw() {
        // drawBackground();
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