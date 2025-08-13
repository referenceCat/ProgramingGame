#include "GuiEngine.hpp"

GuiEngine* GuiEngine::instance = nullptr;

GuiEngine* GuiEngine::getInstance() {
    if (!instance) {
        instance = new GuiEngine();
    }
    return instance;
}
