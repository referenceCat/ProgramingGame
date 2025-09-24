#include "GraphicsEngine.hpp"
#include <filesystem>
#include <iostream>
#include <allegro5/allegro_image.h>

void GraphicsEngine::loadBitmaps()
{

    for (const auto &[key, value] : loadedBitmaps)
        al_destroy_bitmap(value);

    for (const auto &dirEntry :
         std::filesystem::recursive_directory_iterator("resources/assets"))
    {
        loadedBitmaps[dirEntry.path().string()] =
            al_load_bitmap(dirEntry.path().string().c_str());
        std::cout << dirEntry.path().string() << std::endl;
    }
}

ALLEGRO_BITMAP *GraphicsEngine::getBitmap(std::string path)
{
    return loadedBitmaps[path];
}

void GraphicsEngine::drawDebugBackgroung2()
{ // TODO very slow way to draw
  // tiles
    for (int x = -50; x < 50; x++)
    {
        for (int y = -50; y < 50; y++)
        {
            GraphicsEngine::instance()->drawPoint(Vector2d(x + 0.5, y + 0.5), 0,
                                                  al_map_rgb(200, 200, 200), 1.5);
        }
    }
}

void GraphicsEngine::drawStarsBackgroung()
{
    static std::vector<BackgroundStar> stars;
    if (!stars.size())
        initStars(stars, 2000);

    for (auto star : stars)
    {
        al_draw_filled_circle(star.pos.x, star.pos.y, star.radius, star.color);
    }
}

Vector2d GraphicsEngine::transformPoint(Vector2d originalPoint, double z)
{
    Vector2d relativePoint = originalPoint - camera.position;

    Vector2d proportionalPoint = relativePoint;
    proportionalPoint.x /= camera.fov / 2;
    proportionalPoint.y /= camera.fov / 2;
    proportionalPoint.x /= (z - camera.z) / -camera.z;
    proportionalPoint.y /= (z - camera.z) / -camera.z;

    Vector2d resultPoint;
    double pixelPerUnit = camera.displayDimensions.x / 2;
    resultPoint.x =
        camera.displayDimensions.x / 2 + proportionalPoint.x * pixelPerUnit;
    resultPoint.y =
        camera.displayDimensions.y / 2 + proportionalPoint.y * pixelPerUnit;
    return resultPoint;
}

Vector2d GraphicsEngine::transformPointInverse(Vector2d originalPoint)
{
    Vector2d result = camera.position;
    double pixelsPerUnit = camera.displayDimensions.x / camera.fov;
    result = result - Vector2d(camera.fov / 2,
                               camera.displayDimensions.y / 2 / pixelsPerUnit);
    result = result + originalPoint / pixelsPerUnit;
    return result;
}

double GraphicsEngine::transformScalar(double value, double z)
{
    value /= (z - camera.z) / -camera.z;
    value /= camera.fov;
    value *= camera.displayDimensions.x;
    return value;
}

void GraphicsEngine::setCameraParameters(CameraParameters parameters)
{
    camera = parameters;
}

CameraParameters GraphicsEngine::getCameraParameters() { return camera; }

void GraphicsEngine::changeDisplayDimensions(Vector2d d)
{
    auto oldCamera = camera;
    camera.displayDimensions = d;
    camera.fov *= d.x / oldCamera.displayDimensions.x;
}

void GraphicsEngine::drawPoint(Vector2d aPoint, double z, ALLEGRO_COLOR color,
                               double r)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aPoint = transformPoint(aPoint, z);
    al_draw_filled_circle(aPoint.x, aPoint.y, r, color);
}

void GraphicsEngine::drawRectangle(Rect2d aRect, double z, ALLEGRO_COLOR color,
                                   int thickness)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aRect.p1 = transformPoint(aRect.p1, z);
    aRect.p2 = transformPoint(aRect.p2, z);
    if (thickness <= 0)
        al_draw_filled_rectangle(aRect.p1.x, aRect.p1.y, aRect.p2.x, aRect.p2.y,
                                 color);
    else
        al_draw_rectangle(aRect.p1.x, aRect.p1.y, aRect.p2.x, aRect.p2.y, color,
                          thickness);
}

void GraphicsEngine::drawLine(Vector2d aPoint0, Vector2d aPoint1, double z,
                              ALLEGRO_COLOR color, int thickness)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aPoint0 = transformPoint(aPoint0, z);
    aPoint1 = transformPoint(aPoint1, z);
    al_draw_line(aPoint0.x, aPoint0.y, aPoint1.x, aPoint1.y, color, thickness);
}

void GraphicsEngine::drawCircle(Vector2d aPoint, double r, double z,
                                ALLEGRO_COLOR color, int thickness)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aPoint = transformPoint(aPoint, z);
    r = transformScalar(r, z);
    if (thickness <= 0)
        al_draw_filled_circle(aPoint.x, aPoint.y, r, color);
    else
        al_draw_circle(aPoint.x, aPoint.y, r, color, thickness);
}

void GraphicsEngine::drawArcProgressBar(Vector2d aPoint, double fraction, double r, double z, ALLEGRO_COLOR color, double thickness) {
    drawArc(aPoint, M_PI / 2, M_PI * fraction * 2 + M_PI / 2, r, z, color, thickness);
}

void GraphicsEngine::drawArc(Vector2d aPoint, Rotation from, Rotation to, double r, double z, ALLEGRO_COLOR color, double thickness)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aPoint = transformPoint(aPoint, z);
    r = transformScalar(r, z);
    thickness = transformScalar(thickness, z);
    al_draw_arc(aPoint.x, aPoint.y, r, from.radians, to.radians - from.radians, color, thickness);
}

void GraphicsEngine::drawPolygon(std::vector<Vector2d> vertices, double z,
                                 ALLEGRO_COLOR color)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    float dots[vertices.size() * 2];
    for (int i = 0; i < vertices.size(); i++)
    {
        Vector2d tp = transformPoint(vertices.at(i), z);
        dots[i * 2] = tp.x;
        dots[i * 2 + 1] = tp.y;
    }
    al_draw_filled_polygon(dots, vertices.size(), color);
}

void GraphicsEngine::drawBitmap(Vector2d aPoint, ALLEGRO_BITMAP *bitmap,
                                double pixelsPerUnit, double z,
                                Vector2d bitmapPivot, Rotation bitmapRotation)
{
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aPoint = transformPoint(aPoint, z);
    double sizeMultiplayer = transformScalar(1, z) / pixelsPerUnit;
    al_draw_scaled_rotated_bitmap(bitmap, bitmapPivot.x, bitmapPivot.y, aPoint.x,
                                  aPoint.y, sizeMultiplayer, sizeMultiplayer,
                                  bitmapRotation.radians, 0);
}

void GraphicsEngine::drawText(Vector2d aPoint, std::string text, ALLEGRO_FONT *font, double z, ALLEGRO_COLOR color, bool centered) { // TODO make resizable
    setLayerAsTargetBitmap(z);
    if (isnan(z))
        z = 0;

    aPoint = transformPoint(aPoint, z);
    double h = al_get_font_line_height(font) + 2;
    double w = al_get_text_width(font, text.c_str());
    if (centered) aPoint = aPoint - Vector2d(w, h);
    al_draw_text(font, color, aPoint.x, aPoint.y, 0, text.c_str());
}

GraphicsEngine *GraphicsEngine::instance()
{
    static GraphicsEngine instance;
    return &instance;
}

void GraphicsEngine::draw()
{
    drawStarsBackgroung();
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        al_draw_bitmap(layers.at(i).bitmap, 0, 0, 0);
    }
    if (debugLayer.bitmap != nullptr) al_draw_bitmap(debugLayer.bitmap, 0, 0, 0);
}

void GraphicsEngine::clearBitmaps()
{
    for (int i = layers.size() - 1; i >= 0; i--)
    {
        al_destroy_bitmap(layers.at(i).bitmap);
    }
    layers.clear();
    al_destroy_bitmap(debugLayer.bitmap);
    debugLayer.bitmap = nullptr;
}

void GraphicsEngine::setLayerAsTargetBitmap(double z)
{
    if (isnan(z))
    { // check if debug layer should be used (cant use == for NANs)
        if (debugLayer.bitmap == nullptr)
        {
            debugLayer.bitmap =
                al_create_bitmap(al_get_display_width(al_get_current_display()),
                                 al_get_display_height(al_get_current_display()));
            al_set_target_bitmap(debugLayer.bitmap);
            al_clear_to_color(al_map_rgba(0, 0, 0, 0));
        }
        else
        {
            al_set_target_bitmap(debugLayer.bitmap);
        }
        return;
    }

    for (int i = 0; i < layers.size(); i++)
    {
        if (layers.at(i).z == z)
        { // layer is already exists
            al_set_target_bitmap(layers.at(i).bitmap);
            return;
        }
        else if (layers.at(i).z >= z)
        { // layer doesnt exist and must be inserted
          // before other layer
            layers.insert(layers.begin() + i, Layer{z, nullptr});
            layers.at(i).bitmap =
                al_create_bitmap(al_get_display_width(al_get_current_display()),
                                 al_get_display_height(al_get_current_display()));
            al_set_target_bitmap(layers.at(i).bitmap);
            al_clear_to_color(al_map_rgba(0, 0, 0, 0));
            return;
        }
    }

    layers.push_back(
        Layer{z, nullptr}); // layer doesnt exist and must be inserted last
    layers.back().bitmap =
        al_create_bitmap(al_get_display_width(al_get_current_display()),
                         al_get_display_height(al_get_current_display()));
    al_set_target_bitmap(layers.back().bitmap);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
}

double GraphicsEngine::fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void GraphicsEngine::initStars(std::vector<BackgroundStar> &stars, int n)
{
    for (int i = 0; i < n; i++)
    {
        BackgroundStar star;
        star.color = al_map_rgb(fRand(150, 255), fRand(150, 255), fRand(150, 255));
        star.radius = fRand(0.3, 2);
        star.pos = Vector2d(fRand(0, 4000), fRand(0, 2000));
        stars.push_back(star);
    }
}
