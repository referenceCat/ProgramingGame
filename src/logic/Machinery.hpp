#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_

#include <assert.h>
#include <string>
#include <vector>
#include <bitset>
#include "common.hpp"
#include "Arm.hpp"
#include "Box.hpp"
#include "GameObject.hpp"

class Machinery : public GameObject {
    int address = 0;
    std::vector<uint32_t> memory;

protected:
    Rect2d rect;

    struct ProductionArea {
        Rect2d rect;
    };

    std::vector<ProductionArea*> areas;

    void destroyBox(Box* box);
    Box* createBox(ProductionArea area);
    std::vector<Box*> getBoxesInside(ProductionArea area);
    std::vector<Box*> getBoxesTouching(ProductionArea area);
    void setMemoryValue(size_t address, uint32_t value);
    uint32_t getMemoryValue(size_t address);
public:
    Machinery(Rect2d rect, size_t memorySize = 0):
        rect(rect) {
        memory = std::vector<uint32_t>(memorySize, 0);
    };

    virtual void drawInfo() {};
    virtual void drawDebug();
    virtual void draw() {};
    Rect2d getRect();
    virtual void run() {};
    virtual void onMemoryWrite(size_t address, uint32_t value);
    uint32_t onMemoryRead(size_t address);
    virtual void onClick() {};
    void addToGameWorld() override;
    void setCenter(Vector2d pos);
    virtual void setAddress(int id);
    int getAddress();
    size_t getMemorySize();
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_