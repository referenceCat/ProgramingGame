#include "Controller.hpp"
#include "GameWorld.hpp"

void Controller::createWindow() {
    window = GuiEngine::getInstance()->addWindow(Rect2d(Point2d(400, 400), 750, 400), true, true);
    int line = 0;
    for (auto item: instructions) {
        InstructionLine instructionLine;
        instructionLine.label = window->addLabel(Point2d(40, 80), false, item, line);
        instructionLine.breakpointButton = window->addButton(Rect2d(Point2d(18, 80 + line * 14), Point2d(32, 94 + line * 14)));
        instructionLine.breakpointIcon = window->addIcon(Point2d(18 + 7, 80 + line * 14 + 7), GuiEngine::emptyIcon);
        instructionLine.breakpointButton->setOnClickCallback([this, line](){this->toggle(line);});
        instructionsGui.push_back(instructionLine);
        line++;
    }
    InstructionLine lastInstructionLine;
    lastInstructionLine.label = window->addLabel(Point2d(40, 80), false, "", line);
    instructionsGui.push_back(lastInstructionLine); // last empty line
    rInstrLabel = window->addLabel(Point2d(20, 30), false, "rInstr: " + std::to_string(rInstr), 0);
    rDelayLabel = window->addLabel(Point2d(20, 30), false, "rDelay: " + std::to_string(rDelay), 1);

    pauseIcon = window->addIcon(Point2d(315, 40), GuiEngine::unpauseIcon);
    window->addButton(Rect2d(Point2d(315, 40), 18, 18))->setOnClickCallback([this](){this->pauseUnpause();});
    window->addIcon(Point2d(335, 40), GuiEngine::downIcon);
    window->addButton(Rect2d(Point2d(335, 40), 18, 18))->setOnClickCallback([this](){this->down();});
    window->addIcon(Point2d(355, 40), GuiEngine::upIcon);
    window->addButton(Rect2d(Point2d(355, 40), 18, 18))->setOnClickCallback([this](){this->up();});
    window->addIcon(Point2d(375, 40), GuiEngine::nextIcon);
    window->addButton(Rect2d(Point2d(375, 40), 18, 18))->setOnClickCallback([this](){this->next();});
}

void Controller::updateWindow() {
    if (!window) {
        return;
    }

    rInstrLabel->setText("rInstr: " + std::to_string(rInstr));
    rDelayLabel->setText("rDelay: " + std::to_string(rDelay));
    
    for (auto item: instructionsGui) {
        item.label->setFlags(LabelFlags());
    }

    LabelFlags flags;
    flags.background = true;
    instructionsGui.at(rInstr).label->setFlags(flags);
    if (failure) {
        instructionsGui.at(rInstr).label->setBackgroundColor(al_map_rgb(150, 0, 0));
    } else if (paused) {
        instructionsGui.at(rInstr).label->setBackgroundColor(al_map_rgb(150, 150, 0));
    } else {
        instructionsGui.at(rInstr).label->setBackgroundColor(al_map_rgb(100, 100, 100));
    }

    if (paused) {
        pauseIcon->setBitmap(GuiEngine::unpauseIcon);
    } else {
        pauseIcon->setBitmap(GuiEngine::pauseIcon);
    }
    
}

int Controller::execNextInstr() {
    if (rInstr >= instructions.size()) {
            return 3; // program ended
    }

    if (breakpoints.at(rInstr) && !paused) {
        pause();
        return 0;
    }

    if (rDelay) {
        rDelay--;
        if (rDelay == 0) {
            rInstr++;
        }
        return 0;
    }

    std::vector<std::string> instr = split(instructions.at(rInstr), " ");
    if (instr.size() < 1) {
        return 1; // invalid instruction
    }

    std::string command = instr.at(0);
    if (command == "delay") {
        rDelay = std::atoi(instr.at(1).c_str());
        return 0;
    }

    if (command == "angle") {
        int id = std::atoi(instr.at(1).c_str()); 
        ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
        if (!arm) return 2;
        int joint = std::atoi(instr.at(2).c_str()); 
        int angle = std::atoi(instr.at(3).c_str()); 
        arm->setJointTargetRotation(joint, degreesToRadians(angle));
        rInstr++;
        return 0;
    } else if (command == "grab") {
        int id = std::atoi(instr.at(1).c_str()); 
        ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
        if (!arm) return 2;
        arm->grab();
        rInstr++;
        return 0;
    } else if (command == "release") {
        int id = std::atoi(instr.at(1).c_str()); 
        ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
        if (!arm) return 2;
        arm->release();
        rInstr++;
        return 0;
    } else if (command == "goto") {
        rInstr = std::atoi(instr.at(1).c_str()); 
        return 0;
    // } else if (command == "grill") {
    //     int id = std::atoi(instr.at(1).c_str()); 
    //     Machine* item = parentWorld->getMachine(id);
    //     if (!item) return 2; // incorrect slave address
    //     item->grillBoxes();
    //     rInstr++;
    //     return 0;
    // } else if (command == "create") {
    //     int id = std::atoi(instr.at(1).c_str()); 
    //     Machine* item = parentWorld->getMachine(id);
    //     if (!item) return 2; // incorrect slave address
    //     item->createBox();
    //     rInstr++;
    //     return 0;
    // } else if (command == "destroy") {
    //     int id = std::atoi(instr.at(1).c_str()); 
    //     Machine* item = parentWorld->getMachine(id);
    //     if (!item) return 2; // incorrect slave address
    //     item->destroyBoxes();
    //     rInstr++;
    //     return 0;
    } else if (command == "error") {
        int error = std::atoi(instr.at(1).c_str());
        return error;
    } else {
        return 1; // invalid instruction
    }

    return 0;
}

void Controller::toggle(int line) {
    assert(line < instructions.size());
    breakpoints.at(line) = !breakpoints.at(line);
    if (window) {
        if (breakpoints.at(line)) instructionsGui.at(line).breakpointIcon->setBitmap(GuiEngine::breakpointIcon);
        else instructionsGui.at(line).breakpointIcon->setBitmap(GuiEngine::emptyIcon);
    }
}
