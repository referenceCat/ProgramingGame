#include "Machinery.hpp"
#include "GameWorld.hpp"
#include "GraphicsEngine.hpp"

void Machinery::destroyBox(Box* box) {
    GameWorld::instance()->removeBox(box->getId());
}

Box* Machinery::createBox(int portIndex)
{
    auto box = new Box(Rect2d::fromCenterAndDimensions(getPort(portIndex).rect.center() + rect.p1, Vector2d(2, 2)));
    box->addToGameWorld();
    return box;
}

Port& Machinery::getPort(int index) {
    return ports.at(index);
}

std::vector<Box*> Machinery::getBoxesInside(int portIndex) {
    std::vector<Box*> result;
    Rect2d portGlobalRect = getPort(portIndex).rect;
    portGlobalRect.p1.x += rect.p1.x;
    portGlobalRect.p1.y += rect.p1.y;
    portGlobalRect.p2.x += rect.p1.x;
    portGlobalRect.p2.y += rect.p1.y;
    for (auto item : GameWorld::instance()->getBoxes()) {
        if (portGlobalRect.isInside(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Box*> Machinery::getBoxesTouching(int portIndex) {
    std::vector<Box*> result;
    Rect2d portGlobalRect = getPort(portIndex).rect;
    portGlobalRect.p1.x += rect.p1.x;
    portGlobalRect.p1.y += rect.p1.y;
    portGlobalRect.p2.x += rect.p1.x;
    portGlobalRect.p2.y += rect.p1.y;
    for (auto item : GameWorld::instance()->getBoxes()) {
        if (portGlobalRect.isIntersecting(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

void Machinery::setMemoryValue(size_t address, MemoryWord value) {
    memory.at(address) = value;
}

MemoryWord Machinery::getMemoryValue(size_t address) {
    return memory.at(address);
}

void Machinery::addToGameWorld() {
    GameWorld::instance()->addMachinery(this);
}

void Machinery::drawDebug() {
    GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zDebug,
        al_map_rgb(100, 255, 100), 1);

    for (auto item : ports) {
        Rect2d areaRect;
        areaRect.p1 = item.rect.p1 + rect.p1;
        areaRect.p2 = item.rect.p2 + rect.p1;
        GraphicsEngine::instance()->drawRectangle(areaRect, CommonValues::zDebug,
            al_map_rgb(100, 100, 255), 1);
    }
}

void Machinery::addPort(Rect2d rect) {
    ports.emplace_back(rect);
}

Rect2d Machinery::getRect() {
    return rect;
}

void Machinery::onMemoryWrite(size_t address, MemoryWord value) {
    if (address >= memory.size()) return;
    memory.at(address) = value;
}

MemoryWord Machinery::onMemoryRead(size_t address) {
    if (address >= memory.size()) return 0;
    return memory.at(address);
}

void Machinery::setCenter(Vector2d pos) {
    rect = Rect2d::fromCenterAndDimensions(pos, rect.dimensions());
}

void Machinery::setAddress(int id) {
    address = id;
}

int Machinery::getAddress() {
    return address;
}

size_t Machinery::getMemorySize() {
    return memory.size();
}

void AbstractAssembler::drawDebug() {
    Machinery::drawDebug();
}

void AbstractAssembler::draw() {
    Machinery::draw();
    if (drawable) {
        drawable->setParameter("testParam", static_cast<float>(tick) / 10);
        drawable->setPos(getRect().center());
        drawable->draw();
    }
}

void AbstractAssembler::run() {
    Machinery::run();
    tick++;
}

void AbstractAssembler::setDrawable(AbstractDrawable* aDrawable) {
    drawable = aDrawable;
}

// void AbstractAssembler::setReciept(Reciept reciept) {
//     // TODO validate reciept first
// }
