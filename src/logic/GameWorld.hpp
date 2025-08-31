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
#include "GameObject.hpp"
#include "Machine.hpp"
#include "Module.hpp"

class GameWorld {
    std::vector<ManipulatorArm*> arms{};
    std::vector<Box*> boxes{};
    std::vector<Controller*> controllers{};
    std::vector<Machine*> machines{};
    std::vector<Module*> modules{};

public:
    static GameWorld* instance() {
        static GameWorld instance;
        return &instance;
    }

    ManipulatorArm *addManipulatorArm(int aJointsNumber, Vector2d aRootJointPosition);
    Controller *addController(Vector2d aPos);
    Box *addBox(Rect2d aRect);
    void addMachine(Machine* machine);
    void addModule(Module* module);

    void removeBox(int id);

    ManipulatorArm *getManipulatorArm(int id);
    Controller *getController(int id);
    Box *getBox(int id);
    Box *getBox(Vector2d aPos);
    std::vector<Box*> getBoxes();
    Machine *getMachine(int id);
    Module *getModule(int id);

    void drawAll();
    void run();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_