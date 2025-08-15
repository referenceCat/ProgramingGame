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
protected:
    Rect2d rect;
    int counter = 0;

    struct ProductionArea {
        Rect2d rect;
    };

    std::vector<ProductionArea*>  areas;

    void destroyBox(Box* box);
    void createBox();
    std::vector<Box*> getBoxesInside(ProductionArea area);
    std::vector<Box*> getBoxesTouching(ProductionArea area);

public:
    Machine(Rect2d rect, GameWorld* aWorld): rect(rect), GameObject(aWorld) {
        // aWorld->addMachine(this); // TODO not sure
    }

    virtual void draw() {
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(100, 255, 100), 1);

        for (auto item: areas) {
           al_draw_rectangle(item->rect.p1.x + rect.p1.x, item->rect.p1.y + rect.p1.y, item->rect.p2.x + rect.p1.x, item->rect.p2.y + rect.p1.y, al_map_rgb(100, 100, 255), 1); 
        }
    }

    Rect2d getRect() {
        return rect;
    }

    virtual void run() = 0;
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINE_HPP_