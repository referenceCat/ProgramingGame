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
    Vector2d pos;
    Rotation rot;
    ModuleNode* attachedNode = nullptr;
    Module* parentModule;
};

class Module : public GameObject {
protected:
    std::vector<ModuleNode> nodes;
    Vector2d pos;
    Rotation rot;
    Module();
    std::vector<PolygonalArea> walls;
    std::vector<PolygonalArea> buildableAreas;
    std::vector<PolygonalArea> blockingAreas;

public:
    virtual void drawInfo();
    virtual void drawDebug();
    virtual void draw() {};
    void setTransforms(Vector2d aPos, Rotation aRot);
    void setTransforms(ModuleNode* parentNode, ModuleNode* ownNode);
    ModuleNode* getNode(int number);
    std::vector<ModuleNode*> getNodes();

    Vector2d getPos() {
        return pos;
    }

    Rotation getRot() {
        return rot;
    }

    void addToGameWorld() override;
    bool checkWallCollision(Rect2d rect);
    bool checkBlockingAreaCollision(Module* other);
    bool checkTouchesBuildableArea(Rect2d rect);
    virtual nlohmann::json toJson();
    static Module* fromJson(nlohmann::json);
};

class BasicModule : public Module {
    int nodesNumber = 0;
    AbstractDrawable* drawable = nullptr;
    DataId dataId = 0; // e.g. Corridor, Large Module, etc

    struct ModuleSprite {
        Vector2d pivot;
        double z;
        ALLEGRO_BITMAP* bitmap;
    };

    std::vector<ModuleSprite> sprites;

public:
    BasicModule(int nodesNumber);
    static BasicModule* initializeFromJson(nlohmann::json data);
    static BasicModule* fromJson(nlohmann::json);
    void addNode(Vector2d pos, Rotation rot);
    void addBitmap(ALLEGRO_BITMAP* bitmap, Vector2d pivot, double z);
    void addWall(Rect2d rect);
    void addBuildableArea(Rect2d rect);
    void addBlockingArea(Rect2d rect);
    void draw();
    virtual nlohmann::json toJson() override;
    void setDrawable(AbstractDrawable* aDrawable) {
        drawable = aDrawable;
    };
};

class ModulesData {
    std::map<DataId, nlohmann::json> data;

public:
    std::vector<DataId> getAllModuleDataIds() {
        std::vector<DataId> ids;
        for (auto const& module : data)
            ids.push_back(module.first);
        return ids;
    }

    nlohmann::json getModuleJsonData(DataId id) {
        return data[id];
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
            uint32_t DataId = moduleData["id"].get<int>();
            data[DataId] = moduleData;
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
    bool createModulePrototype(DataId dataId);
    bool selectNewNodeNumber(int number);
    void createWindow();
    void createModuleSelectionButtons();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MODULE_HPP_