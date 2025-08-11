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

class Controller : public GameObject {
    Rect2d rect;

    std::vector<std::string> instructions;
    int instrPointer = 0;
    int r1 = 0;
    int r2 = 0;
    int rDelay = 0;

public:
    Controller(Point2d aPos, GameWorld* aWorld): rect(aPos, 40, 30), GameObject(aWorld) {}

    void draw() {
        al_draw_rectangle(rect.p1.x, rect.p1.y, rect.p2.x, rect.p2.y, al_map_rgb(100, 255, 100), 1);
    }

    Rect2d getRect() {
        return rect;
    }

    void addInstruction(std::string instr) {
        instructions.push_back(instr);
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

    // returns error code
    int execNextInstr();

    void drawInstructions() {
        ALLEGRO_FONT *debug_font = nullptr;
        debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
        for (int i = 0; i < instructions.size(); i++) {
            al_draw_text(debug_font, al_map_rgb(255, 255, 255), 30, 40 + i * 10, 0, instructions.at(i).c_str());
        }
        al_draw_line(10,  43 + instrPointer * 10 + 5, 20, 40 + instrPointer * 10 + 5, al_map_rgb(255, 255, 255), 1);
        al_draw_line(10,  37 + instrPointer * 10 + 5, 20, 40 + instrPointer * 10 + 5, al_map_rgb(255, 255, 255), 1);
    }

    void drawRegisters() {
        ALLEGRO_FONT *debug_font = nullptr;
        debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
        al_draw_text(debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 10, 0, std::to_string(instrPointer).c_str());
        al_draw_text(debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 20, 0, std::to_string(r1).c_str());
        al_draw_text(debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 30, 0, std::to_string(r2).c_str());
        al_draw_text(debug_font, al_map_rgb(255, 255, 255), rect.p1.x, rect.p2.y + 40, 0, std::to_string(rDelay).c_str());
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_CONTROLLER_HPP_