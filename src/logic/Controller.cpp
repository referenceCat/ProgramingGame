#include "Controller.hpp"
#include "GameWorld.hpp"

void Controller::createWindow() {
    window = GuiEngine::getInstance()->addWindow(Rect2d(Point2d(400, 400), 600, 400), true, true);
    int line = 0;
    for (auto item: instructions) {
        instrLabels.push_back(window->addLabel(Point2d(40, 80), false, item, line));
        line++;
    }
    instrLabels.push_back(window->addLabel(Point2d(40, 80), false, "end", line));
    rInstrLabel = window->addLabel(Point2d(20, 30), false, "rInstr: " + std::to_string(rInstr), 0);
    rDelayLabel = window->addLabel(Point2d(20, 30), false, "rDelay: " + std::to_string(rDelay), 1);
}

void Controller::updateWindow() {
    rInstrLabel->setText("rInstr: " + std::to_string(rInstr));
    rDelayLabel->setText("rDelay: " + std::to_string(rDelay));
    
    for (auto item: instrLabels) {
        item->setFlags(LabelFlags());
    }

    LabelFlags flags;
    flags.highlighted = 1;
    instrLabels.at(rInstr)->setFlags(flags);
}

int Controller::execNextInstr() {

    if (window) {
        updateWindow();
    }
    

    if (rInstr >= instructions.size()) {
            return 1; // programm ended
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
        return 1;
    }

    std::string command = instr.at(0);
    if (command == "delay") {
        rDelay = std::atoi(instr.at(1).c_str());
        return 0;
    }

    if (command == "angle") {
        int id = std::atoi(instr.at(1).c_str()); 
        ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
        if (!arm) return 1;
        int joint = std::atoi(instr.at(2).c_str()); 
        int angle = std::atoi(instr.at(3).c_str()); 
        arm->setJointTargetRotation(joint, degreesToRadians(angle));
        rInstr++;
        return 0;
    }

    if (command == "grab") {
        int id = std::atoi(instr.at(1).c_str()); 
        ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
        if (!arm) return 1;
        arm->grab();
        rInstr++;
        return 0;
    }

    if (command == "release") {
        int id = std::atoi(instr.at(1).c_str()); 
        ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
        if (!arm) return 1;
        arm->release();
        rInstr++;
        return 0;
    }
    
    if (command == "goto") {
        rInstr = std::atoi(instr.at(1).c_str()); 
        return 0;
    }

    if (command == "grill") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machine* item = parentWorld->getMachine(id);
        if (!item) return 1;
        item->grillBoxes();
        rInstr++;
        return 0;
    }

    if (command == "create") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machine* item = parentWorld->getMachine(id);
        if (!item) return 1;
        item->createBox();
        rInstr++;
        return 0;
    }

    if (command == "destroy") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machine* item = parentWorld->getMachine(id);
        if (!item) return 1;
        item->destroyBoxes();
        rInstr++;
        return 0;
    }

    return 0;
}