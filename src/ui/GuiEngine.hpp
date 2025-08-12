#ifndef __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_

#include <vector>
#include "Button.hpp"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "../common/common.hpp"
#include "Window.hpp"

class GuiEngine {
    std::vector<Button*> buttons;
    std::vector<Window*> windows;
    inline static ALLEGRO_FONT* debug_font = nullptr;

public:
    GuiEngine() {};

    void draw() {
        for (auto item: buttons) {
            item->draw();
        }

        for (auto item: windows) {
            item->draw();
        }
    };

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

    bool moveMouse(Point2d aPos)  {
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

    void addWindow(Rect2d aRect, std::string text, bool movable, bool closable) {
        windows.push_back(new Window(aRect, text, movable, closable));
    };
};


#endif // __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_