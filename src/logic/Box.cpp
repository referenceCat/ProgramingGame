#include <Box.hpp>
#include <GameWorld.hpp>
#include "../graphics/GraphicsEngine.hpp"

void Box::addToGameWorld() { GameWorld::instance()->addBox(this); }

bool Box::isGrabbed() {
    return grabbed;
}

void Box::setGrabbed(bool value) {
    grabbed = value;
}

void Box::draw() {}

Rect2d Box::getRect() { return rect; }

void Box::update() {
    if (temperature > 0)
        temperature -= 0.1;
}

double Box::getTemperature() { return temperature; }

void Box::setTemperature(double aTemperature) { temperature = aTemperature; }

void Box::move(Vector2d position) {
    Vector2d dPosition = position - rect.center();
    rect.p1 = rect.p1 + dPosition;
    rect.p2 = rect.p2 + dPosition;
}

void Box::drawInfo() {
}

void Box::drawDebug() {
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

    GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zDebug, color);
}

void ResourceBoxPrototype::draw() {
    GraphicsEngine::instance()->drawBitmap(getRect().p1, bitmap, 20, CommonValues::zBox);
}

Resource ResourceBoxPrototype::getResource()
{
    return type;
}

ALLEGRO_COLOR TapeBox::lampColor(DataPointType type) {
    if (type == MaterialResearchData) return al_map_rgb(255, 50, 50);
    if (type == ParticleResearchData) return al_map_rgb(50, 255, 50);
    if (type == GeologyResearchData) return al_map_rgb(50, 50, 255);
    return al_map_rgba(0, 0, 0, 30);
}

void TapeBox::draw() {
    GraphicsEngine::instance()->drawBitmap(getRect().p1, GraphicsEngine::instance()->getBitmap("resources/assets/boxes/Tape/empty.png"), 20, CommonValues::zBox);
    for (int i = 0; i < dataPointsSize; i++) {
        auto color = lampColor(dataPoints[i]);
        GraphicsEngine::instance()->drawCircle(getRect().p1 + Vector2d(2.35 + i * 0.185, 1.8), 0.09, CommonValues::zBox, color);
    }
}

void TapeBox::writePoint(DataPointType point) {
    for (int i = 0; i < dataPointsSize; i++) {
        if (dataPoints[i] == NoData) {
            dataPoints[i] = point;
            return;
        }
    }
}

bool TapeBox::isDataPointsEmpty() {
    for (int i = 0; i < dataPointsSize; i++) {
        if (dataPoints[i] != NoData) {
            return false;
        }
    }

    return true;
}
