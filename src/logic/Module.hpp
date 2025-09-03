#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>
#include "../common/common.hpp"
#include "GameObject.hpp"
#include "GuiEngine.hpp"

class GuiEngine;
class GameWorld;
class Module;

struct ModuleNode
{
    Vector2d position;
    Rotation rotation;
    ModuleNode *attachedNode = nullptr;
    Module *parentModule;
};

class Module : public GameObject
{
protected:
    std::vector<ModuleNode> nodes;
    Vector2d position;
    Rotation rotation;
    Module();

public:
    virtual void draw()
    {
        GraphicsEngine::instance()->drawPoint(position, 0, al_map_rgb(255, 255, 255));
        for (auto node : nodes)
        {
            GraphicsEngine::instance()->drawPoint(node.position.rotate(rotation) + position, 0, al_map_rgb(0, 0, 255));
            GraphicsEngine::instance()->drawLine(node.position.rotate(rotation) + position, node.position.rotate(rotation) + position + Vector2d(rotation + node.rotation, 5), 0, al_map_rgb(0, 0, 255));
        }
    };

    void setTransforms(Vector2d aPos, Rotation aRot)
    {
        position = aPos;
        rotation = aRot;
    }

    void setTransforms(ModuleNode *parentNode, ModuleNode *ownNode)
    {
        Module *parentModule = parentNode->parentModule;
        rotation = parentModule->rotation + parentNode->rotation - ownNode->rotation + M_PI;
        position = parentModule->position + parentNode->position.rotate(parentModule->rotation) + ownNode->position.rotate(parentModule->rotation + parentNode->rotation - ownNode->rotation);
    }

    ModuleNode *getNode(int number)
    {
        if (number < nodes.size())
        { // TODO should be ok since nodes size wont change
            return &nodes.at(number);
        }
        return nullptr;
    }

    std::vector<ModuleNode *> getNodes()
    {
        std::vector<ModuleNode *> result;
        for (int i = 0; i < nodes.size(); i++)
        {
            result.push_back(&nodes.at(i));
        }

        return result;
    }

    Vector2d getPosition()
    {
        return position;
    }

    Rotation getRotation()
    {
        return rotation;
    }

    void addToGameWorld() override;
};

// TODO
// class ModulesManager {
//     std::vector<Module> modules;

// public:
//     static ModulesManager* instance() {
//         static ModulesManager instance;
//         return &instance;
//     }
// };

class BasicModule: public Module {
    int nodesNumber = 0;
    struct ModuleSprite {
        Vector2d pivot;
        double z;
        ALLEGRO_BITMAP* bitmap;
    };
    std::vector<ModuleSprite> sprites;

public:
    BasicModule(int nodesNumber) : Module(), nodesNumber(nodesNumber) {
        assert(nodesNumber > 0);
        nodes.reserve(nodesNumber);
    }

    void addNode(Vector2d pos, Rotation rot) {
        assert(nodes.size() < nodesNumber);
        ModuleNode newNode;
        newNode.position = pos;
        newNode.rotation = rot;
        newNode.parentModule = this;
        nodes.push_back(newNode);
    }

    void addBitmap(ALLEGRO_BITMAP* bitmap, Vector2d pivot, double z) {
        sprites.push_back(ModuleSprite{});
        sprites.back().bitmap = bitmap;
        sprites.back().pivot = pivot;
        sprites.back().z = z;
    }

    void draw() {
        Module::draw();
        for (auto sprite: sprites) {
            GraphicsEngine::instance()->drawBitmap(position, sprite.bitmap, 10, sprite.z, sprite.pivot, rotation);
        }
    }
};

enum ModuleType
{
    Corridor,
    JunctionX,
    Junction3,
    DeadendModule
};

class ModuleBuilder
{
    ModuleNode *parentModuleNode;
    int newModuleNodeNumber;
    Module *newModule = nullptr;

    Window *window;
    std::vector<Button *> nodeNumberButtons;
    std::vector<Label *> nodeNumberLabels;

    void updateNodeNumberSelection()
    {
        for (auto button : nodeNumberButtons)
        {
            window->deleteButton(button);
        }
        nodeNumberButtons.clear();

        for (auto label : nodeNumberLabels)
        {
            window->deleteLabel(label);
        }
        nodeNumberLabels.clear();

        int i = 0;
        for (auto node : newModule->getNodes())
        {
            Button *newButton = window->addButton(Rect2d::fromTwoCorners(Vector2d(220, 60 + i * 25), Vector2d(420, 80 + i * 25)));
            Label *newLabel = window->addLabel(newButton->getRect().center(), true, std::to_string(i), 0);
            newButton->setOnClickCallback([this, n = i]()
                                          { this->selectNewNodeNumber(n); });
            nodeNumberButtons.push_back(newButton);
            nodeNumberLabels.push_back(newLabel);
            i++;
        }
    }

    void onWindowClose()
    {
        window = nullptr;
        nodeNumberButtons.clear();
        nodeNumberLabels.clear();
    }

public:
    static ModuleBuilder *instance()
    {
        static ModuleBuilder instance;
        return &instance;
    }

    void setParentNode(ModuleNode *node)
    {
        parentModuleNode = node;
    }

    // true - if success
    bool buildModule();

    bool selectModuleType(ModuleType);

    bool selectNewNodeNumber(int number);

    void createWindow()
    {
        if (window)
            GuiEngine::instance()->closeWindow(window);
        if (parentModuleNode->attachedNode != nullptr)
            return;
        window = GuiEngine::instance()->addWindow(Rect2d(Vector2d(400, 400), 400, 440), true, true);
        window->setOnCloseCallback([this]()
                                   { onWindowClose(); });

        Button *createButton = window->addButton(Rect2d(Vector2d(10, 340), Vector2d(210, 390)));
        window->addLabel(createButton->getRect().center(), true, "Create module", 0);
        createButton->setOnClickCallback([this, &window = window]()
                                         {
            bool result = this->buildModule();
            if (result) GuiEngine::instance()->closeWindow(window); });

        window->addLabel(Vector2d(20, 40), false, "Module type:", 0);
        window->addLabel(Vector2d(220, 40), false, "Node number:", 0);

        Button *corridorButton = window->addButton(Rect2d(Vector2d(20, 60), Vector2d(200, 80)));
        window->addLabel(corridorButton->getRect().center(), true, "Corridor", 0);
        corridorButton->setOnClickCallback([this](){selectModuleType(Corridor); });

        Button *xCorridorButton = window->addButton(Rect2d(Vector2d(20, 85), Vector2d(200, 105)));
        window->addLabel(xCorridorButton->getRect().center(), true, "X Corridor", 0);
        xCorridorButton->setOnClickCallback([this](){selectModuleType(JunctionX); });

        Button *junction3Button = window->addButton(Rect2d(Vector2d(20, 110), Vector2d(200, 130)));
        window->addLabel(junction3Button->getRect().center(), true, "3 way junction", 0);
        junction3Button->setOnClickCallback([this](){selectModuleType(Junction3); });

        Button *deadendButton = window->addButton(Rect2d(Vector2d(20, 135), Vector2d(200, 155)));
        window->addLabel(deadendButton->getRect().center(), true, "Deadend", 0);
        deadendButton->setOnClickCallback([this](){selectModuleType(DeadendModule); });

        selectModuleType(Corridor);
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_