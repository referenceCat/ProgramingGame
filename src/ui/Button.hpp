#ifndef __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_BUTTON_HPP_
#define __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_BUTTON_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "../common/common.hpp"

class GuiEngine;

class Button {
    Rect2d rect;
    std::function<void()> onClickCallback = nullptr;
    int tintLevel = 0;
    bool hover = 0;

public:
    Button(Rect2d aRect): rect(aRect) {};

    void setTinted(bool value) {
        hover = value;
    }

    void draw();

    void setOnClickCallback(std::function<void()> aCallback) {
        onClickCallback = aCallback;
    }

    void click() {
        if (onClickCallback) onClickCallback();
        tintLevel = 10;
    }

    Rect2d getRect() {
        return rect;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_BUTTON_HPP_