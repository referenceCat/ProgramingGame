#include "Machine.hpp"
#include "GameWorld.hpp"

void Machine::destroyBox(Box *box) {
    parentWorld->removeBox(box->getId());
}

void Machine::createBox()
{
    parentWorld->addBox(Rect2d(rect.center(), 24, 24))->setContent(static_cast<BoxContent>(counter % 5));
    counter++;
}

std::vector<Box *> Machine::getBoxesInside(ProductionArea area) {
    std::vector<Box *> result;
    Rect2d areaGlobalRect = area.rect;
    areaGlobalRect.p1.x += rect.p1.x;
    areaGlobalRect.p1.y += rect.p1.y;
    areaGlobalRect.p2.x += rect.p2.x;
    areaGlobalRect.p2.y += rect.p2.y;
    for (auto item: parentWorld->getBoxes()) {
        if  (areaGlobalRect.isInside(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Box *> Machine::getBoxesTouching(ProductionArea area) {
    std::vector<Box *> result;
    Rect2d areaGlobalRect = area.rect;
    areaGlobalRect.p1.x += rect.p1.x;
    areaGlobalRect.p1.y += rect.p1.y;
    areaGlobalRect.p2.x += rect.p2.x;
    areaGlobalRect.p2.y += rect.p2.y;
    for (auto item: parentWorld->getBoxes()) {
        if  (areaGlobalRect.isIntersecting(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}
