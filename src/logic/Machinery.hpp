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
#include "GuiEngine.hpp"

typedef int32_t MemoryWord;
class Machinery : public GameObject {
    int address = 0;
    std::vector<MemoryWord> memory;

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
    void setMemoryValue(size_t address, MemoryWord value);
    MemoryWord getMemoryValue(size_t address);
public:
    Machinery(Rect2d rect, size_t memorySize = 0):
        rect(rect) {
        memory = std::vector<MemoryWord>(memorySize, 0);
    };

    virtual void drawInfo() {};
    virtual void drawDebug();
    virtual void draw() {};
    Rect2d getRect();
    virtual void run() {};
    virtual void onMemoryWrite(size_t address, MemoryWord value);
    MemoryWord onMemoryRead(size_t address);
    virtual void onClick() {};
    void addToGameWorld() override;
    void setCenter(Vector2d pos);
    virtual void setAddress(int id);
    int getAddress();
    size_t getMemorySize();
};

class AddressSelectionWindow {
    Window* window = nullptr;
    Button* addressButtons[16][16] = {nullptr};
    Label* addressLabels[16][16] = {nullptr};
    int address = 0;
    std::function<void(int)> onAddressButtonClicked = nullptr;

public:
    AddressSelectionWindow(int selectedAddress, std::function<void(int)> onAddressButtonClicked):
        onAddressButtonClicked{onAddressButtonClicked} {
        window = new Window(GuiEngine::instance()->getDisplayArea(), Aligment::byDimensionsAndCentered(Vector2d(640, 640)), true);
        window->setDrawPriority(2);
        for (int i = 0; i < 256; i++) {
            addressButtons[i / 16][i % 16] = new Button(window->getInternalArea(), AligmentBuilder().margin(3, 3, 3, 3).tableDimensions(16, 16).tableCell(i % 16, i / 16));
            addressButtons[i / 16][i % 16]->setMouseCallback(Release, [this, i](auto pos) { this->onAddressButtonClick(i); });
            addressLabels[i / 16][i % 16] = new Label(addressButtons[i / 16][i % 16], Aligment(), std::to_string(i));
        }
        setSelectedAddress(selectedAddress);
    };

    ~AddressSelectionWindow() {
        delete window;
    }

    void setSelectedAddress(int address) {
        assert(address >= 0 && address <= 255);
        this->address = address;
        for (int i = 0; i < 256; i++) {
            addressLabels[i / 16][i % 16]->setText(std::to_string(i));
        }
        addressLabels[address / 16][address % 16]->setText(std::format("({})", address));
    };

    void onAddressButtonClick(int address) {
        setSelectedAddress(address);
        onAddressButtonClicked(address);
        // delete this;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_