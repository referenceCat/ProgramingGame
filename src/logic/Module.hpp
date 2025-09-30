#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_

#include <assert.h>
#include "common.hpp"
#include "GameObject.hpp"
#include "GuiEngine.hpp"
#include "collision.h"
// #include <nlohmann/json.hpp>

class GuiEngine;
class GameWorld;
class Module;

struct ModuleNode {
    Vector2d position;
    Rotation rotation;
    ModuleNode* attachedNode = nullptr;
    Module* parentModule;
};

struct PolygonalArea {
    std::vector<Vector2d> initialVerticies;
    std::vector<Vector2d> transformedVerticies;
};

class Module : public GameObject {
protected:
    std::vector<ModuleNode> nodes;
    Vector2d position;
    Rotation rotation;
    Module();
    std::vector<PolygonalArea*> walls;
    std::vector<PolygonalArea*> buildableAreas;
    std::vector<PolygonalArea*> blockingAreas;

public:
    virtual void drawInfo();
    virtual void drawDebug();
    virtual void draw() {};
    void setTransforms(Vector2d aPos, Rotation aRot);
    void setTransforms(ModuleNode* parentNode, ModuleNode* ownNode);
    ModuleNode* getNode(int number);
    std::vector<ModuleNode*> getNodes();

    Vector2d getPosition() {
        return position;
    }

    Rotation getRotation() {
        return rotation;
    }

    void addToGameWorld() override;
    bool checkWallCollision(Rect2d rect);
    bool checkBlockingAreaCollision(Module* other);
    bool checkTouchesBuildableArea(Rect2d rect);
};

class BasicModulePrototype : public Module {
    int nodesNumber = 0;

    struct ModuleSprite {
        Vector2d pivot;
        double z;
        ALLEGRO_BITMAP* bitmap;
    };

    std::vector<ModuleSprite> sprites;

public:
    BasicModulePrototype(int nodesNumber);
    void addNode(Vector2d pos, Rotation rot);
    void addBitmap(ALLEGRO_BITMAP* bitmap, Vector2d pivot, double z);
    void addWall(Rect2d rect);
    void addBuildableArea(Rect2d rect);
    void addBlockingArea(Rect2d rect);
    void draw();
};

enum ModuleType // TODO maybe not
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
    Antena,
    FrameFoundation
};

class ModuleBuilder {
    ModuleNode* parentModuleNode;
    int newModuleNodeNumber;
    Module* modulePrototype = nullptr;

    LegacyWindow* window;
    std::vector<LegacyButton*> nodeNumberButtons;
    std::vector<LegacyLabel*> nodeNumberLabels;

    void updateNodeNumberSelection();
    void onWindowClose();

public:
    static ModuleBuilder* instance();
    void setParentNode(ModuleNode* node);
    bool buildModule(bool initial = false); // returns true on success
    bool createModulePrototype(ModuleType);
    bool selectNewNodeNumber(int number);
    void createWindow();
    void createModuleSelectionButtons();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_