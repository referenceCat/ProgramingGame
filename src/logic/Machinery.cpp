#include "Machinery.hpp"
#include "GameWorld.hpp"
#include "GraphicsEngine.hpp"

void Machinery::destroyBox(Box* box) {
    GameWorld::instance()->removeBox(box->getId());
}

Box* Machinery::createBox(int portIndex) {
    auto box = new Box(Rect2d::fromCenterAndDimensions(getPort(portIndex).rect.center() + rect.p1, Vector2d(2, 2)));
    box->addToGameWorld();
    return box;
}

Port& Machinery::getPort(int index) {
    return ports.at(index);
}

std::vector<Box*> Machinery::getBoxesInside(int portIndex) {
    std::vector<Box*> result;
    Rect2d portGlobalRect = getPort(portIndex).rect;
    portGlobalRect.p1.x += rect.p1.x;
    portGlobalRect.p1.y += rect.p1.y;
    portGlobalRect.p2.x += rect.p1.x;
    portGlobalRect.p2.y += rect.p1.y;
    for (auto item : GameWorld::instance()->getBoxes()) {
        if (portGlobalRect.isInside(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

std::vector<Box*> Machinery::getBoxesTouching(int portIndex) {
    std::vector<Box*> result;
    Rect2d portGlobalRect = getPort(portIndex).rect;
    portGlobalRect.p1.x += rect.p1.x;
    portGlobalRect.p1.y += rect.p1.y;
    portGlobalRect.p2.x += rect.p1.x;
    portGlobalRect.p2.y += rect.p1.y;
    for (auto item : GameWorld::instance()->getBoxes()) {
        if (portGlobalRect.isIntersecting(item->getRect())) {
            result.push_back(item);
        }
    }
    return result;
}

void Machinery::setMemoryValue(size_t address, MemoryWord value) {
    memory.at(address) = value;
}

MemoryWord Machinery::getMemoryValue(size_t address) {
    return memory.at(address);
}

void Machinery::addToGameWorld() {
    GameWorld::instance()->addMachinery(this);
}

void Machinery::drawDebug() {
    GraphicsEngine::instance()->drawRectangle(rect, CommonValues::zDebug,
        al_map_rgb(100, 255, 100), 1);

    for (auto item : ports) {
        Rect2d areaRect;
        areaRect.p1 = item.rect.p1 + rect.p1;
        areaRect.p2 = item.rect.p2 + rect.p1;
        GraphicsEngine::instance()->drawRectangle(areaRect, CommonValues::zDebug,
            al_map_rgb(100, 100, 255), 1);
    }
}

int Machinery::addPort(Rect2d rect) {
    ports.emplace_back(rect);
    return ports.size() - 1;
}

Rect2d Machinery::getRect() {
    return rect;
}

void Machinery::onMemoryWrite(size_t address, MemoryWord value) {
    if (address >= memory.size())
        return;
    memory.at(address) = value;
}

MemoryWord Machinery::onMemoryRead(size_t address) {
    if (address >= memory.size())
        return 0;
    return memory.at(address);
}

void Machinery::setCenter(Vector2d pos) {
    rect = Rect2d::fromCenterAndDimensions(pos, rect.dimensions());
}

void Machinery::setAddress(int id) {
    address = id;
}

int Machinery::getAddress() {
    return address;
}

size_t Machinery::getMemorySize() {
    return memory.size();
}

void AbstractAssembler::drawDebug() {
    Machinery::drawDebug();
}

void AbstractAssembler::draw() {
    Machinery::draw();

    if (drawable) {
        drawable->setPos(getRect().center());
        drawable->draw();

        GraphicsEngine::instance()->drawArcProgressBar(rect.p1 + progressBarPos, 0.7, 0.7, CommonValues::zMachinery, al_map_rgb(150, 150, 150), 0.2);
    }
}

void AbstractAssembler::onClick() {
    createWindow();
}

void AbstractAssembler::createWindow() {
    if (window)
        return;
    window = new Window(GuiEngine::instance()->getDisplayArea(), AligmentBuilder().dimensions({720, 300}).margin(-1, -1, -1, 30), true);
    window->setOnCloseCallback([this]() { window = nullptr; });
    window->setDrawPriority(2);

    auto optionsArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(1, 0).margin(10, 20, 10, 20), "Settings");
    auto memoryArea = new NamedArea(window->getInternalArea(), AligmentBuilder().tableDimensions(3, 1).tableCell(2, 0).margin(10, 20, 20, 20), "Memory");

    auto addressButtonAligment = Aligment::byMargin(5, 5, 5, 5);
    addressButtonAligment.tableRows = 5;
    addressButtonAligment.ownRow = 0;
    auto addressButton = new Button(optionsArea->getInternalArea(), addressButtonAligment);
    addressButton->setMouseCallback(Release, [this](auto pos) { new AddressSelectionWindow(getAddress(), [this](int address) { this->setAddress(address); }); });
    addressLabel = new Label(addressButton, Aligment(), std::format("Addr: {}", getAddress()));

    memoryConsole = new Console(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 1).tableCell(1, 0).margin(5, 5, 5, 5));
    memoryConsole->setEditable(false);
    new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 1).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "status->"), -1)), "status->");
    new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 2).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "progress->"), -1)), "progress->");
    new Label(memoryArea->getInternalArea(), AligmentBuilder().tableDimensions(2, 14).tableCell(0, 3).margin(-1, -1, 5, 10).dimensions(Vector2d(al_get_text_width(GuiEngine::instance()->debugFont, "recipe id->"), -1)), "recipe id->");
}

void AbstractAssembler::run() {
    Machinery::run();
    tick++;
}

AbstractAssembler* AbstractAssembler::initializeFromJson(nlohmann::json data) {
    AbstractAssembler* result = new AbstractAssembler(Rect2d::fromCenterAndDimensions({}, Vector2d::fromJson(data["dimensions"])));
    result->dataId = data["id"].get<int>();
    auto drawable = SpriteCollectionDrawable::fromJson(data["drawable"]);
    result->setDrawable(drawable);
    for (auto portData : data["ports"]) {
        result->addPort(Rect2d::fromJson(portData["rect"]));
    }
    result->progressBarPos = Vector2d::fromJson(data["progressBar"]["pos"]); // TODO it must be possible to disable progress bar
    return result;
}

void AbstractAssembler::setDrawable(AbstractDrawable* aDrawable) {
    drawable = aDrawable;
}

// void AbstractAssembler::setReciept(Reciept reciept) {
//     // TODO validate reciept first
// }
