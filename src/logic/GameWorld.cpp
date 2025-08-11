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

    ManipulatorArm* GameWorld::getManipulatorArm(int n) {
        return arms.at(n);
    }

    Box* GameWorld::getBox(int n) {
        return boxes.at(n);
    }

    Box* GameWorld::getBox(Point2d aPos) {
        for (auto box:  boxes) {
            if (box->getRect().isInside(aPos)) return box;
        }
        return nullptr;
    }

    Controller* GameWorld::getController(int n) {
        return controllers.at(n);
    }

    void GameWorld::drawAll() {
        // draw arms
        for (auto arm: arms) {
            arm->draw();
        }

        // draw boxes
        for (auto box: boxes) {
            box->draw();
        }

        // draw controllers
        for (auto controller: controllers) {
            controller->draw();
            controller->drawRegisters();
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