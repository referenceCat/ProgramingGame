#include <Box.hpp>
#include <GameWorld.hpp>
#include "../graphics/GraphicsEngine.hpp"

void Box::addToGameWorld() { GameWorld::instance()->addBox(this); }

void Box::draw()
{
    double z = 1.5;
    int r = (temperature < -255)
                ? 0
                : ((temperature > 0) ? 255 : (temperature + 255));
    int g = (temperature > 255)
                ? 0
                : ((temperature < 0) ? 255 : (-temperature + 255));
    int b =
        (temperature < -255 || temperature > 255)
            ? 0
            : ((temperature >= 0) ? (-temperature + 255) : (temperature + 255));
    ALLEGRO_COLOR color = al_map_rgb(r, g, b);

    GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zBox, color);
}

Rect2d Box::getRect() { return rect; }

void Box::setContent(BoxContent aContent) { content = aContent; }

BoxContent Box::getContent() { return content; }

void Box::update()
{
    if (temperature > 0)
        temperature -= 0.1;
}

double Box::getTemperature() { return temperature; }

void Box::setTemperature(double aTemperature) { temperature = aTemperature; }

void Box::move(Vector2d position)
{
    Vector2d dPosition = rect.center() - position;
    rect.p1 = rect.p1 + dPosition;
    rect.p2 = rect.p2 + dPosition;
}
