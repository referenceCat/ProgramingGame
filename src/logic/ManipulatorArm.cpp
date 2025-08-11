#include "ManipulatorArm.hpp"
#include "GameWorld.hpp"

void ManipulatorArm::grab()
{
    active = true;
    Box* box = GameObject::parentWorld->getBox(jointsPosition[jointsNumber - 1]);
    takeBox(box);
}