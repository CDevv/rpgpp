#include "room.hpp"
#include "tilemap.hpp"

Room::Room(TileMap *tileMap) {
    this->tileMap = tileMap;
}

void Room::unload() {
    tileMap->unload();
    for (Actor *actor : actors) {
        actor->unload();
    }
    player->unload();
}

void Room::update() {
    for (Actor *actor : actors) {
        actor->update();
    }
    player->update();
}

void Room::draw() {
    this->tileMap->draw();
    for (Actor *actor : actors) {
        actor->draw();
    }
    player->draw();
}

void Room::addActor(Actor *actor) {
    this->actors.push_back(actor);
}

void Room::addPlayer(Player* player)
{
    this->player = player;
}

