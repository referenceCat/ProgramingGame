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
    static GuiEngine* instance;
    GuiEngine() {};

public:
    inline static ALLEGRO_FONT* debug_font = nullptr;
    inline static Vector2d drawingIndent{};
    
    static GuiEngine* getInstance();

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
        bool result = false;
        for (auto item: windows) {
            if (item->getRect().isInside(aPos)) {
                result = true;
            }
            Point2d relativePos = aPos;
            relativePos.x -= item->getRect().p1.x;
            relativePos.y -= item->getRect().p1.y;
            item->moveMouse(relativePos);
        }

        return result;
    }

    bool releaseMouse(Point2d aPos) {
        bool result = false;
        for (auto item: windows) {
            if (item->getRect().isInside(aPos)) {
                Point2d relativePos = aPos;
                relativePos.x -= item->getRect().p1.x;
                relativePos.y -= item->getRect().p1.y;
                item->releaseMouse(relativePos);
                result = true;
            }
        }

        return result;
    }

    Window* addWindow(Rect2d aRect, bool movable, bool closable) {
        windows.push_back(new Window(aRect, movable, closable));
        return windows.back();
    };

    void closeWindow(Window* window) {
        delete window;
        windows.erase(std::remove(windows.begin(), windows.end(), window), windows.end());
    }
};


#endif // __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_