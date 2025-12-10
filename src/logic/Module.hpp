#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_

#include <assert.h>
#include "types.hpp"
#include "GameObject.hpp"
#include "GuiEngine.hpp"
#include "collision.h"
#include <nlohmann/json.hpp>
#include "GraphicsEngine.hpp"
#include <fstream>

class GuiEngine;
class GameWorld;
class Module;

struct ModuleNode {
    Vector2d position;
    Rotation rot;
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
    Rotation rot;
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

    Vector2d getPos() {
        return position;
    }

    Rotation getRot() {
        return rot;
    }

    void addToGameWorld() override;
    bool checkWallCollision(Rect2d rect);
    bool checkBlockingAreaCollision(Module* other);
    bool checkTouchesBuildableArea(Rect2d rect);
};

class BasicModule : public Module {
    int nodesNumber = 0;
    AbstractDrawable* drawable = nullptr;

    struct ModuleSprite {
        Vector2d pivot;
        double z;
        ALLEGRO_BITMAP* bitmap;
    };

    std::vector<ModuleSprite> sprites;

public:
    BasicModule(int nodesNumber);
    static BasicModule* fromJson(nlohmann::json data);
    void addNode(Vector2d pos, Rotation rot);
    void addBitmap(ALLEGRO_BITMAP* bitmap, Vector2d pivot, double z);
    void addWall(Rect2d rect);
    void addBuildableArea(Rect2d rect);
    void addBlockingArea(Rect2d rect);
    void draw();

    void setDrawable(AbstractDrawable* aDrawable) {
        drawable = aDrawable;
    };
};

class ModulesData {
    std::map<std::string, nlohmann::json> data;

public:
    std::vector<std::string> getAllModuleNames() {
        std::vector<std::string> names;
        for (auto const& module : data)
            names.push_back(module.first);
        return names;
    }

    nlohmann::json getModuleJsonData(std::string name) {
        return data[name];
    }

    static ModulesData& instance() {
        static ModulesData inst{};
        return inst;
    }

    void initialize() {
        std::ifstream f("resources/data/modules.json");
        nlohmann::json json = nlohmann::json::parse(f);
        f.close();
        for (auto moduleData: json) {
            std::string name = moduleData["name"].get<std::string>();
            data[name] = moduleData;
        }
    }
};

class ModuleBuilder {

    ModuleNode* parentModuleNode;
    int newModuleNodeNumber;
    Module* modulePrototype = nullptr;

    Window* window;
    NamedArea* nodeSelectionZone;
    std::vector<Button*> nodeNumberButtons;
    std::vector<Label*> nodeNumberLabels;

    void updateNodeNumberSelection();
    void onWindowClose();

public:
    static ModuleBuilder* instance();
    void setParentNode(ModuleNode* node);
    bool buildModule(bool initial = false); // returns true on success
    bool createModulePrototype(std::string name);
    bool selectNewNodeNumber(int number);
    void createWindow();
    void createModuleSelectionButtons();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_