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
#include "Text.hpp"
#include "Icon.hpp"

class GuiEngine;

class Window { // TODO add close event handler + write callbacks for each thing, what creates windows
    Rect2d rect;
    int z = 0;

    // Vector2d indent{8, 8};
    inline static ALLEGRO_COLOR backgroundColor = al_map_rgb(30, 30, 30);
    inline static ALLEGRO_COLOR primaryColor = al_map_rgb(200, 200, 200);

    bool closable;
    bool movable;
    bool moving = false;
    Rect2d dragArea{};
    Point2d mouseDragPos{};

    std::vector<Button*> buttons;
    std::vector<Label*> labels;
    std::vector<Icon*> icons;

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

    Button* addButton(Rect2d aRect) {
        buttons.push_back(new Button(aRect));
        return buttons.back();
    };

    Label* addLabel(Point2d aPos, bool centered, std::string text, int line = 0) {
        aPos.y += line * 14;
        labels.push_back(new Label(aPos, centered, text));
        return labels.back();
    };

    Icon* addIcon(Point2d aPos, ALLEGRO_BITMAP* bitmap) {
        icons.push_back(new Icon(aPos, bitmap));
        return icons.back();
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