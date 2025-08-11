#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <vector>

#include "../common/common.hpp"
#include "ManipulatorArm.hpp"
#include "Box.hpp"
#include "Controller.hpp"

class GameWorld {
    std::vector<ManipulatorArm*> arms{};
    std::vector<Box*> boxes{};
    std::vector<Controller*> controllers{};

public:
    ManipulatorArm *addManipulatorArm(int aJointsNumber, Point2d aRootJointPosition);
    Controller *addController(Point2d aPos);
    Box *addBox(Rect2d aRect);
    ManipulatorArm *getManipulatorArm(int n);
    Box *getBox(int n);
    Box *getBox(Point2d aPos);
    Controller *getController(int n);
    void drawAll();
    void run();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_