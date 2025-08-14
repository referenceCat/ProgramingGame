#include "Machine.hpp"
#include "GameWorld.hpp"

void Machine::createBox() {
    parentWorld->addBox(Rect2d(rect.center(), 24, 24))->setContent(static_cast<BoxContent>(counter % 4));
    counter++;
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
