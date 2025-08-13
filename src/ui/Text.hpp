#ifndef __PROJECTS_PROGRAMINGGAME_SRC_UI_TEXT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_UI_TEXT_HPP_

#include <string>
#include "allegro5/allegro5.h"
#include "allegro5/allegro_primitives.h"
#include "allegro5/allegro_font.h"
#include "allegro5/allegro_ttf.h"
#include <functional>
#include "../common/common.hpp"

class GuiEngine;

struct LabelFlags {
    bool highlighted : 1;
};

class Label {
    Rect2d rect;
    bool isCentered;
    std::string text;
    LabelFlags flags;

public:
    Label (Point2d aPos, bool isCentered, std::string text);

    void setFlags(LabelFlags aFlags) {
        flags = aFlags;
    }

    void draw();

    Rect2d getRect() {
        return rect;
    }

    void setText(std::string aText);
};
#endif // __PROJECTS_PROGRAMINGGAME_SRC_UI_TEXT_HPP_