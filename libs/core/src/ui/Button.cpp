#include "Button.hpp"
#include "GuiEngine.hpp"

void LegacyButton::draw() {
    ALLEGRO_COLOR color = al_map_rgb(20 + tintLevel * 15, 20 + tintLevel * 15, 20 + tintLevel * 15);
    if (hover && tintLevel <= 5) {
        tintLevel = 5;
    } else if (tintLevel > 0) {
        tintLevel--;
    }
    al_draw_filled_rectangle(rect.p1.x + GuiEngine::drawingIndent.x, rect.p1.y + GuiEngine::drawingIndent.y, rect.p2.x + GuiEngine::drawingIndent.x, rect.p2.y + GuiEngine::drawingIndent.y, color);
}