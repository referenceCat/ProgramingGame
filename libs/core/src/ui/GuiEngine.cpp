#include "GuiEngine.hpp"
#include <algorithm>
#include <chrono>

void Label::draw() {
    auto pos = rect.center() - Vector2d(al_get_text_width(GuiEngine::debugFont, text.c_str()) / 2, al_get_font_line_height(GuiEngine::debugFont) / 2);
    al_draw_text(GuiEngine::debugFont, al_map_rgb(255, 255, 255), pos.x + 0.1, pos.y + 0.1, 0, text.c_str());
}

void Icon::draw() {
    auto pos = rect.center() - Vector2d(static_cast<double>(al_get_bitmap_width(bitmap)) / 2, static_cast<double>(al_get_bitmap_height(bitmap)) / 2);
    al_draw_bitmap(bitmap, pos.x, pos.y, 0);
}

Window::Window(GuiElement* parent, Aligment aligment, bool closable):
    GuiElement(parent, aligment) {

    if (closable) {
        Aligment quitButtonAligment;
        quitButtonAligment.marginLeft = 4;
        quitButtonAligment.marginTop = 4;
        quitButtonAligment.dimensions = Vector2d(22, 22);
        quitButton = new Button(this, quitButtonAligment);
        quitButton->setMouseCallback(Click, [this](Vector2d pos) { delete this; });
        auto closeIcon = new Icon(quitButton, Aligment::byDimensionsAndCentered(Vector2d(20, 20)), GuiEngine::instance()->closeWindowIcon);
    }

    Aligment areaAligment;
    areaAligment.marginLeft = 4;
    areaAligment.marginRight = 4;
    areaAligment.marginTop = closable ? 30 : 4;
    areaAligment.marginBottom = 4;
    area = new GuiElement(this, areaAligment);
    drawPriority = 1;
}

void NamedArea::draw() {
    auto color = al_map_rgb(150, 150, 150);
    al_draw_line(rect.p1.x, rect.p1.y + 5, rect.p1.x + 18, rect.p1.y + 5, color, 2);
    al_draw_line(rect.p1.x + 20 + 2 + al_get_text_width(GuiEngine::instance()->debugFont, text.c_str()), rect.p1.y + 5, rect.p2.x, rect.p1.y + 5, color, 2);
    al_draw_line(rect.p2.x, rect.p1.y + 5, rect.p2.x, rect.p2.y, color, 2);
    al_draw_line(rect.p1.x, rect.p2.y, rect.p2.x, rect.p2.y, color, 2);
    al_draw_line(rect.p1.x, rect.p1.y + 5, rect.p1.x, rect.p2.y, color, 2);
    al_draw_text(GuiEngine::instance()->debugFont, color, rect.p1.x + 20, rect.p1.y, 0, text.c_str());
}

Console::Console(GuiElement* parent, Aligment aligment):
    GuiElement(parent, aligment) {
    this->setMouseCallback(WheelMoveDown, [this](auto pos) { scrollLines(1); }); // TODO move to handle mouse event
    this->setMouseCallback(WheelMoveUp, [this](auto pos) { scrollLines(-1); });
    this->setMouseCallback(Click, [this](auto pos) { moveCursor(pos); });

    GuiEngine::instance()->setKeyboardInputHandler(this);

    auto duration = std::chrono::system_clock::now().time_since_epoch(); // TODO do it normaly
    lastTimeCursorMovedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void Console::moveCursor(Vector2d mousePos) {
    moveCursor((mousePos.y - getRect().p1.y - 10) / 15 + lineFrom, (mousePos.x - getRect().p1.x - 50 + 2) / al_get_text_width(GuiEngine::instance()->debugFont, "a"));
}

void Console::draw() {
    al_draw_filled_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(10, 10, 10));
    al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(200, 200, 200), 2);

    linesMax = (getRect().dimensions().y - 10) / 15;
    for (int i = lineFrom; i < (lineFrom + linesMax < lines.size() ? lineFrom + linesMax : lines.size()); i++) {
        al_draw_text(GuiEngine::instance()->debugFont, al_map_rgb(100, 100, 100), rect.p1.x + 10, rect.p1.y + 10.1 + (i - lineFrom) * 15, 0, std::to_string(i).c_str());
        al_draw_text(GuiEngine::instance()->debugFont, al_map_rgb(255, 255, 255), rect.p1.x + 50, rect.p1.y + 10.1 + (i - lineFrom) * 15, 0, lines.at(i).c_str());
    }

    auto duration = std::chrono::system_clock::now().time_since_epoch(); // TODO do it normaly
    long long millisNow = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    if (cursorLine >= lineFrom && cursorLine < lineFrom + linesMax && ((millisNow % 1000 < 500) || (millisNow - lastTimeCursorMovedMillis) < 500)) {
        double x = 50 + rect.p1.x + cursorColumn * al_get_text_width(GuiEngine::instance()->debugFont, "a");
        al_draw_line(x, rect.p1.y + 8 + (cursorLine - lineFrom) * 15, x, rect.p1.y + 22 + (cursorLine - lineFrom) * 15, al_map_rgb(255, 255, 255), 2);
    }
}