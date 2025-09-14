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
    TypeAssembler,
    TypeLab,
    TypeParticleResearch
};

class MachineryBuilder {
    Window* window;
    Machinery* prototype;
    bool stickToGrid = true;
    bool blocked = false;

    void selectType(MachineryType type) {
        switch (type)
        {
        case TypeArm:
            prototype = new ManipulatorTier1(Vector2d());
            break;
        case TypeController:
            prototype = new Controller(Vector2d());
            break;
        case TypeFurnace:
            prototype = new Furnace(Vector2d());
            break;
        case TypeAssembler:
            prototype = new AssemblerTier1(Vector2d());
            break;
        case TypeCreator:
            prototype = new BoxGenerator(Vector2d());
            break;
        case TypeDestroyer:
            prototype = new BoxDestroyer(Vector2d());
            break;
        case TypeLab:
            prototype = new Lab(Vector2d());
            break;
        case TypeParticleResearch:
            prototype = new ParticleResearch(Vector2d());
            break;
        default:
            break;
        }
    }

public:
    static MachineryBuilder *instance()
    {
        static MachineryBuilder instance;
        return &instance;
    }

    bool checkBlocked() {
        if (!prototype) return false;


        bool touchingBuildableArea = false;
        for (auto module: GameWorld::instance()->getModules()) {
            if (module->checkTouchesBuildableArea(prototype->getRect())) {
                touchingBuildableArea = true;
                break;
            }
        }
        if (!touchingBuildableArea) return true;

        for (auto machinery: GameWorld::instance()->getMachinery()) {
            if (machinery->getRect().isIntersecting(prototype->getRect())) return true;
        }

        for (auto module: GameWorld::instance()->getModules()) {
            if (module->checkWallCollision(prototype->getRect())) return true;
        }

        return false;
    }

    void clearItem() {
        delete prototype;
        prototype = nullptr;
    }

    void onClick() {
        if (prototype == nullptr) {
            return;
        }

        blocked = checkBlocked();
        if (blocked) return;
        prototype->addToGameWorld();
        prototype = nullptr;
    };

    void mousePos(Vector2d pos) {
        if (!prototype) return;
        Rect2d lastPos = prototype->getRect();
        if (stickToGrid)  {
            Vector2d roundedPos;
            roundedPos.x = std::round(pos.x - prototype->getRect().dimensions().x / 2) + prototype->getRect().dimensions().x / 2;
            roundedPos.y = std::round(pos.y - prototype->getRect().dimensions().y / 2) + prototype->getRect().dimensions().y / 2;
            prototype->setCenter(roundedPos);
        } else {
            prototype->setCenter(pos);
        }

        if (prototype->getRect() != lastPos) blocked = checkBlocked();
        
    };

    void drawGhost() {
        if (!prototype) return;
        prototype->draw();
        if (!blocked)
            GraphicsEngine::instance()->drawRectangle(prototype->getRect(), 0, al_map_rgba(50, 100, 100, 30));
        else
            GraphicsEngine::instance()->drawRectangle(prototype->getRect(), 0, al_map_rgba(100, 50, 50, 30));
    }

    void createWindow()
    {
        if (window)
            GuiEngine::instance()->closeWindow(window);
        
        window = GuiEngine::instance()->addWindow(Rect2d(Vector2d(150,700), 300, 220), true, false);

        window->addLabel(Vector2d(20, 40), false, "Build machinery:", 0);

        Button *armButton = window->addButton(Rect2d(Vector2d(20, 60), Vector2d(200, 80)));
        window->addLabel(armButton->getRect().center(), true, "Manipulator", 0);
        armButton->setOnClickCallback([this](){this->selectType(TypeArm);});

        Button *controllerButton = window->addButton(Rect2d(Vector2d(20, 85), Vector2d(200, 105)));
        window->addLabel(controllerButton->getRect().center(), true, "Controller", 0);
        controllerButton->setOnClickCallback([this](){this->selectType(TypeController);});

        Button *labButton = window->addButton(Rect2d(Vector2d(20, 110), Vector2d(200, 130)));
        window->addLabel(labButton->getRect().center(), true, "Lab", 0);
        labButton->setOnClickCallback([this](){this->selectType(TypeLab);});

        Button *creatorButton = window->addButton(Rect2d(Vector2d(20, 135), Vector2d(200, 155)));
        window->addLabel(creatorButton->getRect().center(), true, "Box Creator", 0);
        creatorButton->setOnClickCallback([this](){this->selectType(TypeCreator);});

        Button *destroyerButton = window->addButton(Rect2d(Vector2d(20, 160), Vector2d(200, 185)));
        window->addLabel(destroyerButton->getRect().center(), true, "Box Destroyer", 0);
        destroyerButton->setOnClickCallback([this](){this->selectType(TypeDestroyer);});

        Button *particleButton = window->addButton(Rect2d(Vector2d(20, 190), Vector2d(200, 210)));
        window->addLabel(particleButton->getRect().center(), true, "Particle Detector", 0);
        particleButton->setOnClickCallback([this](){this->selectType(TypeParticleResearch);});
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_