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
    bool movable;
    bool closable;
    Vector2d indent{8, 8};
    inline static ALLEGRO_COLOR backgroundColor = al_map_rgb(30, 30, 30);
    inline static ALLEGRO_COLOR primaryColor = al_map_rgb(200, 200, 200);

    Rect2d dragArea = Rect2d();
    std::vector<Button*> buttons;

public:
    Window(Rect2d rect, bool movable, bool closable);

    void draw();

    // returns true if clicked on some gui element
    bool click(Point2d aPos) {
        for (auto item: buttons) {
            if (item->getRect().isInside(aPos)) {
                item->click();
                return true;
            }
        }

        return false;
    }

    bool moveMouse(Point2d aPos) {
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