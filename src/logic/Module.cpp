#include "Module.hpp"
#include "GameWorld.hpp"
#include "GraphicsEngine.hpp"

Module::Module() {
}

void Module::addToGameWorld() {
    GameWorld::instance()->addModule(this);
}

bool ModuleBuilder::buildModule(bool initial) {
    if (modulePrototype == nullptr)
        return false;

    if (initial) {
        modulePrototype->setTransforms(Vector2d(0, -20), Rotation::fromDegrees(90)); // TODO
    } else {
        ModuleNode* newModuleNode = modulePrototype->getNode(newModuleNodeNumber);
        modulePrototype->setTransforms(parentModuleNode, newModuleNode);
        for (auto module : GameWorld::instance()->getModules()) {
            // TODO not sure do i need this
            // if (module == parentModuleNode->parentModule) continue; // ignore collision with parent module
            if (module->checkBlockingAreaCollision(modulePrototype)) {
                return false;
            }
        }
        parentModuleNode->attachedNode = newModuleNode;
        newModuleNode->attachedNode = parentModuleNode;
    }

    // GameWorld::instance()->addModule(modulePrototype);
    modulePrototype->addToGameWorld();
    modulePrototype = nullptr;
    return true;
}

bool ModuleBuilder::createModulePrototype(DataId dataId) {
    delete modulePrototype;
    modulePrototype = BasicModule::initializeFromJson(ModulesData::instance().getModuleJsonData(dataId));
    selectNewNodeNumber(0);
    updateNodeNumberSelection();
    return true;
}

bool ModuleBuilder::selectNewNodeNumber(int number) {
    newModuleNodeNumber = number;
    return true;
}

void Module::drawInfo() {
    for (auto node : nodes) {
        if (node.attachedNode == nullptr && (node.pos.rotate(getRot()) + getPos()).y < GameWorld::surfaceY)
            GraphicsEngine::instance()->drawCircle(
                node.pos.rotate(rot) + pos, 0.5, CommonValues::zDebug,
                al_map_rgba(255, 255, 0, 100), 0.2);
    }
}

void Module::drawDebug() {
    GraphicsEngine::instance()->drawPoint(pos, 0, al_map_rgb(255, 255, 255));
    for (auto wall : walls) {
        GraphicsEngine::instance()->drawPolygon(wall.transformedVerticies, -0.001,
            al_map_rgb(255, 0, 0));
    }

    for (auto area : blockingAreas) {
        GraphicsEngine::instance()->drawPolygon(area.transformedVerticies, -0.001,
            al_map_rgba(100, 0, 0, 40));
    }

    for (auto area : buildableAreas) {
        GraphicsEngine::instance()->drawPolygon(area.transformedVerticies, -0.001,
            al_map_rgba(0, 100, 100, 30));
    }

    for (auto node : nodes) {
        GraphicsEngine::instance()->drawPoint(
            node.pos.rotate(rot) + pos, CommonValues::zDebug,
            al_map_rgb(0, 0, 255));
        GraphicsEngine::instance()->drawLine(
            node.pos.rotate(rot) + pos,
            node.pos.rotate(rot) + pos + Vector2d(rot + node.rot, 5),
            CommonValues::zDebug, al_map_rgb(0, 0, 255));
    }
}

void Module::setTransforms(Vector2d aPos, Rotation aRot) {
    pos = aPos;
    rot = aRot;
    for (auto& wall : walls) {
        wall.transformedVerticies.clear();
        for (auto dot : wall.initialVerticies) {
            wall.transformedVerticies.push_back(pos + dot.rotate(rot));
        }
    }

    for (auto& area : buildableAreas) {
        area.transformedVerticies.clear();
        for (auto dot : area.initialVerticies) {
            area.transformedVerticies.push_back(pos + dot.rotate(rot));
        }
    }

    for (auto& area : blockingAreas) {
        area.transformedVerticies.clear();
        for (auto dot : area.initialVerticies) {
            area.transformedVerticies.push_back(pos + dot.rotate(rot));
        }
    }
}

void Module::setTransforms(ModuleNode* parentNode, ModuleNode* ownNode) {
    Module* parentModule = parentNode->parentModule;
    Rotation newRot = parentModule->rot + parentNode->rot - ownNode->rot + M_PI;
    Vector2d newPos = parentModule->pos + parentNode->pos.rotate(parentModule->rot) + ownNode->pos.rotate(parentModule->rot + parentNode->rot - ownNode->rot);
    setTransforms(newPos, newRot);
}

ModuleNode* Module::getNode(int number) {
    if (number < nodes.size()) { // TODO should be ok since nodes size wont change
        return &nodes.at(number);
    }
    return nullptr;
}

std::vector<ModuleNode*> Module::getNodes() {
    std::vector<ModuleNode*> result;
    for (int i = 0; i < nodes.size(); i++) {
        result.push_back(&nodes.at(i));
    }

    return result;
}

bool Module::checkWallCollision(Rect2d rect) {
    collision::Polygon rectangle({collision::fvec2(rect.p1.x, rect.p1.y),
        collision::fvec2(rect.p2.x, rect.p1.y),
        collision::fvec2(rect.p2.x, rect.p2.y),
        collision::fvec2(rect.p1.x, rect.p2.y)});

    collision::GJKCollisionDetector detector;
    std::vector<collision::fvec2> dots;
    for (auto wall : walls) {

        dots.clear();
        for (auto dot : wall.transformedVerticies) {
            dots.push_back(collision::fvec2(dot.x, dot.y));
        }

        collision::Polygon ownPolygon(dots);
        if (detector.detect(rectangle, ownPolygon))
            return true;
    }

    return false;
}

bool Module::checkBlockingAreaCollision(Module* other) {
    collision::GJKCollisionDetector detector;
    std::vector<collision::fvec2> dots;
    for (auto otherArea : other->blockingAreas) {
        dots.clear();
        for (auto dot : otherArea.transformedVerticies) {
            dots.push_back(collision::fvec2(dot.x, dot.y));
        }
        collision::Polygon otherPolygon(dots);

        for (auto ownArea : blockingAreas) {

            dots.clear();
            for (auto dot : ownArea.transformedVerticies) {
                dots.push_back(collision::fvec2(dot.x, dot.y));
            }

            collision::Polygon ownPolygon(dots);
            if (detector.detect(otherPolygon, ownPolygon))
                return true;
        }
    }
    return false;
}

bool Module::checkTouchesBuildableArea(Rect2d rect) {
    collision::Polygon rectangle({collision::fvec2(rect.p1.x, rect.p1.y),
        collision::fvec2(rect.p2.x, rect.p1.y),
        collision::fvec2(rect.p2.x, rect.p2.y),
        collision::fvec2(rect.p1.x, rect.p2.y)});

    collision::GJKCollisionDetector detector;
    std::vector<collision::fvec2> dots;
    for (auto area : buildableAreas) {

        dots.clear();
        for (auto dot : area.transformedVerticies) {
            dots.push_back(collision::fvec2(dot.x, dot.y));
        }

        collision::Polygon ownPolygon(dots);
        if (detector.detect(rectangle, ownPolygon))
            return true;
    }

    return false;
}

nlohmann::json Module::toJson() {
    auto result = nlohmann::json();
    result["type"] = "Module";
    result["id"] = getId();
    result["pos"] = pos.toJson();
    result["rot"] = rot.toJson();
    auto nodesJson = nlohmann::json::array();
    for (auto node : nodes) {
        auto nodeJson = nlohmann::json();
        if (node.attachedNode != nullptr) {
            nodeJson["attachedNodeNumber"] = std::distance(node.attachedNode->parentModule->nodes.data(), node.attachedNode);
            nodeJson["attachedModuleId"] = node.attachedNode->parentModule->getId();
        }
        nodesJson.push_back(nodeJson);
    }
    result["nodes"] = nodesJson;
    return result;
}

BasicModule::BasicModule(int nodesNumber):
    Module(), nodesNumber(nodesNumber) {
    assert(nodesNumber > 0);
    nodes.reserve(nodesNumber);
}

BasicModule* BasicModule::initializeFromJson(nlohmann::json data) {
    BasicModule* result = new BasicModule(data["nodes"].size());

    for (auto nodeData : data["nodes"]) {
        Vector2d pos = Vector2d::fromJson(nodeData["pos"]);
        Rotation rot = Rotation::fromJson(nodeData["rot"]);
        result->addNode(pos, rot);
    }

    for (auto wallData : data["walls"]) {
        Rect2d rect = Rect2d::fromJson(wallData);
        result->addWall(rect);
    }

    for (auto blockingAreaData : data["blockingAreas"]) {
        Rect2d rect = Rect2d::fromJson(blockingAreaData);
        result->addBlockingArea(rect);
    }

    for (auto buildableAreaData : data["buildableAreas"]) {
        Rect2d rect = Rect2d::fromJson(buildableAreaData);
        result->addBuildableArea(rect);
    }
    result->setDrawable(SpriteCollectionDrawable::fromJson(data["drawable"]));
    result->dataId = data["id"].get<int>();
    return result;
}

BasicModule* BasicModule::fromJson(nlohmann::json data) {
    auto module = BasicModule::initializeFromJson(ModulesData::instance().getModuleJsonData(data["dataId"].get<int>()));
    module->setTransforms(Vector2d::fromJson(data["pos"]), Rotation::fromJson(data["rot"]));
    return module;
}

void BasicModule::addNode(Vector2d pos, Rotation rot) {
    assert(nodes.size() < nodesNumber);
    ModuleNode newNode;
    newNode.pos = pos;
    newNode.rot = rot;
    newNode.parentModule = this;
    nodes.push_back(newNode);
}

void BasicModule::addBitmap(ALLEGRO_BITMAP* bitmap, Vector2d pivot,
    double z) {
    sprites.push_back(ModuleSprite{});
    sprites.back().bitmap = bitmap;
    sprites.back().pivot = pivot;
    sprites.back().z = z;
}

void BasicModule::addWall(Rect2d rect) {
    PolygonalArea wall {};
    wall.initialVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
    wall.initialVerticies.push_back(rect.p2);
    wall.initialVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
    wall.initialVerticies.push_back(rect.p1);
    wall.transformedVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
    wall.transformedVerticies.push_back(rect.p2);
    wall.transformedVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
    wall.transformedVerticies.push_back(rect.p1);

    walls.push_back(wall);
}

void BasicModule::addBuildableArea(Rect2d rect) {
    PolygonalArea area {};
    area.initialVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
    area.initialVerticies.push_back(rect.p2);
    area.initialVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
    area.initialVerticies.push_back(rect.p1);
    area.transformedVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
    area.transformedVerticies.push_back(rect.p2);
    area.transformedVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
    area.transformedVerticies.push_back(rect.p1);

    buildableAreas.push_back(area);
}

void BasicModule::addBlockingArea(Rect2d rect) {
    PolygonalArea area {};
    area.initialVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
    area.initialVerticies.push_back(rect.p2);
    area.initialVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
    area.initialVerticies.push_back(rect.p1);

    area.transformedVerticies.push_back(Vector2d(rect.p1.x, rect.p2.y));
    area.transformedVerticies.push_back(rect.p2);
    area.transformedVerticies.push_back(Vector2d(rect.p2.x, rect.p1.y));
    area.transformedVerticies.push_back(rect.p1);

    blockingAreas.push_back(area);
}

void BasicModule::draw() {
    Module::draw();
    for (auto sprite : sprites) {
        GraphicsEngine::instance()->drawBitmap(pos, sprite.bitmap, 20,
            sprite.z, sprite.pivot, rot);
    }

    if (drawable) {
        drawable->setPos(getPos());
        drawable->setRotation(getRot());
        drawable->draw();
    }
}

nlohmann::json BasicModule::toJson() {
    auto result = Module::toJson();
    result["type"] = "BasicModule";
    result["dataId"] = dataId;
    return result;
}

void ModuleBuilder::onWindowClose() {
    window = nullptr;
    nodeNumberButtons.clear();
    nodeNumberLabels.clear();
}

ModuleBuilder* ModuleBuilder::instance() {
    static ModuleBuilder instance;
    return &instance;
}

void ModuleBuilder::setParentNode(ModuleNode* node) {
    parentModuleNode = node;
}

void ModuleBuilder::createWindow() {
    if (parentModuleNode->attachedNode != nullptr)
        return;
    if (window)
        delete window;

    window = new Window(GuiEngine::instance()->getDisplayArea(), Aligment::byDimensionsAndCentered(Vector2d(600, 460)), true);
    window->setOnCloseCallback([this]() { this->onWindowClose(); });

    Aligment createButtonAligment;
    createButtonAligment.marginLeft = 20;
    createButtonAligment.marginBottom = 20;
    createButtonAligment.dimensions = Vector2d(200, 40);
    auto createButton = new Button(window->getInternalArea(), createButtonAligment);
    createButton->setMouseCallback(Release, [this](auto pos) {
        bool result = this->buildModule();
        if (result)
            delete window;
    });
    new Label(createButton, Aligment(), "Create module");

    Aligment typeSelectionZoneAligment;
    typeSelectionZoneAligment.marginTop = 30;
    typeSelectionZoneAligment.marginLeft = 30;
    typeSelectionZoneAligment.marginBottom = 100;
    typeSelectionZoneAligment.dimensions = Vector2d(250, -1);
    auto typeSelectionZone = new NamedArea(window->getInternalArea(), typeSelectionZoneAligment, "Type selection");

    Aligment nodeSelectionZoneAligment;
    nodeSelectionZoneAligment.marginTop = 30;
    nodeSelectionZoneAligment.marginRight = 30;
    nodeSelectionZoneAligment.marginBottom = 30;
    nodeSelectionZoneAligment.dimensions = Vector2d(250, -1);
    nodeSelectionZone = new NamedArea(window->getInternalArea(), nodeSelectionZoneAligment, "Node selection");

    int line = 0;
    for (auto moduleDataId : ModulesData::instance().getAllModuleDataIds()) {
        Aligment buttonAligment;
        buttonAligment.marginLeft = 5;
        buttonAligment.marginRight = 5;
        buttonAligment.marginTop = 5 + line * 25;
        buttonAligment.dimensions = Vector2d(-1, 20);
        auto button = new Button(typeSelectionZone->getInternalArea(), buttonAligment);
        button->setMouseCallback(Release, [this, dataId = moduleDataId](auto pos) { createModulePrototype(dataId); });
        new Label(button, Aligment(), ModulesData::instance().getModuleJsonData(moduleDataId)["name"].get<std::string>());
        line++;
    }

    createModulePrototype(1); // TODO should be first module or default module not just Corridor
}

void ModuleBuilder::createModuleSelectionButtons() {
}

void ModuleBuilder::updateNodeNumberSelection() {
    if (!window)
        return;
    for (auto button : nodeNumberButtons) {
        delete button;
    }
    nodeNumberButtons.clear();
    nodeNumberLabels.clear();

    int i = 0;
    for (auto node : modulePrototype->getNodes()) {
        Aligment buttonAligment;
        buttonAligment.marginLeft = 5;
        buttonAligment.marginRight = 5;
        buttonAligment.marginTop = 5 + i * 25;
        buttonAligment.dimensions = Vector2d(-1, 20);
        auto button = new Button(nodeSelectionZone->getInternalArea(), buttonAligment);
        button->setMouseCallback(Release, [this, n = i](auto pos) { this->selectNewNodeNumber(n); });
        auto label = new Label(button, Aligment(), std::to_string(i));

        nodeNumberButtons.push_back(button);
        nodeNumberLabels.push_back(label);
        i++;
    }
}
