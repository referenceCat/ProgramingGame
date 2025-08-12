#ifndef __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_

#include <vector>
#include "Button.hpp"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "../common/common.hpp"
#include "Window.hpp"

class GuiEngine {
    std::vector<Window*> windows;

public:
    inline static ALLEGRO_FONT* debug_font = nullptr;
    inline static Vector2d drawingIndent{};
    
    GuiEngine() {};

    void draw() {
        for (auto item: windows) {
            item->draw();
        }
    };

    // returns true if clicked on some gui element
    bool click(Point2d aPos) {
        for (auto item: windows) {
            if (item->getRect().isInside(aPos)) {
                Point2d relativePos = aPos;
                relativePos.x -= item->getRect().p1.x;
                relativePos.y -= item->getRect().p1.y;
                item->click(relativePos);
                return true;
            }
        }

        return false;
    }

    bool moveMouse(Point2d aPos)  {
        for (auto item: windows) {
            if (item->getRect().isInside(aPos)) {
                Point2d relativePos = aPos;
                relativePos.x -= item->getRect().p1.x;
                relativePos.y -= item->getRect().p1.y;
                item->moveMouse(relativePos);
                return true;
            }
        }

        return false;
    }

    void addWindow(Rect2d aRect, bool movable, bool closable) {
        windows.push_back(new Window(aRect, movable, closable));
    };
};


#endif // __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_