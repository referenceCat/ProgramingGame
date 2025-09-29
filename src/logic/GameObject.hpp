#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

class GameObject {
private:
    inline static int lastId = 0;
    const int id;

public:
    GameObject():
        id(++lastId) {};

    int getId() {
        return id;
    }

    virtual void addToGameWorld() = 0;
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_