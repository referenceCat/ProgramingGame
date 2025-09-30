#ifndef __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "common.hpp"
#include "Button.hpp"
#include <memory>
#include <vector>
#include "Text.hpp"
#include "Icon.hpp"

class GuiEngine;

class LegacyWindow { // TODO add close event handler + write callbacks for each thing, what creates windows
    Rect2d rect;
    int z = 0;

    inline static ALLEGRO_COLOR backgroundColor = al_map_rgb(30, 30, 30);
    inline static ALLEGRO_COLOR primaryColor = al_map_rgb(200, 200, 200);

    bool closable;
    bool movable;
    bool moving = false;
    Rect2d dragArea{};
    Vector2d mouseDragPos{};

    std::vector<LegacyButton*> buttons;
    std::vector<LegacyLabel*> labels;
    std::vector<LegacyIcon*> icons;
    std::function<void()> onCloseCallback = nullptr;

public:
    LegacyWindow(Rect2d rect, bool movable, bool closable);
    void draw();
    void setOnCloseCallback(std::function<void()> aCallback);
    // returns true if clicked on some gui element
    bool click(Vector2d aPos);
    void releaseMouse(Vector2d aPos);
    bool moveMouse(Vector2d aPos);
    LegacyButton* addButton(Rect2d aRect);
    void deleteButton(LegacyButton* buttonToRemove);
    void deleteIcon(LegacyIcon* iconToRemove);
    void deleteLabel(LegacyLabel* labelToRemove);
    LegacyLabel* addLabel(Vector2d aPos, bool centered, std::string text, int line = 0);
    LegacyIcon* addIcon(Vector2d aPos, ALLEGRO_BITMAP* bitmap);
    Rect2d getRect();
    ~LegacyWindow();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_