#ifndef __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_

#include "common.hpp"
#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

class ManipulatorArm {
    static constexpr int maxJointsNumber = 256;
    int jointsNumber;
    RelativeRotation jointsRotation[maxJointsNumber];
    Point2d jointsPosition [maxJointsNumber];
    float bonesLength [maxJointsNumber];

    void defaultInit() {
        for (int i = 0; i < jointsNumber; i++) {
            setBoneLength(i, 50);
            setJointRotation(i, RelativeRotation(0.1));
        }
        recalculate();
    }

public:
    ManipulatorArm(int aJointsNumber):  jointsNumber(aJointsNumber) {
        assert(("incorrect number of joints", 1 <= aJointsNumber <= maxJointsNumber));
        defaultInit();
    }

    void setBoneLength(int aBone, double aLenght){
        assert(("incorrect bone number", aBone <= jointsNumber));
        bonesLength[aBone] = aLenght;
    }

    void setJointRotation(int aJoint, RelativeRotation aRotation) {
        assert(("incorrect bone number", aJoint <= jointsNumber));
        jointsRotation[aJoint] = aRotation;
    }

    void rotateJoint(int aJoint, RelativeRotation aRotation) {
        assert(("incorrect bone number", aJoint <= jointsNumber));
        jointsRotation[aJoint] = jointsRotation[aJoint] + aRotation;
    }

    void setRootJointPosition(Point2d position) {
        jointsPosition[0] = position;
    }

    void recalculate() {
        GlobalRotation lastJointRotation{};
        for (int i = 1; i < jointsNumber; i++) {
            lastJointRotation = lastJointRotation + jointsRotation[i - 1];
            jointsPosition[i] = jointsPosition[i - 1] + Vector2d(lastJointRotation, bonesLength[i - 1]);
        }
    }

    void draw() {
        for (int i = 0; i < jointsNumber; i++) {
            ALLEGRO_FONT *debug_font = nullptr;
            debug_font = al_load_ttf_font("./resources/clacon2.ttf", 14, 0);
            al_draw_text(debug_font, al_map_rgb(255, 255, 255), jointsPosition[i].x + 5, jointsPosition[i].y + 5, 0, std::to_string(i).c_str());
            al_draw_filled_circle(jointsPosition[i].x, jointsPosition[i].y, 3, al_map_rgb(255, 255, 255));
        }

        for (int i = 1; i < jointsNumber; i++) {
            al_draw_line(jointsPosition[i - 1].x, jointsPosition[i - 1].y, jointsPosition[i].x, jointsPosition[i].y, al_map_rgb(255, 255, 255), 1);
        }
    }
    
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_
