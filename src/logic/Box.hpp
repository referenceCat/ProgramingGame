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

class Box : public GameObject{
    Rect2d rect;
    int temperature = 0;

public:
    Box(Rect2d aRect, GameWorld* aWorld): rect(aRect), GameObject(aWorld) {}

    void move(Point2d position) {
        Vector2d dPosition(position, rect.center());
        rect.p1 = rect.p1 + dPosition;
        rect.p2 = rect.p2 + dPosition;
    }

    void draw() {
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(255, 255 - temperature, 255 - temperature), 1);
    }

    Rect2d getRect() {
        return rect;
    }

    void grill() {
        if(temperature < 255) temperature++;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_