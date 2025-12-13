#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

class GameObject {
    friend class GameWorld;
    inline static uint64_t next_id = 1;
    uint64_t id;

public:
    GameObject(uint64_t aId) {
        // assert(aId >= next_id); // TODO doesnt work like that then we are loading saves so it is removed for now
        id = aId;
        next_id = id + 1;
    }

    GameObject(): GameObject(next_id) {}

    uint64_t getId() {
        return id;
    }

    void setId(uint64_t aId) { // TODO could cause bugs if used somewhere outside of loading or loading invalid save
        id = aId;
        next_id = id + 1;
    }

    virtual void addToGameWorld() = 0; // TODO should be somewhat same for all types of objects
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_