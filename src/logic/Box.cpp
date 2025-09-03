#include <Box.hpp>
#include <GameWorld.hpp>

void Box::addToGameWorld() {
    GameWorld::instance()->addBox(this);
}