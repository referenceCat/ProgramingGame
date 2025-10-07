#ifndef __PROJECTS_PROGRAMINGGAME_LIBS_CORE_INCLUDE_GUIENGINE_HPP_
#define __PROJECTS_PROGRAMINGGAME_LIBS_CORE_INCLUDE_GUIENGINE_HPP_

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <chrono>
#include <functional>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include "common.hpp"

class GuiEngine;

struct Aligment {
    Vector2d dimensions;
    double marginLeft = -1;
    double marginRight = -1;
    double marginTop = -1;
    double marginBottom = -1;
    unsigned int tableColumns = 1;
    unsigned int tableRows = 1;
    unsigned int ownColumn = 0;
    unsigned int ownRow = 0;

    static Aligment byDimensionsAndCentered(Vector2d aDimensions) {
        Aligment res;
        res.dimensions = aDimensions;
        return res;
    }

    static Aligment byMargin(double left, double top, double right, double bottom) {
        Aligment res;
        res.marginLeft = left;
        res.marginRight = right;
        res.marginTop = top;
        res.marginBottom = bottom;
        return res;
    }
};

class AligmentBuilder {
    Aligment aligment;

public:
    AligmentBuilder() {
    }

    AligmentBuilder(Aligment initial):
        aligment{initial} {
    }

    operator Aligment() const {
        return aligment;
    }

    AligmentBuilder dimensions(Vector2d dimensions) {
        Aligment newAligment = aligment;
        newAligment.dimensions = dimensions;
        return AligmentBuilder(newAligment);
    }

    AligmentBuilder margin(int left = -1, int top = -1, int right = -1, int bottom = -1) {
        Aligment newAligment = aligment;
        newAligment.marginLeft = left;
        newAligment.marginRight = right;
        newAligment.marginTop = top;
        newAligment.marginBottom = bottom;
        return AligmentBuilder(newAligment);
    }

    AligmentBuilder tableDimensions(unsigned int columns = 1, unsigned int rows = 1) {
        assert(columns != 0 && rows != 0);
        Aligment newAligment = aligment;
        newAligment.tableColumns = columns;
        newAligment.tableRows = rows;
        return AligmentBuilder(newAligment);
    }

    AligmentBuilder tableCell(unsigned int column = 0, unsigned int row = 0) {
        assert(column < aligment.tableColumns && row < aligment.tableRows);
        Aligment newAligment = aligment;
        newAligment.ownColumn = column;
        newAligment.ownRow = row;
        return AligmentBuilder(newAligment);
    }
};

enum MouseState {
    None,
    Held,
    Hovered
};

enum MouseEventType {
    Hover,
    Hold,
    Click,
    Release,
    WheelMoveUp,
    WheelMoveDown
};

// button, text, layout, window, etc
class GuiElement {
    GuiElement* parent;
    std::vector<GuiElement*> children;
    MouseState mouseState = None;

protected:
    Rect2d rect;
    Aligment aligment;

    int drawPriority = 0;

    std::function<void()> onCloseCallback = nullptr;
    std::map<MouseEventType, std::function<void(Vector2d)>> mouseCallbacks;
    bool interseptsMouseEvents = true;

public:
    friend class GuiEngine;

    GuiElement(GuiElement* parent, Aligment aligment):
        parent{parent}, aligment{aligment} {
        static int roots = 0; // check if only 1 root is present
        if (parent == nullptr) {
            assert(roots == 0);
            roots++;
        } else {
            parent->children.push_back(this);
        }

        for (auto eventType : {Hover, Hold, Click, Release}) mouseCallbacks[eventType] = nullptr;
    };

    void setMouseCallback(MouseEventType type, std::function<void(Vector2d)> callback) {
        mouseCallbacks[type] = callback;
    }

    MouseState getMouseState() {
        return mouseState;
    }

    Rect2d getRect() {
        return rect;
    }

    virtual void draw() {};

    void setDrawPriority(int value) {
        drawPriority = value;
    } // element with higher ui will be drawn on top if overlaping

    int getDrawPriority() {
        return drawPriority;
    } // element with higher ui will be drawn on top if overlaping

    virtual void handleMouse(MouseEventType type, Vector2d mousePos) {
        // do nothing by default
    }

    virtual void handleKeyChar(char ch) {
        // do nothing by default
    }

    virtual void handleKeyDown(int keycode) {
        // do nothing by default
    }

    void setOnCloseCallback(std::function<void()> aCallback) {
        onCloseCallback = aCallback;
    }

    ~GuiElement();
};

class DisplayArea : public GuiElement {
public:
    DisplayArea():
        GuiElement(nullptr, Aligment()) {
        interseptsMouseEvents = false;
    }
};

class Button : public GuiElement {
public:
    Button(GuiElement* parent, Aligment aligment):
        GuiElement(parent, aligment) {
    }

    void draw() override {
        ALLEGRO_COLOR color = al_map_rgb(20, 20, 20);
        if (getMouseState() == Hovered)
            color = al_map_rgb(50, 50, 50);
        if (getMouseState() == Held)
            color = al_map_rgb(70, 70, 70);

        al_draw_filled_rectangle(getRect().p1.x, getRect().p1.y, getRect().p2.x, getRect().p2.y, color);
    }
};

class Label : public GuiElement {
    std::string text = "";

public:
    Label(GuiElement* parent, Aligment aligment, std::string text):
        GuiElement(parent, aligment), text{text} {
    }

    void draw() override;

    void setText(std::string text) {
        this->text = text;
    }
};

class Icon : public GuiElement {
    ALLEGRO_BITMAP* bitmap = nullptr;

public:
    Icon(GuiElement* parent, Aligment aligment, ALLEGRO_BITMAP* bitmap):
        GuiElement(parent, aligment), bitmap{bitmap} {
        interseptsMouseEvents = false;
    }

    void setBitmap(ALLEGRO_BITMAP*);

    void draw() override;
};

class Window : public GuiElement {
    Button* quitButton;
    GuiElement* area;

    inline static ALLEGRO_COLOR backgroundColor = al_map_rgb(30, 30, 30);
    inline static ALLEGRO_COLOR primaryColor = al_map_rgb(200, 200, 200);

public:
    Window(GuiElement* parent, Aligment aligment, bool closable);

    GuiElement* getInternalArea() {
        return area;
    }

    void draw() override {
        al_draw_filled_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, backgroundColor);
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, primaryColor, 2);
        al_draw_rectangle(area->getRect().p1.x, area->getRect().p1.y, area->getRect().p2.x, area->getRect().p2.y, primaryColor, 2);
    }
};

class NamedArea : public GuiElement {
    std::string text = "";
    GuiElement* area;

public:
    NamedArea(GuiElement* parent, Aligment aligment, std::string text):
        GuiElement(parent, aligment), text{text} {
        Aligment areaAligment;
        areaAligment.marginLeft = 2;
        areaAligment.marginRight = 2;
        areaAligment.marginTop = 12;
        areaAligment.marginBottom = 2;
        area = new GuiElement(this, areaAligment);
    }

    void draw() override;

    GuiElement* getInternalArea() {
        return area;
    }
};

class Console : public GuiElement {
    std::vector<std::string> lines;
    int lineFrom = 0;
    int linesMax = 10;

    bool editable = true;
    int cursorLine = 0;
    int cursorColumn = 0;
    int selectedLine = -1;

    long long int lastTimeCursorMovedMillis = 0;

public:
    Console(GuiElement* parent, Aligment aligment);

    void addLine(std::string line) {
        lines.push_back(line);
    }

    void moveCursor(Vector2d mousePos);

    void moveCursor(int line, int column) {
        cursorLine = line;
        cursorColumn = column;

        if (cursorLine < 0)
            cursorLine = 0;
        if (cursorColumn < 0)
            cursorColumn = 0;
        if (cursorLine >= lines.size())
            cursorLine = lines.size() - 1;
        if (cursorColumn > lines.at(cursorLine).size())
            cursorColumn = lines.at(cursorLine).size();

        auto duration = std::chrono::system_clock::now().time_since_epoch(); // TODO do it normaly
        lastTimeCursorMovedMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        if (cursorLine < lineFrom)
            lineFrom = cursorLine;
        if (cursorLine >= lineFrom + linesMax)
            lineFrom = cursorLine - linesMax + 1;
    }

    void scrollLines(int n) {
        if (lineFrom + n < 0 || lineFrom + n >= lines.size())
            return;
        lineFrom += n;
    }

    void setSelectedLine(int line) {
        selectedLine = line;
    }

    virtual void handleKeyChar(char ch) override {
        if (!editable) {
            return;
        }
        if (ch == '\b') { // backspace
            if (cursorColumn != 0) { // clear char
                lines.at(cursorLine).erase(cursorColumn - 1, 1);
                moveCursor(cursorLine, cursorColumn - 1);
            } else if (cursorLine != 0) { // clear line separation (cant clear first line)
                auto newColumn = lines.at(cursorLine - 1).size(); // TODO move lineFrom if cursor goes out of screen
                lines.at(cursorLine - 1) += lines.at(cursorLine);
                lines.erase(lines.begin() + cursorLine);
                moveCursor(cursorLine - 1, newColumn);
            }
        } else if (ch == '\r') { // handle ENTER
            if (cursorColumn == lines.at(cursorLine).size()) {
                lines.insert(lines.begin() + cursorLine + 1, "");
                moveCursor(cursorLine + 1, 0);
            } else {
                lines.insert(lines.begin() + cursorLine + 1, lines.at(cursorLine).substr(cursorColumn));
                lines.at(cursorLine).erase(lines.at(cursorLine).begin() + cursorColumn, lines.at(cursorLine).end());
                moveCursor(cursorLine + 1, 0);
            }
        } else {
            lines.at(cursorLine).insert(cursorColumn, 1, ch);
            moveCursor(cursorLine, cursorColumn + 1);
        }
    }

    std::vector<std::string> getLines() {
        return lines;
    }

    virtual void handleKeyDown(int keycode) override {
        switch (keycode) {
            case ALLEGRO_KEY_LEFT:
                moveCursor(cursorLine, cursorColumn - 1);
                break;
            case ALLEGRO_KEY_RIGHT:
                moveCursor(cursorLine, cursorColumn + 1);
                break;
            case ALLEGRO_KEY_UP:
                moveCursor(cursorLine - 1, cursorColumn);
                break;
            case ALLEGRO_KEY_DOWN:
                moveCursor(cursorLine + 1, cursorColumn);
                break;

            default:
                break;
        }
    }

    void setEditable(bool editable) {
        this->editable = editable;
    }

    void clearLines() {
        lines.clear();
        addLine("");
        moveCursor(0, 0);
        lineFrom = 0;
    }

    void setLines(std::vector<std::string> lines) {
        if (lines.size() == 0) {
            clearLines();
            return;
        }
        this->lines = lines;
        moveCursor(lines.size(), 1000);
        lineFrom = 0;
    }

    void draw() override;
};

class GuiEngine {
    GuiElement* rootElement = new DisplayArea();
    GuiElement* clickedElement = nullptr;
    GuiElement* keyboardInputHandler = nullptr;

    std::map<std::string, ALLEGRO_BITMAP*> icons;

    GuiEngine() {
        createProceduralIcons();
    };

    void createProceduralIcons() {
        ALLEGRO_COLOR blue = al_map_rgb(20, 120, 200);

        int iconsWidth = 14;
        int iconsHeight = 14;

        icons["run"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["run"]);
        al_draw_triangle(2, 1, 2, 13, 11, 7, al_map_rgb(20, 150, 100), 3);

        icons["stop"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["stop"]);
        al_draw_rectangle(2, 2, 12, 12, al_map_rgb(200, 50, 50), 3);

        icons["pause"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["pause"]);
        al_draw_line(4, 0, 4, 14, blue, 3);
        al_draw_line(11, 0, 11, 14, blue, 3);

        icons["unpause"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["unpause"]);
        al_draw_triangle(3, 2, 3, 12, 12, 7, blue, 3);

        icons["next"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["next"]);
        al_draw_triangle(3, 2, 3, 12, 10, 7, blue, 3);
        al_draw_line(13, 0, 13, 14, blue, 3);

        icons["breakpoint"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["breakpoint"]);
        al_draw_filled_circle(7, 7, 4, al_map_rgb(200, 0, 0));

        icons["close"] = al_create_bitmap(iconsWidth, iconsHeight);
        al_set_target_bitmap(icons["close"]);
        al_draw_line(2, 2, 12, 12, al_map_rgb(200, 200, 200), 2);
        al_draw_line(12, 2, 2, 12, al_map_rgb(200, 200, 200), 2);
    }

    void drawRecursively(GuiElement* element) {
        element->draw();
        std::sort(element->children.begin(), element->children.end(), [](GuiElement* a, GuiElement* b) { return a->getDrawPriority() < b->getDrawPriority(); });
        for (auto child : element->children) {
            drawRecursively(child);
        }
    }

    void updateRectsRecursively(GuiElement* element) {
        if (element == rootElement) {
            element->rect = Rect2d(Vector2d(0, 0), Vector2d(al_get_display_width(al_get_current_display()), al_get_display_height(al_get_current_display())));
        } else {

            auto cell = element->parent->rect; // cell == parent if table (h/w == 0)
            cell.p1 = cell.p1 + Vector2d(element->parent->rect.dimensions().x / element->aligment.tableColumns * element->aligment.ownColumn, element->parent->rect.dimensions().y / element->aligment.tableRows * element->aligment.ownRow);
            cell.p2 = cell.p1 + Vector2d(element->parent->rect.dimensions().x / element->aligment.tableColumns, element->parent->rect.dimensions().y / element->aligment.tableRows);
            element->rect.p1.x = element->aligment.marginLeft >= 0 ?
                cell.p1.x + element->aligment.marginLeft :
                element->aligment.marginRight >= 0 ?
                cell.p2.x - element->aligment.marginRight - element->aligment.dimensions.x :
                cell.center().x - element->aligment.dimensions.x / 2;
            element->rect.p2.x = element->aligment.marginRight >= 0 ?
                cell.p2.x - element->aligment.marginRight :
                element->rect.p1.x + element->aligment.dimensions.x;

            element->rect.p1.y = element->aligment.marginTop >= 0 ?
                cell.p1.y + element->aligment.marginTop :
                element->aligment.marginBottom >= 0 ?
                cell.p2.y - element->aligment.marginBottom - element->aligment.dimensions.y :
                cell.center().y - element->aligment.dimensions.y / 2;
            element->rect.p2.y = element->aligment.marginBottom >= 0 ?
                cell.p2.y - element->aligment.marginBottom :
                element->rect.p1.y + element->aligment.dimensions.y;

            assert(element->parent->rect.isInside(element->rect) || element->parent == rootElement); // check if any element is outside its parent (it is okay for windows to be outside display tho)
        }

        for (auto child : element->children) {
            updateRectsRecursively(child);
        }
    }

    void clearMouseStateRecursively(GuiElement* element) {
        element->mouseState = None;
        for (auto child : element->children) {
            clearMouseStateRecursively(child);
        }
    }

    bool applyEventRecursively(GuiElement* element, MouseEventType type, Vector2d mousePos) {
        if (!element->rect.isInside(mousePos)) {
            return false;
        }

        std::sort(element->children.begin(), element->children.end(), [](GuiElement* a, GuiElement* b) { return a->getDrawPriority() > b->getDrawPriority(); });
        for (auto child : element->children) {
            if (applyEventRecursively(child, type, mousePos))
                return true;
        }
        if (type == Hover) // update mouse state
            element->mouseState = Hovered;
        if (type == Hold)
            element->mouseState = Held;

        if (type == Release && clickedElement != element) // can release mouse only of clicked on this element
            return false;
        if (type == Click)
            clickedElement = element;

        if (element->mouseCallbacks[type]) // call callback if defined
            element->mouseCallbacks[type](mousePos);

        return element->interseptsMouseEvents;
    }

public:
    static GuiEngine* instance() {
        static GuiEngine instance;
        return &instance;
    }

    ALLEGRO_BITMAP* getIcon(std::string name) {
        return icons[name];
    };

    inline static ALLEGRO_FONT* debugFont = nullptr;
    inline static Vector2d drawingIndent{};

    void draw() {
        updateRectsRecursively(rootElement);
        drawRecursively(rootElement);
    };

    GuiElement* getDisplayArea() {
        return rootElement;
    }

    // returns true if clicked on some gui element
    bool click(Vector2d aPos) {
        if (keyboardInputHandler && !keyboardInputHandler->getRect().isInside(aPos)) {
            clearKeyboardInputHandler();
        }
        return applyEventRecursively(rootElement, Click, aPos);
    }

    // returns true if released on some gui element
    bool releaseMouse(Vector2d aPos) {
        return applyEventRecursively(rootElement, Release, aPos);
    }

    // returns true if on some gui element
    bool moveMouseWheel(Vector2d aPos, bool up) {
        return applyEventRecursively(rootElement, up ? WheelMoveUp : WheelMoveDown, aPos);
    }

    // returns true if released on some gui element
    bool updateMousePos(Vector2d aPos, bool clicked) {
        clearMouseStateRecursively(rootElement);
        return applyEventRecursively(rootElement, clicked ? Hold : Hover, aPos);
    }

    bool handlingKeyboardInput() {
        return keyboardInputHandler != nullptr;
    }

    void keyboardKeyPress(int keycode) {
        if (keyboardInputHandler == nullptr)
            return;

        keyboardInputHandler->handleKeyDown(keycode);
    }

    void keyboardCharPress(char ch) {
        if (keyboardInputHandler == nullptr)
            return;

        keyboardInputHandler->handleKeyChar(ch);
    }

    void setKeyboardInputHandler(GuiElement* element) {
        keyboardInputHandler = element;
    }

    GuiElement* getKeyboardInputHandler() {
        return keyboardInputHandler;
    }

    void clearKeyboardInputHandler() {
        keyboardInputHandler = nullptr;
    }
};

// enum GuiElementAligment {

// }
#endif // __PROJECTS_PROGRAMINGGAME_LIBS_CORE_INCLUDE_GUIENGINE_HPP_