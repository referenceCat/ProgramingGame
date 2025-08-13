#ifndef __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "../common/common.hpp"
#include "Button.hpp"
#include <memory>
#include <vector>

class GuiEngine;

class Window {
    Rect2d rect;
    int z = 0;

    Vector2d indent{8, 8};
    inline static ALLEGRO_COLOR backgroundColor = al_map_rgb(30, 30, 30);
    inline static ALLEGRO_COLOR primaryColor = al_map_rgb(200, 200, 200);

    bool movable;
    bool moving = false;
    Rect2d dragArea{};
    Point2d mouseDragPos{};

    bool closable;
    std::vector<Button*> buttons;

public:
    Window(Rect2d rect, bool movable, bool closable);

    void draw();

    // returns true if clicked on some gui element
    bool click(Point2d aPos) {
        if (movable && dragArea.isInside(aPos)) {
            moving = true;
            mouseDragPos = aPos;
            return true;
        }

        for (auto item: buttons) {
            if (item->getRect().isInside(aPos)) {
                item->click();
                return true;
            }
        }

        return false;
    }

    void releaseMouse(Point2d aPos) {
        moving = false;
    }

    bool moveMouse(Point2d aPos) {
        if (moving) {
            Vector2d movement = Vector2d(aPos, mouseDragPos);
            rect.p1 = rect.p1 + movement;
            rect.p2= rect.p2 + movement;
            return true;
        }

        for (auto item: buttons) {
            item->setTinted(false);
        }

        for (auto item: buttons) {
            if (item->getRect().isInside(aPos)) {
                item->setTinted(true);
                return true;
            }
        }

        return false;
    }

    void addButton(Rect2d aRect) {
        buttons.push_back(new Button(aRect));
    };

    Rect2d getRect() {
        return rect;
    }

    ~Window() {
        for (auto item: buttons) {
            delete item;
        }
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_