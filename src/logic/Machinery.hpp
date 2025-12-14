#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_

#include <assert.h>
#include <string>
#include <vector>
#include <bitset>
#include "types.hpp"
#include "Arm.hpp"
#include "Box.hpp"
#include "GameObject.hpp"
#include "GuiEngine.hpp"

typedef int32_t MemoryWord;

struct Port {
    // id? or just numbers?
    Rect2d rect;
    // flags
};

class Machinery : public GameObject {
    int address = 0;
    std::vector<MemoryWord> memory;
    std::vector<Port> ports;

protected:
    Rect2d rect;
    void destroyBox(Box* box);
    Box* createBox(int portIndex);
    Port& getPort(int index);
    std::vector<Box*> getBoxesInside(int portIndex);
    std::vector<Box*> getBoxesTouching(int portIndex);
    void setMemoryValue(size_t address, MemoryWord value);
    MemoryWord getMemoryValue(size_t address);

public:
    Machinery(Rect2d rect, size_t memorySize = 0):
        rect(rect) {
        memory = std::vector<MemoryWord>(memorySize, 0);
    };

    int addPort(Rect2d rect);
    Rect2d getRect();
    MemoryWord onMemoryRead(size_t address);
    void addToGameWorld() override;
    void setCenter(Vector2d pos);
    int getAddress();
    size_t getMemorySize();

    virtual void drawInfo() {};
    virtual void drawDebug();
    virtual void draw() {};
    virtual void setAddress(int id);
    virtual void onClick() {};
    virtual void run() {};
    virtual void onMemoryWrite(size_t address, MemoryWord value);
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

struct Recipe {
    DataId id;
    std::string name;
    unsigned int duration = 0;
    std::vector<DataId> machineryIds;
    std::vector<std::pair<DataId, int>> inputs; // box id, port number
    std::vector<std::pair<DataId, int>> outputs; // box id, port number
};

class AbstractAssembler : public Machinery {
    AbstractDrawable* drawable = nullptr;
    int tick = 0;
    DataId dataId = 0;
    Vector2d progressBarPos = {};

    Window* window = nullptr;
    Label* addressLabel = nullptr;
    Console* memoryConsole = nullptr;


public:
    AbstractAssembler(Rect2d rect): Machinery(rect, 10) {};
        // TODO make virtual and add to machinery as well?
    static AbstractAssembler* initializeFromJson(nlohmann::json);

    void setDrawable(AbstractDrawable* aDrawable);
    void drawInfo() override {};
    void drawDebug() override;
    void draw() override;

    void onClick() override;
    void createWindow();
    void updataWindow();

    void run() override;

    // void updateMemory() {
    //     MemoryWord word0 = 0; // TODO should be uniformal across all machinery (e.g. 1 bit is working, 2 is power, 3 ...)
    //     word0 |= (manualMode << 0);
    //     word0 |= (arm->isActive() << 1);
    //     word0 |= ((arm->getTakenBox() != nullptr) << 2);
    //     word0 |= (((arm->getJointTargetPosition(2) - arm->getJointPosition(2)).lenght() < 0.01) << 3); // arm reached its target
    //     setMemoryValue(0, word0);

    //     if (manualMode)
    //         return;
    //     MemoryWord word1 = getMemoryValue(1);
    //     if (word1)
    //         arm->grab();
    //     else
    //         arm->release();

    //     setRelativeTarget(Vector2d(getMemoryValue(2), getMemoryValue(3)));
    // }

};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_MACHINERY_HPP_