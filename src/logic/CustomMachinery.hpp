#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_

#include <math.h>
#include <cstdlib>
#include <format>
#include <string>
#include <functional>
#include "Machinery.hpp"
#include "GraphicsEngine.hpp"

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

class AddressSelectionWindow {
    Window* window = nullptr;
    Button* addressButtons[16][16] = {nullptr};
    Label* addressLabels[16][16] = {nullptr};
    int address = 0;
    std::function<void(int)> onAddressButtonClicked = nullptr;

public:
    AddressSelectionWindow(int selectedAddress, std::function<void(int)> onAddressButtonClicked):
        onAddressButtonClicked{onAddressButtonClicked} {
        window = new Window(GuiEngine::instance()->getDisplayArea(), Aligment::byDimensionsAndCentered(Vector2d(640, 640)), true);
        window->setDrawPriority(2);
        for (int i = 0; i < 256; i++) {
            addressButtons[i / 16][i % 16] = new Button(window->getInternalArea(), AligmentBuilder().margin(3, 3, 3, 3).tableDimensions(16, 16).tableCell(i % 16, i / 16));
            addressButtons[i / 16][i % 16]->setMouseCallback(Release, [this, i](auto pos) { this->onAddressButtonClick(i); });
            addressLabels[i / 16][i % 16] = new Label(addressButtons[i / 16][i % 16], Aligment(), std::to_string(i));
        }
        setSelectedAddress(selectedAddress);
    };

    ~AddressSelectionWindow() {
        delete window;
    }

    void setSelectedAddress(int address) {
        assert(address >= 0 && address <= 255);
        this->address = address;
        for (int i = 0; i < 256; i++) {
            addressLabels[i / 16][i % 16]->setText(std::to_string(i));
        }
        addressLabels[address / 16][address % 16]->setText(std::format("({})", address));
    };

    void onAddressButtonClick(int address) {
        setSelectedAddress(address);
        onAddressButtonClicked(address);
        // delete this;
    }
};

class Manipulator : public Machinery {
    Arm* arm = nullptr; // game logic
    Vector2d target;
    bool manualMode = false;
    Vector2d manualTarget;

    Window* window = nullptr; // gui // TODO move to its own class
    Label* manualLabel = nullptr;
    Label* grabLabel = nullptr;
    Label* addressLabel = nullptr;
    Console* memoryConsole = nullptr;

    Vector2d leftClampPos, rightClampPos;

    static double q2(double x, double y, double l1, double l2) { // TODO
        return std::acos((x * x + y * y - l1 * l1 - l2 * l2) / (2 * l1 * l2));
    }

    static double q1(double x, double y, double l1, double l2) {
        double q2_value = q2(x, y, l1, l2);
        return std::atan2(y, x) - std::atan2(l2 * std::sin(q2_value), l1 + l2 * std::cos(q2_value));
    }

    void createWindow() {
        if (window)
            return;
        window = new Window(GuiEngine::instance()->getDisplayArea(), AligmentBuilder().dimensions({720, 300}).margin(-1, -1, -1, 30), true);
        window->setOnCloseCallback([this]() { this->onWindowClose(); });
        window->setDrawPriority(2);

        auto manualControlArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(0, 0).margin(20, 20, 10, 20), "Manual control");
        auto optionsArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(1, 0).margin(10, 20, 10, 20), "Settings");
        auto memoryArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(2, 0).margin(10, 20, 20, 20), "Memory");

        auto addressButtonAligment = Aligment::byMargin(5, 5, 5, 5);
        addressButtonAligment.tableRows = 5;
        addressButtonAligment.ownRow = 0;
        auto addressButton = new Button(optionsArea->getInternalArea(), addressButtonAligment);
        addressButton->setMouseCallback(Release, [this](auto pos) { new AddressSelectionWindow(getAddress(), [this](int address) { this->setAddress(address); }); });
        addressLabel = new Label(addressButton, Aligment(), std::format("Addr: {}", getAddress()));

        Aligment buttonAligment = Aligment::byMargin(5, 5, 5, 5);
        buttonAligment.tableColumns = 3;
        buttonAligment.tableRows = 3;

        buttonAligment.ownColumn = 0;
        buttonAligment.ownRow = 0;
        auto manualButton = new Button(manualControlArea->getInternalArea(), buttonAligment);
        manualButton->setMouseCallback(Release, [this](auto pos) { this->onManualControlButtonClick(); });
        manualLabel = new Label(manualButton, Aligment(), manualMode ? "manual" : "auto");

        buttonAligment.ownColumn = 1;
        buttonAligment.ownRow = 1;
        auto grabButton = new Button(manualControlArea->getInternalArea(), buttonAligment);
        grabButton->setMouseCallback(Release, [this](auto pos) { this->onGrabButtonClick(); });
        grabLabel = new Label(grabButton, Aligment(), arm->isActive() ? "release" : "grab");

        buttonAligment.ownColumn = 1;
        buttonAligment.ownRow = 0;
        auto moveButton = new Button(manualControlArea->getInternalArea(), buttonAligment);
        moveButton->setMouseCallback(Hold, [this](auto pos) { setManualTarget(this->manualTarget + Vector2d(0, -0.1)); });
        new Label(moveButton, Aligment(), "up");

        buttonAligment.ownColumn = 1;
        buttonAligment.ownRow = 2;
        moveButton = new Button(manualControlArea->getInternalArea(), buttonAligment);
        moveButton->setMouseCallback(Hold, [this](auto pos) { setManualTarget(this->manualTarget + Vector2d(0, 0.1)); });
        new Label(moveButton, Aligment(), "down");

        buttonAligment.ownColumn = 0;
        buttonAligment.ownRow = 1;
        moveButton = new Button(manualControlArea->getInternalArea(), buttonAligment);
        moveButton->setMouseCallback(Hold, [this](auto pos) { setManualTarget(this->manualTarget + Vector2d(-0.1, 0)); });
        new Label(moveButton, Aligment(), "left");

        buttonAligment.ownColumn = 2;
        buttonAligment.ownRow = 1;
        moveButton = new Button(manualControlArea->getInternalArea(), buttonAligment);
        moveButton->setMouseCallback(Hold, [this](auto pos) { setManualTarget(this->manualTarget + Vector2d(0.1, 0)); });
        new Label(moveButton, Aligment(), "right");

        memoryConsole = new Console(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(1, 0).margin(5, 5, 5, 5));
        memoryConsole->setEditable(false);
        new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 1).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "status->"), -1)), "status->");
        new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 2).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "grab->"), -1)), "grab->");
        new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 3).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "target x->"), -1)), "target x->");
        new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 4).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "target y->"), -1)), "target y->");
    }

    void updateMemoryConsole() {
        if (window == nullptr)
            return;
        memoryConsole->clearLines();
        for (int i = 0; i < getMemorySize(); i++) {
            if (i == 0)
                memoryConsole->setLines({std::to_string(getMemoryValue(i))});
            else
                memoryConsole->addLine(std::to_string(getMemoryValue(i)));
        }
    }

    void onWindowClose() {
        window = nullptr;
    }

    void onManualControlButtonClick() {
        manualMode = !manualMode;
        manualLabel->setText(manualMode ? "manual" : "auto");

        if (manualMode) {
            manualTarget = arm->getLastJointPos();
        } else {
            arm->cleatTarget();
        }
    }

    void onGrabButtonClick() {
        if (arm->isActive()) {
            arm->release();
        } else {
            arm->grab();
        }
        grabLabel->setText(arm->isActive() ? "release" : "grab");
    }

    void setManualTarget(Vector2d aPos) {
        manualTarget = aPos;
        if (manualMode)
            setGlobalTarget(manualTarget);
    }

    void updateMemory() {
        MemoryWord word0 = 0; // TODO should be uniformal across all machinery (e.g. 1 bit is working, 2 is power, 3 ...)
        word0 |= (manualMode << 0);
        word0 |= (arm->isActive() << 1);
        word0 |= ((arm->getTakenBox() != nullptr) << 2);
        word0 |= (((arm->getJointTargetPosition(2) - arm->getJointPosition(2)).lenght() < 0.01) << 3); // arm reached its target
        setMemoryValue(0, word0);

        if (manualMode)
            return;
        MemoryWord word1 = getMemoryValue(1);
        if (word1)
            arm->grab();
        else
            arm->release();

        setRelativeTarget(Vector2d(getMemoryValue(2), getMemoryValue(3)));
    }

public:
    Manipulator(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(5, 3)), 10) {
        manualMode == true;
        setMemoryValue(2, 5);
        setMemoryValue(3, 0);
    }

    void run() override {
        updateMemory();
        if (arm) {
            if (leftClampPos == Vector2d() && rightClampPos == Vector2d()) { // on arm initialisation clamps located at 0, 0 and it causes ugly animation
                leftClampPos = arm->getLastJointPos();
                rightClampPos = arm->getLastJointPos();
            }

            leftClampPos.y = arm->getLastJointPos().y;
            rightClampPos.y = arm->getLastJointPos().y;
            double leftClampTargetX = arm->getLastJointPos().x - 1;
            double rightClampTargetX = arm->getLastJointPos().x + 1;
            if (arm->getTakenBox() != nullptr) {
                leftClampTargetX = arm->getTakenBox()->getRect().p1.x;
                rightClampTargetX = arm->getTakenBox()->getRect().p2.x;
            }

            leftClampPos.x = leftClampPos.x / 2 + leftClampTargetX / 2;
            rightClampPos.x = rightClampPos.x / 2 + rightClampTargetX / 2;
        }
        updateMemoryConsole();
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/Base/main.png"), 20, 0.1); // base

        if (arm) {
            if (manualMode) {
                GraphicsEngine::instance()->drawLine(manualTarget, arm->getJointPosition(2), CommonValues::zDebug, al_map_rgb(255, 50, 50));
                GraphicsEngine::instance()->drawLine(arm->getJointPosition(0), arm->getJointPosition(1), CommonValues::zDebug, al_map_rgb(255, 255, 255));
                GraphicsEngine::instance()->drawLine(arm->getJointPosition(1), arm->getJointPosition(2), CommonValues::zDebug, al_map_rgb(255, 255, 255));

                GraphicsEngine::instance()->drawCircle(arm->getJointPosition(0), 0.3, CommonValues::zDebug, al_map_rgb(255, 255, 255), 0);
                GraphicsEngine::instance()->drawCircle(arm->getJointPosition(1), 0.3, CommonValues::zDebug, al_map_rgb(255, 255, 255), 0);
                GraphicsEngine::instance()->drawCircle(arm->getJointPosition(2), 0.3, CommonValues::zDebug, al_map_rgb(255, 255, 255), 0);
                GraphicsEngine::instance()->drawLine(manualTarget - Vector2d(0.35, 0.35), manualTarget + Vector2d(0.35, 0.35), CommonValues::zDebug, al_map_rgb(255, 255, 255));
                GraphicsEngine::instance()->drawLine(manualTarget - Vector2d(-0.35, 0.35), manualTarget + Vector2d(-0.35, 0.35), CommonValues::zDebug, al_map_rgb(255, 255, 255));
                GraphicsEngine::instance()->drawLine(manualTarget - Vector2d(-0.35, 0.35), manualTarget + Vector2d(-0.35, 0.35), CommonValues::zDebug, al_map_rgb(255, 255, 255));
                if (arm->isActive()) {
                    GraphicsEngine::instance()->drawCircle(arm->getJointPosition(2), 0.7, CommonValues::zDebug, al_map_rgb(255, 255, 255), 0.1);
                }
            }
            GraphicsEngine::instance()->drawLine(arm->getLastJointPos(), leftClampPos, CommonValues::zArm, al_map_rgb(50, 60, 50), 0.2); // left clamp
            GraphicsEngine::instance()->drawLine(arm->getLastJointPos(), leftClampPos, CommonValues::zArm, al_map_rgb(70, 80, 70), 0.1);
            GraphicsEngine::instance()->drawBitmap(leftClampPos + Vector2d(0.2, 0), GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/End1/main.png"), 20, CommonValues::zArm, Vector2d(20, 20), Rotation::fromDegrees(180));

            GraphicsEngine::instance()->drawLine(arm->getLastJointPos(), rightClampPos, CommonValues::zArm, al_map_rgb(50, 60, 50), 0.2); // right clamp
            GraphicsEngine::instance()->drawLine(arm->getLastJointPos(), rightClampPos, CommonValues::zArm, al_map_rgb(70, 80, 70), 0.1);
            GraphicsEngine::instance()->drawBitmap(rightClampPos + Vector2d(-0.2, 0), GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/End1/main.png"), 20, CommonValues::zArm, Vector2d(20, 20));

            GraphicsEngine::instance()->drawBitmap(arm->getLastJointPos(), GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/End0/main.png"), 20, CommonValues::zArm, Vector2d(20, 20)); // arm
            GraphicsEngine::instance()->drawBitmap(arm->getJointPosition(1), GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/Segment1/main.png"), 20, CommonValues::zArm, Vector2d(20, 40), arm->getJointRotation(0) + arm->getJointRotation(1));
            GraphicsEngine::instance()->drawBitmap(arm->getJointPosition(0), GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Manipulator/Segment0/main.png"), 20, CommonValues::zArm, Vector2d(20, 40), arm->getJointRotation(0));
        }
    }

    void drawInfo() override {
        double lineLenght = 2;
        GraphicsEngine::instance()->drawArc(arm->getJointPosition(0), Rotation(0), arm->getJointRotation(0), 1, CommonValues::zDebug, al_map_rgb(255, 255, 255));
        GraphicsEngine::instance()->drawLine(arm->getJointPosition(0), arm->getJointPosition(0) + Vector2d(Rotation(0), lineLenght), CommonValues::zDebug, al_map_rgb(255, 255, 255));
        GraphicsEngine::instance()->drawLine(arm->getJointPosition(0), arm->getJointPosition(0) + Vector2d(arm->getJointRotation(0), lineLenght), CommonValues::zDebug, al_map_rgb(255, 255, 255));
        GraphicsEngine::instance()->drawText(arm->getJointPosition(0) + Vector2d(1, 1), format("{:.0f}", arm->getJointRotation(0).degress()), GraphicsEngine::instance()->debugFont, CommonValues::zDebug, al_map_rgb(255, 255, 255), false);

        GraphicsEngine::instance()->drawArc(arm->getJointPosition(1), arm->getJointRotation(0), arm->getJointRotation(1) + arm->getJointRotation(0), 1, CommonValues::zDebug, al_map_rgb(255, 255, 255));
        GraphicsEngine::instance()->drawLine(arm->getJointPosition(1), arm->getJointPosition(1) + Vector2d(arm->getJointRotation(0), lineLenght), CommonValues::zDebug, al_map_rgb(255, 255, 255));
        GraphicsEngine::instance()->drawLine(arm->getJointPosition(1), arm->getJointPosition(1) + Vector2d(arm->getJointRotation(1) + arm->getJointRotation(0), lineLenght), CommonValues::zDebug, al_map_rgb(255, 255, 255));
        GraphicsEngine::instance()->drawText(arm->getJointPosition(1) + Vector2d(1, 1), format("{:.0f}", arm->getJointRotation(1).degress()), GraphicsEngine::instance()->debugFont, CommonValues::zDebug, al_map_rgb(255, 255, 255), false);
    }

    void setGlobalTarget(Vector2d pos) {
        setRelativeTarget(pos - arm->getJointPosition(0));
    }

    void setRelativeTarget(Vector2d pos) {
        if (pos.lenght() >= 6 + 8) { // target is too far
            arm->setJointTargetRotation(0, pos.getDirection());
            arm->setJointTargetRotation(1, Rotation());
            return;
        }

        if (pos.lenght() <= 8 - 6) { // target is too close
            arm->setJointTargetRotation(0, pos.getDirection());
            arm->setJointTargetRotation(1, Rotation::fromDegrees(180));
            return;
        }

        arm->setJointTargetRotation(0, q1(pos.x, pos.y, 8, 6));
        arm->setJointTargetRotation(1, q2(pos.x, pos.y, 8, 6));
    }

    void addToGameWorld() override { // TODO
        Machinery::addToGameWorld();
        arm = new Arm(3);
        arm->setRootJointPosition(Vector2d(4, 2) + rect.p1);
        arm->setSegmentLength(0, 8);
        arm->setSegmentTargetLength(0, 8);
        arm->setSegmentLength(1, 6);
        arm->setSegmentTargetLength(1, 6);
        arm->recalculate();
        arm->addToGameWorld();
    }

    void onClick() override {
        if (window == nullptr)
            createWindow();
    }

    void setAddress(int id) override {
        Machinery::setAddress(id);
        if (window)
            addressLabel->setText(std::format("Address: {}", id));
    }
};

class Drill : public Machinery {
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
    Drill(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
        miningProcess.duration = 500;
        output0 = ProductionArea{Rect2d::fromCenterAndDimensions(Vector2d(7.5, 2.5), Vector2d(5, 5))};
        areas.push_back(&output0);
        output1 = ProductionArea{Rect2d::fromCenterAndDimensions(Vector2d(7.5, 7.5), Vector2d(5, 5))};
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
        GraphicsEngine::instance()->drawArcProgressBar(rect.p1 + Vector2d(1.8, 1.8), static_cast<double>(miningProcess.progress) / miningProcess.duration, 0.7, CommonValues::zMachinery, al_map_rgb(255, 255, 255), 0.2);

        for (int y = rect.p2.y; y < GameWorld::instance()->surfaceY + 10; y += 10) { // draw drill to the ground level
            GraphicsEngine::instance()->drawBitmap(Vector2d(rect.p1.x, y) + detailShift * 0.5, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Drill/drill.png"), 20, CommonValues::zMachineryBack);
        }
    }
};

class Electrolyzer : public Machinery {
    ProductionArea input0;
    ProductionArea output0;
    ProductionArea output1;
    ProductionArea output2;
    ProductionProcess process;

public:
    Electrolyzer(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
        process.duration = 500;
        input0 = ProductionArea{Rect2d::fromCenterAndDimensions(Vector2d(2.5, 2.5), Vector2d(5, 5))};
        areas.push_back(&input0);
        output0 = ProductionArea{Rect2d::fromCenterAndDimensions(Vector2d(1.6, 8.3), Vector2d(3.33, 3.33))};
        areas.push_back(&output0);
        output1 = ProductionArea{Rect2d::fromCenterAndDimensions(Vector2d(5, 8.3), Vector2d(3.33, 3.33))};
        areas.push_back(&output1);
        output2 = ProductionArea{Rect2d::fromCenterAndDimensions(Vector2d(8.3, 8.3), Vector2d(3.33, 3.33))};
        areas.push_back(&output2);
    }

    void run() override {
        if (process.status == WaitingToStart) {
            if (getBoxesInside(input0).size()) {
                auto box = getBoxesInside(input0).at(0);
                if (box->isGrabbed())
                    return;
                if (dynamic_cast<ResourceBoxPrototype*>(box) == nullptr)
                    return; // check if box is resource box
                if (dynamic_cast<ResourceBoxPrototype*>(box)->getResource() != Regolith)
                    return;
                process.status = Running;
                process.progress = 0;
                destroyBox(box);
            }
        }

        if (process.status == Running) {
            if (process.progress == process.duration) {
                process.status = WaitingToFinish;
            } else {
                process.progress++;
            }
        }

        if (process.status == WaitingToFinish) {
            if (getBoxesTouching(output0).size() == 0 && getBoxesTouching(output1).size() == 0 && getBoxesTouching(output2).size() == 0) {
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
        GraphicsEngine::instance()->drawArcProgressBar(rect.p1 + Vector2d(6.6, 4.4), static_cast<double>(process.progress) / process.duration, 0.7, CommonValues::zMachinery, al_map_rgb(255, 255, 255), 0.2);
    }
};

class Lab : public Machinery {
    long tickCounter = 0;
    bool lamps[16][32] = {0};

    ProductionArea researchArea[6];
    ProductionProcess researchProcess[6];
    TapeBox* processedBox[6];

    void updateLamps() {
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 32; j++) {
                lamps[i][j] = (rand() % 3) == 0;
            }
    }

    void readPoints(TapeBox* box) {
        for (int i = 0; i < TapeBox::dataPointsSize; i++) {
            if (box->getDataPoint(TapeBox::dataPointsSize - 1 - i)) {
                box->setDataPoint(TapeBox::dataPointsSize - 1 - i, NoData);
                return;
            }
        }
    }

    void runResearch(int i) {
        if (researchProcess[i].status == WaitingToStart) {
            if (getBoxesInside(researchArea[i]).size()) {
                auto box = getBoxesInside(researchArea[i]).at(0);
                if (box->isGrabbed())
                    return;
                if (dynamic_cast<TapeBox*>(box) == nullptr)
                    return; // check if box is resource box
                if (dynamic_cast<TapeBox*>(box)->isDataPointsEmpty())
                    return;
                researchProcess[i].status = Running;
                researchProcess[i].progress = 0;
                processedBox[i] = dynamic_cast<TapeBox*>(box);
            }
        }

        if (researchProcess[i].status == Running) {
            if (getBoxesInside(researchArea[i]).size() == 0 || getBoxesInside(researchArea[i]).at(0) != processedBox[i] || processedBox[i]->isGrabbed()) { // check if processed boxed was removed
                processedBox[i] = nullptr;
                researchProcess[i].status = WaitingToStart;
                researchProcess[i].progress = 0;
            }

            if (researchProcess[i].progress == researchProcess[i].duration) {
                researchProcess[i].status = WaitingToFinish;
            } else {
                researchProcess[i].progress++;
            }
        }

        if (researchProcess[i].status == WaitingToFinish) {
            readPoints(processedBox[i]);
            processedBox[i] = nullptr;
            researchProcess[i].status = WaitingToStart;
            researchProcess[i].progress = 0;
        }
    }

public:
    Lab(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(17, 17))) {
        researchArea[0].rect = Rect2d::fromCenterAndDimensions(Vector2d(3, 2), Vector2d(5, 3));
        areas.push_back(&researchArea[0]);
        researchArea[1].rect = Rect2d::fromCenterAndDimensions(Vector2d(8.5, 2), Vector2d(5, 3));
        areas.push_back(&researchArea[1]);
        researchArea[2].rect = Rect2d::fromCenterAndDimensions(Vector2d(14, 2), Vector2d(5, 3));
        areas.push_back(&researchArea[2]);
        researchArea[3].rect = Rect2d::fromCenterAndDimensions(Vector2d(3, 15), Vector2d(5, 3));
        areas.push_back(&researchArea[3]);
        researchArea[4].rect = Rect2d::fromCenterAndDimensions(Vector2d(8.5, 15), Vector2d(5, 3));
        areas.push_back(&researchArea[4]);
        researchArea[5].rect = Rect2d::fromCenterAndDimensions(Vector2d(14, 15), Vector2d(5, 3));
        areas.push_back(&researchArea[5]);
    }

    void run() override {
        tickCounter++;
        if (tickCounter % 30 == 0)
            updateLamps(); // update lamps every 0.5 seconds
        for (int i = 0; i < 6; i++) runResearch(i);
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Lab/background.png"), 20, CommonValues::zMachineryBack);
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Lab/main.png"), 20, CommonValues::zMachinery);

        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 32; j++) {
                if (lamps[i][j] && !(Rect2d(Vector2d(24, 9), Vector2d(31, 15)).isInside(Vector2d(j, i))))
                    GraphicsEngine::instance()->drawCircle(Vector2d(j * 0.5, i * 0.5) + rect.p1 + Vector2d(0.75, 4.75), 0.1, CommonValues::zMachinery, al_map_rgba(255, 255, 100, 100));
            }
    }
};

class Analyzer : public Machinery {
    ProductionArea tapeArea;
    ProductionArea sampleArea;
    ProductionProcess researchProcess;
    std::vector<DataPointType> resultingDataPoints = {};

    std::vector<DataPointType> getDataPoints(Resource resource) {
        if (resource == Regolith)
            return {GeologyResearchData};
        if (resource == Alloy)
            return {MaterialResearchData, MaterialResearchData};
        if (resource == Silicon)
            return {MaterialResearchData};
        return {};
    }

    void writePointsToTapeBox(TapeBox* box) { // excesive data points are lost
        for (auto point : resultingDataPoints) {
            box->writePoint(point);
        }
        resultingDataPoints.clear();
    }

public:
    Analyzer(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(7, 10))) {
        tapeArea = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(2.2, 1.5), Vector2d(5, 3)));
        areas.push_back(&tapeArea);
        sampleArea = ProductionArea(Rect2d::fromCenterAndDimensions(Vector2d(3.5, 7.5), Vector2d(5, 5)));
        areas.push_back(&sampleArea);
    }

    void run() override {
        if (researchProcess.status == WaitingToStart) {
            if (getBoxesInside(sampleArea).size()) {
                auto box = getBoxesInside(sampleArea).at(0);
                if (box->isGrabbed())
                    return;
                if (dynamic_cast<ResourceBoxPrototype*>(box) == nullptr)
                    return; // check if box is resource box
                researchProcess.status = Running;
                researchProcess.progress = 0;
                resultingDataPoints = getDataPoints(dynamic_cast<ResourceBoxPrototype*>(box)->getResource());
                destroyBox(box);
            }
        }

        if (researchProcess.status == Running) {
            if (researchProcess.progress == researchProcess.duration) {
                researchProcess.status = WaitingToFinish;
            } else {
                researchProcess.progress++;
            }
        }

        if (researchProcess.status == WaitingToFinish) {
            if (getBoxesInside(tapeArea).size() == 0)
                return;
            auto box = getBoxesInside(tapeArea).at(0);
            if (box->isGrabbed())
                return;
            if (dynamic_cast<TapeBox*>(box) == nullptr)
                return; // check if box is resource box
            researchProcess.status = WaitingToStart;
            researchProcess.progress = 0;
            writePointsToTapeBox(dynamic_cast<TapeBox*>(box));
        }
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Analyzer/main.png"), 20, CommonValues::zMachineryFront);
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/Analyzer/background.png"), 20, CommonValues::zMachineryBack);
        GraphicsEngine::instance()->drawArcProgressBar(rect.p1 + Vector2d(5.4, 4.5), static_cast<double>(researchProcess.progress) / researchProcess.duration, 0.7, CommonValues::zMachineryFront, al_map_rgb(255, 255, 255), 0.2);
    }
};

class ParticleDetector : public Machinery {
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
    ParticleDetector(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(10, 10))) {
    }

    void run() override {
        tickCounter++;
        if (tickCounter % 15 == 0)
            createBlink(); // update lamps every 0.5 seconds
        if (tickCounter % 15 == 5)
            clearBlink(); // update lamps every 0.5 seconds
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1, GraphicsEngine::instance()->getBitmap("resources/assets/machinery/ParticleResearch/main.png"), 20, CommonValues::zMachineryBack);
        if (blink) {
            GraphicsEngine::instance()->drawCircle(rect.p1 + Vector2d(4.6, 0.85) + Vector2d((blink % 7 * 0.73), (blink / 7) * 0.73), 0.2, CommonValues::zMachinery, al_map_rgb(255, 255, 255));
        }
    }
};

class ProductionMachineryPrototype : public Machinery {
    
};
#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINERY_HPP_