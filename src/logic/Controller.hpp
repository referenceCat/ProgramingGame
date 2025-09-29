#ifndef __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_

#include <assert.h>

#include <string>
#include <vector>

#include "common.hpp"
#include "GuiEngine.hpp"
#include "GraphicsEngine.hpp"

#include "Machinery.hpp"

// TODO change logic to be like irl programming languages?
class Controller : public Machinery {
    struct InstructionLine {
        Label* label;
        Button* breakpointButton;
        Icon* breakpointIcon;
    };

    std::vector<std::string> instructions;
    std::vector<bool> breakpoints;
    int rInstr = 0;
    int r1 = 0;
    int r2 = 0;
    int rDelay = 0;

    Window* window = nullptr;
    std::vector<InstructionLine> instructionsGui;
    Label* rInstrLabel;
    Label* rDelayLabel;
    bool paused = false;
    int failure = false;
    Icon* pauseIcon = nullptr;

    void openFile();

public:
    Controller(Vector2d aPos):
        Machinery(Rect2d::fromCenterAndDimensions(aPos, Vector2d(5, 7))) {
        addInstruction("");
    }

    void draw() override;
    void addInstruction(std::string instr);
    void clearInstructions();
    std::vector<std::string> split(const std::string& s, const std::string& delimiter);
    void createWindow();
    void updateWindow();
    int execNextInstr(); // returns error code
    void drawInstructions();
    void run() override;
    void pause();
    void unpause();
    void pauseUnpause();
    void up();
    void down();
    void next();
    void toggle(int line);
    void onClick() override;
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_