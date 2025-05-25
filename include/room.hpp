#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

#include <vector>
#include <raylib.h>
#include "actor.hpp"
#include "tilemap.hpp"

class Room {
private:
    TileMap *tileMap;
    std::vector<Actor*> actors;
public:
    Room(TileMap *tileMap);
    void unload();
    void update();
    void draw();
    void addActor(Actor *actor);
};

#endif
