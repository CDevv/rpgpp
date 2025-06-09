#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

#include <vector>
#include <raylib.h>
#include "actor.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "interactable.hpp"

class Player;

class Room {
private:
    TileMap *tileMap;
    std::vector<Actor*> actors;
    Player *player;
public:
    Room();
    Room(std::string fileName);
    Room(TileMap *tileMap);
    void unload();
    void update();
    void draw();
    void addActor(Actor *actor);
    void addPlayer(Player *player);
    Player *getPlayer();
    TileMap *getTileMap();
    std::vector<Vector2> getCollisionTiles();
    std::vector<Interactable*> getInteractableTiles();
};

#endif
