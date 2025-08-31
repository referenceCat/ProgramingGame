#include "Module.hpp"
#include "GameWorld.hpp"

Module::Module(GameWorld* aWorld): GameObject(aWorld) {
    aWorld->addModule(this);
 }