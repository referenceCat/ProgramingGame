#ifndef __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../common/common.hpp"
#include "GameObject.hpp"

class GameWorld;

enum BoxContent {
    EmptyBox,
    IronPlate,
    HeavyIronPlate,
    VeryHeavyIronPlate,
    Coal
};

class Box : public GameObject {
    Rect2d rect;
    double temperature = 0;
    BoxContent content;

public:
    Box(Rect2d aRect): rect(aRect) {}
    void move(Vector2d position);
    void draw();
    Rect2d getRect();
    void setContent(BoxContent aContent);
    BoxContent getContent();
    void update();
    double getTemperature();
    void setTemperature(double aTemperature);
    void addToGameWorld() override;
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_