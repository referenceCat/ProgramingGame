#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINES_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINES_HPP_

class Furnace: public Machine {
    ProductionArea heatingArea;
public:
    Furnace(Point2d aPos, GameWorld* aWorld): Machine(Rect2d(aPos, 100, 100), aWorld) {
        heatingArea.rect = Rect2d(Point2d(10, 10), Point2d(rect.dimensions().x - 10, rect.dimensions().y - 10));
        areas.push_back(&heatingArea);
    }

    void run() override {
        for (auto item: getBoxesInside(heatingArea))  {
            item->grill();
        }
    }
};

class BoxGenerator: public Machine {
    ProductionArea creatingArea;
    int cooldown = 200;

public:
    BoxGenerator(Point2d aPos, GameWorld* aWorld): Machine(Rect2d(aPos, 50, 50), aWorld) {
        creatingArea.rect = Rect2d(Point2d(10, 10), Point2d(rect.dimensions().x - 10, rect.dimensions().y - 10));
        areas.push_back(&creatingArea);
    }

    void run() override {
        if (cooldown > 0) cooldown--;

        if (cooldown == 0 && getBoxesTouching(creatingArea).size() == 0) {
            createBox();
            cooldown = 200;
        }
    }
};

class BoxDestroyer: public Machine {
    ProductionArea destroyingArea;
    int cooldown = 200;

public:
    BoxDestroyer(Point2d aPos, GameWorld* aWorld): Machine(Rect2d(aPos, 50, 50), aWorld) {
        destroyingArea.rect = Rect2d(Point2d(10, 10), Point2d(rect.dimensions().x - 10, rect.dimensions().y - 10));
        areas.push_back(&destroyingArea);
    }

    void run() override {
        for (auto item: getBoxesInside(destroyingArea))  {
            destroyBox(item);
        }
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINES_HPP_