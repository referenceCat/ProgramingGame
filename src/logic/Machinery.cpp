#include "Machinery.hpp"
#include "GameWorld.hpp"

void Machinery::destroyBox(Box *box) {
    GameWorld::instance()->removeBox(box->getId());
}

Box* Machinery::createBox(ProductionArea area)
{
    auto box = new Box(Rect2d::fromCenterAndDimensions(area.rect.center() + rect.p1, Vector2d(2, 2)));
    box->addToGameWorld();
    return box;
}

std::vector<Box *> Machinery::getBoxesInside(ProductionArea area) {
    std::vector<Box *> result;
    Rect2d areaGlobalRect = area.rect;
    areaGlobalRect.p1.x += rect.p1.x;
    areaGlobalRect.p1.y += rect.p1.y;
    areaGlobalRect.p2.x += rect.p1.x;
    areaGlobalRect.p2.y += rect.p1.y;
    for (auto item: GameWorld::instance()->getBoxes()) {
        if  (areaGlobalRect.isInside(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Box *> Machinery::getBoxesTouching(ProductionArea area) {
    std::vector<Box *> result;
    Rect2d areaGlobalRect = area.rect;
    areaGlobalRect.p1.x += rect.p1.x;
    areaGlobalRect.p1.y += rect.p1.y;
    areaGlobalRect.p2.x += rect.p2.x;
    areaGlobalRect.p2.y += rect.p2.y;
    for (auto item: GameWorld::instance()->getBoxes()) {
        if  (areaGlobalRect.isIntersecting(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

void Machinery::addToGameWorld()
{
    GameWorld::instance()->addMachinery(this);
}
