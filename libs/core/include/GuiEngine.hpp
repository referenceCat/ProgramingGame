#ifndef __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_

#include <vector>
#include "Button.hpp"
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "common.hpp"
#include "Window.hpp"
#include "Text.hpp"

class GuiEngine {
    std::vector<Window*> windows;
    GuiEngine() {};

public:
    static GuiEngine* instance() {
        static GuiEngine instance;
        return &instance;
    }

    inline static ALLEGRO_BITMAP* circleIcon = nullptr; // TODO move to separate guiResources class
    inline static ALLEGRO_BITMAP* pauseIcon = nullptr;
    inline static ALLEGRO_BITMAP* unpauseIcon = nullptr;
    inline static ALLEGRO_BITMAP* downIcon = nullptr;
    inline static ALLEGRO_BITMAP* upIcon = nullptr;
    inline static ALLEGRO_BITMAP* nextIcon = nullptr;
    inline static ALLEGRO_BITMAP* emptyIcon = nullptr;
    inline static ALLEGRO_BITMAP* breakpointIcon = nullptr;

    inline static ALLEGRO_FONT* debugFont = nullptr;
    inline static Vector2d drawingIndent{};

    void draw() {
        for (auto item : windows) {
            item->draw();
        }
    };

    void init() {
        createProceduralIcons();
    }

    void createProceduralIcons() {
        ALLEGRO_COLOR blue = al_map_rgb(20, 120, 200);

        circleIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(circleIcon);
        al_draw_filled_circle(7, 7, 7, al_map_rgb(255, 255, 255));

        pauseIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(pauseIcon);
        al_draw_line(4, 0, 4, 14, blue, 3);
        al_draw_line(11, 0, 11, 14, blue, 3);

        unpauseIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(unpauseIcon);
        al_draw_triangle(3, 2, 3, 12, 12, 7, blue, 3);

        downIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(downIcon);
        al_draw_line(3, 4, 7, 11, blue, 3);
        al_draw_line(11, 4, 7, 11, blue, 3);

        upIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(upIcon);
        al_draw_line(3, 9, 7, 2, blue, 3);
        al_draw_line(11, 9, 7, 2, blue, 3);

        nextIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(nextIcon);
        al_draw_triangle(3, 2, 3, 12, 10, 7, blue, 3);
        al_draw_line(13, 0, 13, 14, blue, 3);

        emptyIcon = al_create_bitmap(14, 14);

        breakpointIcon = al_create_bitmap(14, 14);
        al_set_target_bitmap(breakpointIcon);
        al_draw_filled_circle(7, 7, 4, al_map_rgb(200, 0, 0));
    }

    // returns true if clicked on some gui element
    bool click(Vector2d aPos) {
        for (auto item : windows) {
            if (item->getRect().isInside(aPos)) {
                Vector2d relativePos = aPos;
                relativePos.x -= item->getRect().p1.x;
                relativePos.y -= item->getRect().p1.y;
                item->click(relativePos);
                return true;
            }
        }

        return false;
    }

    bool moveMouse(Vector2d aPos) {
        bool result = false;
        for (auto item : windows) {
            if (item->getRect().isInside(aPos)) {
                result = true;
            }
            Vector2d relativePos = aPos;
            relativePos.x -= item->getRect().p1.x;
            relativePos.y -= item->getRect().p1.y;
            item->moveMouse(relativePos);
        }

        return result;
    }

    bool releaseMouse(Vector2d aPos) {
        bool result = false;
        for (auto item : windows) {
            if (item->getRect().isInside(aPos)) {
                Vector2d relativePos = aPos;
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

// enum GuiElementAligment {

// }

class GuiElement {
    Rect2d rect;
    int id;
    inline static int nextId;
    Vector2d relativePos;

    
public:
    void draw() {};
    void click() {};
    void release() {};
    
};

#endif // __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_GUIENGINE_HPP_