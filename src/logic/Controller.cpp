#include <iostream>
#include <fstream>
#include <string>

#include "Controller.hpp"
#include "GameWorld.hpp"
#include <regex>

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

void Controller::setAddress(int id) {
    Machinery::setAddress(id);
    if (window)
        addressLabel->setText(std::format("Address: {}", id));
}

void Controller::createWindow() {
    if (window)
        return;

    window = new Window(GuiEngine::instance()->getDisplayArea(), AligmentBuilder().dimensions(Vector2d(700, 700)).margin(-1, 30, 30, 30), true);
    window->setOnCloseCallback([this]() { this->onWindowClose(); });
    codeConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 90, 5, 10));
    outputConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 0).margin(5, 90, 10, 5));
    outputConsole->setEditable(false);
    memoryConsole = new Console(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 1).margin(5, 25, 10, 10));
    memoryConsole->setEditable(false);
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(0, 0).margin(10, 72, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Code:"), 16)), "Code:");
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 0).margin(5, 72, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Output:"), 16)), "Output:");
    new Label(window->getInternalArea(), AligmentBuilder().tableDimensions(2, 2).tableCell(1, 1).margin(5, 5, -1, -1).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "Memory:"), 16)), "Memory:");
    auto controlArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(1, 0).margin(10, 10, 5, -1).dimensions(Vector2d(-1, 50)), "controls");
    auto settingsArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(2, 0).margin(10, 10, 5, -1).dimensions(Vector2d(-1, 50)), "settings");
    auto fileArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(0, 0).margin(10, 10, 5, -1).dimensions(Vector2d(-1, 50)), "file");

    auto runStopButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(6, 1).tableCell(0, 0).margin(3, 3, 3, 3)); // run/stop
    runStopButton->setMouseCallback(Release, [this](auto pos) { this->onRunButtonClick(); });
    runStopButtonIcon = new Icon(runStopButton, Aligment(), GuiEngine::instance()->getIcon("run"));

    auto pauseButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(6, 1).tableCell(1, 0).margin(3, 3, 3, 3)); // pause/continue
    pauseButton->setMouseCallback(Release, [this](auto pos) { this->onPauseButtonClick(); });
    pauseButtonIcon = new Icon(pauseButton, Aligment(), GuiEngine::instance()->getIcon("pause"));

    auto nextButton = new Button(controlArea->getInternalArea(), AligmentBuilder().tableDimensions(6, 1).tableCell(2, 0).margin(3, 3, 3, 3)); // exec 1 instruction
    nextButton->setMouseCallback(Release, [this](auto pos) {});
    nextButtonIcon = new Icon(nextButton, Aligment(), GuiEngine::instance()->getIcon("next"));

    auto openFileButton = new Button(fileArea->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(0, 0).margin(3, 3, 3, 3));
    openFileButton->setMouseCallback(Release, [this](auto pos) { onOpenFileButtonClick(); });
    new Label(openFileButton, Aligment(), "open");

    auto saveFileButton = new Button(fileArea->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(1, 0).margin(3, 3, 3, 3));
    saveFileButton->setMouseCallback(Release, [this](auto pos) { onSaveFileButtonClick(); });
    new Label(saveFileButton, Aligment(), "save");

    auto saveAsFileButton = new Button(fileArea->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(2, 0).margin(3, 3, 3, 3));
    saveAsFileButton->setMouseCallback(Release, [this](auto pos) { onSaveAsFileButtonClick(); });
    new Label(saveAsFileButton, Aligment(), "save as");

    auto addressButton = new Button(settingsArea->getInternalArea(), AligmentBuilder().margin(3, 3, 3, 3));
    addressButton->setMouseCallback(Release, [this](auto pos) { new AddressSelectionWindow(getAddress(), [this](int address) { this->setAddress(address); }); });
    addressLabel = new Label(addressButton, Aligment(), std::format("Addr: {}", getAddress()));

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
        outputConsole->addLine("");
        outputConsole->addLine("Compiling...");
        clearToInitialState();
        bool compilationResult = compile(codeConsole->getLines()); // TODO handle compilation errors

        if (compilationResult) {
            running = true;
            paused = false;

            outputConsole->addLine("Program started.");
        } else {
            outputConsole->addLine("Compilation failed.");
        }
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
    for (int i = 0; i < getMemorySize(); i++) {
        setMemoryValue(i, 0);
    }
}

bool Controller::isMemoryAddressValid(size_t address) {
    return address >= 0 || address < getMemorySize();
}

bool Controller::getMemoryValueByLiteral(std::string literal, MemoryWord& result) {
    if (literal.starts_with("p")) { // indirect access (e.g. at123 will result in value at addrees stored at address 123)
        size_t address1 = std::atoi(literal.substr(1).c_str());
        if (!isMemoryAddressValid(address1))
            return false;
        size_t address2 = getMemoryValue(address1);
        if (!isMemoryAddressValid(address2))
            return false;
        result = getMemoryValue(address2);
        return true;
    } else if (literal.starts_with("at")) { // direct access (e.g. at123 will result in value at addrees 123)
        size_t address = std::atoi(literal.substr(2).c_str());
        if (!isMemoryAddressValid(address))
            return false;
        result = getMemoryValue(address);
        return true;
    } else if (std::atoi(literal.c_str()) || literal == "0") { // literals like 0, 123, -5
        result = std::atoi(literal.c_str());
        return true;
    }
    return false;
}

bool Controller::setMemoryValueByLiteral(std::string literal, MemoryWord value) {
    if (std::atoi(literal.c_str()) || literal == "0") { // writing to literals like 0, 123, -5 wont do anything
        return true;
    } else if (literal.starts_with("at")) { // direct access (e.g. "copy at123 1" will result in 1 stored at addrees 123)
        size_t address = std::atoi(literal.substr(2).c_str());
        if (!isMemoryAddressValid(address))
            return false;
        setMemoryValue(address, value);
        return true;
    } else if (literal.starts_with("p")) { // indirect access (e.g. "copy p123 1" will result in 1 stored at addrees stored at addrees 123)
        size_t address1 = std::atoi(literal.substr(1).c_str());
        if (!isMemoryAddressValid(address1))
            return false;
        size_t address2 = getMemoryValue(address1);
        if (!isMemoryAddressValid(address2))
            return false;
        setMemoryValue(address2, value);
        return true;
    }
    return false;
}

TerminationCode Controller::execNextInstruction() {
    if (rInstr >= instructions.size()) {
        return ProgrammFinished;
    }

    if (rDelay) {
        rDelay--;
        if (rDelay == 0) {
            rInstr++;
        }
        return Continue;
    }

    if (instructions.at(rInstr).size() == 0 || instructions.at(rInstr).starts_with('#') || instructions.at(rInstr).starts_with("//"sv)) { // ignore empty line, preprocessor and comments
        rInstr++;
        return Continue;
    }

    std::vector<std::string> instr = split(instructions.at(rInstr), " ");
    std::string command = instr.at(0);
    if (command == "delay") { // delay n
        if (instr.size() != 2)
            return InvalidNumberOfArguments;
        if (!getMemoryValueByLiteral(instr.at(1), rDelay))
            return SegFault;
        return Continue;
    } else if (command == "goto") { // goto instr_n
        if (instr.size() != 2)
            return InvalidNumberOfArguments;
        if (!getMemoryValueByLiteral(instr.at(1), rInstr))
            return SegFault;
        return Continue;
    } else if (command == "goto_if") { // goto_if instr_n condition
        if (instr.size() != 3)
            return InvalidNumberOfArguments;
        MemoryWord instrN;
        MemoryWord value;
        if (!getMemoryValueByLiteral(instr.at(1), instrN))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(2), value))
            return SegFault;
        if (value != 0)
            rInstr = instrN;
        else
            rInstr++;
        return Continue;
    } else if (command == "goto_ifn") { // goto_ifn instr_n condition
        if (instr.size() != 3)
            return InvalidNumberOfArguments;
        MemoryWord instrN;
        MemoryWord value;
        if (!getMemoryValueByLiteral(instr.at(1), instrN))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(2), value))
            return SegFault;
        if (value == 0)
            rInstr = instrN;
        else
            rInstr++;
        return Continue;
    } else if (command == "throw") { // throw error_code
        if (instr.size() != 2)
            return InvalidNumberOfArguments;
        MemoryWord errorCode;
        if (!getMemoryValueByLiteral(instr.at(1), errorCode))
            return SegFault;
        return static_cast<TerminationCode>(errorCode);
    } else if (command == "set") { // set dest src
        if (instr.size() != 3)
            return InvalidNumberOfArguments;
        MemoryWord src1;
        if (!getMemoryValueByLiteral(instr.at(2), src1))
            return SegFault;
        if (!setMemoryValueByLiteral(instr.at(1), src1))
            return SegFault;
        rInstr++;
        return Continue;
    } else if (command == "add") { // add dest src1 src2
        if (instr.size() != 4)
            return InvalidNumberOfArguments;
        MemoryWord src1;
        MemoryWord src2;
        if (!getMemoryValueByLiteral(instr.at(2), src1))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(3), src2))
            return SegFault;
        if (!setMemoryValueByLiteral(instr.at(1), src1 + src2))
            return SegFault;
        rInstr++;
        return Continue;
    } else if (command == "subtract") { // subtract dest src1 src2
        if (instr.size() != 4)
            return InvalidNumberOfArguments;
        MemoryWord src1;
        MemoryWord src2;
        if (!getMemoryValueByLiteral(instr.at(2), src1))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(3), src2))
            return SegFault;
        if (!setMemoryValueByLiteral(instr.at(1), src1 - src2))
            return SegFault;
        rInstr++;
        return Continue;
    } else if (command == "send") { // send mach_addr mach_memory_addr value

        if (instr.size() != 4)
            return InvalidNumberOfArguments;
        MemoryWord value;
        MemoryWord machineryAddr;
        MemoryWord machineMemoryAddr;
        if (!getMemoryValueByLiteral(instr.at(1), value))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(2), machineryAddr))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(3), machineMemoryAddr))
            return SegFault;

        for (auto machinery : GameWorld::instance()->getMachinery()) {
            if (machinery->getAddress() == machineryAddr) {
                machinery->onMemoryWrite(machineMemoryAddr, value);
            }
        }

        rInstr++;
        return Continue;
    } else if (command == "recieve") {
        if (instr.size() != 4)
            return InvalidNumberOfArguments;
        MemoryWord machineryAddr;
        MemoryWord machineMemoryAddr;
        if (!getMemoryValueByLiteral(instr.at(2), machineryAddr))
            return SegFault;
        if (!getMemoryValueByLiteral(instr.at(3), machineMemoryAddr))
            return SegFault;

        for (auto machinery : GameWorld::instance()->getMachinery()) {
            if (machinery->getAddress() == machineryAddr) {
                if (setMemoryValueByLiteral(instr.at(1), machinery->onMemoryRead(machineMemoryAddr))) {
                    rInstr++;
                    return Continue;
                } else {
                    return SegFault;
                }
            }
        }

        // reach here if didnt find any machines with address
        rInstr++;
        return Continue;
    }
    return InvalidInstruction; // invalid instruction
}

int Controller::getSourceLineNumber() {
    return rInstr; // return source line what coresponds to rInstr (for now it is the same line)
}

bool Controller::compile(std::vector<std::string> sourceCode) {
    instructions = sourceCode;
    for (int j = 0; j < sourceCode.size(); j++) {
        if (sourceCode.at(j).starts_with("#define")) {
            auto lineParts = split(sourceCode.at(j), " ");
            if (lineParts.size() != 3)
                return false; // error
            auto search = lineParts.at(1);
            auto replace = lineParts.at(2);
            for (int i = 0; i < instructions.size(); i++) {
                instructions.at(i) = std::regex_replace(instructions.at(i), std::regex(search), replace);
            }
        } else if (sourceCode.at(j).starts_with("#label")) {
            auto lineParts = split(sourceCode.at(j), " ");
            if (lineParts.size() != 2)
                return false; // error
            auto search = lineParts.at(1);
            auto replace = std::to_string(j);
            for (int i = 0; i < instructions.size(); i++) {
                instructions.at(i) = std::regex_replace(instructions.at(i), std::regex(search), replace);
            }
        }
    }
    return true;
}