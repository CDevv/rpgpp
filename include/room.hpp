#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

#include <vector>
#include <raylib.h>
#include "actor.hpp"
#include "player.hpp"
#include "tilemap.hpp"

class Player;

class Room {
private:
    TileMap *tileMap;
    std::vector<Actor*> actors;
    Player *player;
public:
    Room(TileMap *tileMap);
    void unload();
    void update();
    void draw();
    void addActor(Actor *actor);
    void addPlayer(Player *player);
    TileMap *getTileMap();
    std::vector<Vector2> getCollisionTiles();
    std::vector<Vector2> getInteractableTiles();
};

#endif
