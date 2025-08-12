#ifndef __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_BUTTON_HPP_
#define __PROJECTS_PROGRAMINGGAME_ALLEGROGUI_SRC_BUTTON_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "../common/common.hpp"

class Button {
    Rect2d rect;
    std::string text;
    std::function<void()> onClickCallback = nullptr;
    int tintLevel = 0;
    bool hover = 0;

public:
    inline static ALLEGRO_FONT* debug_font = nullptr;
    Button(Rect2d aRect, std::string text): rect(aRect), text(text) {};
    void setText(std::string aText) {
        text = aText;
    }

    void setTinted(bool value) {
        hover = value;
    }

    void draw() {
        ALLEGRO_COLOR color = al_map_rgb(255 - tintLevel * 15, 255 - tintLevel * 15, 255 - tintLevel * 15);
        if (hover && tintLevel <= 5) {
            tintLevel = 5;
        } else if (tintLevel > 0) {
            tintLevel--;
        }

        al_draw_filled_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, color);
        al_draw_text(debug_font, al_map_rgb(0, 0, 0), rect.center().x - al_get_text_width(debug_font, text.c_str()) / 2, rect.center().y - al_get_font_line_height(debug_font) / 2, 0, text.c_str());
    }

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