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
#include <collision.h>
#include <fstream>
#include <nlohmann/json.hpp>

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

struct PolygonalArea {
    std::vector<Vector2d> initialVerticies;
    std::vector<Vector2d> transformedVerticies;
};

class Module : public GameObject
{
protected:
    std::vector<ModuleNode> nodes;
    Vector2d position;
    Rotation rotation;
    Module();
    std::vector<PolygonalArea*> walls;
    std::vector<PolygonalArea*> buildableAreas;
    std::vector<PolygonalArea*> blockingAreas;

public:
    virtual void drawInfo() {
        for (auto node : nodes) {
            if (node.attachedNode == nullptr)
                GraphicsEngine::instance()->drawCircle(node.position.rotate(rotation) + position, 0.5, CommonValues::zDebug, al_map_rgba(255, 255, 0, 100), 3);
        }
    }

    virtual void drawDebug() {
        GraphicsEngine::instance()->drawPoint(position, 0, al_map_rgb(255, 255, 255));
        for (auto wall: walls) {
            GraphicsEngine::instance()->drawPolygon(wall->transformedVerticies, -0.001, al_map_rgb(255, 0, 0));
        }

        for (auto area: blockingAreas) {
            GraphicsEngine::instance()->drawPolygon(area->transformedVerticies, -0.001, al_map_rgba(100, 0, 0, 40));
        }

        for (auto area: buildableAreas) {
            GraphicsEngine::instance()->drawPolygon(area->transformedVerticies, -0.001, al_map_rgba(0, 100, 100, 30));
        }

        for (auto node : nodes) {
            GraphicsEngine::instance()->drawPoint(node.position.rotate(rotation) + position, CommonValues::zDebug, al_map_rgb(0, 0, 255));
            GraphicsEngine::instance()->drawLine(node.position.rotate(rotation) + position, node.position.rotate(rotation) + position + Vector2d(rotation + node.rotation, 5), CommonValues::zDebug, al_map_rgb(0, 0, 255));
        }
    }

    virtual void draw() {};

    void setTransforms(Vector2d aPos, Rotation aRot)
    {
        position = aPos;
        rotation = aRot;
        for (auto wall: walls) {
            wall->transformedVerticies.clear();
            for (auto dot: wall->initialVerticies) {
                wall->transformedVerticies.push_back(position + dot.rotate(rotation));
            }
        }

        for (auto area: buildableAreas) {
            area->transformedVerticies.clear();
            for (auto dot: area->initialVerticies) {
                area->transformedVerticies.push_back(position + dot.rotate(rotation));
            }
        }

        for (auto area: blockingAreas) {
            area->transformedVerticies.clear();
            for (auto dot: area->initialVerticies) {
                area->transformedVerticies.push_back(position + dot.rotate(rotation));
            }
        }

        
    }

    void setTransforms(ModuleNode *parentNode, ModuleNode *ownNode)
    {
        Module *parentModule = parentNode->parentModule;
        Rotation newRot = parentModule->rotation + parentNode->rotation - ownNode->rotation + M_PI;
        Vector2d newPos = parentModule->position + parentNode->position.rotate(parentModule->rotation) + ownNode->position.rotate(parentModule->rotation + parentNode->rotation - ownNode->rotation);
        setTransforms(newPos, newRot);
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

    bool checkWallCollision(Rect2d rect) {
        collision::Polygon rectangle({
            collision::fvec2(rect.p1.x, rect.p1.y),
            collision::fvec2(rect.p2.x, rect.p1.y),
            collision::fvec2(rect.p2.x, rect.p2.y),
            collision::fvec2(rect.p1.x, rect.p2.y)
        });

        collision::GJKCollisionDetector detector;
        std::vector<collision::fvec2> dots; 
        for (auto wall: walls) {

            dots.clear();
            for (auto dot: wall->transformedVerticies) {
                dots.push_back(collision::fvec2(dot.x, dot.y));
            }
            
            collision::Polygon ownPolygon(dots);
            if (detector.detect(rectangle, ownPolygon)) return true;
        }
        
        return false;
    }

    bool checkBlockingAreaCollision(Module* other) {
        collision::GJKCollisionDetector detector;
        std::vector<collision::fvec2> dots; 
        for (auto otherArea: other->blockingAreas) {
            dots.clear();
            for (auto dot: otherArea->transformedVerticies) {
                dots.push_back(collision::fvec2(dot.x, dot.y));
            }
            collision::Polygon otherPolygon(dots);

            for (auto ownArea: blockingAreas) {

                dots.clear();
                for (auto dot: ownArea->transformedVerticies) {
                    dots.push_back(collision::fvec2(dot.x, dot.y));
                }
            
                collision::Polygon ownPolygon(dots);
                if (detector.detect(otherPolygon, ownPolygon)) return true;
            }
        }
        return false;
    }

    bool checkTouchesBuildableArea(Rect2d rect) {
        collision::Polygon rectangle({
            collision::fvec2(rect.p1.x, rect.p1.y),
            collision::fvec2(rect.p2.x, rect.p1.y),
            collision::fvec2(rect.p2.x, rect.p2.y),
            collision::fvec2(rect.p1.x, rect.p2.y)
        });

        collision::GJKCollisionDetector detector;
        std::vector<collision::fvec2> dots; 
        for (auto area: buildableAreas) {

            dots.clear();
            for (auto dot: area->transformedVerticies) {
                dots.push_back(collision::fvec2(dot.x, dot.y));
            }
            
            collision::Polygon ownPolygon(dots);
            if (detector.detect(rectangle, ownPolygon)) return true;
        }
        
        return false;
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

class BasicModulePrototype: public Module {
    int nodesNumber = 0;
    struct ModuleSprite {
        Vector2d pivot;
        double z;
        ALLEGRO_BITMAP* bitmap;
    };
    std::vector<ModuleSprite> sprites;

public:
    BasicModulePrototype(int nodesNumber) : Module(), nodesNumber(nodesNumber) {
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

    void addWall(Rect2d rect) {
        auto wall = new PolygonalArea{};
        wall->initialVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
        wall->initialVerticies.push_back(rect.p2);
        wall->initialVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
        wall->initialVerticies.push_back(rect.p1);
        
        wall->transformedVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
        wall->transformedVerticies.push_back(rect.p2);
        wall->transformedVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
        wall->transformedVerticies.push_back(rect.p1);
        
        walls.push_back(wall);
    }

    void addBuildableArea(Rect2d rect) {
        auto area = new PolygonalArea{};
        area->initialVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
        area->initialVerticies.push_back(rect.p2);
        area->initialVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
        area->initialVerticies.push_back(rect.p1);
        
        area->transformedVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
        area->transformedVerticies.push_back(rect.p2);
        area->transformedVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
        area->transformedVerticies.push_back(rect.p1);
        
        buildableAreas.push_back(area);
    }

    void addBlockingArea(Rect2d rect) {
        auto area = new PolygonalArea{};
        area->initialVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
        area->initialVerticies.push_back(rect.p2);
        area->initialVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
        area->initialVerticies.push_back(rect.p1);
        
        area->transformedVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
        area->transformedVerticies.push_back(rect.p2);
        area->transformedVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
        area->transformedVerticies.push_back(rect.p1);
        
        blockingAreas.push_back(area);
    }

    void draw() {
        Module::draw();
        for (auto sprite: sprites) {
            GraphicsEngine::instance()->drawBitmap(position, sprite.bitmap, 20, sprite.z, sprite.pivot, rotation);
        }
    }
};

enum ModuleType
{
    Corridor,
    ConnectorCross,
    Connector3,
    ConnectorT,
    Deadend,
    Frame,
    FrameCross,
    Frame3,
    LargeModule,
    SolarPanel,
    Antena
};

class ModuleBuilder
{
    ModuleNode *parentModuleNode;
    int newModuleNodeNumber;
    Module *modulePrototype = nullptr;

    Window *window;
    std::vector<Button *> nodeNumberButtons;
    std::vector<Label *> nodeNumberLabels;

    void updateNodeNumberSelection()
    {
        if (!window) return;
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
        for (auto node : modulePrototype->getNodes())
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
    bool buildModule(bool initial = false);

    bool createModulePrototype(ModuleType);

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

        createModuleSelectionButtons();

        createModulePrototype(Corridor);
    }
    void createModuleSelectionButtons()
    {
        std::string labels[] = {"Corridor", "Cross Connector", "3 Way Connector", "T Connector", "Deadend", "Frame", "Square Frame", "Triangle Frame", "Utility Module", "Solar Panel", "Antena array"};
        ModuleType types[] = {Corridor, ConnectorCross, Connector3, ConnectorT, Deadend, Frame, FrameCross, Frame3, LargeModule, SolarPanel, Antena};
        int n = 11;

        for (int i = 0; i < n; i++) {
            Button *corridorButton = window->addButton(Rect2d(Vector2d(20, 60) + Vector2d(0, i * 25), Vector2d(200, 80) + Vector2d(0, i * 25)));
            window->addLabel(corridorButton->getRect().center(), true, labels[i], 0);
            corridorButton->setOnClickCallback([this, type = types[i]](){ createModulePrototype(type); });
        }
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_