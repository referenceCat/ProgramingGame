#include "ManipulatorArm.hpp"
#include "GameWorld.hpp"
#include "GraphicsEngine.hpp"

void ManipulatorArm::grab() // TODO what if box is already grabbed?
{
    active = true;
    Box* box = GameWorld::instance()->getBox(jointsPosition[jointsNumber - 1]);
    if (box) {
        takeBox(box);
    }
}

void ManipulatorArm::addToGameWorld() {
    GameWorld::instance()->addManipulatorArm(this);
}

Box* ManipulatorArm::getTakenBox() {
    return takenBox;
}

void ManipulatorArm::drawDebug() {
    // draw target state
    for (int i = 0; i < jointsNumber; i++) {
        GraphicsEngine::instance()->drawCircle(jointsTargetPosition[i], 0.2, CommonValues::zDebug, al_map_rgb(0, 100, 100));
    }

    for (int i = 1; i < jointsNumber; i++) {
        GraphicsEngine::instance()->drawLine(
            jointsTargetPosition[i - 1], jointsTargetPosition[i],
            CommonValues::zDebug, al_map_rgb(0, 100, 100), 0.1);
    }

    // draw current state
    for (int i = 0; i < jointsNumber; i++) {
        // TODO al_draw_text(GameObject::debugFont, al_map_rgb(255, 255, 255),
        // jointsPosition[i].x + 5, jointsPosition[i].y + 5, 0,
        // std::to_string(i).c_str());
        GraphicsEngine::instance()->drawCircle(
            jointsPosition[i], 0.2, CommonValues::zDebug, al_map_rgb(255, 255, 255));
    }

    for (int i = 1; i < jointsNumber; i++) {
        GraphicsEngine::instance()->drawLine(
            jointsPosition[i - 1], jointsPosition[i], CommonValues::zDebug,
            al_map_rgb(255, 255, 255), 0.1);
    }

    // draw active arm
    if (active) {
        GraphicsEngine::instance()->drawPoint(jointsPosition[jointsNumber - 1],
            CommonValues::zDebug,
            al_map_rgb(0, 255, 0), 4);
    }
}

void ManipulatorArm::rotateJointToTarget(
    int i) { // TODO stop after reacing target rotation
    double dRotation = std::fmod(
        jointsTargetRotation[i].radians - jointsRotation[i].radians, M_PI * 2);
    if (dRotation < 0)
        dRotation += M_PI * 2;
    double dRotationNegative = M_PI * 2 - dRotation;

    if (dRotation < dRotationNegative) {
        jointsRotation[i] = jointsRotation[i] + std::min(jointRotationSpeed, dRotation);
    } else {
        jointsRotation[i] = jointsRotation[i] - std::min(jointRotationSpeed, dRotationNegative);
    }
}

void ManipulatorArm::resizeSegmentToTarget(
    int i) { // TODO stop after reacing target size
    double dLenght = segmentsTargetLength[i] - segmentsLength[i];
    if (dLenght > 0) {
        segmentsLength[i] += std::min(segmentResizeSpeed, abs(dLenght));
    } else {
        segmentsLength[i] -= std::min(segmentResizeSpeed, abs(dLenght));
    }
}

void ManipulatorArm::moveToTarget() { // TODO PID???
    for (int i = 0; i < jointsNumber; i++) {
        rotateJointToTarget(i);
    }

    for (int i = 0; i < jointsNumber - 1; i++) {
        resizeSegmentToTarget(i);
    }
}

void ManipulatorArm::release() {
    active = false;
    if (takenBox) {
        takenBox->setGrabbed(false);
        takenBox = nullptr;
    }

    relativeBoxPosition = Vector2d();
}

bool ManipulatorArm::isActive() {
    return active;
}

Vector2d ManipulatorArm::getLastJointPos() {
    return jointsPosition[jointsNumber - 1];
}

void ManipulatorArm::removeBox() {
    takenBox = nullptr;
}

void ManipulatorArm::removeBox(Box* aBox) {
    if (takenBox == aBox) {
        removeBox();
    }
}

void ManipulatorArm::takeBox(Box* aBox) {
    takenBox = aBox;
    relativeBoxPosition = takenBox->getRect().center() - jointsPosition[jointsNumber - 1];
    takenBox->setGrabbed(true);
}

void ManipulatorArm::recalculate() {
    Rotation lastJointTargetRotation{};
    jointsTargetPosition[0] = jointsPosition[0];
    for (int i = 1; i < jointsNumber; i++) {
        lastJointTargetRotation = lastJointTargetRotation + jointsTargetRotation[i - 1];
        jointsTargetPosition[i] = jointsTargetPosition[i - 1] + Vector2d(lastJointTargetRotation, segmentsTargetLength[i - 1]);
    }

    Rotation lastJointRotation{};
    for (int i = 1; i < jointsNumber; i++) {
        lastJointRotation = lastJointRotation + jointsRotation[i - 1];
        jointsPosition[i] = jointsPosition[i - 1] + Vector2d(lastJointRotation, segmentsLength[i - 1]);
    }

    if (takenBox) {
        takenBox->move(jointsPosition[jointsNumber - 1] + relativeBoxPosition);
    }
}

void ManipulatorArm::cleatTarget() {
    memcpy(jointsTargetRotation, jointsRotation, maxJointsNumber);
    memcpy(segmentsTargetLength, segmentsLength, maxJointsNumber);
}

void ManipulatorArm::setRootJointPosition(Vector2d position) {
    jointsPosition[0] = position;
}

void ManipulatorArm::rotateJoint(int aJoint, Rotation aRotation) {
    assert(("incorrect bone number", aJoint <= jointsNumber));
    jointsRotation[aJoint] = jointsRotation[aJoint] + aRotation;
}

void ManipulatorArm::setSegmentTargetLength(int aSegment, double aLenght) {
    assert(("incorrect bone number", aSegment <= jointsNumber));
    segmentsTargetLength[aSegment] = aLenght;
}

void ManipulatorArm::setJointTargetRotation(int aJoint, Rotation aRotation) {
    assert(("incorrect bone number", aJoint <= jointsNumber));
    jointsTargetRotation[aJoint] = aRotation;
}

void ManipulatorArm::setJointRotation(int aJoint, Rotation aRotation) {
    assert(("incorrect bone number", aJoint <= jointsNumber));
    jointsRotation[aJoint] = aRotation;
}

void ManipulatorArm::setSegmentLength(int aSegment, double aLenght) {
    assert(("incorrect bone number", aSegment <= jointsNumber));
    segmentsLength[aSegment] = aLenght;
}

Rotation ManipulatorArm::getJointRotation(int n) {
    assert(("incorrect bone number", n < jointsNumber));
    return jointsRotation[n];
}

Vector2d ManipulatorArm::getJointPosition(int n) {
    assert(("incorrect bone number", n < jointsNumber));
    return jointsPosition[n];
}

ManipulatorArm::ManipulatorArm(int aJointsNumber):
    jointsNumber(aJointsNumber) {
    assert(("incorrect number of joints", 1 <= aJointsNumber <= maxJointsNumber));
    defaultInit();
}

void ManipulatorArm::defaultInit() {
    for (int i = 0; i < jointsNumber; i++) {
        setSegmentLength(i, 10);
        setJointRotation(i, Rotation(0.1));
        setSegmentTargetLength(i, 10);
        setJointTargetRotation(i, Rotation(0.1));
    }
    recalculate();
}
