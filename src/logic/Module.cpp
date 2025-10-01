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

    GameWorld::instance()->addModule(modulePrototype);
    modulePrototype->addToGameWorld();
    modulePrototype = nullptr;
    return true;
}

bool ModuleBuilder::createModulePrototype(ModuleType type) {
    delete modulePrototype;
    switch (type) { // put this data to json vvv
        case ModuleType::Corridor: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(6);
            moduleSetup->addNode(Vector2d(Rotation(0), 16), Rotation(0));
            moduleSetup->addNode(Vector2d(Rotation(M_PI), 16), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(-8, -8), Rotation(M_PI / 2 * 3));
            moduleSetup->addNode(Vector2d(8, -8), Rotation(M_PI / 2 * 3));
            moduleSetup->addNode(Vector2d(-8, 8), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(8, 8), Rotation(M_PI / 2));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Corridor/background.png"), Vector2d(320, 320), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Corridor/walls.png"), Vector2d(320, 320), CommonValues::zModuleWalls);
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -9), Vector2d(16, -7)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 9), Vector2d(16, 7)));
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-16, -12), Vector2d(16, 12)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-16, -7), Vector2d(16, 7)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::ConnectorCross: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
            moduleSetup->addNode(Vector2d(Rotation(0), 12), Rotation(0));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 12), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(Rotation(M_PI), 12), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 12), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorCross/background.png"), Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorCross/walls.png"), Vector2d(240, 240), CommonValues::zModuleWalls);
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, -12), Vector2d(-7, -7))); // top left
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, -9), Vector2d(-7, -7)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(7, -12), Vector2d(9, -9))); // top right
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(12, -9), Vector2d(7, -7)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, 9), Vector2d(-7, 7))); // bottom left
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, 12), Vector2d(-7, 7)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(12, 9), Vector2d(7, 7))); // bottom right
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(9, 12), Vector2d(7, 7)));
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-12, -12), Vector2d(12, 12)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -12), Vector2d(7, 12)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-12, -7), Vector2d(12, 7)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::Connector3: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(3);
            moduleSetup->addNode(Vector2d(Rotation(M_PI), 8.62), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 3), 8.62), Rotation(M_PI / 3));
            moduleSetup->addNode(Vector2d(Rotation(-M_PI / 3), 8.62), Rotation(-M_PI / 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Connector3/background.png"), Vector2d(8.62 * 20, 320), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Connector3/walls.png"), Vector2d(8.62 * 20, 320), CommonValues::zModuleWalls);
            // add collision
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::Deadend: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(2);
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 12), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), -4), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Deadend/background.png"), Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Deadend/walls.png"), Vector2d(240, 240), CommonValues::zModuleWalls);
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, 3), Vector2d(-7, 12)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(9, 3), Vector2d(7, 12)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, 3), Vector2d(9, 5)));
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-12, 0), Vector2d(12, 12)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, 5), Vector2d(7, 12)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::ConnectorT: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
            moduleSetup->addNode(Vector2d(Rotation(0), 12), Rotation(0));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 9), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(Rotation(M_PI), 12), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 12), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorT/background.png"), Vector2d(240, 240), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/ConnectorT/walls.png"), Vector2d(240, 240), CommonValues::zModuleWalls);
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-9, -12), Vector2d(-7, -7))); // top left
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, -9), Vector2d(-7, -7)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(7, -12), Vector2d(9, -9))); // top right
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(12, -9), Vector2d(7, -7)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-12, 7), Vector2d(12, 9))); // bottom
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-12, -12), Vector2d(12, 12)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -12), Vector2d(7, -7)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-12, -7), Vector2d(12, 7)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::Frame: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(2);
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 16), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 16), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/FrameStraight/background.png"), Vector2d(320, 320), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-6, -10), Vector2d(6, 10)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -16), Vector2d(7, 16)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::FrameFoundation: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(2);
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 16), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 16), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/FrameStraight/background.png"), Vector2d(320, 320), CommonValues::zModuleFarBackgroung);
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -16), Vector2d(7, 16)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::FrameCross: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
            moduleSetup->addNode(Vector2d(Rotation(0), 7), Rotation(0));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 7), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(Rotation(M_PI), 7), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2 * 3), 7), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/FrameCross/background.png"), Vector2d(140, 140), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-6, -6), Vector2d(6, 6)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-7, -7), Vector2d(7, 7)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::Frame3: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(3);
            moduleSetup->addNode(Vector2d(Rotation(M_PI), 4.04), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 3), 4.04), Rotation(M_PI / 3));
            moduleSetup->addNode(Vector2d(Rotation(-M_PI / 3), 4.04), Rotation(-M_PI / 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/Frame3/background.png"), Vector2d(4.04 * 20, 140), CommonValues::zModuleMainBackgroung);
            // add collision
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::LargeModule: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(4);
            moduleSetup->addNode(Vector2d(16, -16), Rotation(0));
            moduleSetup->addNode(Vector2d(0, 32), Rotation(M_PI / 2));
            moduleSetup->addNode(Vector2d(-16, -16), Rotation(M_PI));
            moduleSetup->addNode(Vector2d(0, -32), Rotation(M_PI / 2 * 3));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/LargeModule/background.png"), Vector2d(320, 640), CommonValues::zModuleMainBackgroung);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/LargeModule/walls.png"), Vector2d(320, 640), CommonValues::zModuleWalls);
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/LargeModule/far_background.png"), Vector2d(320, 640), CommonValues::zModuleVeryFarBackgroung);
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(16, 32)));
            moduleSetup->addBuildableArea(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(16, 32)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(-7, -30))); // top left
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, -32), Vector2d(-14, -23)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, -32), Vector2d(7, -30))); // top rith
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, -32), Vector2d(14, -23)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 32), Vector2d(-7, 30))); // bottom left
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(-16, 32), Vector2d(-14, -9)));
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, 32), Vector2d(7, 30))); // bottom right
            moduleSetup->addWall(Rect2d::fromTwoCorners(Vector2d(16, 32), Vector2d(14, -9)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::SolarPanel: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(1);
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 12), Rotation(M_PI / 2));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/external/SolarPanel/background.png"), Vector2d(160, 320), CommonValues::zModuleFarBackgroung);
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-8, -16), Vector2d(8, 8)));
            modulePrototype = moduleSetup;
            break;
        }
        case ModuleType::Antena: {
            BasicModulePrototype* moduleSetup = new BasicModulePrototype(1);
            moduleSetup->addNode(Vector2d(Rotation(M_PI / 2), 12), Rotation(M_PI / 2));
            moduleSetup->addBitmap(GraphicsEngine::instance()->getBitmap("resources/assets/modules/external/CommunicationArray/background.png"), Vector2d(160, 320), CommonValues::zModuleFarBackgroung);
            moduleSetup->addBlockingArea(Rect2d::fromTwoCorners(Vector2d(-8, -16), Vector2d(8, 8)));
            modulePrototype = moduleSetup;
            break;
        }
        default:
            break;
    }
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
        if (node.attachedNode == nullptr)
            GraphicsEngine::instance()->drawCircle(
                node.position.rotate(rotation) + position, 0.5, CommonValues::zDebug,
                al_map_rgba(255, 255, 0, 100), 0.2);
    }
}

void Module::drawDebug() {
    GraphicsEngine::instance()->drawPoint(position, 0, al_map_rgb(255, 255, 255));
    for (auto wall : walls) {
        GraphicsEngine::instance()->drawPolygon(wall->transformedVerticies, -0.001,
            al_map_rgb(255, 0, 0));
    }

    for (auto area : blockingAreas) {
        GraphicsEngine::instance()->drawPolygon(area->transformedVerticies, -0.001,
            al_map_rgba(100, 0, 0, 40));
    }

    for (auto area : buildableAreas) {
        GraphicsEngine::instance()->drawPolygon(area->transformedVerticies, -0.001,
            al_map_rgba(0, 100, 100, 30));
    }

    for (auto node : nodes) {
        GraphicsEngine::instance()->drawPoint(
            node.position.rotate(rotation) + position, CommonValues::zDebug,
            al_map_rgb(0, 0, 255));
        GraphicsEngine::instance()->drawLine(
            node.position.rotate(rotation) + position,
            node.position.rotate(rotation) + position + Vector2d(rotation + node.rotation, 5),
            CommonValues::zDebug, al_map_rgb(0, 0, 255));
    }
}

void Module::setTransforms(Vector2d aPos, Rotation aRot) {
    position = aPos;
    rotation = aRot;
    for (auto wall : walls) {
        wall->transformedVerticies.clear();
        for (auto dot : wall->initialVerticies) {
            wall->transformedVerticies.push_back(position + dot.rotate(rotation));
        }
    }

    for (auto area : buildableAreas) {
        area->transformedVerticies.clear();
        for (auto dot : area->initialVerticies) {
            area->transformedVerticies.push_back(position + dot.rotate(rotation));
        }
    }

    for (auto area : blockingAreas) {
        area->transformedVerticies.clear();
        for (auto dot : area->initialVerticies) {
            area->transformedVerticies.push_back(position + dot.rotate(rotation));
        }
    }
}

void Module::setTransforms(ModuleNode* parentNode, ModuleNode* ownNode) {
    Module* parentModule = parentNode->parentModule;
    Rotation newRot = parentModule->rotation + parentNode->rotation - ownNode->rotation + M_PI;
    Vector2d newPos = parentModule->position + parentNode->position.rotate(parentModule->rotation) + ownNode->position.rotate(parentModule->rotation + parentNode->rotation - ownNode->rotation);
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
        for (auto dot : wall->transformedVerticies) {
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
        for (auto dot : otherArea->transformedVerticies) {
            dots.push_back(collision::fvec2(dot.x, dot.y));
        }
        collision::Polygon otherPolygon(dots);

        for (auto ownArea : blockingAreas) {

            dots.clear();
            for (auto dot : ownArea->transformedVerticies) {
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
        for (auto dot : area->transformedVerticies) {
            dots.push_back(collision::fvec2(dot.x, dot.y));
        }

        collision::Polygon ownPolygon(dots);
        if (detector.detect(rectangle, ownPolygon))
            return true;
    }

    return false;
}

BasicModulePrototype::BasicModulePrototype(int nodesNumber):
    Module(), nodesNumber(nodesNumber) {
    assert(nodesNumber > 0);
    nodes.reserve(nodesNumber);
}

void BasicModulePrototype::addNode(Vector2d pos, Rotation rot) {
    assert(nodes.size() < nodesNumber);
    ModuleNode newNode;
    newNode.position = pos;
    newNode.rotation = rot;
    newNode.parentModule = this;
    nodes.push_back(newNode);
}

void BasicModulePrototype::addBitmap(ALLEGRO_BITMAP* bitmap, Vector2d pivot,
    double z) {
    sprites.push_back(ModuleSprite{});
    sprites.back().bitmap = bitmap;
    sprites.back().pivot = pivot;
    sprites.back().z = z;
}

void BasicModulePrototype::addWall(Rect2d rect) {
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

void BasicModulePrototype::addBuildableArea(Rect2d rect) {
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

void BasicModulePrototype::addBlockingArea(Rect2d rect) {
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

void BasicModulePrototype::draw() {
    Module::draw();
    for (auto sprite : sprites) {
        GraphicsEngine::instance()->drawBitmap(position, sprite.bitmap, 20,
            sprite.z, sprite.pivot, rotation);
    }
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

    window = new Window(GuiEngine::instance()->getDisplayArea(), Aligment::byDimensionsAndCentered(Vector2d(600, 400)), true);
    window->setOnCloseCallback([this](){this->onWindowClose();});

    Aligment createButtonAligment;
    createButtonAligment.marginLeft = 10;
    createButtonAligment.marginBottom = 10;
    createButtonAligment.dimensions = Vector2d(100, 40);
    auto createButton = new Button(window->getArea(), createButtonAligment);
    createButton->setMouseCallback(Release, [this](auto pos) {
        bool result = this->buildModule();
        if (result) delete window;
    });
    new Label(createButton, Aligment(), "Create module");

    Aligment typeSelectionZoneAligment;
    typeSelectionZoneAligment.marginTop = 1;
    typeSelectionZoneAligment.marginLeft = 1;
    typeSelectionZoneAligment.marginBottom = 60;
    typeSelectionZoneAligment.dimensions = Vector2d(280, -1);
    auto typeSelectionZone = new GuiElement(window->getArea(), typeSelectionZoneAligment);

    Aligment typeSelectionZoneLabelAligment;
    typeSelectionZoneLabelAligment.marginTop = 3;
    typeSelectionZoneLabelAligment.marginLeft = 10;
    typeSelectionZoneLabelAligment.marginRight = 10;
    typeSelectionZoneLabelAligment.dimensions = Vector2d(-1, 20);
    new Label(typeSelectionZone, typeSelectionZoneLabelAligment, "Select type: ");

    Aligment nodeSelectionZoneAligment;
    nodeSelectionZoneAligment.marginTop = 1;
    nodeSelectionZoneAligment.marginRight = 1;
    nodeSelectionZoneAligment.marginBottom = 1;
    nodeSelectionZoneAligment.dimensions = Vector2d(280, -1);
    nodeSelectionZone = new GuiElement(window->getArea(), nodeSelectionZoneAligment);

    Aligment nodeSelectionZoneLabelAligment;
    nodeSelectionZoneLabelAligment.marginTop = 3;
    nodeSelectionZoneLabelAligment.marginLeft = 10;
    nodeSelectionZoneLabelAligment.marginRight = 10;
    nodeSelectionZoneLabelAligment.dimensions = Vector2d(-1, 20);
    new Label(nodeSelectionZone, nodeSelectionZoneLabelAligment, "Select node: ");

    std::string labels[] = {"Corridor", "Cross Connector", "3 Way Connector",
        "T Connector", "Deadend", "Frame",
        "Square Frame", "Triangle Frame", "Utility Module",
        "Solar Panel", "Antena array"};
    ModuleType types[] = {Corridor, ConnectorCross, Connector3, ConnectorT,
        Deadend, Frame, FrameCross, Frame3,
        LargeModule, SolarPanel, Antena};
    int n = 11;

    for (int i = 0; i < n; i++) {
        Aligment buttonAligment;
        buttonAligment.marginLeft = 4;
        buttonAligment.marginRight = 4;
        buttonAligment.marginTop = 25 + i * 25;
        buttonAligment.dimensions = Vector2d(-1, 20);
        auto button = new Button(typeSelectionZone, buttonAligment);
        button->setMouseCallback(Release, [this, type = types[i]](auto pos) { createModulePrototype(type); });
        new Label(button, Aligment(), labels[i]);
    }

    createModulePrototype(Corridor);
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
        buttonAligment.marginLeft = 4;
        buttonAligment.marginRight = 4;
        buttonAligment.marginTop = 25 + i * 25;
        buttonAligment.dimensions = Vector2d(0, 20);
        auto button = new Button(nodeSelectionZone, buttonAligment);
        button->setMouseCallback(Release, [this, n = i](auto pos) { this->selectNewNodeNumber(n); });
        auto label = new Label(button, Aligment(), std::to_string(i));

        nodeNumberButtons.push_back(button);
        nodeNumberLabels.push_back(label);
        i++;
    }
}
