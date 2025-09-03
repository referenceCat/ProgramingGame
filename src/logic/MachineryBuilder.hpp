#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_

#include "Machinery.hpp"
#include "CustomMachinery.hpp"
#include "GuiEngine.hpp"
#include "GameWorld.hpp"
#include "Controller.hpp"

enum MachineryType {
    TypeNone,
    TypeArm,
    TypeController,
    TypeFurnace, 
    TypeCreator,
    TypeDestroyer,
    TypeAssembler
};

class MachineryBuilder {
    Window* window;
    Rect2d ghost = Rect2d();
    MachineryType selectedType = TypeNone;
    bool stickToGrid = true;

    void placeMachinery() {
        // if (nullptr) return;
        // GameWorld::instance()->addMachinery(prototype);
    }

    void selectType(MachineryType type) {
        if (type == TypeNone) {
            clearItem();
            return;
        }

        Vector2d ghostDimensions;

        selectedType = type;
        switch (selectedType)
        {
        case TypeArm:
            ghostDimensions = Vector2d(5, 3);
            break;
        case TypeController:
            ghostDimensions = Vector2d(5, 7);
            break;
        case TypeFurnace:
            ghostDimensions = Vector2d(10, 10);
            break;
        default:
            break;
        }
        ghost = Rect2d::fromCenterAndDimensions(Vector2d(0, 0), ghostDimensions);
    }

public:
    static MachineryBuilder *instance()
    {
        static MachineryBuilder instance;
        return &instance;
    }

    void clearItem() {
        selectedType = TypeNone;
        ghost = Rect2d();
    }

    void onClick() {
        if (selectedType == TypeNone) {
            clearItem();
            return;
        }

        Machinery* newMachinery;
        switch (selectedType)
        {
        case TypeArm:
            newMachinery = new ManipulatorTier1(ghost.center());
            break;
        case TypeController:
            newMachinery = new Controller(ghost.center());
            break;
        case TypeFurnace:
            newMachinery = new Furnace(ghost.center());
            break;
        default:
            break;
        }
        newMachinery->addToGameWorld();
        clearItem();
    };

    void mousePos(Vector2d pos) {
        if (stickToGrid)  {
            Vector2d roundedPos;
            roundedPos.x = std::round(pos.x - ghost.dimensions().x / 2) + ghost.dimensions().x / 2;
            roundedPos.y = std::round(pos.y - ghost.dimensions().y / 2) + ghost.dimensions().y / 2;
            ghost = Rect2d::fromCenterAndDimensions(roundedPos, ghost.dimensions());
        } else {
            ghost = Rect2d::fromCenterAndDimensions(pos, ghost.dimensions());
        }
        
    };

    void drawGhost() {
        if (selectedType == TypeNone) return;
        GraphicsEngine::instance()->drawRectangle(ghost, 0, al_map_rgba(50, 100, 100, 30));
    }

    void createWindow()
    {
        if (window)
            GuiEngine::instance()->closeWindow(window);
        
        window = GuiEngine::instance()->addWindow(Rect2d(Vector2d(130,800), 200, 250), true, false);

        window->addLabel(Vector2d(20, 40), false, "Build machinery:", 0);

        Button *armButton = window->addButton(Rect2d(Vector2d(20, 60), Vector2d(200, 80)));
        window->addLabel(armButton->getRect().center(), true, "Manipulator Tier 1", 0);
        armButton->setOnClickCallback([this](){this->selectType(TypeArm);});

        Button *controllerButton = window->addButton(Rect2d(Vector2d(20, 85), Vector2d(200, 105)));
        window->addLabel(controllerButton->getRect().center(), true, "Controller", 0);
        controllerButton->setOnClickCallback([this](){this->selectType(TypeController);});

        Button *furnaceButton = window->addButton(Rect2d(Vector2d(20, 110), Vector2d(200, 130)));
        window->addLabel(furnaceButton->getRect().center(), true, "Furnace", 0);
        furnaceButton->setOnClickCallback([this](){this->selectType(TypeFurnace);});
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_