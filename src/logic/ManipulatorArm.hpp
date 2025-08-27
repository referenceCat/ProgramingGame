#ifndef __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_

#include "../common/common.hpp"
#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string>
#include <algorithm>
#include "Box.hpp"
#include "GameObject.hpp"

class GameWorld;

class ManipulatorArm : public GameObject { // TODO now supports rendering of 2 segment manipulator only
    static constexpr int maxJointsNumber = 256;
    static constexpr double jointRotationSpeed = 0.01;
    static constexpr double segmentResizeSpeed = 1;
    const int jointsNumber;

    // current state 
    RelativeRotation jointsRotation[maxJointsNumber];
    Point2d jointsPosition[maxJointsNumber];
    float segmentsLength[maxJointsNumber];

    // target state
    RelativeRotation jointsTargetRotation[maxJointsNumber]; // TODO use vector
    Point2d jointsTargetPosition[maxJointsNumber];
    float segmentsTargetLength[maxJointsNumber];

    bool active = false;
    Box* takenBox = nullptr;
    Vector2d relativeBoxPosition{};

    void defaultInit() {
        for (int i = 0; i < jointsNumber; i++) {
            setSegmentLength(i, 100);
            setJointRotation(i, RelativeRotation(0.1));
            setSegmentTargetLength(i, 100);
            setJointTargetRotation(i, RelativeRotation(0.1));
        }
        recalculate();
    }

public:
    ManipulatorArm(int aJointsNumber, GameWorld* aWorld):  jointsNumber(aJointsNumber), GameObject(aWorld) {
        assert(("incorrect number of joints", 1 <= aJointsNumber <= maxJointsNumber));
        defaultInit();
    }

    void setSegmentLength(int aSegment, double aLenght){
        assert(("incorrect bone number", aSegment <= jointsNumber));
        segmentsLength[aSegment] = aLenght;
    }

    void setJointRotation(int aJoint, RelativeRotation aRotation) {
        assert(("incorrect bone number", aJoint <= jointsNumber));
        jointsRotation[aJoint] = aRotation;
    }

    void setJointTargetRotation(int aJoint, RelativeRotation aRotation) {
        assert(("incorrect bone number", aJoint <= jointsNumber));
        jointsTargetRotation[aJoint] = aRotation;
    }

    void setSegmentTargetLength(int aSegment, double aLenght){
        assert(("incorrect bone number", aSegment <= jointsNumber));
        segmentsTargetLength[aSegment] = aLenght;
    }

    void rotateJoint(int aJoint, RelativeRotation aRotation) {
        assert(("incorrect bone number", aJoint <= jointsNumber));
        jointsRotation[aJoint] = jointsRotation[aJoint] + aRotation;
    }

    void setRootJointPosition(Point2d position) {
        jointsPosition[0] = position;
    }

    void cleatTarget() {
        memcpy(jointsTargetRotation, jointsRotation, maxJointsNumber);
        memcpy(segmentsTargetLength, segmentsLength, maxJointsNumber);
    }

    void recalculate() {
        GlobalRotation lastJointTargetRotation{};
        jointsTargetPosition[0] = jointsPosition[0];
        for (int i = 1; i < jointsNumber; i++) {
            lastJointTargetRotation = lastJointTargetRotation + jointsTargetRotation[i - 1];
            jointsTargetPosition[i] = jointsTargetPosition[i - 1] + Vector2d(lastJointTargetRotation, segmentsTargetLength[i - 1]);
        }

        GlobalRotation lastJointRotation{};
        for (int i = 1; i < jointsNumber; i++) {
            lastJointRotation = lastJointRotation + jointsRotation[i - 1];
            jointsPosition[i] = jointsPosition[i - 1] + Vector2d(lastJointRotation, segmentsLength[i - 1]);
        }

        if(takenBox) {
            takenBox->move(jointsPosition[jointsNumber - 1] + relativeBoxPosition);
        }
    }

    void drawDebug() {
        // draw target state
        for (int i = 0; i < jointsNumber; i++) {
            GraphicsEngine::instance()->drawCircle(jointsTargetPosition[i], 3, 0, al_map_rgb(0, 100, 100));
        }

        for (int i = 1; i < jointsNumber; i++) {
           GraphicsEngine::instance()->drawLine(jointsTargetPosition[i - 1], jointsTargetPosition[i], 0, al_map_rgb(0, 100, 100), 1);
        }

        // draw current state
        for (int i = 0; i < jointsNumber; i++) {
            // TODO al_draw_text(GameObject::debug_font, al_map_rgb(255, 255, 255), jointsPosition[i].x + 5, jointsPosition[i].y + 5, 0, std::to_string(i).c_str());
            GraphicsEngine::instance()->drawCircle(jointsPosition[i], 3, 0, al_map_rgb(255, 255, 255), 0);
        }

        for (int i = 1; i < jointsNumber; i++) {
            GraphicsEngine::instance()->drawLine(jointsPosition[i - 1], jointsPosition[i], 0, al_map_rgb(255, 255, 255), 1);
        }

        // draw active arm
        if (active) {
            GraphicsEngine::instance()->drawCircle(jointsPosition[jointsNumber - 1], 4, 0, al_map_rgb(0, 255, 0));
        }
    }

     void draw() {
        if (jointsNumber == 3) {
            GraphicsEngine::instance()->drawBitmap(jointsPosition[0],  GraphicsEngine::instance()->baseSpite, 1, Point2d(25, 25));
            GraphicsEngine::instance()->drawBitmap(jointsPosition[0],  GraphicsEngine::instance()->segment0Sprite, 0.5, Point2d(25, 25), Vector2d(jointsPosition[1], jointsPosition[0]).getDirection());
            GraphicsEngine::instance()->drawBitmap(jointsPosition[1],  GraphicsEngine::instance()->segment1Sprite, 1, Point2d(25, 50), Vector2d(jointsPosition[2], jointsPosition[1]).getDirection());
        } else {
            drawDebug();
        }
        
     }

    void rotateJointToTarget(int i) { // TODO stop after reacing target rotation
        double dRotation = std::fmod(jointsTargetRotation[i].radians - jointsRotation[i].radians, M_PI * 2);
        if (dRotation < 0) dRotation += M_PI * 2;
        double dRotationNegative = M_PI * 2 - dRotation;
        
        if (dRotation < dRotationNegative) {
            jointsRotation[i] = jointsRotation[i] + std::min(jointRotationSpeed, dRotation);
        }
        else {
            jointsRotation[i] = jointsRotation[i] - std::min(jointRotationSpeed, dRotationNegative);
        }
    }

    void resizeSegmentToTarget(int i) { // TODO stop after reacing target size
        double dLenght = segmentsTargetLength[i] - segmentsLength[i];
        if (dLenght > 0)  {
            segmentsLength[i] += std::min(segmentResizeSpeed, abs(dLenght));
        } else {
            segmentsLength[i] -= std::min(segmentResizeSpeed, abs(dLenght));
        }
                
    }

    void moveToTarget() { // TODO PID???
        for (int i = 0; i < jointsNumber; i++) {
            rotateJointToTarget(i);
        }

        for (int i = 0; i < jointsNumber - 1; i++) {
            resizeSegmentToTarget(i);
        }
    }

    void grab();

    void release() {
        active = false;
        takenBox = nullptr;
        relativeBoxPosition = Vector2d();
    }

    bool isActive() {
        return active;
    }

    Point2d getLastJointPos() {
        return jointsPosition[jointsNumber - 1];
    }

    void removeBox() {
        takenBox = nullptr;
    }

    void removeBox(Box* aBox) {
        if (takenBox == aBox) {
            removeBox();
        }
    }

private:
    void takeBox(Box* aBox)  {
        takenBox = aBox;
        relativeBoxPosition = Vector2d(takenBox->getRect().center(), jointsPosition[jointsNumber - 1]);
    }
    
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_
