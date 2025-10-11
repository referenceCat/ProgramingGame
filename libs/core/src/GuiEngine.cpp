#include "GuiEngine.hpp"
#include <algorithm>
#include <chrono>

void Label::draw() {
    auto pos = rect.center() - Vector2d(al_get_text_width(GuiEngine::debugFont, text.c_str()) / 2, al_get_font_line_height(GuiEngine::debugFont) / 2);
    al_draw_text(GuiEngine::debugFont, al_map_rgb(255, 255, 255), pos.x + 0.1, pos.y + 0.1, 0, text.c_str());
}

void Icon::setBitmap(ALLEGRO_BITMAP* bitmap) {
    this->bitmap = bitmap;
}

void Icon::draw() {
    if (bitmap == nullptr)
        return;
    auto pos = rect.center() - Vector2d(static_cast<double>(al_get_bitmap_width(bitmap)) / 2, static_cast<double>(al_get_bitmap_height(bitmap)) / 2);
    al_draw_bitmap(bitmap, pos.x + 0.1, pos.y + 0.1, 0);
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
        auto closeIcon = new Icon(quitButton, Aligment::byDimensionsAndCentered(Vector2d(20, 20)), GuiEngine::instance()->getIcon("close"));
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
    this->setMouseCallback(Click, [this](auto pos) { if(GuiEngine::instance()->getKeyboardInputHandler() != this ) {GuiEngine::instance()->setKeyboardInputHandler(this);} moveCursor(pos); clearSelection(); });
    this->setMouseCallback(Hold, [this](auto pos) { if (selectionColumn == -1 || selectionLine == -1) {setSelection(cursorLine, cursorColumn);} moveCursor(pos);});

    // GuiEngine::instance()->setKeyboardInputHandler(this);

    auto duration = std::chrono::system_clock::now().time_since_epoch(); // TODO do it normaly
    lastTimeCursorMovedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    lines.push_back(""); // must be at least line
}

void Console::moveCursor(Vector2d mousePos) {
    moveCursor((mousePos.y - getRect().p1.y - 10) / 15 + lineFrom, (mousePos.x - getRect().p1.x - 50 + 2) / al_get_text_width(GuiEngine::instance()->debugFont, "a"));
}

void Console::clearSelection() {
    selectionLine = -1;
    selectionColumn = -1;
}

void Console::setSelection(Vector2d mousePos) {
    setSelection((mousePos.y - getRect().p1.y - 10) / 15 + lineFrom, (mousePos.x - getRect().p1.x - 50 + 2) / al_get_text_width(GuiEngine::instance()->debugFont, "a"));
}

void Console::setSelection(int line, int column) {
    selectionLine = line;
    selectionColumn = column;

    if (selectionLine < 0)
        selectionLine = 0;
    if (selectionColumn < 0)
        selectionColumn = 0;
    if (selectionLine >= lines.size())
        selectionLine = lines.size() - 1;
    if (selectionColumn > lines.at(selectionLine).size())
        selectionColumn = lines.at(selectionLine).size();
}

void Console::draw() {
    al_draw_filled_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(10, 10, 10));

    linesMax = (getRect().dimensions().y - 10) / 15;

    // draw highlighted line background
    if (highlightedLine >= lineFrom && highlightedLine < lineFrom + linesMax) {
        al_draw_filled_rectangle(rect.p1.x, rect.p1.y + 8 + (highlightedLine - lineFrom) * 15, rect.p2.x, rect.p1.y + 22 + (highlightedLine - lineFrom) * 15, al_map_rgb(40, 40, 40));
    }

    // draw selection
    if (selectionLine != -1 && selectionColumn != -1) {
        int selectionColumnFrom = cursorColumn;
        int selectionColumnTo = selectionColumn;
        int selectionLineFrom = cursorLine;
        int selectionLineTo = selectionLine;

        if (selectionLine < cursorLine || (selectionLine == cursorLine && selectionColumn < cursorColumn)) {
            selectionColumnFrom = selectionColumn;
            selectionColumnTo = cursorColumn;
            selectionLineFrom = selectionLine;
            selectionLineTo = cursorLine;
        }

        for (int i = selectionLineFrom; i <= selectionLineTo; i++) {
            if (i < lineFrom) continue; // skip lines outside windows
            if (i >= lineFrom + linesMax) continue; // skip lines outside windows
            al_draw_filled_rectangle(50 + rect.p1.x + (i == selectionLineFrom ? selectionColumnFrom * al_get_text_width(GuiEngine::instance()->debugFont, "a") : 0),
                rect.p1.y + 8 + (i - lineFrom) * 15,
                i == selectionLineTo ? 50 + rect.p1.x + selectionColumnTo * al_get_text_width(GuiEngine::instance()->debugFont, "a") : rect.p2.x,
                rect.p1.y + 22 + (i - lineFrom) * 15, al_map_rgb(0, 150, 200));
        }
    }
    // draw lines
    for (int i = lineFrom; i < (lineFrom + linesMax < lines.size() ? lineFrom + linesMax : lines.size()); i++) {
        al_draw_text(GuiEngine::instance()->debugFont, al_map_rgb(100, 100, 100), rect.p1.x + 10, rect.p1.y + 10.1 + (i - lineFrom) * 15, 0, std::to_string(i).c_str());
        int maxStringLength = (rect.dimensions().x - 50) / al_get_text_width(GuiEngine::instance()->debugFont, "a");
        auto displayedLine = (char*)malloc(maxStringLength + 1);
        strncpy(displayedLine, lines.at(i).c_str(), maxStringLength);
        displayedLine[maxStringLength] = 0;
        al_draw_text(GuiEngine::instance()->debugFont, al_map_rgb(255, 255, 255), rect.p1.x + 50, rect.p1.y + 10.1 + (i - lineFrom) * 15, 0, displayedLine);
    }

    // draw cursor
    auto duration = std::chrono::system_clock::now().time_since_epoch(); // TODO do it normaly
    long long millisNow = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
    if (cursorLine >= lineFrom && cursorLine < lineFrom + linesMax && ((millisNow % 1000 < 500) || (millisNow - lastTimeCursorMovedMillis) < 500) && GuiEngine::instance()->getKeyboardInputHandler() == this && editable) {
        double x = 50 + rect.p1.x + cursorColumn * al_get_text_width(GuiEngine::instance()->debugFont, "a");
        if (x < rect.p2.x)
            al_draw_line(x, rect.p1.y + 8 + (cursorLine - lineFrom) * 15, x, rect.p1.y + 22 + (cursorLine - lineFrom) * 15, al_map_rgb(255, 255, 255), 2); // check if cursor is outiside of console
    }

    al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(200, 200, 200), 2);
}

GuiElement::~GuiElement() {
    if (onCloseCallback)
        onCloseCallback();

    for (auto child : children) {
        child->parent = nullptr;
        delete child;
    }
    children.clear();
    if (parent != nullptr)
        parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), this), parent->children.end());

    GuiEngine::instance()->clearKeyboardInputHandler();
}