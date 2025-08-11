#include "Machine.hpp"
#include "GameWorld.hpp"

void Machine::createBox() {
    parentWorld->addBox(Rect2d(rect.center(), 25, 25));
}

void Machine::destroyBoxes() {
    std::vector<int> boxesIdsToRemove;
    for (auto item: parentWorld->getBoxes()) {
        if  (rect.isInside(item->getRect())) {
            boxesIdsToRemove.push_back(item->getId());
        }
    }

    for (auto id: boxesIdsToRemove) {
        parentWorld->removeBox(id);
    }
}

void Machine::grillBoxes() {
    for (auto item: parentWorld->getBoxes()) {
        if (rect.isInside(item->getRect())) {
            item->grill();
        }
    }
}
