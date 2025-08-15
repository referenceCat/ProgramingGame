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
        
        GraphicsEngine::instance()->draw_rectangle(rect, 0, color, 1);

        if (content == BoxContent::IronPlate) {
            GraphicsEngine::instance()->draw_line(rect.p1 + Point2d(4, 2), rect.p1 + Point2d(4, 10), 0, color, 3);
        } else if (content == BoxContent::HeavyIronPlate) {
            GraphicsEngine::instance()->draw_line(rect.p1 + Point2d(4, 2), rect.p1 + Point2d(4, 10), 0, color, 3);
            GraphicsEngine::instance()->draw_line(rect.p1 + Point2d(8, 2), rect.p1 + Point2d(8, 10), 0, color, 3);
        } else if (content == BoxContent::VeryHeavyIronPlate) {
            GraphicsEngine::instance()->draw_line(rect.p1 + Point2d(4, 2), rect.p1 + Point2d(4, 10), 0, color, 3);
            GraphicsEngine::instance()->draw_line(rect.p1 + Point2d(8, 2), rect.p1 + Point2d(8, 10), 0, color, 3);
            GraphicsEngine::instance()->draw_line(rect.p1 + Point2d(12, 2), rect.p1 + Point2d(12, 10), 0, color, 3);
        } else if (content == BoxContent::Coal) {
            GraphicsEngine::instance()->draw_rectangle(rect, 0, color);
        }    
    }

    Rect2d getRect() {
        return rect;
    }

    void setContent(BoxContent aContent) {
        content = aContent;
    }

    BoxContent getContent() {
        return content;
    }

    void update() {
        if (temperature > 0)
        temperature -= 0.1;
    }

    double getTemperature() {
        return temperature;
    }

    void setTemperature(double aTemperature) {
        temperature = aTemperature;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_