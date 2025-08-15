#ifndef __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include  <algorithm>
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
    Box(Rect2d aRect, GameWorld* aWorld): rect(aRect), GameObject(aWorld) {}

    void move(Point2d position) {
        Vector2d dPosition(position, rect.center());
        rect.p1 = rect.p1 + dPosition;
        rect.p2 = rect.p2 + dPosition;
    }

    void draw() {
        int r = (temperature < -255) ? 0 : ((temperature > 0) ? 255 : (temperature + 255));
        int g = (temperature > 255) ? 0 : ((temperature < 0) ? 255 : (-temperature + 255));
        int b = (temperature < -255 || temperature > 255) ? 0 : ((temperature >= 0) ? (-temperature + 255) : (temperature + 255));
        ALLEGRO_COLOR color = al_map_rgb(r, g, b);
        
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, color, 1);

        if (content == BoxContent::IronPlate) {
            al_draw_filled_rectangle(rect.p1.x + 2, rect.p1.y + 2, rect.p1.x + 6, rect.p2.y - 2, color);
        } else if (content == BoxContent::HeavyIronPlate) {
            al_draw_filled_rectangle(rect.p1.x + 2, rect.p1.y + 2, rect.p1.x + 6, rect.p2.y - 2, color);
            al_draw_filled_rectangle(rect.p1.x + 8, rect.p1.y + 2, rect.p1.x + 12, rect.p2.y - 2, color);
        } else if (content == BoxContent::VeryHeavyIronPlate) {
            al_draw_filled_rectangle(rect.p1.x + 2, rect.p1.y + 2, rect.p1.x + 6, rect.p2.y - 2, color);
            al_draw_filled_rectangle(rect.p1.x + 8, rect.p1.y + 2, rect.p1.x + 12, rect.p2.y - 2, color);
            al_draw_filled_rectangle(rect.p1.x + 14, rect.p1.y + 2, rect.p1.x + 18, rect.p2.y - 2, color);
        } else if (content == BoxContent::Coal) {
            al_draw_filled_rectangle(rect.p1.x + 2, rect.p1.y + 2, rect.p2.x - 2, rect.p2.y - 2, color);
        }    
    }

    Rect2d getRect() {
        return rect;
    }

    void grill() {
        if(temperature < 255) temperature++;
    }

    void setContent(BoxContent aContent) {
        content = aContent;
    }

    BoxContent getContent() {
        return content;
    }

    void update() {
        temperature -= temperature * 0.01;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_