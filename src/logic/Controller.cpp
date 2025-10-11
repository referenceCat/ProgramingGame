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
    window->setOnCloseCallback([this]() { this->onWindowClose(); });
    codeConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 90, 5, 10));
    outputConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 0).margin(5, 90, 10, 5));
    outputConsole->setEditable(false);
    memoryConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 1).margin(5, 25, 10, 10));
    memoryConsole->setEditable(false);
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 72, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Code:"), 16)), "Code:");
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 0).margin(5, 72, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Output:"), 16)), "Output:");
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 1).margin(5, 5, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Memory:"), 16)), "Memory:");
    auto controlArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 10, 5, -1).dimensions(Vector2d(-1, 50)), "controls");

    auto runStopButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(12, 1).tableCell(0, 0).margin(3, 3, 3, 3)); // run/stop
    runStopButton->setMouseCallback(Release, [this](auto pos) { this->onRunButtonClick(); });
    runStopButtonIcon = new Icon(runStopButton, Aligment(), GuiEngine::instance()->getIcon("run"));

    auto pauseButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(12, 1).tableCell(1, 0).margin(3, 3, 3, 3)); // pause/continue
    pauseButton->setMouseCallback(Release, [this](auto pos) { this->onPauseButtonClick(); });
    pauseButtonIcon = new Icon(pauseButton, Aligment(), GuiEngine::instance()->getIcon("pause"));

    auto nextButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(12, 1).tableCell(2, 0).margin(3, 3, 3, 3)); // exec 1 instruction
    nextButton->setMouseCallback(Release, [this](auto pos) {});
    nextButtonIcon = new Icon(nextButton, Aligment(), GuiEngine::instance()->getIcon("next"));

    auto openFileButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(4, 1).tableCell(1, 0).margin(3, 3, 3, 3));
    openFileButton->setMouseCallback(Release, [this](auto pos) { onOpenFileButtonClick(); });
    new Label(openFileButton, Aligment(), "open");

    auto saveFileButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(4, 1).tableCell(2, 0).margin(3, 3, 3, 3));
    saveFileButton->setMouseCallback(Release, [this](auto pos) { onSaveFileButtonClick(); });
    new Label(saveFileButton, Aligment(), "save");

    auto saveAsFileButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(4, 1).tableCell(3, 0).margin(3, 3, 3, 3));
    saveAsFileButton->setMouseCallback(Release, [this](auto pos) { onSaveAsFileButtonClick(); });
    new Label(saveAsFileButton, Aligment(), "save as");

    loadConsoledStates();

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
        updateMemoryConsole();
    } else {
        runStopButtonIcon->setBitmap(GuiEngine::instance()->getIcon("run"));
        pauseButtonIcon->setBitmap(nullptr);
        nextButtonIcon->setBitmap(nullptr);
        codeConsole->setEditable(true);
    }
}

void Controller::updateMemoryConsole() {
    memoryConsole->clearLines();
    for (int i = 0; i < getMemorySize(); i++) {
        if (i == 0)
            memoryConsole->setLines({std::to_string(getMemoryValue(i))});
        else
            memoryConsole->addLine(std::to_string(getMemoryValue(i)));
    }
}

void Controller::onWindowClose() {
    saveConsolesStates();
    window = nullptr;
}

void Controller::onRunButtonClick() {
    if (running) {
        running = false;
        clearToInitialState();
        outputConsole->addLine("Program terminated by user.");
    } else {
        running = true;
        paused = false;
        outputConsole->addLine("");
        outputConsole->addLine("Compiling...");
        clearToInitialState();
        compile(codeConsole->getLines()); // TODO handle compilation errors
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

void Controller::saveConsolesStates() {
    codeConsoleSavedLines = codeConsole->getLines(); // TODO save to file
    ouputConsoleSavedLines = outputConsole->getLines();
}

void Controller::loadConsoledStates() {
    codeConsole->setLines(codeConsoleSavedLines);
    outputConsole->setLines(ouputConsoleSavedLines);
}

std::string getFilenameViaDialog(bool save) {
    char filename[1024];
    FILE* f;
    if (save) {
        f = popen("zenity --file-selection --title='Select a File' --save", "r");
    } else {
        f = popen("zenity --file-selection --title='Select a File'", "r");
    }

    fgets(filename, 1024, f);
    if (filename == "�?tXUU")
        return {}; // check if no file was selected

    for (int i = 0; i < sizeof(filename); i++) { // remove \n from file name
        if (filename[i] == '\n')
            filename[i] = '\0';
    }

    return filename;
}

void Controller::saveCodeToFile(std::string filepath) {
    std::ofstream myfile;
    myfile.open(filepath);
    for (auto line : codeConsole->getLines()) {
        myfile << line << '\n';
    }
    myfile << std::flush;
    myfile.close();
    return;
}

void Controller::onSaveFileButtonClick() {
    assert(window != nullptr);
    if (filepath.empty()) {
        onSaveAsFileButtonClick();
        return;
    }
    saveCodeToFile(filepath);
}

void Controller::onSaveAsFileButtonClick() {
    assert(window != nullptr);
    auto dialogResult = getFilenameViaDialog(true);
    if (dialogResult.empty()) {
        return;
    }
    filepath = dialogResult;
    saveCodeToFile(filepath);
}

void Controller::onReloadFileButtonclick() {
    assert(window != nullptr);
}

void Controller::onOpenFileButtonClick() {
    assert(window != nullptr);

    auto dialogResult = getFilenameViaDialog(false);
    std::ifstream file(dialogResult);
    if (!file.is_open())
        return;
    codeConsole->clearLines();
    std::string line;
    if (std::getline(file, line)) {
        codeConsole->setLines({line});
    }
    while (std::getline(file, line)) {
        codeConsole->addLine(line);
    }
    filepath = dialogResult;
    updateWindow();
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
        int result = execNextInstruction();
        codeConsole->setHighlightedLine(getSourceLineNumber());
        if (result == 3) {
            outputConsole->addLine(std::format("Program finished! (code: {})", result));
            running = false;
        } else if (result != 0) {
            outputConsole->addLine(std::format("Program failed with code: {}!", result));
            outputConsole->addLine(std::format("At line: {}", getSourceLineNumber()));
            running = false;
        }
    }

    updateWindow();
}

void Controller::clearToInitialState() {
    rInstr = 0;
    rDelay = 0;
}

int Controller::execNextInstruction() {
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

    if (instructions.at(rInstr).size() == 0 || instructions.at(rInstr).at(0) == '#') { // ignore this
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
        int machineryAddress = std::atoi(instr.at(1).c_str());
        int memoryAddress = std::atoi(instr.at(2).c_str());
        int value = std::atoi(instr.at(3).c_str());

        for (auto machinery : GameWorld::instance()->getMachinery()) {
            if (machinery->getAddress() == machineryAddress) {
                machinery->onMemoryWrite(memoryAddress, value);
            }
        }

        rInstr++;
        return 0;
    }
    if (command == "recieve") {
        int machineryAddress = std::atoi(instr.at(1).c_str());
        int machineryMemoryAddress = std::atoi(instr.at(2).c_str());
        int ownMemoryAddress = std::atoi(instr.at(3).c_str());

        for (auto machinery : GameWorld::instance()->getMachinery()) {
            if (machinery->getAddress() == machineryAddress) {
                setMemoryValue(ownMemoryAddress, machinery->onMemoryRead(machineryMemoryAddress));
            }
        }

        rInstr++;
        return 0;
    }
    return 1; // invalid instruction
}

int Controller::getSourceLineNumber() {
    return rInstr; // return source line what coresponds to rInstr (for now it is the same line)
}

bool Controller::compile(std::vector<std::string> sourceCode) {
    instructions = sourceCode;
    return true;
}