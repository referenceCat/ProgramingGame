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
        newModule = new CorridorModule(GameWorld::instance());
        break;
    case ModuleType::JunctionX:
        newModule = new XCorridorModule(GameWorld::instance());
        break;
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
