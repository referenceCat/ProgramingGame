#ifndef __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../common/common.hpp"
#include "GameObject.hpp"
#include "../graphics/GraphicsEngine.hpp"

class GameWorld;

class Box : public GameObject {
    Rect2d rect;
    double temperature = 0;
    bool grabbed = false;

public:
    Box(Rect2d aRect):
        rect(aRect) {
    }

    void move(Vector2d position);
    virtual void drawInfo();
    virtual void drawDebug();
    virtual void draw();
    Rect2d getRect();
    void update();
    double getTemperature();
    void setTemperature(double aTemperature);
    void addToGameWorld() override;
    bool isGrabbed();
    void setGrabbed(bool value);
};

enum Resource {
    Regolith,
    Alloy,
    Silicon,
    Oxygen
};

class ResourceBoxPrototype : public Box {
    ALLEGRO_BITMAP* bitmap;
    Resource type;

public:
    ResourceBoxPrototype(Rect2d aRect, ALLEGRO_BITMAP* bitmap, Resource type):
        bitmap(bitmap), type(type), Box(aRect) {
    }

    void draw() override;
    Resource getResource();
};

enum DataPointType {
    NoData,
    MaterialResearchData,
    ParticleResearchData,
    GeologyResearchData
};

class TapeBox : public Box { // TODO all bs
    ALLEGRO_COLOR lampColor(DataPointType);

public:
    inline static constexpr int dataPointsSize = 6;
    TapeBox(Vector2d aPos):
        Box(Rect2d::fromCenterAndDimensions(aPos, Vector2d(4, 2))) {};
    void draw() override;
    void writePoint(DataPointType point);
    bool isDataPointsEmpty();

    DataPointType getDataPoint(int index) {
        return dataPoints[index];
    }

    void setDataPoint(int index, DataPointType value) {
        dataPoints[index] = value;
    }

private:
    DataPointType dataPoints[dataPointsSize] = {NoData};
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_