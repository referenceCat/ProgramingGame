 #include "Text.hpp"
 #include "GuiEngine.hpp"
 
void Label::draw() {
    Rect2d globalRect = Rect2d(rect.p1 + GuiEngine::drawingIndent, rect.p2 + GuiEngine::drawingIndent);
    if (flags.highlighted) {
        al_draw_filled_rectangle(globalRect.p1.x - 4, globalRect.p1.y - 2, globalRect.p2.x + 2, globalRect.p2.y + 1, al_map_rgb(100, 100, 100));
    }
    al_draw_text(GuiEngine::debug_font, al_map_rgb(255, 255, 255), globalRect.p1.x, globalRect.p1.y, 0, text.c_str());
};

Label::Label(Point2d aPos, bool isCentered, std::string text): text(text), isCentered(isCentered), flags(LabelFlags()) {
    double h = al_get_font_line_height(GuiEngine::debug_font) + 2;
    double w = al_get_text_width(GuiEngine::debug_font, text.c_str());
    if (isCentered) {
        rect = Rect2d(aPos, h, w);
    } else {
        rect = Rect2d(aPos, Point2d(aPos.x + w, aPos.y + h));
    }
};