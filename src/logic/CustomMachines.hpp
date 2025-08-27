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
            if(item->getTemperature() < 255) item->setTemperature(item->getTemperature() + 1);
        }
    }

    void draw() override {
        GraphicsEngine::instance()->drawBitmap(rect.p1,  GraphicsEngine::instance()->furnaceSprite, 5);
        // Machine::draw();
    }
};

class BoxGenerator: public Machine {
    ProductionArea creatingArea;
    int period = 200;
    int cooldown = period;
    BoxContent boxContent = BoxContent::EmptyBox;
    

public:
    BoxGenerator(Point2d aPos, GameWorld* aWorld): Machine(Rect2d(aPos, 50, 50), aWorld) {
        creatingArea.rect = Rect2d(Point2d(10, 10), Point2d(rect.dimensions().x - 10, rect.dimensions().y - 10));
        areas.push_back(&creatingArea);
    }

    void setPeriod(int aPeriod) {
        period = aPeriod;
    }

    void setType(BoxContent aContent) {
        boxContent = aContent;
    }

    void run() override {
        if (cooldown > 0) cooldown--;

        if (cooldown == 0 && getBoxesTouching(creatingArea).size() == 0) {
            createBox(creatingArea)->setContent(boxContent);
            cooldown = period;
        }
    }

    void draw() override {
        Machine::draw();
        GraphicsEngine::instance()->drawLine(Point2d(rect.p1.x, rect.p2.y + 10), Point2d(rect.p1.x + (period - cooldown) / 4, rect.p2.y + 10), 0, al_map_rgb(100, 100, 100), 2);
        GraphicsEngine::instance()->drawLine(Point2d(rect.p1.x, rect.p2.y + 10), Point2d(rect.p1.x + (period - cooldown) / 4, rect.p2.y + 10), -5, al_map_rgb(255, 255, 255), 2);
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

class PlateCombiner: public Machine {
    ProductionArea ingridientArea0;
    ProductionArea ingridientArea1;
    ProductionArea resultArea;
    int period = 160;
    int processTime = 0;
    bool processRunning = false;

    int pressShiftByProcessTime() {
        if (processTime < 20 || processTime > 140) return 0;
        if (processTime > 60 && processTime < 100) return 20;
        if (processTime < 80) return (processTime - 20) / 2;
        return (140 - processTime) / 2;
    }
    

public:
    PlateCombiner(Point2d aPos, GameWorld* aWorld): Machine(Rect2d(aPos, 75, 160), aWorld) {
        ingridientArea0.rect = Rect2d(Point2d(5, 5), Point2d(35, 35)); // TODO process class for this kind of behavior
        ingridientArea1.rect = Rect2d(Point2d(5, 40), Point2d(35, 70));
        resultArea.rect = Rect2d(Point2d(125, 5), Point2d(155, 35));
        areas.push_back(&ingridientArea0);
        areas.push_back(&ingridientArea1);
        areas.push_back(&resultArea);
    }

    void setPeriod(int aPeriod) {
        period = aPeriod;
    }

    void run() override {
        if (!processRunning && getBoxesInside(ingridientArea0).size() && getBoxesInside(ingridientArea1).size()) {
            auto box0 = getBoxesInside(ingridientArea0).at(0);
            auto box1 = getBoxesInside(ingridientArea1).at(0);
            if (box0->getContent() == BoxContent::IronPlate 
                && box1->getContent() == BoxContent::IronPlate
                && box0->getTemperature() > 100
                && box1->getTemperature() > 100) {
                destroyBox(box0); // process begins here
                destroyBox(box1);
                processRunning = true;
            }
        }

        if (processRunning && processTime < period) processTime++;

        if (processRunning && processTime >= period) {
            if (getBoxesTouching(resultArea).size() == 0) { // check if can create box
                auto resultBox = createBox(resultArea); // process stops here
                resultBox->setContent(BoxContent::HeavyIronPlate);
                processRunning = false;
                processTime = 0;
            }
        }
    }

    void draw() override {
        // Machine::draw();
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerBaseSprite, 2, Point2d(165, 165));
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerCyllindersSprite, 10, Point2d(165, 165));
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerPressSprite, 2, Point2d(165, 165 - pressShiftByProcessTime() + 4));
        GraphicsEngine::instance()->drawBitmap(rect.p2,  GraphicsEngine::instance()->assemblerPlateSprite, 5, Point2d(165, 165));
        GraphicsEngine::instance()->drawLine(Point2d(rect.p1.x, rect.p2.y + 10), Point2d(rect.p1.x + processTime, rect.p2.y + 10), 0, al_map_rgb(100, 100, 100), 2);
        GraphicsEngine::instance()->drawLine(Point2d(rect.p1.x, rect.p2.y + 10), Point2d(rect.p1.x + processTime, rect.p2.y + 10), -5, al_map_rgb(255, 255, 255), 2);
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_CUSTOMMACHINES_HPP_