#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_

class Furnace: public Machinery {
    ProductionArea heatingArea;
public:
    Furnace(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
        heatingArea.rect = Rect2d::fromCenterAndDimensions(Vector2d(5, 5), Vector2d(8, 8));
        areas.push_back(&heatingArea);
    }

    void run() override {
        for (auto item: getBoxesInside(heatingArea))  {
            if(item->getTemperature() < 255) item->setTemperature(item->getTemperature() + 1);
        }
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->furnaceSprite, 10, CommonValues::zMachinery);
        // Machinery::draw();
    }
};

class ManipulatorTier1: public Machinery {
    ManipulatorArm* arm = nullptr;
public:
    ManipulatorTier1(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(5, 3))) {
    }

    void run() override {
        // TODO manipulator update here
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->baseSpite, 20, 0.1);
        if (arm) {
            GraphicsEngine::instance()->drawBitmap(arm->getJointPosition(0),  GraphicsEngine::instance()->segment0Sprite, 20, 0.05, Vector2d(20, 40), arm->getJointRotation(0));
            GraphicsEngine::instance()->drawBitmap(arm->getJointPosition(1),  GraphicsEngine::instance()->segment1Sprite, 20, 0.1, Vector2d(20, 40), arm->getJointRotation(0) + arm->getJointRotation(1));
        }

    }

    void onCommandRecive(int cmd, int arg) override {
        switch (cmd) {
        case 0:
            arm->setJointTargetRotation(0, Rotation::fromDegrees(arg));
            break;
        case 1:
            arm->setJointTargetRotation(1, Rotation::fromDegrees(arg));
            break;
        case 100:
            arm->release();
            break;
        case 200:
            arm->grab();
            break;
        default:
            break;
        }
    }

    void addToGameWorld() override {  // TODO 
        Machinery::addToGameWorld();
        arm = new ManipulatorArm(3);
        arm->setRootJointPosition(Vector2d(4, 2) + rect.p1);
        arm->setSegmentLength(0, 8);
        arm->setSegmentTargetLength(0, 8);
        arm->setSegmentLength(1, 6);
        arm->setSegmentTargetLength(1, 6);
        arm->recalculate();
        arm->addToGameWorld();
    }
};

class BoxGenerator: public Machinery {
    ProductionArea creatingArea;
    int period = 200;
    int cooldown = period;
    BoxContent boxContent = BoxContent::EmptyBox;
    

public:
    BoxGenerator(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(5, 5))) {
        creatingArea.rect = Rect2d::fromCenterAndDimensions(Vector2d(2.5, 2.5), Vector2d(4, 4));
        areas.push_back(&creatingArea);
    }

    void setPeriod(int aPeriod) {
        period = aPeriod;
    }

    void setType(BoxContent aContent) {
        boxContent = aContent;
    }

    void run() override {
        if (cooldown > 0) cooldown--;

        if (cooldown == 0 && getBoxesTouching(creatingArea).size() == 0) {
            createBox(creatingArea)->setContent(boxContent);
            cooldown = period;
        }
    }

    void draw() override {
        // Machinery::draw();
        GraphicsEngine::instance()->drawLine(Vector2d(rect.p1.x, rect.p2.y + 1), Vector2d(rect.p1.x + (period - cooldown) / 40, rect.p2.y + 1), 0, al_map_rgb(100, 100, 100), 2);
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->boxCreatorDestroyerBaseSprite, 10, CommonValues::zMachinery);
    }

};

class BoxDestroyer: public Machinery {
    ProductionArea destroyingArea;
    int cooldown = 200;

public:
    BoxDestroyer(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(5, 5))) {
        destroyingArea.rect = Rect2d::fromCenterAndDimensions(Vector2d(2.5, 2.5), Vector2d(4, 4));
        areas.push_back(&destroyingArea);
    }

    void run() override {
        for (auto item: getBoxesInside(destroyingArea))  {
            destroyBox(item);
        }
    }

    void draw() override {
        // Machinery::draw();
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->boxCreatorDestroyerBaseSprite, 10, CommonValues::zMachinery);
    }
};

class AssemblerTier1: public Machinery {
    ProductionArea ingridientArea0;
    ProductionArea ingridientArea1;
    ProductionArea resultArea;
    int period = 160;
    int processTime = 0;
    bool processRunning = false;

    int pressShiftByProcessTime() { // TODO bad function name xd
        if (processTime < 20 || processTime > 140) return 0;
        if (processTime > 60 && processTime < 100) return 20;
        if (processTime < 80) return (processTime - 20) / 2;
        return (140 - processTime) / 2;
    }
    

public:
    AssemblerTier1(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(16, 8))) {
        ingridientArea0.rect = Rect2d::fromCenterAndDimensions(Vector2d(2, 2), Vector2d(3.5, 3.5)); // TODO process class for this kind of behavior
        ingridientArea1.rect = Rect2d::fromCenterAndDimensions(Vector2d(2, 6), Vector2d(3.5, 3.5));
        resultArea.rect = Rect2d::fromCenterAndDimensions(Vector2d(14, 2), Vector2d(3.5, 3.5));
        areas.push_back(&ingridientArea0);
        areas.push_back(&ingridientArea1);
        areas.push_back(&resultArea);
    }

    void setPeriod(int aPeriod) {
        period = aPeriod;
    }

    void run() override {
        if (!processRunning && getBoxesInside(ingridientArea0).size() && getBoxesInside(ingridientArea1).size()) {
            auto box0 = getBoxesInside(ingridientArea0).at(0);
            auto box1 = getBoxesInside(ingridientArea1).at(0);
            if (box0->getContent() == BoxContent::IronPlate 
                && box1->getContent() == BoxContent::IronPlate
                && box0->getTemperature() > 100
                && box1->getTemperature() > 100) {
                destroyBox(box0); // process begins here
                destroyBox(box1);
                processRunning = true;
            }
        }

        if (processRunning && processTime < period) processTime++;

        if (processRunning && processTime >= period) {
            if (getBoxesTouching(resultArea).size() == 0) { // check if can create box
                auto resultBox = createBox(resultArea); // process stops here
                resultBox->setContent(BoxContent::HeavyIronPlate);
                processRunning = false;
                processTime = 0;
            }
        }
    }

    void draw() override {
        // Machinery::draw();
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerBaseSprite, 10, CommonValues::zMachineryFront, Vector2d(165, 165));
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerCyllindersSprite, 10, CommonValues::zMachineryBack, Vector2d(165, 165));
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerPressSprite, 10, CommonValues::zMachinery, Vector2d(165, 165 - pressShiftByProcessTime() + 4));
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerPlateSprite, 10, CommonValues::zMachinery, Vector2d(165, 165));
        GraphicsEngine::instance()->drawLine(Vector2d(rect.p1.x, rect.p2.y + 1), Vector2d(rect.p1.x + processTime / 10, rect.p2.y + 1), CommonValues::zMachinery, al_map_rgb(100, 100, 100), 2);
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_