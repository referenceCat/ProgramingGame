#include "GuiEngine.hpp"

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
        quitButton->setMouseCallback(Click, [this](Vector2d pos){delete this;});
        auto closeIcon = new Icon(quitButton, Aligment::byDimensionsAndCentered(Vector2d(20, 20)), GuiEngine::instance()->breakpointIcon);
    }

    Aligment areaAligment;
    areaAligment.marginLeft = 4;
    areaAligment.marginRight = 4;
    areaAligment.marginTop = closable ? 30 : 4;
    areaAligment.marginBottom = 4;
    area = new GuiElement(this, areaAligment);
    drawPriority = 1;
}