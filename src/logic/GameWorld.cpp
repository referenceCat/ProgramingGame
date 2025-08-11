#include "GameWorld.hpp"

ManipulatorArm* GameWorld::addManipulatorArm(int aJointsNumber, Point2d aRootJointPosition) {
    arms.push_back(new ManipulatorArm(aJointsNumber, this));
    arms.back()->setRootJointPosition(aRootJointPosition);
    return arms.back();
}

Controller* GameWorld::addController(Point2d aPos) {
    controllers.push_back(new Controller(aPos, this));
    return controllers.back();
}

Box* GameWorld::addBox(Rect2d aRect) {
    boxes.push_back(new Box(Rect2d(aRect), this));
    return boxes.back();
}

Machine* GameWorld::addMachine(Point2d aPos) {
    machines.push_back(new Machine(aPos, this));
    return machines.back();
}

void GameWorld::removeBox(int id) {
    Box* boxToRemove = getBox(id);
    for (auto item: arms) {
        item->removeBox(boxToRemove);
    }
    delete boxToRemove;
    boxes.erase(std::remove(boxes.begin(), boxes.end(), boxToRemove), boxes.end());
}

ManipulatorArm* GameWorld::getManipulatorArm(int id) {
    for (auto item: arms) {
        if (item->getId() == id) return item;
    }
    return nullptr;
}

Machine* GameWorld::getMachine(int id) {
    for (auto item: machines) {
        if (item->getId() == id) return item;
    }
    return nullptr;
}

std::vector<Box *> GameWorld::getBoxes() {
    std::vector<Box *> result = boxes;
    return result;
}

Box* GameWorld::getBox(int id) {
    for (auto item: boxes) {
        if (item->getId() == id) return item;
    }
    return nullptr;
}

Box* GameWorld::getBox(Point2d aPos) {
    for (auto item: boxes) {
        if (item->getRect().isInside(aPos)) return item;
    }
    return nullptr;
}

Controller* GameWorld::getController(int id) {
    for (auto item: controllers) {
        if (item->getId() == id) return item;
    }
    return nullptr;
}

void GameWorld::drawAll() {
    for (auto item: arms) {
        item->draw();
    }

    for (auto item: boxes) {
        item->draw();
    }

    for (auto item: controllers) {
        item->draw();
        item->drawRegisters();
    }

    for (auto item: machines) {
        item->draw();
    }
}

void GameWorld::run() {
    // run arms
    for (auto arm: arms) {
        arm->moveToTarget();
        arm->recalculate();
    }
    // run controllers
    for (auto controller: controllers) {
        controller->execNextInstr();
    }
}