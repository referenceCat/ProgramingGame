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

public:
    Box(Rect2d aRect): rect(aRect) {}
    void move(Vector2d position);
    virtual void drawInfo();
    virtual void drawDebug();
    virtual void draw();
    Rect2d getRect();
    void update();
    double getTemperature();
    void setTemperature(double aTemperature);
    void addToGameWorld() override;
};

enum Resource {
    Regolith,
    Alloy,
    Silicon,
    Oxygen
};

class ResourceBoxPrototype: public Box {
    ALLEGRO_BITMAP* bitmap;
    Resource type;

public:
    ResourceBoxPrototype(Rect2d aRect, ALLEGRO_BITMAP* bitmap, Resource type): bitmap(bitmap), type(type), Box(aRect) {}
    void draw() override {
        GraphicsEngine::instance()->drawBitmap(getRect().p1, bitmap, 20, CommonValues::zBox);
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_