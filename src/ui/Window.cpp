#include "Window.hpp"
#include "GuiEngine.hpp"

Window::Window(Rect2d rect, bool movable = false, bool closable = false): rect(rect), movable(movable), closable(closable) {
    if (movable) {
        dragArea = Rect2d(Point2d(20, 4), Point2d(70, 16));
    }
    if (closable) {
        buttons.push_back(new Button(Rect2d(Point2d(10, 10), 14, 14)));
        buttons.back()->setOnClickCallback([=]() {GuiEngine::getInstance()->closeWindow(this);});
    }
    if (closable || movable) {
        // indent.y = 24;
    }
};

void Window::draw() {
    al_draw_filled_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, backgroundColor);
    al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, primaryColor, 2);
    if (movable || closable) {
        al_draw_rectangle(rect.p1.x + 4, rect.p1.y + 20, rect.p2.x - 4, rect.p2.y - 4, primaryColor, 2);
    } else {
        al_draw_rectangle(rect.p1.x + 4, rect.p1.y + 4, rect.p2.x - 4, rect.p2.y - 4, primaryColor, 2);
    }

    if (movable) {
        al_draw_filled_rectangle(rect.p1.x + dragArea.p1.x, rect.p1.y + dragArea.p1.y, rect.p1.x + dragArea.p2.x, rect.p1.y + dragArea.p2.y, primaryColor);
    }

    GuiEngine::drawingIndent.x += rect.p1.x;
    GuiEngine::drawingIndent.y += rect.p1.y;

    // draw every child here

    for (auto item: buttons) {
        item->draw();
    }

    for (auto item: labels) {
        item->draw();
    }

    GuiEngine::drawingIndent.x -= rect.p1.x;
    GuiEngine::drawingIndent.y -= rect.p1.y;

    if (closable) {
        al_draw_line(rect.p1.x + 5, rect.p1.y + 5, rect.p1.x + 16, rect.p1.y + 16, primaryColor, 2);
        al_draw_line(rect.p1.x + 5, rect.p1.y + 16, rect.p1.x + 16, rect.p1.y + 5, primaryColor, 2);
    }
};