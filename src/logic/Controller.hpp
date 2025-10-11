#ifndef __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_

#include <assert.h>

#include <string>
#include <vector>

#include "common.hpp"
#include "GuiEngine.hpp"
#include "GraphicsEngine.hpp"

#include "Machinery.hpp"

class Controller : public Machinery {
    bool paused = true;
    int running = false;

    std::vector<std::string> instructions;
    int rInstr = 0;
    int rDelay = 0;
    Machinery* ownMachinery;

    Window* window = nullptr;
    Icon* runStopButtonIcon = nullptr;
    Icon* pauseButtonIcon = nullptr;
    Icon* nextButtonIcon = nullptr;
    Console* codeConsole = nullptr;
    Console* outputConsole = nullptr;
    Console* memoryConsole = nullptr;

    std::vector<std::string> codeConsoleSavedLines;
    std::vector<std::string> ouputConsoleSavedLines;
    std::string filepath = "";
    void onOpenFileButtonClick();
    void createWindow();
    void updateWindow();
    void updateMemoryConsole();
    void onWindowClose();
    void onRunButtonClick();
    void onPauseButtonClick();
    void saveConsolesStates();
    void loadConsoledStates();
    void saveCodeToFile(std::string filepath);
    void onSaveFileButtonClick();
    void onSaveAsFileButtonClick();
    void onReloadFileButtonclick();
    void clearToInitialState();
    int execNextInstruction();
    int getSourceLineNumber();
    bool compile(std::vector<std::string> sourceCode); // TODO maybe return results like "error here and here"

public:
    Controller(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(5, 7)), 100) {
    }

    void draw() override;
    std::vector<std::string> split(const std::string& s, const std::string& delimiter);
    void run() override;
    void onClick() override;
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_