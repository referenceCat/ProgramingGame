#include "Window.hpp"
#include "GuiEngine.hpp"

LegacyWindow::LegacyWindow(Rect2d rect, bool movable = false, bool closable = false):
    rect(rect), movable(movable), closable(closable) {
    if (movable) {
        dragArea = Rect2d(Vector2d(20, 4), Vector2d(70, 16));
    }
    if (closable) {
        buttons.push_back(new LegacyButton(Rect2d(Vector2d(10, 10), 14, 14)));
        buttons.back()->setOnClickCallback([this]() { GuiEngine::instance()->closeWindow(this); });
    }
    if (closable || movable) {
        // indent.y = 24;
    }
};

void LegacyWindow::draw() {
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

    for (auto item : buttons) {
        item->draw();
    }

    for (auto item : labels) {
        item->draw();
    }

    for (auto item : icons) {
        item->draw();
    }

    GuiEngine::drawingIndent.x -= rect.p1.x;
    GuiEngine::drawingIndent.y -= rect.p1.y;

    if (closable) {
        al_draw_line(rect.p1.x + 5, rect.p1.y + 5, rect.p1.x + 16, rect.p1.y + 16, primaryColor, 2);
        al_draw_line(rect.p1.x + 5, rect.p1.y + 16, rect.p1.x + 16, rect.p1.y + 5, primaryColor, 2);
    }
};

void LegacyWindow::setOnCloseCallback(std::function<void()> aCallback) {
    onCloseCallback = aCallback;
}

// returns true if clicked on some gui element
bool LegacyWindow::click(Vector2d aPos) {
    if (movable && dragArea.isInside(aPos)) {
        moving = true;
        mouseDragPos = aPos;
        return true;
    }

    for (auto item : buttons) {
        if (item->getRect().isInside(aPos)) {
            item->click();
            return true;
        }
    }

    return false;
}

void LegacyWindow::releaseMouse(Vector2d aPos) {
    moving = false;
}

bool LegacyWindow::moveMouse(Vector2d aPos) {
    if (moving) {
        Vector2d movement = aPos - mouseDragPos;
        rect.p1 = rect.p1 + movement;
        rect.p2 = rect.p2 + movement;
        return true;
    }

    for (auto item : buttons) {
        item->setTinted(false);
    }

    for (auto item : buttons) {
        if (item->getRect().isInside(aPos)) {
            item->setTinted(true);
            return true;
        }
    }

    return false;
}

LegacyButton* LegacyWindow::addButton(Rect2d aRect) {
    buttons.push_back(new LegacyButton(aRect));
    return buttons.back();
};

void LegacyWindow::deleteButton(LegacyButton* buttonToRemove) {
    delete buttonToRemove;
    buttons.erase(std::remove(buttons.begin(), buttons.end(), buttonToRemove), buttons.end());
};

void LegacyWindow::deleteIcon(LegacyIcon* iconToRemove) {
    delete iconToRemove;
    icons.erase(std::remove(icons.begin(), icons.end(), iconToRemove), icons.end());
};

void LegacyWindow::deleteLabel(LegacyLabel* labelToRemove) {
    delete labelToRemove;
    labels.erase(std::remove(labels.begin(), labels.end(), labelToRemove), labels.end());
};

LegacyLabel* LegacyWindow::addLabel(Vector2d aPos, bool centered, std::string text, int line) {
    aPos.y += line * 14;
    labels.push_back(new LegacyLabel(aPos, centered, text));
    return labels.back();
};

LegacyIcon* LegacyWindow::addIcon(Vector2d aPos, ALLEGRO_BITMAP* bitmap) {
    icons.push_back(new LegacyIcon(aPos, bitmap));
    return icons.back();
};

Rect2d LegacyWindow::getRect() {
    return rect;
}

LegacyWindow::~LegacyWindow() {
    if (onCloseCallback)
        onCloseCallback();
    for (auto item : buttons) {
        delete item;
    }
}