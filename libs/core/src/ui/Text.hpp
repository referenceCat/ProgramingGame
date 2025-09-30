#ifndef __PROJECTS_PROGRAMINGGAME_SRC_UI_TEXT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_UI_TEXT_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "common.hpp"

class GuiEngine;

struct LabelFlags {
    bool background : 1;
};

class LegacyLabel {
    Rect2d rect;
    bool isCentered;
    std::string text;
    LabelFlags flags;
    ALLEGRO_COLOR backgroundColor = al_map_rgb(100, 100, 100);

public:
    LegacyLabel(Vector2d aPos, bool isCentered, std::string text);
    void setFlags(LabelFlags aFlags);
    void draw();
    Rect2d getRect();
    void setText(std::string aText);
    void setBackgroundColor(ALLEGRO_COLOR aColor);
};
#endif // __PROJECTS_PROGRAMINGGAME_SRC_UI_TEXT_HPP_