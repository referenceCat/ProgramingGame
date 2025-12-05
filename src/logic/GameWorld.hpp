#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_

#include <assert.h>
#include <vector>

#include "types.hpp"
#include "Arm.hpp"
#include "Box.hpp"
#include "GameObject.hpp"
#include "Machinery.hpp"
#include "Module.hpp"

class GameWorld {
    std::vector<Arm*> arms{};
    std::vector<Box*> boxes{};
    std::vector<Machinery*> machines{};
    std::vector<Module*> modules{};

public:
    constexpr static double surfaceY = 0;

    static GameWorld* instance() {
        static GameWorld instance;
        return &instance;
    }

    void addManipulatorArm(Arm* arm);
    void addBox(Box* box);
    void addMachinery(Machinery* machine);
    void addModule(Module* module);

    void removeBox(int id);

    Arm* getManipulatorArm(int id);
    Box* getBox(int id);
    Box* getBox(Vector2d aPos);
    std::vector<Box*> getBoxes();
    Machinery* getMachinery(int id);
    std::vector<Machinery*> getMachinery();
    Module* getModule(int id);
    std::vector<Module*> getModules();

    void drawSurface();
    void drawAll(bool, bool);
    void run();
    void click(Vector2d point);
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEWORLD_HPP_