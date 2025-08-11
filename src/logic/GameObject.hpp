#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

class GameObject {
private:
    inline static int lastId = 0;
    const int id;

protected:
    GameWorld* parentWorld;

public:
    GameObject(GameWorld* aWorld):id(++lastId), parentWorld(aWorld) {};
    int getId() {
        return id;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_