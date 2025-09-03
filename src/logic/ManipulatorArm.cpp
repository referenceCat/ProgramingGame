#include "ManipulatorArm.hpp"
#include "GameWorld.hpp"

void ManipulatorArm::grab() // TODO what if box is already grabbed?
{
    active = true;
    Box* box = GameWorld::instance()->getBox(jointsPosition[jointsNumber - 1]);
    if (box) {
        takeBox(box);
    }
}

void ManipulatorArm::addToGameWorld()
{
    GameWorld::instance()->addManipulatorArm(this);
}
