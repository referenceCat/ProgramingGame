#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_

#include <cstdlib> 
#include "Machinery.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include <math.h>

enum ProductionProcessStatus {
    Running,
    WaitingToFinish,
    WaitingToStart
};

struct ProductionProcess {
    int duration = 100;
    int progress = 0;
    ProductionProcessStatus status = WaitingToStart;
};

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
        // GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->furnaceSprite, 10, CommonValues::zMachinery);
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
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/Base/main.png"), 20, 0.1);
        if (arm) {
            GraphicsEngine::instance()->drawBitmap(arm->getJointPosition(0),  GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/Segment0/main.png"), 20, 0.05, Vector2d(20, 40), arm->getJointRotation(0));
            GraphicsEngine::instance()->drawBitmap(arm->getJointPosition(1),  GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/Segment1/main.png"), 20, 0.1, Vector2d(20, 40), arm->getJointRotation(0) + arm->getJointRotation(1));
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

class Drill: public Machinery {
    ProductionArea output0;
    ProductionArea output1;
    ProductionProcess miningProcess;
    int tick = 0;
    Vector2d detailShift = Vector2d();

    void updateAnimation() {
        tick++;
        detailShift = Vector2d(std::sin(static_cast<double>(tick) / 2), std::sin(static_cast<double>(tick) / 3)) / 15;
    }

public:
    Drill(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
        miningProcess.duration = 500;
        output0 = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(7.5, 2.5), Vector2d(5, 5)));
        areas.push_back(&output0);
        output1 = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(7.5, 7.5), Vector2d(5, 5)));
        areas.push_back(&output1);
    }

    void run() override {
        if (miningProcess.status == WaitingToStart) {
            miningProcess.status = Running;
            miningProcess.progress = 0;
        }

        if (miningProcess.status == Running) {
            if (miningProcess.progress == miningProcess.duration) {
                miningProcess.status = WaitingToFinish;
            } else {
                miningProcess.progress++;
            }
            updateAnimation(); // shake drill sprite if running
        }

        if (miningProcess.status == WaitingToFinish) {
            if (getBoxesTouching(output0).size() == 0) {
                auto box = new ResourceBoxPrototype(Rect2d::fromCenterAndDimensions(rect.p1 + output0.rect.center(), Vector2d(4, 4)), GraphicsEngine::instance()->getBitmap("resources/assets/boxes/Regolith/main.png"), Resource::Regolith);
                box->addToGameWorld();
                miningProcess.status = WaitingToStart;
                miningProcess.progress = 0;
            } else if (getBoxesTouching(output1).size() == 0) {
                auto box = new ResourceBoxPrototype(Rect2d::fromCenterAndDimensions(rect.p1 + output1.rect.center(), Vector2d(4, 4)), GraphicsEngine::instance()->getBitmap("resources/assets/boxes/Regolith/main.png"), Resource::Regolith);
                box->addToGameWorld();
                miningProcess.status = WaitingToStart;
                miningProcess.progress = 0;
            }
            // else continue waiting
        }


    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1 + detailShift, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/detail1.png"), 20, CommonValues::zMachinery);
        GraphicsEngine::instance()->drawBitmap(rect.p1 + detailShift * 0.7, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/detail2.png"), 20, CommonValues::zMachinery);
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/main.png"), 20, CommonValues::zMachinery);
        GraphicsEngine::instance()->drawBitmap(rect.p1 + detailShift * 0.5, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/drill.png"), 20, CommonValues::zMachineryBack);
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/background.png"), 20, CommonValues::zMachineryBack);
        GraphicsEngine::instance()->drawArcProgressBar(rect.p1 + Vector2d(1.8, 1.8), static_cast<double> (miningProcess.progress) / miningProcess.duration, 0.7, CommonValues::zMachinery, al_map_rgb(255, 255, 255), 0.2);

        for (int y = rect.p2.y; y < GameWorld::instance()->surfaceY + 10; y += 10) { // draw drill to the ground level
            GraphicsEngine::instance()->drawBitmap(Vector2d(rect.p1.x, y) + detailShift * 0.5, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/drill.png"), 20, CommonValues::zMachineryBack);
        }
    }
};

class Electrolyzer: public Machinery {
    ProductionArea input0;
    ProductionArea output0;
    ProductionArea output1;
    ProductionArea output2;
    ProductionProcess process;

public:
    Electrolyzer(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
        process.duration = 500;
        input0 = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(2.5, 2.5), Vector2d(5, 5)));
        areas.push_back(&input0);
        output0 = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(1.6, 8.3), Vector2d(3.33, 3.33)));
        areas.push_back(&output0);
        output1 = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(5, 8.3), Vector2d(3.33, 3.33)));
        areas.push_back(&output1);
        output2 = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(8.3, 8.3), Vector2d(3.33, 3.33)));
        areas.push_back(&output2);
    }

    void run() override {
        if (process.status == WaitingToStart) {
            if (getBoxesInside(input0).size()) {
                auto box = getBoxesInside(input0).at(0);
                process.status = Running;
                process.progress = 0;
                destroyBox(box);
            } // TODO check resource type
        }

        if (process.status == Running) {
            if (process.progress == process.duration) {
                process.status = WaitingToFinish;
            } else {
                process.progress++;
            }
        }

        if (process.status == WaitingToFinish) {
            if (getBoxesTouching(output0).size() == 0 && getBoxesTouching(output1).size() == 0 && getBoxesTouching(output1).size() == 0) {
                auto box = new ResourceBoxPrototype(Rect2d::fromCenterAndDimensions(rect.p1 + output0.rect.center(), Vector2d(3, 3)), GraphicsEngine::instance()->getBitmap("resources/assets/boxes/Oxygen/main.png"), Resource::Oxygen);
                box->addToGameWorld();
                box = new ResourceBoxPrototype(Rect2d::fromCenterAndDimensions(rect.p1 + output1.rect.center(), Vector2d(3, 3)), GraphicsEngine::instance()->getBitmap("resources/assets/boxes/Alloy/main.png"), Resource::Alloy);
                box->addToGameWorld();
                box = new ResourceBoxPrototype(Rect2d::fromCenterAndDimensions(rect.p1 + output2.rect.center(), Vector2d(3, 3)), GraphicsEngine::instance()->getBitmap("resources/assets/boxes/Silicon/main.png"), Resource::Silicon);
                box->addToGameWorld();
                process.status = WaitingToStart;
                process.progress = 0;
            }
            // else continue waiting
        }

    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Electrolyzer/main.png"), 20, CommonValues::zMachinery);
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Electrolyzer/background.png"), 20, CommonValues::zMachineryBack);
        GraphicsEngine::instance()->drawArcProgressBar(rect.p1 + Vector2d(6.6, 4.4), static_cast<double> (process.progress) / process.duration, 0.7, CommonValues::zMachinery, al_map_rgb(255, 255, 255), 0.2);
    }
};

class Lab: public Machinery {
    long tickCounter = 0;
    bool lamps[16][32] = {0};

public:
    Lab(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(17, 17))) {
    }

    void updateLamps() {
        for (int i = 0; i < 16; i++) for (int j = 0; j < 32; j++) {
            lamps[i][j] = (rand() % 3) == 0;
        }
    }

    void run() override {
        tickCounter++;
        if (tickCounter % 30 == 0) updateLamps(); // update lamps every 0.5 seconds
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Lab/background.png"), 20, CommonValues::zMachineryBack);
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Lab/main.png"), 20, CommonValues::zMachinery);
        
        for (int i = 0; i < 16; i++) for (int j = 0; j < 32; j++) {
            if (lamps[i][j] && !(Rect2d(Vector2d(24, 9), Vector2d(31, 15)).isInside(Vector2d(j, i)))) GraphicsEngine::instance()->drawCircle(Vector2d(j *  0.5, i * 0.5) + rect.p1 + Vector2d(0.75, 4.75), 0.1, CommonValues::zMachinery, al_map_rgba(255, 255, 100, 100));
        }
    }
};

class ParticleResearch: public Machinery {
    long tickCounter = 0;
    ProductionArea destroyingArea;
    int cooldown = 200;
    int blink = 0;

    void createBlink() {
        blink = rand() % 28;
    }

    void clearBlink() {
        blink = 0;
    }

public:
    ParticleResearch(Vector2d aPos): Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
    }

    void run() override {
        tickCounter++;
        if (tickCounter % 15 == 0) createBlink(); // update lamps every 0.5 seconds
        if (tickCounter % 15 == 5) clearBlink(); // update lamps every 0.5 seconds
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->getBitmap("resources/assets/machinery/ParticleResearch/main.png"), 20, CommonValues::zMachineryBack);
        if (blink) {
            GraphicsEngine::instance()->drawCircle(rect.p1 + Vector2d(4.6, 0.85) + Vector2d((blink % 7 * 0.73), (blink / 7) * 0.73), 0.2, CommonValues::zMachinery, al_map_rgb(255, 255, 255));
        }
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_