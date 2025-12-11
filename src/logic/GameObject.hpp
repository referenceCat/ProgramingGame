#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

class GameObject {
    uint64_t id;

public:
    GameObject() {
        static uint64_t next_id = 1;
        id = next_id;
        next_id++;
    }

    uint64_t getId() {
        return id;
    }

    virtual void addToGameWorld() = 0; // TODO should be somewhat same for all types of objects
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_