#include "Module.hpp"
#include "GameWorld.hpp"

Module::Module() {}

void Module::addToGameWorld()
{
    GameWorld::instance()->addModule(this);
}

bool ModuleBuilder::buildModule(bool initial) {
    if (modulePrototype == nullptr) return false;

    if (initial) {
        modulePrototype->setTransforms(Vector2d(0, 0), Rotation(0));
    } else {
        ModuleNode* newModuleNode = modulePrototype->getNode(newModuleNodeNumber);
        modulePrototype->setTransforms(parentModuleNode, newModuleNode);
        for (auto module: GameWorld::instance()->getModules()) {
            if (module == parentModuleNode->parentModule) continue; // ignore collision with parent module
            if (module->checkBlockingAreaCollision(modulePrototype)) {
                return false;
            }
            parentModuleNode->attachedNode = newModuleNode;
            newModuleNode->attachedNode = parentModuleNode;
        }
    }
    

    GameWorld::instance()->addModule(modulePrototype);
    modulePrototype->addToGameWorld();
    modulePrototype = nullptr;
    return true;
}

bool ModuleBuilder::createModulePrototype(ModuleType type) {
    delete modulePrototype;
    switch (type) { // put this data to json vvv
    case ModuleType::Corridor:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(2);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Corridor/background.png"), Vector2d(320, 320), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Corridor/walls.png"), Vector2d(320, 320), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -9), Vector2d(16, -7)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 9), Vector2d(16, 7)));
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-16, -12), Vector2d(16, 12)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-16, -7), Vector2d(16, 7)));
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::ConnectorCross:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
        moduleSetup->addNode(Vector2d(Rotation(0), 12), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 12), Rotation(M_PI / 2));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 12), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 12), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorCross/background.png"), Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorCross/walls.png"), Vector2d(240, 240), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, -12), Vector2d(-7, -7))); // top left
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, -9), Vector2d(-7, -7)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(7, -12), Vector2d(9, -9))); // top right
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(12, -9), Vector2d(7, -7)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, 9), Vector2d(-7, 7))); // bottom left
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, 12), Vector2d(-7, 7)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(12, 9), Vector2d(7, 7))); // bottom right
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(9, 12), Vector2d(7, 7)));
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-12, -12), Vector2d(12, 12)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -12), Vector2d(7, 12)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-12, -7), Vector2d(12, 7)));
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::Connector3:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(3);
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 8.62), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 3), 8.62), Rotation(M_PI / 3));
        moduleSetup->addNode(Vector2d(Rotation(-M_PI / 3), 8.62), Rotation(-M_PI / 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Connector3/background.png"), Vector2d(8.62 * 20, 320), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Connector3/walls.png"), Vector2d(8.62 * 20, 320), CommonValues::zModuleWalls);
        // add collision
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::Deadend:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(1);
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 12), Rotation(M_PI / 2));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Deadend/background.png"), Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Deadend/walls.png"), Vector2d(240, 240), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, 3), Vector2d(-7, 12)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(9, 3), Vector2d(7, 12)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, 3), Vector2d(9, 5)));
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-12, 0), Vector2d(12, 12)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, 5), Vector2d(7, 12)));
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::ConnectorT:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(3);
        moduleSetup->addNode(Vector2d(Rotation(0), 12), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 12), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 12), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorT/background.png"), Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorT/walls.png"), Vector2d(240, 240), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, -12), Vector2d(-7, -7))); // top left
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, -9), Vector2d(-7, -7)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(7, -12), Vector2d(9, -9))); // top right
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(12, -9), Vector2d(7, -7)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, 7), Vector2d(12, 9))); // bottom
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-12, -12), Vector2d(12, 12)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -12), Vector2d(7, -7)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-12, -7), Vector2d(12, 7)));
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::Frame:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(2);
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 16), Rotation(M_PI / 2));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 16), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/FrameStraight/background.png"), Vector2d(320, 320), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-6, -16), Vector2d(6, 16)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -16), Vector2d(7, 16)));
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::FrameCross:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
        moduleSetup->addNode(Vector2d(Rotation(0), 7), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 7), Rotation(M_PI / 2));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 7), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 7), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/FrameCross/background.png"), Vector2d(140, 140), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-6, -6), Vector2d(6, 6)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -7), Vector2d(7, 7)));
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::Frame3:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(3);
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 4.04), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 3), 4.04), Rotation(M_PI / 3));
        moduleSetup->addNode(Vector2d(Rotation(-M_PI / 3), 4.04), Rotation(-M_PI / 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Frame3/background.png"), Vector2d(4.04 * 20, 140), CommonValues::zModuleMainBackgroung);
        // add collision
        modulePrototype = moduleSetup;
        break;
    }
    case ModuleType::LargeModule:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
        moduleSetup->addNode(Vector2d(16, -16), Rotation(0));
        moduleSetup->addNode(Vector2d(0, 32), Rotation(M_PI / 2));
        moduleSetup->addNode(Vector2d(-16, -16), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(0, -32), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/LargeModule/background.png"), Vector2d(320, 640), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/LargeModule/walls.png"), Vector2d(320, 640), CommonValues::zModuleWalls);
        moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/LargeModule/far_backgound.png"), Vector2d(320, 640), CommonValues::zModuleFarBackgroung);
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(16, 32)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(16, 32)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(-7, -30))); // top left
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(-14, -23))); 
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, -32), Vector2d(7, -30))); // top rith
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, -32), Vector2d(14, -23))); 
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 32), Vector2d(-7, 30))); // bottom left
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 32), Vector2d(-14, -9))); 
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, 32), Vector2d(7, 30))); // bottom right
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, 32), Vector2d(14, -9))); 
        modulePrototype = moduleSetup;
        break;
    }
    default:
        break;
    }
    selectNewNodeNumber(0);
    updateNodeNumberSelection();
    return true;
}

bool ModuleBuilder::selectNewNodeNumber(int number) {
    newModuleNodeNumber = number;
    return true;
}
