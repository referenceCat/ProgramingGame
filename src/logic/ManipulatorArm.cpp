#include "ManipulatorArm.hpp"
#include "GameWorld.hpp"

void ManipulatorArm::grab()
{
    active = true;
    Box* box = world->getBox(jointsPosition[jointsNumber - 1]);
    takeBox(box);
}