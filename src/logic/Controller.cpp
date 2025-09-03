#include "Controller.hpp"
#include "GameWorld.hpp"

void Controller::createWindow() {
    window = GuiEngine::instance()->addWindow(Rect2d::fromCenterAndDimensions(Vector2d(400, 400), Vector2d(400, 750)), true, true);
    int line = 0;
    for (auto item: instructions) {
        InstructionLine instructionLine;
        instructionLine.label = window->addLabel(Vector2d(40, 80), false, item, line);
        instructionLine.breakpointButton = window->addButton(Rect2d::fromTwoCorners(Vector2d(18, 80 + line * 14), Vector2d(32, 94 + line * 14)));
        instructionLine.breakpointIcon = window->addIcon(Vector2d(18 + 7, 80 + line * 14 + 7), GuiEngine::emptyIcon);
        instructionLine.breakpointButton->setOnClickCallback([this, line](){this->toggle(line);});
        instructionsGui.push_back(instructionLine);
        line++;
    }
    InstructionLine lastInstructionLine;
    lastInstructionLine.label = window->addLabel(Vector2d(40, 80), false, "", line);
    instructionsGui.push_back(lastInstructionLine); // last empty line
    rInstrLabel = window->addLabel(Vector2d(20, 30), false, "rInstr: " + std::to_string(rInstr), 0);
    rDelayLabel = window->addLabel(Vector2d(20, 30), false, "rDelay: " + std::to_string(rDelay), 1);

    pauseIcon = window->addIcon(Vector2d(315, 40), GuiEngine::unpauseIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(315, 40), Vector2d(18, 18)))->setOnClickCallback([this](){this->pauseUnpause();});
    window->addIcon(Vector2d(335, 40), GuiEngine::downIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(335, 40), Vector2d(18, 18)))->setOnClickCallback([this](){this->down();});
    window->addIcon(Vector2d(355, 40), GuiEngine::upIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(355, 40), Vector2d(18, 18)))->setOnClickCallback([this](){this->up();});
    window->addIcon(Vector2d(375, 40), GuiEngine::nextIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(375, 40), Vector2d(18, 18)))->setOnClickCallback([this](){this->next();});

    updateWindow();
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
    } else if (command == "goto") {
        rInstr = std::atoi(instr.at(1).c_str()); 
        return 0;
    } else if (command == "error") {
        int error = std::atoi(instr.at(1).c_str());
        return error;
    } else if (command == "send") {
        int id = std::atoi(instr.at(1).c_str()); 
        Machinery* machinery = GameWorld::instance()->getMachinery(id);
        if (!machinery) return 2;
        int command = std::atoi(instr.at(2).c_str());
        int argument = std::atoi(instr.at(3).c_str());
        machinery->onCommandRecive(command, argument);
        rInstr++;
        return 0;
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
