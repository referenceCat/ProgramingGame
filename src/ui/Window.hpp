#ifndef __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "../common/common.hpp"

class Window {
    Rect2d rect;
    bool movable;
    bool closable;
    inline static ALLEGRO_COLOR backgroundColor = al_map_rgb(30, 30, 30);
    inline static ALLEGRO_COLOR primaryColor = al_map_rgb(200, 200, 200);

public:
    Window(Rect2d rect, bool movable = false, bool closable = false): rect(rect), movable(movable), closable(closable) {};

    void draw() {
        al_draw_filled_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, backgroundColor);
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, primaryColor, 2);
        if (movable || closable) {
            al_draw_rectangle(rect.p1.x + 4, rect.p1.y + 20, rect.p2.x - 4, rect.p2.y - 4, primaryColor, 2);
        } else {
            al_draw_rectangle(rect.p1.x + 4, rect.p1.y + 4, rect.p2.x - 4, rect.p2.y - 4, primaryColor, 2);
        }
        
        if (closable) {
            al_draw_line(rect.p1.x + 6, rect.p1.y + 6, rect.p1.x + 16, rect.p1.y + 16, primaryColor, 2);
            al_draw_line(rect.p1.x + 6, rect.p1.y + 16, rect.p1.x + 16, rect.p1.y + 6, primaryColor, 2);
        }

        if (movable) {
            al_draw_filled_rectangle(rect.p1.x + 20, rect.p1.y + 4, rect.p1.x + 70, rect.p1.y + 16, primaryColor);
        }
        
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_UI_WINDOW_HPP_