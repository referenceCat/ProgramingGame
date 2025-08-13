#include "Controller.hpp"
#include "GameWorld.hpp"

void Controller::createWindow() {
    window = GuiEngine::getInstance()->addWindow(Rect2d(Point2d(400, 400), 600, 400), true, true);
    int line = 0;
    for (auto item: instructions) {
        instrLabels.push_back(window->addLabel(Point2d(40, 80), false, item, line));
        line++;
    }
    instrLabels.push_back(window->addLabel(Point2d(40, 80), false, "", line)); // last empty line
    rInstrLabel = window->addLabel(Point2d(20, 30), false, "rInstr: " + std::to_string(rInstr), 0);
    rDelayLabel = window->addLabel(Point2d(20, 30), false, "rDelay: " + std::to_string(rDelay), 1);

    window->addIcon(Point2d(295, 40), GuiEngine::pauseIcon);
    window->addButton(Rect2d(Point2d(295, 40), 18, 18));
    window->addIcon(Point2d(315, 40), GuiEngine::unpauseIcon);
    window->addButton(Rect2d(Point2d(315, 40), 18, 18));
    window->addIcon(Point2d(335, 40), GuiEngine::downIcon);
    window->addButton(Rect2d(Point2d(335, 40), 18, 18));
    window->addIcon(Point2d(355, 40), GuiEngine::upIcon);
    window->addButton(Rect2d(Point2d(355, 40), 18, 18));
    window->addIcon(Point2d(375, 40), GuiEngine::nextIcon);
    window->addButton(Rect2d(Point2d(375, 40), 18, 18));
}

void Controller::updateWindow() {
    if (!window) {
        return;
    }

    rInstrLabel->setText("rInstr: " + std::to_string(rInstr));
    rDelayLabel->setText("rDelay: " + std::to_string(rDelay));
    
    for (auto item: instrLabels) {
        item->setFlags(LabelFlags());
    }

    LabelFlags flags;
    flags.highlighted = true;
    instrLabels.at(rInstr)->setFlags(flags);
    if (failure) {
        instrLabels.at(rInstr)->setBackgroundColor(al_map_rgb(150, 0, 0));
    } else if (paused) {
        instrLabels.at(rInstr)->setBackgroundColor(al_map_rgb(150, 150, 0));
    } else {
        instrLabels.at(rInstr)->setBackgroundColor(al_map_rgb(100, 100, 100));
    }
    
}

int Controller::execNextInstr() {
    if (rInstr >= instructions.size()) {
            return 3; // program ended
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
    } else if (command == "grill") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machine* item = parentWorld->getMachine(id);
        if (!item) return 2; // incorrect slave address
        item->grillBoxes();
        rInstr++;
        return 0;
    } else if (command == "create") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machine* item = parentWorld->getMachine(id);
        if (!item) return 2; // incorrect slave address
        item->createBox();
        rInstr++;
        return 0;
    } else if (command == "destroy") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machine* item = parentWorld->getMachine(id);
        if (!item) return 2; // incorrect slave address
        item->destroyBoxes();
        rInstr++;
        return 0;
    } else if (command == "error") {
        int error = std::atoi(instr.at(1).c_str());
        return error;
    } else if (command == "break") {
        paused = true;
        rInstr++;
        return 0;
    } else {
        return 1; // invalid instruction
    }

    return 0;
}