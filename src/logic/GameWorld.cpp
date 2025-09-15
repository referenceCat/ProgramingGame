#include "GameWorld.hpp"
#include "../graphics/GraphicsEngine.hpp"

void GameWorld::addManipulatorArm(ManipulatorArm *arm)
{
    arms.push_back(arm);
}

void GameWorld::addBox(Box *box)
{
    boxes.push_back(box);
}

void GameWorld::addMachinery(Machinery *machinery)
{
    machines.push_back(machinery);
}

void GameWorld::addModule(Module *module)
{
    modules.push_back(module);
}

void GameWorld::removeBox(int id)
{
    Box *boxToRemove = getBox(id);
    for (auto item : arms)
    {
        item->removeBox(boxToRemove);
    }
    delete boxToRemove;
    boxes.erase(std::remove(boxes.begin(), boxes.end(), boxToRemove), boxes.end());
}

ManipulatorArm *GameWorld::getManipulatorArm(int id)
{
    for (auto item : arms)
    {
        if (item->getId() == id)
            return item;
    }
    return nullptr;
}

Machinery *GameWorld::getMachinery(int id)
{
    for (auto item : machines)
    {
        if (item->getId() == id)
            return item;
    }
    return nullptr;
}

std::vector<Machinery *> GameWorld::getMachinery()
{
    return machines;
}

Module *GameWorld::getModule(int id)
{
    for (auto item : modules)
    {
        if (item->getId() == id)
            return item;
    }
    return nullptr;
}

std::vector<Box *> GameWorld::getBoxes()
{
    std::vector<Box *> result = boxes;
    return result;
}

std::vector<Module *> GameWorld::getModules()
{
    std::vector<Module *> result = modules;
    return result;
}

Box *GameWorld::getBox(int id)
{
    for (auto item : boxes)
    {
        if (item->getId() == id)
            return item;
    }
    return nullptr;
}

Box *GameWorld::getBox(Vector2d aPos)
{
    for (auto item : boxes)
    {
        if (item->getRect().isInside(aPos))
            return item;
    }
    return nullptr;
}

void GameWorld::drawSurface() {
    GraphicsEngine::instance()->drawRectangle(Rect2d(Vector2d(-500, surfaceY + 3), Vector2d(500, surfaceY + 100)), CommonValues::zSurface, al_map_rgb(15, 15, 15));
    for (int i = -30; i < 30; i++) {
        GraphicsEngine::instance()->drawBitmap(Vector2d(i * 10, surfaceY), GraphicsEngine::instance()->getBitmap("resources/assets/environment/surface/tile0.png"), 20, CommonValues::zSurface);
    }
}

void GameWorld::drawAll(bool info = false, bool debug = true)
{
    for (auto item : arms)
    {
        item->draw();
        if (info)
            item->drawInfo();
        if (debug)
            item->drawDebug();
    }

    for (auto item : boxes)
    {
        item->draw();
    }

    for (auto item : machines)
    {
        item->draw();
        if (info)
            item->drawInfo();
        if (debug)
            item->drawDebug();
    }

    for (auto item : modules)
    {
        item->draw();
        if (info)
            item->drawInfo();
        if (debug)
            item->drawDebug();
    }
    drawSurface();
}

void GameWorld::run()
{
    // run arms
    for (auto arm : arms)
    {
        arm->moveToTarget();
        arm->recalculate();
    }

    // run machinery
    for (auto machinery : machines)
    {
        machinery->run();
    }

    // run boxes
    for (auto box : boxes)
    {
        box->update();
    }
}
void GameWorld::click(Vector2d point)
{
    for (auto machinery : machines)
    {
        if (machinery->getRect().isInside(point))
        {
            machinery->onClick();
            return;
        }
    }
}
