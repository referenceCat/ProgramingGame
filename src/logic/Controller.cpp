#include <iostream>
#include <fstream>
#include <string>

#include "Controller.hpp"
#include "GameWorld.hpp"

std::vector<std::string> split(const std::string& s,
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

void Controller::onClick() {
    if (window == nullptr) {
        createWindow();
    }
}

void Controller::createWindow() {
    if (window)
        return;

    window = new Window(GuiEngine::instance()->getDisplayArea(), AligmentBuilder().dimensions(Vector2d(700, 700)).margin(-1, 50, 50, 50), true);
    window->setOnCloseCallback([this](){this->onWindowClose();});
    codeConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 90, 5, 10));
    outputConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(1, 0).margin(5, 90, 10, 10));
    outputConsole->setEditable(false);
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 72, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Code:"), 16)), "Code:");
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(1, 0).margin(5, 72, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Output:"), 16)), "Output:");
    auto controlArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 10, 5, -1).dimensions(Vector2d(-1, 50)), "controls");

    auto runStopButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(9, 1).tableCell(0, 0).margin(3, 3, 3, 3)); // run/stop
    runStopButton->setMouseCallback(Release, [this](auto pos) { this->onRunButtonClick(); });
    runStopButtonIcon = new Icon(runStopButton, Aligment(), GuiEngine::instance()->getIcon("run"));

    auto pauseButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(9, 1).tableCell(1, 0).margin(3, 3, 3, 3)); // pause/continue
    pauseButton->setMouseCallback(Release, [this](auto pos) { this->onPauseButtonClick(); });
    pauseButtonIcon = new Icon(pauseButton, Aligment(), GuiEngine::instance()->getIcon("pause"));

    auto nextButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(9, 1).tableCell(2, 0).margin(3, 3, 3, 3)); // exec 1 instruction
    nextButtonIcon = new Icon(nextButton, Aligment(), GuiEngine::instance()->getIcon("next"));
    nextButtonIcon->setMouseCallback(Release, [this](auto pos) {});

    updateWindow();
}

void Controller::updateWindow() {
    if (!window)
        return;
    if (running) {
        runStopButtonIcon->setBitmap(GuiEngine::instance()->getIcon("stop"));
        pauseButtonIcon->setBitmap(GuiEngine::instance()->getIcon(paused ? "unpause" : "pause"));
        nextButtonIcon->setBitmap(paused ? GuiEngine::instance()->getIcon("next") : nullptr);
        codeConsole->setEditable(false);
    } else {
        runStopButtonIcon->setBitmap(GuiEngine::instance()->getIcon("run"));
        pauseButtonIcon->setBitmap(nullptr);
        nextButtonIcon->setBitmap(nullptr);
        codeConsole->setEditable(true);
    }
}

void Controller::onWindowClose() {
    window = nullptr;
}

void Controller::onRunButtonClick() {
    if (running) {
        running = false;
        interpreter.clearToInitialState();
        outputConsole->addLine("Program terminated by user.");
    } else {
        running = true;
        paused = false;
        outputConsole->addLine("");
        outputConsole->addLine("Compiling...");
        interpreter.clearToInitialState();
        interpreter.compile(codeConsole->getLines()); // TODO handle compilation errors
        outputConsole->addLine("Program started.");
    }
    updateWindow();
}

void Controller::onPauseButtonClick() {
    if (!running)
        return;

    if (paused)
        paused = false;
    else
        paused = true;
    updateWindow();
}

void Controller::openFile() {
    // char filename[1024];
    // FILE* f = popen("zenity --file-selection", "r");
    // fgets(filename, 1024, f);
    // if (filename == "�?tXUU")
    //     return; // check if no file was selected

    // for (int i = 0; i < sizeof(filename); i++) { // remove \n from file name
    //     if (filename[i] == '\n')
    //         filename[i] = '\0';
    // }

    // std::cout << "Opening file " << filename << "." << std::endl;
    // std::ifstream file(filename);
    // if (!file.is_open())
    //     return;
    // clearInstructions();
    // std::string line;
    // while (std::getline(file, line)) {
    //     std::cout << line << std::endl;
    //     addInstruction(line);
    // }
    // pause();
    // updateWindow();
}

void Controller::draw() {
    GraphicsEngine::instance()->drawBitmap(
        rect.p1,
        GraphicsEngine::instance()->getBitmap(
            "resources/assets/machinery/Controller/main.png"),
        20, 0.2);
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

void Controller::run() {
    if (!paused && running) {
        int result = interpreter.execNextInstruction();
        if (result == 3) {
            outputConsole->addLine(std::format("Program finished! (code: {})", result));
            running = false;
        } else if (result != 0) {
            outputConsole->addLine(std::format("Program failed with code: {}!", result));
            outputConsole->addLine(std::format("At line: {}", interpreter.getSourceLineNumber()));
            running = false;
        }
    }

    updateWindow();
}

void Interpreter::clearToInitialState() {
    rInstr = 0;
    rDelay = 0;
}

int Interpreter::execNextInstruction() {
    if (rInstr >= instructions.size()) {
        return 3; // program ended
    }

    // if (breakpoints.at(rInstr) && !paused) {
    //     pause();
    //     return 0;
    // }

    if (rDelay) {
        rDelay--;
        if (rDelay == 0) {
            rInstr++;
        }
        return 0;
    }

    if (instructions.at(rInstr).size() == 0 || instructions.at(rInstr).at(0) == '#') {
        rInstr++;
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

int Interpreter::getSourceLineNumber() {
    return rInstr; // return source line what coresponds to rInstr (for now it is the same line)
}

bool Interpreter::compile(std::vector<std::string> sourceCode) {
    instructions = sourceCode;
    return true;
}