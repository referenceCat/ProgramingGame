#include "Module.hpp"
#include "GameWorld.hpp"

Module::Module() {}

void Module::addToGameWorld()
{
    GameWorld::instance()->addModule(this);
}

bool ModuleBuilder::buildModule() {
    if (newModule == nullptr) return false;
    ModuleNode* newModuleNode = newModule->getNode(newModuleNodeNumber);
    newModule->setTransforms(parentModuleNode, newModuleNode);

    for (auto module: GameWorld::instance()->getModules()) {
        if (module == parentModuleNode->parentModule) continue; // ignore collision with parent module
        if (module->checkBlockingAreaCollision(newModule)) {
            return false;
        }
    }

    parentModuleNode->attachedNode = newModuleNode;
    newModuleNode->attachedNode = parentModuleNode;
    GameWorld::instance()->addModule(newModule);
    newModule->addToGameWorld();
    newModule = nullptr;
    return true;
}

bool ModuleBuilder::selectModuleType(ModuleType type) {
    delete newModule;
    switch (type) {
    case ModuleType::Corridor:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(2);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addBitmap(GraphicsEngine::instance()->corridorModuleLayer0, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->corridorModuleLayer1, Vector2d(160, 160), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 8), Vector2d(16, 9)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -9), Vector2d(16, -8)));
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-16, -12), Vector2d(16, 12)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-16, -8), Vector2d(16, 8)));
        newModule = moduleSetup;
        break;
    }
    case ModuleType::JunctionX:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 16), Rotation(M_PI / 2));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 16), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->xModuleLayer0, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->xModuleLayer1, Vector2d(160, 160), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -16), Vector2d(-8, -8)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(8, -16), Vector2d(16, -8)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 8), Vector2d(-8, 16)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(8, 8), Vector2d(16, 16)));
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(16, 16), Vector2d(-16, -16)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(16, 16), Vector2d(-16, -16)));
        newModule = moduleSetup;
        break;
    }
    case ModuleType::Junction3:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(3);
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 3), 16), Rotation(M_PI / 3));
        moduleSetup->addNode(Vector2d(Rotation(-M_PI / 3), 16), Rotation(-M_PI / 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->junction3Layer0, Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->junction3Layer1, Vector2d(240, 240), CommonValues::zModuleWalls);
        // add collision
        newModule = moduleSetup;
        break;
    }
    case ModuleType::DeadendModule:
    {
        BasicModulePrototype* moduleSetup = new BasicModulePrototype(1);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addBitmap(GraphicsEngine::instance()->endLayer0, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
        moduleSetup->addBitmap(GraphicsEngine::instance()->endLayer1, Vector2d(160, 160), CommonValues::zModuleWalls);
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, 8), Vector2d(10, 9)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, -9), Vector2d(10, -8)));
        moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(11, -9), Vector2d(10, 9)));
        moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(16, 10), Vector2d(8, -10)));
        moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(16, 8), Vector2d(10, -8)));
        newModule = moduleSetup;
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
