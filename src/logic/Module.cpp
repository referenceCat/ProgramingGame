#include "Module.hpp"
#include "GameWorld.hpp"

Module::Module(GameWorld* aWorld): GameObject(aWorld) {
    aWorld->addModule(this);
 }

bool ModuleBuilder::buildModule() {
    Module* newModule = new CorridorModule(GameWorld::instance());
    ModuleNode* newModuleNode = newModule->getNode(newModuleNodeNumber);
    newModule->setTransforms(parentModuleNode, newModuleNode);
    parentModuleNode->attachedNode = newModuleNode;
    newModuleNode->attachedNode = parentModuleNode;
    return true;
}