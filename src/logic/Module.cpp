#include "Module.hpp"
#include "GameWorld.hpp"

Module::Module(GameWorld* aWorld): GameObject(aWorld) {}

bool ModuleBuilder::buildModule() {
    if (newModule == nullptr) return false;
    ModuleNode* newModuleNode = newModule->getNode(newModuleNodeNumber);
    newModule->setTransforms(parentModuleNode, newModuleNode);
    parentModuleNode->attachedNode = newModuleNode;
    newModuleNode->attachedNode = parentModuleNode;
    GameWorld::instance()->addModule(newModule);
    newModule = nullptr;
    return true;
}

bool ModuleBuilder::selectModuleType(ModuleType type) {
    delete newModule;
    switch (type) {
    case ModuleType::Corridor:
    {
        BasicModule* moduleSetup = new BasicModule(GameWorld::instance(), 2);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addBitmap(GraphicsEngine::instance()->corridorModuleLayer0, Vector2d(160, 160), CommonValues::zModuleWalls);
        moduleSetup->addBitmap(GraphicsEngine::instance()->corridorModuleLayer1, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
        newModule = moduleSetup;
        break;
    }
    case ModuleType::JunctionX:
    {
        BasicModule* moduleSetup = new BasicModule(GameWorld::instance(), 4);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 16), Rotation(M_PI / 2));
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 16), Rotation(M_PI / 2 * 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->xModuleLayer0, Vector2d(160, 160), CommonValues::zModuleWalls);
        moduleSetup->addBitmap(GraphicsEngine::instance()->xModuleLayer1, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
        newModule = moduleSetup;
        break;
    }
    case ModuleType::Junction3:
    {
        BasicModule* moduleSetup = new BasicModule(GameWorld::instance(), 3);
        moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
        moduleSetup->addNode(Vector2d(Rotation(M_PI / 3), 16), Rotation(M_PI / 3));
        moduleSetup->addNode(Vector2d(Rotation(-M_PI / 3), 16), Rotation(-M_PI / 3));
        moduleSetup->addBitmap(GraphicsEngine::instance()->junction3Layer0, Vector2d(240, 240), CommonValues::zModuleWalls);
        moduleSetup->addBitmap(GraphicsEngine::instance()->junction3Layer1, Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
        newModule = moduleSetup;
        break;
    }
    case ModuleType::DeadendModule:
    {
        BasicModule* moduleSetup = new BasicModule(GameWorld::instance(), 1);
        moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
        moduleSetup->addBitmap(GraphicsEngine::instance()->endLayer0, Vector2d(160, 160), CommonValues::zModuleWalls);
        moduleSetup->addBitmap(GraphicsEngine::instance()->endLayer1, Vector2d(160, 160), CommonValues::zModuleMainBackgroung);
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
