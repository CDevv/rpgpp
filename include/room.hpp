#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

#include <raylib.h>
#include "tilemap.hpp"

class Room {
private:
    TileMap *tileMap;
public:
    Room(TileMap *tileMap);
    ~Room();
    void draw();
};

#endif
