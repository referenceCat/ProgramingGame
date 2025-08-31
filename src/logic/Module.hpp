#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include  <algorithm>
#include "../common/common.hpp"
#include "GameObject.hpp"

class GameWorld;
class Module;

struct ModuleNode {
    Vector2d position;
    Rotation rotation;
    ModuleNode* attachedNode = nullptr;
    Module* parentModule;
};

class Module : public GameObject {
protected:
    std::vector<ModuleNode> nodes;
    Vector2d position;
    Rotation rotation;
    Module (GameWorld* gameWorld);

public:
    virtual void draw() {
        GraphicsEngine::instance()->drawPoint(position, 0, al_map_rgb(255, 255, 255));
        for (auto node: nodes) {
            GraphicsEngine::instance()->drawPoint(node.position.rotate(rotation) + position, 0, al_map_rgb(0, 0, 255));
            GraphicsEngine::instance()->drawLine(node.position.rotate(rotation) + position, node.position.rotate(rotation) + position + Vector2d(rotation + node.rotation, 15), 0, al_map_rgb(0, 0, 255));
        }
    };

    void setTransforms(Vector2d aPos, Rotation aRot) {
        position = aPos;
        rotation = aRot;
    }

    void setTransforms(ModuleNode* parentNode, ModuleNode* ownNode) {
        Module* parentModule = parentNode->parentModule;
        rotation = parentModule->rotation + parentNode->rotation + ownNode->rotation + M_PI;
        position = parentModule->position + parentNode->position.rotate(parentModule->rotation) + ownNode->position.rotate(ownNode->rotation + parentModule->rotation + parentNode->rotation);
    }

    ModuleNode* getNode(int number) {
        if (number < nodes.size()) { // TODO should be ok since nodes size wont change
            return &nodes.at(number);
        }
        return nullptr;
    }

    std::vector<ModuleNode *> getNodes() {
        std::vector<ModuleNode *> result;
        for (int i = 0; i < nodes.size(); i++) {
            result.push_back(&nodes.at(i));
        }

        return result;
    }

    Vector2d getPosition() {
        return position;
    }
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

class CorridorModule: public Module {
public:
    CorridorModule(GameWorld* aWorld): Module(aWorld) {
        nodes.reserve(2);
        ModuleNode leftNode;
        leftNode.position = Vector2d(-160, 0);
        leftNode.rotation = Rotation(M_PI);
        leftNode.parentModule = this;
        nodes.push_back(leftNode);

        ModuleNode rightNode;
        rightNode.position = Vector2d(160, 0);
        rightNode.rotation = Rotation(0);
        rightNode.parentModule = this;
        nodes.push_back(rightNode);
    }

    void draw() override {
        Module::draw();
        GraphicsEngine* graphicsEngine = GraphicsEngine::instance();
        graphicsEngine->drawBitmap(position, graphicsEngine->corridorModuleLayer0, 5, Vector2d(160, 160), rotation);
        graphicsEngine->drawBitmap(position, graphicsEngine->corridorModuleLayer1, -2, Vector2d(160, 160), rotation);
    }
};

class XCorridorModule: public Module {
public:
    XCorridorModule(GameWorld* aWorld): Module(aWorld) {
        nodes.reserve(4);
        ModuleNode leftNode;
        leftNode.position = Vector2d(-160, 0);
        leftNode.rotation = Rotation(M_PI);
        leftNode.parentModule = this;
        nodes.push_back(leftNode);

        ModuleNode rightNode;
        rightNode.position = Vector2d(160, 0);
        rightNode.rotation = Rotation(0);
        rightNode.parentModule = this;
        nodes.push_back(rightNode);

        ModuleNode downNode;
        downNode.position = Vector2d(0, 160);
        downNode.rotation = Rotation(M_PI / 2);
        downNode.parentModule = this;
        nodes.push_back(downNode);

        ModuleNode upNode;
        upNode.position = Vector2d(0, -160);
        upNode.rotation = Rotation(M_PI * 3 / 2);
        upNode.parentModule = this;
        nodes.push_back(upNode);
    }

    void draw() override {
        Module::draw();
        GraphicsEngine* graphicsEngine = GraphicsEngine::instance();
        graphicsEngine->drawBitmap(position, graphicsEngine->xModuleLayer0, 5, Vector2d(160, 160), rotation);
        graphicsEngine->drawBitmap(position, graphicsEngine->xModuleLayer1, -2, Vector2d(160, 160), rotation);
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_