#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINE_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <vector>
#include "../common/common.hpp"
#include "ManipulatorArm.hpp"
#include "Box.hpp"
#include "GameObject.hpp"

class Machine : public GameObject {
    Rect2d rect;

public:
    Machine(Point2d aPos, GameWorld* aWorld): rect(aPos, 50, 50), GameObject(aWorld) {}

    void draw() {
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(100, 255, 100), 1);
    }

    Rect2d getRect() {
        return rect;
    }

    void createBox();
    void destroyBoxes();
    void grillBoxes();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINE_HPP_