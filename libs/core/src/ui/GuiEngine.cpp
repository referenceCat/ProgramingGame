#include "GuiEngine.hpp"

void Label::draw() {
    auto pos = rect.center() - Vector2d(al_get_text_width(GuiEngine::debugFont, text.c_str()) / 2, al_get_font_line_height(GuiEngine::debugFont) / 2);
    al_draw_text(GuiEngine::debugFont, al_map_rgb(255, 255, 255), pos.x + 0.1, pos.y + 0.1, 0, text.c_str());
}