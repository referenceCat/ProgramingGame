#ifndef __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../common/common.hpp"

class Box {
    Rect2d rect;

public:
    Box(Rect2d aRect): rect(aRect) {}

    void move(Point2d position) {
        Vector2d dPosition(position, rect.center());
        rect.p1 = rect.p1 + dPosition;
        rect.p2 = rect.p2 + dPosition;
    }

    void draw() {
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(255, 255, 255), 1);
    }

    Rect2d getRect() {
        return rect;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_