#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_

enum ModuleType
{
    Corridor,
    JunctionX,
    Junction3,
    DeadendModule
};

class Builder {
    Module *newModule = nullptr;

    Window *window;
    std::vector<Label *> nodeNumberLabels;

    void onWindowClose()
    {
        window = nullptr;
        nodeNumberButtons.clear();
        nodeNumberLabels.clear();
    }

public:
    static ModuleBuilder *instance()
    {
        static ModuleBuilder instance;
        return &instance;
    }

    void setParentNode(ModuleNode *node)
    {
        parentModuleNode = node;
    }

    // true - if success
    bool buildModule();

    bool selectModuleType(ModuleType);

    bool selectNewNodeNumber(int number);

    void createWindow()
    {
        if (window)
            GuiEngine::instance()->closeWindow(window);
        if (parentModuleNode->attachedNode != nullptr)
            return;
        window = GuiEngine::instance()->addWindow(Rect2d(Vector2d(400, 400), 400, 440), true, true);
        window->setOnCloseCallback([this]()
                                   { onWindowClose(); });

        Button *createButton = window->addButton(Rect2d(Vector2d(10, 340), Vector2d(210, 390)));
        window->addLabel(createButton->getRect().center(), true, "Create module", 0);
        createButton->setOnClickCallback([this, &window = window]()
                                         {
            bool result = this->buildModule();
            if (result) GuiEngine::instance()->closeWindow(window); });

        window->addLabel(Vector2d(20, 40), false, "Module type:", 0);
        window->addLabel(Vector2d(220, 40), false, "Node number:", 0);

        Button *corridorButton = window->addButton(Rect2d(Vector2d(20, 60), Vector2d(200, 80)));
        window->addLabel(corridorButton->getRect().center(), true, "Corridor", 0);
        corridorButton->setOnClickCallback([this](){selectModuleType(Corridor); });

        Button *xCorridorButton = window->addButton(Rect2d(Vector2d(20, 85), Vector2d(200, 105)));
        window->addLabel(xCorridorButton->getRect().center(), true, "X Corridor", 0);
        xCorridorButton->setOnClickCallback([this](){selectModuleType(JunctionX); });

        Button *junction3Button = window->addButton(Rect2d(Vector2d(20, 110), Vector2d(200, 130)));
        window->addLabel(junction3Button->getRect().center(), true, "3 way junction", 0);
        junction3Button->setOnClickCallback([this](){selectModuleType(Junction3); });

        Button *deadendButton = window->addButton(Rect2d(Vector2d(20, 135), Vector2d(200, 155)));
        window->addLabel(deadendButton->getRect().center(), true, "Deadend", 0);
        deadendButton->setOnClickCallback([this](){selectModuleType(DeadendModule); });

        selectModuleType(Corridor);
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_BUILDER_HPP_