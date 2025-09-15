#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_

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

class Machinery : public GameObject {
protected:
    Rect2d rect;

    struct ProductionArea {
        Rect2d rect;
    };

    std::vector<ProductionArea*>  areas;

    void destroyBox(Box* box);
    Box* createBox(ProductionArea area);
    std::vector<Box*> getBoxesInside(ProductionArea area);
    std::vector<Box*> getBoxesTouching(ProductionArea area);

public:
    Machinery(Rect2d rect): rect(rect) {};

    virtual void drawInfo() {}

    virtual void drawDebug() {
        GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zDebug,  al_map_rgb(100, 255, 100), 1);

        for (auto item: areas) {
            Rect2d areaRect;
            areaRect.p1 = item->rect.p1 + rect.p1;
            areaRect.p2 = item->rect.p2 + rect.p1;
            GraphicsEngine::instance()->drawRectangle(areaRect, CommonValues::zDebug, al_map_rgb(100, 100, 255), 1);
        }
    }

    virtual void draw() {}

    Rect2d getRect() {
        return rect;
    }

    virtual void run() {};
    virtual void onCommandRecive(int command, int argument) {}
    virtual void onClick() {};
    void addToGameWorld() override;
    void setCenter(Vector2d pos) {
        rect = Rect2d::fromCenterAndDimensions(pos, rect.dimensions());
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_