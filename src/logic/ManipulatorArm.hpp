#ifndef __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_

#include "../common/common.hpp"
#include <assert.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "Box.hpp"
#include "GameObject.hpp"

class GameWorld;

class ManipulatorArm : public GameObject { // TODO now supports rendering of 2 segment manipulator only
    static constexpr int maxJointsNumber = 256;
    static constexpr double jointRotationSpeed = 0.01;
    static constexpr double segmentResizeSpeed = 1;
    const int jointsNumber;

    // current state
    Rotation jointsRotation[maxJointsNumber];
    Vector2d jointsPosition[maxJointsNumber];
    float segmentsLength[maxJointsNumber];

    // target state
    Rotation jointsTargetRotation[maxJointsNumber]; // TODO use vector
    Vector2d jointsTargetPosition[maxJointsNumber];
    float segmentsTargetLength[maxJointsNumber];

    bool active = false;
    Box* takenBox = nullptr;
    Vector2d relativeBoxPosition{};

    void defaultInit();

public:
    ManipulatorArm(int aJointsNumber);
    Vector2d getJointPosition(int n);
    Rotation getJointRotation(int n);
    void setSegmentLength(int aSegment, double aLenght);
    void setJointRotation(int aJoint, Rotation aRotation);
    void setJointTargetRotation(int aJoint, Rotation aRotation);
    void setSegmentTargetLength(int aSegment, double aLenght);
    void rotateJoint(int aJoint, Rotation aRotation);
    void setRootJointPosition(Vector2d position);
    void cleatTarget();
    void recalculate();
    void drawInfo() { }
    void drawDebug();
    void draw() { }
    void rotateJointToTarget(int i);
    void resizeSegmentToTarget(int i);
    void moveToTarget();
    void grab();
    void release();
    bool isActive();
    Vector2d getLastJointPos();
    void removeBox();
    void removeBox(Box* aBox);
    void addToGameWorld() override;
    Box* getTakenBox();

private:
    void takeBox(Box* aBox);
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_MANIPULATORARM_HPP_
