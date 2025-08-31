#ifndef __PROJECTS_PROGRAMINGGAME_SRC_UI_ICON_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_UI_ICON_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "../common/common.hpp"

class GuiEngine;

class Icon {
    Rect2d rect;
    ALLEGRO_BITMAP* bitmap = nullptr;

public:
    Icon (Vector2d aPos, ALLEGRO_BITMAP* aBitmap): bitmap(aBitmap) {
        rect = Rect2d(aPos, al_get_bitmap_width(aBitmap), al_get_bitmap_width(aBitmap));
    };

    void draw();

    Rect2d getRect() {
        return rect;
    }

    void setBitmap(ALLEGRO_BITMAP* aBitmap) {
        bitmap = aBitmap;
        rect = Rect2d(rect.center(), al_get_bitmap_width(aBitmap), al_get_bitmap_width(aBitmap));
    };
};
#endif // __PROJECTS_PROGRAMINGGAME_SRC_UI_ICON_HPP_