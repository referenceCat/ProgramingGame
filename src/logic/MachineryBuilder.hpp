#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_

#include "Machinery.hpp"
#include "CustomMachinery.hpp"
#include "GuiEngine.hpp"
#include "GameWorld.hpp"
#include "Controller.hpp"

enum MachineryType {
    TypeArm,
    TypeController,
    TypeFurnace, 
    TypeCreator,
    TypeDestroyer,
    TypeAssembler
};

class MachineryBuilder {
    Machinery* prototype = nullptr;
    Window* window;
    Vector2d positionToPlace = Vector2d(0, 0);

    void createPrototype(MachineryType type) {
        clearPrototype();
        switch (type)
        {
        case TypeArm:
            prototype = new ManipulatorTier1(Vector2d(0, 0), GameWorld::instance());
            break;
        case TypeController:
            prototype = new Controller(Vector2d(0, 0), GameWorld::instance());
            break;
        case TypeFurnace:
            prototype = new Furnace(Vector2d(0, 0), GameWorld::instance());
            break;
        default:
            break;
        }
    }

    void placeMachinery() {
        if (nullptr) return;
        GameWorld::instance()->addMachinery(prototype);
    }

    void clearPrototype() {
        delete prototype;
        prototype = nullptr;
    }

public:
    static MachineryBuilder *instance()
    {
        static MachineryBuilder instance;
        return &instance;
    }

    void onClick() {};

    void createWindow()
    {
        if (window)
            GuiEngine::instance()->closeWindow(window);
        
        window = GuiEngine::instance()->addWindow(Rect2d(Vector2d(130,800), 200, 250), true, false);

        window->addLabel(Vector2d(20, 40), false, "Build machinery:", 0);

        Button *armButton = window->addButton(Rect2d(Vector2d(20, 60), Vector2d(200, 80)));
        window->addLabel(armButton->getRect().center(), true, "Manipulator Tier 1", 0);
        armButton->setOnClickCallback([this](){createPrototype(TypeArm); });

        Button *controllerButton = window->addButton(Rect2d(Vector2d(20, 85), Vector2d(200, 105)));
        window->addLabel(controllerButton->getRect().center(), true, "Controller", 0);
        controllerButton->setOnClickCallback([this](){createPrototype(TypeController); });

        Button *furnaceButton = window->addButton(Rect2d(Vector2d(20, 110), Vector2d(200, 130)));
        window->addLabel(furnaceButton->getRect().center(), true, "Furnace", 0);
        furnaceButton->setOnClickCallback([this](){createPrototype(TypeFurnace); });
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_