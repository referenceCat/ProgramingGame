#include "GameWorld.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include <format>

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
    GraphicsEngine::instance()->drawRectangle(Rect2d(Vector2d(-500, surfaceY + 10), Vector2d(500, surfaceY + 300)), -1, al_map_rgb(5, 5, 5));
    for (int i = -30; i < 30; i++) {
        if (i % 5 == 0) {
            GraphicsEngine::instance()->drawBitmap(Vector2d(i * 40, surfaceY), GraphicsEngine::instance()->getBitmap("resources/assets/environment/surface/tile2/main.png"), 10, -1);
        // } else if ((i + 3)% 5 == 0) {
        //     GraphicsEngine::instance()->drawBitmap(Vector2d(i * 40, surfaceY), GraphicsEngine::instance()->getBitmap("resources/assets/environment/surface/tile1/main.png"), 10, -1);
        } else {
            GraphicsEngine::instance()->drawBitmap(Vector2d(i * 40, surfaceY), GraphicsEngine::instance()->getBitmap("resources/assets/environment/surface/tile0/main.png"), 10, -1);
        }
        
    }

    for (int i = -5; i < 5; i++) {
        GraphicsEngine::instance()->drawBitmap(Vector2d(i * 200, surfaceY + 25), GraphicsEngine::instance()->getBitmap(std::format("resources/assets/environment/surface/plains{}/main.png", (i + 9) % 3)), 6, 10,  Vector2d(600,  400));
        GraphicsEngine::instance()->drawBitmap(Vector2d(i * 300 - 100, surfaceY + 30), GraphicsEngine::instance()->getBitmap(std::format("resources/assets/environment/surface/plains{}/background.png", (i + 8) % 3)), 4, 20,  Vector2d(600,  400));
        
    }

    

    // GraphicsEngine::instance()->drawBitmap(Vector2d(200, surfaceY + 50), GraphicsEngine::instance()->getBitmap("resources/assets/environment/mountains/peaks.png"), 5, 40, Vector2d(600,  1200));
    // GraphicsEngine::instance()->drawBitmap(Vector2d(-100, surfaceY + 20), GraphicsEngine::instance()->getBitmap("resources/assets/environment/mountains/peaks.png"), 15, 25, Vector2d(600,  1200));
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
