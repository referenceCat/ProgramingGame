 #include "Icon.hpp"
 #include "GuiEngine.hpp"
 
void Icon::draw() {
    Rect2d globalRect = Rect2d(rect.p1 + GuiEngine::drawingIndent, rect.p2 + GuiEngine::drawingIndent);
    al_draw_bitmap(bitmap, globalRect.p1.x, globalRect.p1.y, 0);
};