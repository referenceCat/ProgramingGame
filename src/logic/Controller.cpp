#include "Controller.hpp"
#include "GameWorld.hpp"

int Controller::execNextInstr() {
    if (instrPointer >= instructions.size()) {
            return 1; // programm ended
        }

        if (rDelay) {
            rDelay--;

            if (rDelay == 0) {
                instrPointer++;
            }

            return 0;
        }

        std::vector<std::string> instr = split(instructions.at(instrPointer), " ");

        if (instr.size() < 1) {
            return 1;
        }

        std::string command = instr.at(0);

        if (command == "delay") {
            rDelay = std::atoi(instr.at(1).c_str());
            return 0;
        }

        if (command == "angle") {
            int id = std::atoi(instr.at(1).c_str()); 
            ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
            if (!arm) return 1;
            int joint = std::atoi(instr.at(2).c_str()); 
            int angle = std::atoi(instr.at(3).c_str()); 
            arm->setJointTargetRotation(joint, degreesToRadians(angle));
            instrPointer++;
            return 0;
        }

        if (command == "grab") {
            int id = std::atoi(instr.at(1).c_str()); 
            ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
            if (!arm) return 1;
            arm->grab();
            instrPointer++;
            return 0;
        }

        if (command == "release") {
            int id = std::atoi(instr.at(1).c_str()); 
            ManipulatorArm* arm = parentWorld->getManipulatorArm(id);
            if (!arm) return 1;
            arm->release();
            instrPointer++;
            return 0;
        }

        if (command == "goto") {
            instrPointer = std::atoi(instr.at(1).c_str()); 
            return 0;
        }

        return 0;
}