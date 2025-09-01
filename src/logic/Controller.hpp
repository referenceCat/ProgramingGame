#ifndef __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_

#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <vector>
#include "../common/common.hpp"
#include "ManipulatorArm.hpp"
#include "Box.hpp"
#include "GameObject.hpp"
#include "../ui/GuiEngine.hpp"

class Controller : public GameObject {
    Rect2d rect;

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

public:
    Controller(Vector2d aPos, GameWorld* aWorld): rect(aPos, 7, 5), GameObject(aWorld) {}

    void draw() {
        // GraphicsEngine::instance()->drawRectangle(rect, 0, al_map_rgb(100, 255, 100));
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->controllerSprite, 0.2, Vector2d(), Rotation(), 20);
    }

    Rect2d getRect() {
        return rect;
    }

    void addInstruction(std::string instr) {
        instructions.push_back(instr);
        breakpoints.push_back(false);
    }

    std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
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

    void createWindow();

    void updateWindow();

    // returns error code
    int execNextInstr();

    void drawInstructions() {
        for (int i = 0; i < instructions.size(); i++) {
            al_draw_text(GameObject::debug_font, al_map_rgb(255, 255, 255), 30, 40 + i * 10, 0, instructions.at(i).c_str());
        }
        al_draw_line(10,  43 + rInstr * 10 + 5, 20, 40 + rInstr * 10 + 5, al_map_rgb(255, 255, 255), 1);
        al_draw_line(10,  37 + rInstr * 10 + 5, 20, 40 + rInstr * 10 + 5, al_map_rgb(255, 255, 255), 1);
    }

    void drawRegisters() {
        // TODO
        // al_draw_text(GameObject::debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 10, 0, std::to_string(rInstr).c_str());
        // al_draw_text(GameObject::debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 20, 0, std::to_string(r1).c_str());
        // al_draw_text(GameObject::debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 30, 0, std::to_string(r2).c_str());
        // al_draw_text(GameObject::debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 40, 0, std::to_string(rDelay).c_str());
    }

    void run() {
        if (paused) {
            return;
        }

        if (failure) {
            return;
        }

        failure = execNextInstr();
        updateWindow();
    }

    void pause() {
        paused = true;
        updateWindow();
    }

    void unpause() {
        if (failure) {
            return;
        }

        failure = execNextInstr();
        paused = false;
        updateWindow();
    }

    void pauseUnpause() {
        if (paused) {
            unpause();
        } else {
            pause();
        }
    }

    void up() {
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

    void down() {
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

    void next() {
        if (failure) {
            return;
        }

        if (!paused) {
            return;
        }

        failure = execNextInstr();
        updateWindow();
    }

    void toggle(int line);
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_