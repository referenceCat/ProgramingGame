#include "Controller.hpp"
#include "GameWorld.hpp"
#include <iostream>
#include <fstream>
#include <string>

void Controller::createWindow() {
    window = GuiEngine::instance()->addWindow(Rect2d::fromCenterAndDimensions(Vector2d(400, 400), Vector2d(400, 750)), true, true);

    rInstrLabel = window->addLabel(Vector2d(20, 30), false, "rInstr: " + std::to_string(rInstr), 0);
    rDelayLabel = window->addLabel(Vector2d(20, 30), false, "rDelay: " + std::to_string(rDelay), 1);

    window->addLabel(Vector2d(270, 40), true, "open");
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(270, 40), Vector2d(60, 18)))->setOnClickCallback([this]() { this->openFile(); });

    pauseIcon = window->addIcon(Vector2d(315, 40), GuiEngine::unpauseIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(315, 40), Vector2d(18, 18)))->setOnClickCallback([this]() { this->pauseUnpause(); });
    window->addIcon(Vector2d(335, 40), GuiEngine::downIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(335, 40), Vector2d(18, 18)))->setOnClickCallback([this]() { this->down(); });
    window->addIcon(Vector2d(355, 40), GuiEngine::upIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(355, 40), Vector2d(18, 18)))->setOnClickCallback([this]() { this->up(); });
    window->addIcon(Vector2d(375, 40), GuiEngine::nextIcon);
    window->addButton(Rect2d::fromCenterAndDimensions(Vector2d(375, 40), Vector2d(18, 18)))->setOnClickCallback([this]() { this->next(); });
    window->setOnCloseCallback([this]() {this->window = nullptr; instructionsGui.clear(); });
    updateWindow();
}

void Controller::updateWindow() {
    if (!window) {
        return;
    }

    for (auto line : instructionsGui) {
        window->deleteButton(line.breakpointButton);
        window->deleteIcon(line.breakpointIcon);
        window->deleteLabel(line.label);
    }

    instructionsGui.clear();

    int line = 0;
    for (auto item : instructions) {
        InstructionLine instructionLine;
        instructionLine.label = window->addLabel(Vector2d(40, 80), false, item, line);
        instructionLine.breakpointButton = window->addButton(Rect2d::fromTwoCorners(Vector2d(18, 80 + line * 14), Vector2d(32, 94 + line * 14)));
        instructionLine.breakpointIcon = window->addIcon(Vector2d(18 + 7, 80 + line * 14 + 7), GuiEngine::emptyIcon);
        instructionLine.breakpointButton->setOnClickCallback([this, line]() { this->toggle(line); });
        instructionsGui.push_back(instructionLine);
        line++;
    }

    rInstrLabel->setText("rInstr: " + std::to_string(rInstr));
    rDelayLabel->setText("rDelay: " + std::to_string(rDelay));

    for (auto item : instructionsGui) {
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

    if (instructions.at(rInstr).size() == 0 || instructions.at(rInstr).at(0) == '#') {
        rInstr++;
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
        int address = std::atoi(instr.at(1).c_str());
        int command = std::atoi(instr.at(2).c_str());
        int argument = std::atoi(instr.at(3).c_str());

        for (auto item : GameWorld::instance()->getMachinery()) {
            if (item->getAddress() == address)
                item->onCommandRecive(command, argument);
        }

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
        if (breakpoints.at(line))
            instructionsGui.at(line).breakpointIcon->setBitmap(GuiEngine::breakpointIcon);
        else
            instructionsGui.at(line).breakpointIcon->setBitmap(GuiEngine::emptyIcon);
    }
}

void Controller::openFile() {
    char filename[1024];
    FILE* f = popen("zenity --file-selection", "r");
    fgets(filename, 1024, f);
    if (filename == "�?tXUU")
        return; // check if no file was selected

    for (int i = 0; i < sizeof(filename); i++) { // remove \n from file name
        if (filename[i] == '\n')
            filename[i] = '\0';
    }

    std::cout << "Opening file " << filename << "." << std::endl;
    std::ifstream file(filename);
    if (!file.is_open())
        return;
    clearInstructions();
    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << std::endl;
        addInstruction(line);
    }
    pause();
    updateWindow();
}

void Controller::draw() {
    // GraphicsEngine::instance()->drawRectangle(rect, 0, al_map_rgb(100, 255,
    // 100));
    GraphicsEngine::instance()->drawBitmap(
        rect.p1,
        GraphicsEngine::instance()->getBitmap(
            "resources/assets/machinery/Controller/main.png"),
        20, 0.2);
}

void Controller::addInstruction(std::string instr) {
    instructions.push_back(instr);
    breakpoints.push_back(false);
}

void Controller::clearInstructions() {
    breakpoints.clear();
    instructions.clear();
    rDelay = 0;
    rInstr = 0;
}

std::vector<std::string> Controller::split(const std::string& s,
    const std::string& delimiter) {
    std::string strCopy = s;
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = strCopy.find(delimiter)) != std::string::npos) {
        token = strCopy.substr(0, pos);
        tokens.push_back(token);
        strCopy.erase(0, pos + delimiter.length());
    }
    tokens.push_back(strCopy);

    return tokens;
}

void Controller::drawInstructions() {
    for (int i = 0; i < instructions.size(); i++) {
        al_draw_text(GuiEngine::debugFont, al_map_rgb(255, 255, 255), 30,
            40 + i * 10, 0, instructions.at(i).c_str());
    }
    al_draw_line(10, 43 + rInstr * 10 + 5, 20, 40 + rInstr * 10 + 5,
        al_map_rgb(255, 255, 255), 1);
    al_draw_line(10, 37 + rInstr * 10 + 5, 20, 40 + rInstr * 10 + 5,
        al_map_rgb(255, 255, 255), 1);
}

void Controller::run() {
    if (paused) {
        return;
    }

    if (failure) {
        return;
    }

    failure = execNextInstr();
    updateWindow();
}

void Controller::pause() {
    paused = true;
    updateWindow();
}

void Controller::unpause() {
    if (failure) {
        return;
    }

    failure = execNextInstr();
    paused = false;
    updateWindow();
}

void Controller::pauseUnpause() {
    if (paused) {
        unpause();
    } else {
        pause();
    }
}

void Controller::up() {
    if (!paused) {
        return;
    }
    failure = 0;
    rDelay = 0;
    if (rInstr > 0) {
        rInstr--;
    }
    updateWindow();
}

void Controller::down() {
    if (!paused) {
        return;
    }
    failure = 0;
    rDelay = 0;
    if (rInstr < instructions.size() - 1) {
        rInstr++;
    }
    updateWindow();
}

void Controller::next() {
    if (failure) {
        return;
    }

    if (!paused) {
        return;
    }

    failure = execNextInstr();
    updateWindow();
}

void Controller::onClick() {
    if (window == nullptr) {
        createWindow();
    }
}
