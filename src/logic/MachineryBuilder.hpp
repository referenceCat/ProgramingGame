#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_

#include "Machinery.hpp"
#include "CustomMachinery.hpp"
#include "GuiEngine.hpp"
#include "GameWorld.hpp"
#include "Controller.hpp"

enum MachineryType {
    TypeArm,
    TypeController,
    TypeLab,
    TypeParticleResearch,
    TypeDrill,
    TypeElectrolyzer,
    TypeAnalyzer,
    Test
};

class MachineryBuilder {
    Window* window;
    Machinery* prototype;
    bool stickToGrid = true;
    bool blocked = false;

    void selectType(MachineryType type) {
        switch (type) {
            case TypeArm:
                prototype = new Manipulator(Vector2d());
                break;
            case TypeController:
                prototype = new Controller(Vector2d());
                break;
            case TypeDrill:
                prototype = new Drill(Vector2d());
                break;
            case TypeElectrolyzer:
                prototype = new Electrolyzer(Vector2d());
                break;
            case TypeLab:
                prototype = new Lab(Vector2d());
                break;
            case TypeParticleResearch:
                prototype = new ParticleDetector(Vector2d());
                break;
            case TypeAnalyzer:
                prototype = new Analyzer(Vector2d());
                break;
            case Test: {
                auto abstactAssembler = new AbstractAssembler(Rect2d::fromCenterAndDimensions(Vector2d(), Vector2d(5, 5)));
                auto* drawable = new BitmapCollectionDrawable();
                drawable->addSprite({GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Controller/main.png"), CommonValues::zMachinery, {0, 0}, Rotation::fromDegrees(15), {30, 30}});
                abstactAssembler->setDrawable(drawable);
                prototype = abstactAssembler;
            } break;
            default:
                break;
        }
    }

public:
    static MachineryBuilder* instance() {
        static MachineryBuilder instance;
        return &instance;
    }

    bool checkBlocked() {
        if (!prototype)
            return false;

        bool touchingBuildableArea = false;
        for (auto module : GameWorld::instance()->getModules()) {
            if (module->checkTouchesBuildableArea(prototype->getRect())) {
                touchingBuildableArea = true;
                break;
            }
        }
        if (!touchingBuildableArea)
            return true;

        for (auto machinery : GameWorld::instance()->getMachinery()) {
            if (machinery->getRect().isIntersecting(prototype->getRect()))
                return true;
        }

        for (auto module : GameWorld::instance()->getModules()) {
            if (module->checkWallCollision(prototype->getRect()))
                return true;
        }

        return false;
    }

    void clearItem() {
        delete prototype;
        prototype = nullptr;
    }

    void onClick() {
        if (prototype == nullptr) {
            return;
        }

        blocked = checkBlocked();
        if (blocked)
            return;
        prototype->addToGameWorld();
        prototype = nullptr;
    };

    bool placingMachinery() {
        return prototype != nullptr;
    }

    void mousePos(Vector2d pos) {
        if (!prototype)
            return;
        Rect2d lastPos = prototype->getRect();
        if (stickToGrid) {
            Vector2d roundedPos;
            roundedPos.x = std::round(pos.x - prototype->getRect().dimensions().x / 2) + prototype->getRect().dimensions().x / 2;
            roundedPos.y = std::round(pos.y - prototype->getRect().dimensions().y / 2) + prototype->getRect().dimensions().y / 2;
            prototype->setCenter(roundedPos);
        } else {
            prototype->setCenter(pos);
        }

        if (prototype->getRect() != lastPos)
            blocked = checkBlocked();
    };

    void drawGhost() {
        if (!prototype)
            return;

        prototype->draw(); // TODO draw in blue color
        if (!blocked)
            GraphicsEngine::instance()->drawRectangle(prototype->getRect(), 0, al_map_rgba(50, 100, 100, 30));
        else
            GraphicsEngine::instance()->drawRectangle(prototype->getRect(), 0, al_map_rgba(100, 50, 50, 30));

        // draw grid

        const int gridSize = 15;
        const double gridExpantion = 2;
        for (int i = 0; i < gridSize; i++) {
            double x = round(prototype->getRect().center().x) - gridSize / 2 + i;
            double y1 = prototype->getRect().center().y - gridSize / 2 - gridExpantion;
            double y2 = prototype->getRect().center().y + gridSize / 2 + gridExpantion;
            GraphicsEngine::instance()->drawLine(Vector2d(x, y1), Vector2d(x, y2), 0, al_map_rgba(100, 50, 50, 30), 0.1);
        }

        for (int i = 0; i < gridSize; i++) {
            double y = round(prototype->getRect().center().y) - gridSize / 2 + i;
            double x1 = prototype->getRect().center().x - gridSize / 2 - gridExpantion;
            double x2 = prototype->getRect().center().x + gridSize / 2 + gridExpantion;
            GraphicsEngine::instance()->drawLine(Vector2d(x1, y), Vector2d(x2, y), 0, al_map_rgba(100, 50, 50, 30), 0.1);
        }
    }

    void createWindow() {
        if (window)
            delete window;

        Aligment windowAligment;
        windowAligment.marginLeft = 10;
        windowAligment.marginBottom = 10;
        windowAligment.dimensions = Vector2d(300, 150);
        window = new Window(GuiEngine::instance()->getDisplayArea(), windowAligment, false);

        Aligment labelAligment;
        labelAligment.marginTop = 4;
        labelAligment.marginLeft = 4;
        labelAligment.marginRight = 4;
        labelAligment.dimensions = Vector2d(0, 20);

        std::string labels[] = {"Arm", "Controller", "Lab", "Drill", "Electrolyzer", "Particle Detector", "Analyzer", "Test"};
        MachineryType types[] = {TypeArm, TypeController, TypeLab, TypeDrill, TypeElectrolyzer, TypeParticleResearch, TypeAnalyzer, Test};

        for (int i = 0; i < 8; i++) {
            Aligment buttonAligment;
            buttonAligment.marginLeft = 4;
            buttonAligment.marginRight = 4;
            buttonAligment.marginTop = 4;
            buttonAligment.marginBottom = 4;
            buttonAligment.tableColumns = 2;
            buttonAligment.tableRows = 4;
            buttonAligment.ownRow = i / 2;
            buttonAligment.ownColumn = i % 2;
            auto button = new Button(window->getInternalArea(), buttonAligment);
            button->setMouseCallback(Release, [this, type = types[i]](auto pos) { this->selectType(type); });
            new Label(button, Aligment(), labels[i]);
        }
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_