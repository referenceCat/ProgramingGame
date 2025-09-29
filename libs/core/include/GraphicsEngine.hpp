#ifndef __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_

#include <common.hpp>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <cmath>
#include <map>
#include <string>

struct CameraParameters {
    Vector2d position;
    double z;
    double fov; // horizontal
    Vector2d displayDimensions;
};

namespace CommonValues {
constexpr double zDebug = NAN;
constexpr double zModuleWalls = 0;
constexpr double zModuleMainBackgroung = 0.3;
constexpr double zSurface = 0.5;
constexpr double zModuleFarBackgroung = 0.7;
constexpr double zModuleVeryFarBackgroung = 1.5;
constexpr double zMachinery = 0.15;
constexpr double zMachineryFront = -0.1;
constexpr double zMachineryBack = 0.25;
constexpr double zBox = 0.01;
constexpr double zArm = 0;
}; // namespace CommonValues

class GraphicsEngine {
    CameraParameters camera;

    struct Layer {
        double z;
        ALLEGRO_BITMAP* bitmap = nullptr;
    };

    struct BackgroundStar {
        Vector2d pos;
        double radius;
        ALLEGRO_COLOR color;
    };

    std::vector<Layer> layers{};
    Layer debugLayer{0, nullptr};

    std::map<std::string, ALLEGRO_BITMAP*> loadedBitmaps;

    void setLayerAsTargetBitmap(double z);
    double fRand(double fMin, double fMax);
    void initStars(std::vector<BackgroundStar>& stars, int n);

public:
    ALLEGRO_FONT* debugFont = nullptr;

    static GraphicsEngine* instance();
    void drawLayers();

    void loadBitmaps();
    ALLEGRO_BITMAP* getBitmap(std::string path);
    void clearBitmaps();

    Vector2d transformPoint(Vector2d originalPoint, double z);
    Vector2d transformPointInverse(Vector2d originalPoint);
    double transformScalar(double value, double z);

    void setCameraParameters(CameraParameters parameters);
    CameraParameters getCameraParameters();
    void changeDisplayDimensions(Vector2d d);

    void drawPoint(Vector2d aPoint, double z, ALLEGRO_COLOR color, double r = 3);
    void drawRectangle(Rect2d aRect, double z, ALLEGRO_COLOR color, double thickness = 0);
    void drawLine(Vector2d aPoint0, Vector2d aPoint1, double z, ALLEGRO_COLOR color, double thickness = 0.1);
    void drawCircle(Vector2d aPoint, double r, double z, ALLEGRO_COLOR color, double thickness = 0);
    void drawPolygon(std::vector<Vector2d> vertices, double z, ALLEGRO_COLOR color);
    void drawBitmap(Vector2d aPoint, ALLEGRO_BITMAP* bitmap, double pixelsPerUnit, double z, Vector2d bitmapPivot = Vector2d(), Rotation bitmapRotation = Rotation());
    void drawText(Vector2d aPoint, std::string text, ALLEGRO_FONT* font, double z, ALLEGRO_COLOR color, bool centered);
    void drawArc(Vector2d aPoint, Rotation from, Rotation to, double r, double z, ALLEGRO_COLOR color, double thickness = 0.1);
    void drawArcProgressBar(Vector2d aPoint, double fraction, double r, double z, ALLEGRO_COLOR color, double thickness = 0.1);
    void drawDebugBackgroung2();
    void drawStarsBackgroung();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_GRAPHICS_GRAPHICSENGINE_HPP_