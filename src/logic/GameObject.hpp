#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

class GameObject {
    friend class GameWorld;
    inline static uint64_t next_id = 1;
    uint64_t id;

public:
    GameObject(uint64_t aId) {
        assert(aId >= next_id);
        id = aId;
        next_id = id + 1;
    }

    GameObject(): GameObject(next_id) {}

    uint64_t getId() {
        return id;
    }

    virtual void addToGameWorld() {}; // TODO should be somewhat same for all types of objects
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_