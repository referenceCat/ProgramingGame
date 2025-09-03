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
    Box(Rect2d aRect): rect(aRect) {}

    void move(Vector2d position) {
        Vector2d dPosition = rect.center() - position;
        rect.p1 = rect.p1 + dPosition;
        rect.p2 = rect.p2 + dPosition;
    }

    void draw() {
        double z = 1.5;
        int r = (temperature < -255) ? 0 : ((temperature > 0) ? 255 : (temperature + 255));
        int g = (temperature > 255) ? 0 : ((temperature < 0) ? 255 : (-temperature + 255));
        int b = (temperature < -255 || temperature > 255) ? 0 : ((temperature >= 0) ? (-temperature + 255) : (temperature + 255));
        ALLEGRO_COLOR color = al_map_rgb(r, g, b);
        
        GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zBox, color);

        // if (content == BoxContent::IronPlate) {
        //     GraphicsEngine::instance()->drawLine(rect.p1 + Vector2d(4, 2), rect.p1 + Vector2d(4, 10), z, al_map_rgb(0, 0, 0), 3);
        // } else if (content == BoxContent::HeavyIronPlate) {
        //     GraphicsEngine::instance()->drawLine(rect.p1 + Vector2d(4, 2), rect.p1 + Vector2d(4, 10), z, al_map_rgb(0, 0, 0), 3);
        //     GraphicsEngine::instance()->drawLine(rect.p1 + Vector2d(8, 2), rect.p1 + Vector2d(8, 10), z, al_map_rgb(0, 0, 0), 3);
        // } else if (content == BoxContent::VeryHeavyIronPlate) {
        //     GraphicsEngine::instance()->drawLine(rect.p1 + Vector2d(4, 2), rect.p1 + Vector2d(4, 10), z, al_map_rgb(0, 0, 0), 3);
        //     GraphicsEngine::instance()->drawLine(rect.p1 + Vector2d(8, 2), rect.p1 + Vector2d(8, 10), z, al_map_rgb(0, 0, 0), 3);
        //     GraphicsEngine::instance()->drawLine(rect.p1 + Vector2d(12, 2), rect.p1 + Vector2d(12, 10), z, al_map_rgb(0, 0, 0), 3);
        // } else if (content == BoxContent::Coal) {
        //     GraphicsEngine::instance()->drawRectangle(rect, z, al_map_rgb(0, 0, 0));
        // }    
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
    
    void addToGameWorld() override;
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_BOX_HPP_