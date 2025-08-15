#ifndef __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_
#define __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_

class GameWorld;

#include "../graphics/GraphicsEngine.hpp"
#include "allegro5/allegro_font.h"

class GameObject {
private:
    inline static int lastId = 0;
    const int id;

protected:
    GameWorld* parentWorld;

public:
    inline static ALLEGRO_FONT* debug_font = nullptr; // TODO wrong way to do it
    GameObject(GameWorld* aWorld):id(++lastId), parentWorld(aWorld) {};
    int getId() {
        return id;
    }
};

#endif // __PROJECTS_PROGRAMINGGAME_SRC_LOGIC_GAMEOBJECT_HPP_