#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <vector>

#include "../common/math.hpp"
#include "ManipulatorArm.hpp"
#include "Box.hpp"
#include "Controller.hpp"

class GameWorld {
    std::vector<ManipulatorArm*> arms{};
    std::vector<Box*> boxes{};
    std::vector<Controller*> controllers{};

public:
    ManipulatorArm* addManipulatorArm(int aJointsNumber, Point2d aRootJointPosition) {
        arms.push_back(new ManipulatorArm(aJointsNumber));
        arms.back()->setRootJointPosition(aRootJointPosition);
        return arms.back();
    }

    Controller* addController(Point2d aPos) {
        controllers.push_back(new Controller(aPos));
        return controllers.back();
    }

    Box* addBox(Rect2d aRect) {
        boxes.push_back(new Box(Rect2d(aRect)));
        return boxes.back();
    }

    ManipulatorArm* getManipulatorArm(int n) {
        return arms.at(n);
    }

    Box* getBox(int n) {
        return boxes.at(n);
    }

    Controller* getController(int n) {
        return controllers.at(n);
    }

    void drawAll() {
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

    void run() {
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
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_