#ifndef __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_

#include <../common/common.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <vector>
#include <iostream>
#include <filesystem>
#include <map>


struct CameraParameters {
    Vector2d position;
    double z;
    double fov; // horizontal
    Vector2d displayDimensions;
};

namespace CommonValues {
    constexpr double zModuleWalls = 0;
    constexpr double zModuleMainBackgroung = 0.4;
    constexpr double zModuleFarBackgroung = 2;
    constexpr double zMachinery = 0.3;
    constexpr double zMachineryFront = 0.1;
    constexpr double zMachineryBack = 0.35;
    constexpr double zBox = 0.2;
    constexpr double zArmBack = 0;
    constexpr double zArmFront = 0.01;
};

class GraphicsEngine {
    CameraParameters camera;

    struct Layer {
        double z;
        ALLEGRO_BITMAP* bitmap = nullptr;
    };

    struct BackgroundStar{
        Vector2d pos;
        double radius;
        ALLEGRO_COLOR color;
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

    double fRand(double fMin, double fMax) {
        double f = (double)rand() / RAND_MAX;
        return fMin + f * (fMax - fMin);
    }

    void initStars(std::vector<BackgroundStar> &stars, int n) {
        for (int i = 0; i < n; i++) {
            BackgroundStar star;
            star.color = al_map_rgb(fRand(150, 255), fRand(150, 255), fRand(150, 255));
            star.radius = fRand(0.3, 2);
            star.pos = Vector2d(fRand(0, 3000), fRand(0, 1500));
            stars.push_back(star);
        }
    }

public:
    ALLEGRO_FONT* debugFont = nullptr;

    std::map<std::string, ALLEGRO_BITMAP*> loadedBitmaps;
    // TODO remove ASAP
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

    ALLEGRO_BITMAP* xModuleLayer0 = nullptr;
    ALLEGRO_BITMAP* xModuleLayer1 = nullptr;

    ALLEGRO_BITMAP* corridorModuleLayer0 = nullptr;
    ALLEGRO_BITMAP* corridorModuleLayer1 = nullptr;

    ALLEGRO_BITMAP* beamLayer0 = nullptr;
    ALLEGRO_BITMAP* beamLayer1 = nullptr;

    ALLEGRO_BITMAP* junction3Layer0 = nullptr;
    ALLEGRO_BITMAP* junction3Layer1 = nullptr;

    ALLEGRO_BITMAP* endLayer0 = nullptr;
    ALLEGRO_BITMAP* endLayer1 = nullptr;
    
    // TODO memory leak then reloading bitmaps
    void loadImagesLegacyTesting() {
        backhroundTile100x100 = al_create_bitmap(100, 100);
        al_set_target_bitmap(backhroundTile100x100);
        al_draw_filled_rectangle(0, 0, 100, 100, al_map_rgb(50, 50, 50));
        for (int i = 0; i < 100; i++) {
            int x = i % 10;
            int y = i / 10;
            if ((x + y) % 2) al_draw_filled_rectangle(x * 10, y * 10, x * 10 + 10, y * 10 + 10, al_map_rgb(20, 20, 20));

        }

        furnaceSprite = al_load_bitmap("resources/assets/heater.png");

        baseSpite = al_load_bitmap("resources/assets/new_manipulator_base.png");
        segment0Sprite = al_load_bitmap("resources/assets/new_manipulator_segment0.png");
        segment1Sprite = al_load_bitmap("resources/assets/new_manipulator_segment1.png");

        assemblerBaseSprite = al_load_bitmap("resources/assets/assembler0.png");
        assemblerCyllindersSprite = al_load_bitmap("resources/assets/assembler1.png");
        assemblerPressSprite = al_load_bitmap("resources/assets/assembler3.png");
        assemblerPlateSprite = al_load_bitmap("resources/assets/assembler2.png");

        controllerSprite = al_load_bitmap("resources/assets/controller.png");

        boxCreatorDestroyerBaseSprite = al_load_bitmap("resources/assets/box_creator_destroyer_base.png");

        worldLayer0 = al_load_bitmap("resources/assets/background0.png");
        worldLayer1 = al_load_bitmap("resources/assets/background1_no_shadow.png");
        worldLayer2 = al_load_bitmap("resources/assets/background2.png");

        xModuleLayer0 = al_load_bitmap("resources/assets/oldModules/xcorridor_background.png");
        xModuleLayer1 = al_load_bitmap("resources/assets/oldModules/xcorridor_walls.png");

        corridorModuleLayer0 = al_load_bitmap("resources/assets/oldModules/corridor_background.png");
        corridorModuleLayer1 = al_load_bitmap("resources/assets/oldModules/corridor_walls.png");

        beamLayer0 = al_load_bitmap("resources/assets/oldModules/beam1.png");
        beamLayer1 = al_load_bitmap("resources/assets/oldModules/beam3.png");

        junction3Layer0 = al_load_bitmap("resources/assets/oldModules/junction3_background.png");
        junction3Layer1 = al_load_bitmap("resources/assets/oldModules/junction3_walls.png");

        endLayer0 = al_load_bitmap("resources/assets/oldModules/end_background.png");
        endLayer1 = al_load_bitmap("resources/assets/oldModules/end_walls.png");
    }

    void loadBitmaps() { 

        for (const auto & [key, value] : loadedBitmaps)
            al_destroy_bitmap(value);

        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator("resources/assets")) {
            loadedBitmaps[dirEntry.path().string()] = al_load_bitmap(dirEntry.path().string().c_str());
            std::cout << dirEntry.path().string() << std::endl;
        }
    }

    ALLEGRO_BITMAP* getBitmap(std::string path) {
        return loadedBitmaps[path];
    }

    void drawDebugBackgroung() { // TODO very slow way to draw tiles
        double tileSize = transformScalar(al_get_bitmap_width(backhroundTile100x100), 0);
        Vector2d position = transformPoint(Vector2d(static_cast<int> (camera.position.x) / 100 * 100, static_cast<int> (camera.position.y) / 100 * 100) , 0);
        for (int x = -10; x < 10; x++) for (int y = -10; y < 10; y++) {
            al_draw_scaled_bitmap(backhroundTile100x100, 0, 0, 100, 100, position.x + x * tileSize, position.y + y * tileSize, tileSize, tileSize, 0);
        }
    }

    void drawDebugBackgroung2() { // TODO very slow way to draw tiles
        for (int x = -50; x < 50; x++) {
            for (int y = -50; y < 50; y++) {
                GraphicsEngine::instance()->drawPoint(Vector2d(x + 0.5, y + 0.5), 0, al_map_rgb(200, 200, 200), 1.5);
            }
        }
    }

    void drawStarsBackgroung() {
        static std::vector<BackgroundStar> stars;
        if (!stars.size()) initStars(stars, 2000);

        for (auto star: stars) {
            al_draw_filled_circle(star.pos.x, star.pos.y, star.radius, star.color);
        }
    }

    

    Vector2d transformPoint(Vector2d originalPoint, double z) {
        Vector2d relativePoint = originalPoint - camera.position;

        Vector2d proportionalPoint = relativePoint;
        proportionalPoint.x /= camera.fov / 2;
        proportionalPoint.y /= camera.fov / 2;
        proportionalPoint.x /= (z - camera.z) / -camera.z;
        proportionalPoint.y /= (z - camera.z) / -camera.z;

        Vector2d resultPoint;
        double pixelPerUnit = camera.displayDimensions.x / 2;
        resultPoint.x = camera.displayDimensions.x / 2 + proportionalPoint.x * pixelPerUnit;
        resultPoint.y = camera.displayDimensions.y / 2 + proportionalPoint.y * pixelPerUnit;
        return resultPoint;
    }

    Vector2d transformPointInverse(Vector2d originalPoint) {
        Vector2d result = camera.position;
        double pixelsPerUnit = camera.displayDimensions.x / camera.fov;
        result = result - Vector2d(camera.fov / 2, camera.displayDimensions.y / 2 / pixelsPerUnit);
        result = result + originalPoint / pixelsPerUnit;
        return result;
    }

    double transformScalar(double value, double z) {
        value /= (z - camera.z) / -camera.z;
        value /= camera.fov;
        value *= camera.displayDimensions.x;
        return value;
    }

    void setCameraParameters(CameraParameters parameters) {
        camera = parameters;
    }

    CameraParameters getCameraParameters() {
        return camera;
    }

    void changeDisplayDimensions(Vector2d d) {
        auto oldCamera = camera;
        camera.displayDimensions = d;
        camera.fov *= d.x / oldCamera.displayDimensions.x;

    }

    void drawPoint(Vector2d aPoint, double z, ALLEGRO_COLOR color, double r = 3) {
        setLayerAsTargetBitmap(z);
        aPoint = transformPoint(aPoint, z);
        al_draw_filled_circle(aPoint.x, aPoint.y, r, color);
    }

    void drawRectangle(Rect2d aRect, double z, ALLEGRO_COLOR color, int thickness = 0) {
        setLayerAsTargetBitmap(z);
        aRect.p1 = transformPoint(aRect.p1, z);
        aRect.p2 = transformPoint(aRect.p2, z);
        if (thickness <= 0) al_draw_filled_rectangle(aRect.p1.x, aRect.p1.y, aRect.p2.x, aRect.p2.y, color);
        else al_draw_rectangle(aRect.p1.x, aRect.p1.y, aRect.p2.x, aRect.p2.y, color, thickness);
    }

    void drawLine(Vector2d aPoint0, Vector2d aPoint1, double z, ALLEGRO_COLOR color, int thickness = 0) {
        setLayerAsTargetBitmap(z);
        aPoint0 = transformPoint(aPoint0, z);
        aPoint1 = transformPoint(aPoint1, z);
        al_draw_line(aPoint0.x, aPoint0.y, aPoint1.x, aPoint1.y, color, thickness);
    }

    void drawCircle(Vector2d aPoint, double r, double z, ALLEGRO_COLOR color, int thickness = 0) {
        setLayerAsTargetBitmap(z);
        aPoint = transformPoint(aPoint, z);
        r = transformScalar(r, z);
        if (thickness <= 0) al_draw_filled_circle(aPoint.x, aPoint.y, r, color);
        else al_draw_circle(aPoint.x, aPoint.y, r, color, thickness);
    }

    void drawPolygon(std::vector<Vector2d> vertices, double z, ALLEGRO_COLOR color) {
        setLayerAsTargetBitmap(z);

        float dots [vertices.size() * 2];
        for (int i = 0; i < vertices.size(); i++) {
            Vector2d tp = transformPoint(vertices.at(i), z);
            dots[i * 2] = tp.x;
            dots[i * 2 + 1] = tp.y;
        }
        al_draw_filled_polygon(dots, vertices.size(), color);
    }

    void drawBitmap(Vector2d aPoint, ALLEGRO_BITMAP* bitmap, double pixelsPerUnit, double z, Vector2d bitmapPivot = Vector2d(), Rotation bitmapRotation = Rotation()) {
        setLayerAsTargetBitmap(z);
        aPoint = transformPoint(aPoint, z);
        double sizeMultiplayer = transformScalar(1, z) / pixelsPerUnit;
        al_draw_scaled_rotated_bitmap(bitmap, bitmapPivot.x, bitmapPivot.y, aPoint.x, aPoint.y, sizeMultiplayer, sizeMultiplayer, bitmapRotation.radians, 0);
    }

    static GraphicsEngine* instance() {
        static GraphicsEngine instance;
        return &instance;
    }

    void draw() {
        drawStarsBackgroung();
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