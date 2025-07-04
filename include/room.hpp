#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

class Player;

#include <memory>
#include <vector>
#include <raylib.h>
#include "actor.hpp"
#include "interactablesContainer.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "interactable.hpp"

class Room {
private:
    std::unique_ptr<InteractablesContainer> interactables;
    std::unique_ptr<TileMap> tileMap;
    std::unique_ptr<std::vector<Actor>> actors;
    std::unique_ptr<Player> player;
public:
    Room();
    Room(std::string fileName);
    Room(std::unique_ptr<TileMap> tileMap);
    void unload();
    void update();
    void draw();
    void addActor(Actor actor);
    void addPlayer(std::unique_ptr<Player> player);
    Player& getPlayer();
    TileMap& getTileMap();
    std::vector<Vector2> getCollisionTiles();
    std::vector<Interactable> getInteractableTiles();
};

#endif
