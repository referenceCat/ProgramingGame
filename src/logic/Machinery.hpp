#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_

#include <assert.h>
#include <string>
#include <vector>
#include "common.hpp"
#include "Arm.hpp"
#include "Box.hpp"
#include "GameObject.hpp"

class Machinery : public GameObject {
protected:
    Rect2d rect;

    struct ProductionArea {
        Rect2d rect;
    };

    std::vector<ProductionArea*>  areas;

    void destroyBox(Box* box);
    Box* createBox(ProductionArea area);
    std::vector<Box*> getBoxesInside(ProductionArea area);
    std::vector<Box*> getBoxesTouching(ProductionArea area);\

    int address = 0;

public:
    Machinery(Rect2d rect): rect(rect) {};
    virtual void drawInfo() {};
    virtual void drawDebug();
    virtual void draw() {};
    Rect2d getRect();
    virtual void run() {};
    virtual void onCommandRecive(int command, int argument) {}
    virtual void onClick() {};
    void addToGameWorld() override;
    void setCenter(Vector2d pos);
    void setAddress(int id);
    int getAddress();
};

// class AddressSelectionWindow {
//     LegacyWindow* window;
// };

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_