#include "Machinery.hpp"
#include "GameWorld.hpp"
#include "../graphics/GraphicsEngine.hpp"

void Machinery::destroyBox(Box *box) {
    GameWorld::instance()->removeBox(box->getId());
}

Box* Machinery::createBox(ProductionArea area) // TODO no
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
    areaGlobalRect.p2.x += rect.p1.x;
    areaGlobalRect.p2.y += rect.p1.y;
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
void Machinery::drawDebug() {
    GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zDebug,
                                            al_map_rgb(100, 255, 100), 1);

  for (auto item : areas) {
    Rect2d areaRect;
    areaRect.p1 = item->rect.p1 + rect.p1;
    areaRect.p2 = item->rect.p2 + rect.p1;
    GraphicsEngine::instance()->drawRectangle(areaRect, CommonValues::zDebug,
                                              al_map_rgb(100, 100, 255), 1);
  }
}
Rect2d Machinery::getRect() { return rect; }
void Machinery::setCenter(Vector2d pos) {
  rect = Rect2d::fromCenterAndDimensions(pos, rect.dimensions());
}

void Machinery::setAddress(int id) {
    address = id;
}

int Machinery::getAddress() {
    return address;
}
