#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

class GameObject {
    friend class GameWorld;
    inline static ObjectId next_id = 1;
    ObjectId id;

public:
    GameObject(ObjectId aId) {
        // assert(aId >= next_id); // TODO doesnt work like that then we are loading saves so it is removed for now
        id = aId;
        next_id = id + 1;
    }

    GameObject(): GameObject(next_id) {}

    ObjectId getId() {
        return id;
    }

    void setId(ObjectId aId) { // TODO could cause bugs if used somewhere outside of loading or loading invalid save
        id = aId;
        next_id = id + 1;
    }

    virtual void addToGameWorld() = 0; // TODO should be somewhat same for all types of objects
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_